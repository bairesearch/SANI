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
 * File Name: SANIgenerateCompactContinuous.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1q2b 19-September-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Continuous - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#include "SANIgenerateCompactContinuous.hpp"
#include "SHAREDvars.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_CONTINUOUS

bool SANIgenerateCompactContinuousClass::findAndReconcileVariationWrapper(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, const vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool supportVariableFirstComponents)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_RULES_PRINT_SENTENCES
	cout << "\nfindAndReconcileVariationWrapper:" << endl;
	#endif
	
	if(!findAndReconcileVariation(translatorVariables, SANIGroupTypes, forwardPropogationSequenceData, supportVariableFirstComponents))
	{
		result = false;
	}
		
	return result;
}
			
bool SANIgenerateCompactContinuousClass::findAndReconcileVariation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool supportVariableFirstComponents)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_AFTER_GENERATION
	if(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal <= translatorVariables->maxNumberSentences/2)
	{
	#endif
		bool foundAndReconciledMissingOrDifferentIncrementalNeurons = findAndReconcileIncrementalVariation(translatorVariables, SANIGroupTypes, forwardPropogationSequenceData, supportVariableFirstComponents);
		result = foundAndReconciledMissingOrDifferentIncrementalNeurons;

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_TEST_VERIFICATION_AFTER_GENERATION
	}
	#endif

	return result;
}

bool SANIgenerateCompactContinuousClass::findAndReconcileIncrementalVariation(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool supportVariableFirstComponents)
{
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIgenerateCompactContinuousClass::findAndReconcileIncrementalVariation" << endl;
	#endif
		
	bool foundAndReconciledMissingOrDifferentIncrementalNeurons = false;
	
	
	//if SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each sequenceIndex only
	vector<SANIGroupNeuralNetwork*> firstLayer;
	forwardPropogationSequenceData->firstLayer = &firstLayer;
	SANIpropagateCompact.defineFirstLayer(translatorVariables, SANIGroupTypes, forwardPropogationSequenceData);
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_HISTORY
	forwardPropogationSequenceData->listOfHighLevelNeuronsCompleteHistory.clear();
	#endif
	
	/*
	algorithm2;
	let l be the first sequenceElement in the sentence sequenceElement sequence
	for every input neuron in sentence sequenceElement sequence starting at l*
		propagate its input through the network
			ensure sequenceElement index is contiguous during forward prop (SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT: SANIpropagateOperations.componentWordConnectivityTestsWrapper)
			mark all hidden neurons that fire (these are candidates)
			mark all hidden neurons that are only partially activated
				these become candidates for splitting
				never split a neuron below 2 components
			take the hidden neuron that support the most sequenceElements in the sentence from l (whether it is fully activated or partially activated)
				if necessary (it is partially activated) split the neuron;
					b) take its components and forge a new neuron
					add to vector q
				c) if it is fully activated add to vector q
				then repeat* from last sequenceElement index (l)
		if no appropriate hidden neuron is found create a a) new hidden neuron containing the first input in the current part of the sentence sequenceElement sequence*
			add to vector q
			OLD: how to determine whether to use this new neuron or just connect its first input to..
			then repeat* from last sequenceElement index (l)
	combine all the a) new, b) splitted, or c) fully activated neurons into a d) new neuron on a higher layer
		this new higher layer neuron d) represents a successfully parsed sentence m
			OPTIONAL: add this neuron to the highest level full gramatical sentence neuron
	issue with this algorithm is that it cannot reuse groups for embeded optional components eg rcmod;
		1 the dog barks
		2 the dog that is near the house barks
		will create 2 independent top level groups (the dog barks group will not be reused by 2)
		possibly look for and catch recursive group connectivity and ensure that groups can only support varied sequenceElement index ranges when they refer to recursive (optional) groups
	*/
	
	SANIpropagateOperations.setParseSentenceReverse(true, forwardPropogationSequenceData);
	//SANIGroupNeuralNetwork* firstLayerNeuronGroupStart = NULL;
	
	int indexInSequence = 0;
	bool stillIdentifyingHighLevelNeurons = true;
	bool toplevelGroupActivationFound = false;

	vector<SANIGroupNeuralNetwork*> listOfHighLevelNeurons1;	//vectorQ
	vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeuronsCurrent = &listOfHighLevelNeurons1;
	bool creatingNewNeuronSequence1 = false;	//this is used to ensure that only a single neuron is created to store the unrecognised sequence (not 1 neuron per sequenceElement of the sequence)
	int neuronSequenceIndex1 = 0;
	SANIGroupNeuralNetwork* newNeuronSequenceGroup1 = NULL;

	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
	bool findingCandidateComponent1 = true;
	bool findingCandidateComponent2 = false;
	bool findingCentralVariation = false;
	int activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed = INT_DEFAULT_VALUE;	//for central variation detection only
	int activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed = INT_DEFAULT_VALUE;	//for central variation detection only
	SANIGroupParseTree* candidateNeuronBaseParseTree = NULL;
	vector<SANIGroupNeuralNetwork*> listOfHighLevelNeuronsCentralVariation;
	#endif
	
	int loopIndex = 0;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableStartComponent = false;	//see SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady: if(forwardPropogationSignalData->firstIndexInSequence == 0) check
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
	forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableEndComponent = false;
	#endif
	//#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
	SANIGroupNeuralNetwork* highLevelNeuronExpectingVariableLastComponent = NULL;
	//#endif
		
	while(stillIdentifyingHighLevelNeurons)
	{	
		int currentFirstInputNeuronIndexInSequence;
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			currentFirstInputNeuronIndexInSequence = forwardPropogationSequenceData->sentenceContents->size()-1-indexInSequence;
		}
		else
		{
			currentFirstInputNeuronIndexInSequence = indexInSequence;
		}
		
		LRPpreprocessorPlainTextWord* currentWord = (*(forwardPropogationSequenceData->sentenceContents))[currentFirstInputNeuronIndexInSequence];
		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
		cout << "currentWord->tagName = " << currentWord->tagName << endl;
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
		if(highLevelNeuronExpectingVariableLastComponent == NULL)
		{
			forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableEndComponent = true;
		}
		else
		{
			forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableEndComponent = false;
		}
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		if(loopIndex > 0)
		{
			forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableStartComponent = true;
		}
		#endif

		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		cout << "\e[35m \n** loopIndex = " << loopIndex << ", indexInSequence = " << indexInSequence << ", sequenceElement = " << (forwardPropogationSequenceData->sentenceContents)->at(currentFirstInputNeuronIndexInSequence)->tagName << " \e[0m" << endl;
		#endif

		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		//if SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY, first layer contains firstPOS neuron for each sequenceIndex only
		SANIGroupNeuralNetwork* currentLayerNeuronGroupStart = (firstLayer)[currentFirstInputNeuronIndexInSequence];	//will only be valid for pos unambiguous sequenceElements
		#else
		SANIGroupNeuralNetwork* currentLayerNeuronGroupStart = (firstLayer)[currentFirstInputNeuronIndexInSequence];
		#endif
		
		forwardPropogationSequenceData->toplevelGroupActivationFound = false;

		forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage = true;
		forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage = NULL;
		forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoveragePartial = false;
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponent = false;
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
		forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableEndComponent = false;
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
		forwardPropogationSequenceData->simultaneousAmbiguousPOSpropagation = true;
		#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT
		forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageRequirePosAmbiguousContext = true;
		#endif
		#endif

		#ifdef SANI_SEQUENCE_GRAMMAR_DISALLOW_DIRECT_REFSET_AND_DELIMITER_POS_COMPONENTS_ON_SAME_NEURON
		bool highLevelListHasDirectInputRefSetPOS = false;
		bool highLevelListHasDirectInputRefSetDelimiterPOS = false;
		determineListHighLevelNeuronsDirectInput(newNeuronSequenceGroup1, &highLevelListHasDirectInputRefSetPOS, &highLevelListHasDirectInputRefSetDelimiterPOS);
		#endif
				
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
		if(findingCentralVariation)
		{
			bool resetAllNeuronComponents = true;
			if(SANIpropagateCompact.performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSequenceData, resetAllNeuronComponents, false, indexInSequence, activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed, activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed))
			{
				toplevelGroupActivationFound = true;	//note this will only be set true if the entire sentence sequenceElement indices have been covered (ie only when currentFirstInputNeuronIndexInSequence=start)
			}
		}
		else
		{
		#endif			
			bool resetAllNeuronComponents = true;
			if(SANIpropagateCompact.performPropagation(translatorVariables, SANIGroupTypes, forwardPropogationSequenceData, resetAllNeuronComponents, false, indexInSequence))
			{
				toplevelGroupActivationFound = true;	//note this will only be set true if the entire sentence sequenceElement indices have been covered (ie only when currentFirstInputNeuronIndexInSequence=start)
			}
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
		}
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
		if(findingCandidateComponent2)
		{
			findingCandidateComponent2 = false;
			
			if(forwardPropogationSequenceData->foundCandidateComponent2)
			{
				listOfHighLevelNeuronsCurrent->push_back(candidateNeuronBaseParseTree->groupRef);

				SANIGroupParseTree* candidateComponent2sourceParseTreeGroup = forwardPropogationSequenceData->candidateComponent2sourceParseTreeGroup;

				if(forwardPropogationSequenceData->parseSentenceReverse)
				{
					activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed = candidateNeuronBaseParseTree->parseTreeMinSequenceIndex-1;
					activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed = candidateComponent2sourceParseTreeGroup->parseTreeMaxSequenceIndex+1;
				}
				else
				{
					activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed = candidateNeuronBaseParseTree->parseTreeMaxSequenceIndex+1;
					activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed = candidateComponent2sourceParseTreeGroup->parseTreeMinSequenceIndex-1;
				}
				
				if(activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed == activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed)
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
				SANIGroupNeuralNetwork* newNeuron = splitNeuron(SANIGroupTypes, forwardPropogationSequenceData, candidateNeuronBaseParseTree);	//listOfHighLevelNeuronsCurrent CHECKTHIS
				listOfHighLevelNeuronsCurrent->push_back(newNeuron);
								
				//go back to standard findAndReconcileIncrementalVariation routine
			}
			
			forwardPropogationSequenceData->findingCandidateComponent2 = false;
			forwardPropogationSequenceData->candidateComponent2ToFind = NULL;
			candidateNeuronBaseParseTree = NULL;
		}
		else
		{
		#endif
			if(!SANIpropagateCompactGenerateOperations.verifyActivatedNeuronsAtLeastOne(forwardPropogationSequenceData, forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage, newNeuronSequenceGroup1))
			{
				//case a						
				foundAndReconciledMissingOrDifferentIncrementalNeurons = true;
				
				#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_SIMULTANEOUSLY_PROPAGATE_UNAMBIGUOUS_POS_PERMUTATIONS
				if(LRPpreprocessorPOStagger.isWordPOSambiguous(currentWord))
				{
					currentLayerNeuronGroupStart = SANIpropagateCompact.createNewArtificalNeuronRepresentingAmbiguousPOSinputPermutation(translatorVariables, currentWord, currentFirstInputNeuronIndexInSequence, forwardPropogationSequenceData);
				}	
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_POS_MAP_LONGEST_POS_UNAMBIGUOUS_SUBSEQUENCES
				if(LRPpreprocessorPOStagger.isWordPOSambiguous(currentWord))
				{
					foundAndReconciledMissingOrDifferentIncrementalNeurons = false;	//cannot add sentence to network (it contains pos ambiguous sequences that cannot be reconciled)
					stillIdentifyingHighLevelNeurons = false;
				}
				#endif
				
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
				cout << "A createOrAppendFirstLevelHiddenLayerGroup, indexInSequence = " << indexInSequence << ", groupIndex = " << currentLayerNeuronGroupStart->groupIndex << endl;
				#endif
				
				#ifdef SANI_SEQUENCE_GRAMMAR_DISALLOW_DIRECT_REFSET_AND_DELIMITER_POS_COMPONENTS_ON_SAME_NEURON
				bool refsetDelimiterPOSfound = sequenceElementHasRefsetDelimiterPOS(currentLayerNeuronGroupStart);
				bool highLevelListEnd = false;
				if(refsetDelimiterPOSfound)
				{
					if(highLevelListHasDirectInputRefSetPOS)
					{
						highLevelListEnd = true;
					}
				}
				else
				{
					if(highLevelListHasDirectInputRefSetDelimiterPOS)
					{
						highLevelListEnd = true;
					}
				}
				if(highLevelListEnd)
				{
					cout << "highLevelListEnd" << endl;
					creatingNewNeuronSequence1 = false;
					neuronSequenceIndex1 = 0;
				}
				#endif

				createOrAppendFirstLevelHiddenLayerGroup(SANIGroupTypes, forwardPropogationSequenceData, currentLayerNeuronGroupStart, &creatingNewNeuronSequence1, &neuronSequenceIndex1, &newNeuronSequenceGroup1, listOfHighLevelNeuronsCurrent);
				
				indexInSequence++;
			}
			else
			{
				creatingNewNeuronSequence1 = false;
				neuronSequenceIndex1 = 0;
				foundAndReconciledMissingOrDifferentIncrementalNeurons = true;

				#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_MARK_AS_UNAMBIGUOUS
				SANIgenerateCompactOperations.markAmbiguousFirstHiddenLayerNeuronsAsUnambiguous(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage, true);
				#endif
						
				#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
				if(!(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoveragePartial || forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableEndComponent))
				#else
				if(!(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoveragePartial))
				#endif
				{
				#endif
					//case c
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
					cout << "B addNeuronToList, indexInSequence = " << indexInSequence  << ", nextIndexInSequence = " << SANIgenerateCompactOperations.calculateNextIndexInSequence(forwardPropogationSequenceData) << ", groupIndexes = " << SANInodes.printParseTreeGroupIndicesFlat(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage) << endl;
					#endif
					
					//fully activated group coverage+weight is > partially activated group coverage+weight
					
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
					addVariableFirstComponent(SANIGroupTypes, forwardPropogationSequenceData, listOfHighLevelNeuronsCurrent, forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage->groupRef);
					#endif
	
					addNeuronToList(SANIGroupTypes, forwardPropogationSequenceData, listOfHighLevelNeuronsCurrent, forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage->groupRef, &indexInSequence, &highLevelNeuronExpectingVariableLastComponent);
				
				#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
				}
				else
				{
					//case b
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
					cout << "C splitGroupAtLastActivatedComponent + addNeuronToList, forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage->groupRef->groupIndex = " << forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage->groupRef->groupIndex << endl;
					#endif
					//partially activated group coverage+weight is > fully activated group coverage+weight
					
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
					bool foundLocalVariationCandidate = false;
					if(findingCandidateComponent1)
					{
						SANIComponentNeuralNetwork* candidateComponent2 = NULL; 
						candidateNeuronBaseParseTree = forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage;
						if(verifyAndMarkNeuronAsCandidateWrapper(forwardPropogationSequenceData, candidateNeuronBaseParseTree, &candidateComponent2))
						{
							foundLocalVariationCandidate = true;
							findingCandidateComponent1 = false;
							findingCandidateComponent2 = true;
							forwardPropogationSequenceData->findingCandidateComponent2 = true;
							forwardPropogationSequenceData->candidateComponent2ToFind = candidateComponent2;
						}
					}
					if(!foundLocalVariationCandidate)
					{
					#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
						if(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableEndComponent)
						{
							prepareVariableLastComponent(SANIGroupTypes, forwardPropogationSequenceData, listOfHighLevelNeuronsCurrent, forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage, &highLevelNeuronExpectingVariableLastComponent);
						}
						else
						{
						#endif
							#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
							addVariableFirstComponent(SANIGroupTypes, forwardPropogationSequenceData, listOfHighLevelNeuronsCurrent, forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage->groupRef);
							#endif
	
							SANIGroupNeuralNetwork* newNeuron = splitNeuron(SANIGroupTypes, forwardPropogationSequenceData, forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage);
							
							addNeuronToList(SANIGroupTypes, forwardPropogationSequenceData, listOfHighLevelNeuronsCurrent, newNeuron, &indexInSequence, &highLevelNeuronExpectingVariableLastComponent);
							
						#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
						}
						#endif
	
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
					}
					#endif
				}
				#endif
			}			

			#ifdef SANI_FREE_MEMORY
			SANIpropagateOperationsParseTree.deinitialiseParseTreeGroupList(SANIGroupTypes, &(forwardPropogationSequenceData->parseTreeGroupListPropagate));
			#endif
			
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
			if(findingCentralVariation)
			{
				if(currentFirstInputNeuronIndexInSequence == activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed)
				{
					SANIGroupNeuralNetwork* grammaticalSentenceNeuron = NULL;
					if(connectListOfHighLevelNeuronsToNewNeuron(SANIGroupTypes, forwardPropogationSequenceData, listOfHighLevelNeuronsCurrent, &grammaticalSentenceNeuron, false))
					{
						listOfHighLevelNeurons1.push_back(grammaticalSentenceNeuron);
					}
					findingCentralVariation = false;
					findingCandidateComponent1 = true;
					listOfHighLevelNeuronsCurrent = &listOfHighLevelNeurons1;
					activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed = INT_DEFAULT_VALUE;
					activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed = INT_DEFAULT_VALUE;
					//candidateNeuronBaseParseTree = NULL;	//already done
				}
			}
			#endif

		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
		}
		#endif
		
		if(indexInSequence < forwardPropogationSequenceData->sentenceContents->size())
		{
			stillIdentifyingHighLevelNeurons = true;	
			//see how many sequenceElements successfully propagated through the next time
		}
		else
		{
			stillIdentifyingHighLevelNeurons = false;
		}	
		
		loopIndex++;
	}
	
	if(loopIndex == 1)
	{
		cerr << "SANIgenerateCompactContinuousClass::findAndReconcileIncrementalVariation error: matched whole subset in network, prior test should have passed" << endl;
		exit(EXIT_ERROR);
	}
		
	SANIGroupNeuralNetwork* grammaticalSentenceNeuron = NULL;
	connectListOfHighLevelNeuronsToNewNeuron(SANIGroupTypes, forwardPropogationSequenceData, &listOfHighLevelNeurons1, &grammaticalSentenceNeuron, true);
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	SANIgenerateCompactIdentify.markSubNeuronsReset(grammaticalSentenceNeuron);	
	#endif

	forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableStartComponent = false;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
	forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableEndComponent = false;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT
	forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageRequirePosAmbiguousContext = false;
	#endif
					
	return foundAndReconciledMissingOrDifferentIncrementalNeurons;
}


		

bool SANIgenerateCompactContinuousClass::addNeuronToList(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron, int* indexInSequence, SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent)
{	
	bool result = true;
	
	#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
	if(*highLevelNeuronExpectingVariableLastComponent != NULL)
	{
		addVariableLastComponent(SANIGroupTypes, forwardPropogationSequenceData, listOfHighLevelNeurons, neuron, highLevelNeuronExpectingVariableLastComponent);
		neuron = *highLevelNeuronExpectingVariableLastComponent;
		*highLevelNeuronExpectingVariableLastComponent = NULL;
	}
	#endif
	#endif
	
	*indexInSequence = SANIgenerateCompactOperations.calculateNextIndexInSequence(forwardPropogationSequenceData);
		
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
	SANIgenerateCompactOperations.addParentNeuronsToList(&(forwardPropogationSequenceData->listOfHighLevelNeuronsCompleteHistory), neuron);
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	SANIgenerateCompactOperations.addParentNeuronsToListReset(neuron);
	#endif
	#endif
	#endif
	
	/*//this code has been moved outside of SANIgenerateCompactContinuousClass::addNeuronToList @SANI1m6a
	#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
	addVariableFirstComponent(SANIGroupTypes, forwardPropogationSequenceData, listOfHighLevelNeurons, neuron);
	#endif
	*/
	
	#endif

	listOfHighLevelNeurons->push_back(neuron);	
	
	return result;
}

#ifndef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
SANIGroupNeuralNetwork* SANIgenerateCompactContinuousClass::splitNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* partiallyActivatedNeuronWithMaxSequenceIndexCoverage)
{	
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	
	int indexToSplitVector1 = INT_DEFAULT_VALUE;
	int indexToSplitVector2 = INT_DEFAULT_VALUE;			
	SANIpropagateCompactGenerateOperations.identifyComponentIndexFirstAndLastActivatedIndexUnordered(forwardPropogationSequenceData, partiallyActivatedNeuronWithMaxSequenceIndexCoverage, &indexToSplitVector1, &indexToSplitVector2);
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	if(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponent)
	{
		//ensure not to split off the first variable component (keep it in the group)
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			indexToSplitVector2 = partiallyActivatedNeuronWithMaxSequenceIndexCoverage->groupRef->components.size()-1;
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
	SANIGroupNeuralNetwork* newNeuron = splitGroupAtLastActivatedComponentUnordered(forwardPropogationSequenceData, SANIGroupTypes, partiallyActivatedNeuronWithMaxSequenceIndexCoverage->groupRef, indexToSplitVector1, indexToSplitVector2);	//note passing partiallyActivatedNeuronWithMaxSequenceIndexCoverage instead of partiallyActivatedNeuronWithMaxSequenceIndexCoverage->groupRef because groupRef's activations may have been overwritten if the neuron was reset after being saved to partiallyActivatedNeuronWithMaxSequenceIndexCoverage
	
	#else
	
	int indexToSplitVector = INT_DEFAULT_VALUE;
	SANIpropagateCompact.identifyComponentIndexLastActivatedIndex(forwardPropogationSequenceData, partiallyActivatedNeuronWithMaxSequenceIndexCoverage, &indexToSplitVector);
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "indexToSplitVector = " << indexToSplitVector << endl;				
	#endif
	SANIGroupNeuralNetwork* newNeuron = splitGroupAtLastActivatedComponent(forwardPropogationSequenceData, SANIGroupTypes, partiallyActivatedNeuronWithMaxSequenceIndexCoverage->groupRef, indexToSplitVector);	//note passing partiallyActivatedNeuronWithMaxSequenceIndexCoverage instead of partiallyActivatedNeuronWithMaxSequenceIndexCoverage->groupRef because groupRef's activations may have been overwritten if the neuron was reset after being saved to partiallyActivatedNeuronWithMaxSequenceIndexCoverage
	#endif

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "partiallyActivatedNeuronWithMaxSequenceIndexCoverage->parseTreeMaxSequenceIndex = " << partiallyActivatedNeuronWithMaxSequenceIndexCoverage->parseTreeMaxSequenceIndex << endl;
	cout << "partiallyActivatedNeuronWithMaxSequenceIndexCoverage->parseTreeMinSequenceIndex = " << partiallyActivatedNeuronWithMaxSequenceIndexCoverage->parseTreeMinSequenceIndex << endl;
	//cout << "indexInSequence = " << indexInSequence << endl;
	
	#endif
	
	return newNeuron;
}
#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
bool SANIgenerateCompactContinuousClass::addVariableFirstComponent(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork* neuron)
{	
	bool result = true;
	
	if(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponent)
	{		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		cout << "(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponent)" << endl;
		#endif
		
		int variableComponentInGroup = 0;
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			variableComponentInGroup = neuron->components.size() - 1;
		}
		else
		{
			variableComponentInGroup = 0;
		}
		SANIComponentNeuralNetwork* variableComponent = neuron->components[variableComponentInGroup];
		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		for(int l=0; l<variableComponent->SANIbackGroupConnectionList.size(); l++)
		{
			SANIGroupNeuralNetwork* groupSource = variableComponent->SANIbackGroupConnectionList[l];
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
		if(connectListOfHighLevelNeuronsToNewNeuron(SANIGroupTypes, forwardPropogationSequenceData, listOfHighLevelNeurons, &grammaticalSentenceNeuron, false))
		{
			listOfHighLevelNeurons->clear();
			SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSequenceData, grammaticalSentenceNeuron, variableComponent);
		}
	}	
	
	return result;
}
#endif
#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
bool SANIgenerateCompactContinuousClass::prepareVariableLastComponent(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, constEffective SANIGroupParseTree* partiallyActivatedNeuronWithMaxSequenceIndexCoverage, constEffective SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent)
{	
	bool result = true;

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
	cout << "prepareVariableLastComponent:" << endl;
	#endif

	SANIGroupNeuralNetwork* grammaticalSentenceNeuron = NULL;
	if(connectListOfHighLevelNeuronsToNewNeuron(SANIGroupTypes, forwardPropogationSequenceData, listOfHighLevelNeurons, &grammaticalSentenceNeuron, false))
	{
		listOfHighLevelNeurons->clear();
	}	
	
	*highLevelNeuronExpectingVariableLastComponent = partiallyActivatedNeuronWithMaxSequenceIndexCoverage->groupRef;
	
	return result;	
}
bool SANIgenerateCompactContinuousClass::addVariableLastComponent(const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, const SANIGroupNeuralNetwork* neuron, SANIGroupNeuralNetwork** highLevelNeuronExpectingVariableLastComponent)
{	
	bool result = true;

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
	cout << "(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableEndComponent)" << endl;
	#endif

	int variableComponentInGroup;
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		variableComponentInGroup = 0;
	}
	else
	{
		variableComponentInGroup = (*highLevelNeuronExpectingVariableLastComponent)->components.size() - 1;
	}
	SANIComponentNeuralNetwork* variableComponent = (*highLevelNeuronExpectingVariableLastComponent)->components[variableComponentInGroup];

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
	for(int l=0; l<variableComponent->SANIbackGroupConnectionList.size(); l++)
	{
		SANIGroupNeuralNetwork* groupSource = variableComponent->SANIbackGroupConnectionList[l];
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

	SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSequenceData, (*highLevelNeuronExpectingVariableLastComponent), variableComponent);
		
	return result;	
}
#endif
#endif




bool SANIgenerateCompactContinuousClass::connectListOfHighLevelNeuronsToNewNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons, SANIGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
	cout << "connectListOfHighLevelNeuronsToNewNeuron = ";
	//cout << "listOfHighLevelNeurons->size() = " << listOfHighLevelNeurons->size() << endl;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_TOP_LEVEL_NEURON_DUPLICATION
	if(listOfHighLevelNeurons->size() > 1)
	{
	#endif
		result = true;
		
		SANIGroupNeuralNetwork* grammaticalSentenceNeuronSub = SANIgenerateCompactOperations.createNewHiddenLayerGroup(forwardPropogationSequenceData, SANIGroupTypes);
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
		cout << grammaticalSentenceNeuronSub->groupIndex << " ";
		#endif
		#ifdef SANI_DEBUG_FORMATION
		cout << "\t\t SANIgenerateCompactContinuousClass::connectListOfHighLevelNeuronsToNewNeuron: grammaticalSentenceNeuronSub->groupIndex = " << grammaticalSentenceNeuronSub->groupIndex << endl;
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
		//SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_ELONGATED_TREE
		for(int k=0; k<listOfHighLevelNeurons->size(); k++)
		{
			SANIGroupNeuralNetwork* currentHighLevelNeuron = (*listOfHighLevelNeurons)[k];
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
			cout << "currentHighLevelNeuron->groupIndex = " << currentHighLevelNeuron->groupIndex << ", k = " << k << endl;
			#endif
				
			SANIGroupNeuralNetwork* grammaticalSentenceNeuronSubHigher = NULL;
	
			bool componentTypeString = SANInodes.calculateComponentTypeString(currentHighLevelNeuron);
			SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSequenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSub, componentTypeString, false);
			
			//add new higher level group in elongated tree
			if(k>0 && k<listOfHighLevelNeurons->size()-1)
			{//there is still another neuron in listOfHighLevelNeurons to be added to the tree
				
				grammaticalSentenceNeuronSubHigher = SANIgenerateCompactOperations.createNewHiddenLayerGroup(forwardPropogationSequenceData, SANIGroupTypes);
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
				cout << grammaticalSentenceNeuronSubHigher->groupIndex << " ";
				#endif
				SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSequenceData, grammaticalSentenceNeuronSub, grammaticalSentenceNeuronSubHigher, false, false);

				
				#ifdef SANI_DEBUG_FORMATION
				cout << "\t\t SANIgenerateCompactContinuousClass::connectListOfHighLevelNeuronsToNewNeuron: grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
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
			if(!directWireLowLevelPOSneuronToGrammaticalSentenceNeuron(SANIGroupTypes, forwardPropogationSequenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSub))
			{
			#endif
				SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSequenceData, currentHighLevelNeuron, grammaticalSentenceNeuronSub, false, false);
			#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
			}
			#endif
		}
		#endif
		
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
		cout << "SANIgenerateCompactContinuousClass::connectListOfHighLevelNeuronsToNewNeuron - assigning top level sentence neuron;" << endl;
		#endif
		(*grammaticalSentenceNeuron) = grammaticalSentenceNeuronSub;
		#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_TOP_LEVEL_NEURONS
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
bool SANIgenerateCompactContinuousClass::verifyAndMarkNeuronAsCandidateWrapper(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* candidateNeuronBaseParseTree, SANIComponentNeuralNetwork** candidateComponent2)
{
	bool result = false;
	
	SANIGroupNeuralNetwork* candidateNeuronBase = candidateNeuronBaseParseTree->groupRef;
		
	int lastActivatedIndex = INT_DEFAULT_VALUE;
	int firstUnactivatedIndex = INT_DEFAULT_VALUE;	
	SANIpropagateCompactGenerateOperations.identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(forwardPropogationSequenceData, candidateNeuronBaseParseTree, &lastActivatedIndex, &firstUnactivatedIndex);
		
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	bool missingStartComponentFound = false;
	int indexOfFirstComponent;
	if(forwardPropogationSequenceData->parseSentenceReverse)
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
		if(forwardPropogationSequenceData->parseSentenceReverse)
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
			if(verifyAndMarkNeuronAsCandidate(forwardPropogationSequenceData, candidateNeuronBase, candidateComponent2))
			{
				result = true;
			}
		}
		
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	}
	#endif

	return result;
}
	
bool SANIgenerateCompactContinuousClass::verifyAndMarkNeuronAsCandidate(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIGroupNeuralNetwork* currentNeuron, SANIComponentNeuralNetwork** candidateComponent2)
{
	bool result = false;
	
	for(int i=0; i<currentNeuron->SANIfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (currentNeuron->SANIfrontComponentConnectionList)[i];
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
			if(forwardPropogationSequenceData->parseSentenceReverse)
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
			if(verifyAndMarkNeuronAsCandidate(forwardPropogationSequenceData, higherLevelGroup, candidateComponent2))
			{
				result = true;
			}
		}
	}
	
	return result;
}
#endif
	

bool SANIgenerateCompactContinuousClass::createOrAppendFirstLevelHiddenLayerGroup(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* currentLayerNeuronGroupStart, bool* creatingNewNeuronSequence, int* neuronSequenceIndex, SANIGroupNeuralNetwork** newNeuronSequenceGroup, vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeurons)
{
	bool result = true;
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIgenerateCompactContinuousClass::createOrAppendFirstLevelHiddenLayerGroup" << endl;
	#endif

	//currentLayerNeuronGroupStart->inputLayerNeuron = true;	//moved to SANIformation: @SANI1p6a

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
		SANIgenerateCompactOperations.addComponentToFirstLevelHiddenLayerGroup(forwardPropogationSequenceData, currentLayerNeuronGroupStart, *newNeuronSequenceGroup);
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		cout << "creatingNewNeuronSequence: newNeuronSequenceGroup->groupIndex = " << (*newNeuronSequenceGroup)->groupIndex << endl;
		#endif
	}
	else
	{
		*neuronSequenceIndex = 0;
		*creatingNewNeuronSequence = true;
		SANIGroupNeuralNetwork* newNeuron = SANIgenerateCompactOperations.createNewHiddenLayerGroup(forwardPropogationSequenceData, SANIGroupTypes);
		#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
		newNeuron->firstHiddenLayerNeuron = true;
		#endif
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
		cout << "!creatingNewNeuronSequence: newNeuron->groupIndex = " << newNeuron->groupIndex << endl;
		#endif
		SANIgenerateCompactOperations.addComponentToFirstLevelHiddenLayerGroup(forwardPropogationSequenceData, currentLayerNeuronGroupStart, newNeuron);
		listOfHighLevelNeurons->push_back(newNeuron);
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		//all sublayer neurons are POS inputs/strings and will not be added to listOfHighLevelNeuronsCompleteHistory
		#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
		#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
		newNeuron->marked = true;
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
bool SANIgenerateCompactContinuousClass::directWireLowLevelPOSneuronToGrammaticalSentenceNeuron(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* singleComponentNeuronCandidate, SANIGroupNeuralNetwork* grammaticalSentenceNeuron)
{
	bool directWire = false;
	if(singleComponentNeuronCandidate->firstHiddenLayerNeuron)
	{		
		if(singleComponentNeuronCandidate->components.size() == 1)
		{
			SANIGroupNeuralNetwork* singleComponentNeuron = singleComponentNeuronCandidate;

			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_BASIC
			cout << "SANIgenerateCompactContinuousClass::directWireLowLevelPOSneuronToGrammaticalSentenceNeuron: singleComponentNeuron->groupIndex = " << singleComponentNeuron->groupIndex << endl;
			#endif
			
			//direct wire low level POS neuron to grammaticalSentenceNeuron (dont ever create intermediary single component neuron)
			directWire = true;

			SANIComponentNeuralNetwork* component = singleComponentNeuron->components[0];

			//doesnt seem to ever be required;
			for(int i=0; i<singleComponentNeuron->SANIfrontComponentConnectionList.size(); i++)
			{
				SANIComponentNeuralNetwork* currentComponent = (singleComponentNeuron->SANIfrontComponentConnectionList)[i];
				SANIGroupNeuralNetwork* higherLevelGroup = currentComponent->ownerGroup;
				
				for(int j=0; j<currentComponent->SANIbackGroupConnectionList.size(); j++)
				{
					SANIGroupNeuralNetwork* firstHiddenLayerGroup = (currentComponent->SANIbackGroupConnectionList)[j];
					if(firstHiddenLayerGroup == singleComponentNeuron)
					{
						currentComponent->SANIbackGroupConnectionList.erase(currentComponent->SANIbackGroupConnectionList.begin()+j);
						j--;
					}
				}
			}

			for(int i=0; i<component->SANIbackGroupConnectionList.size(); i++)
			{
				SANIGroupNeuralNetwork* lowerLevelPOSgroup = (component->SANIbackGroupConnectionList)[i];
				
				for(int j=0; j<lowerLevelPOSgroup->SANIfrontComponentConnectionList.size(); j++)
				{
					SANIComponentNeuralNetwork* currentComponent = (lowerLevelPOSgroup->SANIfrontComponentConnectionList)[j];
					if(currentComponent == component)
					{
						lowerLevelPOSgroup->SANIfrontComponentConnectionList.erase(lowerLevelPOSgroup->SANIfrontComponentConnectionList.begin()+j);
						j--;
					}
				}

				SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSequenceData, lowerLevelPOSgroup, grammaticalSentenceNeuron, true, false);	//fixed GIA3j5aTEMP61 
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

			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_CLEAN_PARSE_TREE_MEMORY
			//required when SANIpropagateCompactGenerateClass::connectListOfHighLevelNeuronsToNewNeuron:directWireLowLevelPOSneuronToGrammaticalSentenceNeuron is executed before deinitialiseParseTreeGroupList
			for(int i2=0; i2<forwardPropogationSequenceData->parseTreeGroupListPropagate.size(); i2++)
			{
				SANIGroupParseTree* parseTreeGroupTemp = (forwardPropogationSequenceData->parseTreeGroupListPropagate)[i2];
				if(parseTreeGroupTemp == singleComponentNeuron->currentParseTreeGroupTemp)
				{
					//cout << "(parseTreeGroupTemp == singleComponentNeuron->currentParseTreeGroupTemp)" << endl;
					SANInodes.deleteGroup(parseTreeGroupTemp);
					forwardPropogationSequenceData->parseTreeGroupListPropagate.erase(forwardPropogationSequenceData->parseTreeGroupListPropagate.begin()+i2);
					i2--;
				}
			}
			#endif
			
			SANInodes.deleteGroup(singleComponentNeuron);	
		}
	}
	if(!directWire)
	{
		singleComponentNeuronCandidate->firstHiddenLayerNeuron = false;	//NOT REQUIRED? prevents parsing of future sentence from matching this neuron as activatedNeuronWithMaxSequenceIndexCoverage, and then treating it like firstHiddenLayerNeuron above
	}
	
	return directWire;
}
#endif



//NB indexToSplitVector = lastActivatedComponent (the last component index in the first part of component to be splitted) (do not assume normal order: "first" and "last" definitions respect (forwardPropogationSequenceData->parseSentenceReverse))
SANIGroupNeuralNetwork* SANIgenerateCompactContinuousClass::splitGroupAtLastActivatedComponent(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupNeuralNetwork* neuronToSplit, const int indexToSplitVector)
{
	SANIGroupNeuralNetwork* newHiddenLayerNeuron = SANIgenerateCompactOperations.createNewHiddenLayerGroup(forwardPropogationSequenceData, SANIGroupTypes);	
		
	vector<SANIComponentNeuralNetwork*>* components = &(neuronToSplit->components);
	vector<SANIComponentNeuralNetwork*> componentsPart1(components->begin(), components->begin() + indexToSplitVector+1);
        vector<SANIComponentNeuralNetwork*> componentsPart2(components->begin() + indexToSplitVector+1, components->end());
	
	if(forwardPropogationSequenceData->parseSentenceReverse)
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
	
	SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSequenceData, newHiddenLayerNeuron, neuronToSplit, false, true);	//CHECKTHIS	
		
	return newHiddenLayerNeuron;
}

#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
//indexToSplitVector1 = firstActivatedComponent (unordered; always assume normal order: "first" and "last" definitions do not respect (forwardPropogationSequenceData->parseSentenceReverse))
//indexToSplitVector2 = lastActivatedComponent (unordered; always assume normal order: "first" and "last" definitions do not respect (forwardPropogationSequenceData->parseSentenceReverse))
SANIGroupNeuralNetwork* SANIgenerateCompactContinuousClass::splitGroupAtLastActivatedComponentUnordered(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupNeuralNetwork* neuronToSplit, const int indexToSplitVector1, int indexToSplitVector2)
{
	SANIGroupNeuralNetwork* newHiddenLayerNeuron = SANIgenerateCompactOperations.createNewHiddenLayerGroup(forwardPropogationSequenceData, SANIGroupTypes);
				
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
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			insertAtStart = false;	//compensate for SANIgenerateCompactOperations.addComponentToGroup specification limitation
		}
		SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSequenceData, newHiddenLayerNeuron, neuronToSplit, false, insertAtStart);	//CHECKTHIS
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
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			insertAtStart = true;	//compensate for SANIgenerateCompactOperations.addComponentToGroup specification limitation
		}
		SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSequenceData, newHiddenLayerNeuron, neuronToSplit, false, insertAtStart);	//CHECKTHIS
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
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			insertAtStart = true;	//compensate for SANIgenerateCompactOperations.addComponentToGroup specification limitation
		}
		SANIgenerateCompactOperations.addComponentToGroup(forwardPropogationSequenceData, newHiddenLayerNeuron, neuronToSplit, false, insertAtStart);	//CHECKTHIS
		
		neuronToSplit->components.insert(neuronToSplit->components.end(), componentsPart3.begin(), componentsPart3.end());
		SANInodes.updateComponentsOwnerGroupAndIndexes(neuronToSplit, &(neuronToSplit->components), false, NULL);
	}
	
	return newHiddenLayerNeuron;
}
#endif


#ifdef SANI_SEQUENCE_GRAMMAR_DISALLOW_DIRECT_REFSET_AND_DELIMITER_POS_COMPONENTS_ON_SAME_NEURON
bool SANIgenerateCompactContinuousClass::determineListHighLevelNeuronsDirectInput(SANIGroupNeuralNetwork* newNeuronSequenceGroup1, bool* highLevelListHasDirectInputRefSetPOS, bool* highLevelListHasDirectInputRefSetDelimiterPOS)
{
	bool result = true;
		
	if(newNeuronSequenceGroup1 != NULL)
	{
		//cout << "SANIgenerateCompactContinuousClass::determineListHighLevelNeuronsDirectInput" << endl;

		for(int i=0; i<newNeuronSequenceGroup1->components.size(); i++)
		{
			SANIComponentNeuralNetwork* component = newNeuronSequenceGroup1->components.at(i);
			SANIGroupNeuralNetwork* inputLayerNeuron = (component->SANIbackGroupConnectionList)[0];	//newNeuronSequenceGroup1 doesn't contain variable components (only input neurons)
			//cout << "inputLayerNeuron->groupIndex = " << inputLayerNeuron->groupIndex << endl;

			if(inputLayerNeuron->inputLayerNeuron)
			{
				//cout << "inputLayerNeuron->inputLayerNeuron" << endl;

				bool refsetDelimiterPOSfound = sequenceElementHasRefsetDelimiterPOS(inputLayerNeuron);
				if(refsetDelimiterPOSfound)
				{
					*highLevelListHasDirectInputRefSetDelimiterPOS = true;
					//cout << "highLevelListHasDirectInputRefSetDelimiterPOS" << endl;
				}
				else
				{
					*highLevelListHasDirectInputRefSetPOS = true;
					//cout << "highLevelListHasDirectInputRefSetPOS" << endl;
				}
			}
		}
	}
	
	return result;
}

bool SANIgenerateCompactContinuousClass::determineListHighLevelNeuronsDirectInput(vector<SANIGroupNeuralNetwork*>* listOfInputNeurons, bool* highLevelListHasDirectInputRefSetPOS, bool* highLevelListHasDirectInputRefSetDelimiterPOS)
{
	bool result = true;
		
	for(int i=0; i<listOfInputNeurons->size(); i++)
	{
		SANIGroupNeuralNetwork* inputLayerNeuron = listOfInputNeurons->at(i);
		if(inputLayerNeuron->inputLayerNeuron)
		{			
			bool refsetDelimiterPOSfound = sequenceElementHasRefsetDelimiterPOS(inputLayerNeuron);
			if(refsetDelimiterPOSfound)
			{
				*highLevelListHasDirectInputRefSetDelimiterPOS = true;
			}
			else
			{
				*highLevelListHasDirectInputRefSetPOS = true;
			}
		}
	}
	
	return result;
}		
		
bool SANIgenerateCompactContinuousClass::sequenceElementHasRefsetDelimiterPOS(SANIGroupNeuralNetwork* inputNeuron)
{
	bool refsetDelimiterPOSfound = false;
	
	for(int q=0; q<SANI_SEQUENCE_GRAMMAR_REFSET_DELIMITER_POS_NUMBER_OF_TYPES; q++)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
		if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(inputNeuron->sequenceElementObject->POSambiguityInfo, refsetDelimiterPOSarray[q]))
		#else
		if(inputNeuron->wordPOStype == refsetDelimiterPOSarray[q])	//highLevelListHasDirectInputDelimiterPOS = SHAREDvars.intInIntArray(inputNeuron->wordPOStype, refsetDelimiterPOSarray, SANI_SEQUENCE_GRAMMAR_REFSET_DELIMITER_POS_NUMBER_OF_TYPES))
		#endif
		{
			refsetDelimiterPOSfound = true;
		}
	}
	
	return refsetDelimiterPOSfound;
}
#endif

		
#endif
