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
 * File Name: SANIpropagateCompactGenerate.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1m4b 01-September-2020
 * Requirements: 
 * Description: SANI (Sequentially Activated Neuronal Input neural network) Propagate Compact - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#include "SANIpropagateCompactGenerate.hpp"
#include "SHAREDvars.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR

//static int newNeuronIndex;

#ifdef SANI_FREE_MEMORY
static vector<SANIGroupParseTree*>* parseTreeGroupListPointer;
#endif





bool SANIpropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool createNewConnections)
{
	SANIGroupParseTree* topLevelParseTreeGroup = NULL; 
	bool parseIsolatedSubreferenceSets = false;
	bool parserEnabled = false;
	int performance = 0;
	return executeTxtRelTranslatorNeuralNetwork(translatorVariables, SANIGroupTypes, sentenceContents, &topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performance, createNewConnections);
}

bool SANIpropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance, const bool createNewConnections)
{
	bool result = false;
	
	#ifdef SANI_FREE_MEMORY
	parseTreeGroupListPointer = SANIpropagateCompact.getParseTreeGroupList();
	#endif

	SANIForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.sentenceContents = sentenceContents;

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
	cout << "\n\n\n\n\nSANIpropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork 1" << endl;
	#endif
	
	bool toplevelGroupActivationFound = SANIpropagateCompact.executeTxtRelTranslatorNeuralNetworkPart1(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
	cout << "SANIpropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork 2" << endl;
	#endif
	
	if(!toplevelGroupActivationFound)
	{				
		if(createNewConnections)
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
			bool supportVariableFirstComponents = true;
			#else
			bool supportVariableFirstComponents = false;	//unused
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_PRELIM_TEST
			SANIGroupTypesBackup = copyRuleGroupType(SANIGroupTypes);
			#endif
			if(!findAndReconcileVariationWrapper(translatorVariables, SANIGroupTypes, sentenceContents, &forwardPropogationSentenceData, supportVariableFirstComponents))
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_PRELIM_TEST
				SANIGroupTypes = SANIGroupTypesBackup;
				bool supportVariableFirstComponents = false;
				if(findAndReconcileVariationWrapper(translatorVariables, SANIGroupTypes, sentenceContents, &forwardPropogationSentenceData, supportVariableFirstComponents))
				{
				
				}
				#endif
			}
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_OPTIONAL
			supportVariableFirstComponents = false;
			if(findAndReconcileVariationWrapper(translatorVariables, SANIGroupTypes, sentenceContents, &forwardPropogationSentenceData, supportVariableFirstComponents))
			{

			}
			#endif
		}
	}
	
	SANIpropagateCompact.executeTxtRelTranslatorNeuralNetworkPart2(translatorVariables, SANIGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance);
		
	return toplevelGroupActivationFound;
}

bool SANIpropagateCompactGenerateClass::findAndReconcileVariationWrapper(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents)
{
	bool result = true;

	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_SENTENCES
	cout << "\nfindAndReconcileVariationWrapper:" << endl;
	#endif
		
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH	
	forwardPropogationSentenceData->variableFirstComponentMaxDepth = 0;
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	bool executePrelimPassover = true;
	bool passOverGenerationFindVariableEndComponent = true;
	bool passOverGenerationFindGroupNeuron = false;
	bool foundGroupNeuron = false;
	bool foundCoverageVariableEndComponent = false;
	int loopIndex = 0;
	int executePrelimPassoverNextLoopIndex = 0;
	#endif

	int indexInSequenceStart = 0;
	bool completedSentenceNetworkGeneration = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	vector<SANIGroupNeuralNetwork*> highLevelNeuronPriorArray;	//contains prior neuron at each level of the array
	#else
	SANIGroupNeuralNetwork* highLevelNeuronPrior;
	#endif
	while(!completedSentenceNetworkGeneration)
	{	
		bool resultTemp = false;
		
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
		cout << "\nloopIndex = " << loopIndex << endl;
		#endif
		
		vector<SANIGroupNeuralNetwork*> listOfHighLevelNeurons;
		
		if(!executePrelimPassover)
		{
			if(loopIndex == executePrelimPassoverNextLoopIndex)
			{
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
				cout << "(loopIndex == executePrelimPassoverNextLoopIndex)" << endl;
				#endif
				executePrelimPassover = true;
			}		
		}
		
		bool enableVariableEndComponentDetection = false;
		bool enableVariableEndComponentDetectionExpectedCompletedIdentifyingSentenceHighLevelNeurons = false;
		int indexInSequenceStartPrelimPassover = indexInSequenceStart;
		if(executePrelimPassover)
		{
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
			cout << "executePrelimPassover: passOverGenerationFindVariableEndComponent = true" << endl;
			#endif
			
			bool enableVariableEndComponentDetectionPrelim = false;
			
			passOverGenerationFindVariableEndComponent = true;
			passOverGenerationFindGroupNeuron = false;
			foundGroupNeuron = false;
			foundCoverageVariableEndComponent = false;
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
			cout << "indexInSequenceStartPrelimPassover = " << indexInSequenceStartPrelimPassover << endl;
			#endif

			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_FIRST_AND_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE_PRE
			forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.clear();	//required for compatibility with SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENT (prelim rather than post clear of listOfHighLevelNeuronsCompleteHistory)
			#endif
						
			if(findAndReconcileIncrementalVariationLimitNumComponentsSection(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, supportVariableFirstComponents, &indexInSequenceStartPrelimPassover, &highLevelNeuronPriorArray, enableVariableEndComponentDetectionPrelim, false, passOverGenerationFindVariableEndComponent, passOverGenerationFindGroupNeuron, &foundGroupNeuron, &foundCoverageVariableEndComponent, &listOfHighLevelNeurons))
			{	
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_FIRST_AND_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE
				SANIGroupNeuralNetwork* neuronWithVariableEndComponent = listOfHighLevelNeurons.back();
				#endif
				listOfHighLevelNeurons.clear();
							
				if(foundCoverageVariableEndComponent && (indexInSequenceStartPrelimPassover != sentenceContents->size()))
				{
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_FIRST_AND_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_FIRST_AND_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE_PRE
					forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.clear();
					#endif
					SANIComponentNeuralNetwork* neuronWithVariableEndComponentStartInput = SANInodes.getFirstComponent(forwardPropogationSentenceData, neuronWithVariableEndComponent, true);
					for(int j=0; j<neuronWithVariableEndComponentStartInput->ANNbackGroupConnectionList.size(); j++)
					{
						SANIGroupNeuralNetwork* neuronWithVariableEndComponentStartInputNeuron = neuronWithVariableEndComponentStartInput->ANNbackGroupConnectionList[j];
						if(neuronWithVariableEndComponentStartInputNeuron == NULL)
						{
							cerr << "(neuronWithVariableEndComponentStartInputNeuron == NULL)" << endl;
							exit(EXIT_ERROR);
						}
						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_FIRST_AND_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE_PRE
						forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.push_back(neuronWithVariableEndComponentStartInputNeuron);
						#endif
					}
					#endif
				
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
					cout << "\texecutePrelimPassover: foundCoverageVariableEndComponent = true" << endl;
					#endif
					
					passOverGenerationFindVariableEndComponent = false;
					passOverGenerationFindGroupNeuron = true;
					foundGroupNeuron = false;
					foundCoverageVariableEndComponent = false;
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
					cout << "\tindexInSequenceStartPrelimPassover = " << indexInSequenceStartPrelimPassover << endl;
					#endif
					
					if(findAndReconcileIncrementalVariationLimitNumComponentsSection(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, supportVariableFirstComponents, &indexInSequenceStartPrelimPassover, &highLevelNeuronPriorArray, enableVariableEndComponentDetectionPrelim, false, passOverGenerationFindVariableEndComponent, passOverGenerationFindGroupNeuron, &foundGroupNeuron, &foundCoverageVariableEndComponent, &listOfHighLevelNeurons))
					{
						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_FIRST_AND_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE
						SANIGroupNeuralNetwork* variableEndComponentNeuron = listOfHighLevelNeurons.back();
						#endif
						listOfHighLevelNeurons.clear();
						
						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_FIRST_AND_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE_POST
						bool unique = true;
						for(int j=0; j<neuronWithVariableEndComponentStartInput->ANNbackGroupConnectionList.size(); j++)
						{
							SANIGroupNeuralNetwork* neuronWithVariableEndComponentStartInputNeuron = neuronWithVariableEndComponentStartInput->ANNbackGroupConnectionList[j];
							if(neuronWithVariableEndComponentStartInputNeuron == variableEndComponentNeuron)
							{
								unique = false;
							}
						}
						if(unique)
						{
						#endif
							if(foundGroupNeuron)
							{
								#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
								cout << "\t\texecutePrelimPassover: foundGroupNeuron, enableVariableEndComponentDetection = true" << endl;
								#endif
								enableVariableEndComponentDetection = true;	//enable for this round
								if(indexInSequenceStartPrelimPassover == forwardPropogationSentenceData->sentenceContents->size())
								{
									//implied: completedIdentifyingSentenceHighLevelNeurons = true
									enableVariableEndComponentDetectionExpectedCompletedIdentifyingSentenceHighLevelNeurons = true;
								}
								executePrelimPassover = false;
								executePrelimPassoverNextLoopIndex = loopIndex+2;	//stop passover for next 2 iterations
							}
						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_FIRST_AND_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE_POST
						}
						#endif
					}
					
					if(!foundGroupNeuron)
					{
						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_FIRST_AND_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE_PRE
						forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.clear();
						#endif
					}
				}
			}
		}
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
		//real run
		passOverGenerationFindVariableEndComponent = false;
		passOverGenerationFindGroupNeuron = false;
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
		cout << "findAndReconcileIncrementalVariationLimitNumComponentsSection:" << endl;
		cout << "indexInSequenceStart = " << indexInSequenceStart << endl;
		#endif
		if(listOfHighLevelNeurons.size() > 0)
		{
			cerr << "SANIpropagateCompactGenerateClass::findAndReconcileVariationWrapper error; initialised (listOfHighLevelNeurons.size() > 0)" << endl;
			exit(EXIT_ERROR);
		}
		if(findAndReconcileIncrementalVariationLimitNumComponentsSection(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, supportVariableFirstComponents, &indexInSequenceStart, &highLevelNeuronPriorArray, enableVariableEndComponentDetection, enableVariableEndComponentDetectionExpectedCompletedIdentifyingSentenceHighLevelNeurons, passOverGenerationFindVariableEndComponent, passOverGenerationFindGroupNeuron, &foundGroupNeuron, &foundCoverageVariableEndComponent, &listOfHighLevelNeurons))
		#else
		if(findAndReconcileIncrementalVariationLimitNumComponentsSection(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, supportVariableFirstComponents, &indexInSequenceStart, &highLevelNeuronPriorArray))	
		#endif
		#else
		if(findAndReconcileIncrementalVariationLimitNumComponentsSection(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, supportVariableFirstComponents, &indexInSequenceStart, &highLevelNeuronPrior))
		#endif
		{
			resultTemp = true;
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
			loopIndex++;
			#endif
		}
		else
		{
			cerr << "!findAndReconcileIncrementalVariationLimitNumComponentsSection" << endl;
			exit(EXIT_ERROR);
		}
		if(indexInSequenceStart == sentenceContents->size())
		{
			completedSentenceNetworkGeneration = true;
			#ifndef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
			result = resultTemp;
			#endif
		}
		
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_FIRST_AND_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE_PRE
		forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.clear();
		#endif

	}
	
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED_PREVENT_INTRASENTENCE_MATCHING_SECTIONED_RESET_AT_END
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_BASIC
	forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.clear();
	#endif
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT	
	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	SANIGroupNeuralNetwork* highestLayerNeuron = NULL;
	int highestLayerNeuronIndex = 0;
	if(findHighestLayerNeuron(highLevelNeuronPriorArray, &highestLayerNeuron, &highestLayerNeuronIndex))
	{	
		markSubNeuronsReset(highestLayerNeuron);
	}
	else
	{
		cerr << "SANIpropagateCompactGenerateClass::findAndReconcileVariationWrapper error: !findHighestLayerNeuron" << endl;
		exit(EXIT_ERROR);
	}
	#else
	markSubNeuronsReset(*highLevelNeuronPrior);
	#endif	
	#endif
	#endif	
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	#ifdef SANI_SEQUENCE_GRAMMAR_VERIFICATION
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION
	cout << "SANI_SEQUENCE_GRAMMAR_VERIFICATION: performPropagationTest:" << endl;
	#endif
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_SENTENCES
	cout << "\nfindAndReconcileVariationWrapper SANI_SEQUENCE_GRAMMAR_VERIFICATION: performPropagationTest:" << endl;
	#endif
	if(!SANIpropagateCompact.performPropagationTest(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData))
	{
		cerr << "SANIpropagateCompactGenerateClass::findAndReconcileVariationWrapper fail: !performPropagationTest verification" << endl;
		exit(EXIT_ERROR);
		result = false;
	}
	else
	{
		//cout << "SANIpropagateCompactGenerateClass::findAndReconcileVariationWrapper: SANIpropagateCompact.performPropagationTest pass" << endl;
	}	
	#endif
	#endif
	
	
	#else
	
	if(!findAndReconcileVariation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, supportVariableFirstComponents))
	{
		result = false;
	}
	
	#endif
	
	return result;
}
			


#ifndef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
bool SANIpropagateCompactGenerateClass::findAndReconcileVariationLimitNumComponentsSection(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_AFTER_GENERATION
	if(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal <= translatorVariables->maxNumberSentences/2)
	{
	#endif
		bool foundAndReconciledMissingOrDifferentIncrementalNeurons = findAndReconcileIncrementalVariationLimitNumComponentsSection(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, supportVariableFirstComponents, indexInSequenceStart, highLevelNeuronPrior);
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_AFTER_GENERATION
	}
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_VERIFICATION
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION
	cout << "SANI_SEQUENCE_GRAMMAR_VERIFICATION: performPropagationTest:" << endl;
	#endif
	if(!SANIpropagateCompact.performPropagationTest(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData))
	{
		cerr << "SANIpropagateCompactGenerateClass::findAndReconcileVariationLimitNumComponentsSection fail: !performPropagationTest verification" << endl;
		exit(EXIT_ERROR);
		result = false;
	}	
	#endif

	return result;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
bool SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariationLimitNumComponentsSection(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, const bool enableVariableEndComponentDetection, const bool enableVariableEndComponentDetectionExpectedCompletedIdentifyingSentenceHighLevelNeurons, const bool passOverGenerationFindVariableEndComponent, const bool passOverGenerationFindGroupNeuron, bool* foundGroupNeuron, bool* foundCoverageVariableEndComponent, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons)
#else
bool SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariationLimitNumComponentsSection(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray)
#endif
#else
bool SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariationLimitNumComponentsSection(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior)
#endif
{
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariation" << endl;
	#endif
		
	bool foundAndReconciledMissingOrDifferentIncrementalNeurons = false;
	
	
	//if SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each wordIndex only
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	vector<vector<SANIGroupNeuralNetwork*>*> firstLayer;
	for(int i=0; i<forwardPropogationSentenceData->sentenceContents.size(); i++)
	{
		firstLayer.push_back(new vector<SANIGroupNeuralNetwork*>);
	}
	#else
	vector<SANIGroupNeuralNetwork*> firstLayer;
	#endif
	forwardPropogationSentenceData->firstLayer = &firstLayer;
	SANIpropagateCompact.defineFirstLayer(translatorVariables, forwardPropogationSentenceData);
	
	#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_FIRST_AND_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE_PRE
	#ifndef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED_PREVENT_INTRASENTENCE_MATCHING_SECTIONED_RESET_AT_END
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_BASIC
	forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.clear();
	#endif
	#endif
	#endif
	#endif
	
	SANIpropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	
	int indexInSequence = *indexInSequenceStart;
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << "(*(forwardPropogationSentenceData->sentenceContents))[*indexInSequenceStart]->tagName = " << (*(forwardPropogationSentenceData->sentenceContents))[*indexInSequenceStart]->tagName << endl;
	#endif
	
	bool stillIdentifyingHighLevelNeurons = true;
	bool completedIdentifyingSentenceHighLevelNeurons = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	if(enableVariableEndComponentDetectionExpectedCompletedIdentifyingSentenceHighLevelNeurons)
	{
		completedIdentifyingSentenceHighLevelNeurons = true;
	}
	#endif
	bool toplevelGroupActivationFound = false;

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeuronsCurrent = listOfHighLevelNeurons;
	#else
	vector<SANIGroupNeuralNetwork*> listOfHighLevelNeurons1;	//vectorQ
	vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeuronsCurrent = &listOfHighLevelNeurons1;
	#endif
	bool creatingNewNeuronSequence1 = false;	//this is used to ensure that only a single neuron is created to store the unrecognised sequence (not 1 neuron per word of the sequence)
	int neuronSequenceIndex1 = 0;
	SANIGroupNeuralNetwork* newNeuronSequenceGroup1 = NULL;
		
		
	int loopIndex = 0;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
	forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = false;	//see SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady: if(forwardPropogationSignalData->firstIndexInSequence == 0) check
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
	forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent = false;
	#endif
			
	while(stillIdentifyingHighLevelNeurons)
	{	
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
		#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
		if(enableVariableEndComponentDetection || passOverGenerationFindVariableEndComponent)
		{
			forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent = true;
		}
		#else
		forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent = true;	//not supported
		#endif
		#else
		cerr << "recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent has only been coded for SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED" << endl;
		#endif
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
		#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
		if(*indexInSequenceStart != 0)
		{
			forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = true;	
		}
		else
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
			//first 1 loop does not support recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent
			#else
			//first 2 loops do not support recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent
			#endif
		}
		#else
		#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		if(loopIndex == 1)	//only allow recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent for second+ neuron in listOfHighLevelNeurons1
		{
			forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = true;
		}
		#else
		if(loopIndex == 2)	//only allow recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent for third+ neuron in listOfHighLevelNeurons1
		{
			forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = true;
		}
		#endif
		#endif

		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING_COMPREHENSIVE
		#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE_SIZEABLE_SUBTREE
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent)
		{
			if(highLevelNeuronPriorArray->size() > 0)	//redundant
			{
				SANIGroupNeuralNetwork* lowestLayerNeuron = NULL;
				int lowestLayerNeuronIndex = 0;
				bool foundLowestLayerNeuron = findLowestLayerNeuron(highLevelNeuronPriorArray, &lowestLayerNeuron, &lowestLayerNeuronIndex);
				if(foundLowestLayerNeuron)
				{
					if(lowestLayerNeuron->inputLayerNeuron)
					{
						forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = false;
					}
				}
			}
		}
		#endif
		#endif
		#endif

		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
		if(enableVariableEndComponentDetection)
		{
			forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = false;
		}
		else
		{
			if(passOverGenerationFindVariableEndComponent)
			{

			}
			if(passOverGenerationFindGroupNeuron)
			{
				forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = false;
			}
		}
		#else
		//rule #1;
		#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		SANIGroupNeuralNetwork* highLevelNeuronPriorTemp = NULL;
		int lowestLayerNeuronIndex = 0;
		bool foundLowestLayerNeuron = findLowestLayerNeuron(highLevelNeuronPriorArray, &highLevelNeuronPriorTemp, &lowestLayerNeuronIndex);
		#else
		SANIGroupNeuralNetwork* highLevelNeuronPriorTemp = *highLevelNeuronPrior;
		#endif	
		if((highLevelNeuronPriorTemp != NULL) && (highLevelNeuronPriorTemp->activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp))
		{
			//previous high level neuron found has missing end component
			forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = false;	//CHECKTHIS
			forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent = false;	//CHECKTHIS
		}
		#endif
		#endif
		
		
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH
		#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		forwardPropogationSentenceData->variableFirstComponentMaxDepth = calculateLowestLayerNeuronMaxDepth(highLevelNeuronPriorArray);
		#else
		#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
		int variableFirstComponentMaxDepthModifier;		
		if(*indexInSequenceStart == 0)
		{
			variableFirstComponentMaxDepthModifier = 0;
			if(loopIndex > 0)
			{
				variableFirstComponentMaxDepthModifier = 1;	//increase the max depth by 1 for every new candidate neuron added to the generated subnet (note the first layer is connected to 2 input neurons of same depth)
			}
		}
		else
		{
			variableFirstComponentMaxDepthModifier = 1;	//increase the max depth by 1 for every new candidate neuron added to the generated subnet (note the first layer is connected to 2 input neurons of same depth)
		}
		#else
		int variableFirstComponentMaxDepthModifier = 0;
		if(loopIndex > 0)
		{
			variableFirstComponentMaxDepthModifier = 1;	//increase the max depth by 1 for every new candidate neuron added to the generated subnet (note the first layer is connected to 2 input neurons of same depth)
		}
		#endif
		#endif
		#endif
		#endif
			
		int currentFirstInputNeuronIndexInSequence;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			currentFirstInputNeuronIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1-indexInSequence;
		}
		else
		{
			currentFirstInputNeuronIndexInSequence = indexInSequence;
		}

		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		cout << "\e[35m \n** loopIndex = " << loopIndex << ", indexInSequence = " << indexInSequence << ", word = " << (forwardPropogationSentenceData->sentenceContents)->at(currentFirstInputNeuronIndexInSequence)->tagName << " \e[0m" << endl;
		#endif

		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		//if SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each wordIndex only
		cerr << "SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariation hasnt yet been coded for SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
		exit(EXIT_ERROR);
		#else
		SANIGroupNeuralNetwork* currentLayerNeuronGroupStart = (firstLayer)[currentFirstInputNeuronIndexInSequence];
		#endif
		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
		cout << "SANIpropagateCompactGenerateClass::stillIdentifyingHighLevelNeurons" << endl;
		#endif
		
		forwardPropogationSentenceData->toplevelGroupActivationFound = false;

		forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage = true;
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage = NULL;
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial = false;
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent = false;
		#endif
		
		
		bool resetAllNeuronComponents = true;
		if(SANIpropagateCompact.performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, false, indexInSequence))
		{
			toplevelGroupActivationFound = true;	//note this will only be set true if the entire sentence word indices have been covered (ie only when currentFirstInputNeuronIndexInSequence=start)
		}
		

		#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
		if(!(SANIpropagateCompact.verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) && (!forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial)))		//(!forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial) should be enforced via SANI_SEQUENCE_GRAMMAR_IGNORE_PARTIAL_DETECTIONS
		#else
		if(!SANIpropagateCompact.verifyActivatedNeuronsAtLeastOne(forwardPropogationSentenceData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, newNeuronSequenceGroup1))
		#endif
		{
			//case a	
			#ifdef SANI_DEBUG_FORMATION2
			cout << "A createOrAppendFirstLevelHiddenLayerGroup, indexInSequence = " << indexInSequence << ", groupIndex = " << currentLayerNeuronGroupStart->groupIndex << ", GIApreprocessorPOStypeNameArray[currentLayerNeuronGroupStart->wordPOStype] = " << GIApreprocessorPOStypeNameArray[currentLayerNeuronGroupStart->wordPOStype] << endl;
			#endif		
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
			cout << "A createOrAppendFirstLevelHiddenLayerGroup, indexInSequence = " << indexInSequence << ", groupIndex = " << currentLayerNeuronGroupStart->groupIndex << ", GIApreprocessorPOStypeNameArray[currentLayerNeuronGroupStart->wordPOStype] = " << GIApreprocessorPOStypeNameArray[currentLayerNeuronGroupStart->wordPOStype] << endl;
			#endif
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
			cout << "A createOrAppendFirstLevelHiddenLayerGroup, indexInSequence = " << indexInSequence << ", groupIndex = " << currentLayerNeuronGroupStart->groupIndex << ", GIApreprocessorPOStypeNameArray[currentLayerNeuronGroupStart->wordPOStype] = " << GIApreprocessorPOStypeNameArray[currentLayerNeuronGroupStart->wordPOStype] << endl;
			#endif
			
			foundAndReconciledMissingOrDifferentIncrementalNeurons = true;

			createOrAppendFirstLevelHiddenLayerGroup(SANIGroupTypes, forwardPropogationSentenceData, currentLayerNeuronGroupStart, &creatingNewNeuronSequence1, &neuronSequenceIndex1, &newNeuronSequenceGroup1, listOfHighLevelNeuronsCurrent);

			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH	
			#ifndef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
			forwardPropogationSentenceData->variableFirstComponentMaxDepth = forwardPropogationSentenceData->variableFirstComponentMaxDepth + variableFirstComponentMaxDepthModifier;
			#endif
			#endif

			indexInSequence++;
		}
		else
		{
			creatingNewNeuronSequence1 = false;
			neuronSequenceIndex1 = 0;
			foundAndReconciledMissingOrDifferentIncrementalNeurons = true;

			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
			if(!(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial))
			{	
			#endif
				//case c
				//fully activated group coverage+weight is > partially activated group coverage+weight
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
				cout << "B addNeuronToList, indexInSequence = " << indexInSequence  << ", nextIndexInSequence = " << calculateNextIndexInSequence(forwardPropogationSentenceData) << ", groupIndexes = " << SANIpropagateOperations.printParseTreeGroupIndicesFlat(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) << endl;
				//cout << "\tforwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent = " << forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent << endl;			
				//SANIpropagateOperations.printParseTreeGroupIndices(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, 0);
				//SANIpropagateOperations.printParseTree(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, 0);
				#endif
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
				cout << "B addNeuronToList, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex = " << forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex << endl;
				//cout << "\tSANInodes.countParseTreeLeafSize(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) = " << SANInodes.countParseTreeLeafSize(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) << endl;
				//cout << "\tforwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent = " << forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent << endl;
				#endif
				#ifdef SANI_DEBUG_FORMATION2
				cout << "B addNeuronToList, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex = " << forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex << endl;
				//SANIpropagateOperations.printParseTree(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, 0);
				#endif

				addNeuronToList(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef, &indexInSequence);

				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
				if(passOverGenerationFindGroupNeuron)
				{
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
					if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent)
					{
						cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT: SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: passOverGenerationFindGroupNeuron && (forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent)" << endl;
						exit(EXIT_ERROR);				
					}
					#endif
					*foundGroupNeuron = true;
				}
				else
				{
				#endif
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent;
					#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
				}
				#endif
				
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
			}
			else
			{
				//case b
				//partially activated group coverage+weight is > fully activated group coverage+weight
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
				cout << "C addNeuronToList, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex = " << forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex << endl;
				#endif
				#ifdef SANI_DEBUG_FORMATION2
				cout << "C addNeuronToList, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex = " << forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex << endl;
				#endif
				
				addNeuronToList(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef, &indexInSequence);

				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
				if(passOverGenerationFindVariableEndComponent)
				{
					//cout << "passOverGenerationFindVariableEndComponent, foundCoverageVariableEndComponent = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial" << endl;
					
					*foundCoverageVariableEndComponent = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial;
				}
				else
				{
				#endif
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
					cout << "set activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp" << endl;
					#endif
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial;	//always true
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
				}
				#endif
				#endif
			}
			#endif
			
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH
			#ifndef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
			#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
			int matchedNeuronDepth = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->networkDepth;
			#else
			int matchedNeuronDepth = SANIpropagateOperations.calculateDepthFromBinaryTreeLeafSize(SANInodes.countParseTreeLeafSize(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage));
			#endif
			forwardPropogationSentenceData->variableFirstComponentMaxDepth = max((forwardPropogationSentenceData->variableFirstComponentMaxDepth + variableFirstComponentMaxDepthModifier), matchedNeuronDepth);
			#endif
			#endif
				
		}			
		
		#ifdef SANI_FREE_MEMORY
		SANIpropagateOperations.deinitialiseParseTreeGroupList(SANIGroupTypes, parseTreeGroupListPointer);
		#endif
		
		stillIdentifyingHighLevelNeurons = false;
		if(indexInSequence < forwardPropogationSentenceData->sentenceContents->size())
		{
			#ifndef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
			#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
			if((*indexInSequenceStart == 0) && (loopIndex < 1))
			{
				//must find 2 neurons for first layer in sentence network
				stillIdentifyingHighLevelNeurons = true;
			}
			#else
			stillIdentifyingHighLevelNeurons = true;	
			//see how many words successfully propagated through the next time		
			#endif
			#endif
		}
		else
		{			
			completedIdentifyingSentenceHighLevelNeurons = true;
		}
		
		loopIndex++;
	}

	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED	
	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	if(*indexInSequenceStart == 0)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
		if(completedIdentifyingSentenceHighLevelNeurons && !enableVariableEndComponentDetectionExpectedCompletedIdentifyingSentenceHighLevelNeurons)
		#else
		if(completedIdentifyingSentenceHighLevelNeurons)
		#endif
		{
			cerr << "SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: matched whole subset in network, prior test should have passed" << endl;
			cerr << "SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE: SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: (*indexInSequenceStart == 0) && completedIdentifyingSentenceHighLevelNeurons" << endl;
			exit(EXIT_ERROR);
		}
	}
	#else
	if((*indexInSequenceStart == 0))
	{
		if((loopIndex == 1) || (listOfHighLevelNeuronsCurrent->size() == 1))
		{
			cerr << "SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: matched whole subset in network, prior test should have passed" << endl;
			cerr << "SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_ELONGATED_TREE: SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: (*indexInSequenceStart == 0) && ((loopIndex == 1) || (listOfHighLevelNeuronsCurrent->size() == 1))" << endl;
			exit(EXIT_ERROR);
		}
	}
	#endif
	#endif
			
				
	SANIGroupNeuralNetwork* grammaticalSentenceNeuron = NULL;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	if(passOverGenerationFindVariableEndComponent || passOverGenerationFindGroupNeuron)
	{
		//pass over generation
	}
	else
	{
	#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
		#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, &grammaticalSentenceNeuron, true, completedIdentifyingSentenceHighLevelNeurons, *indexInSequenceStart, highLevelNeuronPriorArray);			
		#else
		connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, &grammaticalSentenceNeuron, true, completedIdentifyingSentenceHighLevelNeurons, *indexInSequenceStart, highLevelNeuronPrior);
		#endif
		#else
		connectListOfHighLevelNeuronsToNewNeuron(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, &grammaticalSentenceNeuron, true);
		#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	}
	#endif
	
	#ifndef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED_PREVENT_INTRASENTENCE_MATCHING_SECTIONED_RESET_AT_END
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	//NA
	#else
	SANIGroupNeuralNetwork* highestLayerNeuron = NULL;
	int highestLayerNeuronIndex = 0;
	if(findHighestLayerNeuron(highLevelNeuronPriorArray, &highestLayerNeuron, &highestLayerNeuronIndex))
	{	
		markSubNeuronsReset(highestLayerNeuron);
	}
	else
	{
		cerr << "SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: !findHighestLayerNeuron" << endl;
		exit(EXIT_ERROR);
	}
	#endif
	#else
	markSubNeuronsReset(*highLevelNeuronPrior);
	#endif
	#else
	markSubNeuronsReset(grammaticalSentenceNeuron);		
	#endif	
	#endif
	#endif
	

	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
	*indexInSequenceStart = indexInSequence;
	#endif
	
	forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
	forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = false;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_GENERATION
	SANIpropagateCompact.printSANInetworkSVG(translatorVariables, SANIGroupTypes);
	#endif
	
	return foundAndReconciledMissingOrDifferentIncrementalNeurons;
}


bool SANIpropagateCompactGenerateClass::findAndReconcileVariation(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_AFTER_GENERATION
	if(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal <= translatorVariables->maxNumberSentences/2)
	{
	#endif
		bool foundAndReconciledMissingOrDifferentIncrementalNeurons = findAndReconcileIncrementalVariation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, supportVariableFirstComponents);

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_AFTER_GENERATION
	}
	#endif
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
	cout << "\n\n\n\n\nSANIpropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork 3" << endl;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_VERIFICATION
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION
	cout << "SANI_SEQUENCE_GRAMMAR_VERIFICATION: performPropagationTest:" << endl;
	#endif
	if(!SANIpropagateCompact.performPropagationTest(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData))
	{
		cerr << "SANIpropagateCompactGenerateClass::findAndReconcileVariation fail: !performPropagationTest verification" << endl;
		exit(EXIT_ERROR);
		result = false;
	}	
	#endif
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
	cout << "SANIpropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork 4" << endl;
	#endif

	return result;
}

bool SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariation(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents)
{
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariation" << endl;
	#endif
		
	bool foundAndReconciledMissingOrDifferentIncrementalNeurons = false;
	
	
	//if SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each wordIndex only
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	vector<vector<SANIGroupNeuralNetwork*>*> firstLayer;
	for(int i=0; i<forwardPropogationSentenceData->sentenceContents.size(); i++)
	{
		firstLayer.push_back(new vector<SANIGroupNeuralNetwork*>);
	}
	#else
	vector<SANIGroupNeuralNetwork*> firstLayer;
	#endif
	forwardPropogationSentenceData->firstLayer = &firstLayer;
	SANIpropagateCompact.defineFirstLayer(translatorVariables, forwardPropogationSentenceData);
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_BASIC
	forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.clear();
	#endif
	#endif
	
	/*
	algorithm2;
	let l be the first word in the sentence word sequence
	for every input neuron in sentence word sequence starting at l*
		propagate its input through the network
			ensure word index is contiguous during forward prop (SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT: SANIpropagateOperations.componentWordConnectivityTestsWrapper)
			mark all hidden neurons that fire (these are candidates)
			mark all hidden neurons that are only partially activated
				these become candidates for splitting
				never split a neuron below 2 components
			take the hidden neuron that support the most words in the sentence from l (whether it is fully activated or partially activated)
				if necessary (it is partially activated) split the neuron;
					b) take its components and forge a new neuron
					add to vector q
				c) if it is fully activated add to vector q
				then repeat* from last word index (l)
		if no appropriate hidden neuron is found create a a) new hidden neuron containing the first input in the current part of the sentence word sequence*
			add to vector q
			OLD: how to determine whether to use this new neuron or just connect its first input to..
			then repeat* from last word index (l)
	combine all the a) new, b) splitted, or c) fully activated neurons into a d) new neuron on a higher layer
		this new higher layer neuron d) represents a successfully parsed sentence m
			OPTIONAL: add this neuron to the highest level full gramatical sentence neuron
	issue with this algorithm is that it cannot reuse groups for embeded optional components eg rcmod;
		1 the dog barks
		2 the dog that is near the house barks
		will create 2 independent top level groups (the dog barks group will not be reused by 2)
		possibly look for and catch recursive group connectivity and ensure that groups can only support varied word index ranges when they refer to recursive (optional) groups
	*/
	
	SANIpropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	//SANIGroupNeuralNetwork* firstLayerNeuronGroupStart = NULL;
	
	int indexInSequence = 0;
	bool stillIdentifyingHighLevelNeurons = true;
	bool toplevelGroupActivationFound = false;

	vector<SANIGroupNeuralNetwork*> listOfHighLevelNeurons1;	//vectorQ
	vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeuronsCurrent = &listOfHighLevelNeurons1;
	bool creatingNewNeuronSequence1 = false;	//this is used to ensure that only a single neuron is created to store the unrecognised sequence (not 1 neuron per word of the sequence)
	int neuronSequenceIndex1 = 0;
	SANIGroupNeuralNetwork* newNeuronSequenceGroup1 = NULL;

	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
	bool findingCandidateComponent1 = true;
	bool findingCandidateComponent2 = false;
	bool findingCentralVariation = false;
	int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = INT_DEFAULT_VALUE;	//for central variation detection only
	int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = INT_DEFAULT_VALUE;	//for central variation detection only
	SANIGroupParseTree* candidateNeuronBaseParseTree = NULL;
	vector<SANIGroupNeuralNetwork*> listOfHighLevelNeuronsCentralVariation;
	#endif
		
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH	
	forwardPropogationSentenceData->variableFirstComponentMaxDepth = 0;
	#endif
		
	int loopIndex = 0;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = false;	//see SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady: if(forwardPropogationSignalData->firstIndexInSequence == 0) check
	#endif
		
	while(stillIdentifyingHighLevelNeurons)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
		cout << "(*(forwardPropogationSentenceData->sentenceContents))[indexInSequence]->tagName = " << (*(forwardPropogationSentenceData->sentenceContents))[indexInSequence]->tagName << endl;
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
		if(loopIndex == 2)	//only allow recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent for third+ neuron in listOfHighLevelNeurons1
		{
			forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = true;
		}
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH
		int variableFirstComponentMaxDepthModifier = 0;
		if(loopIndex > 0)
		{
			variableFirstComponentMaxDepthModifier = 1;
		}
		#endif
		#else
		if(loopIndex > 0)
		{
			forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = true;
		}
		#endif
		#endif
			
		int currentFirstInputNeuronIndexInSequence;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			currentFirstInputNeuronIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1-indexInSequence;
		}
		else
		{
			currentFirstInputNeuronIndexInSequence = indexInSequence;
		}

		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		cout << "\e[35m \n** loopIndex = " << loopIndex << ", indexInSequence = " << indexInSequence << ", word = " << (forwardPropogationSentenceData->sentenceContents)->at(currentFirstInputNeuronIndexInSequence)->tagName << " \e[0m" << endl;
		#endif

		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		//if SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each wordIndex only
		cerr << "SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariation hasnt yet been coded for SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
		exit(EXIT_ERROR);
		#else
		SANIGroupNeuralNetwork* currentLayerNeuronGroupStart = (firstLayer)[currentFirstInputNeuronIndexInSequence];
		#endif
		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
		cout << "SANIpropagateCompactGenerateClass::stillIdentifyingHighLevelNeurons" << endl;
		#endif
		
		forwardPropogationSentenceData->toplevelGroupActivationFound = false;

		forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage = true;
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage = NULL;
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial = false;
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent = false;
		#endif
		
						
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
		if(findingCentralVariation)
		{
			bool resetAllNeuronComponents = true;
			if(SANIpropagateCompact.performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, false, indexInSequence, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed))
			{
				toplevelGroupActivationFound = true;	//note this will only be set true if the entire sentence word indices have been covered (ie only when currentFirstInputNeuronIndexInSequence=start)
			}
		}
		else
		{
		#endif			
			bool resetAllNeuronComponents = true;
			if(SANIpropagateCompact.performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, false, indexInSequence))
			{
				toplevelGroupActivationFound = true;	//note this will only be set true if the entire sentence word indices have been covered (ie only when currentFirstInputNeuronIndexInSequence=start)
			}
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
		}
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
		if(findingCandidateComponent2)
		{
			findingCandidateComponent2 = false;
			
			if(forwardPropogationSentenceData->foundCandidateComponent2)
			{
				listOfHighLevelNeuronsCurrent->push_back(candidateNeuronBaseParseTree->groupRef);

				SANIGroupParseTree* candidateComponent2sourceParseTreeGroup = forwardPropogationSentenceData->candidateComponent2sourceParseTreeGroup;

				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = candidateNeuronBaseParseTree->parseTreeMinWordIndex-1;
					activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = candidateComponent2sourceParseTreeGroup->parseTreeMaxWordIndex+1;
				}
				else
				{
					activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = candidateNeuronBaseParseTree->parseTreeMaxWordIndex+1;
					activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = candidateComponent2sourceParseTreeGroup->parseTreeMinWordIndex-1;
				}
				
				if(activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed == activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
				{
					//perfect match detected between activated section of candidateNeuronBaseParseTree and candidateComponent2sourceParseTreeGroup, set component2 of candidateNeuronBase as optional
				}
				else
				{
					findingCentralVariation = true;
					listOfHighLevelNeuronsCurrent = &listOfHighLevelNeuronsCentralVariation;
				}
			}
			else
			{
				SANIGroupNeuralNetwork* newNeuron = splitNeuron(SANIGroupTypes, forwardPropogationSentenceData, candidateNeuronBaseParseTree);	//listOfHighLevelNeuronsCurrent CHECKTHIS
				listOfHighLevelNeuronsCurrent->push_back(newNeuron);
								
				//go back to standard findAndReconcileIncrementalVariation routine
			}
			
			forwardPropogationSentenceData->findingCandidateComponent2 = false;
			forwardPropogationSentenceData->candidateComponent2ToFind = NULL;
			candidateNeuronBaseParseTree = NULL;
		}
		else
		{
		#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
			if(!(SANIpropagateCompact.verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) && (!forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial)))		//(!forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial) should be enforced via SANI_SEQUENCE_GRAMMAR_IGNORE_PARTIAL_DETECTIONS
			#else
			if(!SANIpropagateCompact.verifyActivatedNeuronsAtLeastOne(forwardPropogationSentenceData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, newNeuronSequenceGroup1))
			#endif
			{
				//case a			
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
				cout << "A createOrAppendFirstLevelHiddenLayerGroup, indexInSequence = " << indexInSequence << ", groupIndex = " << currentLayerNeuronGroupStart->groupIndex << endl;
				#endif
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
				cout << "A createOrAppendFirstLevelHiddenLayerGroup, indexInSequence = " << indexInSequence << ", groupIndex = " << currentLayerNeuronGroupStart->groupIndex << endl;
				#endif
				foundAndReconciledMissingOrDifferentIncrementalNeurons = true;

				createOrAppendFirstLevelHiddenLayerGroup(SANIGroupTypes, forwardPropogationSentenceData, currentLayerNeuronGroupStart, &creatingNewNeuronSequence1, &neuronSequenceIndex1, &newNeuronSequenceGroup1, listOfHighLevelNeuronsCurrent);
				
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH	
				forwardPropogationSentenceData->variableFirstComponentMaxDepth = forwardPropogationSentenceData->variableFirstComponentMaxDepth + variableFirstComponentMaxDepthModifier;
				#endif
	
				indexInSequence++;
			}
			else
			{
				creatingNewNeuronSequence1 = false;
				neuronSequenceIndex1 = 0;
				foundAndReconciledMissingOrDifferentIncrementalNeurons = true;

				#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
				if(!(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial))
				{	
				#endif
					//case c
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
					cout << "B addNeuronToList, indexInSequence = " << indexInSequence  << ", nextIndexInSequence = " << calculateNextIndexInSequence(forwardPropogationSentenceData) << ", groupIndexes = " << SANIpropagateOperations.printParseTreeGroupIndicesFlat(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) << endl;
					#endif
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
					cout << "B addNeuronToList, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex = " << forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex << endl;
					cout << "\tSANInodes.countParseTreeLeafSize(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) = " << SANInodes.countParseTreeLeafSize(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) << endl;
					cout << "\tforwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent = " << forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent << endl;
					#endif
					//fully activated group coverage+weight is > partially activated group coverage+weight
					
					addNeuronToList(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef, &indexInSequence);
					
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent;
					#endif
					
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH	
					#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
					int matchedNeuronDepth = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->networkDepth;
					#else
					int matchedNeuronDepth = SANIpropagateOperations.calculateDepthFromBinaryTreeLeafSize(SANInodes.countParseTreeLeafSize(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage));
					#endif
					forwardPropogationSentenceData->variableFirstComponentMaxDepth = max((forwardPropogationSentenceData->variableFirstComponentMaxDepth + variableFirstComponentMaxDepthModifier), matchedNeuronDepth);
					#endif
				
				#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
				}
				else
				{
					//case b
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
					cout << "C splitGroupAtLastActivatedComponent + addNeuronToList, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex = " << forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex << endl;
					#endif
					//partially activated group coverage+weight is > fully activated group coverage+weight
					
					/*//not coded;
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableEndComponent;
					#endif
					*/
						
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
					bool foundLocalVariationCandidate = false;
					if(findingCandidateComponent1)
					{
						SANIComponentNeuralNetwork* candidateComponent2 = NULL; 
						candidateNeuronBaseParseTree = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage;
						if(verifyAndMarkNeuronAsCandidateWrapper(forwardPropogationSentenceData, candidateNeuronBaseParseTree, &candidateComponent2))
						{
							foundLocalVariationCandidate = true;
							findingCandidateComponent1 = false;
							findingCandidateComponent2 = true;
							forwardPropogationSentenceData->findingCandidateComponent2 = true;
							forwardPropogationSentenceData->candidateComponent2ToFind = candidateComponent2;
						}
					}
					if(!foundLocalVariationCandidate)
					{
					#endif
						SANIGroupNeuralNetwork* newNeuron = splitNeuron(SANIGroupTypes, forwardPropogationSentenceData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage);
	
						addNeuronToList(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, newNeuron, &indexInSequence);

						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
						newNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageMissingStartComponent;	//NOTUSED (if used requires resetting)
						#endif
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
					}
					#endif
				}
				#endif
			}			

			#ifdef SANI_FREE_MEMORY
			SANIpropagateOperations.deinitialiseParseTreeGroupList(SANIGroupTypes, parseTreeGroupListPointer);
			#endif
			
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
			if(findingCentralVariation)
			{
				if(currentFirstInputNeuronIndexInSequence == activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
				{
					SANIGroupNeuralNetwork* grammaticalSentenceNeuron = NULL;
					if(connectListOfHighLevelNeuronsToNewNeuron(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, &grammaticalSentenceNeuron, false))
					{
						listOfHighLevelNeurons1.push_back(grammaticalSentenceNeuron);
					}
					findingCentralVariation = false;
					findingCandidateComponent1 = true;
					listOfHighLevelNeuronsCurrent = &listOfHighLevelNeurons1;
					activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = INT_DEFAULT_VALUE;
					activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = INT_DEFAULT_VALUE;
					//candidateNeuronBaseParseTree = NULL;	//already done
				}
			}
			#endif

		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
		}
		#endif
		
		if(indexInSequence < forwardPropogationSentenceData->sentenceContents->size())
		{
			stillIdentifyingHighLevelNeurons = true;	
			//see how many words successfully propagated through the next time
		}
		else
		{
			stillIdentifyingHighLevelNeurons = false;
		}	
		
		loopIndex++;
	}
	
	if(loopIndex == 1)
	{
		cerr << "SANIpropagateCompactGenerateClass::findAndReconcileIncrementalVariation error: matched whole subset in network, prior test should have passed" << endl;
		exit(EXIT_ERROR);
	}
		
	SANIGroupNeuralNetwork* grammaticalSentenceNeuron = NULL;
	connectListOfHighLevelNeuronsToNewNeuron(SANIGroupTypes, forwardPropogationSentenceData, &listOfHighLevelNeurons1, &grammaticalSentenceNeuron, true);
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	markSubNeuronsReset(grammaticalSentenceNeuron);	
	#endif

	forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = false;
	#endif
		
	return foundAndReconciledMissingOrDifferentIncrementalNeurons;
}

#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
SANIGroupNeuralNetwork* SANIpropagateCompactGenerateClass::splitNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* partiallyActivatedNeuronWithMaxWordIndexCoverage)
{	
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	int indexToSplitVector1 = INT_DEFAULT_VALUE;
	int indexToSplitVector2 = INT_DEFAULT_VALUE;			
	SANIpropagateCompact.identifyComponentIndexFirstAndLastActivatedIndexUnordered(forwardPropogationSentenceData, partiallyActivatedNeuronWithMaxWordIndexCoverage, &indexToSplitVector1, &indexToSplitVector2);
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent)
	{
		//ensure not to split off the first variable component (keep it in the group)
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			indexToSplitVector2 = partiallyActivatedNeuronWithMaxWordIndexCoverage->groupRef->components.size()-1;
		}
		else
		{
			indexToSplitVector1 = 0;
		}
	}
	#endif
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "indexToSplitVector1 = " << indexToSplitVector1 << endl;
	cout << "indexToSplitVector2 = " << indexToSplitVector2 << endl;				
	#endif				
	SANIGroupNeuralNetwork* newNeuron = splitGroupAtLastActivatedComponentUnordered(forwardPropogationSentenceData, SANIGroupTypes, partiallyActivatedNeuronWithMaxWordIndexCoverage->groupRef, indexToSplitVector1, indexToSplitVector2);	//note passing partiallyActivatedNeuronWithMaxWordIndexCoverage instead of partiallyActivatedNeuronWithMaxWordIndexCoverage->groupRef because groupRef's activations may have been overwritten if the neuron was reset after being saved to partiallyActivatedNeuronWithMaxWordIndexCoverage
	#else
	int indexToSplitVector = INT_DEFAULT_VALUE;
	SANIpropagateCompact.identifyComponentIndexLastActivatedIndex(forwardPropogationSentenceData, partiallyActivatedNeuronWithMaxWordIndexCoverage, &indexToSplitVector);
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "indexToSplitVector = " << indexToSplitVector << endl;				
	#endif
	SANIGroupNeuralNetwork* newNeuron = splitGroupAtLastActivatedComponent(forwardPropogationSentenceData, SANIGroupTypes, partiallyActivatedNeuronWithMaxWordIndexCoverage->groupRef, indexToSplitVector);	//note passing partiallyActivatedNeuronWithMaxWordIndexCoverage instead of partiallyActivatedNeuronWithMaxWordIndexCoverage->groupRef because groupRef's activations may have been overwritten if the neuron was reset after being saved to partiallyActivatedNeuronWithMaxWordIndexCoverage
	#endif
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex = " << partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex << endl;
	cout << "partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex = " << partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex << endl;
	//cout << "indexInSequence = " << indexInSequence << endl;
	#endif				
	
	return newNeuron;
}
#endif

	
bool SANIpropagateCompactGenerateClass::addNeuronToList(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence)
{	
	bool result = true;
	
	*indexInSequence = calculateNextIndexInSequence(forwardPropogationSentenceData);
		
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	markSubNeurons(neuron);	
	#else
	addSubNeuronsToList(&(forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory), neuron);	//add subneurons to listOfHighLevelNeuronsCompleteHistory before connecting its first component to listOfHighLevelNeurons heirachy
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	addSubNeuronsToListReset(neuron);
	#endif
	#endif
	#else
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	neuron->marked = true;
	#else
	forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.push_back(neuron);
	#endif
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_BASIC	//SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_DO_NOT_HAVE_CONNECTIONS_TO_ANY_NEW_CANDIDATE has not been implemented for SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICENT
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_DO_NOT_HAVE_CONNECTIONS_TO_ANY_NEW_CANDIDATE
	addParentNeuronsToList(&(forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory), neuron);
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	addParentNeuronsToListReset(neuron);
	#endif
	#endif
	#endif
	
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
	//why doesnt the following "if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent)" code work?
	#else
	if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		cout << "(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent)" << endl;
		#endif
		
		int variableComponentInGroup = 0;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			variableComponentInGroup = neuron->components.size() - 1;
		}
		else
		{
			variableComponentInGroup = 0;
		}
		SANIComponentNeuralNetwork* variableComponent = neuron->components[variableComponentInGroup];
		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		for(int l=0; l<variableComponent->ANNbackGroupConnectionList.size(); l++)
		{
			SANIGroupNeuralNetwork* groupSource = variableComponent->ANNbackGroupConnectionList[l];
			if(groupSource->groupTypeIsString)
			{
				cout << "variableComponent groupSource->wordPOStype = " << groupSource->wordPOStype << endl;
			}
			else
			{
				cout << "variableComponent groupSource->groupIndex = " << groupSource->groupIndex << endl;
			}
		}
		#endif
		
		
		SANIGroupNeuralNetwork* grammaticalSentenceNeuron = NULL;
		if(connectListOfHighLevelNeuronsToNewNeuron(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeurons, &grammaticalSentenceNeuron, false))
		{
			listOfHighLevelNeurons->clear();
			addComponentToGroup(forwardPropogationSentenceData, grammaticalSentenceNeuron, variableComponent);
		}
	}
	#endif
	
	#endif

	listOfHighLevelNeurons->push_back(neuron);
	
	
	return result;
}



#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
bool SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, const bool completedIdentifyingSentenceHighLevelNeurons, int indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << "connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection = ";
	#endif
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TRACE
	cout << "connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection" << endl;
	cout << "listOfHighLevelNeurons->size() = " << listOfHighLevelNeurons->size() << endl;
	#endif
	
	result = true;

	SANIGroupNeuralNetwork* currentHighLevelNeuron = (*listOfHighLevelNeurons)[0];
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << currentHighLevelNeuron->groupIndex << " ";
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	//rule #3;
	SANIGroupNeuralNetwork* variableEndComponentNeuron = NULL;
	bool foundTempVariableEndComponentNeuron = findTempVariableEndComponentNeuron(highLevelNeuronPriorArray, &variableEndComponentNeuron);
	if(foundTempVariableEndComponentNeuron)
	{	
		SANIGroupNeuralNetwork* grammaticalSentenceNeuronSubHigher = variableEndComponentNeuron;
		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
		cout << "lowestLayerNeuron->activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp" << endl;
		cout << "grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
		cout << "currentHighLevelNeuron->groupIndex = " << currentHighLevelNeuron->groupIndex << endl;
		#endif
		
		if(currentHighLevelNeuron->inputLayerNeuron)
		{
			cerr << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS: SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection: (currentHighLevelNeuron->inputLayerNeuron)" << endl;
			exit(EXIT_ERROR);
		}
		
		addVariableComponentToGroup(forwardPropogationSentenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSubHigher, false);

		#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
		grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, currentHighLevelNeuron->networkDepth);
		#endif

		grammaticalSentenceNeuronSubHigher->activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp = false;

		#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE_ALWAYS_UPDATE_COMPONENT_INDICES
		SANInodes.updateComponentsOwnerGroupAndIndexes(grammaticalSentenceNeuronSubHigher, &(grammaticalSentenceNeuronSubHigher->components), false, NULL);
		#endif
	}
	else if(currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
		cout << "(currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp)" << endl;
		cout << "currentHighLevelNeuron->groupIndex = " << currentHighLevelNeuron->groupIndex << endl;
		#endif
		
		int lowestLayerNeuronIndex = 0;
		updateHighLevelNeuronHierachy(SANIGroupTypes, forwardPropogationSentenceData, highLevelNeuronPriorArray, lowestLayerNeuronIndex, currentHighLevelNeuron, completedIdentifyingSentenceHighLevelNeurons);

		SANIGroupNeuralNetwork* variableEndComponentNeuron = currentHighLevelNeuron;
		setTempVariableEndComponentNeuron(highLevelNeuronPriorArray, variableEndComponentNeuron);
	}
	else
	{
	#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
		if(currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp)
		{
			SANIGroupNeuralNetwork* grammaticalSentenceNeuronSubHigher = NULL;

			SANIGroupNeuralNetwork* lowestLayerNeuron = NULL;
			int lowestLayerNeuronIndex = 0;
			bool foundLowestLayerNeuron = findLowestLayerNeuron(highLevelNeuronPriorArray, &lowestLayerNeuron, &lowestLayerNeuronIndex);

			#ifdef SANI_DEBUG_FORMATION
			cout << "currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp" << endl;
			#endif

			grammaticalSentenceNeuronSubHigher = currentHighLevelNeuron;
			addVariableComponentToGroup(forwardPropogationSentenceData, lowestLayerNeuron, grammaticalSentenceNeuronSubHigher, true);

			#ifdef SANI_DEBUG_FORMATION2
			SANInodes.printParseTreeDebugIndentation(lowestLayerNeuronIndex);
			cout << "addVariableComponentToGroup: lowestLayerNeuron->groupIndex = " << lowestLayerNeuron->groupIndex << " to grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
			#endif

			#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
			grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, lowestLayerNeuron->networkDepth + 1);
			#endif

			currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp = false;

			(*highLevelNeuronPriorArray)[lowestLayerNeuronIndex] = grammaticalSentenceNeuronSubHigher;	//added GIA3k16a
			if(completedIdentifyingSentenceHighLevelNeurons)	//added GIA3k16a
			{
				(*highLevelNeuronPriorArray)[lowestLayerNeuronIndex] = NULL;	//added GIA3k16a
				updateHighLevelNeuronHierachy(SANIGroupTypes, forwardPropogationSentenceData, highLevelNeuronPriorArray, lowestLayerNeuronIndex, grammaticalSentenceNeuronSubHigher, completedIdentifyingSentenceHighLevelNeurons);
			}
			#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE_ALWAYS_UPDATE_COMPONENT_INDICES
			else
			{
				SANInodes.updateComponentsOwnerGroupAndIndexes(grammaticalSentenceNeuronSubHigher, &(grammaticalSentenceNeuronSubHigher->components), false, NULL);
			}
			#endif

		}
		else
		{
		#endif
			int lowestLayerNeuronIndex = 0;
			updateHighLevelNeuronHierachy(SANIGroupTypes, forwardPropogationSentenceData, highLevelNeuronPriorArray, lowestLayerNeuronIndex, currentHighLevelNeuron, completedIdentifyingSentenceHighLevelNeurons);

		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
		}
		#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	}
	#endif
		
	if(completedIdentifyingSentenceHighLevelNeurons)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
		cout << "SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuron - assigning top level sentence neuron;" << endl;
		#endif

		SANIGroupNeuralNetwork* highestLayerNeuron = NULL;
		int highestLayerNeuronIndex = 0;
		bool foundLowestLayerNeuron = findHighestLayerNeuron(highLevelNeuronPriorArray, &highestLayerNeuron, &highestLayerNeuronIndex);
	
		(*grammaticalSentenceNeuron) = highestLayerNeuron;
		#ifdef SANI_ANN_SEGREGATE_TOP_LAYER_NEURONS
		if(createTopLevelNeuron)
		{
			(*grammaticalSentenceNeuron)->topLevelSentenceNeuron = true;
		}
		#endif		
	}

	if(createTopLevelNeuron)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TRACE
		SANIpropagateOperations.traceBackpropNeuralNetwork((*grammaticalSentenceNeuron), 0, -1, -1);
		#endif
	}


	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << endl;
	#endif
	
	return result;
}
#else	//SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_ELONGATED_TREE
bool SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, const bool completedIdentifyingSentenceHighLevelNeurons, int indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << "connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection = ";
	#endif
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TRACE
	cout << "connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection" << endl;
	cout << "listOfHighLevelNeurons->size() = " << listOfHighLevelNeurons->size() << endl;
	#endif
	
	result = true;

	//#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
	SANIGroupNeuralNetwork* grammaticalSentenceNeuronSub = NULL;
	if(indexInSequenceStart == 0)	//(*highLevelNeuronPrior == NULL)
	{
		grammaticalSentenceNeuronSub = createNewHiddenLayerGroup(forwardPropogationSentenceData, SANIGroupTypes);
	}
	else
	{
		grammaticalSentenceNeuronSub = *highLevelNeuronPrior;
	}
	//#endif
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << grammaticalSentenceNeuronSub->groupIndex << " ";
	#endif
	#ifdef SANI_DEBUG_FORMATION
	cout << "\t\t SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuron: grammaticalSentenceNeuronSub->groupIndex = " << grammaticalSentenceNeuronSub->groupIndex << endl;
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
	//rule #3;
	if(grammaticalSentenceNeuronSub->activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp)
	{
		cerr << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS: SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection - doesnt currently support SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_ELONGATED_TREE" << endl;
		exit(EXIT_ERROR);
	}
	else
	{
	#endif
		if(indexInSequenceStart == 0)
		{
			#ifdef SANI_DEBUG_FORMATION
			//cout << "(indexInSequenceStart == 0)" << endl;
			#endif

			for(int k=0; k<listOfHighLevelNeurons->size(); k++)
			{
				#ifdef SANI_DEBUG_FORMATION
				//cout << "k = " << k << endl;
				#endif

				SANIGroupNeuralNetwork* currentHighLevelNeuron = (*listOfHighLevelNeurons)[k];
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
				cout << currentHighLevelNeuron->groupIndex << " ";
				#endif

				int componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
				if(currentHighLevelNeuron->inputLayerNeuron)
				{
					componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING;
				}
				addComponentToGroup(forwardPropogationSentenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSub, componentType, false);

				#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
				grammaticalSentenceNeuronSub->networkDepth = max(grammaticalSentenceNeuronSub->networkDepth, currentHighLevelNeuron->networkDepth + 1);
				#endif
			}	
		}
		else
		{
			SANIGroupNeuralNetwork* currentHighLevelNeuron = (*listOfHighLevelNeurons)[0];
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
			cout << currentHighLevelNeuron->groupIndex << " ";
			#endif

			SANIGroupNeuralNetwork* grammaticalSentenceNeuronSubHigher = NULL;

			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
			if(currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp)
			{	
				grammaticalSentenceNeuronSubHigher = currentHighLevelNeuron;
				addVariableComponentToGroup(forwardPropogationSentenceData, grammaticalSentenceNeuronSub, grammaticalSentenceNeuronSubHigher, true);

				#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
				grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, grammaticalSentenceNeuronSub->networkDepth + 1);
				#endif

				currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp = false;

				grammaticalSentenceNeuronSub = grammaticalSentenceNeuronSubHigher;
			}
			else
			{
			#endif
				grammaticalSentenceNeuronSubHigher = createNewHiddenLayerGroup(forwardPropogationSentenceData, SANIGroupTypes);
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
				cout << grammaticalSentenceNeuronSubHigher->groupIndex << " ";
				#endif
				addComponentToGroup(forwardPropogationSentenceData, grammaticalSentenceNeuronSub, grammaticalSentenceNeuronSubHigher, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);

				#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
				grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, grammaticalSentenceNeuronSub->networkDepth + 1);
				#endif

				int componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
				if(currentHighLevelNeuron->inputLayerNeuron)
				{
					componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING;
				}
				addComponentToGroup(forwardPropogationSentenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSubHigher, componentType, false);

				#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
				grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, currentHighLevelNeuron->networkDepth + 1);
				#endif

				grammaticalSentenceNeuronSub = grammaticalSentenceNeuronSubHigher;

			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
			}
			#endif	

			#ifdef SANI_DEBUG_FORMATION
			cout << "\t\t SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection: grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
			#endif

		}
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
	}
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
	SANInodes.adjustNetworkDepth(grammaticalSentenceNeuronSub);
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	SANInodes.adjustNetworkDepthReset(grammaticalSentenceNeuronSub);
	#endif
	#endif
		
	SANInodes.updateComponentsOwnerGroupAndIndexes(grammaticalSentenceNeuronSub, &(grammaticalSentenceNeuronSub->components), false, NULL);
	

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
	if(grammaticalSentenceNeuronSub->components.size() > SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)
	{
		cerr << "\nSANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS: SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection error: (grammaticalSentenceNeuronSub->components.size() > SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)" << endl;
		cerr << "grammaticalSentenceNeuronSub->components.size() = " << grammaticalSentenceNeuronSub->components.size() << endl;
		exit(EXIT_ERROR);
	}
	if(grammaticalSentenceNeuronSub->components.size() < SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)
	{
		cerr << "\nSANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS: SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection error: (grammaticalSentenceNeuronSub->components.size() < SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)" << endl;
		cerr << "grammaticalSentenceNeuronSub->components.size() = " << grammaticalSentenceNeuronSub->components.size() << endl;
		exit(EXIT_ERROR);
	}
	#endif
		
		
	if(completedIdentifyingSentenceHighLevelNeurons)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
		cout << "SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuron - assigning top level sentence neuron;" << endl;
		#endif
		(*grammaticalSentenceNeuron) = grammaticalSentenceNeuronSub;
		#ifdef SANI_ANN_SEGREGATE_TOP_LAYER_NEURONS
		if(createTopLevelNeuron)
		{
			(*grammaticalSentenceNeuron)->topLevelSentenceNeuron = true;
		}
		#endif		
	}

	//#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
	*highLevelNeuronPrior = grammaticalSentenceNeuronSub;
	//#endif
	
	if(createTopLevelNeuron)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TRACE
		SANIpropagateOperations.traceBackpropNeuralNetwork((*grammaticalSentenceNeuron), 0, -1, -1);
		#endif
	}


	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << endl;
	#endif
	
	return result;
}
#endif


bool SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << "connectListOfHighLevelNeuronsToNewNeuron = ";
	#endif
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TRACE
	cout << "connectListOfHighLevelNeuronsToNewNeuron" << endl;
	cout << "listOfHighLevelNeurons->size() = " << listOfHighLevelNeurons->size() << endl;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_TOP_LEVEL_NEURON_DUPLICATION
	if(listOfHighLevelNeurons->size() > 1)
	{
	#endif
		result = true;
		
		SANIGroupNeuralNetwork* grammaticalSentenceNeuronSub = createNewHiddenLayerGroup(forwardPropogationSentenceData, SANIGroupTypes);
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
		cout << grammaticalSentenceNeuronSub->groupIndex << " ";
		#endif
		#ifdef SANI_DEBUG_FORMATION
		cout << "\t\t SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuron: grammaticalSentenceNeuronSub->groupIndex = " << grammaticalSentenceNeuronSub->groupIndex << endl;
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
		//SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_ELONGATED_TREE
		for(int k=0; k<listOfHighLevelNeurons->size(); k++)
		{
			SANIGroupNeuralNetwork* currentHighLevelNeuron = (*listOfHighLevelNeurons)[k];
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
			cout << currentHighLevelNeuron->groupIndex << " ";
			#endif
				
			SANIGroupNeuralNetwork* grammaticalSentenceNeuronSubHigher = NULL;
	
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
			if(currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp)
			{
				grammaticalSentenceNeuronSubHigher = currentHighLevelNeuron;

				addVariableComponentToGroup(forwardPropogationSentenceData, grammaticalSentenceNeuronSub, grammaticalSentenceNeuronSubHigher, true);
			}
			else
			{
			#endif
				int componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
				if(currentHighLevelNeuron->inputLayerNeuron)
				{
					componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING;
				}
				addComponentToGroup(forwardPropogationSentenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSub, componentType, false);

			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
			}
			#endif
			
			//add new higher level group in elongated tree
			if(k>0 && k<listOfHighLevelNeurons->size()-1)
			{//there is still another neuron in listOfHighLevelNeurons to be added to the tree
							
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
				if(currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp)
				{
					currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp = false;
				}
				else
				{
				#endif				
					grammaticalSentenceNeuronSubHigher = createNewHiddenLayerGroup(forwardPropogationSentenceData, SANIGroupTypes);
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
					cout << grammaticalSentenceNeuronSubHigher->groupIndex << " ";
					#endif
					addComponentToGroup(forwardPropogationSentenceData, grammaticalSentenceNeuronSub, grammaticalSentenceNeuronSubHigher, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
				}
				#endif
				
				#ifdef SANI_DEBUG_FORMATION
				cout << "\t\t SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuron: grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
				#endif
				
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH
				#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
				grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, grammaticalSentenceNeuronSub->networkDepth + 1);
				SANInodes.adjustNetworkDepth(grammaticalSentenceNeuronSubHigher);
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
				SANInodes.adjustNetworkDepthReset(grammaticalSentenceNeuronSubHigher);
				#endif
				#endif
				#endif

				SANInodes.updateComponentsOwnerGroupAndIndexes(grammaticalSentenceNeuronSub, &(grammaticalSentenceNeuronSub->components), false, NULL);
				
				grammaticalSentenceNeuronSub = grammaticalSentenceNeuronSubHigher;
			}
		}	
		#else
		for(int k=0; k<listOfHighLevelNeurons->size(); k++)
		{
			SANIGroupNeuralNetwork* currentHighLevelNeuron = (*listOfHighLevelNeurons)[k];
				
			#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
			if(!directWireLowLevelPOSneuronToGrammaticalSentenceNeuron(SANIGroupTypes, forwardPropogationSentenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSub))
			{
			#endif
				addComponentToGroup(forwardPropogationSentenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSub, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);
			#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
			}
			#endif
		}
		#endif
		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
		cout << "SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuron - assigning top level sentence neuron;" << endl;
		#endif
		(*grammaticalSentenceNeuron) = grammaticalSentenceNeuronSub;
		#ifdef SANI_ANN_SEGREGATE_TOP_LAYER_NEURONS
		if(createTopLevelNeuron)
		{
			(*grammaticalSentenceNeuron)->topLevelSentenceNeuron = true;
		}
		#endif
		
		SANInodes.updateComponentsOwnerGroupAndIndexes((*grammaticalSentenceNeuron), &((*grammaticalSentenceNeuron)->components), false, NULL);

		if(createTopLevelNeuron)
		{
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TRACE
			SANIpropagateOperations.traceBackpropNeuralNetwork((*grammaticalSentenceNeuron), 0, -1, -1);
			#endif
		}
	#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_TOP_LEVEL_NEURON_DUPLICATION
	}
	else
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
		if(listOfHighLevelNeurons->size() == 1)
		{
			result = true;
			
			*grammaticalSentenceNeuron = (*listOfHighLevelNeurons)[0];
			(*grammaticalSentenceNeuron)->firstHiddenLayerNeuron = false;
			
			if(createTopLevelNeuron)
			{			
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TRACE
				SANIpropagateOperations.traceBackpropNeuralNetwork((*grammaticalSentenceNeuron), 0, -1, -1);
				#endif
			}
		}
		#endif
	}
	#endif
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << endl;
	#endif
	
	return result;
}


	

	
#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
bool SANIpropagateCompactGenerateClass::verifyAndMarkNeuronAsCandidateWrapper(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* candidateNeuronBaseParseTree, SANIComponentNeuralNetwork** candidateComponent2)
{
	bool result = false;
	
	SANIGroupNeuralNetwork* candidateNeuronBase = candidateNeuronBaseParseTree->groupRef;
		
	int lastActivatedIndex = INT_DEFAULT_VALUE;
	int firstUnactivatedIndex = INT_DEFAULT_VALUE;	
	SANIpropagateCompact.identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(forwardPropogationSentenceData, candidateNeuronBaseParseTree, &lastActivatedIndex, &firstUnactivatedIndex);
		
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	bool missingStartComponentFound = false;
	int indexOfFirstComponent;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		indexOfFirstComponent = candidateNeuronBaseParseTree->components.size()-1;
		SANIComponentParseTree* firstActiveComponent = candidateNeuronBaseParseTree->components[indexOfFirstComponent];
		if(firstActiveComponent->componentRef->componentIndex != candidateNeuronBase->components.size()-1)
		{
			missingStartComponentFound = true;
		}
	}
	else
	{
		indexOfFirstComponent = 0;
		SANIComponentParseTree* firstActiveComponent = candidateNeuronBaseParseTree->components[indexOfFirstComponent];
		if(firstActiveComponent->componentRef->componentIndex != 0)
		{
			missingStartComponentFound = true;
		}
	}
	if(!missingStartComponentFound)
	{
	#endif
		bool missingEndComponentsFound = false;
		int missingEndComponentsIndex;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			if(firstUnactivatedIndex > 0)
			{
				missingEndComponentsFound = true;
				missingEndComponentsIndex = firstUnactivatedIndex-1;
			}
		}
		else
		{
			if(firstUnactivatedIndex < candidateNeuronBase->components.size()-1)
			{
				missingEndComponentsFound = true;
				missingEndComponentsIndex = firstUnactivatedIndex+1;
			}
		}
		
		if(missingEndComponentsFound)
		{
			*candidateComponent2 = candidateNeuronBase->components[missingEndComponentsIndex];
			result = true;
		}
		else
		{
			if(verifyAndMarkNeuronAsCandidate(forwardPropogationSentenceData, candidateNeuronBase, candidateComponent2))
			{
				result = true;
			}
		}
		
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	}
	#endif

	return result;
}
	
bool SANIpropagateCompactGenerateClass::verifyAndMarkNeuronAsCandidate(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentNeuron, SANIComponentNeuralNetwork** candidateComponent2)
{
	bool result = false;
	
	for(int i=0; i<currentNeuron->ANNfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (currentNeuron->ANNfrontComponentConnectionList)[i];
		SANIGroupNeuralNetwork* higherLevelGroup = currentComponent->ownerGroup;
		
		//note this function currently relies on higherLevelGroup instead of higherLevelGroup->parseTreeTemp to detect component activations (neuronComponentConnectionActive); CHECKTHIS
		//SANIGroupNeuralNetwork* higherLevelGroupParseTree = higherLevelGroup->parseTreeTemp;
		
		bool foundPriorComponent = false;
		bool foundPostComponent = false;
		bool foundCurrentComponent = false;
		bool allPriorComponentsActive = true;
		for(int j=0; j<higherLevelGroup->components.size(); j++)
		{
			int c;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				c = higherLevelGroup->components.size()-1-j;
			}
			else
			{
				c = j;
			}
				
			SANIComponentNeuralNetwork* currentComponent2 = higherLevelGroup->components[c];
			if(foundCurrentComponent)
			{
				if(!foundPostComponent)
				{
					foundPostComponent = true;
					if(allPriorComponentsActive)
					{
						//record first post component
						*candidateComponent2 = currentComponent2;
						result = true;
					}
				}
			}
			else
			{
				if(c == currentComponent->componentIndex)
				{
					foundCurrentComponent = true;
				}
				else
				{
					foundPriorComponent = true;
					if(!(currentComponent2->neuronComponentConnectionActive))
					{
						allPriorComponentsActive = false;
					}
				}
			}
		}
		if(allPriorComponentsActive && !foundPostComponent)
		{
			if(verifyAndMarkNeuronAsCandidate(forwardPropogationSentenceData, higherLevelGroup, candidateComponent2))
			{
				result = true;
			}
		}
	}
	
	return result;
}
#endif
	

bool SANIpropagateCompactGenerateClass::createOrAppendFirstLevelHiddenLayerGroup(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentLayerNeuronGroupStart, bool* creatingNewNeuronSequence, int* neuronSequenceIndex, SANIGroupNeuralNetwork** newNeuronSequenceGroup, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons)
{
	bool result = true;
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIpropagateCompactGenerateClass::createOrAppendFirstLevelHiddenLayerGroup" << endl;
	#endif

	currentLayerNeuronGroupStart->inputLayerNeuron = true;

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
	listOfHighLevelNeurons->push_back(currentLayerNeuronGroupStart);	
	#else
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ORIG
	if(*neuronSequenceIndex >= SANI_SEQUENCE_GRAMMAR_MAX_NUMBER_COMPONENTS)
	{
		*creatingNewNeuronSequence = false;
		*neuronSequenceIndex = 0;
	}
	#endif

	if(*creatingNewNeuronSequence)
	{
		addComponentToFirstLevelHiddenLayerGroup(forwardPropogationSentenceData, currentLayerNeuronGroupStart, *newNeuronSequenceGroup);
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		cout << "creatingNewNeuronSequence: newNeuronSequenceGroup->groupIndex = " << (*newNeuronSequenceGroup)->groupIndex << endl;
		#endif
	}
	else
	{
		*neuronSequenceIndex = 0;
		*creatingNewNeuronSequence = true;
		SANIGroupNeuralNetwork* newNeuron = createNewHiddenLayerGroup(forwardPropogationSentenceData, SANIGroupTypes);
		#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
		newNeuron->firstHiddenLayerNeuron = true;
		#endif
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		cout << "!creatingNewNeuronSequence: newNeuron->groupIndex = " << newNeuron->groupIndex << endl;
		#endif
		addComponentToFirstLevelHiddenLayerGroup(forwardPropogationSentenceData, currentLayerNeuronGroupStart, newNeuron);
		listOfHighLevelNeurons->push_back(newNeuron);
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		//all sublayer neurons are POS inputs/strings and will not be added to listOfHighLevelNeuronsCompleteHistory
		#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
		#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
		newNeuron->marked = true;
		#else
		forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.push_back(newNeuron);
		#endif
		#endif
		#endif
		*newNeuronSequenceGroup = newNeuron;
	}
	*neuronSequenceIndex = *neuronSequenceIndex + 1;
	#endif
	
	return result;
}		
			
#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
bool SANIpropagateCompactGenerateClass::directWireLowLevelPOSneuronToGrammaticalSentenceNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* singleComponentNeuronCandidate, SANIGroupNeuralNetwork* grammaticalSentenceNeuron)
{
	bool directWire = false;
	if(singleComponentNeuronCandidate->firstHiddenLayerNeuron)
	{		
		if(singleComponentNeuronCandidate->components.size() == 1)
		{
			SANIGroupNeuralNetwork* singleComponentNeuron = singleComponentNeuronCandidate;

			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
			cout << "SANIpropagateCompactGenerateClass::directWireLowLevelPOSneuronToGrammaticalSentenceNeuron: singleComponentNeuron->groupIndex = " << singleComponentNeuron->groupIndex << endl;
			#endif
			
			//direct wire low level POS neuron to grammaticalSentenceNeuron (dont ever create intermediary single component neuron)
			directWire = true;

			SANIComponentNeuralNetwork* component = singleComponentNeuron->components[0];

			//doesnt seem to ever be required;
			for(int i=0; i<singleComponentNeuron->ANNfrontComponentConnectionList.size(); i++)
			{
				SANIComponentNeuralNetwork* currentComponent = (singleComponentNeuron->ANNfrontComponentConnectionList)[i];
				SANIGroupNeuralNetwork* higherLevelGroup = currentComponent->ownerGroup;
				
				for(int j=0; j<currentComponent->ANNbackGroupConnectionList.size(); j++)
				{
					SANIGroupNeuralNetwork* firstHiddenLayerGroup = (currentComponent->ANNbackGroupConnectionList)[j];
					if(firstHiddenLayerGroup == singleComponentNeuron)
					{
						currentComponent->ANNbackGroupConnectionList.erase(currentComponent->ANNbackGroupConnectionList.begin()+j);
						j--;
					}
				}
			}

			for(int i=0; i<component->ANNbackGroupConnectionList.size(); i++)
			{
				SANIGroupNeuralNetwork* lowerLevelPOSgroup = (component->ANNbackGroupConnectionList)[i];
				
				for(int j=0; j<lowerLevelPOSgroup->ANNfrontComponentConnectionList.size(); j++)
				{
					SANIComponentNeuralNetwork* currentComponent = (lowerLevelPOSgroup->ANNfrontComponentConnectionList)[j];
					if(currentComponent == component)
					{
						lowerLevelPOSgroup->ANNfrontComponentConnectionList.erase(lowerLevelPOSgroup->ANNfrontComponentConnectionList.begin()+j);
						j--;
					}
				}

				addComponentToGroup(forwardPropogationSentenceData, lowerLevelPOSgroup, grammaticalSentenceNeuron, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING, false);	//fixed GIA3j5aTEMP61 
			}

			SANIGroupType* groupType = SANInodes.getSequenceGrammarGroupTypeDefault(SANIGroupTypes);
			//groupType->groups.erase(remove(groupType->groups.begin(), groupType->groups.end(), singleComponentNeuron), groupType->groups.end());
			for(int i2=0; i2<groupType->groups.size(); i2++)
			{
				SANIGroupNeuralNetwork* group = (groupType->groups)[i2];
				if(group == singleComponentNeuron)
				{
					groupType->groups.erase(groupType->groups.begin()+i2);
        				i2--;
				}
			}

			SANInodes.deleteGroup(singleComponentNeuron);	
		}
	}
	if(!directWire)
	{
		singleComponentNeuronCandidate->firstHiddenLayerNeuron = false;	//NOT REQUIRED? prevents parsing of future sentence from matching this neuron as activatedNeuronWithMaxWordIndexCoverage, and then treating it like firstHiddenLayerNeuron above
	}
	
	return directWire;
}
#endif









bool SANIpropagateCompactGenerateClass::addComponentToFirstLevelHiddenLayerGroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* inputLayerPOSneuron, SANIGroupNeuralNetwork* newNeuronSequenceGroup)
{
	bool result = true;

	addComponentToGroup(forwardPropogationSentenceData, inputLayerPOSneuron, newNeuronSequenceGroup, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING, false);
	//SANIformation.createGroupANNconnection(group, higherLevelComponent);

	return result;
}

SANIGroupNeuralNetwork* SANIpropagateCompactGenerateClass::createNewHiddenLayerGroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIpropagateCompactGenerateClass::createNewHiddenLayerGroup" << endl;
	#endif
		
	SANIGroupNeuralNetwork* newNeuron = createNewGroup();	
	SANIGroupType* groupType = SANInodes.getSequenceGrammarGroupTypeDefault(SANIGroupTypes);	
	groupType->groups.push_back(newNeuron);

	return newNeuron;
}

//NB indexToSplitVector = lastActivatedComponent (the last component index in the first part of component to be splitted) (do not assume normal order: "first" and "last" definitions respect (forwardPropogationSentenceData->parseSentenceReverse))
SANIGroupNeuralNetwork* SANIpropagateCompactGenerateClass::splitGroupAtLastActivatedComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupNeuralNetwork* neuronToSplit, int indexToSplitVector)
{
	SANIGroupNeuralNetwork* newHiddenLayerNeuron = createNewGroup();	
	SANIGroupType* groupType = SANInodes.getSequenceGrammarGroupTypeDefault(SANIGroupTypes);	
	groupType->groups.push_back(newHiddenLayerNeuron);
		
	vector<SANIComponentNeuralNetwork*>* components = &(neuronToSplit->components);
	vector<SANIComponentNeuralNetwork*> componentsPart1(components->begin(), components->begin() + indexToSplitVector+1);
        vector<SANIComponentNeuralNetwork*> componentsPart2(components->begin() + indexToSplitVector+1, components->end());
	
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		newHiddenLayerNeuron->components = componentsPart2;
		neuronToSplit->components = componentsPart1;
	}
	else
	{
		newHiddenLayerNeuron->components = componentsPart1;
		neuronToSplit->components = componentsPart2;
	}
	SANInodes.updateComponentsOwnerGroupAndIndexes(newHiddenLayerNeuron, &(newHiddenLayerNeuron->components), false, NULL);
	SANInodes.updateComponentsOwnerGroupAndIndexes(neuronToSplit, &(neuronToSplit->components), false, NULL);	
	
	addComponentToGroup(forwardPropogationSentenceData, newHiddenLayerNeuron, neuronToSplit, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, true);	//CHECKTHIS	
		
	return newHiddenLayerNeuron;
}

#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
//indexToSplitVector1 = firstActivatedComponent (unordered; always assume normal order: "first" and "last" definitions do not respect (forwardPropogationSentenceData->parseSentenceReverse))
//indexToSplitVector2 = lastActivatedComponent (unordered; always assume normal order: "first" and "last" definitions do not respect (forwardPropogationSentenceData->parseSentenceReverse))
SANIGroupNeuralNetwork* SANIpropagateCompactGenerateClass::splitGroupAtLastActivatedComponentUnordered(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupNeuralNetwork* neuronToSplit, int indexToSplitVector1, int indexToSplitVector2)
{
	SANIGroupNeuralNetwork* newHiddenLayerNeuron = createNewGroup();	
	SANIGroupType* groupType = SANInodes.getSequenceGrammarGroupTypeDefault(SANIGroupTypes);	
	groupType->groups.push_back(newHiddenLayerNeuron);
				
	vector<SANIComponentNeuralNetwork*>* components = &(neuronToSplit->components);
	
	if(indexToSplitVector1 == 0)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SPLIT
		cout << "split at right (indexToSplitVector2) only: newHiddenLayerNeuron->groupIndex = " << newHiddenLayerNeuron->groupIndex << ", neuronToSplit->groupIndex = " << neuronToSplit->groupIndex << endl;
		#endif
		//split at right of centre (indexToSplitVector2) only - there was no unmatched first section (from SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION)
		vector<SANIComponentNeuralNetwork*> componentsPart1(components->begin(), components->begin()+indexToSplitVector2+1);
        	vector<SANIComponentNeuralNetwork*> componentsPart2(components->begin()+indexToSplitVector2+1, components->end());

		newHiddenLayerNeuron->components = componentsPart1;
		neuronToSplit->components = componentsPart2;
		
		SANInodes.updateComponentsOwnerGroupAndIndexes(newHiddenLayerNeuron, &(newHiddenLayerNeuron->components), false, NULL);
		SANInodes.updateComponentsOwnerGroupAndIndexes(neuronToSplit, &(neuronToSplit->components), false, NULL);	

		bool insertAtStart = true;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			insertAtStart = false;	//compensate for addComponentToGroup specification limitation
		}
		addComponentToGroup(forwardPropogationSentenceData, newHiddenLayerNeuron, neuronToSplit, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, insertAtStart);	//CHECKTHIS
	}
	else if(indexToSplitVector2 == components->size()-1)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SPLIT
		cout << "split at left (indexToSplitVector1) only: newHiddenLayerNeuron->groupIndex = " << newHiddenLayerNeuron->groupIndex << ", neuronToSplit->groupIndex = " << neuronToSplit->groupIndex << endl;
		#endif
		//split at left of centre (indexToSplitVector1) only - there was no unmatched last section
		vector<SANIComponentNeuralNetwork*> componentsPart1(components->begin(), components->begin()+indexToSplitVector1);
		vector<SANIComponentNeuralNetwork*> componentsPart2(components->begin()+indexToSplitVector1, components->end());
		
		newHiddenLayerNeuron->components = componentsPart2;
		neuronToSplit->components = componentsPart1;
		
		SANInodes.updateComponentsOwnerGroupAndIndexes(newHiddenLayerNeuron, &(newHiddenLayerNeuron->components), false, NULL);
		SANInodes.updateComponentsOwnerGroupAndIndexes(neuronToSplit, &(neuronToSplit->components), false, NULL);	

		bool insertAtStart = false;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			insertAtStart = true;	//compensate for addComponentToGroup specification limitation
		}
		addComponentToGroup(forwardPropogationSentenceData, newHiddenLayerNeuron, neuronToSplit, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, insertAtStart);	//CHECKTHIS
	}
	else
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SPLIT
		cout << "split middle: newHiddenLayerNeuron->groupIndex = " << newHiddenLayerNeuron->groupIndex << ", neuronToSplit->groupIndex = " << neuronToSplit->groupIndex << endl;
		#endif
		//split middle
		vector<SANIComponentNeuralNetwork*> componentsPart1(components->begin(), components->begin()+indexToSplitVector1);
		vector<SANIComponentNeuralNetwork*> componentsPart2(components->begin()+indexToSplitVector1, components->begin()+indexToSplitVector2+1);	
		vector<SANIComponentNeuralNetwork*> componentsPart3(components->begin()+indexToSplitVector2+1, components->end());		
		
		newHiddenLayerNeuron->components = componentsPart2;
		SANInodes.updateComponentsOwnerGroupAndIndexes(newHiddenLayerNeuron, &(newHiddenLayerNeuron->components), false, NULL);

		//neuronToSplit->components = componentsPart1;
		neuronToSplit->components.clear();
		neuronToSplit->components.insert(neuronToSplit->components.end(), componentsPart1.begin(), componentsPart1.end());
		SANInodes.updateComponentsOwnerGroupAndIndexes(neuronToSplit, &(neuronToSplit->components), false, NULL);
		
		bool insertAtStart = false;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			insertAtStart = true;	//compensate for addComponentToGroup specification limitation
		}
		addComponentToGroup(forwardPropogationSentenceData, newHiddenLayerNeuron, neuronToSplit, GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, insertAtStart);	//CHECKTHIS
		
		neuronToSplit->components.insert(neuronToSplit->components.end(), componentsPart3.begin(), componentsPart3.end());
		SANInodes.updateComponentsOwnerGroupAndIndexes(neuronToSplit, &(neuronToSplit->components), false, NULL);
	}
	
	return newHiddenLayerNeuron;
}
#endif





bool SANIpropagateCompactGenerateClass::addVariableComponentToGroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelComponentGroupOwner, bool addToStart)
{
	bool result = true;

	SANIComponentNeuralNetwork* variableComponent = SANInodes.getFirstComponent(forwardPropogationSentenceData, higherLevelComponentGroupOwner, addToStart);

	if(variableComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		cerr << "SANIpropagateCompactGenerateClass::addVariableComponentToGroup error: (variableComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)" << endl;
		exit(EXIT_ERROR);
	}
					
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_PREVENT_DUPLICATE_CONNECTIONS
	bool duplicateConnectionFound = false;
	for(int j=0; j<variableComponent->ANNbackGroupConnectionList.size(); j++)
	{
		SANIGroupNeuralNetwork* subGroup = (variableComponent->ANNbackGroupConnectionList)[j];
		if(subGroup == group)
		{
			duplicateConnectionFound = true;
		}
	}
	if(!duplicateConnectionFound)
	{
	#endif
		addComponentToGroup(forwardPropogationSentenceData, group, variableComponent);
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_PREVENT_DUPLICATE_CONNECTIONS
	}
	#endif
		
	return result;
}


bool SANIpropagateCompactGenerateClass::addComponentToGroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelComponentGroupOwner, int componentType, bool insertAtStart)
{
	bool result = true;

	SANIComponentNeuralNetwork* newComponent = new SANIComponentNeuralNetwork();

	newComponent->ownerGroup = higherLevelComponentGroupOwner;
	
	//see SANInodes.getFirstComponent
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		insertAtStart = !insertAtStart;
	}
	if(insertAtStart)
	{
		int componentIndex = SANI_COMPONENT_INDEX_FIRST;
		newComponent->componentIndex = componentIndex;
		//higherLevelComponentGroupOwner->components.push_front(newComponent);
		higherLevelComponentGroupOwner->components.insert(higherLevelComponentGroupOwner->components.begin(), newComponent);
		SANInodes.updateComponentsOwnerGroupAndIndexes(higherLevelComponentGroupOwner, &(higherLevelComponentGroupOwner->components), false, NULL);
		
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
		if(higherLevelComponentGroupOwner->components.size() > SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)
		{
			cerr << "\nSANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS: SANIpropagateCompactGenerateClass::addComponentToGroup error:" << endl;
			cerr << "higherLevelComponentGroupOwner->components.size() = " << higherLevelComponentGroupOwner->components.size() << endl;
			exit(EXIT_ERROR);
		}
		#endif
	}
	else
	{
		int componentIndex = SANI_COMPONENT_INDEX_FIRST+higherLevelComponentGroupOwner->components.size();
		newComponent->componentIndex = componentIndex;
		higherLevelComponentGroupOwner->components.push_back(newComponent);
	}
			
	newComponent->componentType = componentType;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	//should never be true
	newComponent->groupRefName = group->groupName;
	newComponent->groupTypeRefName = group->groupTypeName;
	#endif
	
	/*
	this is only for initialisation (SANIformation) and parseTreeGroup components?;
	newComponent->groupRef = group;	//newComponent->ownerGroup instead
	newComponent->groupTypeRef = group->groupType;
	*/
	if(componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
	{
	
	}
	else if(componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		newComponent->stringType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS;
		newComponent->wordPOStype = group->wordPOStype;	
	}
	else
	{
		cerr << "SANIpropagateCompactGenerateClass::addComponentToGroup error: componentType != GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP/STRING" << endl;
		exit(EXIT_ERROR);
	}

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIpropagateCompactGenerateClass::addComponentToGroup: higherLevelComponentGroupOwner->groupIndex = " << higherLevelComponentGroupOwner->groupIndex << ", newComponent->componentIndex = " << newComponent->componentIndex << endl;
	#endif
	
	SANIformation.createGroupANNconnection(group, newComponent);
	
	return result;
}

bool SANIpropagateCompactGenerateClass::addComponentToGroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* higherLevelComponent)
{
	bool result = true;

	SANIformation.createGroupANNconnection(group, higherLevelComponent);
	
	return result;
}


SANIGroupNeuralNetwork* SANIpropagateCompactGenerateClass::createNewGroup()
{
	SANIGroupNeuralNetwork* newGroup = new SANIGroupNeuralNetwork();
	newGroup->groupName = SANI_SEQUENCE_GRAMMAR_GROUP_NAME;
	newGroup->groupTypeName = SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_NAME;
	
	int newNeuronIndex = SANInodes.assignGroupIndex(newGroup);
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIpropagateCompactGenerateClass::createNewGroup - groupIndex = " << newNeuronIndex << endl;
	#endif
	#ifdef SANI_ANN
	string formatString = string("%0") + SHAREDvars.convertIntToString(SANI_ANN_GROUP_INDEX_DISPLAY_FORMAT_NUMBER_CHARACTERS) + "d";
	newGroup->initiateANNneuron("groupIndex:" + SHAREDvars.convertIntToString(newNeuronIndex-1, formatString));	//SANI_SEQUENCE_GRAMMAR_NEURON_NAME	//newGroup->groupTypeName + ":" + newGroup->groupName
	#endif
	
	return newGroup;
}

bool SANIpropagateCompactGenerateClass::findGroupDirectlyAbove(SANIGroupNeuralNetwork* group1, SANIGroupNeuralNetwork* group2)
{
	bool result = false;
	
	for(int i=0; i<group1->ANNfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (group1->ANNfrontComponentConnectionList)[i];

		SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	
		SANIComponentNeuralNetwork* ownerComponent = currentComponent;	//not used by SANI_SEQUENCE_GRAMMAR (as subcomponents are never generated)
		int componentIndex = currentComponent->componentIndex;	
		int ownerComponentIndex = componentIndex;	//not used by SANI_SEQUENCE_GRAMMAR (as subcomponents are never generated)
	
		if(ownerGroup == group2)
		{
			result = true;
		}
	}
	
	return result;
}


bool SANIpropagateCompactGenerateClass::verifyLastWordIndex(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroup, int lastWordIndex, bool* adjacent)
{
	bool result = false;
	
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		if(parseTreeGroup->parseTreeMinWordIndex >= lastWordIndex)
		{
			result = true;
		}
		if(parseTreeGroup->parseTreeMinWordIndex == lastWordIndex)
		{
			*adjacent = true;
		}
	}
	else
	{
		if(parseTreeGroup->parseTreeMaxWordIndex <= lastWordIndex)
		{
			result = true;
		}
		if(parseTreeGroup->parseTreeMaxWordIndex == lastWordIndex)
		{
			*adjacent = true;
		}
	}

	return result;
}

#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
bool SANIpropagateCompactGenerateClass::markSubNeurons(SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = true;
	
	if(!currentNeuron->marked)
	{
		if(!SANInodes.isNeuronString(currentNeuron))	//CHECKTHIS
		{
			currentNeuron->marked = true;

			#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
			if(currentNeuron->verified)
			{
				cout << "SANIpropagateCompactGenerateClass::addSubNeuronsToList error: currentNeuron has already been parsed (circular loop detected)" << endl;
				exit(EXIT_ERROR);
			}
			currentNeuron->verified = true;
			#endif

			for(int i=0; i<currentNeuron->components.size(); i++)
			{
				SANIComponentNeuralNetwork* currentComponent = currentNeuron->components[i];
				for(int j=0; j<currentComponent->ANNbackGroupConnectionList.size(); j++)
				{
					SANIGroupNeuralNetwork* subGroup = (currentComponent->ANNbackGroupConnectionList)[j];
					markSubNeurons(subGroup);
				}
			}

			#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
			currentNeuron->verified = false;
			#endif
		}
	}
	
	return result;
}
bool SANIpropagateCompactGenerateClass::markSubNeuronsReset(SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = true;
	
	if(currentNeuron->marked)
	{
		if(!SANInodes.isNeuronString(currentNeuron))	//CHECKTHIS
		{
			currentNeuron->marked = false;

			for(int i=0; i<currentNeuron->components.size(); i++)
			{
				SANIComponentNeuralNetwork* currentComponent = currentNeuron->components[i];
				for(int j=0; j<currentComponent->ANNbackGroupConnectionList.size(); j++)
				{
					SANIGroupNeuralNetwork* subGroup = (currentComponent->ANNbackGroupConnectionList)[j];
					markSubNeuronsReset(subGroup);
				}
			}
		}
	}
	
	return result;
}
#endif

#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_BASIC
bool SANIpropagateCompactGenerateClass::addSubNeuronsToList(vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeuronsCompleteHistory, SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = true;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	if(!currentNeuron->counted)
	{
		currentNeuron->counted = true;
	#endif
		if(!SANInodes.isNeuronString(currentNeuron))
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
			if(currentNeuron->verified)
			{
				cout << "SANIpropagateCompactGenerateClass::addSubNeuronsToList error: currentNeuron has already been parsed (circular loop detected)" << endl;
				exit(EXIT_ERROR);
			}
			currentNeuron->verified = true;
			#endif

			listOfHighLevelNeuronsCompleteHistory->push_back(currentNeuron);

			for(int i=0; i<currentNeuron->components.size(); i++)
			{
				SANIComponentNeuralNetwork* currentComponent = currentNeuron->components[i];
				for(int j=0; j<currentComponent->ANNbackGroupConnectionList.size(); j++)
				{
					SANIGroupNeuralNetwork* subGroup = (currentComponent->ANNbackGroupConnectionList)[j];
					addSubNeuronsToList(listOfHighLevelNeuronsCompleteHistory, subGroup);
				}
			}

			#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
			currentNeuron->verified = false;
			#endif
		}
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	}
	#endif
	
	return result;
}
#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
bool SANIpropagateCompactGenerateClass::addSubNeuronsToListReset(SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = true;
	
	if(currentNeuron->counted)
	{
		currentNeuron->counted = false;
		if(!SANInodes.isNeuronString(currentNeuron))
		{
			for(int i=0; i<currentNeuron->components.size(); i++)
			{
				SANIComponentNeuralNetwork* currentComponent = currentNeuron->components[i];
				for(int j=0; j<currentComponent->ANNbackGroupConnectionList.size(); j++)
				{
					SANIGroupNeuralNetwork* subGroup = (currentComponent->ANNbackGroupConnectionList)[j];
					addSubNeuronsToListReset(subGroup);
				}
			}
		}
	}
	
	return result;
}
#endif
#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_DO_NOT_HAVE_CONNECTIONS_TO_ANY_NEW_CANDIDATE
bool SANIpropagateCompactGenerateClass::addParentNeuronsToList(vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeuronsCompleteHistory, SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = true;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	if(!currentNeuron->counted)
	{
		currentNeuron->counted = true;
	#endif

		#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
		if(currentNeuron->verified)
		{
			cout << "SANIpropagateCompactGenerateClass::addParentNeuronsToList error: currentNeuron has already been parsed (circular loop detected)" << endl;
			exit(EXIT_ERROR);
		}
		currentNeuron->verified = true;
		#endif

		listOfHighLevelNeuronsCompleteHistory->push_back(currentNeuron);

		for(int j=0; j<currentNeuron->ANNfrontComponentConnectionList.size(); j++)
		{
			SANIComponentNeuralNetwork* parentComponent = (currentNeuron->ANNfrontComponentConnectionList)[j];
			SANIGroupNeuralNetwork* higherLevelGroup = parentComponent->ownerGroup;

			addParentNeuronsToList(listOfHighLevelNeuronsCompleteHistory, higherLevelGroup);
		}

		#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
		currentNeuron->verified = false;
		#endif
		
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	}
	#endif
	
	return result;
}
#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
bool SANIpropagateCompactGenerateClass::addParentNeuronsToListReset(SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = true;
	
	if(currentNeuron->counted)
	{
		currentNeuron->counted = false;
		
		for(int j=0; j<currentNeuron->ANNfrontComponentConnectionList.size(); j++)
		{
			SANIComponentNeuralNetwork* parentComponent = (currentNeuron->ANNfrontComponentConnectionList)[j];
			SANIGroupNeuralNetwork* higherLevelGroup = parentComponent->ownerGroup;

			addParentNeuronsToListReset(higherLevelGroup);
		}
	}
	
	return result;
}
#endif
#endif
#endif
#endif


int SANIpropagateCompactGenerateClass::calculateNextIndexInSequence(SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	return SANIpropagateCompact.calculateNextIndexInSequenceProspective(forwardPropogationSentenceData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage);
}



#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE

#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
bool SANIpropagateCompactGenerateClass::findTempVariableEndComponentNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** variableEndComponentNeuron)
{
	bool foundTempVariableEndComponentNeuron = false;
	
	if(highLevelNeuronPriorArray->size() > 0)
	{
		if((*highLevelNeuronPriorArray)[0] != NULL)
		{
			if((*highLevelNeuronPriorArray)[0]->activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp)
			{
				foundTempVariableEndComponentNeuron = true;
				*variableEndComponentNeuron = (*highLevelNeuronPriorArray)[0];
				
				if((*variableEndComponentNeuron)->lowestLayerInArray)
				{
					//(*highLevelNeuronPriorArray)[0] = NULL;	//remove temporary addition from array	//removed GIA3l7d
					(*variableEndComponentNeuron)->lowestLayerInArray = false;
				}	
			}
		}
	}
	else
	{
		//cerr << "SANIpropagateCompactGenerateClass::findTempVariableEndComponentNeuron: error: (highLevelNeuronPriorArray->size() == 0)" << endl;
	}
	
	return foundTempVariableEndComponentNeuron;
}
void SANIpropagateCompactGenerateClass::setTempVariableEndComponentNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork* variableEndComponentNeuron)
{	
	if(highLevelNeuronPriorArray->size() > 0)
	{
		if((*highLevelNeuronPriorArray)[0] != NULL)
		{
			if((*highLevelNeuronPriorArray)[0] == variableEndComponentNeuron)
			{
				variableEndComponentNeuron->lowestLayerInArray = true;	//if true, do not clear (*highLevelNeuronPriorArray)[0] upon findTempVariableEndComponentNeuron
			}
			else
			{
				cerr << "SANIpropagateCompactGenerateClass::setTempVariableEndComponentNeuron: (highLevelNeuronPriorArray[0] != NULL) && (highLevelNeuronPriorArray[0] != variableEndComponentNeuron)" << endl;
				exit(EXIT_ERROR);
			}
		}
		(*highLevelNeuronPriorArray)[0] = variableEndComponentNeuron;
	}
	else
	{
		cerr << "SANIpropagateCompactGenerateClass::setTempVariableEndComponentNeuron: error: (highLevelNeuronPriorArray->size() == 0)" << endl;
		exit(EXIT_ERROR);		
	}
}
#endif


#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH
int SANIpropagateCompactGenerateClass::calculateLowestLayerNeuronMaxDepth(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray)
{
	int lowestLayerNeuronMaxDepth = 0;
	
	SANIGroupNeuralNetwork* lowestLayerNeuron = NULL;
	int lowestLayerNeuronIndex = 0;
	bool foundLowestLayerNeuron = findLowestLayerNeuron(highLevelNeuronPriorArray, &lowestLayerNeuron, &lowestLayerNeuronIndex);
	if(foundLowestLayerNeuron)
	{
		lowestLayerNeuronMaxDepth = lowestLayerNeuron->networkDepth;	
	}
	
	return lowestLayerNeuronMaxDepth;
}
#endif
bool SANIpropagateCompactGenerateClass::findLowestLayerNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** lowestLayerNeuron, int* lowestLayerNeuronIndex)
{
	bool foundLowestLayerNeuron = false;
	
	for(int i=0; i<highLevelNeuronPriorArray->size(); i++)
	{
		if(!foundLowestLayerNeuron)
		{
			if((*highLevelNeuronPriorArray)[i] != NULL)
			{
				*lowestLayerNeuron = (*highLevelNeuronPriorArray)[i];
				*lowestLayerNeuronIndex = i;

				foundLowestLayerNeuron = true;
			}
		}
	}
	
	return foundLowestLayerNeuron;
}
bool SANIpropagateCompactGenerateClass::findHighestLayerNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** highestLayerNeuron, int* highestLayerNeuronIndex)
{
	bool foundHighestLayerNeuron = false;
	
	for(int i=0; i<highLevelNeuronPriorArray->size(); i++)
	{
		if((*highLevelNeuronPriorArray)[i] != NULL)
		{
			*highestLayerNeuron = (*highLevelNeuronPriorArray)[i];
			*highestLayerNeuronIndex = i;

			foundHighestLayerNeuron = true;
		}
	}
	
	return foundHighestLayerNeuron;
}
//grammaticalSentenceNeuronSub = neuron currently being considered to being added to highLevelNeuronPriorArray[currentLayerIndex]
bool SANIpropagateCompactGenerateClass::updateHighLevelNeuronHierachy(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, int lowestLayerNeuronIndex, SANIGroupNeuralNetwork* grammaticalSentenceNeuronSub, const bool completedIdentifyingSentenceHighLevelNeurons)
{
	bool result = true;
	
	bool foundHigherLevelWithoutNeuronNeuronPrior = false;
	int i = lowestLayerNeuronIndex;
	while(!foundHigherLevelWithoutNeuronNeuronPrior)
	{
		if((i < highLevelNeuronPriorArray->size()) && ((*highLevelNeuronPriorArray)[i] != NULL))
		{
			SANIGroupNeuralNetwork* highLevelNeuronPriorCurrent = (*highLevelNeuronPriorArray)[i];

			//combine grammaticalSentenceNeuronSub and highLevelNeuronPriorArray[i], to create neuron in layer i+1

			SANIGroupNeuralNetwork* grammaticalSentenceNeuronSubHigher = createNewHiddenLayerGroup(forwardPropogationSentenceData, SANIGroupTypes);
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
			cout << grammaticalSentenceNeuronSubHigher->groupIndex << " ";
			#endif
			int componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
			if(highLevelNeuronPriorCurrent->inputLayerNeuron)
			{
				componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING;
			}
			addComponentToGroup(forwardPropogationSentenceData, highLevelNeuronPriorCurrent, grammaticalSentenceNeuronSubHigher, componentType, false);

			#ifdef SANI_DEBUG_FORMATION2
			SANInodes.printParseTreeDebugIndentation(i);
			cout << "(*highLevelNeuronPriorArray)[i] != NULL), i = " << i << endl;
			SANInodes.printParseTreeDebugIndentation(i);
			cout << "adding highLevelNeuronPriorCurrent->groupIndex = " << highLevelNeuronPriorCurrent->groupIndex << " to grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
			#endif
			
			#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
			grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, highLevelNeuronPriorCurrent->networkDepth + 1);
			#endif

			componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
			if(grammaticalSentenceNeuronSub->inputLayerNeuron)
			{
				componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING;
			}
			addComponentToGroup(forwardPropogationSentenceData, grammaticalSentenceNeuronSub, grammaticalSentenceNeuronSubHigher, componentType, false);

			#ifdef SANI_DEBUG_FORMATION2
			SANInodes.printParseTreeDebugIndentation(i);
			cout << "adding grammaticalSentenceNeuronSub->groupIndex = " << grammaticalSentenceNeuronSub->groupIndex << " to grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
			#endif
			
			#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
			grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, grammaticalSentenceNeuronSub->networkDepth + 1);
			#endif

			grammaticalSentenceNeuronSub = grammaticalSentenceNeuronSubHigher;


			#ifdef SANI_DEBUG_FORMATION
			cout << "\t\t SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection: grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
			#endif

			#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
			SANInodes.adjustNetworkDepth(grammaticalSentenceNeuronSubHigher);
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
			SANInodes.adjustNetworkDepthReset(grammaticalSentenceNeuronSubHigher);
			#endif
			#endif
			
			(*highLevelNeuronPriorArray)[i] = NULL;	//empty current layer record 

			SANInodes.updateComponentsOwnerGroupAndIndexes(grammaticalSentenceNeuronSubHigher, &(grammaticalSentenceNeuronSubHigher->components), false, NULL);
		}
		else
		{
			if(completedIdentifyingSentenceHighLevelNeurons)
			{
				//skip layer until find layer with a neuron prior (as all words in sentence have been parsed, the last group found must always be added to a higher level neuron)
				if(i == highLevelNeuronPriorArray->size())
				{
					foundHigherLevelWithoutNeuronNeuronPrior = true;
					highLevelNeuronPriorArray->push_back(grammaticalSentenceNeuronSub);
					
					#ifdef SANI_DEBUG_FORMATION2
					SANInodes.printParseTreeDebugIndentation(i);
					cout << "completedIdentifyingSentenceHighLevelNeurons && foundHigherLevelWithoutNeuronNeuronPrior; grammaticalSentenceNeuronSub->groupIndex = " << grammaticalSentenceNeuronSub->groupIndex << endl;
					#endif
				}
			}
			else
			{
				foundHigherLevelWithoutNeuronNeuronPrior = true;	

				#ifdef SANI_DEBUG_FORMATION2
				SANInodes.printParseTreeDebugIndentation(i);
				cout << "foundHigherLevelWithoutNeuronNeuronPrior; grammaticalSentenceNeuronSub->groupIndex = " << grammaticalSentenceNeuronSub->groupIndex << endl;
				#endif
			
				if(i >= highLevelNeuronPriorArray->size())
				{
					highLevelNeuronPriorArray->push_back(grammaticalSentenceNeuronSub);
				}
				else
				{
					(*highLevelNeuronPriorArray)[i] = grammaticalSentenceNeuronSub;
				}
			}
		}
		
		i++;
	}
		
	return result;
}


		
		
#endif

		
#endif
