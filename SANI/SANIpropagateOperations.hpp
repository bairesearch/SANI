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
 * File Name: SANIpropagateOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1n3b 21-October-2020
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Operations - generic functions
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_OPERATIONS
#define HEADER_SANI_PROPAGATE_OPERATIONS


#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif


#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
//#include "SANIformation.hpp"
#ifdef SANI_PARSE
//#include "GIAposRelTranslatorParserOperations.hpp"
#endif
#include "LRPpreprocessorSentenceClass.hpp"
#include "LRPpreprocessorWordClass.hpp"

#ifdef SANI_DEBUG_PROPAGATE_EXTRA8
extern bool firstExecution;
#endif


class SANIpropagateOperationsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	//private: SANIformationClass SANIformation;
	#ifdef SANI_PARSE
	//private: GIAposRelTranslatorParserOperationsClass GIAposRelTranslatorParserOperations;
	#endif

	#ifdef SANI_FORWARD
	
	public: void setParseSentenceReverseStrict(const bool parseSentenceReverseNew, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);	//set parseSentenceReverse irrespective of SANI_REVERSE_DIRECTION
	public: void setParseSentenceReverse(const bool normalDirection, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);	//determine and set parseSentenceReverse depending on SANI_REVERSE_DIRECTION	

	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent);
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
	#ifdef SANI_SUPPORT_COMPONENTS_OR					
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components);
	#endif
	#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected);
	#endif
	#endif
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent, bool* existingActivationFoundStartComponent);
			public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, bool* missingStartComponentFound, bool* missingOrVariableStartComponentFound, bool* missingOrVariableEndComponentFound, int* numberOfInactiveComponentsRemaining, SANIGroupNeuralNetwork* ownerGroup, const bool componentWordConnectivityTestsResult);
				private: bool identifySequentialActivationFound(vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, SANIGroupNeuralNetwork* ownerGroup, const bool componentWordConnectivityTestsResult);
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
				private: bool identifyMissingOrVariableStartComponentFound(vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* missingOrVariableStartComponentFound);
				#endif
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_FIRST_COMPONENTS_SHARED
					public: bool identifyMissingOrVariableStartComponentFound(vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* currentComponent);
					#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
				private: bool identifyMissingOrVariableEndComponentFound(vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* missingOrVariableEndComponentFound);
				#endif
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_LAST_COMPONENTS_SHARED
					public: bool identifyMissingOrVariableEndComponentFound(vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* currentComponent);
					#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_SHARED
						private: bool calculateVariableComponentPassCriteria(SANIComponentNeuralNetwork* component, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool testFirstComponentNotString, const bool testLastComponentNotString, const bool testFirstComponentSubgroupsUnique, const bool testLastComponentSubgroupsUnique, const bool testNotDualLowerLevelConnections);	
						#endif
				private: bool identifyMissingStartComponentFound(vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* missingStartComponentFound, bool* stillParsingActiveComponents);
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
		#ifdef SANI_SUPPORT_COMPONENTS_OR					
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, bool* existingActivationFoundStartComponent);
		#endif
		#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected, bool* existingActivationFoundStartComponent);
		#endif
		#endif

	#ifdef SANI_SEQUENCE_GRAMMAR
	private: bool upperNeuronContainsWordIndexOfProspectiveComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, LRPpreprocessorPlainTextWord* neuronComponentConnectionActiveWordRecord, SANIComponentNeuralNetwork* component, SANIGroupNeuralNetwork* group);
	#endif	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED
	private: bool repeatedSequenceDetected(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIGroupParseTree* currentParseTreeGroupTemp, const int wordTranslatorSentenceWordIndex);
	#endif

	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	private: bool groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient(SANIGroupNeuralNetwork* currentNeuron);
	#endif
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_BASIC
	private: bool groupContainsNeuronWithinProspectiveAlternateSubgroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentNeuron);
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	private: bool groupContainsNeuronWithinProspectiveAlternateSubgroupReset(SANIGroupNeuralNetwork* currentNeuron);
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_NEXT_SEQUENCE_IS_SAME_AS_CURRENT_SEQUENCE
	private: bool consecutiveSequenceDetected(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationWordData* forwardPropogationWordData, vector<SANIComponentNeuralNetwork*>* components, SANIComponentNeuralNetwork* component);
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_ACTIVATION_IF_REPEATED_SEQUENCE_MISMATCH_DETECTED
	private: bool findValidDualLowerLevelConnectionAlternate(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIComponentNeuralNetwork*>* components, SANIComponentNeuralNetwork* component);
		private: bool findGroup1InFirstSectionOfGroup2BackConnection(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentGroup2, SANIGroupNeuralNetwork* group1ToFound);
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF
	private: bool findValidDualLowerLevelConnection(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIComponentNeuralNetwork*>* components, SANIComponentNeuralNetwork* component, bool assumeFirstComponentActive);	
		#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE
		private: bool findGroup2InForwardConnectionBranchOfGroup1(SANIGroupNeuralNetwork* group2, SANIGroupNeuralNetwork* currentGroup);
		#endif
	#endif	

	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	#ifdef SANI_HEAVY
	public: bool componentWordConnectivityTestsWrapper(SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData);
	#else
	#ifdef SANI_LIGHT_UNOPTIMISED
	public: bool componentWordConnectivityTestsWrapper(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData);
	#else
	public: bool componentWordConnectivityTestsWrapper(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent);
	#endif	
		public: bool componentWordConnectivityTestsWrapper(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent, const bool existingActivationFoundEndComponent);
	#endif
			private: bool componentWordConnectivityTests(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, SANIComponent* lastActiveComponent, SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, SANIComponentParseTree* lastActiveComponentInParseTree, SANIGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef, SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent);
				public: bool getMinMaxWordIndexInParseTree(SANIGroupParseTree* currentParseTreeGroup, bool findMaxOrMinWordIndex, int* wordIndexMaxOrMin, int level);
				#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
				private: bool componentWordConnectivityTestsCompareToMemory(const int wordIndexProspectiveFirst, SANIGroupNeuralNetwork* ownerGroup, SANIComponent* lastActiveComponent);
				#endif
	#endif
		
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
	public: bool calculatePerformanceWeightOfParseTree(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* maxWeight);
		public: bool setPerformanceWeightOfParseTree(SANIGroupParseTree* currentParseTreeGroup, const int maxWeight);
	#endif

	#ifdef SANI_PARSE
	#ifdef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	public: bool updatePerformance(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
	#endif
		public: bool updatePerformanceGroup(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
	#endif
		public: bool traceBackpropParseTree(SANIGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<LRPpreprocessorPlainTextWord*>* sentenceContents);
			public: bool traceBackpropParseTree(SANIGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const bool calculateMaxWeight, int* maxWeight);
	#endif
		
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
	public: bool isSentenceWordDataFullyConnected(SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
	public: void isSentenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, SANIForwardPropogationWordData* currentWordData);
	#endif

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	public: bool componentTests1(SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
	#endif
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	public: bool componentTests2(SANIGroupNeuralNetwork* group, SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
		private: bool getFirstWordInParseTree(SANIGroupParseTree* currentParseTreeGroup, LRPpreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceWordIndexMin, int* numComponentsFound);

		private: bool findPreviousWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, LRPpreprocessorPlainTextWord* currentWord, int previousWordPOStype);
		private: bool findPreceedingWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, LRPpreprocessorPlainTextWord* currentWord, int existsPreceedingWordPOStype);
			public: bool checkWordPOStype(LRPpreprocessorPlainTextWord* currentWord, int unambiguousPOSindex);
	#endif
	#endif

	public: bool resetAllNeurons(vector<SANIGroupType*>* SANIGroupTypes, const int groupBoolIndexType);
	public: bool resetAllNeuronComponents(vector<SANIGroupType*>* SANIGroupTypes, const int groupBoolIndexType);

	#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS
	public: bool isLastComponentFuzzy(SANIGroupNeuralNetwork* group);
	#endif

	public: bool resetGroupActivation(SANIGroupNeuralNetwork* group);
		public: bool resetComponentsActivation(vector<SANIComponentNeuralNetwork*>* components);
	public: bool restoreGroupActivation(SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* groupOrig);
		public: bool restoreComponentsActivation(vector<SANIComponentNeuralNetwork*>* components, vector<SANIComponentNeuralNetwork*>* componentsOrig);
	#ifdef SANI_PARSE
	#ifdef SANI_HEAVY
	#ifdef SANI_HEAVY_OPTIMISED
	public: bool resetGroupParseTreeGroupRef(SANIGroupActivationMemory* activationMemoryGroup, SANIGroupParseTree* currentParseTreeGroup, const bool deleteExistingParseTreeGroupRef);
	#else
	public: bool resetGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef);
	#endif
	#else
	public: bool resetGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef);
	public: bool restoreGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, SANIGroupParseTree* parseTreeGroupOrig);
	#endif
	#endif
	
	#ifdef SANI_PARSE
	public: bool resetNeuronBackprop(SANIGroupParseTree* currentParseTreeGroup, const int groupBoolIndexType);
	#endif
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	public: bool resetNeuronForwardProp(SANIGroupNeuralNetwork* group, const int groupBoolIndexType);
	#endif	

	public: bool doesRequireResetGroupActivation(SANIComponentNeuralNetwork* Xcomponent, int indexOfXcomponentInGroup, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationWordData* forwardPropogationWordData);
		private: bool findNextAvailableComponent(int indexOfXcomponentInGroup, SANIGroupNeuralNetwork* ownerGroup);
	
	#endif
	
	#ifdef SANI_FREE_MEMORY
	public: bool initialiseParseTreeGroupList(vector<SANIGroupType*>* SANIGroupTypes, vector<SANIGroupParseTree*>* parseTreeGroupList);
	public: bool deinitialiseParseTreeGroupList(vector<SANIGroupType*>* SANIGroupTypes, vector<SANIGroupParseTree*>* parseTreeGroupList);
	#endif
	
	
	
	
};



#endif
