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
 * File Name: SANIpropagateCompact.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Compact - ~O(n)
 * /
 *******************************************************************************/


#include "SANIpropagateCompact.hpp"
#include "SHAREDvars.hpp"
#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
#ifdef USE_GIA
#include "GIAneuralNetworkOperations.hpp"
#endif
#include "ANNdisplay.hpp"
#endif

#ifdef SANI_COMPACT

#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
static int incrementalGenerationIndex;
void SANIpropagateCompactClass::initialiseIncrementalGeneration()
{
	incrementalGenerationIndex = 0;
}
#endif

//if SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each sequenceIndex only
bool SANIpropagateCompactClass::defineFirstLayer(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	bool result = true;

	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
	forwardPropogationSequenceData->artificialInputNeuronLinkPosAmbiguousPermuations = true;
	#endif
			
	for(int i=0; i<forwardPropogationSequenceData->sentenceContents->size(); i++)
	{
		bool getFirstLayer = true;
		
		int sequenceIndex = i;

		SANIForwardPropogationSignalData forwardPropogationSignalDataNOTUSED;
		
		#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_SIMULTANEOUSLY_PROPAGATE_UNAMBIGUOUS_POS_PERMUTATIONS
		forwardPropogationSequenceData->firstLayer->push_back(NULL);
		#endif

		if(!propagateWordThroughNetworkIntro(translatorVariables, SANIGroupTypes, sequenceIndex, &forwardPropogationSignalDataNOTUSED, forwardPropogationSequenceData, getFirstLayer))
		{
			result = false;
		}
	}
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
	forwardPropogationSequenceData->artificialInputNeuronLinkPosAmbiguousPermuations = false;
	#endif
	
	return result;
}	
	
bool SANIpropagateCompactClass::executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const bool simultaneousAmbiguousPOSpropagation)
{
	SANIGroupParseTree* topLevelParseTreeGroup = NULL; 
	bool parseIsolatedSubreferenceSets = false;
	bool parserEnabled = false;
	int performance = 0;
	return executePosRelTranslatorNeuralNetwork(translatorVariables, SANIGroupTypes, sentenceContents, &topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performance, simultaneousAmbiguousPOSpropagation);
}

bool SANIpropagateCompactClass::executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, constEffective SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, bool parserEnabled, int* performance, const bool simultaneousAmbiguousPOSpropagation)
{
	bool result = false;

	SANIForwardPropogationSequenceData forwardPropogationSequenceData;
	forwardPropogationSequenceData.sentenceContents = sentenceContents;
	forwardPropogationSequenceData.simultaneousAmbiguousPOSpropagation = simultaneousAmbiguousPOSpropagation;

	#ifndef SANI_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	executePosRelTranslatorNeuralNetworkStart(translatorVariables, SANIGroupTypes, &forwardPropogationSequenceData, parseIsolatedSubreferenceSets, parserEnabled, performance);

	if(performPropagationTest(translatorVariables, SANIGroupTypes, &forwardPropogationSequenceData, true, topLevelParseTreeGroup))
	{
		result = true;
	}

	executePosRelTranslatorNeuralNetworkEnd(translatorVariables, SANIGroupTypes, &forwardPropogationSequenceData, parseIsolatedSubreferenceSets, parserEnabled, performance);
	#endif
	
	return result;
}

bool SANIpropagateCompactClass::executePosRelTranslatorNeuralNetworkStart(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool parseIsolatedSubreferenceSets, bool parserEnabled, int* performance)
{
	bool result = true;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;

	#ifdef SANI_PARSE_SAVE_PARSE_TREE
	forwardPropogationSequenceData->topLevelParseTreeGroupPropagate = NULL;
	#endif
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT	
	forwardPropogationSequenceData->parseTreeMaxWeightPropagate = 0;
	#endif
	
	//sentence specific variables:
	SANIpropagateOperations.setParseSentenceReverse(true, forwardPropogationSequenceData);
	#ifdef SANI_PARSE_SIMULTANEOUS
	forwardPropogationSequenceData->parserEnabled = parserEnabled;
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION
	forwardPropogationSequenceData->SANIGroupTypes = SANIGroupTypes;
	#endif
	
	for(int sequenceIndex=0; sequenceIndex<forwardPropogationSequenceData->sentenceContents->size(); sequenceIndex++)
	{	
		SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = new SANIForwardPropogationSequenceElementData();
		forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray.push_back(forwardPropogationSequenceElementData);
	}
		
	return result;
}

bool SANIpropagateCompactClass::executePosRelTranslatorNeuralNetworkEnd(const SANItranslatorVariablesClass* translatorVariables, const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = true;
	
	#ifdef SANI_FREE_MEMORY
	for(int sequenceIndex=0; sequenceIndex<forwardPropogationSequenceData->sentenceContents->size(); sequenceIndex++)
	{
		SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray[sequenceIndex];
		delete forwardPropogationSequenceElementData;
	}
	#endif
	
	if(result)
	{
		#ifdef SANI_PARSE_PERFORMANCE
		*performance = forwardPropogationSequenceData->sentenceContents->size();
		#else
		*performance = 1;
		#endif
	}
	
	return result;
}
	
bool SANIpropagateCompactClass::performPropagationTest(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{	
	bool recordTopLevelParseTreeGroup = false;
	return performPropagationTest(translatorVariables, SANIGroupTypes, forwardPropogationSequenceData, recordTopLevelParseTreeGroup, NULL);
}
bool SANIpropagateCompactClass::performPropagationTest(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool recordTopLevelParseTreeGroup, constEffective SANIGroupParseTree** topLevelParseTreeGroup)
{
	bool result = false;
	
	bool deinitialiseParseTreeGroupAfterFinish = true;
	bool resetAllNeuronComponents = true;
	int firstIndexInSequence = 0;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	SANIpropagateOperations.setParseSentenceReverse(false, forwardPropogationSequenceData);
	#endif
	
	if(performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSequenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence))
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
		cout << "toplevelGroupActivationFound: groupIndexes = " << SANInodes.printParseTreeGroupIndices(forwardPropogationSequenceData->topLevelParseTreeGroupPropagate, 0) << endl;
		#endif
		result = true;
		
		#ifdef SANI_PARSE_SAVE_PARSE_TREE
		if(recordTopLevelParseTreeGroup)
		{
			*topLevelParseTreeGroup = forwardPropogationSequenceData->topLevelParseTreeGroupPropagate;
		}
		#endif
	}
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	SANIpropagateOperations.setParseSentenceReverse(true, forwardPropogationSequenceData);
	#endif
	
	return result;
}

bool SANIpropagateCompactClass::performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence)
{	
	int activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed;
	int activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed;
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed = forwardPropogationSequenceData->sentenceContents->size()-1 - firstIndexInSequence;
		activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed = 0;
	}
	else
	{
		activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed = firstIndexInSequence;
		activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed = forwardPropogationSequenceData->sentenceContents->size()-1;
	}
	return performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSequenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed, activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed);
}

bool SANIpropagateCompactClass::performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed, const int activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed)
{
	int firstIndexInSequence;
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		firstIndexInSequence = forwardPropogationSequenceData->sentenceContents->size()-1;
	}
	else
	{
		firstIndexInSequence = 0;
	}
	return performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSequenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed, activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed);

}

bool SANIpropagateCompactClass::performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence, const int activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed, const int activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed)
{
	bool result = false;
	
	forwardPropogationSequenceData->toplevelGroupActivationFound = false;
	
	if(resetAllNeuronComponents)
	{
		SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE); //this is required for SANI_LIGHT_OPTIMISED
		SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of SANIpropagateCompactClass::executePosRelTranslatorNeuralNetwork)
	}
	#ifdef SANI_FREE_MEMORY
	SANIpropagateOperationsParseTree.initialiseParseTreeGroupList(SANIGroupTypes, &(forwardPropogationSequenceData->parseTreeGroupListPropagate));
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	int iStart = firstIndexInSequence;
	#else
	int iStart = 0;
	#endif
	for(int i=iStart; i<forwardPropogationSequenceData->sentenceContents->size(); i++)	//OLD: for(int i=currentFirstInputNeuronIndexInSequence; ...
	{
		int firstLayerNeuronIndex;	//sequenceIndex
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			firstLayerNeuronIndex = forwardPropogationSequenceData->sentenceContents->size()-1-i;
		}
		else
		{
			firstLayerNeuronIndex = i;
		}	
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR	
		cout << "\n\n \e[33m SANIpropagateCompactClass::performPropagation: i = " << i << ", firstLayerNeuronIndex = " << firstLayerNeuronIndex << " - " << (*(forwardPropogationSequenceData->sentenceContents))[firstLayerNeuronIndex]->tagName << " \e[0m" << endl;
		#endif
	
		//group specific variables:
		SANIForwardPropogationSignalData forwardPropogationSignalData;
		forwardPropogationSignalData.activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed = activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed;
		forwardPropogationSignalData.activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed = activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed;
		#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		forwardPropogationSignalData.firstIndexInSequence = firstIndexInSequence;
		forwardPropogationSignalData.currentIndexInSequence = i;
		#endif
		
		bool getFirstLayer = false;
		if(propagateWordThroughNetworkIntro(translatorVariables, SANIGroupTypes, firstLayerNeuronIndex, &forwardPropogationSignalData, forwardPropogationSequenceData, getFirstLayer))
		{

		}
	}

	if(deinitialiseParseTreeGroupAfterFinish)
	{
		#ifdef SANI_FREE_MEMORY
		SANIpropagateOperationsParseTree.deinitialiseParseTreeGroupList(SANIGroupTypes, &(forwardPropogationSequenceData->parseTreeGroupListPropagate));
		#endif
	}
	
	if(forwardPropogationSequenceData->toplevelGroupActivationFound)
	{
		result = true;
	}
	
	return result;
}

bool SANIpropagateCompactClass::propagateWordThroughNetworkIntro(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, const int sequenceIndex, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool getFirstLayer)
{
	bool result = true;
	
	//cout << "sequenceIndex = " << sequenceIndex << endl;
	
	LRPpreprocessorPlainTextWord* currentWord = (*(forwardPropogationSequenceData->sentenceContents))[sequenceIndex];

	SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = (forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray[sequenceIndex]);
	forwardPropogationSequenceElementData->wordReference = currentWord;
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationSequenceElementData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
	#endif
	forwardPropogationSequenceElementData->sequenceIndex = sequenceIndex;

	#ifdef SANI_DEBUG_PROPAGATE
	cout << "propagateWordThroughNetworkIntro: currentWord = " << currentWord->tagName << endl;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	int sequenceElementPOStypeNOTUSED = INT_DEFAULT_VALUE;
	if(!propagateWordThroughNetworkGroupInit(translatorVariables, SANIGroupTypes, sequenceElementPOStypeNOTUSED, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, getFirstLayer))
	{
		result = false;
	}	
	#else
	//#ifndef SANI_SEQUENCE_GRAMMAR_DETERMINE_POS_AMIGUITY_INFO_AT_START	//already inferred POS type from POSunknown
	if(!SANInodes.currentWordPOSunknown(currentWord))
	{
	//#endif

		#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
		if(forwardPropogationSequenceData->artificialInputNeuronLinkPosAmbiguousPermuations)	//implied true: getFirstLayer
		{
			if(LRPpreprocessorPOStagger.isWordPOSambiguous(currentWord))
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
				createNewArtificalNeuronRepresentingAmbiguousPOSinputPermutation(translatorVariables, currentWord, sequenceIndex, forwardPropogationSequenceData);
				#else
				//do not assign the sequenceElement an input layer neuron at present (as it is POS ambiguous).
				//if the pos ambiguous sequence is not matched with an existing network subnet, then createNewArtificalNeuronRepresentingAmbiguousPOSinputPermutation will be called
				#endif
			}
			else
			{
				//case normal:
				int wordPOStype = currentWord->unambiguousPOSindex;
				if(!propagateWordThroughNetworkGroupInit(translatorVariables, SANIGroupTypes, wordPOStype, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, getFirstLayer))
				{
					result = false;
				}
			}
		}
		else
		{
		#endif
	
			if(!forwardPropogationSequenceData->simultaneousAmbiguousPOSpropagation)
			{
				int wordPOStype = currentWord->unambiguousPOSindex;
				if(!propagateWordThroughNetworkGroupInit(translatorVariables, SANIGroupTypes, wordPOStype, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, getFirstLayer))
				{
					result = false;
				}
			}
			else
			{
				//cout << "forwardPropogationSequenceData->simultaneousAmbiguousPOSpropagation" << endl;
				#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
				forwardPropogationSignalData->firstPOSval = false;
				#endif
				for(int wordPOStype=0; wordPOStype<LRP_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
				{
					if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, wordPOStype))
					{						
						if(!propagateWordThroughNetworkGroupInit(translatorVariables, SANIGroupTypes, wordPOStype, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, getFirstLayer))
						{
							result = false;
						}
						#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
						forwardPropogationSignalData->firstPOSval = false;
						#endif
					}
				}
			}
		#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
		}
		#endif
	//#ifndef SANI_SEQUENCE_GRAMMAR_DETERMINE_POS_AMIGUITY_INFO_AT_START
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
	else
	{
		int wordPOStype = INT_DEFAULT_VALUE;
		bool pass = LRPpreprocessorPOStagger.getWordPOStypeFromWordPOSunknown(currentWord, &wordPOStype);
		if(pass)
		{
			if(!propagateWordThroughNetworkGroupInit(translatorVariables, SANIGroupTypes, wordPOStype, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, getFirstLayer))
			{
				result = false;
			}		
		}
	}
	#endif
	#endif
	//#endif
	
	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
SANIGroupNeuralNetwork* SANIpropagateCompactClass::createNewArtificalNeuronRepresentingAmbiguousPOSinputPermutation(SANItranslatorVariablesClass* translatorVariables, const LRPpreprocessorPlainTextWord* currentWord, const int sequenceIndex, SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	bool result = true;

	//cout << "createNewArtificalNeuronRepresentingAmbiguousPOSinputPermutation: sequenceIndex = " << sequenceIndex << endl;
	
	//create new artifical neuron representing ambiguous POS input permutation (to be deleted when added to network by SANIgenerateCompact..)
	SANIGroupNeuralNetwork* inputLayerGroupArtificial = new SANIGroupNeuralNetwork();
	inputLayerGroupArtificial->inputLayerNeuronArtificialAmbiguousPOSpermutations = true;
	//#ifdef SANI_SEQUENCE_GRAMMAR
	//initialisations taken from SANIformationClass::createInputNeuronLayerSectionWordPOStype:
	inputLayerGroupArtificial->inputLayerNeuron = true;
	inputLayerGroupArtificial->groupTypeIsString = true;
	inputLayerGroupArtificial->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNKNOWN;	//CHECKTHIS: cannot be used by SANIpropagateCompact as it is ambiguous
	//#endif
	inputLayerGroupArtificial->inputLayerNeuronArtificialAmbiguousPOSpermutationsPOSambiguityInfo = currentWord->POSambiguityInfo;
	if(!propagateWordThroughNetworkGroupAddToFirstLayer(translatorVariables, sequenceIndex, inputLayerGroupArtificial, forwardPropogationSequenceData))
	{
		result = false;
	}
	
	return inputLayerGroupArtificial;
}
#endif
				
				
bool SANIpropagateCompactClass::propagateWordThroughNetworkGroupInit(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool getFirstLayer)
{
	bool result = true;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_DYNAMIC	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	SANIGroupNeuralNetwork* inputLayerGroup;
	if(!SANIformation.findInputNeuronLayerSectionWordOrig(forwardPropogationSequenceElementData, &inputLayerGroup))
	{
		SANIformation.addInputNeuronLayerSectionWordOrig(forwardPropogationSequenceElementData, &inputLayerGroup, SANIGroupTypes);
		#ifdef SANI_SEQUENCE_GRAMMAR_PARSE_TREE_SAVE_LEAF_NODES_ADD_INPUT_NEURONS_TO_GROUPTYPES_ARRAY
		SANIpropagateOperationsParseTree.resetGroupParseTreeGroupRef(inputLayerGroup, true);
		#endif
	}
	#else //SANI_SEQUENCE_GRAMMAR_INPUT_POS
	cerr << "SANIpropagateCompactClass::propagateWordThroughNetworkGroupInit error: SANI_SEQUENCE_GRAMMAR_INPUT_DYNAMIC requires SANI_SEQUENCE_GRAMMAR_INPUT_WORDS" << endl;
	exit(EXIT_ERROR);
	#endif
	#else
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	cerr << "SANIpropagateCompactClass::propagateWordThroughNetworkGroupInit error: !SANI_SEQUENCE_GRAMMAR_INPUT_DYNAMIC requires SANI_SEQUENCE_GRAMMAR_INPUT_POS" << endl;
	exit(EXIT_ERROR);
	#else //SANI_SEQUENCE_GRAMMAR_INPUT_POS
	SANIGroupNeuralNetwork* inputLayerGroup = SANIformation.getInputGroupLayerSection(SANIformation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);		
	#endif
	#endif
		
	forwardPropogationSequenceElementData->wordPOStype = wordPOStype;
	/*
	//moved to SANIformation: @SANI1p6a
	#ifdef SANI_SEMANTIC_NETWORK
	inputLayerGroup->sequenceElementObject = forwardPropogationSequenceElementData;
	#else
	inputLayerGroup->wordPOStype = wordPOStype;
	#endif
	*/
	
	#ifdef SANI_DEBUG_PROPAGATE
	cout << "SANIpropagateCompactClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif
	
	if(getFirstLayer)
	{
		//cout << "SANIpropagateCompactClass::propagateWordThroughNetworkIntro: getFirstLayer, wordPOStype = " << wordPOStype << ", forwardPropogationSequenceElementData->sequenceIndex = " << forwardPropogationSequenceElementData->sequenceIndex << endl;
		if(!propagateWordThroughNetworkGroupAddToFirstLayer(translatorVariables, forwardPropogationSequenceElementData->sequenceIndex, inputLayerGroup, forwardPropogationSequenceData))
		{
			result = false;
		}	
	}
	else
	{
		if(!propagateWordThroughNetworkGroupInit2(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData))
		{
			result = false;
		}
	}
		
	return result;
}

bool SANIpropagateCompactClass::propagateWordThroughNetworkGroupAddToFirstLayer(SANItranslatorVariablesClass* translatorVariables, int sequenceIndex, SANIGroupNeuralNetwork* inputLayerGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	bool result = true;	
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_SIMULTANEOUSLY_PROPAGATE_UNAMBIGUOUS_POS_PERMUTATIONS
	(*forwardPropogationSequenceData->firstLayer)[sequenceIndex] = inputLayerGroup;	//cannot assume that propagateWordThroughNetworkGroupAddToFirstLayer will be executed via defineFirstLayer at start of generation
	#else
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	//OLD2: forwardPropogationSequenceData->firstLayer->insert(forwardPropogationSequenceData->firstLayer->begin()+sequenceIndex, inputLayerGroup);	//will only be valid for pos unambiguous sequenceElements	//CHECKTHIS - only work if neuron is inserted in order
	//OLD1: (forwardPropogationSequenceData->firstLayer->at(sequenceIndex))->push_back(inputLayerGroup);	//will only be valid for pos unambiguous sequenceElements
	forwardPropogationSequenceData->firstLayer->push_back(inputLayerGroup);	//CHECKTHIS
	#else
	forwardPropogationSequenceData->firstLayer->push_back(inputLayerGroup);
	#endif
	#endif
		
	return result;
}

bool SANIpropagateCompactClass::propagateWordThroughNetworkGroupInit2(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* inputLayerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	bool result = true;	

	#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION_INPUT_NEURONS
	inputLayerGroup->neuronActive = true;
	inputLayerGroup->neuronReference->activationLevel = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_FULL;
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_PARSE_TREE_SAVE_LEAF_NODES	//SANI_PARSE
	SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup = inputLayerGroup->currentParseTreeGroupTemp;	//if(inputLayerGroup->artificialInputNeuronLinkPosAmbiguousPermuations), inputLayerGroup->currentParseTreeGroupTemp will not have been defined
	#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_PARSE_TREE_SAVE_LEAF_NODES
	cout << "DEBUG: inputLayerGroup->currentParseTreeGroupTemp->groupName = " << inputLayerGroup->currentParseTreeGroupTemp->groupName << endl;	//tests whether currentParseTreeGroupTemp object has been generated
	#endif
	#else
	SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup = NULL;	//not properly used by SANI_SEQUENCE_GRAMMAR
	#endif
	int layer = 0;
	propagateWordThroughNetworkGroup(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup);

	#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION_INPUT_NEURONS
	inputLayerGroup->neuronActive = false;
	inputLayerGroup->neuronReference->activationLevel = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_INACTIVE;
	#endif
			
	return result;
}	

bool SANIpropagateCompactClass::propagateWordThroughNetworkGroup(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_PROPAGATE
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << "group->groupName << group->groupIndex = " << group->groupIndex << " \e[0m" << endl;
	#endif
			
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops	//only required for recursion (not yet supported by SANI_SEQUENCE_GRAMMAR)
	{
		group->neuronPropagated = true;
	#endif
		
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup))
		{
			result = true;
		}
		
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	#endif
	
	return result;
}
		
bool SANIpropagateCompactClass::propagateWordThroughNetworkGroupSelect(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup)
{
	bool result = false;

	//now search for existing sequence in network
	
	for(int i=0; i<group->SANIfrontComponentConnectionList.size(); i++)
	{
		#ifndef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		if(!(forwardPropogationSequenceData->finishedPassingSequenceElements))
		{
		#endif
			SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i];
			SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
	
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
			if((forwardPropogationSequenceData->findingCandidateComponent2))
			{
				if(!(forwardPropogationSequenceData->foundCandidateComponent2))
				{
					if(currentComponent == forwardPropogationSequenceData->candidateComponent2ToFind)
					{
						forwardPropogationSequenceData->foundCandidateComponent2 = true;
						forwardPropogationSequenceData->candidateComponent2sourceParseTreeGroup = activationPathSequenceElementCurrentParseTreeGroup;
					}
				}
			}
			#endif
		
			int componentIndex = currentComponent->componentIndex;	
			
			#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			if(!ownerGroup->neuronPropagated)	//prevent circular loops
			{
			#endif	
				#ifdef SANI_DEBUG_PROPAGATE_EXTRA2
				SANInodes.printParseTreeDebugIndentation(layer+1);	
				cout << "2a: propagateWordThroughNetworkGroup: ownerGroup->groupIndex = " <<  ownerGroup->groupIndex << ", currentComponent->componentIndex = " << currentComponent->componentIndex << endl;
				#endif
				
				if(group->inputLayerNeuron)	//if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					currentComponent->candidateStringMatch = forwardPropogationSequenceElementData->wordReference;
				}
	
				bool sequentialActivationFound = false;
				bool existingActivationFoundStartComponent = false;
				if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup, i, &sequentialActivationFound, &existingActivationFoundStartComponent))
				{
					result = true;
				}

			#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			}
			#endif
		#ifndef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		}
		#endif
	}
	
	return result;
}	
	


bool SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFoundAndExistingActivationFound = false;
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "3_: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", ownerGroup->groupIndex: " << ownerGroup->groupIndex << ", currentComponent->componentIndex = " << currentComponent->componentIndex << endl;	
	#endif
	
	SANIComponentNeuralNetwork* previousActiveComponent = NULL;	
	SANIComponentNeuralNetwork* finalActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;	//NOTUSED
	bool missingStartComponentFound = false;
	bool missingOrVariableStartComponentFound = false;
	//#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
	bool missingOrVariableEndComponentFound = false;
        //#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_LAST_COMPONENTS_REMEMBER_FIRST_COMPONENT_SEQUENCE_INDEX
	bool existingActivationFoundEndComponent = false;
	//#endif
	//#endif
	bool variableStartComponentFound = false;	//temp debug only
	int numberOfInactiveComponentsRemaining = 0;
        	
	bool componentWordConnectivityTestsPrelimResult = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	if(SANIpropagateOperations.componentWordConnectivityTestsWrapper(forwardPropogationSequenceElementData, forwardPropogationSequenceData, ownerGroup, activationPathSequenceElementCurrentParseTreeGroup, false))
	{
		componentWordConnectivityTestsPrelimResult = true;
	}
	#endif
	
	if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, currentComponent, &(ownerGroup->components), activationPathSequenceElementCurrentParseTreeGroup, &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &finalActiveComponent, existingActivationFoundStartComponent, &existingActivationFoundEndComponent, &missingStartComponentFound, &missingOrVariableStartComponentFound, &missingOrVariableEndComponentFound, &numberOfInactiveComponentsRemaining, ownerGroup, componentWordConnectivityTestsPrelimResult))
	{	
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA3_PRIMARY
		SANInodes.printParseTreeDebugIndentation(layer+1);
		cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << " - missingStartComponentFound = " << missingStartComponentFound << ", missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << "ownerGroup->currentParseTreeGroupTemp->components.size() = " << ownerGroup->currentParseTreeGroupTemp->components.size() << endl;
		#endif
		
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
		SANIGroupParseTree* currentParseTreeGroupBackup = ownerGroup->currentParseTreeGroupTemp;
		SANIForwardPropogationSequenceElementData* previousActiveComponentActiveSequenceElementRecordBackup = NULL;
		if(previousActiveComponent != NULL)
		{
			previousActiveComponentActiveSequenceElementRecordBackup = previousActiveComponent->neuronComponentConnectionActiveSequenceElementRecord;
		}
		#endif
		
		//execute sequentialActivationConnectivityTests at start to save multiple executions;
		bool componentWordConnectivityTests = false;
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFIED	//or SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
		bool skipWordConnectivityTests = missingOrVariableStartComponentFound;
		#else
		bool skipWordConnectivityTests = missingStartComponentFound;
		#endif
		#else
		bool skipWordConnectivityTests = false;
		#endif
		if(sequentialActivationConnectivityTests(translatorVariables, currentComponent, ownerGroup, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, activationSequenceCompleted, layer, activationPathSequenceElementCurrentParseTreeGroup, *existingActivationFoundStartComponent, existingActivationFoundEndComponent, skipWordConnectivityTests))
		{
			componentWordConnectivityTests = true;
			*sequentialActivationFound = true;
		}
	
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_LAST_COMPONENTS
		SANIpropagateCompactGenerateOperations.propagateVariableEndComponentRemoveLastParseTreeComponent(translatorVariables, layer, ownerGroup, currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, &activationSequenceCompleted, previousActiveComponent, finalActiveComponent, &existingActivationFoundEndComponent, componentWordConnectivityTests, sequentialActivationFound, activationPathSequenceElementCurrentParseTreeGroup);
		#endif		
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		SANIpropagateCompactGenerateOperations.verifyMissingOrVariableStartComponentFound(translatorVariables, layer, ownerGroup, currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, &activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, existingActivationFoundStartComponent, &missingStartComponentFound, &variableStartComponentFound, componentWordConnectivityTests, &missingOrVariableStartComponentFound, numberOfInactiveComponentsRemaining, sequentialActivationFound, activationPathSequenceElementCurrentParseTreeGroup);
		SANIpropagateCompactGenerateOperations.verifyMissingOrVariableStartComponentFoundAllowedWordIndices(translatorVariables, layer, ownerGroup, currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, &activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, existingActivationFoundStartComponent, &missingStartComponentFound, &variableStartComponentFound, componentWordConnectivityTests, &missingOrVariableStartComponentFound, numberOfInactiveComponentsRemaining, sequentialActivationFound, activationPathSequenceElementCurrentParseTreeGroup);
		#endif
				
		if(*sequentialActivationFound)
		{						
			#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
			SANInodes.printParseTreeDebugIndentation(layer+1);
			cout << "3c2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", missingStartComponentFound = " << missingStartComponentFound << ", missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << ", variableStartComponentFound = " << variableStartComponentFound << endl;	
			#endif
		
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFY2
			bool missingOrVariableStartComponentFoundStored = missingOrVariableStartComponentFound;
			#else
			bool missingOrVariableStartComponentFoundStored = false;
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
			if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)
			{
				missingOrVariableStartComponentFoundStored = ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound;	//CHECKTHIS; why not use bool missingOrVariableStartComponentFound
			}
			#endif
			#endif
			if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, activationSequenceCompleted, layer, activationPathSequenceElementCurrentParseTreeGroup, groupFrontComponentConnectionListIndex, *existingActivationFoundStartComponent, missingStartComponentFound, missingOrVariableStartComponentFoundStored))
			{
				result = true;
			}
		}
		else
		{
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
			ownerGroup->currentParseTreeGroupTemp = currentParseTreeGroupBackup;
			if(previousActiveComponent != NULL)
			{
				previousActiveComponent->neuronComponentConnectionActiveSequenceElementRecord = previousActiveComponentActiveSequenceElementRecordBackup;
			}
			#endif
		}
	}

	SANIpropagateCompactGenerateOperations.updateActivatedNeuronWithMaxSequenceIndexCoverageWrapper(ownerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, activationSequenceCompleted, *sequentialActivationFound, missingStartComponentFound, missingOrVariableStartComponentFound, missingOrVariableEndComponentFound);

	return result;
}
	
bool SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponent(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool activationSequenceCompleted, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFoundStartComponent, bool missingStartComponentFound, bool missingOrVariableStartComponentFound)
{
	bool result = true;
		
	if(existingActivationFoundStartComponent)
	{		
		#ifdef SANI_DEBUG_PROPAGATE
		cout << "******** SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponent: resetGroupParseTreeGroupRef, groupIndex = " << ownerGroup->groupIndex << endl;
		#endif
		ownerGroup->neuronActive = false;	//required for SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION
		SANIpropagateOperations.resetGroupActivation(ownerGroup);
		SANIpropagateOperationsParseTree.resetGroupParseTreeGroupRef(ownerGroup, false);
		#ifdef SANI_FREE_MEMORY
		(forwardPropogationSequenceData->parseTreeGroupListPropagate).push_back(ownerGroup->currentParseTreeGroupTemp);
		#endif	
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
		ownerGroup->parseTreeGroupMemory.push_back(ownerGroup->currentParseTreeGroupTemp);
		#endif
	}
	
	#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
	//if(!(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)) {
	currentComponent->componentStrength = currentComponent->componentStrength + 1;
	//}
	#endif
	currentComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActiveSequenceElementRecord = forwardPropogationSequenceElementData;
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	currentComponent->neuronComponentConnectionActiveSequenceElementRecordMemory.push_back(currentComponent->neuronComponentConnectionActiveSequenceElementRecord);	//note neuronComponentConnectionActiveSequenceElementRecordMemory is stored in currentComponent not newParseComponent such that it can be retained across executions of resetGroupParseTreeGroupRef(ownerGroup, false)
	#endif
	
	if(forwardPropogationSequenceElementData->sequenceIndex < 0)
	{
		cout << "SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponent error: forwardPropogationSequenceElementData->sequenceIndex = " << forwardPropogationSequenceElementData->sequenceIndex << endl;
		exit(EXIT_ERROR);
	}
	
	#ifdef SANI_PARSE
	//create new parseTreeGroup
	//copied from SANIpropagateInverseClass::generateRulesGroupTreeComponents;	
	SANIComponentParseTree* newParseComponent = SANInodes.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new SANIComponentParseTree(*SANInodes.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP
	if(SANInodes.hasComponentTypeString(currentComponent))	//OR if(group->inputLayerNeuron)
	{
		newParseComponent->parseTreeComponentTypeString = true;
	}
	#endif
	newParseComponent->componentRef = currentComponent;
	newParseComponent->neuronComponentConnectionActive = true;	//implied
	newParseComponent->neuronComponentConnectionActiveSequenceElementRecord = currentComponent->neuronComponentConnectionActiveSequenceElementRecord;
	#ifdef SANI_PARSE_GENERATE_PARSE_TREE
	newParseComponent->parseTreeGroupRef = activationPathSequenceElementCurrentParseTreeGroup;
	#endif
	#ifdef SANI_PARSE_RECORD_COMPONENT_SEQUENCE_INDEX
	newParseComponent->sequenceIndex = forwardPropogationSequenceElementData->sequenceIndex;
	#endif
	newParseComponent->wordPOStypeInferred = forwardPropogationSequenceElementData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
						
	SANIGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;

	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		//currentParseTreeGroupTemp->components.push_front(newParseComponent);
		currentParseTreeGroupTemp->components.insert(currentParseTreeGroupTemp->components.begin(), newParseComponent);
	}
	else
	{
		currentParseTreeGroupTemp->components.push_back(newParseComponent);
	}
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION
	if(!(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage))
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION_RESTRICT_TO_POST_GENERATION_PHASE
		if(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal > translatorVariables->maxNumberSentences/2)
		{
		#endif
			printSANInetworkSVG(translatorVariables, forwardPropogationSequenceData->SANIGroupTypes);
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION_RESTRICT_TO_POST_GENERATION_PHASE
		}
		#endif
	}
	#endif
	
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	if(currentParseTreeGroupTemp->parseTreeMaxSequenceIndex == INT_DEFAULT_VALUE)
	{
		currentParseTreeGroupTemp->parseTreeMaxSequenceIndex = currentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
		currentParseTreeGroupTemp->parseTreeMinSequenceIndex = currentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		currentParseTreeGroupTemp->parseTreeMaxWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeMinWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeTotalWeight = ownerGroup->groupWeight;
		#endif
	}
	SANIpropagateOperationsParseTree.updateParseTreeMaxMinSequenceIndexOfParent(currentParseTreeGroupTemp, newParseComponent, activationPathSequenceElementCurrentParseTreeGroup);
	#endif
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA4
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << ", newParseComponent->componentIndex = " << newParseComponent->componentIndex << ", ownerGroup->components.size() = " << ownerGroup->components.size() << ", groupIndex = " << ownerGroup->groupIndex << ", existingActivationFoundStartComponent = " << existingActivationFoundStartComponent << ", activationSequenceCompleted = " << activationSequenceCompleted << endl; 
	#endif
		
	#ifdef SANI_PARSE
	SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroupTemp;
	#endif
	

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS
	verifySequenceIndexCoverageIntegrity(forwardPropogationSequenceData, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSequenceElementData);
	#endif

	if(activationSequenceCompleted)
	{			
		//cout << "activationSequenceCompleted" << endl;
		
		ownerGroup->neuronActive = true;	//required for SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION
		
		#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_ALL_PROPAGATED_NEURONS
		if(forwardPropogationSequenceData->updateNeuronRecency)
		{
			ownerGroup->timeIndex = translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal;	
		}
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
		//if(!(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)) {
		ownerGroup->groupStrength = ownerGroup->groupStrength + 1;
		//}
		#endif
	
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA
		SANInodes.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif
		
		SANIForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = forwardPropogationSignalData;
		
		bool candidateCoveragePartial = false;	
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_SEQUENCE_INDEX_COVERAGE
		cout << "activationSequenceCompleted" << endl;
		#endif
		if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)
		{
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_SEQUENCE_INDEX_COVERAGE
			cout << "(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)" << endl;
			#endif
			//consider relying on: activationPathWordCurrentParseTreeGroupOwner->missingOrVariableStartComponentFound/missingStartComponentFound, rather than passing missingOrVariableStartComponentFound/missingStartComponentFound
			SANIpropagateCompactGenerateOperations.updateActivatedNeuronWithMaxSequenceIndexCoverage(ownerGroup, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, missingStartComponentFound, missingOrVariableStartComponentFound, false, candidateCoveragePartial);
		}
		
			
		bool topLevelGroup = false;	
		#ifdef SANI_PREVENT_RECORDING_TOP_LEVEL_NEURON_IF_RECORDING_MAX_COVERAGE_NEURON
		if(!(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage))
		{			
		#endif
			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
			int maxWeight;
			SANIpropagateOperationsParseTree.calculatePerformanceWeightOfParseTree(activationPathWordCurrentParseTreeGroupOwner, &maxWeight);
			if(maxWeight >= forwardPropogationSequenceData->parseTreeMaxWeightPropagate)
			{
			#endif				
				int wComparisonInt;
				if(forwardPropogationSequenceData->parseSentenceReverse)
				{
					wComparisonInt = 0;	//start of sentence found
				}
				else
				{
					wComparisonInt = forwardPropogationSequenceData->sentenceContents->size()-1;	//end of sentence punctuation mark found
				}
				if(forwardPropogationSequenceElementData->sequenceIndex == wComparisonInt)	//start of sentence found
				{
					int topLevelParseTreeGroupSequenceIndexCoverage = SANInodes.calculateCoverage(activationPathWordCurrentParseTreeGroupOwner);

					if(topLevelParseTreeGroupSequenceIndexCoverage == forwardPropogationSequenceData->sentenceContents->size())
					{
						#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_ONLY_ONE_TOP_LEVEL_NEURON_FOUND_PER_SENTENCE
						if(forwardPropogationSequenceData->toplevelGroupActivationFound)
						{
							cerr << "SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponent - topLevelGroup && already found (forwardPropogationSequenceData->toplevelGroupActivationFound)" << endl;
							exit(EXIT_ERROR);
						}
						else
						{
						#endif
							#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_TOP_LEVEL_NEURON_FOUND
							if(ownerGroup->topLevelSentenceNeuron)
							{
							#endif
							
								bool sequenceElementConnectivityVerification = SANIpropagateOperationsParseTree.verifyPerformanceGroupSentence(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSequenceData, layer);
								#ifndef SANI_SEQUENCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFICATION_DO_NOT_STOP
								#ifdef SANI_SEQUENCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFICATION
								if(!sequenceElementConnectivityVerification)
								{
									cerr << "SANI_SEQUENCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFICATION; SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponent error: !SANIpropagateOperationsParse.updatePerformance" << endl;
									cerr << "forwardPropogationSequenceData->performance = " << forwardPropogationSequenceData->performance << endl;
									cerr << "forwardPropogationSequenceData->sentenceContents->size() = " << forwardPropogationSequenceData->sentenceContents->size() << endl;
									SANInodes.printParseTree(activationPathWordCurrentParseTreeGroupOwner, 0);
									exit(EXIT_ERROR);
								}
								#endif
								#endif
								#ifdef SANI_SEQUENCE_GRAMMAR_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_AFTER_SIMULTANEOUS_POS_PROPAGATION
								if(sequenceElementConnectivityVerification)
								{
								#endif
																
									#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
									int leafSize = SANInodes.countParseTreeLeafSize(activationPathWordCurrentParseTreeGroupOwner);
									int maxLeafSize = 0;
									int maxDepth = 0;
									#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
									SANInodes.getNeuralNetworkDepth(ownerGroup, &maxDepth);
									#else
									SANIpropagateOperations.countNeuralNetworkMaxLeafSizeAndDepth(ownerGroup, &maxLeafSize, &maxDepth);
									#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
									SANIpropagateOperations.countNeuralNetworkMaxLeafSizeAndDepthReset(ownerGroup);
									#endif
									#endif
									cout << "topLevelGroup" << endl;
									cout << "\tparseTree leafSize = " << leafSize << endl;
									cout << "\tneuralNetwork maxLeafSize = " << maxLeafSize << endl;
									cout << "\tneuralNetwork maxDepth = " << maxDepth << endl;
									#endif
									#ifdef SANI_DEBUG_RULES_PRINT_PARSE_PROCESS_POS_TYPES
									cout << "topLevelGroup" << endl;
									#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
									cout << "parseTreeMaxWeight1 = " << forwardPropogationSequenceData->parseTreeMaxWeightPropagate << endl;
									#endif
									cout << "forwardPropogationSequenceData->topLevelParseTreeGroupPropagate->groupName = " << activationPathWordCurrentParseTreeGroupOwner->groupName << endl;
									printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 3);
									#endif

									topLevelGroup = true;

									forwardPropogationSequenceData->finishedPassingSequenceElements = true;

									forwardPropogationSequenceData->toplevelGroupActivationFound = true;

									forwardPropogationSequenceData->parseTreeMaxWeightPropagate = maxWeight;

									#ifdef SANI_PARSE_SAVE_PARSE_TREE
									#ifdef SANI_REPLICATE_TOP_LEVEL_PARSE_TREE
									SANInodes.deleteParseTree(forwardPropogationSequenceData->topLevelParseTreeGroupPropagate, 0);
									forwardPropogationSequenceData->topLevelParseTreeGroupPropagate = SANInodes.replicateParseTree(activationPathWordCurrentParseTreeGroupOwner, 0);
									#ifdef SANI_DEBUG_RULES_PRINT_PARSE_PROCESS_POS_TYPES
									//printBackpropParseTree(forwardPropogationSequenceData->topLevelParseTreeGroupPropagate, 3);
									#endif
									#else
									//copy currentParseTreeGroupTemp so it cant be overwritten;
									forwardPropogationSequenceData->topLevelParseTreeGroupPropagate = SANInodes.copyGroup(activationPathWordCurrentParseTreeGroupOwner);
									#endif
									#endif
								#ifdef SANI_SEQUENCE_GRAMMAR_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_AFTER_SIMULTANEOUS_POS_PROPAGATION
								}
								#endif
							#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_TOP_LEVEL_NEURON_FOUND
							}
							#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_AFTER_SIMULTANEOUS_POS_PROPAGATION
						}
						#endif
					}
				}

			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
			}
			#endif
		#ifdef SANI_PREVENT_RECORDING_TOP_LEVEL_NEURON_IF_RECORDING_MAX_COVERAGE_NEURON
		}
		#endif
				
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		if(!missingOrVariableStartComponentFound)	//prevents propagation up if variableStartComponentFound
		{
		#endif	

			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
			if(activationPathWordCurrentParseTreeGroupOwner->components.size() == 1)
			{
				cout << "SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponent: activationSequenceCompleted; activationPathWordCurrentParseTreeGroupOwner->components.size() == 1" << endl;
				cout << "ownerGroup->components.size() = " << ownerGroup->components.size() << endl;
				exit(EXIT_ERROR);
			}
			#endif

			if(!topLevelGroup)
			{
				#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
				ownerGroup->neuronPropagated = true;
				#endif
								
				//activation sequence completed (and not top level group), propagate next layer up
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationSequenceElementData, forwardPropogationSequenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))
				{
					//result = true;
				}

				#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
				ownerGroup->neuronPropagated = false;
				#endif
			}
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		}
		#endif
	}

	return result;
}
	
bool SANIpropagateCompactClass::sequentialActivationConnectivityTests(const SANItranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool activationSequenceCompleted, const int layer, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, bool existingActivationFoundStartComponent, const bool existingActivationFoundEndComponent, const bool skipWordConnectivityTests)
{	
	bool sequentialActivationFound = false;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
	if(upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponentWrapper(forwardPropogationSequenceData, forwardPropogationSignalData, forwardPropogationSequenceElementData, currentComponent, ownerGroup, activationPathSequenceElementCurrentParseTreeGroup, existingActivationFoundStartComponent))
	{
	#endif
		bool passBasicWordConnectivityTest = true;
		if(!skipWordConnectivityTests)
		{
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS		
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS2
			if(currentComponent != SANInodes.getFirstComponent(forwardPropogationSequenceData, ownerGroup, true))
			{
			#endif
				if(!existingActivationFoundStartComponent)
				{
					bool groupHasPreceedingComponent = false;
					if(currentParseTreeGroupTemp->components.size() > 0)
					{
						groupHasPreceedingComponent = true;
					}
					if(groupHasPreceedingComponent)
					{
			#endif
						passBasicWordConnectivityTest = false;
						if(SANIpropagateOperations.componentWordConnectivityTestsWrapper(forwardPropogationSequenceElementData, forwardPropogationSequenceData, ownerGroup, currentParseTreeGroupTemp, activationPathSequenceElementCurrentParseTreeGroup, existingActivationFoundStartComponent, existingActivationFoundEndComponent))
						{
							//cout << "passBasicWordConnectivityTest" << endl;
							passBasicWordConnectivityTest = true;
						}
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS			
					}
				}
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS2
			}
			#endif
			#endif
			#endif
		}
		
		if(passBasicWordConnectivityTest)
		{			
			/*
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
			Note if variableStartComponentFound, then componentWordConnectivityTestsWrapper is not required to be reexecuted, since if the previous component was variable (ie miscellaneously activated) and components->size()==1, no sequenceElement index connectivity test is required to be conducted. 
			If the first component was variable, but components->size() >= 2, then the previous component would be valid and would already have been tested when componentWordConnectivityTestsWrapper was executed the first time
			#endif
			*/
		
			sequentialActivationFound = true;

		}
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
	}
	#endif	
	
	return sequentialActivationFound;
}	

bool SANIpropagateCompactClass::printBackpropParseTree(SANIGroupParseTree* group, const int level)
{
	#ifdef SANI_DEBUG_PARSE_TREE_PRINT_SUPPORT_RECURSION
	SANIpropagateOperationsParseTree.traceBackpropParseTreePrint(group, 1);
	SANIpropagateOperationsParseTree.resetNeuronBackprop(group, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);	
	#else
	SANInodes.printParseTree(group, level);
	#endif
}



bool SANIpropagateCompactClass::identifyComponentIndexLastActivatedIndex(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex)
{
	bool result = false;
	int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
	SANIGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		result = true;
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			*lastActivatedIndex = groupOrig->components.size()-numberOfActivatedComponents;
		}
		else
		{
			*lastActivatedIndex = numberOfActivatedComponents-1;
		}
	}
	return result;
}



#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
bool SANIpropagateCompactClass::upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponentWrapper(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIComponentNeuralNetwork* component, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, bool existingActivationFoundStartComponent)
{
	bool result = false;
	
	SANIGroupParseTree* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
	if((ownerGroupParseTreeGroup->components.size() == 0) || existingActivationFoundStartComponent)
	{		
		int firstSequenceIndexInSequence;
		//expect to find upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent if current group->parseTreeMinSequenceIndex/parseTreeMaxSequenceIndex doesnt encapsulate firstSequenceIndexInSequence

		int sequenceIndexMax = -1;
		int sequenceIndexMin = REALLY_LARGE_INT;
		const SANIGroupParseTree* parseTreeGroupToFindSequenceIndexMin;
		const SANIGroupParseTree* parseTreeGroupToFindSequenceIndexMax;
		int sequenceIndexMinMin;
		int sequenceIndexMaxMax;
		int sequenceIndexMinToFindAbove;		
		int sequenceIndexMaxToFindAbove;
		int sequenceIndexLastAboveEffectiveAfterGroupReset = INT_DEFAULT_VALUE;

		bool execute = false;
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "\nSANIpropagateCompactClass::upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponentWrapper start:" << endl;
			#endif
			
			#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			firstSequenceIndexInSequence = forwardPropogationSequenceData->sentenceContents->size()-1-forwardPropogationSignalData->firstIndexInSequence;
			#else
			firstSequenceIndexInSequence = forwardPropogationSequenceData->sentenceContents->size()-1;
			#endif

			parseTreeGroupToFindSequenceIndexMax = activationPathSequenceElementCurrentParseTreeGroup;
			sequenceIndexMaxMax = forwardPropogationSequenceElementData->sequenceIndex;	//forwardPropogationSequenceElementData->sequenceIndex;
			if(parseTreeGroupToFindSequenceIndexMax != NULL)
			{
				if(!SANIpropagateOperationsParseTree.getMinMaxSequenceIndexInParseTree(parseTreeGroupToFindSequenceIndexMax, true, &sequenceIndexMax, 0))
				{

				}
			}
			
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "sequenceIndexMaxMax = " << sequenceIndexMaxMax << endl;
			cout << "sequenceIndexMax = " << sequenceIndexMax << endl;
			#endif
			
			sequenceIndexMax = SHAREDvars.maxInt(sequenceIndexMax, sequenceIndexMaxMax);
			
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "sequenceIndexMax (maxInt) = " << sequenceIndexMax << endl;
			#endif
			
			if(sequenceIndexMax < firstSequenceIndexInSequence)
			{
				sequenceIndexMaxToFindAbove = firstSequenceIndexInSequence;
				sequenceIndexMinToFindAbove = sequenceIndexMax+1;
			
				//need to disregard sequenceIndexMin/Max of last activated component in uppergroups (assuming it was effectively activated by current component)
				if(existingActivationFoundStartComponent)	//ie ownerGroupParseTreeGroup != NULL
				{
					sequenceIndexLastAboveEffectiveAfterGroupReset = ownerGroupParseTreeGroup->parseTreeMaxSequenceIndex+1;//CHECKTHIS; ??
				}
				
				execute = true;
			}
		}
		else
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			firstSequenceIndexInSequence = forwardPropogationSignalData->firstIndexInSequence;
			#else
			firstSequenceIndexInSequence = 0;
			#endif

			parseTreeGroupToFindSequenceIndexMin = activationPathSequenceElementCurrentParseTreeGroup;
			sequenceIndexMinMin = forwardPropogationSequenceElementData->sequenceIndex;	//forwardPropogationSequenceElementData->sequenceIndex;
			if(parseTreeGroupToFindSequenceIndexMin != NULL)
			{
				if(!SANIpropagateOperationsParseTree.getMinMaxSequenceIndexInParseTree(parseTreeGroupToFindSequenceIndexMin, false, &sequenceIndexMin, 0))
				{

				}
			}
			sequenceIndexMin = SHAREDvars.minInt(sequenceIndexMin, sequenceIndexMinMin);
			if(sequenceIndexMin > firstSequenceIndexInSequence)
			{
				sequenceIndexMinToFindAbove = firstSequenceIndexInSequence;
				sequenceIndexMaxToFindAbove = sequenceIndexMin-1;
			
				//need to disregard sequenceIndexMin/Max of last activated component in uppergroups (assuming it was effectively activated by current component)
				if(existingActivationFoundStartComponent)	//ie ownerGroupParseTreeGroup != NULL
				{
					sequenceIndexLastAboveEffectiveAfterGroupReset = ownerGroupParseTreeGroup->parseTreeMinSequenceIndex-1;	
				}
				
				execute = true;
			}
		}	
		
		if(execute)
		{
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "sequenceIndexMaxToFindAbove = " << sequenceIndexMaxToFindAbove << endl;
			cout << "sequenceIndexMinToFindAbove = " << sequenceIndexMinToFindAbove << endl;
			cout << "sequenceIndexLastAboveEffectiveAfterGroupReset = " << sequenceIndexLastAboveEffectiveAfterGroupReset << endl;
			#endif
				
			if(upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent(forwardPropogationSequenceData, sequenceIndexMinToFindAbove, sequenceIndexMaxToFindAbove, sequenceIndexLastAboveEffectiveAfterGroupReset, 0, ownerGroup, false, false))
			{
				result = true;
			}
			else
			{
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
				cout << "!upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent" << endl;
				#endif
			}
		}
		else
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}
						
	return result;
}

//bool SANIpropagateOperationsClass::upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, bool findMaxOrMinSequenceIndex, int sequenceIndexMaxOrMinToFind, int level, SANIGroupNeuralNetwork* group)
bool SANIpropagateCompactClass::upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int sequenceIndexMinToFindAbove, const int sequenceIndexMaxToFindAbove, const int sequenceIndexLastAboveEffectiveAfterGroupReset, int level, const SANIGroupNeuralNetwork* group, bool sequenceIndexMinFound, bool sequenceIndexMaxFound)
{
	bool result = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
	bool keepExecuting = true;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
	sequenceIndexMinFound = false;
	sequenceIndexMaxFound = false;
	#endif
	
	for(int i=0; i<group->SANIfrontComponentConnectionList.size(); i++)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
		if(keepExecuting)
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
			bool keepExecutingLocal = true;
			#endif
		#endif
			
			SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i];
			SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;

			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			SANInodes.printParseTreeDebugIndentation(level);
			cout << "ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
			#endif

			SANIGroupParseTree* ownerGroupParseTree = ownerGroup->currentParseTreeGroupTemp;
			int parseTreeMinSequenceIndexEffectiveAfterGroupReset = ownerGroupParseTree->parseTreeMinSequenceIndex;
			int parseTreeMaxSequenceIndexEffectiveAfterGroupReset = ownerGroupParseTree->parseTreeMaxSequenceIndex;

			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			SANInodes.printParseTreeDebugIndentation(level);
			cout << "parseTreeMinSequenceIndex = " << parseTreeMinSequenceIndexEffectiveAfterGroupReset << endl;
			SANInodes.printParseTreeDebugIndentation(level);
			cout << "parseTreeMaxSequenceIndex = " << parseTreeMaxSequenceIndexEffectiveAfterGroupReset << endl;
			#endif


			#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
			bool groupHasPreceedingComponent = false;
			if(forwardPropogationSequenceData->parseSentenceReverse)
			{
				if(currentComponent->componentIndex < ownerGroup->components.size()-1)
				{
					groupHasPreceedingComponent = true;
				}
			}
			else
			{
				 if(currentComponent->componentIndex > 0)
				 {
			 		groupHasPreceedingComponent = true;
				 }
			}
			#endif

			if(currentComponent->neuronComponentConnectionActive)
			{//currentComponent was activated by the group being reset - so determine the ownerGroup's expected [older] parseTreeMinSequenceIndex/parseTreeMaxSequenceIndex values assuming the group is reset
				if(sequenceIndexLastAboveEffectiveAfterGroupReset != INT_DEFAULT_VALUE)
				{
					if(forwardPropogationSequenceData->parseSentenceReverse)
					{
						if(parseTreeMinSequenceIndexEffectiveAfterGroupReset < sequenceIndexLastAboveEffectiveAfterGroupReset)
						{
							parseTreeMinSequenceIndexEffectiveAfterGroupReset = sequenceIndexLastAboveEffectiveAfterGroupReset;
						}
					}
					else
					{
						if(parseTreeMaxSequenceIndexEffectiveAfterGroupReset > sequenceIndexLastAboveEffectiveAfterGroupReset)
						{
							parseTreeMaxSequenceIndexEffectiveAfterGroupReset = sequenceIndexLastAboveEffectiveAfterGroupReset;
						}
					}
				}
			}

			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			SANInodes.printParseTreeDebugIndentation(level);
			cout << "parseTreeMinSequenceIndexEffectiveAfterGroupReset = " << parseTreeMinSequenceIndexEffectiveAfterGroupReset << endl;
			SANInodes.printParseTreeDebugIndentation(level);
			cout << "parseTreeMaxSequenceIndexEffectiveAfterGroupReset = " << parseTreeMaxSequenceIndexEffectiveAfterGroupReset << endl;
			#endif

			if(forwardPropogationSequenceData->parseSentenceReverse)
			{
				//assume will find sequenceIndexMin match before sequenceIndexMax match when propagating up network - CHECKTHIS
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				if(!sequenceIndexMinFound)
				{
				#endif
					if(parseTreeMinSequenceIndexEffectiveAfterGroupReset == sequenceIndexMinToFindAbove)
					{
						sequenceIndexMinFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIpropagateOperationsClass::upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent - sequenceIndexMinFound" << endl;
						#endif
					}
					#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
					else
					{
						if(groupHasPreceedingComponent)
						{
							keepExecutingLocal = false;
						}
					}
					#endif
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				}
				if(sequenceIndexMinFound)
				{
				#endif
					if(parseTreeMaxSequenceIndexEffectiveAfterGroupReset == sequenceIndexMaxToFindAbove)
					{
						sequenceIndexMaxFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIpropagateOperationsClass::upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent - sequenceIndexMaxFound" << endl;
						#endif
					}
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				}
				#endif
			}
			else
			{
				//assume will find sequenceIndexMax match before sequenceIndexMin match when propagating up network - CHECKTHIS
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				if(!sequenceIndexMaxFound)
				{
				#endif
					if(parseTreeMaxSequenceIndexEffectiveAfterGroupReset == sequenceIndexMaxToFindAbove)
					{
						sequenceIndexMaxFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIpropagateOperationsClass::upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent - sequenceIndexMaxFound" << endl;
						#endif
					}
					#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
					else
					{
						if(groupHasPreceedingComponent)
						{
							keepExecutingLocal = false;
						}
					}
					#endif
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST					
				}
				if(sequenceIndexMaxFound)
				{
				#endif
					if(parseTreeMinSequenceIndexEffectiveAfterGroupReset == sequenceIndexMinToFindAbove)
					{
						sequenceIndexMinFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIpropagateOperationsClass::upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent - sequenceIndexMinFound" << endl;
						#endif
					}
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST					
				}
				#endif
			}


			if(sequenceIndexMinFound && sequenceIndexMaxFound)
			{
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
				cout << "SANIpropagateOperationsClass::upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent - (sequenceIndexMinFound && sequenceIndexMaxFound); result == true" << endl;
				#endif
				result = true;
				#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
				keepExecuting = false;
				#endif
			}
			else
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
				if(keepExecutingLocal)
				{
				#endif
					if(upperNeuronLastSequenceIndexAlignsWithThatOfProspectiveComponent(forwardPropogationSequenceData, sequenceIndexMinToFindAbove, sequenceIndexMaxToFindAbove, sequenceIndexLastAboveEffectiveAfterGroupReset, level+1, ownerGroup, sequenceIndexMinFound, sequenceIndexMaxFound))
					{
						result = true;
					}
				#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED2
				}
				#endif
			}
		#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
		}
		#endif
	}
	
	return result;
}

#endif


#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
bool SANIpropagateCompactClass::printSANInetworkSVG(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	string outputTextSVGFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + "IncrementalGeneration" + SHAREDvars.convertIntToString(incrementalGenerationIndex) + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
	cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK: outputTextSVGFileName = " << outputTextSVGFileName << endl;
	
	#ifdef SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	SANIformation.createANNconnectivity(SANIGroupTypes);
	#endif
	
	GIAneuralNetworkOperationsClass().generateNeuralNetFromSANInet(translatorVariables);	//generate GIA NLP neural network

	int rasterImageWidth = 0;
	int rasterImageHeight = 0;
	bool ANNdisplayInOpenGL = false;
	bool ANNuseInputXMLFile = false;
	string ANNinputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputXMLFile = false;
	string ANNoutputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputLDRFile = false;
	string ANNoutputLDRFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
	bool ANNuseOutputSVGFile = true;
	string ANNoutputSVGFileName = outputTextSVGFileName;
	bool ANNuseOutputPPMFile = false;
	string ANNoutputPPMFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
	bool ANNuseOutputPPMFileRaytraced = false;
	string ANNoutputPPMFileNameRaytraced = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
	string ANNoutputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;
	bool ANNuseSprites = true;
	
	//small:
	int widthSVG = 3840;	//1920
	int heightSVG = 2160;	//1080	//1400
	
	//large/medium:
	//int widthSVG = 14400;
	//int heightSVG = 2430;
	
	ANNdisplay.outputNeuralNetworkToVectorGraphicsAndRaytrace(translatorVariables->firstInputNeuronInNetwork, ANNuseSprites, ANNuseOutputPPMFileRaytraced, ANNdisplayInOpenGL, ANNuseOutputLDRFile, ANNuseOutputSVGFile, ANNuseOutputPPMFile, ANNoutputLDRFileName, ANNoutputSVGFileName, ANNoutputPPMFileName, ANNoutputPPMFileNameRaytraced, ANNoutputTALFileName, rasterImageWidth, rasterImageHeight, widthSVG, heightSVG);

	#ifdef SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	SANIformation.createANNconnectivityReset(SANIGroupTypes);
	#endif
	
	incrementalGenerationIndex++;
	
	return result;
}
#endif


#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS
bool SANIpropagateCompactClass::verifySequenceIndexCoverageIntegrity(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* currentParseTreeGroup, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData)
{
	bool result = true;
	
	//cout << "SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS;" << endl;

			
	int parseTreeGroupSequenceIndexCoverage = SANInodes.calculateCoverage(currentParseTreeGroup);

	int sequenceIndexCurrent = forwardPropogationSequenceElementData->sequenceIndex;

	if(!SANIpropagateOperationsParseTree.traceBackpropParseTreeSequenceIndexCoverage(currentParseTreeGroup, 1, forwardPropogationSequenceData->sentenceContents))
	{
		cerr << "SANIpropagateCompactClass::verifySequenceIndexCoverageIntegrity fail #1" << endl;
		cerr << "!traceBackpropParseTreeSequenceIndexCoverage" << endl;
		result = false;
	}
	SANIpropagateOperationsParseTree.resetNeuronBackprop(currentParseTreeGroup, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);

	int performance = 0;
	for(int i=0; i<forwardPropogationSequenceData->sentenceContents->size(); i++)
	{
		LRPpreprocessorPlainTextWord* currentWord = (forwardPropogationSequenceData->sentenceContents)->at(i);
		if(currentWord->alreadyFoundMatch)
		{
			//cout << "currentParseTreeGroup->parseTreeMinSequenceIndex = " << currentParseTreeGroup->parseTreeMinSequenceIndex << endl;
			//cout << "currentParseTreeGroup->parseTreeMaxSequenceIndex = " << currentParseTreeGroup->parseTreeMaxSequenceIndex << endl;

			if((i>=currentParseTreeGroup->parseTreeMinSequenceIndex) && (i<=currentParseTreeGroup->parseTreeMaxSequenceIndex))	//parseTreeMaxSequenceIndex or= sequenceIndexCurrent
			{
				performance = performance + 1;
			}
			currentWord->alreadyFoundMatch = false;
		}
	}

	if(result)
	{
		if(performance == currentParseTreeGroup->parseTreeMaxSequenceIndex-currentParseTreeGroup->parseTreeMinSequenceIndex+1)	//sequenceIndexCurrent+1-currentParseTreeGroup->parseTreeMinSequenceIndex
		{
			result = true;
		}
		else
		{
			result = false;
			cerr << "SANIpropagateCompactClass::verifySequenceIndexCoverageIntegrity fail #2" << endl;
			cerr << "SANIpropagateCompactClass::verifySequenceIndexCoverageIntegrity fail: (performance != currentParseTreeGroup->parseTreeMaxSequenceIndex-currentParseTreeGroup->parseTreeMinSequenceIndex+1)" << endl;
		}
	}
	if(!result)
	{
		cerr << "SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS;" << endl;
		cerr << "currentParseTreeGroup->parseTreeMinSequenceIndex = " << currentParseTreeGroup->parseTreeMinSequenceIndex << endl;
		cerr << "currentParseTreeGroup->parseTreeMaxSequenceIndex = " << currentParseTreeGroup->parseTreeMaxSequenceIndex << endl;
		cerr << "sequenceIndexCurrent = " << sequenceIndexCurrent << endl;
		cerr << "performance = " << performance << endl;
		cerr << "parseTreeGroupSequenceIndexCoverage = " << parseTreeGroupSequenceIndexCoverage << endl;
		cerr << "currentParseTreeGroup->components.size() = " << currentParseTreeGroup->components.size() << endl;
		cerr << "currentParseTreeGroup->components[0]->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex = " << currentParseTreeGroup->components[0]->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex << endl;
		cerr << "currentParseTreeGroup->components[0]->componentType = " << currentParseTreeGroup->components[0]->componentType << endl;
		cerr << "currentParseTreeGroup->components[0]->componentRef->componentIndex = " << currentParseTreeGroup->components[0]->componentRef->componentIndex << endl;
		cerr << "currentParseTreeGroup->groupRef = " << endl;
		SANInodes.printGroup(currentParseTreeGroup->groupRef, 1);
		cerr << "printParseTree:" << endl;
		SANInodes.printParseTree(currentParseTreeGroup, 1);
		exit(EXIT_ERROR);
	}
	return result;
}
#endif



#endif
