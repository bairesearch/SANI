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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
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
#include "LRPpreprocessorPOStagger.hpp"
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
	private: LRPpreprocessorPOStaggerClass LRPpreprocessorPOStagger;
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
	
	public: void setParseSentenceReverseStrict(bool parseSentenceReverseNew, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);	//set parseSentenceReverse irrespective of SANI_REVERSE_DIRECTION
	public: void setParseSentenceReverse(const bool normalDirection, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);	//determine and set parseSentenceReverse depending on SANI_REVERSE_DIRECTION	

	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent);
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
	#ifdef SANI_SUPPORT_COMPONENTS_OR					
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components);
	#endif
	#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
	public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components, bool* repeatDetected);
	#endif
	#endif
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent, bool* existingActivationFoundStartComponent);
			public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, bool* missingStartComponentFound, bool* missingOrVariableStartComponentFound, bool* missingOrVariableEndComponentFound, int* numberOfInactiveComponentsRemaining, SANIGroupNeuralNetwork* ownerGroup, const bool componentWordConnectivityTestsResult);
				private: bool identifySequentialActivationFound(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIComponentNeuralNetwork*>* components, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, SANIGroupNeuralNetwork* ownerGroup, bool componentWordConnectivityTestsResult);
				private: bool identifyMissingStartComponentFound(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIComponentNeuralNetwork*>* components, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* missingStartComponentFound, bool* stillParsingActiveComponents);
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
		#ifdef SANI_SUPPORT_COMPONENTS_OR					
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components, bool* existingActivationFoundStartComponent);
		#endif
		#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
		public: bool propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components, bool* repeatDetected, const bool* existingActivationFoundStartComponent);
		#endif
		#endif

	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	#ifdef SANI_HEAVY
	public: bool componentWordConnectivityTestsWrapper(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup);
	#else
	#ifdef SANI_LIGHT_UNOPTIMISED
	public: bool componentWordConnectivityTestsWrapper(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup);
	#else
	public: bool componentWordConnectivityTestsWrapper(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const bool existingActivationFoundStartComponent);
	#endif	
		public: bool componentWordConnectivityTestsWrapper(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const bool existingActivationFoundStartComponent, const bool existingActivationFoundEndComponent);
	#endif
			private: bool componentWordConnectivityTests(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, SANIComponent* lastActiveComponent, const SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, const SANIComponentParseTree* lastActiveComponentInParseTree, const SANIGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef, const bool existingActivationFoundStartComponent);
	#endif
		
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC_STRICT
	public: bool isSequenceWordDataFullyConnected(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
	public: void isSequenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, const SANIForwardPropogationSequenceElementData* currentSequenceElementData);
	#endif

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	public: bool componentTests1(const SANIComponentNeuralNetwork* currentComponent, const SANIGroupNeuralNetwork* group, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
	#endif
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	public: bool componentTests2(const SANIGroupNeuralNetwork* group, const SANIGroupParseTree* currentParseTreeGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
		private: bool getFirstWordInParseTree(const SANIGroupParseTree* currentParseTreeGroup, LRPpreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceSequenceIndexMin, int* numComponentsFound);

		#ifndef SANI_SEQUENCE_GRAMMAR
		private: bool findPreviousWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const LRPpreprocessorPlainTextWord* currentWord, const int previousWordPOStype);
		private: bool findPreceedingWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const LRPpreprocessorPlainTextWord* currentWord, const int existsPreceedingWordPOStype);
			public: bool checkWordPOStype(const LRPpreprocessorPlainTextWord* currentWord, const int unambiguousPOSindex);
		#endif
	#endif
	#endif

	public: bool resetAllNeurons(vector<SANIGroupType*>* SANIGroupTypes, const int groupBoolIndexType);
	public: bool resetAllNeuronComponents(vector<SANIGroupType*>* SANIGroupTypes, const int groupBoolIndexType);

	#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS
	public: bool isLastComponentFuzzy(const SANIGroupNeuralNetwork* group);
	#endif

	public: bool resetGroupActivation(SANIGroupNeuralNetwork* group);
		public: bool resetComponentsActivation(const vector<SANIComponentNeuralNetwork*>* components);
	public: bool restoreGroupActivation(SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* groupOrig);
		public: bool restoreComponentsActivation(const vector<SANIComponentNeuralNetwork*>* components, const vector<SANIComponentNeuralNetwork*>* componentsOrig);
	
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	public: bool resetNeuronForwardProp(const SANIGroupNeuralNetwork* group, const int groupBoolIndexType);
	#endif	

	public: bool doesRequireResetGroupActivation(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIComponentNeuralNetwork* Xcomponent, const int indexOfXcomponentInGroup, const SANIGroupNeuralNetwork* ownerGroup, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData);
		private: bool findNextAvailableComponent(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int indexOfXcomponentInGroup, const SANIGroupNeuralNetwork* ownerGroup);
	
	#endif
};


#endif
