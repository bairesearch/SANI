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
 * File Name: SANIpropagateCompactReferenceSets.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Compact Reference Sets - ~O(n)
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_COMPACT_REFERENCESETS
#define HEADER_SANI_PROPAGATE_COMPACT_REFERENCESETS

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif
#include "SHAREDvars.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "LRPpreprocessorSentenceClass.hpp"
#include "LRPpreprocessorPOStagger.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperations.hpp"
#include "SANIformation.hpp"



#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE


class SANIpropagateCompactReferenceSetsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: LRPpreprocessorPOStaggerClass LRPpreprocessorPOStagger;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: SANIformationClass SANIformation;

	public: bool executePosRelTranslatorNeuralNetworkInitialise(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
	public: bool executePosRelTranslatorNeuralNetworkDeinitialise(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
		private: bool executePosRelTranslatorNeuralNetworkStart(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
		private: bool executePosRelTranslatorNeuralNetworkEnd(const SANItranslatorVariablesClass* translatorVariables, const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
	public: bool performPropagationIndex(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, multimap<double, SANIGroupNeuralNetwork*>* propagatedGroupsListPerformance, const int firstWordPropagatedIndex, const int indexInSequence, const bool detectConcepts);
		private: bool propagateWordThroughNetworkIntro(SANItranslatorVariablesClass* translatorVariables, const int w, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
			private: bool propagateWordThroughNetworkGroupInit(SANItranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
				private: bool propagateWordThroughNetworkGroup(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup);
					private: bool propagateWordThroughNetworkGroupSelect(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup);
						private: bool propagateWordThroughNetworkGroupComponentWrapper(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent);
							private: bool propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIGroupNeuralNetwork* ownerGroup, bool* successorComponentAlreadyActivated, bool* firstComponentInGroup);
							private: void calculateNeuronActivationSignal(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* testComponent, double* componentActivationSignal, int* numberComponentsActivated);
								private: void appendToWordsCaptured(vector<LRPpreprocessorPlainTextWord*>* sequenceElementsCaptured, const SANIComponentNeuralNetwork* currentComponent);
									private: void appendToWordsCaptured(vector<LRPpreprocessorPlainTextWord*>* sequenceElementsCaptured, LRPpreprocessorPlainTextWord* sequenceElementCaptured);
							private: double calculateActivationSignalStrength(double componentActivationSignal, double fractionOfComponentsActivated, bool componentActivationOrderSequential, double fractionOfComponentWordsCaptured, bool componentWordsOrderSequential, double neuronPopularity, double neuronRecencyAge);
							private: bool verifyWordsCapturedSequentiality(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIGroupNeuralNetwork* ownerGroup);

	//public: int64_t convertActivationSignalStrengthToLong(double activationSignalStrength);
	public: bool clearSANIpropagateCompactReferenceSetsNetworkCache(SANIGroupNeuralNetwork* neuron);

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
	private: bool printSANInetworkSVG(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes);
	#endif
					
};

#endif

#endif
