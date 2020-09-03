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
 * File Name: SANIpropagateCompactGenerate.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1m5b 01-September-2020
 * Requirements: 
 * Description: SANI (Sequentially Activated Neuronal Input neural network) Propagate Compact - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_COMPACT_GENERATE
#define HEADER_SANI_PROPAGATE_COMPACT_GENERATE

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperations.hpp"
#include "SANIpropagateCompact.hpp"
#include "SANIformation.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "ANNdisplay.hpp"



#ifdef SANI_SEQUENCE_GRAMMAR

class SANIpropagateCompactGenerateClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: SANIpropagateCompactClass SANIpropagateCompact;
	private: SANIformationClass SANIformation;

	private: ANNdisplayClass ANNdisplay;

	public: bool generatePosRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool createNewConnections);
		public: bool generatePosRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance, const bool createNewConnection);
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
			private: bool findAndLinkReferenceSetCandidates1(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup, int layer);
				private: bool findAndLinkReferenceSetCandidates2(SANIGroupNeuralNetwork* neuralNetworkGroup, SANIGroupNeuralNetwork* referenceSetCandidate1, vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
					private: bool createReferenceSetCandidateVector(SANIGroupParseTree* currentParseTreeGroup, vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector);
					private: bool createReferenceSetCandidateVector(SANIGroupNeuralNetwork* currentNeuron, vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector);
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_CENTRAL_COMPONENTS
			private: bool findAndLinkCentralVariationCandidates(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
				private: bool populateSecondComponentOfOutputNeuronSourceVector(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* group, vector<SANIGroupNeuralNetwork*>* secondComponentOfOutputNeuronSourceVector);
			#endif
			private: bool findAndReconcileVariationWrapper(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents);
				#ifndef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
				private: bool findAndReconcileVariationLimitNumComponentsSection(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior);
				#endif	
					#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
					private: bool findAndReconcileIncrementalVariationLimitNumComponentsSection(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, const bool enableVariableEndComponentDetection, const bool enableVariableEndComponentDetectionExpectedCompletedIdentifyingSentenceHighLevelNeurons, const bool passOverGenerationFindVariableEndComponent, const bool passOverGenerationFindGroupNeuron, bool* foundGroupNeuron, bool* foundCoverageVariableEndComponent, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons);
					#else
					private: bool findAndReconcileIncrementalVariationLimitNumComponentsSection(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray);
					#endif
					#else
					private: bool findAndReconcileIncrementalVariationLimitNumComponentsSection(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior);
					#endif
				private: bool findAndReconcileVariation(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents);
					private: bool findAndReconcileIncrementalVariation(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents);
						#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
						private: SANIGroupNeuralNetwork* splitNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* partiallyActivatedNeuronWithMaxWordIndexCoverage);
						#endif
						private: bool addNeuronToList(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence);
						#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
						private: bool connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, const bool completedIdentifyingSentenceHighLevelNeurons, int indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray);
						#else
						private: bool connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, const bool completedIdentifyingSentenceHighLevelNeurons, int indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior);
						#endif
						private: bool connectListOfHighLevelNeuronsToNewNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron);
						#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
						private: bool verifyAndMarkNeuronAsCandidateWrapper(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* candidateNeuronBaseParseTree, SANIComponentNeuralNetwork** candidateComponent2);
							private: bool verifyAndMarkNeuronAsCandidate(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentNeuron, SANIComponentNeuralNetwork** candidateComponent2);
						#endif
						private: bool createOrAppendFirstLevelHiddenLayerGroup(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentLayerNeuronGroupStart, bool* creatingNewNeuronSequence, int* neuronSequenceIndex, SANIGroupNeuralNetwork** newNeuronSequenceGroup, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons);
						#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
						private: bool directWireLowLevelPOSneuronToGrammaticalSentenceNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* singleComponentNeuronCandidate, SANIGroupNeuralNetwork* grammaticalSentenceNeuron);
						#endif

	private: bool addComponentToFirstLevelHiddenLayerGroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* inputLayerPOSneuron, SANIGroupNeuralNetwork* newNeuronSequenceGroup);
	private: SANIGroupNeuralNetwork* createNewHiddenLayerGroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes);
	//private: SANIGroupNeuralNetwork* createNewFirstLevelHiddenLayerGroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupNeuralNetwork* inputLayerPOSneuron);
	private: SANIGroupNeuralNetwork* splitGroupAtLastActivatedComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupNeuralNetwork* neuronToSplit, int indexToSplitVector);
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	private: SANIGroupNeuralNetwork* splitGroupAtLastActivatedComponentUnordered(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupNeuralNetwork* neuronToSplit, int indexToSplitVector1, int indexToSplitVector2);
	#endif	

	private: bool addVariableComponentToGroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelComponentGroupOwner, bool addToStart);
	private: bool addComponentToGroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelComponentGroupOwner, int componentType, bool insertAtStart);
	private: bool addComponentToGroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* higherLevelComponent);
		private: SANIGroupNeuralNetwork* createNewGroup();

	private: bool findGroupDirectlyAbove(SANIGroupNeuralNetwork* group1, SANIGroupNeuralNetwork* group2);

	private: bool verifyLastWordIndex(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroup, int lastWordIndex, bool* adjacent);
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	private: bool markSubNeurons(SANIGroupNeuralNetwork* currentNeuron);
	private: bool markSubNeuronsReset(SANIGroupNeuralNetwork* currentNeuron);
	#endif
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_BASIC
	private: bool addSubNeuronsToList(vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeuronsCompleteHistory, SANIGroupNeuralNetwork* currentNeuron);
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	private: bool addSubNeuronsToListReset(SANIGroupNeuralNetwork* currentNeuron);
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_DO_NOT_HAVE_CONNECTIONS_TO_ANY_NEW_CANDIDATE
	private: bool addParentNeuronsToList(vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeuronsCompleteHistory, SANIGroupNeuralNetwork* currentNeuron);
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	private: bool addParentNeuronsToListReset(SANIGroupNeuralNetwork* currentNeuron);
	#endif
	#endif
	
	#endif
	#endif
		
	private: int calculateNextIndexInSequence(SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
	
	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	private: bool findTempVariableEndComponentNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** variableEndComponentNeuron);
	private: void setTempVariableEndComponentNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork* variableEndComponentNeuron);
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH
	private: int calculateLowestLayerNeuronMaxDepth(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray);
	#endif
	private: bool findLowestLayerNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** lowestLayerNeuron, int* lowestLayerNeuronIndex);
	private: bool findHighestLayerNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** highestLayerNeuron, int* highestLayerNeuronIndex);
	private: bool updateHighLevelNeuronHierachy(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, int lowestLayerNeuronIndex, SANIGroupNeuralNetwork* grammaticalSentenceNeuronSubHigher, const bool completedIdentifyingSentenceHighLevelNeurons);
	#endif
};

#endif

#endif
