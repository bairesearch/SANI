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
 * File Name: SANIgenerateCompactSectionedVariable.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p6a 20-March-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Sectioned Variable components - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#include "SANIgenerateCompactSectionedVariable.hpp"
#include "SHAREDvars.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED


bool SANIgenerateCompactSectionedVariableClass::findAndReconcileVariationWrapper(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_RULES_PRINT_SENTENCES
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
					for(int j=0; j<neuronWithVariableEndComponentStartInput->SANIbackGroupConnectionList.size(); j++)
					{
						SANIGroupNeuralNetwork* neuronWithVariableEndComponentStartInputNeuron = neuronWithVariableEndComponentStartInput->SANIbackGroupConnectionList[j];
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
						for(int j=0; j<neuronWithVariableEndComponentStartInput->SANIbackGroupConnectionList.size(); j++)
						{
							SANIGroupNeuralNetwork* neuronWithVariableEndComponentStartInputNeuron = neuronWithVariableEndComponentStartInput->SANIbackGroupConnectionList[j];
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
			cerr << "SANIgenerateCompactSectionedVariableClass::findAndReconcileVariationWrapper error; initialised (listOfHighLevelNeurons.size() > 0)" << endl;
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
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_HISTORY
	forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.clear();
	#endif
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT	
	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	SANIGroupNeuralNetwork* highestLayerNeuron = NULL;
	int highestLayerNeuronIndex = 0;
	if(SANIgenerateCompactOperations.findHighestLayerNeuron(highLevelNeuronPriorArray, &highestLayerNeuron, &highestLayerNeuronIndex))
	{	
		SANIgenerateCompactIdentify.markSubNeuronsReset(highestLayerNeuron);
	}
	else
	{
		cerr << "SANIgenerateCompactSectionedVariableClass::findAndReconcileVariationWrapper error: !SANIgenerateCompactOperations.findHighestLayerNeuron" << endl;
		exit(EXIT_ERROR);
	}
	#else
	SANIgenerateCompactIdentify.markSubNeuronsReset(*highLevelNeuronPrior);
	#endif
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
			


#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED

#ifndef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
bool SANIgenerateCompactSectionedVariableClass::findAndReconcileVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior)
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

	return result;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
bool SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, const bool enableVariableEndComponentDetection, const bool enableVariableEndComponentDetectionExpectedCompletedIdentifyingSentenceHighLevelNeurons, const bool passOverGenerationFindVariableEndComponent, const bool passOverGenerationFindGroupNeuron, bool* foundGroupNeuron, bool* foundCoverageVariableEndComponent, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons)
#else
bool SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray)
#endif
#else
bool SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool supportVariableFirstComponents, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior)
#endif
{
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariationLimitNumComponentsSection" << endl;
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
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_HISTORY
	forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.clear();
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
	
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISALLOW_IDENTICAL_INPUTS
		forwardPropogationSentenceData->highLevelNeuronPriorTemp = NULL;
		#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		SANIGroupNeuralNetwork* highLevelNeuronPriorTemp = NULL;
		int lowestLayerNeuronIndex = 0;
		bool foundLowestLayerNeuron = SANIgenerateCompactOperations.findLowestLayerNeuron(highLevelNeuronPriorArray, &highLevelNeuronPriorTemp, &lowestLayerNeuronIndex);	
		#else
		SANIGroupNeuralNetwork* highLevelNeuronPriorTemp = *highLevelNeuronPrior;
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ALLOW_IDENTICAL_INPUTS_NON_STRING
		if((highLevelNeuronPriorTemp != NULL) && !SANInodes.isNeuronString(highLevelNeuronPriorTemp))
		{
		#endif
			forwardPropogationSentenceData->highLevelNeuronPriorTemp = highLevelNeuronPriorTemp;
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ALLOW_IDENTICAL_INPUTS_NON_STRING
		}
		#endif
		#endif
		
		
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
		if(enableVariableEndComponentDetection || passOverGenerationFindVariableEndComponent)
		{
			forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent = true;
		}
		#else
		forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent = true;	//not supported
		#endif
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
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
		
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING_COMPREHENSIVE
		#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE_SIZEABLE_SUBTREE
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent)
		{
			if(highLevelNeuronPriorArray->size() > 0)	//redundant
			{
				SANIGroupNeuralNetwork* lowestLayerNeuron = NULL;
				int lowestLayerNeuronIndex = 0;
				bool foundLowestLayerNeuron = SANIgenerateCompactOperations.findLowestLayerNeuron(highLevelNeuronPriorArray, &lowestLayerNeuron, &lowestLayerNeuronIndex);
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
		bool foundLowestLayerNeuron = SANIgenerateCompactOperations.findLowestLayerNeuron(highLevelNeuronPriorArray, &highLevelNeuronPriorTemp, &lowestLayerNeuronIndex);
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
		cerr << "SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariation hasnt yet been coded for SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
		exit(EXIT_ERROR);
		#else
		SANIGroupNeuralNetwork* currentLayerNeuronGroupStart = (firstLayer)[currentFirstInputNeuronIndexInSequence];
		#endif
		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
		cout << "SANIgenerateCompactSectionedVariableClass::stillIdentifyingHighLevelNeurons" << endl;
		#endif
		
		forwardPropogationSentenceData->toplevelGroupActivationFound = false;

		forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage = true;
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage = NULL;
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial = false;
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent = false;
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableEndComponent = false;
		#endif
		
		
		bool resetAllNeuronComponents = true;
		if(SANIpropagateCompact.performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, false, indexInSequence))
		{
			toplevelGroupActivationFound = true;	//note this will only be set true if the entire sentence word indices have been covered (ie only when currentFirstInputNeuronIndexInSequence=start)
		}
		

		#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
		if(!(SANIpropagateCompactGenerateOperations.verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) && (!forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial)))		//(!forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial) should be enforced via SANI_SEQUENCE_GRAMMAR_IGNORE_PARTIAL_DETECTIONS
		#else
		if(!SANIpropagateCompactGenerateOperations.verifyActivatedNeuronsAtLeastOne(forwardPropogationSentenceData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, newNeuronSequenceGroup1))
		#endif
		{
			//case a	
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
			cout << "A createOrAppendFirstLevelHiddenLayerGroup, indexInSequence = " << indexInSequence << ", groupIndex = " << currentLayerNeuronGroupStart->groupIndex << ", LRPpreprocessorPOStypeNameArray[currentLayerNeuronGroupStart->wordPOStype] = " << LRPpreprocessorPOStypeNameArray[currentLayerNeuronGroupStart->wordPOStype] << endl;
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
			if(!(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableEndComponent))
			{	
			#endif
				//case c
				//fully activated group coverage+weight is > partially activated group coverage+weight
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
				cout << "B addNeuronToListSectioned, indexInSequence = " << indexInSequence  << ", nextIndexInSequence = " << SANIgenerateCompactOperations.calculateNextIndexInSequence(forwardPropogationSentenceData) << ", groupIndexes = " << SANInodes.printParseTreeGroupIndicesFlat(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) << endl;
				#endif

				addNeuronToListSectioned(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef, &indexInSequence);

				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
				if(passOverGenerationFindGroupNeuron)
				{
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
					if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent)
					{
						cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT: SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: passOverGenerationFindGroupNeuron && (forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent)" << endl;
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
				cout << "C addNeuronToListSectioned, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex = " << forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex << endl;
				#endif
				
				addNeuronToListSectioned(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef, &indexInSequence);

				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
				if(passOverGenerationFindVariableEndComponent)
				{
					//cout << "passOverGenerationFindVariableEndComponent, foundCoverageVariableEndComponent = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableEndComponent" << endl;
					
					*foundCoverageVariableEndComponent = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableEndComponent;
				}
				else
				{
				#endif
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC2
					cout << "set activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp" << endl;
					#endif
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef->activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp = forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableEndComponent;	//always true
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
		SANIpropagateOperationsParseTree.deinitialiseParseTreeGroupList(SANIGroupTypes, &(forwardPropogationSentenceData->parseTreeGroupListPropagate));
		#endif
		
		stillIdentifyingHighLevelNeurons = false;
		if(indexInSequence < forwardPropogationSentenceData->sentenceContents->size())
		{
			#ifndef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
			if((*indexInSequenceStart == 0) && (loopIndex < 1))
			{
				//must find 2 neurons for first layer in sentence network
				stillIdentifyingHighLevelNeurons = true;
			}
			#endif
		}
		else
		{			
			completedIdentifyingSentenceHighLevelNeurons = true;
			
			#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_VERIFY_MORE_THAN_ONE_SECTION_FOUND
			if(*indexInSequenceStart == 0)
			{
				cerr << "SANI_SEQUENCE_GRAMMAR_GENERATE_VERIFY_MORE_THAN_ONE_SECTION_FOUND: SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: only single section found: (*indexInSequenceStart == 0) && indexInSequence == forwardPropogationSentenceData->sentenceContents->size(); SANIpropagateCompact.executePosRelTranslatorNeuralNetworkPart1 should have passed" << endl;
				exit(EXIT_ERROR);
			}
			#endif
		}
		
		loopIndex++;
	}

	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	if(*indexInSequenceStart == 0)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
		if(completedIdentifyingSentenceHighLevelNeurons && !enableVariableEndComponentDetectionExpectedCompletedIdentifyingSentenceHighLevelNeurons)
		#else
		if(completedIdentifyingSentenceHighLevelNeurons)
		#endif
		{
			cerr << "SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: matched whole subset in network, prior test should have passed" << endl;
			cerr << "SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE: SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: (*indexInSequenceStart == 0) && completedIdentifyingSentenceHighLevelNeurons" << endl;
			exit(EXIT_ERROR);
		}
	}
	#else
	if((*indexInSequenceStart == 0))
	{
		if((loopIndex == 1) || (listOfHighLevelNeuronsCurrent->size() == 1))
		{
			cerr << "SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: matched whole subset in network, prior test should have passed" << endl;
			cerr << "SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_ELONGATED_TREE: SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: (*indexInSequenceStart == 0) && ((loopIndex == 1) || (listOfHighLevelNeuronsCurrent->size() == 1))" << endl;
			exit(EXIT_ERROR);
		}
	}
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
		#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, &grammaticalSentenceNeuron, true, completedIdentifyingSentenceHighLevelNeurons, *indexInSequenceStart, highLevelNeuronPriorArray);			
		#else
		connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, &grammaticalSentenceNeuron, true, completedIdentifyingSentenceHighLevelNeurons, *indexInSequenceStart, highLevelNeuronPrior);
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
	if(SANIgenerateCompactOperations.findHighestLayerNeuron(highLevelNeuronPriorArray, &highestLayerNeuron, &highestLayerNeuronIndex))
	{	
		SANIgenerateCompactIdentify.markSubNeuronsReset(highestLayerNeuron);
	}
	else
	{
		cerr << "SANIgenerateCompactSectionedVariableClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: !SANIgenerateCompactOperations.findHighestLayerNeuron" << endl;
		exit(EXIT_ERROR);
	}
	#endif
	#else
	SANIgenerateCompactIdentify.markSubNeuronsReset(*highLevelNeuronPrior);
	#endif
	#else
	SANIgenerateCompactIdentify.markSubNeuronsReset(grammaticalSentenceNeuron);
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
#endif


#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
bool SANIgenerateCompactSectionedVariableClass::addNeuronToListSectioned(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence)
{
	SANIGroupNeuralNetwork* highLevelNeuronExpectingVariableLastComponent = NULL;
	return addNeuronToList(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeurons, neuron, indexInSequence, &highLevelNeuronExpectingVariableLastComponent);
}
#endif
bool SANIgenerateCompactSectionedVariableClass::addNeuronToList(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence, constEffective SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent)
{	
	bool result = true;
	
	*indexInSequence = SANIgenerateCompactOperations.calculateNextIndexInSequence(forwardPropogationSentenceData);
		
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	SANIgenerateCompactIdentify.markSubNeurons(neuron);	
	#endif
	#else
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	neuron->marked = true;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_DO_NOT_HAVE_CONNECTIONS_TO_ANY_NEW_CANDIDATE
	SANIgenerateCompactOperations.addParentNeuronsToList(&(forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory), neuron);
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	SANIgenerateCompactOperations.addParentNeuronsToListReset(neuron);
	#endif
	#endif
	
	#endif
	
	/*//this code has been moved outside of SANIgenerateCompactSectionedVariableClass::addNeuronToList @SANI1m6a
	#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
	addVariableFirstComponent(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeurons, neuron);
	#endif
	*/
	
	#endif

	listOfHighLevelNeurons->push_back(neuron);	
	
	return result;
}



#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
bool SANIgenerateCompactSectionedVariableClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, bool completedIdentifyingSentenceHighLevelNeurons, const int indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << "connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection = ";
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
			cerr << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS: SANIgenerateCompactSectionedVariableClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection: (currentHighLevelNeuron->inputLayerNeuron)" << endl;
			exit(EXIT_ERROR);
		}
		
		SANIgenerateCompactOperations.addVariableComponentToGroup(forwardPropogationSentenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSubHigher, false);

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
		SANIgenerateCompactOperations.updateHighLevelNeuronHierachy(SANIGroupTypes, forwardPropogationSentenceData, highLevelNeuronPriorArray, lowestLayerNeuronIndex, currentHighLevelNeuron, completedIdentifyingSentenceHighLevelNeurons);

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
			bool foundLowestLayerNeuron = SANIgenerateCompactOperations.findLowestLayerNeuron(highLevelNeuronPriorArray, &lowestLayerNeuron, &lowestLayerNeuronIndex);

			grammaticalSentenceNeuronSubHigher = currentHighLevelNeuron;
			SANIgenerateCompactOperations.addVariableComponentToGroup(forwardPropogationSentenceData, lowestLayerNeuron, grammaticalSentenceNeuronSubHigher, true);

			#ifdef SANI_DEBUG_FORMATION
			cout << "currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp" << endl;
			#endif
			#ifdef SANI_DEBUG_FORMATION2
			SANInodes.printParseTreeDebugIndentation(lowestLayerNeuronIndex);
			cout << "SANIgenerateCompactOperations.addVariableComponentToGroup: lowestLayerNeuron->groupIndex = " << lowestLayerNeuron->groupIndex << " to grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
			#endif

			#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
			grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, lowestLayerNeuron->networkDepth + 1);
			#endif

			currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp = false;

			(*highLevelNeuronPriorArray)[lowestLayerNeuronIndex] = grammaticalSentenceNeuronSubHigher;	//added GIA3k16a
			if(completedIdentifyingSentenceHighLevelNeurons)	//added GIA3k16a
			{
				(*highLevelNeuronPriorArray)[lowestLayerNeuronIndex] = NULL;	//added GIA3k16a
				SANIgenerateCompactOperations.updateHighLevelNeuronHierachy(SANIGroupTypes, forwardPropogationSentenceData, highLevelNeuronPriorArray, lowestLayerNeuronIndex, grammaticalSentenceNeuronSubHigher, completedIdentifyingSentenceHighLevelNeurons);
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
			SANIgenerateCompactOperations.updateHighLevelNeuronHierachy(SANIGroupTypes, forwardPropogationSentenceData, highLevelNeuronPriorArray, lowestLayerNeuronIndex, currentHighLevelNeuron, completedIdentifyingSentenceHighLevelNeurons);

		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
		}
		#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	}
	#endif
		
	if(completedIdentifyingSentenceHighLevelNeurons)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
		cout << "SANIgenerateCompactSectionedVariableClass::connectListOfHighLevelNeuronsToNewNeuron - assigning top level sentence neuron;" << endl;
		#endif

		SANIGroupNeuralNetwork* highestLayerNeuron = NULL;
		int highestLayerNeuronIndex = 0;
		bool foundLowestLayerNeuron = SANIgenerateCompactOperations.findHighestLayerNeuron(highLevelNeuronPriorArray, &highestLayerNeuron, &highestLayerNeuronIndex);
	
		(*grammaticalSentenceNeuron) = highestLayerNeuron;
		#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_TOP_LEVEL_NEURONS
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
bool SANIgenerateCompactSectionedVariableClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, const bool completedIdentifyingSentenceHighLevelNeurons, const int indexInSequenceStart, constEffective SANIGroupNeuralNetwork** highLevelNeuronPrior)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << "connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection = ";
	//cout << "listOfHighLevelNeurons->size() = " << listOfHighLevelNeurons->size() << endl;
	#endif
	
	result = true;

	SANIGroupNeuralNetwork* grammaticalSentenceNeuronSub = NULL;
	if(indexInSequenceStart == 0)	//(*highLevelNeuronPrior == NULL)
	{
		grammaticalSentenceNeuronSub = SANIgenerateCompactOperations.createNewHiddenLayerGroup(forwardPropogationSentenceData, SANIGroupTypes);
	}
	else
	{
		grammaticalSentenceNeuronSub = *highLevelNeuronPrior;
	}
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << grammaticalSentenceNeuronSub->groupIndex << " ";
	#endif
	#ifdef SANI_DEBUG_FORMATION
	cout << "\t\t SANIgenerateCompactSectionedVariableClass::connectListOfHighLevelNeuronsToNewNeuron: grammaticalSentenceNeuronSub->groupIndex = " << grammaticalSentenceNeuronSub->groupIndex << endl;
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
	//rule #3;
	if(grammaticalSentenceNeuronSub->activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp)
	{
		cerr << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS: SANIgenerateCompactSectionedVariableClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection - doesnt currently support SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_ELONGATED_TREE" << endl;
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

				bool componentTypeString = SANInodes.calculateComponentTypeString(currentHighLevelNeuron);
				SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSentenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSub, componentTypeString, false);

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
				SANIgenerateCompactOperations.addVariableComponentToGroup(forwardPropogationSentenceData, grammaticalSentenceNeuronSub, grammaticalSentenceNeuronSubHigher, true);

				#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
				grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, grammaticalSentenceNeuronSub->networkDepth + 1);
				#endif

				currentHighLevelNeuron->activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp = false;

				grammaticalSentenceNeuronSub = grammaticalSentenceNeuronSubHigher;
			}
			else
			{
			#endif
				grammaticalSentenceNeuronSubHigher = SANIgenerateCompactOperations.createNewHiddenLayerGroup(forwardPropogationSentenceData, SANIGroupTypes);
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
				cout << grammaticalSentenceNeuronSubHigher->groupIndex << " ";
				#endif
				SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSentenceData, grammaticalSentenceNeuronSub, grammaticalSentenceNeuronSubHigher, false, false);

				#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
				grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, grammaticalSentenceNeuronSub->networkDepth + 1);
				#endif
	
				bool componentTypeString = SANInodes.calculateComponentTypeString(currentHighLevelNeuron);
				SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSentenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSubHigher, componentTypeString, false);

				#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
				grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, currentHighLevelNeuron->networkDepth + 1);
				#endif

				grammaticalSentenceNeuronSub = grammaticalSentenceNeuronSubHigher;

			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
			}
			#endif	

			#ifdef SANI_DEBUG_FORMATION
			cout << "\t\t SANIgenerateCompactSectionedVariableClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection: grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
			#endif

		}
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
	}
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
	SANInodes.adjustNetworkDepth(grammaticalSentenceNeuronSub);
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	SANInodes.adjustNetworkDepthReset(grammaticalSentenceNeuronSub);
	#endif
	#endif
		
	SANInodes.updateComponentsOwnerGroupAndIndexes(grammaticalSentenceNeuronSub, &(grammaticalSentenceNeuronSub->components), false, NULL);
	

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
	if(grammaticalSentenceNeuronSub->components.size() > SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)
	{
		cerr << "\nSANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS: SANIgenerateCompactSectionedVariableClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection error: (grammaticalSentenceNeuronSub->components.size() > SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)" << endl;
		cerr << "grammaticalSentenceNeuronSub->components.size() = " << grammaticalSentenceNeuronSub->components.size() << endl;
		exit(EXIT_ERROR);
	}
	if(grammaticalSentenceNeuronSub->components.size() < SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)
	{
		cerr << "\nSANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS: SANIgenerateCompactSectionedVariableClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection error: (grammaticalSentenceNeuronSub->components.size() < SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)" << endl;
		cerr << "grammaticalSentenceNeuronSub->components.size() = " << grammaticalSentenceNeuronSub->components.size() << endl;
		exit(EXIT_ERROR);
	}
	#endif
		
		
	if(completedIdentifyingSentenceHighLevelNeurons)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
		cout << "SANIgenerateCompactSectionedVariableClass::connectListOfHighLevelNeuronsToNewNeuron - assigning top level sentence neuron;" << endl;
		#endif
		(*grammaticalSentenceNeuron) = grammaticalSentenceNeuronSub;
		#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_TOP_LEVEL_NEURONS
		if(createTopLevelNeuron)
		{
			(*grammaticalSentenceNeuron)->topLevelSentenceNeuron = true;
		}
		#endif		
	}

	*highLevelNeuronPrior = grammaticalSentenceNeuronSub;
	
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


	


bool SANIgenerateCompactSectionedVariableClass::createOrAppendFirstLevelHiddenLayerGroup(const vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentLayerNeuronGroupStart, const bool* creatingNewNeuronSequence, const int* neuronSequenceIndex, constEffective SANIGroupNeuralNetwork** newNeuronSequenceGroup, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons)
{
	bool result = true;
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIgenerateCompactSectionedVariableClass::createOrAppendFirstLevelHiddenLayerGroup" << endl;
	#endif

	//currentLayerNeuronGroupStart->inputLayerNeuron = true;	//moved to SANIformation: @SANI1p6a

	listOfHighLevelNeurons->push_back(currentLayerNeuronGroupStart);
	
	return result;
}		
			



#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE

#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH
int SANIgenerateCompactSectionedVariableClass::calculateLowestLayerNeuronMaxDepth(const vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray)
{
	int lowestLayerNeuronMaxDepth = 0;
	
	SANIGroupNeuralNetwork* lowestLayerNeuron = NULL;
	int lowestLayerNeuronIndex = 0;
	bool foundLowestLayerNeuron = SANIgenerateCompactOperations.findLowestLayerNeuron(highLevelNeuronPriorArray, &lowestLayerNeuron, &lowestLayerNeuronIndex);
	if(foundLowestLayerNeuron)
	{
		lowestLayerNeuronMaxDepth = lowestLayerNeuron->networkDepth;	
	}
	
	return lowestLayerNeuronMaxDepth;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
bool SANIgenerateCompactSectionedVariableClass::findTempVariableEndComponentNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** variableEndComponentNeuron)
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
		//cerr << "SANIgenerateCompactSectionedVariableClass::findTempVariableEndComponentNeuron: error: (highLevelNeuronPriorArray->size() == 0)" << endl;
	}
	
	return foundTempVariableEndComponentNeuron;
}
void SANIgenerateCompactSectionedVariableClass::setTempVariableEndComponentNeuron(vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork* variableEndComponentNeuron)
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
				cerr << "SANIgenerateCompactSectionedVariableClass::setTempVariableEndComponentNeuron: (highLevelNeuronPriorArray[0] != NULL) && (highLevelNeuronPriorArray[0] != variableEndComponentNeuron)" << endl;
				exit(EXIT_ERROR);
			}
		}
		(*highLevelNeuronPriorArray)[0] = variableEndComponentNeuron;
	}
	else
	{
		cerr << "SANIgenerateCompactSectionedVariableClass::setTempVariableEndComponentNeuron: error: (highLevelNeuronPriorArray->size() == 0)" << endl;
		exit(EXIT_ERROR);		
	}
}
#endif
#endif



		
#endif
