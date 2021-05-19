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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p7b 24-March-2021
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
#include "LRPpreprocessorSentenceClass.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "LRPpreprocessorPOStagger.hpp"
#include "SANIposRelTranslatorVariables.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperations.hpp"
#include "SANIpropagateOperationsParseTree.hpp"
#include "SANIformation.hpp"
#ifdef SANI_SEQUENCE_GRAMMAR
#include "SANIpropagateCompactGenerateOperations.hpp"
#endif
#include "ANNdisplay.hpp"



#ifdef SANI_COMPACT


class SANIpropagateCompactClass
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
	private: SANIformationClass SANIformation;
	#ifdef SANI_SEQUENCE_GRAMMAR
	private: SANIpropagateCompactGenerateOperationsClass SANIpropagateCompactGenerateOperations;
	#endif
	private: ANNdisplayClass ANNdisplay;

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
	public: void initialiseIncrementalGeneration();
	#endif
	
	public: bool defineFirstLayer(SANItranslatorVariablesClass* translatorVariables, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
	public: bool executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents);
		public: bool executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, constEffective SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, bool parserEnabled, int* performance);
			public: bool executePosRelTranslatorNeuralNetworkPart1(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, constEffective SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, bool parserEnabled, int* performance);
			public: bool executePosRelTranslatorNeuralNetworkPart2(const SANItranslatorVariablesClass* translatorVariables, const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, constEffective SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
				public: bool performPropagationTest(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
					public: bool performPropagationTest(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool recordTopLevelParseTreeGroup, constEffective SANIGroupParseTree** topLevelParseTreeGroup);
				public: bool performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence);
				public: bool performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);
					public: bool performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);
				private: bool propagateWordThroughNetworkIntro(SANItranslatorVariablesClass* translatorVariables, const int w, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer);
					private: bool propagateWordThroughNetworkGroupInit(SANItranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer);
				private: bool propagateWordThroughNetworkGroup(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
					private: bool propagateWordThroughNetworkGroupSelect(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
						private: bool propagateWordThroughNetworkGroupComponentWrapper(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent);
							private: bool propagateWordThroughNetworkGroupComponent(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFoundStartComponent, bool missingStartComponentFound, bool missingOrVariableStartComponentFound);
				private: bool sequentialActivationConnectivityTests(const SANItranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, const int layer, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, bool existingActivationFoundStartComponent, const bool existingActivationFoundEndComponent, const bool skipWordConnectivityTests);
				private: bool printBackpropParseTree(SANIGroupParseTree* group, const int level);

	public: bool identifyComponentIndexLastActivatedIndex(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex);
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
	public: bool upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, const SANIComponentNeuralNetwork* component, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, bool existingActivationFoundStartComponent);
		public: bool upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int wordIndexMinToFindAbove, const int wordIndexMaxToFindAbove, const int wordIndexLastAboveEffectiveAfterGroupReset, int level, const SANIGroupNeuralNetwork* group, bool wordIndexMinFound, bool wordIndexMaxFound);
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
	public: bool printSANInetworkSVG(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes);
	#endif
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS
	public: bool verifyWordIndexCoverageIntegrity(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup, const SANIForwardPropogationWordData* forwardPropogationWordData);
	#endif
};

#endif

#endif
