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
 * File Name: SANIgenerateCompactSectionedVariable.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p1a 04-March-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Sectioned Variable components - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_GENERATE_COMPACT_SECTIONED_VARIABLE
#define HEADER_SANI_GENERATE_COMPACT_SECTIONED_VARIABLE

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

class SANIgenerateCompactSectionedVariableClass
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

	public: bool findAndReconcileVariationWrapper(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents);

		#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
		#ifndef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		private: bool findAndReconcileVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior);
		#endif	
			#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
			private: bool findAndReconcileIncrementalVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, const bool enableVariableEndComponentDetection, const bool enableVariableEndComponentDetectionExpectedCompletedIdentifyingSentenceHighLevelNeurons, const bool passOverGenerationFindVariableEndComponent, const bool passOverGenerationFindGroupNeuron, bool* foundGroupNeuron, bool* foundCoverageVariableEndComponent, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons);
			#else
			private: bool findAndReconcileIncrementalVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray);
			#endif
			#else
			private: bool findAndReconcileIncrementalVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior);
			#endif
		#endif

				#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
				private: bool addNeuronToListSectioned(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence);
				#endif	
					private: bool addNeuronToList(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence, constEffective SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent);

				#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
				private: bool connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, bool completedIdentifyingSentenceHighLevelNeurons, const int indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray);
				#else
				private: bool connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, const bool completedIdentifyingSentenceHighLevelNeurons, const int indexInSequenceStart, constEffective SANIGroupNeuralNetwork** highLevelNeuronPrior);
				#endif

				private: bool createOrAppendFirstLevelHiddenLayerGroup(const vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentLayerNeuronGroupStart, const bool* creatingNewNeuronSequence, const int* neuronSequenceIndex, constEffective SANIGroupNeuralNetwork** newNeuronSequenceGroup, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons);

	
	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH
	private: int calculateLowestLayerNeuronMaxDepth(const vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray);
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	private: bool findTempVariableEndComponentNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** variableEndComponentNeuron);
	private: void setTempVariableEndComponentNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork* variableEndComponentNeuron);
	#endif
	#endif
};

#endif

#endif
