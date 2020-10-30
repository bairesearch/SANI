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
 * File Name: SANIneuralNetworkOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1n3a 21-October-2020
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Neural Network Operations
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_NEURAL_NETWORK_OPERATIONS
#define HEADER_SANI_NEURAL_NETWORK_OPERATIONS

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif

#ifdef SANI_NEURAL_NETWORK

#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
//#include "GIAtranslatorOperations.hpp"
#ifdef SANI_ANN
#include "SANInodes.hpp"
#include "SANIformation.hpp"
#include "LRPpreprocessorSentenceClass.hpp"
//#include "LRPpreprocessorWordClass.hpp"
#endif
#include "ANNxmlConversion.hpp"
#include "ANNxmlConversion.hpp"
#include "SHAREDvars.hpp"




class SANIneuralNetworkOperationsClass
{
	#ifdef SANI_ANN
	private: SANInodesClass SANInodes;
	private: SANIformationClass SANIformation;
	#endif
	private: ANNneuronClassClass ANNneuronClass;
	private: ANNxmlConversionClass ANNxmlConversion;
	private: SHAREDvarsClass SHAREDvars;

	public: bool readNeuralNetXMLfile(const string xmlFileName, ANNneuron* firstInputNeuronInNetwork);
	public: bool writeNeuralNetXMLfile(const string xmlFileName, ANNneuron* ANNfirstInputNeuronInNetwork);
			
	#ifdef SANI_ANN
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
	public: void initialiseIncrementalGeneration();
	//private: bool generateNeuralNetFromSANInetReset(ANNtranslatorVariablesClass* translatorVariables);
	#endif
	public: bool generateNeuralNetFromSANInet(ANNtranslatorVariablesClass* translatorVariables);
		private: bool determinePositonsOfInputNeurons(int64_t* idBase, ANNneuron** firstOutputNeuronInNetworkPre);
		private: bool determinePositonsOfNeurons(vector<SANIGroupType*>* SANIGroupTypes, int64_t* idBase, ANNneuron* firstOutputNeuronInNetworkPre, ANNneuron** firstOutputNeuronInNetworkPost);
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
		private: bool determinePositonsOfNeuronsReset(vector<SANIGroupType*>* SANIGroupTypes);
		#endif	
	#endif
};

#endif

#endif

