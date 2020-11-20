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
 * File Name: SANIgenerateCompact.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1o4a 17-November-2020
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#include "SANIgenerateCompact.hpp"
#include "SHAREDvars.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR


bool SANIgenerateCompactClass::generatePosRelTranslatorNeuralNetworkWrapper(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const bool createNewConnections)
{
	SANIGroupParseTree* topLevelParseTreeGroup = NULL; 
	bool parseIsolatedSubreferenceSets = false;
	bool parserEnabled = false;
	int performance = 0;
	return generatePosRelTranslatorNeuralNetwork(translatorVariables, SANIGroupTypes, sentenceContents, &topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performance, createNewConnections);
}

bool SANIgenerateCompactClass::generatePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, bool parseIsolatedSubreferenceSets, bool parserEnabled, int* performance, const bool createNewConnections)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
	cout << "SANIgenerateCompactClass::generatePosRelTranslatorNeuralNetwork" << endl;
	#endif
	
	SANIForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.sentenceContents = sentenceContents;
	
	bool toplevelGroupActivationFound = SANIpropagateCompact.executePosRelTranslatorNeuralNetworkPart1(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
	
	if(!toplevelGroupActivationFound)
	{				
		if(createNewConnections)
		{
			if(!generateSentenceNetworkNodes(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData))
			{
				result = false;
			}
	
			#ifdef SANI_DEBUG_PROPAGATE_TRACE
			cout << "\ntestSentenceNetworkNodes #1" << endl;
			#endif
			if(!testSentenceNetworkNodes(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup))
			{
				result = false;
			}
					
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY
			if(result)
			{
				if(!SANIgenerateCompactIdentify.identifyVariableComponentsAndReferenceSets(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData, *topLevelParseTreeGroup))
				{
					result = false;
				}

				#ifdef SANI_DEBUG_PROPAGATE_TRACE
				cout << "testSentenceNetworkNodes #2" << endl;
				#endif
				if(!testSentenceNetworkNodes(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup))
				{
					result = false;
				}
			}
			#endif
			
			#ifdef SANI_ANN_COLOUR_NODES_FROM_LAST_GENERATED_SENTENCE
			SANIgenerateCompactIdentify.setGeneratedForLastSentence(translatorVariables);
			#endif
		
			#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_NEWLY_GENERATED_NEURONS
			SANIgenerateCompactIdentify.clearHiddenLayerGroupsTemp();
			#endif
		}
	}
		
	SANIpropagateCompact.executePosRelTranslatorNeuralNetworkPart2(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
	
		
	return toplevelGroupActivationFound;
}

bool SANIgenerateCompactClass::generateSentenceNetworkNodes(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	
	bool supportVariableFirstComponents = true;
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
	if(!SANIgenerateCompactSectionedVariable.findAndReconcileVariationWrapper(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData->sentenceContents, forwardPropogationSentenceData, supportVariableFirstComponents))
	{

	}
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_OPTIONAL
	supportVariableFirstComponents = false;
	if(SANIgenerateCompactSectionedVariable.findAndReconcileVariationWrapper(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData->sentenceContents, forwardPropogationSentenceData, supportVariableFirstComponents))
	{

	}
	#endif
	#else
	if(!SANIgenerateCompactContinuous.findAndReconcileVariationWrapper(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData->sentenceContents, forwardPropogationSentenceData, supportVariableFirstComponents))	
	{

	}	
	#endif
	
	#else	
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
	if(!SANIgenerateCompactSectionedUnique.findAndReconcileVariationWrapper(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData->sentenceContents, forwardPropogationSentenceData))	
	#else
	bool supportVariableFirstComponents = false;	//unused
	if(!SANIgenerateCompactContinuous.findAndReconcileVariationWrapper(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData->sentenceContents, forwardPropogationSentenceData, supportVariableFirstComponents))
	#endif
	{

	}	
	#endif
	
	return result;
}

bool SANIgenerateCompactClass::testSentenceNetworkNodes(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree** topLevelParseTreeGroup)
{
	bool result = true;

	*topLevelParseTreeGroup = NULL;
	
	/*
	bool performTest = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
	performTest = true;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_VERIFICATION
	performTest = true;
	#ifdef SANI_DEBUG_RULES_PRINT_SENTENCES
	cout << "\nfindAndReconcileVariationWrapper SANI_SEQUENCE_GRAMMAR_VERIFICATION: testSentenceNetworkNodes:" << endl;
	#endif
	#endif
	if(performTest)
	{
	*/
	
	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_ALL_PROPAGATED_NEURONS
	forwardPropogationSentenceData->updateNeuronRecency = true;
	#endif
	if(!SANIpropagateCompact.performPropagationTest(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, true, topLevelParseTreeGroup))
	{
		result = false;
		#ifndef SANI_DEBUG_IGNORE_TEST_FAILURE
		cerr << "SANIgenerateCompactClass::generatePosRelTranslatorNeuralNetwork error: !SANIpropagateCompact.performPropagationTest" << endl;
		exit(EXIT_ERROR);
		#endif
	}
	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_ALL_PROPAGATED_NEURONS
	forwardPropogationSentenceData->updateNeuronRecency = false;
	#endif
	
	/*
	}
	*/
			
	return result;
}


		
#endif
