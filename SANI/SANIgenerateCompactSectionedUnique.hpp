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
 * File Name: SANIgenerateCompactSectionedUnique.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1o2a 08-November-2020
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Sectioned Unique components - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_GENERATE_COMPACT_SECTIONED_UNIQUE
#define HEADER_SANI_GENERATE_COMPACT_SECTIONED_UNIQUE

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "LRPpreprocessorSentenceClass.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperations.hpp"
#include "SANIpropagateOperationsParseTree.hpp"
#include "SANIpropagateCompact.hpp"
#include "SANIpropagateCompactGenerateOperations.hpp"
#include "SANIgenerateCompactIdentify.hpp"
#include "SANIformation.hpp"
#include "SANIgenerateCompactOperations.hpp"




#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED

class SANIgenerateCompactSectionedUniqueClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: SANIpropagateOperationsParseTreeClass SANIpropagateOperationsParseTree;
	private: SANIpropagateCompactClass SANIpropagateCompact;
	private: SANIpropagateCompactGenerateOperationsClass SANIpropagateCompactGenerateOperations;
	private: SANIformationClass SANIformation;
	private: SANIgenerateCompactIdentifyClass SANIgenerateCompactIdentify;
	private: SANIgenerateCompactOperationsClass SANIgenerateCompactOperations;

	private: ANNdisplayClass ANNdisplay;

	public: bool findAndReconcileVariationWrapper(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);

		#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
		#ifndef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		private: bool findAndReconcileVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior);
		#endif	
			#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
			private: bool findAndReconcileIncrementalVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray);
			#else
			private: bool findAndReconcileIncrementalVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior);
			#endif
		#endif

				#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
				private: bool addNeuronToListSectioned(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence);
				#endif	
					private: bool addNeuronToList(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence, SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent);

				#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
				private: bool connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, const bool completedIdentifyingSentenceHighLevelNeurons, int indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray);
				#else
				private: bool connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, const bool completedIdentifyingSentenceHighLevelNeurons, int indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior);
				#endif

				private: bool createOrAppendFirstLevelHiddenLayerGroup(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentLayerNeuronGroupStart, bool* creatingNewNeuronSequence, int* neuronSequenceIndex, SANIGroupNeuralNetwork** newNeuronSequenceGroup, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons);

};

#endif

#endif
