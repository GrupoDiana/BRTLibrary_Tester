/**
*
* \brief This is the header file of the example project 1 using BRT Library
* \date	June 2023
*
* \authors 3DI-DIANA Research Group (University of Malaga), in alphabetical order: M. Cuevas-Rodriguez, D. Gonzalez-Toledo, L. Molina-Tanco, F. Morales-Benitez ||
* Coordinated by , A. Reyes-Lecuona (University of Malaga)||
* \b Contact: areyes@uma.es
*
* \b Contributions: (additional authors/contributors can be added here)
*
* \b Project: SONICOM ||
* \b Website: https://www.sonicom.eu/
*
* \b Copyright: University of Malaga 2023. Code based in the 3DTI Toolkit library (https://github.com/3DTune-In/3dti_AudioToolkit) with Copyright University of Malaga and Imperial College London - 2018
*
* \b Licence: This program is free software, you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* \b Acknowledgement: This project has received funding from the European Union�s Horizon 2020 research and innovation programme under grant agreement no.101017743
*/

#ifndef _BASICSPATIALISATIONRTAUDIO_H_
#define _BASICSPATIALISATIONRTAUDIO_H_

#define SAMPLERATE 48000
//#define SOFA1_FILEPATH "../../resources/hrtf.sofa"
//#define SOFA2_FILEPATH "../../resources/0_IRC_1008_R_HRIR.sofa"
#define SOFA3_FILEPATH "../../resources/ListenResamp15.sofa"
#define SOFA4_FILEPATH "../../resources/SOFATransparentFront.sofa"
#define SOURCE1_FILEPATH "../../resources/WhiteNoise_16bits_48000.wav"
//#define SOURCE2_FILEPATH "../../resources/speech.wav"
#define HRTFRESAMPLINGSTEP 15
#define ILD_NearFieldEffect_44100 "../../resources/NearFieldCompensation_ILD_44100.sofa"
#define ILD_NearFieldEffect_48000 "../../resources/NearFieldCompensation_ILD_48000.sofa"
#define ILD_NearFieldEffect_96000 "../../resources/NearFieldCompensation_ILD_96000.sofa"
#define EXTRAPOLATION_METHOD "NearestPoint"

#define SOURCE1_INITIAL_AZIMUTH     0
#define SOURCE1_INITIAL_ELEVATION   0
#define SOURCE1_INITIAL_DISTANCE    2
#define SOURCE1_INITIAL_SPEED       0.1


#include <cstdio>
#include <cstring>
#include <RtAudio.h>
#include <BRTLibrary.h>
#include "ServiceModules/HRTFTester.hpp"

std::shared_ptr<RtAudio>						audio;												 // Pointer to RtAudio API

Common::CGlobalParameters globalParameters;                                                     // Class where the global BRT parameters are defined.
BRTBase::CBRTManager brtManager;                                                                // BRT global manager interface
std::shared_ptr<BRTListenerModel::CListenerHRTFbasedModel> listener;                            // Pointer to listener model
std::shared_ptr<BRTSourceModel::CSourceSimpleModel> source1BRT;                               // Pointers to each audio source model
//std::shared_ptr<BRTSourceModel::CSourceSimpleModel> sourceSteps;                                // Pointers to each audio source model

BRTReaders::CSOFAReader sofaReader;                                                             // SOFA reader provide by BRT Library
BRTServices::CHRTFTester hrtfTester;

std::vector<std::shared_ptr<BRTServices::CHRTF>> HRTF_list;                                     // List of HRTFs sofa loaded
std::vector<std::shared_ptr<BRTServices::CILD>> ILD_list;                                       // List of NearField coeffients loaded

//Common::CTransform						sourcePosition;										 // Storages the position of the steps source
float source1Azimuth;
float source1Elevation;
float source1Distance;

Common::CEarPair<CMonoBuffer<float>>	outputBufferStereo;									 // Stereo buffer containing processed audio
std::vector<float>						samplesVectorSource1;			                     // Storages the audio from the wav files
//std::vector<float>						samplesVectorSteps;			                        // Storages the audio from the wav files

unsigned int							wavSamplePositionSource1, positionEndFrameSpeech,	 // Storages, respectively, the starting and ending position of the frame being rendered for each source
                                        wavSamplePositionSteps,  positionEndFrameSteps ;

unsigned int                            loopCounter = 0;



/** \brief This method gathers all audio processing (spatialization and reverberation)
*	\param [out] bufferOutput output buffer processed
*	\param [in] bufferSize size of buffer in samples
*/
void audioProcess(Common::CEarPair<CMonoBuffer<float>>& bufferOutput, int bufferSize);

/** \brief This method shows the user a very simple menu that allows him to choose the audio interface to be used.
*	\param [out] int AudioDeviceID
*/
int SelectAudioDevice();

void AudioSetup();

void SourceSetup();

void ListenerSetup();

void AudioSetupAndStart();

void LoadHRTF();

void ResetOrientationSource();

/** \brief Fills a buffer with the correct audio frame from the input float vector
*	\param [out] output output buffer
*	\param [in,out] position starting position of the frame to be processed
*	\param [in,out] endChunk ending position of the frame to be processed
*	\param [in] samplesVector float vector containing the whole audio
*/
void FillBuffer(CMonoBuffer<float>& output, unsigned int & position, unsigned int & endChunk, std::vector<float>& samplesVector);

/** \brief Loads a mono, 16-bit, 44.1kHz ".wav" file
*	\param [out] samplesVector float vector that will storage the whole audio
*	\param [in] stringIn name of the ".wav" file to open
*/
void LoadWav(std::vector<float>& samplesVector, const char* stringIn);

/** \brief This function is called each time RtAudio needs a buffer to output
*	\param [out] outputBuffer output buffer to be filled
*	\param [out] inputBuffer unused input buffer
*	\param [in] bufferSize size of buffer in samples
*	\param [in] streamTime time in seconds since the stream started
*	\param [in] status feedback that if does not equal zero indicates stream over/underflow
*	\param [in] data unused data pointer
*	\retval Integer value that equals 0 if we don't want the stream to stop
*/
static int rtAudioCallback(void *outputBuffer, void *inputBuffer, unsigned int bufferSize, double streamTime, RtAudioStreamStatus status, void *data);

/**
 * @brief Loads a SOFA File with certain resampling Step
 * @param _filePath 
 * @return 
*/
bool LoadSofaFile(std::string _filePath);

/**
 * @brief Loads ILD from SOFA
 * @param _ildFilePath 
 * @return 
*/
bool LoadILD(std::string _ildFilePath);

void MoveSource();

/**
 * @brief Move the source in the transverse plane, with a circular trajectory
 * @param loopCounter 
*/
void MoveSource_CircularPathTransversePlane(unsigned int& loopCounter);

/**
 * @brief Move the source in the Sagittal plane, with a circular trajectory
 * @param loopCounter 
*/
void MoveSource_CircularPathSagittalPlane(unsigned int& loopCounter);

/**
 * @brief Convert spherical coordinates to Cartesians
 * @param azimuth 
 * @param elevation 
 * @param radius 
 * @return 
*/
Common::CVector3 Spherical2Cartesians(float azimuth, float elevation, float radius);

double d2r(double d);

int MenuTest();

/**
 * @brief Method that tests of the creation of the grid
 * @param _filePath 
*/
void TestGridCreationMain(std::string _filePath);

/**
 * @brief Method that tests the (not) interpolation of a SOFA already interpolated
 * @param _filePath 
*/
void TestGridInterpolationOffline_SOFAInterpolated(std::string _filePath);

int TestOfflineInterpolation();

int TestOnlineInterpolation();

void ChangeResamplingStep();


#endif
