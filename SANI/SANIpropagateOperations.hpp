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
 * Project Version: 1o3a 16-November-2020
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
#include "LRPpreprocessorSentenceClass.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperationsParseTree.hpp"
#ifdef SANI_SEQUENCE_GRAMMAR
#include "SANIpropagateCompactOperations.hpp"
#include "SANIpropagateCompactGenerateOperations.hpp"
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
	private: SANIpropagateOperationsParseTreeClass SANIpropagateOperationsParseTree;
	#ifdef SANI_SEQUENCE_GRAMMAR
	private: SANIpropagateCompactOperationsClass SANIpropagateCompactOperations;
	private: SANIpropagateCompactGenerateOperationsClass SANIpropagateCompactGenerateOperations;
	#endif

	#ifdef SANI_FORWARD
	
	public: void setParseSentenceReverseStrict(bool parseSentenceReverseNew, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);	//set parseSentenceReverse irrespective of SANI_REVERSE_DIRECTION
	public: void setParseSentenceReverse(bool normalDirection, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);	//determine and set parseSentenceReverse depending on SANI_REVERSE_DIRECTION	

	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent);
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
	#ifdef SANI_SUPPORT_COMPONENTS_OR					
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components);
	#endif
	#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, bool* repeatDetected);
	#endif
	#endif
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent, bool* existingActivationFoundStartComponent);
			public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, bool* missingStartComponentFound, bool* missingOrVariableStartComponentFound, bool* missingOrVariableEndComponentFound, int* numberOfInactiveComponentsRemaining, SANIGroupNeuralNetwork* ownerGroup, bool componentWordConnectivityTestsResult);
				private: bool identifySequentialActivationFound(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIComponentNeuralNetwork*>* components, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, SANIGroupNeuralNetwork* ownerGroup, bool componentWordConnectivityTestsResult);
				private: bool identifyMissingStartComponentFound(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIComponentNeuralNetwork*>* components, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* missingStartComponentFound, bool* stillParsingActiveComponents);
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
		#ifdef SANI_SUPPORT_COMPONENTS_OR					
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, bool* existingActivationFoundStartComponent);
		#endif
		#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, bool* repeatDetected, bool* existingActivationFoundStartComponent);
		#endif
		#endif

	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	#ifdef SANI_HEAVY
	public: bool componentWordConnectivityTestsWrapper(SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
	#else
	#ifdef SANI_LIGHT_UNOPTIMISED
	public: bool componentWordConnectivityTestsWrapper(SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
	#else
	public: bool componentWordConnectivityTestsWrapper(SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, bool existingActivationFoundStartComponent);
	#endif	
		public: bool componentWordConnectivityTestsWrapper(SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, bool existingActivationFoundStartComponent, bool existingActivationFoundEndComponent);
	#endif
			private: bool componentWordConnectivityTests(SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, SANIComponent* lastActiveComponent, SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, SANIComponentParseTree* lastActiveComponentInParseTree, SANIGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef, bool existingActivationFoundStartComponent);
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

	public: bool resetAllNeurons(vector<SANIGroupType*>* SANIGroupTypes, int groupBoolIndexType);
	public: bool resetAllNeuronComponents(vector<SANIGroupType*>* SANIGroupTypes, int groupBoolIndexType);

	#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS
	public: bool isLastComponentFuzzy(SANIGroupNeuralNetwork* group);
	#endif

	public: bool resetGroupActivation(SANIGroupNeuralNetwork* group);
		public: bool resetComponentsActivation(vector<SANIComponentNeuralNetwork*>* components);
	public: bool restoreGroupActivation(SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* groupOrig);
		public: bool restoreComponentsActivation(vector<SANIComponentNeuralNetwork*>* components, vector<SANIComponentNeuralNetwork*>* componentsOrig);
	
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	public: bool resetNeuronForwardProp(SANIGroupNeuralNetwork* group, int groupBoolIndexType);
	#endif	

	public: bool doesRequireResetGroupActivation(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* Xcomponent, int indexOfXcomponentInGroup, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationWordData* forwardPropogationWordData);
		private: bool findNextAvailableComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int indexOfXcomponentInGroup, SANIGroupNeuralNetwork* ownerGroup);
	
	#endif
};


#endif
