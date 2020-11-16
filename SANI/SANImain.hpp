/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: SANImain.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1o3b 16-November-2020
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Main
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_MAIN
#define HEADER_SANI_MAIN



#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif
#include "SANIposRelTranslatorVariables.hpp"
#include "XMLrulesClass.hpp"

#include "LRPpreprocessor.hpp"
#include "LRPpreprocessorWordIdentification.hpp"
#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
#include "LRPpreprocessorWordReduction.hpp" 
#endif

#ifdef SANI_NEURAL_NETWORK
#include "ANNneuronClass.hpp"
#include "ANNdisplay.hpp"
#include "ANNdraw.hpp"
#include "ANNxmlConversion.hpp"
#include "SANIneuralNetworkOperations.hpp"
#endif
#include "SANIposRelTranslator.hpp"



#ifdef COMPILE_SANI

int main(const int argc, const char** argv);

class SANImainClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: LRPpreprocessorClass LRPpreprocessor;
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	private: LRPpreprocessorWordReductionClass LRPpreprocessorWordReduction;
	#endif
	//private: XMLrulesClassClass XMLrulesClass;
	#ifdef SANI_NEURAL_NETWORK
	private: ANNneuronClassClass ANNneuronClassClassObject;
	private: ANNdisplayClass ANNdisplay;
	private: ANNdrawClass ANNdraw;
	private: SANIneuralNetworkOperationsClass SANIneuralNetworkOperations;
	#endif
	private: SANIposRelTranslatorClass SANIposRelTranslator;

	public: bool executeSANI(

		SANItranslatorVariablesClass* translatorVariables,
		#ifdef SANI_SUPPORT_QUERIES
		SANItranslatorVariablesClass* translatorVariablesQuery,
		#endif
		
		string inputFolderLocal, 
		string outputFolderLocal,

		bool useInputTextPlainTXTFile,
		string inputTextPlainTXTfileName,
		#ifdef SANI_SUPPORT_QUERIES
		bool useInputQueryPlainTXTFile,
		string inputQueryPlainTXTFileName,	
		#endif
		#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
		bool useInputTextXMLFile,
		string inputTextXMLFileName,	
		bool useOutputTextXMLFile,
		string outputTextXMLFileName,
		#ifdef SANI_SUPPORT_QUERIES
		bool useInputQueryXMLFile,
		string inputQueryXMLFileName,
		bool useOutputQueryXMLFile,
		string outputQueryXMLFileName,	
		#endif
		#endif
		bool useOutputTextAllFile,
		string outputTextAllFileName,

		#ifdef SANI_NEURAL_NETWORK
		bool ANNuseInputXMLFile,
		string ANNinputXMLFileName,
		bool ANNuseOutputXMLFile,
		string ANNoutputXMLFileName,
		bool ANNuseOutputLDRFile,
		string ANNoutputLDRFileName,
		bool ANNuseOutputSVGFile,
		string ANNoutputSVGFileName,
		bool ANNuseOutputPPMFile,
		string ANNoutputPPMFileName,
		bool ANNuseOutputPPMFileRaytraced,
		string ANNoutputPPMFileNameRaytraced,
		bool ANNuseOutputAllFile,
		string ANNoutputAllFileName,
		bool ANNuseSprites,
		#endif

		int rasterImageWidth,
		int rasterImageHeight,

		#ifdef SANI_SUPPORT_QUERIES
		bool useInputQuery,
		#endif

		#ifdef LRP_PREPROCESSOR
		bool useLRP,
		bool useOutputLRPTextPlainTXTFile,
		string outputLRPTextPlainTXTFileName,
		string lrpDataFolderName
		#endif
		);
		
	#ifdef USE_CS_WORKAROUND
	public: bool executeSANI2();
	#endif
};
#endif




#endif
