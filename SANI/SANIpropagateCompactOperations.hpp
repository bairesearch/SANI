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
 * File Name: SANIpropagateCompactOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p8a 29-April-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Operations - generic functions
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_COMPACT_OPERATIONS
#define HEADER_SANI_PROPAGATE_COMPACT_OPERATIONS


#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif


#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "LRPpreprocessorSentenceClass.hpp"
#include "LRPpreprocessorPOStagger.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperationsParseTree.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR

class SANIpropagateCompactOperationsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: LRPpreprocessorPOStaggerClass LRPpreprocessorPOStagger;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsParseTreeClass SANIpropagateOperationsParseTree;

	public: bool identifySequentialActivationFound(const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, SANIGroupNeuralNetwork* ownerGroup, const bool componentWordConnectivityTestsResult);
	public: bool identifyMissingStartComponentFound(const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, const SANIComponentNeuralNetwork* currentComponent, const SANIComponentNeuralNetwork* previousActiveComponent, bool* missingStartComponentFound, bool* stillParsingActiveComponents);

	#ifdef SANI_SEQUENCE_GRAMMAR
	private: bool upperNeuronContainsWordIndexOfProspectiveComponent(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const LRPpreprocessorPlainTextWord* neuronComponentConnectionActiveWordRecord, const SANIComponentNeuralNetwork* component, const SANIGroupNeuralNetwork* group);
	#endif	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED
	private: bool repeatedSequenceDetected(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIForwardPropogationWordData* forwardPropogationWordData, const SANIGroupParseTree* currentParseTreeGroupTemp, const int wordTranslatorSentenceWordIndex);
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_NEXT_SEQUENCE_IS_SAME_AS_CURRENT_SEQUENCE
	private: bool consecutiveSequenceDetected(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIForwardPropogationWordData* forwardPropogationWordData, const vector<SANIComponentNeuralNetwork*>* components, const SANIComponentNeuralNetwork* component);
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_ACTIVATION_IF_REPEATED_SEQUENCE_MISMATCH_DETECTED
	private: bool findValidDualLowerLevelConnectionAlternate(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIComponentNeuralNetwork* component);
		private: bool findGroup1InFirstSectionOfGroup2BackConnection(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentGroup2, const SANIGroupNeuralNetwork* group1ToFound);
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF
	public: bool findValidDualLowerLevelConnection(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIComponentNeuralNetwork* component, const bool assumeFirstComponentActive);	
		#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE
		private: bool findGroup2InForwardConnectionBranchOfGroup1(const SANIGroupNeuralNetwork* group2, const SANIGroupNeuralNetwork* currentGroup);
		#endif
	#endif
	
};

#endif

#endif
