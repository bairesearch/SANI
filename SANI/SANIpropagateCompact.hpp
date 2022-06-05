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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
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
	
	public: bool defineFirstLayer(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
	public: bool executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const bool simultaneousAmbiguousPOSpropagation);
		public: bool executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, constEffective SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, bool parserEnabled, int* performance, const bool simultaneousAmbiguousPOSpropagation);
			public: bool executePosRelTranslatorNeuralNetworkStart(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool parseIsolatedSubreferenceSets, bool parserEnabled, int* performance);
			public: bool executePosRelTranslatorNeuralNetworkEnd(const SANItranslatorVariablesClass* translatorVariables, const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
				public: bool performPropagationTest(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
					public: bool performPropagationTest(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool recordTopLevelParseTreeGroup, constEffective SANIGroupParseTree** topLevelParseTreeGroup);
				public: bool performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence);
				public: bool performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed, const int activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed);
					public: bool performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence, const int activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed, const int activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed);
				private: bool propagateWordThroughNetworkIntro(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, const int sequenceIndex, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool getFirstLayer);
					#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
					public: SANIGroupNeuralNetwork* createNewArtificalNeuronRepresentingAmbiguousPOSinputPermutation(SANItranslatorVariablesClass* translatorVariables, const LRPpreprocessorPlainTextWord* currentWord, const int sequenceIndex, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
					#endif
					private: bool propagateWordThroughNetworkGroupInit(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool getFirstLayer);
						private: bool propagateWordThroughNetworkGroupAddToFirstLayer(SANItranslatorVariablesClass* translatorVariables, int sequenceIndex, SANIGroupNeuralNetwork* inputLayerGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
						private: bool propagateWordThroughNetworkGroupInit2(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* inputLayerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
				private: bool propagateWordThroughNetworkGroup(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup);
					private: bool propagateWordThroughNetworkGroupSelect(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup);
						private: bool propagateWordThroughNetworkGroupComponentWrapper(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent);
							private: bool propagateWordThroughNetworkGroupComponent(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool activationSequenceCompleted, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFoundStartComponent, bool missingStartComponentFound, bool missingOrVariableStartComponentFound);
				private: bool sequentialActivationConnectivityTests(const SANItranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool activationSequenceCompleted, const int layer, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, bool existingActivationFoundStartComponent, const bool existingActivationFoundEndComponent, const bool skipWordConnectivityTests);
				private: bool printBackpropParseTree(SANIGroupParseTree* group, const int level);

	public: bool identifyComponentIndexLastActivatedIndex(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex);
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
	public: bool upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponentWrapper(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIComponentNeuralNetwork* component, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, bool existingActivationFoundStartComponent);
		public: bool upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int sequenceIndexMinToFindAbove, const int sequenceIndexMaxToFindAbove, const int sequenceIndexLastAboveEffectiveAfterGroupReset, int level, const SANIGroupNeuralNetwork* group, bool sequenceIndexMinFound, bool sequenceIndexMaxFound);
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
	public: bool printSANInetworkSVG(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes);
	#endif
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS
	public: bool verifySequenceIndexCoverageIntegrity(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* currentParseTreeGroup, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData);
	#endif
};

#endif

#endif
