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
 * File Name: SANIgenerateCompactSectionedUnique.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p11b 27-May-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Sectioned Unique components - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#include "SANIgenerateCompactSectionedUnique.hpp"
#include "SHAREDvars.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED

bool SANIgenerateCompactSectionedUniqueClass::findAndReconcileVariationWrapper(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_RULES_PRINT_SENTENCES
	cout << "\nfindAndReconcileVariationWrapper:" << endl;
	#endif
		
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED

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
		
		#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
		if(findAndReconcileIncrementalVariationLimitNumComponentsSection(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, &indexInSequenceStart, &highLevelNeuronPriorArray))	
		#else
		if(findAndReconcileIncrementalVariationLimitNumComponentsSection(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, &indexInSequenceStart, &highLevelNeuronPrior))
		#endif
		{
			resultTemp = true;
		}
		else
		{
			result = false;
			completedSentenceNetworkGeneration = true;	//premature exit sentence generation loop
			#ifndef SANI_SEQUENCE_GRAMMAR_POS_MAP_LONGEST_POS_UNAMBIGUOUS_SUBSEQUENCES
			cerr << "!findAndReconcileIncrementalVariationLimitNumComponentsSection" << endl;
			exit(EXIT_ERROR);
			#endif
		}
		
		if(indexInSequenceStart == sentenceContents->size())
		{
			completedSentenceNetworkGeneration = true;
			#ifndef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
			result = resultTemp;
			#endif
		}
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
		cerr << "SANIgenerateCompactSectionedUniqueClass::findAndReconcileVariationWrapper error: !SANIgenerateCompactOperations.findHighestLayerNeuron" << endl;
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
bool SANIgenerateCompactSectionedUniqueClass::findAndReconcileVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_AFTER_GENERATION
	if(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal <= translatorVariables->maxNumberSentences/2)
	{
	#endif
		bool foundAndReconciledMissingOrDifferentIncrementalNeurons = findAndReconcileIncrementalVariationLimitNumComponentsSection(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, indexInSequenceStart, highLevelNeuronPrior);
		result = foundAndReconciledMissingOrDifferentIncrementalNeurons;
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_AFTER_GENERATION
	}
	#endif

	return result;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
bool SANIgenerateCompactSectionedUniqueClass::findAndReconcileIncrementalVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* indexInSequenceStart, constEffective vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray)
#else
bool SANIgenerateCompactSectionedUniqueClass::findAndReconcileIncrementalVariationLimitNumComponentsSection(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* indexInSequenceStart, SANIGroupNeuralNetwork** highLevelNeuronPrior)
#endif
{
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIgenerateCompactSectionedUniqueClass::findAndReconcileIncrementalVariationLimitNumComponentsSection, indexInSequenceStart = " << *indexInSequenceStart << endl;
	#endif
		
	bool foundAndReconciledMissingOrDifferentIncrementalNeurons = false;
		
	//if SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each wordIndex only
	vector<SANIGroupNeuralNetwork*> firstLayer;
	forwardPropogationSentenceData->firstLayer = &firstLayer;
	SANIpropagateCompact.defineFirstLayer(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData);
	
	#ifndef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED_PREVENT_INTRASENTENCE_MATCHING_SECTIONED_RESET_AT_END
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_HISTORY
	forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.clear();
	#endif
	#endif
	
	SANIpropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	
	int indexInSequence = *indexInSequenceStart;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << "(*(forwardPropogationSentenceData->sentenceContents))[*indexInSequenceStart]->tagName = " << (*(forwardPropogationSentenceData->sentenceContents))[*indexInSequenceStart]->tagName << endl;
	#endif
	
	bool stillIdentifyingHighLevelNeurons = true;
	bool completedIdentifyingSentenceHighLevelNeurons = false;
	bool toplevelGroupActivationFound = false;

	vector<SANIGroupNeuralNetwork*> listOfHighLevelNeurons1;	//vectorQ
	vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeuronsCurrent = &listOfHighLevelNeurons1;
	bool creatingNewNeuronSequence1 = false;	//this is used to ensure that only a single neuron is created to store the unrecognised sequence (not 1 neuron per word of the sequence)
	int neuronSequenceIndex1 = 0;
	SANIGroupNeuralNetwork* newNeuronSequenceGroup1 = NULL;
		
		
	int loopIndex = 0;
			
	while(stillIdentifyingHighLevelNeurons)
	{	
		int currentFirstInputNeuronIndexInSequence;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			currentFirstInputNeuronIndexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1-indexInSequence;
		}
		else
		{
			currentFirstInputNeuronIndexInSequence = indexInSequence;
		}
		
		LRPpreprocessorPlainTextWord* currentWord =  (forwardPropogationSentenceData->sentenceContents)->at(currentFirstInputNeuronIndexInSequence);

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

		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		cout << "\e[35m \n** loopIndex = " << loopIndex << ", indexInSequence = " << indexInSequence << ", word = " << currentWord->tagName << " \e[0m" << endl;
		#endif

		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		//if SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each wordIndex only
		SANIGroupNeuralNetwork* currentLayerNeuronGroupStart = (firstLayer)[currentFirstInputNeuronIndexInSequence];	//will only be valid for pos unambiguous words
		#else
		SANIGroupNeuralNetwork* currentLayerNeuronGroupStart = (firstLayer)[currentFirstInputNeuronIndexInSequence];	
		#endif
		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
		cout << "SANIgenerateCompactSectionedUniqueClass::stillIdentifyingHighLevelNeurons" << endl;
		#endif
		
		forwardPropogationSentenceData->toplevelGroupActivationFound = false;

		forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage = true;
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage = NULL;
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial = false;

		#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
		forwardPropogationSentenceData->simultaneousAmbiguousPOSpropagation = true;
		#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT
		forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageRequirePosAmbiguousContext = true;
		#endif	
		#endif
		
		bool resetAllNeuronComponents = true;
		if(SANIpropagateCompact.performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, resetAllNeuronComponents, false, indexInSequence))
		{
			toplevelGroupActivationFound = true;	//note this will only be set true if the entire sentence word indices have been covered (ie only when currentFirstInputNeuronIndexInSequence=start)
		}
		

		if(!(SANIpropagateCompactGenerateOperations.verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) && (!forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial)))		//(!forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial) should be enforced via SANI_SEQUENCE_GRAMMAR_IGNORE_PARTIAL_DETECTIONS
		{
			//case a	
			
			foundAndReconciledMissingOrDifferentIncrementalNeurons = true;

			#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_SIMULTANEOUSLY_PROPAGATE_UNAMBIGUOUS_POS_PERMUTATIONS
			if(LRPpreprocessorPOStagger.isWordPOSambiguous(currentWord))
			{
				//cout << "SANIpropagateCompact.createNewArtificalNeuronRepresentingAmbiguousPOSinputPermutation" << endl;
				currentLayerNeuronGroupStart = SANIpropagateCompact.createNewArtificalNeuronRepresentingAmbiguousPOSinputPermutation(translatorVariables, currentWord, currentFirstInputNeuronIndexInSequence, forwardPropogationSentenceData);
			}	
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_POS_MAP_LONGEST_POS_UNAMBIGUOUS_SUBSEQUENCES
			if(LRPpreprocessorPOStagger.isWordPOSambiguous(currentWord))
			{
				foundAndReconciledMissingOrDifferentIncrementalNeurons = false;	//cannot add sentence to network (it contains pos ambiguous sequences that cannot be reconciled)
				stillIdentifyingHighLevelNeurons = false;
			}
			#endif
			
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
			string neuronName = currentLayerNeuronGroupStart->neuronReference->SANIneuronName;
			cout << "A createOrAppendFirstLevelHiddenLayerGroup, indexInSequence = " << indexInSequence << ", groupIndex = " << currentLayerNeuronGroupStart->groupIndex << ", neuronName = " << neuronName << ", LRPpreprocessorPOStypeNameArray[currentLayerNeuronGroupStart->wordPOStype] = " << LRPpreprocessorPOStypeNameArray[currentLayerNeuronGroupStart->wordPOStype] << endl;
			#endif
				
			createOrAppendFirstLevelHiddenLayerGroup(SANIGroupTypes, forwardPropogationSentenceData, currentLayerNeuronGroupStart, &creatingNewNeuronSequence1, &neuronSequenceIndex1, &newNeuronSequenceGroup1, listOfHighLevelNeuronsCurrent);

			indexInSequence++;
		}
		else
		{	
			creatingNewNeuronSequence1 = false;
			neuronSequenceIndex1 = 0;
			foundAndReconciledMissingOrDifferentIncrementalNeurons = true;

			#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_MARK_AS_UNAMBIGUOUS
			SANIgenerateCompactOperations.markAmbiguousFirstHiddenLayerNeuronsAsUnambiguous(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, true);
			#endif
			
			//case c
			//fully activated group coverage+weight is > partially activated group coverage+weight
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
			cout << "B addNeuronToListSectioned, indexInSequence = " << indexInSequence  << ", nextIndexInSequence = " << SANIgenerateCompactOperations.calculateNextIndexInSequence(forwardPropogationSentenceData) << ", groupIndexes = " << SANInodes.printParseTreeGroupIndicesFlat(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage) << endl;
			#endif

			addNeuronToListSectioned(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage->groupRef, &indexInSequence);
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
				cerr << "SANI_SEQUENCE_GRAMMAR_GENERATE_VERIFY_MORE_THAN_ONE_SECTION_FOUND: SANIgenerateCompactSectionedUniqueClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: only single section found: (*indexInSequenceStart == 0) && indexInSequence == forwardPropogationSentenceData->sentenceContents->size(); SANIpropagateCompact.performPropagationTest should have passed" << endl;
				exit(EXIT_ERROR);
			}
			#endif
		}
		
		loopIndex++;
	}

	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	if(*indexInSequenceStart == 0)
	{
		if(completedIdentifyingSentenceHighLevelNeurons)
		{
			cerr << "SANIgenerateCompactSectionedUniqueClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: matched whole subset in network, prior test should have passed" << endl;
			cerr << "SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE: SANIgenerateCompactSectionedUniqueClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: (*indexInSequenceStart == 0) && completedIdentifyingSentenceHighLevelNeurons" << endl;
			exit(EXIT_ERROR);
		}
	}
	#else
	if((*indexInSequenceStart == 0))
	{
		if((loopIndex == 1) || (listOfHighLevelNeuronsCurrent->size() == 1))
		{
			cerr << "SANIgenerateCompactSectionedUniqueClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: matched whole subset in network, prior test should have passed" << endl;
			cerr << "SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_ELONGATED_TREE: SANIgenerateCompactSectionedUniqueClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: (*indexInSequenceStart == 0) && ((loopIndex == 1) || (listOfHighLevelNeuronsCurrent->size() == 1))" << endl;
			exit(EXIT_ERROR);
		}
	}
	#endif		
				
	SANIGroupNeuralNetwork* grammaticalSentenceNeuron = NULL;
	

	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, &grammaticalSentenceNeuron, true, completedIdentifyingSentenceHighLevelNeurons, *indexInSequenceStart, highLevelNeuronPriorArray);			
	#else
	connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeuronsCurrent, &grammaticalSentenceNeuron, true, completedIdentifyingSentenceHighLevelNeurons, *indexInSequenceStart, highLevelNeuronPrior);
	#endif
	

	#ifndef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED_PREVENT_INTRASENTENCE_MATCHING_SECTIONED_RESET_AT_END
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	SANIGroupNeuralNetwork* highestLayerNeuron = NULL;
	int highestLayerNeuronIndex = 0;
	if(SANIgenerateCompactOperations.findHighestLayerNeuron(highLevelNeuronPriorArray, &highestLayerNeuron, &highestLayerNeuronIndex))
	{	
		SANIgenerateCompactIdentify.markSubNeuronsReset(highestLayerNeuron);
	}
	else
	{
		cerr << "SANIgenerateCompactSectionedUniqueClass::findAndReconcileIncrementalVariationLimitNumComponentsSection error: !SANIgenerateCompactOperations.findHighestLayerNeuron" << endl;
		exit(EXIT_ERROR);
	}
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

	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
	forwardPropogationSentenceData->simultaneousAmbiguousPOSpropagation = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT
	forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageRequirePosAmbiguousContext = false;
	#endif		
	#endif
			
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_GENERATION
	SANIpropagateCompact.printSANInetworkSVG(translatorVariables, SANIGroupTypes);
	#endif
	
	return foundAndReconciledMissingOrDifferentIncrementalNeurons;
}
#endif


#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
bool SANIgenerateCompactSectionedUniqueClass::addNeuronToListSectioned(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence)
{
	SANIGroupNeuralNetwork* highLevelNeuronExpectingVariableLastComponent = NULL;
	return addNeuronToList(SANIGroupTypes, forwardPropogationSentenceData, listOfHighLevelNeurons, neuron, indexInSequence, &highLevelNeuronExpectingVariableLastComponent);
}
#endif
bool SANIgenerateCompactSectionedUniqueClass::addNeuronToList(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence, constEffective SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent)
{	
	bool result = true;
	
	*indexInSequence = SANIgenerateCompactOperations.calculateNextIndexInSequence(forwardPropogationSentenceData);

	listOfHighLevelNeurons->push_back(neuron);	
	
	return result;
}



#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
bool SANIgenerateCompactSectionedUniqueClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, bool completedIdentifyingSentenceHighLevelNeurons, const int indexInSequenceStart, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << "connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection = ";
	cout << "listOfHighLevelNeurons->size() = " << listOfHighLevelNeurons->size() << endl;
	#endif
	
	result = true;

	SANIGroupNeuralNetwork* currentHighLevelNeuron = (*listOfHighLevelNeurons)[0];
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << "currentHighLevelNeuron->groupIndex = " << currentHighLevelNeuron->groupIndex << " ";
	#endif


	int lowestLayerNeuronIndex = 0;
	SANIgenerateCompactOperations.updateHighLevelNeuronHierachy(SANIGroupTypes, forwardPropogationSentenceData, highLevelNeuronPriorArray, lowestLayerNeuronIndex, currentHighLevelNeuron, completedIdentifyingSentenceHighLevelNeurons);

		
	if(completedIdentifyingSentenceHighLevelNeurons)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
		cout << "SANIgenerateCompactSectionedUniqueClass::connectListOfHighLevelNeuronsToNewNeuron - assigning top level sentence neuron;" << endl;
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
bool SANIgenerateCompactSectionedUniqueClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron, const bool completedIdentifyingSentenceHighLevelNeurons, const int indexInSequenceStart, constEffective SANIGroupNeuralNetwork** highLevelNeuronPrior)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << "connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection = ";
	//cout << "listOfHighLevelNeurons->size() = " << listOfHighLevelNeurons->size() << endl;
	#endif
	
	result = true;

	//#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
	SANIGroupNeuralNetwork* grammaticalSentenceNeuronSub = NULL;
	if(indexInSequenceStart == 0)	//(*highLevelNeuronPrior == NULL)
	{
		grammaticalSentenceNeuronSub = SANIgenerateCompactOperations.createNewHiddenLayerGroup(forwardPropogationSentenceData, SANIGroupTypes);
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
	cout << "\t\t SANIgenerateCompactSectionedUniqueClass::connectListOfHighLevelNeuronsToNewNeuron: grammaticalSentenceNeuronSub->groupIndex = " << grammaticalSentenceNeuronSub->groupIndex << endl;
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


		#ifdef SANI_DEBUG_FORMATION
		cout << "\t\t SANIgenerateCompactSectionedUniqueClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection: grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
		#endif

	}


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
		cerr << "\nSANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS: SANIgenerateCompactSectionedUniqueClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection error: (grammaticalSentenceNeuronSub->components.size() > SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)" << endl;
		cerr << "grammaticalSentenceNeuronSub->components.size() = " << grammaticalSentenceNeuronSub->components.size() << endl;
		exit(EXIT_ERROR);
	}
	if(grammaticalSentenceNeuronSub->components.size() < SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)
	{
		cerr << "\nSANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS: SANIgenerateCompactSectionedUniqueClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection error: (grammaticalSentenceNeuronSub->components.size() < SANI_SEQUENCE_GRAMMAR_MAX_NUM_COMPONENTS)" << endl;
		cerr << "grammaticalSentenceNeuronSub->components.size() = " << grammaticalSentenceNeuronSub->components.size() << endl;
		exit(EXIT_ERROR);
	}
	#endif
		
		
	if(completedIdentifyingSentenceHighLevelNeurons)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
		cout << "SANIgenerateCompactSectionedUniqueClass::connectListOfHighLevelNeuronsToNewNeuron - assigning top level sentence neuron;" << endl;
		#endif
		(*grammaticalSentenceNeuron) = grammaticalSentenceNeuronSub;
		#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_TOP_LEVEL_NEURONS
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


	


bool SANIgenerateCompactSectionedUniqueClass::createOrAppendFirstLevelHiddenLayerGroup(const vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentLayerNeuronGroupStart, const bool* creatingNewNeuronSequence, const int* neuronSequenceIndex, constEffective SANIGroupNeuralNetwork** newNeuronSequenceGroup, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons)
{
	bool result = true;
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIgenerateCompactSectionedUniqueClass::createOrAppendFirstLevelHiddenLayerGroup" << endl;
	#endif

	//currentLayerNeuronGroupStart->inputLayerNeuron = true;	//moved to SANIformation: @SANI1p6a

	listOfHighLevelNeurons->push_back(currentLayerNeuronGroupStart);	
	
	return result;
}		
			


		
#endif
