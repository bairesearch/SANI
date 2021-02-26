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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1o5a 25-February-2021
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



#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_CONTINUOUS

class SANIgenerateCompactContinuousClass
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

	public: bool findAndReconcileVariationWrapper(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, const vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents);

		private: bool findAndReconcileVariation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents);
			private: bool findAndReconcileIncrementalVariation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents);

					private: bool addNeuronToList(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence, SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent);

				private: SANIGroupNeuralNetwork* splitNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* partiallyActivatedNeuronWithMaxWordIndexCoverage);
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
				private: bool addVariableFirstComponent(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron);
				#endif						
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
				private: bool prepareVariableLastComponent(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, constEffective SANIGroupParseTree* partiallyActivatedNeuronWithMaxWordIndexCoverage, constEffective SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent);
				private: bool addVariableLastComponent(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, const SANIGroupNeuralNetwork* neuron, SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent);
				#endif

				private: bool connectListOfHighLevelNeuronsToNewNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron);
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
				private: bool verifyAndMarkNeuronAsCandidateWrapper(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* candidateNeuronBaseParseTree, SANIComponentNeuralNetwork** candidateComponent2);
					private: bool verifyAndMarkNeuronAsCandidate(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIGroupNeuralNetwork* currentNeuron, SANIComponentNeuralNetwork** candidateComponent2);
				#endif
				private: bool createOrAppendFirstLevelHiddenLayerGroup(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentLayerNeuronGroupStart, bool* creatingNewNeuronSequence, int* neuronSequenceIndex, SANIGroupNeuralNetwork** newNeuronSequenceGroup, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons);
				#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
				private: bool directWireLowLevelPOSneuronToGrammaticalSentenceNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* singleComponentNeuronCandidate, SANIGroupNeuralNetwork* grammaticalSentenceNeuron);
				#endif

	private: SANIGroupNeuralNetwork* splitGroupAtLastActivatedComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupNeuralNetwork* neuronToSplit, const int indexToSplitVector);
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	private: SANIGroupNeuralNetwork* splitGroupAtLastActivatedComponentUnordered(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupNeuralNetwork* neuronToSplit, const int indexToSplitVector1, int indexToSplitVector2);
	#endif	

	
};

#endif

#endif
