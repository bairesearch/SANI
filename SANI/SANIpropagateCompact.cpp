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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p3a 15-March-2021
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

//if SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each wordIndex only
bool SANIpropagateCompactClass::defineFirstLayer(SANItranslatorVariablesClass* translatorVariables, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
		
	for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
	{
		bool getFirstLayer = true;
		
		int w = i;

		SANIForwardPropogationSignalData forwardPropogationSignalDataNOTUSED;
	
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSignalDataNOTUSED, forwardPropogationSentenceData, getFirstLayer))
		{
			result = false;
		}
	}
	
	return result;
}	
	
bool SANIpropagateCompactClass::executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents)
{
	SANIGroupParseTree* topLevelParseTreeGroup = NULL; 
	bool parseIsolatedSubreferenceSets = false;
	bool parserEnabled = false;
	int performance = 0;
	return executePosRelTranslatorNeuralNetwork(translatorVariables, SANIGroupTypes, sentenceContents, &topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performance);
}

bool SANIpropagateCompactClass::executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, constEffective SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, bool parserEnabled, int* performance)
{
	bool result = false;

	SANIForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.sentenceContents = sentenceContents;

	#ifndef SANI_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	executePosRelTranslatorNeuralNetworkPart1(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
	#endif
	
	#ifndef SANI_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	executePosRelTranslatorNeuralNetworkPart2(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
	#endif
	
	return result;
}

bool SANIpropagateCompactClass::executePosRelTranslatorNeuralNetworkPart1(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, constEffective SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;

	#ifdef SANI_PARSE_SAVE_PARSE_TREE
	forwardPropogationSentenceData->topLevelParseTreeGroupPropagate = NULL;
	#endif
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT	
	forwardPropogationSentenceData->parseTreeMaxWeightPropagate = 0;
	#endif
	
	//sentence specific variables:
	SANIpropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	#ifdef SANI_PARSE_SIMULTANEOUS
	forwardPropogationSentenceData->parserEnabled = parserEnabled;
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION
	forwardPropogationSentenceData->SANIGroupTypes = SANIGroupTypes;
	#endif
	
	for(int w=0; w<forwardPropogationSentenceData->sentenceContents->size(); w++)
	{	
		SANIForwardPropogationWordData* forwardPropogationWordData = new SANIForwardPropogationWordData();
		forwardPropogationSentenceData->forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
	}
		
	#ifndef SANI_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	if(performPropagationTest(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, true, topLevelParseTreeGroup))
	{
		result = true;
	}
	#endif
	
	return result;
}

bool SANIpropagateCompactClass::executePosRelTranslatorNeuralNetworkPart2(const SANItranslatorVariablesClass* translatorVariables, const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, constEffective SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = true;
	
	#ifdef SANI_FREE_MEMORY
	for(int w=0; w<forwardPropogationSentenceData->sentenceContents->size(); w++)
	{
		SANIForwardPropogationWordData* forwardPropogationWordData = forwardPropogationSentenceData->forwardPropogationWordDataArray[w];
		delete forwardPropogationWordData;
	}
	#endif
	
	if(result)
	{
		#ifdef SANI_PARSE_PERFORMANCE
		*performance = forwardPropogationSentenceData->sentenceContents->size();
		#else
		*performance = 1;
		#endif
	}
	
	return result;
}
	
bool SANIpropagateCompactClass::performPropagationTest(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{	
	bool recordTopLevelParseTreeGroup = false;
	return performPropagationTest(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, recordTopLevelParseTreeGroup, NULL);
}
bool SANIpropagateCompactClass::performPropagationTest(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool recordTopLevelParseTreeGroup, constEffective SANIGroupParseTree** topLevelParseTreeGroup)
{
	bool result = false;
	
	bool deinitialiseParseTreeGroupAfterFinish = true;
	bool resetAllNeuronComponents = true;
	int firstIndexInSequence = 0;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	SANIpropagateOperations.setParseSentenceReverse(false, forwardPropogationSentenceData);
	#endif
	
	if(performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence))
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
		cout << "toplevelGroupActivationFound: groupIndexes = " << SANInodes.printParseTreeGroupIndices(forwardPropogationSentenceData->topLevelParseTreeGroupPropagate, 0) << endl;
		#endif
		result = true;
		
		#ifdef SANI_PARSE_SAVE_PARSE_TREE
		if(recordTopLevelParseTreeGroup)
		{
			*topLevelParseTreeGroup = forwardPropogationSentenceData->topLevelParseTreeGroupPropagate;
		}
		#endif
	}
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_PROPAGATION_IN_OPPOSITE_DIRECTION
	SANIpropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	#endif
	
	return result;
}

bool SANIpropagateCompactClass::performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence)
{	
	int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed;
	int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = forwardPropogationSentenceData->sentenceContents->size()-1 - firstIndexInSequence;
		activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = 0;
	}
	else
	{
		activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = firstIndexInSequence;
		activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = forwardPropogationSentenceData->sentenceContents->size()-1;
	}
	return performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);
}

bool SANIpropagateCompactClass::performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
{
	int firstIndexInSequence;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		firstIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1;
	}
	else
	{
		firstIndexInSequence = 0;
	}
	return performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, deinitialiseParseTreeGroupAfterFinish, firstIndexInSequence, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed);

}

bool SANIpropagateCompactClass::performPropagation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool resetAllNeuronComponents, const bool deinitialiseParseTreeGroupAfterFinish, const int firstIndexInSequence, const int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, const int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
{
	bool result = false;
	
	forwardPropogationSentenceData->toplevelGroupActivationFound = false;
	
	if(resetAllNeuronComponents)
	{
		SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE); //this is required for SANI_LIGHT_OPTIMISED
		SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of SANIpropagateCompactClass::executePosRelTranslatorNeuralNetwork)
	}
	#ifdef SANI_FREE_MEMORY
	SANIpropagateOperationsParseTree.initialiseParseTreeGroupList(SANIGroupTypes, &(forwardPropogationSentenceData->parseTreeGroupListPropagate));
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	int iStart = firstIndexInSequence;
	#else
	int iStart = 0;
	#endif
	for(int i=iStart; i<forwardPropogationSentenceData->sentenceContents->size(); i++)	//OLD: for(int i=currentFirstInputNeuronIndexInSequence; ...
	{
		int firstLayerNeuronIndex;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			firstLayerNeuronIndex = forwardPropogationSentenceData->sentenceContents->size()-1-i;
		}
		else
		{
			firstLayerNeuronIndex = i;
		}	
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR	
		cout << "\n\n \e[33m SANIpropagateCompactClass::performPropagation: i = " << i << ", firstLayerNeuronIndex = " << firstLayerNeuronIndex << " - " << (*(forwardPropogationSentenceData->sentenceContents))[firstLayerNeuronIndex]->tagName << " \e[0m" << endl;
		#endif
	
		//group specific variables:
		SANIForwardPropogationSignalData forwardPropogationSignalData;
		forwardPropogationSignalData.activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed;
		forwardPropogationSignalData.activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;
		#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		forwardPropogationSignalData.firstIndexInSequence = firstIndexInSequence;
		#endif
		
		bool getFirstLayer = false;
		if(propagateWordThroughNetworkIntro(translatorVariables, firstLayerNeuronIndex, &forwardPropogationSignalData, forwardPropogationSentenceData, getFirstLayer))
		{

		}
	}

	if(deinitialiseParseTreeGroupAfterFinish)
	{
		#ifdef SANI_FREE_MEMORY
		SANIpropagateOperationsParseTree.deinitialiseParseTreeGroupList(SANIGroupTypes, &(forwardPropogationSentenceData->parseTreeGroupListPropagate));
		#endif
	}
	
	if(forwardPropogationSentenceData->toplevelGroupActivationFound)
	{
		result = true;
	}
	
	return result;
}

bool SANIpropagateCompactClass::propagateWordThroughNetworkIntro(SANItranslatorVariablesClass* translatorVariables, const int w, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer)
{
	bool result = true;
	
	//cout << "w = " << w << endl;
	
	LRPpreprocessorPlainTextWord* currentWord = (*(forwardPropogationSentenceData->sentenceContents))[w];

	SANIForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);
	forwardPropogationWordData->wordReference = currentWord;
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
	#endif
	forwardPropogationWordData->w = w;

	#ifdef SANI_DEBUG_PROPAGATE
	cout << "propagateWordThroughNetworkIntro: currentWord = " << currentWord->tagName << endl;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	int wordPOStypeNOTUSED = INT_DEFAULT_VALUE;
	if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStypeNOTUSED, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, getFirstLayer))
	{
		result = false;
	}	
	#else
	if(!SANInodes.currentWordAmbiguous(currentWord))
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		int wordPOStype = currentWord->unambiguousPOSindex;
		#else
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		forwardPropogationSignalData->firstPOSval = false;
		#endif
		for(int wordPOStype=0; wordPOStype<LRP_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		{
			if(SHAREDvars.getBitValue(currentWord->POSambiguityInfo, wordPOStype))
			{
		#endif	
				if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, getFirstLayer))
				{
					result = false;
				}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		#else
				#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
				forwardPropogationSignalData->firstPOSval = false;
				#endif
			}
		}
		#endif
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
	else
	{
		int wordPOStype = INT_DEFAULT_VALUE;
		bool pass = SANInodes.getWordPOStypeFromAmbiguousWord(currentWord, &wordPOStype);
		if(pass)
		{
			if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, getFirstLayer))
			{
				result = false;
			}		
		}
	}
	#endif
	#endif
	
	return result;
}

bool SANIpropagateCompactClass::propagateWordThroughNetworkGroupInit(SANItranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool getFirstLayer)
{
	bool result = true;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	LRPpreprocessorPlainTextWord* currentWord = forwardPropogationWordData->wordReference;
	SANIGroupNeuralNetwork* inputLayerGroup;
	if(!SANIformation.findInputNeuronLayerSectionWordOrig(currentWord, &inputLayerGroup))
	{
		SANIformation.addInputNeuronLayerSectionWordOrig(currentWord, &inputLayerGroup);
	}
	#else
	SANIGroupNeuralNetwork* inputLayerGroup = SANIformation.getInputGroupLayerSection(SANIformation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);	
	#endif
	
	forwardPropogationWordData->wordPOStype = wordPOStype;
	inputLayerGroup->wordPOStype = wordPOStype;

	if(getFirstLayer)
	{
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		(forwardPropogationSentenceData->firstLayer->at(w))->push_back(inputLayerGroup);
		#else
		forwardPropogationSentenceData->firstLayer->push_back(inputLayerGroup);
		#endif
	}
	else
	{
		#ifdef SANI_DEBUG_PROPAGATE
		cout << "SANIpropagateCompactClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
		#endif
		
		#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION_INPUT_NEURONS
		inputLayerGroup->neuronActive = true;
		inputLayerGroup->neuronReference->activationLevel = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_FULL;
		#endif
	
		SANIGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;	//not properly used by SANI_SEQUENCE_GRAMMAR
		int layer = 0;
		propagateWordThroughNetworkGroup(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup);
		
		#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION_INPUT_NEURONS
		inputLayerGroup->neuronActive = false;
		inputLayerGroup->neuronReference->activationLevel = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_INACTIVE;
		#endif
	}
			
	return result;
}	

bool SANIpropagateCompactClass::propagateWordThroughNetworkGroup(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
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
		
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup))
		{
			result = true;
		}
		
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	#endif
	
	return result;
}
		
bool SANIpropagateCompactClass::propagateWordThroughNetworkGroupSelect(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;

	//now search for existing sequence in network
	
	for(int i=0; i<group->SANIfrontComponentConnectionList.size(); i++)
	{
		#ifndef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
		{
		#endif
			SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i];
			SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
	
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
			if((forwardPropogationSentenceData->findingCandidateComponent2))
			{
				if(!(forwardPropogationSentenceData->foundCandidateComponent2))
				{
					if(currentComponent == forwardPropogationSentenceData->candidateComponent2ToFind)
					{
						forwardPropogationSentenceData->foundCandidateComponent2 = true;
						forwardPropogationSentenceData->candidateComponent2sourceParseTreeGroup = activationPathWordCurrentParseTreeGroup;
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
					currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
				}
	
				bool sequentialActivationFound = false;
				bool existingActivationFoundStartComponent = false;
				if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, &sequentialActivationFound, &existingActivationFoundStartComponent))
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
	


bool SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponentWrapper(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFoundAndExistingActivationFound = false;
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "3_: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", ownerGroup->groupIndex: " << ownerGroup->groupIndex << endl;	
	#endif
	
	SANIComponentNeuralNetwork* previousActiveComponent = NULL;	
	SANIComponentNeuralNetwork* finalActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;	//NOTUSED
	bool missingStartComponentFound = false;
	bool missingOrVariableStartComponentFound = false;
	//#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
	bool missingOrVariableEndComponentFound = false;
        //#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_LAST_COMPONENTS_REMEMBER_FIRST_COMPONENT_WORD_INDEX
	bool existingActivationFoundEndComponent = false;
	//#endif
	//#endif
	bool variableStartComponentFound = false;	//temp debug only
	int numberOfInactiveComponentsRemaining = 0;
        	
	bool componentWordConnectivityTestsPrelimResult = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	if(SANIpropagateOperations.componentWordConnectivityTestsWrapper(forwardPropogationWordData, forwardPropogationSentenceData, ownerGroup, activationPathWordCurrentParseTreeGroup, false))
	{
		componentWordConnectivityTestsPrelimResult = true;
	}
	#endif
	
	if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, currentComponent, &(ownerGroup->components), activationPathWordCurrentParseTreeGroup, &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &finalActiveComponent, existingActivationFoundStartComponent, &existingActivationFoundEndComponent, &missingStartComponentFound, &missingOrVariableStartComponentFound, &missingOrVariableEndComponentFound, &numberOfInactiveComponentsRemaining, ownerGroup, componentWordConnectivityTestsPrelimResult))
	{	
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA3_PRIMARY
		SANInodes.printParseTreeDebugIndentation(layer+1);
		cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << " - missingStartComponentFound = " << missingStartComponentFound << ", missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << "ownerGroup->currentParseTreeGroupTemp->components.size() = " << ownerGroup->currentParseTreeGroupTemp->components.size() << endl;
		#endif
		
		#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
		SANIGroupParseTree* currentParseTreeGroupBackup = ownerGroup->currentParseTreeGroupTemp;
		LRPpreprocessorPlainTextWord* previousActiveComponentActiveWordRecordBackup = NULL;
		if(previousActiveComponent != NULL)
		{
			previousActiveComponentActiveWordRecordBackup = previousActiveComponent->neuronComponentConnectionActiveWordRecord;
		}
		#endif
		
		//execute sequentialActivationConnectivityTests at start to save multiple executions;
		bool componentWordConnectivityTests = false;
		#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFIED	//or SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
		bool skipWordConnectivityTests = missingOrVariableStartComponentFound;
		#else
		bool skipWordConnectivityTests = missingStartComponentFound;
		#endif
		#else
		bool skipWordConnectivityTests = false;
		#endif
		if(sequentialActivationConnectivityTests(translatorVariables, currentComponent, ownerGroup, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, *existingActivationFoundStartComponent, existingActivationFoundEndComponent, skipWordConnectivityTests))
		{
			componentWordConnectivityTests = true;
			*sequentialActivationFound = true;
		}
	
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_LAST_COMPONENTS
		SANIpropagateCompactGenerateOperations.propagateVariableEndComponentRemoveLastParseTreeComponent(translatorVariables, layer, ownerGroup, currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, previousActiveComponent, finalActiveComponent, &existingActivationFoundEndComponent, componentWordConnectivityTests, sequentialActivationFound, activationPathWordCurrentParseTreeGroup);
		#endif		
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		SANIpropagateCompactGenerateOperations.verifyMissingOrVariableStartComponentFound(translatorVariables, layer, ownerGroup, currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, existingActivationFoundStartComponent, &missingStartComponentFound, &variableStartComponentFound, componentWordConnectivityTests, &missingOrVariableStartComponentFound, numberOfInactiveComponentsRemaining, sequentialActivationFound, activationPathWordCurrentParseTreeGroup);
		SANIpropagateCompactGenerateOperations.verifyMissingOrVariableStartComponentFoundAllowedWordIndices(translatorVariables, layer, ownerGroup, currentComponent, &(ownerGroup->components), forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, &activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, existingActivationFoundStartComponent, &missingStartComponentFound, &variableStartComponentFound, componentWordConnectivityTests, &missingOrVariableStartComponentFound, numberOfInactiveComponentsRemaining, sequentialActivationFound, activationPathWordCurrentParseTreeGroup);
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
			if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
			{
				missingOrVariableStartComponentFoundStored = ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound;	//CHECKTHIS; why not use bool missingOrVariableStartComponentFound
			}
			#endif
			#endif
			if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, groupFrontComponentConnectionListIndex, *existingActivationFoundStartComponent, missingStartComponentFound, missingOrVariableStartComponentFoundStored))
			{
				result = true;
			}
		}
		else
		{
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
			ownerGroup->currentParseTreeGroupTemp = currentParseTreeGroupBackup;
			if(previousActiveComponent != NULL)
			{
				previousActiveComponent->neuronComponentConnectionActiveWordRecord = previousActiveComponentActiveWordRecordBackup;
			}
			#endif
		}
	}

	SANIpropagateCompactGenerateOperations.updateActivatedNeuronWithMaxWordIndexCoverageWrapper(ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, *sequentialActivationFound, missingStartComponentFound, missingOrVariableStartComponentFound, missingOrVariableEndComponentFound);

	return result;
}
	
bool SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponent(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFoundStartComponent, bool missingStartComponentFound, bool missingOrVariableStartComponentFound)
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
		(forwardPropogationSentenceData->parseTreeGroupListPropagate).push_back(ownerGroup->currentParseTreeGroupTemp);
		#endif	
		#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
		ownerGroup->parseTreeGroupMemory.push_back(ownerGroup->currentParseTreeGroupTemp);
		#endif
	}
	
	#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
	//if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)) {
	currentComponent->componentStrength = currentComponent->componentStrength + 1;
	//}
	#endif
	currentComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	currentComponent->neuronComponentConnectionActiveWordRecordMemory.push_back(currentComponent->neuronComponentConnectionActiveWordRecord);	//note neuronComponentConnectionActiveWordRecordMemory is stored in currentComponent not newParseComponent such that it can be retained across executions of resetGroupParseTreeGroupRef(ownerGroup, false)
	#endif
	
	if(forwardPropogationWordData->wordReference->translatorSentenceWordIndex < 0)
	{
		cout << "SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponent error: forwardPropogationWordData->wordReference->translatorSentenceWordIndex = " << forwardPropogationWordData->wordReference->translatorSentenceWordIndex << endl;
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
	newParseComponent->neuronComponentConnectionActiveWordRecord = currentComponent->neuronComponentConnectionActiveWordRecord;
	#ifdef SANI_PARSE_GENERATE_PARSE_TREE
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	#endif
	#ifdef SANI_PARSE_RECORD_COMPONENT_WORD_INDEX
	newParseComponent->wordIndex = forwardPropogationWordData->w;
	#endif
	newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
						
	SANIGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;

	if(forwardPropogationSentenceData->parseSentenceReverse)
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
	if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage))
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION_RESTRICT_TO_POST_GENERATION_PHASE
		if(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal > translatorVariables->maxNumberSentences/2)
		{
		#endif
			printSANInetworkSVG(translatorVariables, forwardPropogationSentenceData->SANIGroupTypes);
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION_RESTRICT_TO_POST_GENERATION_PHASE
		}
		#endif
	}
	#endif
	
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	if(currentParseTreeGroupTemp->parseTreeMaxWordIndex == INT_DEFAULT_VALUE)
	{
		currentParseTreeGroupTemp->parseTreeMaxWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		currentParseTreeGroupTemp->parseTreeMinWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		currentParseTreeGroupTemp->parseTreeMaxWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeMinWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeTotalWeight = ownerGroup->groupWeight;
		#endif
	}
	SANIpropagateOperationsParseTree.updateParseTreeMaxMinWordIndexOfParent(currentParseTreeGroupTemp, newParseComponent, activationPathWordCurrentParseTreeGroup);
	#endif
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA4
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: missingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << ", newParseComponent->componentIndex = " << newParseComponent->componentIndex << ", ownerGroup->components.size() = " << ownerGroup->components.size() << ", groupIndex = " << ownerGroup->groupIndex << ", existingActivationFoundStartComponent = " << existingActivationFoundStartComponent << ", activationSequenceCompleted = " << activationSequenceCompleted << endl; 
	#endif
		
	#ifdef SANI_PARSE
	SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroupTemp;
	#endif
	

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS
	verifyWordIndexCoverageIntegrity(forwardPropogationSentenceData, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationWordData);
	#endif

	if(activationSequenceCompleted)
	{			
		ownerGroup->neuronActive = true;	//required for SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION
		
		#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_ALL_PROPAGATED_NEURONS
		if(forwardPropogationSentenceData->updateNeuronRecency)
		{
			ownerGroup->timeIndex = translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal;	
		}
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
		//if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)) {
		ownerGroup->groupStrength = ownerGroup->groupStrength + 1;
		//}
		#endif
	
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA
		SANInodes.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif
		
		SANIForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = forwardPropogationSignalData;
		
		bool candidateCoveragePartial = false;	
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
		cout << "activationSequenceCompleted" << endl;
		#endif
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
		{
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
			cout << "(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)" << endl;
			#endif
			//consider relying on: activationPathWordCurrentParseTreeGroupOwner->missingOrVariableStartComponentFound/missingStartComponentFound, rather than passing missingOrVariableStartComponentFound/missingStartComponentFound
			SANIpropagateCompactGenerateOperations.updateActivatedNeuronWithMaxWordIndexCoverage(ownerGroup, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, missingStartComponentFound, missingOrVariableStartComponentFound, false, candidateCoveragePartial);
		}
		
			
		bool topLevelGroup = false;	
		#ifdef SANI_PREVENT_RECORDING_TOP_LEVEL_NEURON_IF_RECORDING_MAX_COVERAGE_NEURON
		if(!(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage))
		{			
		#endif
			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
			int maxWeight;
			SANIpropagateOperationsParseTree.calculatePerformanceWeightOfParseTree(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, &maxWeight);
			if(maxWeight >= forwardPropogationSentenceData->parseTreeMaxWeightPropagate)
			{
			#endif				
				int wComparisonInt;
				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					wComparisonInt = 0;	//start of sentence found
				}
				else
				{
					wComparisonInt = forwardPropogationSentenceData->sentenceContents->size()-1;	//end of sentence punctuation mark found
				}
				if(forwardPropogationWordData->w == wComparisonInt)	//start of sentence found
				{
					int topLevelParseTreeGroupWordIndexCoverage = SANInodes.calculateCoverage(activationPathWordCurrentParseTreeGroupOwner);

					if(topLevelParseTreeGroupWordIndexCoverage == forwardPropogationSentenceData->sentenceContents->size())
					{
						#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_ONLY_ONE_TOP_LEVEL_NEURON_FOUND_PER_SENTENCE
						if(forwardPropogationSentenceData->toplevelGroupActivationFound)
						{
							cerr << "SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponent - topLevelGroup && already found (forwardPropogationSentenceData->toplevelGroupActivationFound)" << endl;
							exit(EXIT_ERROR);
						}
						else
						{
						#endif
							#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_TOP_LEVEL_NEURON_FOUND
							if(ownerGroup->topLevelSentenceNeuron)
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
								cout << "parseTreeMaxWeight1 = " << forwardPropogationSentenceData->parseTreeMaxWeightPropagate << endl;
								#endif
								cout << "forwardPropogationSentenceData->topLevelParseTreeGroupPropagate->groupName = " << activationPathWordCurrentParseTreeGroupOwner->groupName << endl;
								printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 3);
								#endif
								
								
								#ifdef SANI_SEQUENCE_WORDCONNECTIVITY_VERIFICATION
								if(!SANIpropagateOperationsParseTree.updatePerformanceGroup(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
								{
									cerr << "SANI_SEQUENCE_WORDCONNECTIVITY_VERIFICATION; SANIpropagateCompactClass::propagateWordThroughNetworkGroupComponent error: !SANIpropagateOperationsParse.updatePerformance" << endl;
									cerr << "forwardPropogationSentenceData->performance = " << forwardPropogationSentenceData->performance << endl;
									cerr << "forwardPropogationSentenceData->sentenceContents->size() = " << forwardPropogationSentenceData->sentenceContents->size() << endl;
									SANInodes.printParseTree(activationPathWordCurrentParseTreeGroupOwner, 0);
									exit(EXIT_ERROR);
								}
								#endif

								topLevelGroup = true;

								forwardPropogationSentenceData->finishedPassingSentenceWords = true;

								forwardPropogationSentenceData->toplevelGroupActivationFound = true;

								forwardPropogationSentenceData->parseTreeMaxWeightPropagate = maxWeight;

								#ifdef SANI_PARSE_SAVE_PARSE_TREE
								#ifdef SANI_REPLICATE_TOP_LEVEL_PARSE_TREE
								SANInodes.deleteParseTree(forwardPropogationSentenceData->topLevelParseTreeGroupPropagate, 0);
								forwardPropogationSentenceData->topLevelParseTreeGroupPropagate = SANInodes.replicateParseTree(activationPathWordCurrentParseTreeGroupOwner, 0);
								#ifdef SANI_DEBUG_RULES_PRINT_PARSE_PROCESS_POS_TYPES
								//printBackpropParseTree(forwardPropogationSentenceData->topLevelParseTreeGroupPropagate, 3);
								#endif
								#else
								//copy currentParseTreeGroupTemp so it cant be overwritten;
								forwardPropogationSentenceData->topLevelParseTreeGroupPropagate = SANInodes.copyGroup(activationPathWordCurrentParseTreeGroupOwner);
								#endif
								#endif

							#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_TOP_LEVEL_NEURON_FOUND
							}
							#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_ONLY_ONE_TOP_LEVEL_NEURON_FOUND_PER_SENTENCE
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
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))
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
	
bool SANIpropagateCompactClass::sequentialActivationConnectivityTests(const SANItranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, const int layer, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, bool existingActivationFoundStartComponent, const bool existingActivationFoundEndComponent, const bool skipWordConnectivityTests)
{	
	bool sequentialActivationFound = false;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP
	if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationWordData, currentComponent, ownerGroup, activationPathWordCurrentParseTreeGroup, existingActivationFoundStartComponent))
	{
	#endif
		bool passBasicWordConnectivityTest = true;
		if(!skipWordConnectivityTests)
		{
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS		
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS2
			if(currentComponent != SANInodes.getFirstComponent(forwardPropogationSentenceData, ownerGroup, true))
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
						if(SANIpropagateOperations.componentWordConnectivityTestsWrapper(forwardPropogationWordData, forwardPropogationSentenceData, ownerGroup, currentParseTreeGroupTemp, activationPathWordCurrentParseTreeGroup, existingActivationFoundStartComponent, existingActivationFoundEndComponent))
						{
							//cout << "passBasicWordConnectivityTest" << endl;
							passBasicWordConnectivityTest = true;
						}
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS			
					}
				}
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_PRECHECKS2
			}
			#endif
			#endif
			#endif
		}
		
		if(passBasicWordConnectivityTest)
		{			
			/*
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
			Note if variableStartComponentFound, then componentWordConnectivityTestsWrapper is not required to be reexecuted, since if the previous component was variable (ie miscellaneously activated) and components->size()==1, no word index connectivity test is required to be conducted. 
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
	bool print = true;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	SANIpropagateOperationsParseTree.traceBackpropParseTree(group, 1, print, performancePreprocess, &performanceNOTUSED, NULL);
	SANIpropagateOperationsParseTree.resetNeuronBackprop(group, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
}



bool SANIpropagateCompactClass::identifyComponentIndexLastActivatedIndex(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex)
{
	bool result = false;
	int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
	SANIGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		result = true;
		if(forwardPropogationSentenceData->parseSentenceReverse)
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
bool SANIpropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, const SANIComponentNeuralNetwork* component, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, bool existingActivationFoundStartComponent)
{
	bool result = false;
	
	SANIGroupParseTree* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
	if((ownerGroupParseTreeGroup->components.size() == 0) || existingActivationFoundStartComponent)
	{		
		int firstWordIndexInSequence;
		//expect to find upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent if current group->parseTreeMinWordIndex/parseTreeMaxWordIndex doesnt encapsulate firstWordIndexInSequence

		int wordIndexMax = -1;
		int wordIndexMin = REALLY_LARGE_INT;
		const SANIGroupParseTree* parseTreeGroupToFindWordIndexMin;
		const SANIGroupParseTree* parseTreeGroupToFindWordIndexMax;
		int wordIndexMinMin;
		int wordIndexMaxMax;
		int wordIndexMinToFindAbove;		
		int wordIndexMaxToFindAbove;
		int wordIndexLastAboveEffectiveAfterGroupReset = INT_DEFAULT_VALUE;

		bool execute = false;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "\nSANIpropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponentWrapper start:" << endl;
			#endif
			
			#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			firstWordIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1-forwardPropogationSignalData->firstIndexInSequence;
			#else
			firstWordIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1;
			#endif

			parseTreeGroupToFindWordIndexMax = activationPathWordCurrentParseTreeGroup;
			wordIndexMaxMax = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;	//forwardPropogationWordData->w;
			if(parseTreeGroupToFindWordIndexMax != NULL)
			{
				if(!SANIpropagateOperationsParseTree.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true, &wordIndexMax, 0))
				{

				}
			}
			
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "wordIndexMaxMax = " << wordIndexMaxMax << endl;
			cout << "wordIndexMax = " << wordIndexMax << endl;
			#endif
			
			wordIndexMax = SHAREDvars.maxInt(wordIndexMax, wordIndexMaxMax);
			
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "wordIndexMax (maxInt) = " << wordIndexMax << endl;
			#endif
			
			if(wordIndexMax < firstWordIndexInSequence)
			{
				wordIndexMaxToFindAbove = firstWordIndexInSequence;
				wordIndexMinToFindAbove = wordIndexMax+1;
			
				//need to disregard wordIndexMin/Max of last activated component in uppergroups (assuming it was effectively activated by current component)
				if(existingActivationFoundStartComponent)	//ie ownerGroupParseTreeGroup != NULL
				{
					wordIndexLastAboveEffectiveAfterGroupReset = ownerGroupParseTreeGroup->parseTreeMaxWordIndex+1;//CHECKTHIS; ??
				}
				
				execute = true;
			}
		}
		else
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			firstWordIndexInSequence = forwardPropogationSignalData->firstIndexInSequence;
			#else
			firstWordIndexInSequence = 0;
			#endif

			parseTreeGroupToFindWordIndexMin = activationPathWordCurrentParseTreeGroup;
			wordIndexMinMin = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;	//forwardPropogationWordData->w;
			if(parseTreeGroupToFindWordIndexMin != NULL)
			{
				if(!SANIpropagateOperationsParseTree.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false, &wordIndexMin, 0))
				{

				}
			}
			wordIndexMin = SHAREDvars.minInt(wordIndexMin, wordIndexMinMin);
			if(wordIndexMin > firstWordIndexInSequence)
			{
				wordIndexMinToFindAbove = firstWordIndexInSequence;
				wordIndexMaxToFindAbove = wordIndexMin-1;
			
				//need to disregard wordIndexMin/Max of last activated component in uppergroups (assuming it was effectively activated by current component)
				if(existingActivationFoundStartComponent)	//ie ownerGroupParseTreeGroup != NULL
				{
					wordIndexLastAboveEffectiveAfterGroupReset = ownerGroupParseTreeGroup->parseTreeMinWordIndex-1;	
				}
				
				execute = true;
			}
		}	
		
		if(execute)
		{
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			cout << "wordIndexMaxToFindAbove = " << wordIndexMaxToFindAbove << endl;
			cout << "wordIndexMinToFindAbove = " << wordIndexMinToFindAbove << endl;
			cout << "wordIndexLastAboveEffectiveAfterGroupReset = " << wordIndexLastAboveEffectiveAfterGroupReset << endl;
			#endif
				
			if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(forwardPropogationSentenceData, wordIndexMinToFindAbove, wordIndexMaxToFindAbove, wordIndexLastAboveEffectiveAfterGroupReset, 0, ownerGroup, false, false))
			{
				result = true;
			}
			else
			{
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
				cout << "!upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent" << endl;
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

//bool SANIpropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool findMaxOrMinWordIndex, int wordIndexMaxOrMinToFind, int level, SANIGroupNeuralNetwork* group)
bool SANIpropagateCompactClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int wordIndexMinToFindAbove, const int wordIndexMaxToFindAbove, const int wordIndexLastAboveEffectiveAfterGroupReset, int level, const SANIGroupNeuralNetwork* group, bool wordIndexMinFound, bool wordIndexMaxFound)
{
	bool result = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
	bool keepExecuting = true;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
	wordIndexMinFound = false;
	wordIndexMaxFound = false;
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
			int parseTreeMinWordIndexEffectiveAfterGroupReset = ownerGroupParseTree->parseTreeMinWordIndex;
			int parseTreeMaxWordIndexEffectiveAfterGroupReset = ownerGroupParseTree->parseTreeMaxWordIndex;

			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			SANInodes.printParseTreeDebugIndentation(level);
			cout << "parseTreeMinWordIndex = " << parseTreeMinWordIndexEffectiveAfterGroupReset << endl;
			SANInodes.printParseTreeDebugIndentation(level);
			cout << "parseTreeMaxWordIndex = " << parseTreeMaxWordIndexEffectiveAfterGroupReset << endl;
			#endif


			#ifdef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_OPTIMISED
			bool groupHasPreceedingComponent = false;
			if(forwardPropogationSentenceData->parseSentenceReverse)
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
			{//currentComponent was activated by the group being reset - so determine the ownerGroup's expected [older] parseTreeMinWordIndex/parseTreeMaxWordIndex values assuming the group is reset
				if(wordIndexLastAboveEffectiveAfterGroupReset != INT_DEFAULT_VALUE)
				{
					if(forwardPropogationSentenceData->parseSentenceReverse)
					{
						if(parseTreeMinWordIndexEffectiveAfterGroupReset < wordIndexLastAboveEffectiveAfterGroupReset)
						{
							parseTreeMinWordIndexEffectiveAfterGroupReset = wordIndexLastAboveEffectiveAfterGroupReset;
						}
					}
					else
					{
						if(parseTreeMaxWordIndexEffectiveAfterGroupReset > wordIndexLastAboveEffectiveAfterGroupReset)
						{
							parseTreeMaxWordIndexEffectiveAfterGroupReset = wordIndexLastAboveEffectiveAfterGroupReset;
						}
					}
				}
			}

			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
			SANInodes.printParseTreeDebugIndentation(level);
			cout << "parseTreeMinWordIndexEffectiveAfterGroupReset = " << parseTreeMinWordIndexEffectiveAfterGroupReset << endl;
			SANInodes.printParseTreeDebugIndentation(level);
			cout << "parseTreeMaxWordIndexEffectiveAfterGroupReset = " << parseTreeMaxWordIndexEffectiveAfterGroupReset << endl;
			#endif

			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				//assume will find wordIndexMin match before wordIndexMax match when propagating up network - CHECKTHIS
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				if(!wordIndexMinFound)
				{
				#endif
					if(parseTreeMinWordIndexEffectiveAfterGroupReset == wordIndexMinToFindAbove)
					{
						wordIndexMinFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIpropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMinFound" << endl;
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
				if(wordIndexMinFound)
				{
				#endif
					if(parseTreeMaxWordIndexEffectiveAfterGroupReset == wordIndexMaxToFindAbove)
					{
						wordIndexMaxFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIpropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMaxFound" << endl;
						#endif
					}
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				}
				#endif
			}
			else
			{
				//assume will find wordIndexMax match before wordIndexMin match when propagating up network - CHECKTHIS
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST
				if(!wordIndexMaxFound)
				{
				#endif
					if(parseTreeMaxWordIndexEffectiveAfterGroupReset == wordIndexMaxToFindAbove)
					{
						wordIndexMaxFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIpropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMaxFound" << endl;
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
				if(wordIndexMaxFound)
				{
				#endif
					if(parseTreeMinWordIndexEffectiveAfterGroupReset == wordIndexMinToFindAbove)
					{
						wordIndexMinFound = true;
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
						cout << "SANIpropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - wordIndexMinFound" << endl;
						#endif
					}
				#ifndef SANI_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY_BETWEEN_NEWLY_ACTIVATED_COMPONENT_AND_PREVIOUSLY_ACTIVATED_GROUP_INST					
				}
				#endif
			}


			if(wordIndexMinFound && wordIndexMaxFound)
			{
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_ENSURE_PROSPECTIVE_WORD_CONNECTIVITY
				cout << "SANIpropagateOperationsClass::upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent - (wordIndexMinFound && wordIndexMaxFound); result == true" << endl;
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
					if(upperNeuronLastWordIndexAlignsWithThatOfProspectiveComponent(forwardPropogationSentenceData, wordIndexMinToFindAbove, wordIndexMaxToFindAbove, wordIndexLastAboveEffectiveAfterGroupReset, level+1, ownerGroup, wordIndexMinFound, wordIndexMaxFound))
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
bool SANIpropagateCompactClass::verifyWordIndexCoverageIntegrity(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup, const SANIForwardPropogationWordData* forwardPropogationWordData)
{
	bool result = true;
	
	//cout << "SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS;" << endl;

			
	int parseTreeGroupWordIndexCoverage = SANInodes.calculateCoverage(currentParseTreeGroup);

	int wordIndexCurrent = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;

	int performanceTemp = 0;
	bool print = false;
	bool performancePreprocess = true;
	if(!SANIpropagateOperationsParseTree.traceBackpropParseTree(currentParseTreeGroup, 1, print, performancePreprocess, &performanceTemp, forwardPropogationSentenceData->sentenceContents))
	{
		cerr << "SANInodesClass::verifyWordIndexCoverageIntegrity fail #1" << endl;
		cerr << "!traceBackpropParseTree" << endl;
		result = false;
	}
	SANIpropagateOperationsParseTree.resetNeuronBackprop(currentParseTreeGroup, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);

	int performance = 0;
	for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
	{
		LRPpreprocessorPlainTextWord* currentWord = (forwardPropogationSentenceData->sentenceContents)->at(i);
		if(currentWord->alreadyFoundMatch)
		{
			//cout << "currentParseTreeGroup->parseTreeMinWordIndex = " << currentParseTreeGroup->parseTreeMinWordIndex << endl;
			//cout << "currentParseTreeGroup->parseTreeMaxWordIndex = " << currentParseTreeGroup->parseTreeMaxWordIndex << endl;

			if((i>=currentParseTreeGroup->parseTreeMinWordIndex) && (i<=currentParseTreeGroup->parseTreeMaxWordIndex))	//parseTreeMaxWordIndex or= wordIndexCurrent
			{
				performance = performance + 1;
			}
			currentWord->alreadyFoundMatch = false;
		}
	}

	if(result)
	{
		if(performance == currentParseTreeGroup->parseTreeMaxWordIndex-currentParseTreeGroup->parseTreeMinWordIndex+1)	//wordIndexCurrent+1-currentParseTreeGroup->parseTreeMinWordIndex
		{
			result = true;
		}
		else
		{
			result = false;
			cerr << "SANInodesClass::verifyWordIndexCoverageIntegrity fail #2" << endl;
			cerr << "SANInodesClass::verifyWordIndexCoverageIntegrity fail: (performance != currentParseTreeGroup->parseTreeMaxWordIndex-currentParseTreeGroup->parseTreeMinWordIndex+1)" << endl;
		}
	}
	if(!result)
	{
		cerr << "SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION_CONTINUOUS;" << endl;
		cerr << "currentParseTreeGroup->parseTreeMinWordIndex = " << currentParseTreeGroup->parseTreeMinWordIndex << endl;
		cerr << "currentParseTreeGroup->parseTreeMaxWordIndex = " << currentParseTreeGroup->parseTreeMaxWordIndex << endl;
		cerr << "wordIndexCurrent = " << wordIndexCurrent << endl;
		cerr << "performance = " << performance << endl;
		cerr << "parseTreeGroupWordIndexCoverage = " << parseTreeGroupWordIndexCoverage << endl;
		cerr << "currentParseTreeGroup->components.size() = " << currentParseTreeGroup->components.size() << endl;
		cerr << "currentParseTreeGroup->components[0]->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex = " << currentParseTreeGroup->components[0]->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex << endl;
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
