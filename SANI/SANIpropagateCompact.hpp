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
 * File Name: SANIpropagateCompact.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1n4a 28-October-2020
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Compact - ~O(n)
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_COMPACT
#define HEADER_SANI_PROPAGATE_COMPACT

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "SANIposRelTranslatorVariables.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperations.hpp"
#include "SANIformation.hpp"
#include "LRPpreprocessorSentenceClass.hpp"
#include "LRPpreprocessorWordClass.hpp"
//#include "LRPpreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"



#ifdef SANI_COMPACT


class SANIpropagateCompactClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	//private: LRPpreprocessorPOStaggerClass LRPpreprocessorPOStagger;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: SANIformationClass SANIformation;

	private: ANNdisplayClass ANNdisplay;

	#ifdef SANI_FREE_MEMORY
	public: vector<SANIGroupParseTree*>* getParseTreeGroupList();
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
	public: void initialiseIncrementalGeneration();
	#endif
	
	public: bool defineFirstLayer(SANItranslatorVariablesClass* translatorVariables, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
	public: bool executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents);
		public: bool executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
			public: bool executeTxtRelTranslatorNeuralNetworkPart1(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
			public: bool executeTxtRelTranslatorNeuralNetworkPart2(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
				public: bool performPropagationTest(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
					public: bool performPropagationTest(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool recordTopLevelParseTreeGroup, SANIGroupParseTree** topLevelParseTreeGroup);
				public: bool performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence);
				public: bool performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);
					public: bool performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);
				private: bool propagateWordThroughNetworkIntro(SANItranslatorVariablesClass* translatorVariables, const int w, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer);
					private: bool propagateWordThroughNetworkGroupInit(SANItranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer);
				private: bool propagateWordThroughNetworkGroup(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
					private: bool propagateWordThroughNetworkGroupSelect(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
						private: bool propagateWordThroughNetworkGroupComponentWrapper(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent);
							private: bool propagateWordThroughNetworkGroupComponent(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFoundStartComponent, const bool missingStartComponentFound, const bool missingOrVariableStartComponentFound);
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
				private: bool verifyMissingOrVariableStartComponentFound(SANItranslatorVariablesClass* translatorVariables, int layer, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, SANIComponentNeuralNetwork* previousActiveComponent, SANIComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFoundStartComponent, bool* missingStartComponentFound, bool* variableStartComponentFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
				private: bool verifyMissingOrVariableStartComponentFoundAllowedWordIndices(SANItranslatorVariablesClass* translatorVariables, int layer, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, SANIComponentNeuralNetwork* previousActiveComponent, SANIComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFoundStartComponent, bool* missingStartComponentFound, bool* variableStartComponentFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_LAST_COMPONENTS
				private: bool propagateVariableEndComponentRemoveLastParseTreeComponent(SANItranslatorVariablesClass* translatorVariables, int layer, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, SANIComponentNeuralNetwork* previousActiveComponent, SANIComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFoundEndComponent, const bool componentWordConnectivityTests, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
				#endif
				private: bool updateActivatedNeuronWithMaxWordIndexCoverage(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool missingStartComponentFound, const bool missingOrVariableStartComponentFound, const bool missingOrVariableEndComponentFound, const bool candidateCoveragePartial);
				private: bool sequentialActivationConnectivityTests(SANItranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool existingActivationFoundStartComponent, const bool existingActivationFoundEndComponent, const bool skipWordConnectivityTests);
				private: void updateParseTreeMaxMinWordIndexOfParent(SANIGroupParseTree* parentGroup, SANIComponentParseTree* parentComponent, SANIGroupParseTree* childGroup);
					private: void updateParseTreeMaxMinWordIndexOfParentBasic(SANIGroupParseTree* parentGroup, SANIForwardPropogationWordData* forwardPropogationWordData, SANIGroupParseTree* childGroup);
				private: bool printBackpropParseTree(SANIGroupParseTree* group, int level);
			
	//public: bool verifyActivatedNeuronWithMaxWordIndexCoverage(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronBaseline, SANIGroupParseTree* activatedNeuronCandidate, const bool testWordIndicesAllowed, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const bool testMinNumComponents, const bool candidateCoveragePartial, const bool candidateCoverageVariable);
		public: bool verifyActivatedNeuronWithMaxWordIndexCoverage(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIGroupParseTree* activatedNeuronBaseline, SANIGroupParseTree* activatedNeuronCandidate, const bool testWordIndicesAllowed, const bool testMinNumComponents, const bool candidateCoveragePartial, const bool missingOrVariableStartComponentFound, const bool missingOrVariableEndComponentFound);
			public: bool verifyActivatedNeuronWithMaxWordIndexAllowed(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIGroupParseTree* activatedNeuronCandidate, const bool strictStartingCondition);
	public: bool verifyActivatedNeuronsAtLeastOne(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, SANIGroupNeuralNetwork* newNeuronSequenceGroup1);
	#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
	public: bool verifyNeverSplitGroupBetweenTwoIdenticalComponents(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, const bool candidateCoveragePartial);		
		private: bool neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage);
	#endif
		public: bool verifyActivatedNeuronsAtLeastOneBasic(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage);

	public: bool identifyComponentIndexLastActivatedIndex(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex);
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	public: bool identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex);
		public: bool identifyComponentIndexFirstAndLastActivatedIndexUnordered(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* firstActivatedIndex, int* lastActivatedIndex);
	#else
	public: bool identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex);
		public: bool identifyComponentIndexLastActivatedIndexUnordered(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex);	
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
	public: bool upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIComponentNeuralNetwork* component, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool existingActivationFoundStartComponent);
		public: bool upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int wordIndexMinToFindAbove, const int wordIndexMaxToFindAbove, const int wordIndexLastAboveEffectiveAfterGroupReset, int level, SANIGroupNeuralNetwork* group, bool wordIndexMinFound, bool wordIndexMaxFound);
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
	public: bool printSANInetworkSVG(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes);
	#endif
	
	public: int calculateNextIndexInSequenceProspective(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverageProspective);

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS
	public: bool verifyWordIndexCoverageIntegrity(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData);
	#endif
};

#endif

#endif
