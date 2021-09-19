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
 * File Name: SANIgenerateCompactContinuous.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1q2a 19-September-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Continuous - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_GENERATE_COMPACT_CONTINUOUS
#define HEADER_SANI_GENERATE_COMPACT_CONTINUOUS

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
#include "LRPpreprocessorPOStagger.hpp"
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



#define SANI_SEQUENCE_GRAMMAR_REFSET_DELIMITER_POS_NUMBER_OF_TYPES (3)
static int refsetDelimiterPOSarray[SANI_SEQUENCE_GRAMMAR_REFSET_DELIMITER_POS_NUMBER_OF_TYPES] = {LRP_SHARED_POS_TYPE_VERB, LRP_SHARED_POS_TYPE_PREPOSITION, LRP_SHARED_POS_TYPE_CONJUNCTION};
	
			
#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_CONTINUOUS

class SANIgenerateCompactContinuousClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: LRPpreprocessorPOStaggerClass LRPpreprocessorPOStagger;
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

	public: bool findAndReconcileVariationWrapper(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, const vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool supportVariableFirstComponents);

		private: bool findAndReconcileVariation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool supportVariableFirstComponents);
			private: bool findAndReconcileIncrementalVariation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool supportVariableFirstComponents);

					private: bool addNeuronToList(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence, SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent);

				private: SANIGroupNeuralNetwork* splitNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* partiallyActivatedNeuronWithMaxSequenceIndexCoverage);
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
				private: bool addVariableFirstComponent(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron);
				#endif						
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
				private: bool prepareVariableLastComponent(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, constEffective SANIGroupParseTree* partiallyActivatedNeuronWithMaxSequenceIndexCoverage, constEffective SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent);
				private: bool addVariableLastComponent(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, const SANIGroupNeuralNetwork* neuron, SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent);
				#endif

				private: bool connectListOfHighLevelNeuronsToNewNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron);
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
				private: bool verifyAndMarkNeuronAsCandidateWrapper(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* candidateNeuronBaseParseTree, SANIComponentNeuralNetwork** candidateComponent2);
					private: bool verifyAndMarkNeuronAsCandidate(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIGroupNeuralNetwork* currentNeuron, SANIComponentNeuralNetwork** candidateComponent2);
				#endif
				private: bool createOrAppendFirstLevelHiddenLayerGroup(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* currentLayerNeuronGroupStart, bool* creatingNewNeuronSequence, int* neuronSequenceIndex, SANIGroupNeuralNetwork** newNeuronSequenceGroup, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons);
				#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
				private: bool directWireLowLevelPOSneuronToGrammaticalSentenceNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* singleComponentNeuronCandidate, SANIGroupNeuralNetwork* grammaticalSentenceNeuron);
				#endif

	private: SANIGroupNeuralNetwork* splitGroupAtLastActivatedComponent(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupNeuralNetwork* neuronToSplit, const int indexToSplitVector);
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	private: SANIGroupNeuralNetwork* splitGroupAtLastActivatedComponentUnordered(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupNeuralNetwork* neuronToSplit, const int indexToSplitVector1, int indexToSplitVector2);
	#endif	
	
	#ifdef SANI_SEQUENCE_GRAMMAR_DISALLOW_DIRECT_REFSET_AND_DELIMITER_POS_COMPONENTS_ON_SAME_NEURON
	public: bool determineListHighLevelNeuronsDirectInput(SANIGroupNeuralNetwork* newNeuronSequenceGroup1, bool* highLevelListHasDirectInputRefSetPOS, bool* highLevelListHasDirectInputRefSetDelimiterPOS);
	public: bool determineListHighLevelNeuronsDirectInput(vector<SANIGroupNeuralNetwork*>* listOfInputNeurons, bool* highLevelListHasDirectInputRefSetPOS, bool* highLevelListHasDirectInputRefSetDelimiterPOS);
		private: bool sequenceElementHasRefsetDelimiterPOS(SANIGroupNeuralNetwork* inputNeuron);
	#endif
	
};

#endif

#endif
