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
 * File Name: SANIpropagateOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1m4a 01-September-2020
 * Requirements: 
 * Description: SANI (Sequentially Activated Neuronal Input neural network) Operations - generic functions
 * /
 *******************************************************************************/


#include "SANIpropagateOperations.hpp"


#ifdef SANI

static bool parseSentenceReverse;

#ifdef SANI_DEBUG_PROPAGATE_EXTRA8
static bool firstExecution;
#endif

void SANIpropagateOperationsClass::setParseSentenceReverseStrict(const bool parseSentenceReverseNew, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	parseSentenceReverse = parseSentenceReverseNew;
	forwardPropogationSentenceData->parseSentenceReverse = parseSentenceReverseNew;
}


void SANIpropagateOperationsClass::setParseSentenceReverse(const bool normalDirection, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool parseSentenceReverseNew = false;
	if(normalDirection)
	{
		#ifdef SANI_REVERSE_DIRECTION
		parseSentenceReverseNew = true;
		#else
		parseSentenceReverseNew = false;
		#endif
	}
	else
	{
		#ifdef SANI_REVERSE_DIRECTION
		parseSentenceReverseNew = false;
		#else
		parseSentenceReverseNew = true;
		#endif		
	}
	parseSentenceReverse = parseSentenceReverseNew;
	forwardPropogationSentenceData->parseSentenceReverse = parseSentenceReverseNew;
}

bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(testComponent, components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, &existingActivationFoundNOTUSED);
}
bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(testComponent, components, &existingActivationFoundNOTUSED);
}
bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(testComponent, components, forwardPropogationWordData, repeatDetected, &existingActivationFoundNOTUSED);
}

bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent, bool* existingActivationFoundStartComponent)
{
	bool missingStartComponentsFoundNOTUSED = false;
	bool missingOrVariableStartComponentFoundNOTUSED = false;
	bool missingOrVariableEndComponentFoundNOTUSED = false;
	int numberOfInactiveComponentsRemaining = 0;
	SANIGroupParseTree* activationPathWordCurrentParseTreeGroupNOTUSED = NULL;
	SANIGroupNeuralNetwork* ownerGroupNOTUSED = NULL;
	bool componentWordConnectivityTestsResultNOTUSED = false;
        bool existingActivationFoundEndComponentNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(testComponent, components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordCurrentParseTreeGroupNOTUSED, activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, existingActivationFoundStartComponent, &existingActivationFoundEndComponentNOTUSED, &missingStartComponentsFoundNOTUSED, &missingOrVariableStartComponentFoundNOTUSED, &missingOrVariableEndComponentFoundNOTUSED, &numberOfInactiveComponentsRemaining, ownerGroupNOTUSED, componentWordConnectivityTestsResultNOTUSED);
}

bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, bool* missingStartComponentFound, bool* missingOrVariableStartComponentFound, bool* missingOrVariableEndComponentFound, int* numberOfInactiveComponentsRemaining, SANIGroupNeuralNetwork* ownerGroup, const bool componentWordConnectivityTestsResult)
{	
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	*numberOfInactiveComponentsRemaining = 0;
		
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY
	*previousActiveComponent = NULL;
	#endif
		
	for(int i=0; i<components->size(); i++)
	{		
		int c;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			c = components->size()-1-i;
		}
		else
		{
			c = i;
		}

		SANIComponentNeuralNetwork* currentComponent = (*components)[c];

		#ifdef SANI_ENFORCE_WORD_CONNECTIVITY
		if(currentComponent->neuronComponentConnectionActive)
		{
			*finalActiveComponent = currentComponent;
		}
		#endif
						
		if(stillParsingActiveComponents)
		{
			if(currentComponent == testComponent)
			{
				#ifdef SANI_ENFORCE_WORD_CONNECTIVITY
				if(*previousActiveComponent == NULL)
				{
					*firstActiveComponentInGroup = true;
				}
				#endif
				
				stillParsingActiveComponents = false;

				identifySequentialActivationFound(components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordCurrentParseTreeGroup, i, currentComponent, *previousActiveComponent, &sequentialActivationFound, existingActivationFoundStartComponent, existingActivationFoundEndComponent, ownerGroup, componentWordConnectivityTestsResult);
				
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
				identifyMissingOrVariableEndComponentFound(components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordCurrentParseTreeGroup, i, currentComponent, *previousActiveComponent, missingOrVariableEndComponentFound);
				#endif
			}
			else
			{
				#ifdef SANI_ENFORCE_WORD_CONNECTIVITY
				if(currentComponent->neuronComponentConnectionActive)
				{
					*previousActiveComponent = currentComponent;
				}
				#endif
				
				#ifdef SANI_SUPPORT_COMPONENTS_OPTIONAL
				if(currentComponent->optional)
				{
					//ignore optional components
				}
				else
				{			
				#endif
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
					identifyMissingOrVariableStartComponentFound(components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordCurrentParseTreeGroup, i, currentComponent, *previousActiveComponent, missingOrVariableStartComponentFound);
					#endif
					
					identifyMissingStartComponentFound(components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationPathWordCurrentParseTreeGroup, i, currentComponent, *previousActiveComponent, missingStartComponentFound, &stillParsingActiveComponents);
				#ifdef SANI_SUPPORT_COMPONENTS_OPTIONAL
				}
				#endif
			}
		}
		else
		{
			if(sequentialActivationFound)
			{
				#ifdef SANI_SUPPORT_COMPONENTS_OPTIONAL
				if(!(currentComponent->optional))
				{
				#endif
					if(!(currentComponent->neuronComponentConnectionActive))
					{
						*numberOfInactiveComponentsRemaining = *numberOfInactiveComponentsRemaining + 1;	
					}
					else
					{
						#ifdef SANI_SEQUENCE_GRAMMAR_PERFORM_AUTORESET_OF_ACTIVATIONS
						bool foundFollowingActiveComponent = true;
						//perform autoreset of activations
						if(*existingActivationFoundStartComponent)
						{
							*numberOfInactiveComponentsRemaining = *numberOfInactiveComponentsRemaining + 1;
						}
						else
						{
							//group not suitable for activation, following component is active
							sequentialActivationFound = false;
						}
						#else
						//group not suitable for activation, following component is active
						sequentialActivationFound = false;
						#endif
					}
				#ifdef SANI_SUPPORT_COMPONENTS_OPTIONAL
				}
				#endif
			}
		}
	}
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFIED
	if((*missingStartComponentFound) && !(*missingOrVariableStartComponentFound))
	{
		sequentialActivationFound = false;
	}
	#endif
	
	if(sequentialActivationFound)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		bool passMissingStartComponentTests = true;
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent)
		{
			passMissingStartComponentTests = false;
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFIED
			if(true)
			#else
			if(!((*missingStartComponentFound) && !(*missingOrVariableStartComponentFound)))
			#endif
			{
				passMissingStartComponentTests = true;
			}
		}
		if(passMissingStartComponentTests)
		{
		#else
		#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		if(!(*missingStartComponentFound))
		{
		#endif	
		#endif
			if(*numberOfInactiveComponentsRemaining == 0)
			{
				*activationSequenceCompleted = true;
			}
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS	
		}
		#else
		#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		}
		#endif	
		#endif
	}

	return sequentialActivationFound;
}



bool SANIpropagateOperationsClass::identifySequentialActivationFound(vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, SANIGroupNeuralNetwork* ownerGroup, const bool componentWordConnectivityTestsResult)
{
	bool result = true;
	
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	bool previousComponentWasActivatedBySameWordDifferentPOSpropagation = false;
	if(previousActiveComponent != NULL)
	{
		if((previousActiveComponent)->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference)
		{
			previousComponentWasActivatedBySameWordDifferentPOSpropagation = true;
		}
	}
	if(!previousComponentWasActivatedBySameWordDifferentPOSpropagation)
	{
	#endif		
	#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
	bool upperNeuronContainsWordIndexOfProspectiveComponentTest = false;
	if(!(currentComponent->ownerGroup->neuronActive))
	{
		if(upperNeuronContainsWordIndexOfProspectiveComponent(forwardPropogationSentenceData, forwardPropogationWordData->wordRecord, currentComponent, currentComponent->ownerGroup))
		{
			upperNeuronContainsWordIndexOfProspectiveComponentTest = true;
		}
	}
	if(!upperNeuronContainsWordIndexOfProspectiveComponentTest)
	{
	#endif

		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_ACTIVATION_IF_REPEATED_SEQUENCE_MISMATCH_DETECTED
		bool allowFirstComponentActivation = true;
		if(i == 0)	//start component in group
		{
			int wordTranslatorSentenceWordIndex = forwardPropogationWordData->w;
			//if repeated POS instance detected in sentence starting from forwardPropogationWordData, and first section [lowest level input] of second component is not equivalent to same input group as first component, then disallow activation[/reactivation] of first component
			if(repeatedSequenceDetected(forwardPropogationSentenceData, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup, wordTranslatorSentenceWordIndex))
			{
				if(!findValidDualLowerLevelConnectionAlternate(forwardPropogationSentenceData, forwardPropogationWordData, components, currentComponent))
				{
					allowFirstComponentActivation = false;
					//cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_ACTIVATION_IF_REPEATED_SEQUENCE_MISMATCH_DETECTED: repeatedSequenceDetected && !findValidDualLowerLevelConnectionAlternate: !allowComponentActivation" << endl;
				}
			}
			//if repeated POS instance not detected in sentence starting from forwardPropogationWordData, and first section [lowest level input] of second component is equivalent to same input group as first component, then disallow activation[/reactivation] of first component 
			if(!repeatedSequenceDetected(forwardPropogationSentenceData, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup, wordTranslatorSentenceWordIndex))
			{
				if(findValidDualLowerLevelConnectionAlternate(forwardPropogationSentenceData, forwardPropogationWordData, components, currentComponent))
				{
					allowFirstComponentActivation = false;
					//cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_ACTIVATION_IF_REPEATED_SEQUENCE_MISMATCH_DETECTED: !repeatedSequenceDetected && findValidDualLowerLevelConnectionAlternate: !allowComponentActivation" << endl;
				}
			}
		}
		if(allowFirstComponentActivation)
		{
		#endif			
			if(currentComponent->neuronComponentConnectionActive)
			{	
				#ifdef SANI_SUPPORT_COMPONENTS_REPEAT						
				if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)	//if the component contains repeats, then accept it if it is already active
				{
					//sequential activation found
					*sequentialActivationFound = true;
				}
				else
				{
				#endif

					//component already activated
					#ifdef SANI_SEQUENCE_GRAMMAR
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS_REMEMBER_FIRST_COMPONENT_WORD_INDEX
					if(i != 0)
					{
						if(previousActiveComponent != NULL)	//alternate test does not support !SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS: if(!(*missingStartComponentFound))
						{
							*sequentialActivationFound = true;
							*existingActivationFoundEndComponent = true;
							//cout << "existingActivationFoundEndComponent" << endl;

							if(ownerGroup->currentParseTreeGroupTemp == NULL)
							{
								cerr << "SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady error: (ownerGroup->currentParseTreeGroupTemp == NULL)" << endl;
								exit(EXIT_ERROR);
							}
							else
							{
								if(ownerGroup->currentParseTreeGroupTemp->components.size() == 0)
								{
									cerr << "SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady error: (ownerGroup->currentParseTreeGroupTemp->components.size() == 0)" << endl;
									exit(EXIT_ERROR);
								}
							}
						}
					}
					#endif
					if(i == 0)	//start component in group
					{
						bool allowFirstComponentReset = true;
						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED
						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED_ACTIVE
						//if first component input group matches second component input group, and repeated POS sequences detected in sentence from firstComponent matching first and second componets, then disallow reactivation/reset of first component 
						int wordTranslatorSentenceWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
						if(findValidDualLowerLevelConnection(forwardPropogationSentenceData, forwardPropogationWordData, components, currentComponent, true))
						{
							if(repeatedSequenceDetected(forwardPropogationSentenceData, forwardPropogationWordData, currentComponent->ownerGroup->currentParseTreeGroupTemp, wordTranslatorSentenceWordIndex))
							{
								#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED_AND_WORD_INDEX_NOT_ALREADY_ENCAPSULATED_BY_HIGHER_LEVEL_GROUP_PREVIOUS_COMPONENT
								if(!upperNeuronContainsWordIndexOfProspectiveComponent(forwardPropogationSentenceData, currentComponent->neuronComponentConnectionActiveWordRecord, currentComponent, currentComponent->ownerGroup))
								{
								#endif
									allowFirstComponentReset = false;
									//cout << "1allowFirstComponentReset = false;" << endl;
								#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED_AND_WORD_INDEX_NOT_ALREADY_ENCAPSULATED_BY_HIGHER_LEVEL_GROUP_PREVIOUS_COMPONENT
								}
								#endif
							}
						}
						#endif
						#else
						#ifdef SANI_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
						if(componentWordConnectivityTestsResult)
						{
							//cout << "componentWordConnectivityTestsResult" << endl;
						#endif
							#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_NEXT_SEQUENCE_IS_SAME_AS_CURRENT_SEQUENCE
							if(consecutiveSequenceDetected(forwardPropogationSentenceData, forwardPropogationWordData, components, currentComponent))
							{
								//cout << "!allowFirstComponentReset: consecutiveSequenceDetected - currentComponent->ownerGroup->groupIndex = " << currentComponent->ownerGroup->groupIndex << endl;
								//cout << "!allowFirstComponentReset; consecutiveSequenceDetected" << endl;
								allowFirstComponentReset = false;
								//cout << "2allowFirstComponentReset = false;" << endl;
							}
							#endif
							#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF
							if(findValidDualLowerLevelConnection(forwardPropogationSentenceData, forwardPropogationWordData, components, currentComponent, true))
							{
								//cout << "!allowFirstComponentReset; findValidDualLowerLevelConnection" << endl;
								allowFirstComponentReset = false;
								//cout << "3allowFirstComponentReset = false;" << endl;
							}
							#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_FORCE_RESET_IF_NO_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
						}
						else
						{
							//cout << "!componentWordConnectivityTestsResult" << endl;
							//always reset, as existing first activated component is not connected (by wordIndices) to prospective activated next component
						}
						#endif
						#endif
						/*//alternate method;
						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED_AND_WORD_INDEX_NOT_ALREADY_ENCAPSULATED_BY_HIGHER_LEVEL_GROUP_PREVIOUS_COMPONENT
						if(currentComponent->neuronComponentConnectionActive)
						{
							if(upperNeuronContainsWordIndexOfProspectiveComponent(forwardPropogationSentenceData, currentComponent->neuronComponentConnectionActiveWordRecord, currentComponent, currentComponent->ownerGroup))
							{
								allowFirstComponentReset = true;
							}
						}
						#endif
						*/						
						/*
						#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
						if(!(currentComponent->ownerGroup->neuronComponentConnectionActive))	//?
						{
							if(upperNeuronContainsWordIndexOfProspectiveComponent(forwardPropogationSentenceData, forwardPropogationWordData->wordRecord, currentComponent, currentComponent->ownerGroup))
							{
								allowFirstComponentReset = true;
							}
						}
						#endif
						*/
						#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
						if(existingActivationFoundStartComponent)
						{
							if(!(forwardPropogationSignalData->firstPOSval))
							{
								if(currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference)
								{
									//component has been activated by previous POS propagation
									allowFirstComponentReset = false;
									//cout << "4allowFirstComponentReset = false;" << endl;
								}
							}
						}
						#endif
						if(allowFirstComponentReset)
						{
							*sequentialActivationFound = true;
							*existingActivationFoundStartComponent = true;
						}
						else
						{
							*sequentialActivationFound = false;
						}
					}
					#else
					#ifdef SANI_ALLOW_MULTIPLE_ACTIVATIONS
					*sequentialActivationFound = true;
					*existingActivationFoundStartComponent = true;	
					#endif
					#endif
				#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
				}
				#endif
			}
			else
			{
				*sequentialActivationFound = true;
				//cout << "*sequentialActivationFound" << endl;
			}
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_ACTIVATION_IF_REPEATED_SEQUENCE_MISMATCH_DETECTED
		}
		else
		{
			//*sequentialActivationFound = false;
		}
		#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_WORD_INDEX_OF_EXISTING_COMPONENT
	}
	#endif
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	}
	#endif
	
	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
bool SANIpropagateOperationsClass::identifyMissingOrVariableEndComponentFound(vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* missingOrVariableEndComponentFound)
{
	bool result = true;
	
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent)
	{					
		if(i == 0)
		{
			bool variableFirstComponentTypeRequirements = false;
			if(!(currentComponent->neuronComponentConnectionActive))	//consider removing this requirement to allow groups identified for last variable components creation to have their activations reset
			{
				variableFirstComponentTypeRequirements = true;
			}
			if(variableFirstComponentTypeRequirements)
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS
				bool testFirstComponentNotString = false;
				bool testLastComponentNotString = false;
				bool testFirstComponentSubgroupsUnique = false;
				bool testLastComponentSubgroupsUnique = false;
				bool testNotDualLowerLevelConnections = false;
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_NON_STRING
				testFirstComponentNotString = true;
				testLastComponentNotString = true;
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_ALL_NEURONS_IN_FIRST_COMPONENT_SUBGROUPS_ARE_UNIQUE
				testFirstComponentSubgroupsUnique = true;
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENT_ALL_NEURONS_IN_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE
				testLastComponentSubgroupsUnique = true;
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISABLE_MULTIPLE_INPUTS_IF_HAS_IDENTICAL_COMPONENTS
				testNotDualLowerLevelConnections = true;
				#endif
				if(calculateVariableComponentPassCriteria(currentComponent, components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, testFirstComponentNotString, testLastComponentNotString, testFirstComponentSubgroupsUnique, testLastComponentSubgroupsUnique, testNotDualLowerLevelConnections))
				{
				#endif
					//cout << "missingOrVariableEndComponentFound = true" << endl;
					*missingOrVariableEndComponentFound = true;
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS
				}
				#endif
			}	
		}
		else
		{
			*missingOrVariableEndComponentFound = false;
		}
	}
	
	return result;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
bool SANIpropagateOperationsClass::identifyMissingOrVariableStartComponentFound(vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* missingOrVariableStartComponentFound)
{
	bool result = true;
	
	//note it doesn't matter if the start component is active or inactive. Even if it has been activated miscellaneously (ie without maintaining word index connectivity), it can be reset.
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent)
	{
		if(i == 0)
		{
			bool variableFirstComponentTypeRequirements = false;
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_MISSING
			if(!(currentComponent->neuronComponentConnectionActive))
			{
				variableFirstComponentTypeRequirements = true;
			}
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_VARIABLE
			if(currentComponent->neuronComponentConnectionActive)
			{
				variableFirstComponentTypeRequirements = true;
			}
			#endif
			if(variableFirstComponentTypeRequirements)
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS
				bool testFirstComponentNotString = false;
				bool testLastComponentNotString = false;
				bool testFirstComponentSubgroupsUnique = false;
				bool testLastComponentSubgroupsUnique = false;
				bool testNotDualLowerLevelConnections = false;
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING
				testFirstComponentNotString = true;
				#endif
				//testLastComponentNotString = true;
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENT_ALL_NEURONS_IN_FIRST_COMPONENT_SUBGROUPS_ARE_UNIQUE
				testFirstComponentSubgroupsUnique = true;
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENT_ALL_NEURONS_IN_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE
				testLastComponentSubgroupsUnique = true;
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISABLE_MULTIPLE_INPUTS_IF_HAS_IDENTICAL_COMPONENTS
				testNotDualLowerLevelConnections = true;
				#endif
				if(calculateVariableComponentPassCriteria(currentComponent, components, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, testFirstComponentNotString, testLastComponentNotString, testFirstComponentSubgroupsUnique, testLastComponentSubgroupsUnique, testNotDualLowerLevelConnections))
				{
				#endif
					//cout << "missingOrVariableStartComponentFound = true" << endl;
					*missingOrVariableStartComponentFound = true;
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS
				}
				#endif
			}	
		}
		else
		{
			*missingOrVariableStartComponentFound = false;	//removed @GIA3j5aTEMP18
		}
	}
	
	return result;
}
#endif

bool SANIpropagateOperationsClass::identifyMissingStartComponentFound(vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* missingStartComponentFound, bool* stillParsingActiveComponents)
{
	bool result = true;
	
	if(!(currentComponent->neuronComponentConnectionActive))
	{

		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_MISSING
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent)
		{							
			if(previousActiveComponent == NULL)
			{
				//support activation of group components with missing start components
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING_COMPREHENSIVE
				if(currentComponent->componentType != GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
				#endif
					*missingStartComponentFound = true;
					//cout << "2missingStartComponentFound" << endl;
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING_COMPREHENSIVE
				}
				else
				{
					//cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING_COMPREHENSIVE: (currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)" << endl;
					*stillParsingActiveComponents = false;	
				}
				#endif

			}
			else
			{	
				if((i == 0) && (currentComponent->componentType != GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING))
				{
					//ie (*missingOrVariableStartComponentFound)
				}
				else
				{
					*stillParsingActiveComponents = false;
				}
			}
		}
		else
		{
			*stillParsingActiveComponents = false;
		}
		#else
		*stillParsingActiveComponents = false;
		#endif
		#else

		#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
		//#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_MISSING
		if(forwardPropogationSignalData->firstIndexInSequence == 0)
		{
			*stillParsingActiveComponents = false;	
		}
		else
		{							
			if(previousActiveComponent == NULL)
			{
				//support activation of group components with missing start components
				*missingStartComponentFound = true;
				//cout << "1missingStartComponentFound" << endl;
			}
			else
			{
				*stillParsingActiveComponents = false;	//found an active component then a missing component (fail)
			}
		}
		//#else
		//stillParsingActiveComponents = false;
		//#endif
		#else
		*stillParsingActiveComponents = false;	
		#endif

		#endif
	}
	
	return result;		
}


#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS
bool SANIpropagateOperationsClass::calculateVariableComponentPassCriteria(SANIComponentNeuralNetwork* component, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool testFirstComponentNotString, const bool testLastComponentNotString, const bool testFirstComponentSubgroupsUnique, const bool testLastComponentSubgroupsUnique, const bool testNotDualLowerLevelConnections)
{
	bool passCriteria = true;

	SANIComponentNeuralNetwork* secondComponent = NULL;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		secondComponent = (*components)[0];	//(*components)[c-1];	//as i==0
	}
	else
	{
		secondComponent = (*components)[1];	//(*components)[c+1];	//as i==0
	}
			
	if(testFirstComponentNotString)
	{
		if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)	//added GIA3j5aTEMP65
		{
			passCriteria = false;
		}
	}
	if(testLastComponentNotString)
	{
		if(secondComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		{
			passCriteria = false;
		}
	}	
	if(testFirstComponentSubgroupsUnique)
	{
		for(int j=0; j<component->ANNbackGroupConnectionList.size(); j++)
		{
			SANIGroupNeuralNetwork* subGroup = (component->ANNbackGroupConnectionList)[j];
			#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
			if(groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient(subGroup))
			#else
			if(groupContainsNeuronWithinProspectiveAlternateSubgroup(forwardPropogationSentenceData, subGroup))
			#endif
			{	
				passCriteria = false;
			}
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
			groupContainsNeuronWithinProspectiveAlternateSubgroupReset(subGroup);
			#endif
		}
	}
	if(testLastComponentSubgroupsUnique)
	{	
		#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
		if(secondComponent->ANNbackGroupConnectionList.size() > 1)
		{
			cerr << "testLastComponentSubgroupsUnique; SANIpropagateOperationsClass::calculateVariableComponentPassCriteria error: (secondComponent->ANNbackGroupConnectionList.size() > 1)" << endl;
			exit(EXIT_ERROR);
		}
		#endif
		
		SANIGroupNeuralNetwork* secondComponentSubGroup = (secondComponent->ANNbackGroupConnectionList)[0];	//second component only has 1 reference	
		#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
		if(groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient(secondComponentSubGroup))
		#else
		if(groupContainsNeuronWithinProspectiveAlternateSubgroup(forwardPropogationSentenceData, secondComponentSubGroup))
		#endif
		{	
			passCriteria = false;
			//cout << "groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient" << endl;
		}
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
		groupContainsNeuronWithinProspectiveAlternateSubgroupReset(secondComponentSubGroup);
		#endif
	}
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISABLE_MULTIPLE_INPUTS_IF_HAS_IDENTICAL_COMPONENTS
	if(testNotDualLowerLevelConnections)
	{
		if(findValidDualLowerLevelConnection(forwardPropogationSentenceData, forwardPropogationWordData, components, component, false))
		{	
			passCriteria = false;
		}
	}
	#endif
	
	return passCriteria;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR
bool SANIpropagateOperationsClass::upperNeuronContainsWordIndexOfProspectiveComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIApreprocessorPlainTextWord* neuronComponentConnectionActiveWordRecord, SANIComponentNeuralNetwork* component, SANIGroupNeuralNetwork* group)
{
	bool result = false;
	
	for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
		SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
		//cout << "SANIpropagateOperationsClass::upperNeuronContainsWordIndexOfProspectiveComponent: ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
	
		//method2;
		SANIComponentNeuralNetwork* previousComponent = NULL;
		int c = currentComponent->componentIndex;
		int cPrevious;
		bool foundPreviousComponent = false;
		if(parseSentenceReverse)
		{
			cPrevious = c+1;
			if(cPrevious < ownerGroup->components.size())
			{
				foundPreviousComponent = true;
			}
		}
		else
		{
			cPrevious = c-1;
			if(cPrevious >= 0)
			{
				foundPreviousComponent = true;
			}
		}
		if(foundPreviousComponent)
		{
			previousComponent = (ownerGroup->components)[cPrevious];

			if(previousComponent->neuronComponentConnectionActive)
			{				
				if(previousComponent->neuronComponentConnectionActiveWordRecord == neuronComponentConnectionActiveWordRecord)
				{
					result = true;
					//cout << "upperNeuronContainsWordIndexOfProspectiveComponent - forwardPropogationWordData has already been propagated to an upper group (prevent usage duplication)" << endl;
					//cout << "previousComponent->neuronComponentConnectionActiveWordRecord->w = " << previousComponent->neuronComponentConnectionActiveWordRecord->w << endl;
					//cout << "neuronComponentConnectionActiveWordRecord->w = " << neuronComponentConnectionActiveWordRecord->w << endl;
					//exit(EXIT_ERROR);
				}
			}
		}
			
		/*
		//method1;
		SANIGroupParseTree* ownerGroupParseTree = ownerGroup->currentParseTreeGroupTemp;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			if(ownerGroupParseTree->currentParseTreeGroupTemp->parseTreeMinWordIndex == neuronComponentConnectionActiveWordRecord->w)	//<=	//component->neuronComponentConnectionActiveWordRecord->w
			{
				result = true;
				cout << "upperNeuronContainsWordIndexOfProspectiveComponent 1 - forwardPropogationWordData has already been propagated to an upper group (prevent usage duplication)" << endl;
			}
		}
		else
		{
			if(ownerGroupParseTree->currentParseTreeGroupTemp->parseTreeMaxWordIndex == neuronComponentConnectionActiveWordRecord->w)	//>=
			{	
				result = true;
				cout << "upperNeuronContainsWordIndexOfProspectiveComponent 2 - forwardPropogationWordData has already been propagated to an upper group (prevent usage duplication)" << endl;
			}
		}
		*/

		if(upperNeuronContainsWordIndexOfProspectiveComponent(forwardPropogationSentenceData, neuronComponentConnectionActiveWordRecord, component, ownerGroup))
		{
			result = true;
		}	

	}
							
	return result;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED
//assumes component refers to current component [forwardPropogationWordData->w is last word index in this component], and testing against next component in group
bool SANIpropagateOperationsClass::repeatedSequenceDetected(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIGroupParseTree* currentParseTreeGroupTemp, int wordTranslatorSentenceWordIndex)
{
	bool result = false;
	
	int componentWordIndexCoverage = 0;
	
	if(currentParseTreeGroupTemp != NULL)
	{
		componentWordIndexCoverage = currentParseTreeGroupTemp->parseTreeMaxWordIndex - currentParseTreeGroupTemp->parseTreeMinWordIndex + 1;
	}
	else
	{
		componentWordIndexCoverage = 1;	//groupType=string
	}

	vector<GIApreprocessorPlainTextWord*> componentWordArray1;
	vector<GIApreprocessorPlainTextWord*> componentWordArray2;

	int componentWmin1 = INT_DEFAULT_VALUE;
	int componentWmax1 = INT_DEFAULT_VALUE;
	int componentWmin2 = INT_DEFAULT_VALUE;
	int componentWmax2 = INT_DEFAULT_VALUE;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		componentWmax1 = wordTranslatorSentenceWordIndex+(componentWordIndexCoverage-1);
		componentWmin1 = wordTranslatorSentenceWordIndex;
		componentWmax2 = wordTranslatorSentenceWordIndex-1;
		componentWmin2 = wordTranslatorSentenceWordIndex-1-(componentWordIndexCoverage-1);
	}
	else
	{
		componentWmin1 = wordTranslatorSentenceWordIndex-(componentWordIndexCoverage-1);
		componentWmax1 = wordTranslatorSentenceWordIndex;
		componentWmin2 = wordTranslatorSentenceWordIndex+1;
		componentWmax2 = wordTranslatorSentenceWordIndex+1+(componentWordIndexCoverage-1);
	}
	
	if(((componentWmin1 >= 0) && (componentWmin2 >= 0)) && ((componentWmax1 < forwardPropogationSentenceData->sentenceContents->size()) && (componentWmax2 < forwardPropogationSentenceData->sentenceContents->size())))
	{
		result = true;
		
		for(int w = componentWmin1; w<=componentWmax1; w++)
		{
			GIApreprocessorPlainTextWord* currentWord = (*(forwardPropogationSentenceData->sentenceContents))[w];	
			componentWordArray1.push_back(currentWord);
		}
		for(int w = componentWmin2; w<=componentWmax2; w++)
		{
			GIApreprocessorPlainTextWord* currentWord = (*(forwardPropogationSentenceData->sentenceContents))[w];	
			componentWordArray2.push_back(currentWord);
		}
		for(int i=0; i<componentWmax1-componentWmin1+1; i++)
		{
			//must sync code with SANIpropagateCompactClass::propagateWordThroughNetworkIntro !currentWordAmbiguous exceptions
			
			#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
			if(componentWordArray1[i]->tagName != componentWordArray2[i]->tagName)
			{
				result = false;
			}
			#else
			if(!SANInodes.currentWordAmbiguous(componentWordArray1[i]) && !SANInodes.currentWordAmbiguous(componentWordArray2[i]))
			{
				if(componentWordArray1[i]->unambiguousPOSindex != componentWordArray2[i]->unambiguousPOSindex)
				{
					result = false;
				}
			}
			else
			{				
				#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
				if(SANInodes.currentWordAmbiguous(componentWordArray1[i]))
				{	
					int wordPOStype1 = INT_DEFAULT_VALUE;
					bool pass = SANInodes.getWordPOStypeFromAmbiguousWord(componentWordArray1[i], &wordPOStype1);
					if(pass)
					{
						if(componentWordArray2[i]->unambiguousPOSindex != wordPOStype1)
						{
							result = false;
						}
					}
					else
					{
						result = false;
					}
				}		
				else if(SANInodes.currentWordAmbiguous(componentWordArray2[i]))
				{
					int wordPOStype2 = INT_DEFAULT_VALUE;
					bool pass = SANInodes.getWordPOStypeFromAmbiguousWord(componentWordArray2[i], &wordPOStype2);
					if(pass)
					{
						if(componentWordArray1[i]->unambiguousPOSindex != wordPOStype2)
						{
							result = false;
						}
					}
					else
					{
						result = false;
					}
				}
				else
				{
					//both words are ambiguous

					int wordPOStype1 = INT_DEFAULT_VALUE;
					bool pass1 = SANInodes.getWordPOStypeFromAmbiguousWord(componentWordArray1[i], &wordPOStype1);
					int wordPOStype2 = INT_DEFAULT_VALUE;
					bool pass2 = SANInodes.getWordPOStypeFromAmbiguousWord(componentWordArray2[i], &wordPOStype2);
					if(pass1 && pass2)
					{
						if(wordPOStype1 != wordPOStype2)
						{
							result = false;
						}
					}
					else
					{
						result = false;
					}
				}
				#endif
			}
			#endif
		}	
	}
					
	return result;
}
#endif

#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
bool SANIpropagateOperationsClass::groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient(SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = false;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	if(!currentNeuron->counted)
	{
		currentNeuron->counted = true;
	#endif
		if(!SANInodes.isNeuronString(currentNeuron))	//CHECKTHIS
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
			if(currentNeuron->verified)
			{
				cout << "SANIpropagateOperationsClass::groupContainsNeuronWithinProspectiveAlternateSubgroup error: currentNeuron has already been parsed (circular loop detected)" << endl;
				exit(EXIT_ERROR);
			}
			currentNeuron->verified = true;
			#endif

			if(currentNeuron->marked)
			{
				//cout << "groupContainsNeuronWithinProspectiveAlternateSubgroup" << endl;
				result = true;	
			}	
			if(!result)
			{
				for(int i=0; i<currentNeuron->components.size(); i++)
				{
					SANIComponentNeuralNetwork* currentComponent = currentNeuron->components[i];
					for(int j=0; j<currentComponent->ANNbackGroupConnectionList.size(); j++)
					{
						SANIGroupNeuralNetwork* subGroup = (currentComponent->ANNbackGroupConnectionList)[j];
						if(groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient(subGroup))
						{
							result = true;
						}
					}
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
#endif
#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_BASIC
bool SANIpropagateOperationsClass::groupContainsNeuronWithinProspectiveAlternateSubgroup(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = false;
	
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
				cout << "SANIpropagateOperationsClass::groupContainsNeuronWithinProspectiveAlternateSubgroup error: currentNeuron has already been parsed (circular loop detected)" << endl;
				exit(EXIT_ERROR);
			}
			currentNeuron->verified = true;
			#endif

			bool candidateNeuronInCompleteHistory = false;
			for(int i=0; i<forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.size(); i++)
			{
				if(currentNeuron == forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory[i])
				{
					//cout << "existingSubgroupContainsNeuronWithinProspectiveAlternateSubgroup" << endl;
					result = true;	
				}
			}	
			if(!result)
			{
				for(int i=0; i<currentNeuron->components.size(); i++)
				{
					SANIComponentNeuralNetwork* currentComponent = currentNeuron->components[i];
					for(int j=0; j<currentComponent->ANNbackGroupConnectionList.size(); j++)
					{
						SANIGroupNeuralNetwork* subGroup = (currentComponent->ANNbackGroupConnectionList)[j];
						if(groupContainsNeuronWithinProspectiveAlternateSubgroup(forwardPropogationSentenceData, subGroup))
						{
							result = true;
						}
					}
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
#endif
#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
bool SANIpropagateOperationsClass::groupContainsNeuronWithinProspectiveAlternateSubgroupReset(SANIGroupNeuralNetwork* currentNeuron)
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
					groupContainsNeuronWithinProspectiveAlternateSubgroupReset(subGroup);
				}
			}
		}
	}
	
	return result;
}
#endif


#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_NEXT_SEQUENCE_IS_SAME_AS_CURRENT_SEQUENCE
bool SANIpropagateOperationsClass::consecutiveSequenceDetected(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationWordData* forwardPropogationWordData, vector<SANIComponentNeuralNetwork*>* components, SANIComponentNeuralNetwork* component)
{
	bool result = false;
	
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
	{			
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			if(component->ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex == forwardPropogationWordData->w+1)
			{
				//group is being repeated:
				result = true;
				//cout << "consecutiveSequenceDetected 1" << endl;
			}
		}
		else
		{
			if(component->ownerGroup->currentParseTreeGroupTemp->parseTreeMaxWordIndex == forwardPropogationWordData->w-1)
			{	
				//group is being repeated:
				result = true;
				//cout << "consecutiveSequenceDetected 2" << endl;
			}
		}
	}
	else
	{
		//cout << "SANIpropagateOperationsClass::consecutiveSequenceDetected warning: !(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)" << endl;
	}
							
	return result;
}
#endif



#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_ACTIVATION_IF_REPEATED_SEQUENCE_MISMATCH_DETECTED
bool SANIpropagateOperationsClass::findValidDualLowerLevelConnectionAlternate(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationWordData* forwardPropogationWordData, vector<SANIComponentNeuralNetwork*>* components, SANIComponentNeuralNetwork* component)
{	
	bool validDualLowerLevelConnectionFound = false;
	bool secondComponentFound = false;
	bool firstComponentFound = false;
	for(int i2=0; i2<components->size(); i2++)
	{
		int c2;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			c2 = components->size()-1-i2;
		}
		else
		{
			c2 = i2;
		}

		SANIComponentNeuralNetwork* component2 = (*components)[c2];
		
		if(!firstComponentFound)
		{
			firstComponentFound = true;
		}
		else
		{
			if(!secondComponentFound)
			{
				secondComponentFound = true;
				for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
				{
					SANIGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];

					for(int l2=0; l2<component2->ANNbackGroupConnectionList.size(); l2++)
					{
						SANIGroupNeuralNetwork* groupSource2 = component2->ANNbackGroupConnectionList[l2];

						if(findGroup1InFirstSectionOfGroup2BackConnection(forwardPropogationSentenceData, groupSource2, groupSource))
						{
							validDualLowerLevelConnectionFound = true;
						}
					}
				}
			}
		}
	}
	return validDualLowerLevelConnectionFound;
}

bool SANIpropagateOperationsClass::findGroup1InFirstSectionOfGroup2BackConnection(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentGroup2, SANIGroupNeuralNetwork* group1ToFound)
{
	bool result = false;
	
	if(currentGroup2 == group1ToFound)
	{
		result = true;	
	}
	
	if(currentGroup2->components.size() > 0)
	{
		int c2;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			c2 = currentGroup2->components.size()-1;
		}
		else
		{
			c2 = 0;
		}
		SANIComponentNeuralNetwork* component2 = (currentGroup2->components)[c2];

		for(int l2=0; l2<component2->ANNbackGroupConnectionList.size(); l2++)
		{
			SANIGroupNeuralNetwork* groupSource2 = component2->ANNbackGroupConnectionList[l2];
			if(findGroup1InFirstSectionOfGroup2BackConnection(forwardPropogationSentenceData, groupSource2, group1ToFound))
			{
				result = true;
			}
		}
	}
	
	return result;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF

//assumes component refers to current component, and testing against next component in group
bool SANIpropagateOperationsClass::findValidDualLowerLevelConnection(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationWordData* forwardPropogationWordData, vector<SANIComponentNeuralNetwork*>* components, SANIComponentNeuralNetwork* component, bool assumeFirstComponentActive)
{	
	bool validDualLowerLevelConnectionFound = false;
	bool secondComponentFound = false;
	bool firstComponentFound = false;
	for(int i2=0; i2<components->size(); i2++)
	{
		int c2;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			c2 = components->size()-1-i2;
		}
		else
		{
			c2 = i2;
		}

		SANIComponentNeuralNetwork* component2 = (*components)[c2];
		
		if(!firstComponentFound)
		{
			if(assumeFirstComponentActive)
			{
				if(component2->neuronComponentConnectionActive)
				{
					firstComponentFound = true;
				}
			}
			else
			{
				firstComponentFound = true;
			}
		}
		else
		{
			if(!secondComponentFound)
			{
				if(!(component2->neuronComponentConnectionActive) || !assumeFirstComponentActive)
				{
					secondComponentFound = true;
					for(int l2=0; l2<component2->ANNbackGroupConnectionList.size(); l2++)
					{
						SANIGroupNeuralNetwork* groupSource2 = component2->ANNbackGroupConnectionList[l2];

						for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
						{
							SANIGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];

							if(groupSource2 == groupSource)
							{
								#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_AND_SATISIFIES_WORD_INDEX
								if(component->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex == forwardPropogationWordData->wordReference->translatorSentenceWordIndex-1)	//or component->wordIndex
								{
								#endif
									validDualLowerLevelConnectionFound = true;
									//cout << "validDualLowerLevelConnectionFound" << endl;
									//cout << "\n\n\n\n SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE: SANIpropagateOperationsClass::findValidDualLowerLevelConnection validDualLowerLevelConnectionFound" << endl;
									//exit(EXIT_ERROR);
								#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_AND_SATISIFIES_WORD_INDEX
								}
								#endif
							}
						}
					}
				}
			}
		}
	}
	return validDualLowerLevelConnectionFound;
}
#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE
bool SANIpropagateOperationsClass::findGroup2InForwardConnectionBranchOfGroup1(SANIGroupNeuralNetwork* group2, SANIGroupNeuralNetwork* currentGroup)
{
	bool result = false;
	
	for(int i=0; i<currentGroup->ANNfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (currentGroup->ANNfrontComponentConnectionList)[i];
		SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	
		if(group2 == ownerGroup)
		{
			result = true;
		}
		else
		{
			if(findGroup2InForwardConnectionBranchOfGroup1(group2, ownerGroup))
			{
				result = true;
			}
		}
	}
	
	return result;
}
#endif
#endif

							
bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, bool* existingActivationFoundStartComponent)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	for(int i=0; i<components->size(); i++)
	{
		int c;
		if(parseSentenceReverse)
		{
			c = components->size()-1-i;
		}
		else
		{
			c = i;
		}
		SANIComponentNeuralNetwork* component = (*components)[c];
		if(component == testComponent)
		{
			if(component->neuronComponentConnectionActive)
			{
				//component already activated
				#ifdef SANI_ALLOW_MULTIPLE_ACTIVATIONS
				sequentialActivationFound = true;
				*existingActivationFoundStartComponent = true;
				#endif
			}	
			else
			{
				//sequential activation found
				sequentialActivationFound = true;
			}
		}
	}
	
	return sequentialActivationFound;
}

//precondition: repeat sequences can contain only 1 component
bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationWordData* forwardPropogationWordData, bool* repeatDetected, bool* existingActivationFoundStartComponent)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	for(int i=0; i<components->size(); i++)
	{
		int c;
		if(parseSentenceReverse)
		{
			c = components->size()-1-i;
		}
		else
		{
			c = i;
		}
		SANIComponentNeuralNetwork* component = (*components)[c];
		if(component == testComponent)
		{
			#ifdef SANI_REPEAT_SEQUENCES_ENFORCE_CONSECUTIVE_STRING_COMPONENTS
			bool pass = true;
			if(testComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
				if(component->neuronComponentConnectionActive)
				{
					pass = false;
					int translatorSentenceWordIndexComparisonInt;
					if(parseSentenceReverse)
					{
						translatorSentenceWordIndexComparisonInt = forwardPropogationWordData->wordReference->translatorSentenceWordIndex + 1;
					}
					else
					{
						translatorSentenceWordIndexComparisonInt = forwardPropogationWordData->wordReference->translatorSentenceWordIndex - 1;
					}
					if(component->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex == translatorSentenceWordIndexComparisonInt)
					{
						pass = true;
					}
				}
			}
			if(pass)
			{
			#endif
				//sequential activation found
				if(component->neuronComponentConnectionActive)
				{
					*repeatDetected = true;
					/*
					//do not treat repeats as existingActivationFoundStartComponent
					#ifdef SANI_ALLOW_MULTIPLE_ACTIVATIONS
					//if(component->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex == forwardPropogationWordData->wordReference->translatorSentenceWordIndex)
					//{
						*existingActivationFoundStartComponent = true;
					//}
					#endif
					*/
				}
				sequentialActivationFound = true;
			#ifdef SANI_REPEAT_SEQUENCES_ENFORCE_CONSECUTIVE_STRING_COMPONENTS
			}
			#endif
		}
		else
		{
			cerr << "SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady{} error: repeat sequences can contain only 1 component" << endl;
			exit(EXIT_ERROR);
		}
	}
	
	return sequentialActivationFound;
}



#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
#ifdef SANI_HEAVY
bool SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper(SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData)
{
	bool result = false;

	bool existingActivationFoundNOTUSED = false;

	SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() > 0)
	{
		int lastActiveComponentInParseTreeIndex = activationPathWordCurrentParseTreeGroupOwner->components.size() - 1;
		SANIComponentParseTree* lastActiveComponentInParseTree = (activationPathWordCurrentParseTreeGroupOwner->components)[lastActiveComponentInParseTreeIndex];
		SANIComponent* lastActiveComponentNOTUSED = NULL;
		SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
		
		SANIGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRefNOTUSED = NULL;
		SANIGroupNeuralNetwork* ownerGroupNOTUSED = NULL;
		result = componentWordConnectivityTests(ownerGroupNOTUSED, prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentNOTUSED, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, previousActiveComponentInParseTreeParseTreeGroupRefNOTUSED, forwardPropogationWordData, existingActivationFoundNOTUSED);
	}
	else
	{
		result = true;
	}
	
	return result;
}
#else
#ifdef SANI_LIGHT_UNOPTIMISED
bool SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData)
{
	bool existingActivationFoundStartComponent = false;
	bool existingActivationFoundEndComponent = false;
	return componentWordConnectivityTestsWrapper(ownerGroup, ownerGroup->currentParseTreeGroupTemp, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, existingActivationFoundStartComponent, existingActivationFoundEndComponent);
}
#else
bool SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent)
{
	bool existingActivationFoundEndComponent = false;
	return componentWordConnectivityTestsWrapper(ownerGroup, ownerGroup->currentParseTreeGroupTemp, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, existingActivationFoundStartComponent, existingActivationFoundEndComponent);
}
#endif
bool SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent, const bool existingActivationFoundEndComponent)
{
	bool result = false;

	SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;

	SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef = NULL;
	SANIGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef = NULL;	
	SANIComponentParseTree* lastActiveComponentInParseTree = NULL;
	SANIGroupParseTree* ownerGroupParseTreeGroup = currentParseTreeGroupTemp;

	#ifdef SANI_SEQUENCE_GRAMMAR
	if(existingActivationFoundStartComponent)
	{
		//group activations will be reset so assume real ownerGroupParseTreeGroup->components.size() == 0 
		result = true;
	}
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS_REMEMBER_FIRST_COMPONENT_WORD_INDEX
	else if(existingActivationFoundEndComponent)
        {
        	if(ownerGroupParseTreeGroup->components.size() == 2)	//assumes 2 active components in parsetree
		{	
			#ifdef SANI_PARSE_SAVE_PARSE_TREE
			int lastActiveComponentInParseTreeIndex;
			if(parseSentenceReverse)
			{
				lastActiveComponentInParseTreeIndex = 1;
			}
			else
			{
				lastActiveComponentInParseTreeIndex = 0;
			}
			SANIComponentParseTree* lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];
			lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
			//note lastActiveComponentInParseTreeParseTreeGroupRef will be empty in the case of a component->type == STRING
			
			//added GIA3k13a for SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY;
			int lastActiveComponentIndex = lastActiveComponentInParseTree->componentIndex;
			SANIComponent* lastActiveComponent = ownerGroup->components[lastActiveComponentIndex];
			#else
			cerr << "SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper currently requires SANI_PARSE_SAVE_PARSE_TREE" << endl;
			exit(EXIT_ERROR);
			#endif
			
			if(componentWordConnectivityTests(ownerGroup, prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponent, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, previousActiveComponentInParseTreeParseTreeGroupRef, forwardPropogationWordData, existingActivationFoundStartComponent))
			{	
				//cout << "existingActivationFoundEndComponent pass" << endl;
				result = true;
			}
			else
			{
				//cout << "existingActivationFoundEndComponent fail" << endl;	
			}  
		}
		else
		{
			cerr << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_SUPPORT_VARIABLE_LAST_COMPONENTS_REMEMBER_FIRST_COMPONENT_WORD_INDEX: SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper error: (ownerGroupParseTreeGroup->components.size() != 2)" << endl;
			exit(EXIT_ERROR);
		}	
        }
	#endif
        else
	{
	#endif
		if(ownerGroupParseTreeGroup->components.size() > 0)
		{	
			#ifdef SANI_PARSE_SAVE_PARSE_TREE
		
			int lastActiveComponentInParseTreeIndex;
			#ifdef SANI_ALLOW_MULTIPLE_ACTIVATIONS
			int previousActiveComponentInParseTreeIndex;
			#endif
			
			if(parseSentenceReverse)
			{
				lastActiveComponentInParseTreeIndex = 0;
				#ifdef SANI_ALLOW_MULTIPLE_ACTIVATIONS
				previousActiveComponentInParseTreeIndex = 1;	
				#endif
			}
			else
			{
				lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
				#ifdef SANI_ALLOW_MULTIPLE_ACTIVATIONS
				previousActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 2;	
				#endif
			}

			SANIComponentParseTree* lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];
			lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
			
			//note lastActiveComponentInParseTreeParseTreeGroupRef will be empty in the case of a component->type == STRING
			
			//added GIA3k13a for SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY;
			int lastActiveComponentIndex = lastActiveComponentInParseTree->componentIndex;
			SANIComponent* lastActiveComponent = ownerGroup->components[lastActiveComponentIndex];

			SANIComponentParseTree* previousActiveComponentInParseTree = NULL;
			#ifdef SANI_ALLOW_MULTIPLE_ACTIVATIONS
			if(ownerGroupParseTreeGroup->components.size() > 1)
			{
				previousActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[previousActiveComponentInParseTreeIndex];
				previousActiveComponentInParseTreeParseTreeGroupRef = previousActiveComponentInParseTree->parseTreeGroupRef;
			}
			#endif

			#else
			cerr << "SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper currently requires SANI_PARSE_SAVE_PARSE_TREE" << endl;
			exit(EXIT_ERROR);
			#endif
			
			if(componentWordConnectivityTests(ownerGroup, prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponent, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, previousActiveComponentInParseTreeParseTreeGroupRef, forwardPropogationWordData, existingActivationFoundStartComponent))
			{
				result = true;
			}		
		}
		else
		{
			result = true;
		}
	#ifdef SANI_SEQUENCE_GRAMMAR
	}
	#endif
			
	return result;
}
#endif

//note if !SANI_PARSE_GENERATE_PARSE_TREE, lastActiveComponentInParseTreeParseTreeGroupRef will be NULL, so must rely on lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex
bool SANIpropagateOperationsClass::componentWordConnectivityTests(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, SANIComponent* lastActiveComponent, SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, SANIComponentParseTree* lastActiveComponentInParseTree, SANIGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef, SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
	cout << "SANIpropagateOperationsClass::componentWordConnectivityTests:" << endl;
	#endif
	
	int wordIndexMax = -1;
	int wordIndexMin = REALLY_LARGE_INT;
	SANIGroupParseTree* parseTreeGroupToFindWordIndexMin;
	SANIGroupParseTree* parseTreeGroupToFindWordIndexMax;
	int wordIndexMinMin;
	int wordIndexMaxMax;
	if(parseSentenceReverse)
	{
		parseTreeGroupToFindWordIndexMin = lastActiveComponentInParseTreeParseTreeGroupRef;
		parseTreeGroupToFindWordIndexMax = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
		wordIndexMinMin = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;	//CHECKTHIS
		wordIndexMaxMax = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;	//CHECKTHIS
	}
	else
	{
		parseTreeGroupToFindWordIndexMin = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
		parseTreeGroupToFindWordIndexMax = lastActiveComponentInParseTreeParseTreeGroupRef;
		wordIndexMinMin = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		wordIndexMaxMax = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
	}

	if(parseTreeGroupToFindWordIndexMax != NULL)
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
		cout << "parseTreeGroupToFindWordIndexMax != NULL" << endl;
		cout << "parseTreeGroupToFindWordIndexMax->groupRef->groupIndex = " << parseTreeGroupToFindWordIndexMax->groupRef->groupIndex << endl;
		#endif
		if(!getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax, true, &wordIndexMax, 0))
		{
		}
	}
	wordIndexMax = SHAREDvars.maxInt(wordIndexMax, wordIndexMaxMax);

	if(parseTreeGroupToFindWordIndexMin != NULL)
	{
		if(!getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMin, false, &wordIndexMin, 0))
		{
		}
	}
	wordIndexMin = SHAREDvars.minInt(wordIndexMin, wordIndexMinMin);

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
	cout << "ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;	
	cout << "wordIndexMax = " << wordIndexMax << endl;
	cout << "wordIndexMin = " << wordIndexMin << endl;
	cout << "wordIndexMaxMax = " << wordIndexMaxMax << endl;
	cout << "wordIndexMinMin = " << wordIndexMinMin << endl;
	cout << "existingActivationFoundStartComponent = " << existingActivationFoundStartComponent << endl;
	#endif
	
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	int wordIndexProspectiveFirst;
	if(parseSentenceReverse)
	{
		wordIndexProspectiveFirst = wordIndexMax;
	}
	else
	{
		wordIndexProspectiveFirst = wordIndexMin;
	}
	if(componentWordConnectivityTestsCompareToMemory(wordIndexProspectiveFirst, ownerGroup, lastActiveComponent))	//OR: (wordIndexProspectiveFirst, [!stringComponent || SANI_PARSE_GENERATE_PARSE_TREE: lastActiveComponentInParseTreeParseTreeGroupRef->groupRef,] lastActiveComponentInParseTree->componentRef)
	#else
	if(wordIndexMin-wordIndexMax == 1)
	#endif
	{
		result = true;
	}
	#ifndef SANI_SEQUENCE_GRAMMAR
	#ifdef SANI_ALLOW_MULTIPLE_ACTIVATIONS
	else if(existingActivationFoundStartComponent)
	{
		//assumes SANI_REVERSE_DIRECTION:
		#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_MUTUALLY_EXCLUSIVE_BIO
		if(previousActiveComponentInParseTreeParseTreeGroupRef != NULL)
		{
			int wordIndexMinPrevious = previousActiveComponentInParseTreeParseTreeGroupRef->parseTreeMinWordIndex;
			if(wordIndexMinPrevious-wordIndexMax == 1)
			{
				result = true;
			}
		}
		else
		{
			if(wordIndexMin <= wordIndexMax)
			{
				result = true;
			}
		}
		#else
		if(wordIndexMin <= wordIndexMax)
		{
			result = true;
		}
		#endif
	}
	#endif
	#endif

	return result;
}

bool SANIpropagateOperationsClass::getMinMaxWordIndexInParseTree(SANIGroupParseTree* currentParseTreeGroup, bool findMaxOrMinWordIndex, int* wordIndexMaxOrMin, int level)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
	if(findMaxOrMinWordIndex)
	{
		SANInodes.printParseTreeDebugIndentation(level);
		cout << "SANIpropagateOperationsClass::getMinMaxWordIndexInParseTree: currentParseTreeGroup: " <<  currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;
	}
	#endif
	
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	if(currentParseTreeGroup->components.size() > 0)
	{
		if(findMaxOrMinWordIndex)
		{
			*wordIndexMaxOrMin = currentParseTreeGroup->parseTreeMaxWordIndex;
			//cout << "currentParseTreeGroup->parseTreeMaxWordIndex = " << currentParseTreeGroup->parseTreeMaxWordIndex << endl;
			result = true;
		}
		else
		{
			*wordIndexMaxOrMin = currentParseTreeGroup->parseTreeMinWordIndex;
			//cout << "currentParseTreeGroup->parseTreeMinWordIndex = " << currentParseTreeGroup->parseTreeMinWordIndex << endl;
			result = true;
		}
	}
	#else
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
		if(!findMaxOrMinWordIndex)
		{
			SANInodes.printParseTreeDebugIndentation(level);
			cout << "currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex = " << currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex  << endl;
		}
		#endif
		
		if(findMaxOrMinWordIndex)
		{
			if(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex > *wordIndexMaxOrMin)
			{
				*wordIndexMaxOrMin = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
				result = true;
			}
		}
		else
		{
			if(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex < *wordIndexMaxOrMin)
			{
				*wordIndexMaxOrMin = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
				result = true;
			}
		}
						
		if(currentComponent->parseTreeGroupRef != NULL)
		{					
			if(getMinMaxWordIndexInParseTree(currentComponent->parseTreeGroupRef, findMaxOrMinWordIndex, wordIndexMaxOrMin, level+1))
			{
				result = true;
			}
		}
	}
	#endif
	
	return result;	
}

#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
bool SANIpropagateOperationsClass::componentWordConnectivityTestsCompareToMemory(const int wordIndexProspectiveFirst, SANIGroupNeuralNetwork* ownerGroup, SANIComponent* lastActiveComponent)
{
	bool result = false;
	
	if(lastActiveComponent->neuronComponentConnectionActiveWordRecord == NULL)
	{
		cerr << "lastActiveComponent->componentIndex = " << lastActiveComponent->componentIndex << endl;
		cerr << "lastActiveComponent->componentType = " << lastActiveComponent->componentType << endl;
		cerr << "ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
		cerr << "SANIpropagateOperationsClass::componentWordConnectivityTestsCompareToMemory error: lastActiveComponent->neuronComponentConnectionActiveWordRecord == NULL" << endl;
		exit(EXIT_ERROR);
	}
	
	if(lastActiveComponent->neuronComponentConnectionActiveWordRecordMemory.size() != ownerGroup->parseTreeGroupMemory.size())
	{
		cerr << "lastActiveComponent->neuronComponentConnectionActiveWordRecordMemory.size() = " << lastActiveComponent->neuronComponentConnectionActiveWordRecordMemory.size() << endl;
		cerr << "ownerGroup->parseTreeGroupMemory.size() = " << ownerGroup->parseTreeGroupMemory.size() << endl;
		cerr << "SANIpropagateOperationsClass::componentWordConnectivityTestsCompareToMemory error: (lastActiveComponent->neuronComponentConnectionActiveWordRecordMemory.size() != ownerGroup->parseTreeGroupMemory.size())" << endl;
		exit(EXIT_ERROR);
	}
	
	int ownerGroupParseTreeGroupMemorySize = ownerGroup->parseTreeGroupMemory.size();	//OR: lastActiveComponent->neuronComponentConnectionActiveWordRecordMemory.size()
	GIApreprocessorPlainTextWord* lastActiveComponentNeuronComponentConnectionActiveWordRecordOrig = lastActiveComponent->neuronComponentConnectionActiveWordRecord;
	
	for(int m=0; m<ownerGroupParseTreeGroupMemorySize; m++)
	{
		//cout << "m = " << m << endl;
		
		int wordIndexMin;
		int wordIndexMax;
		GIApreprocessorPlainTextWord* neuronComponentConnectionActiveWordOption = lastActiveComponent->neuronComponentConnectionActiveWordRecordMemory[m];
		
		if(parseSentenceReverse)
		{
			wordIndexMin = neuronComponentConnectionActiveWordOption->translatorSentenceWordIndex;
			wordIndexMax = wordIndexProspectiveFirst;
		}
		else
		{
			wordIndexMin = wordIndexProspectiveFirst;
			wordIndexMax = neuronComponentConnectionActiveWordOption->translatorSentenceWordIndex;
		}
		
		/*
		cout << "wordIndexProspectiveFirst = " << wordIndexProspectiveFirst << endl;
		cout << "neuronComponentConnectionActiveWordOption->translatorSentenceWordIndex = " << neuronComponentConnectionActiveWordOption->translatorSentenceWordIndex << endl;
		cout << "wordIndexMin = " << wordIndexMin << endl;
		cout << "wordIndexMax = " << wordIndexMax << endl;
		*/
		
		if(wordIndexMin-wordIndexMax == 1)
		{
			bool overWrite = false;
			if(!result || neuronComponentConnectionActiveWordOption == lastActiveComponentNeuronComponentConnectionActiveWordRecordOrig)
			{
				overWrite = true;	//favour existing currentParseTreeGroupTemp
			}
			if(overWrite)
			{
				lastActiveComponent->neuronComponentConnectionActiveWordRecord = neuronComponentConnectionActiveWordOption;	//GIA3k13a
				ownerGroup->currentParseTreeGroupTemp = ownerGroup->parseTreeGroupMemory[m];	//GIA3k13a

				result = true;
			}
		}	
	}
	
	return result;
}
#endif

#endif



#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
bool SANIpropagateOperationsClass::calculatePerformanceWeightOfParseTree(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int* maxWeight)
{
	bool result;
	
	#ifdef SANI_PARSE_PERFORMANCE_WEIGHT_BIO		
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
	*maxWeight = currentParseTreeGroup->parseTreeTotalWeight;
	#else
	*maxWeight = currentParseTreeGroup->parseTreeMaxWeight;
	#endif
	#else
	bool print = false;
	bool performancePreprocess = false;
	int performanceNOTUSED = 0;
	bool calculateMaxWeight = true;
	*maxWeight = 0;
	traceBackpropParseTree(currentParseTreeGroup, 1, print, performancePreprocess, &performanceNOTUSED, forwardPropogationSentenceData->sentenceContents, calculateMaxWeight, maxWeight);
	resetNeuronBackprop(currentParseTreeGroup, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
	#endif
	
	return result;
}

bool SANIpropagateOperationsClass::setPerformanceWeightOfParseTree(SANIGroupParseTree* currentParseTreeGroup, const int maxWeight)
{
	bool result;
	
	#ifdef SANI_PARSE_PERFORMANCE_WEIGHT_BIO		
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
	currentParseTreeGroup->parseTreeTotalWeight = maxWeight;
	#else
	currentParseTreeGroup->parseTreeMaxWeight = maxWeight;
	#endif
	#else
	cerr << "SANIpropagateOperationsClass::setPerformanceWeightOfParseTree requires SANI_PARSE_PERFORMANCE_WEIGHT_BIO" << endl;
	exit(EXIT_ERROR);
	#endif
	
	return result;
}
#endif


#ifdef SANI_PARSE

#ifdef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE

#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
bool SANIpropagateOperationsClass::updatePerformance(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
	
	bool topLevelGroup = SANInodesGroupClassObject.isTopLevelGroupType(currentParseTreeGroup->groupTypeName, currentParseTreeGroup->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
	if(topLevelGroup)
	{
		result = updatePerformanceGroup(currentParseTreeGroup, forwardPropogationSentenceData, layer);
	}
	
	return result;
}
#endif

//doesn't perform topLevelGroup check;
bool SANIpropagateOperationsClass::updatePerformanceGroup(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = true;
				 
	#ifdef SANI_PARSE_PERFORMANCE_BIO
	forwardPropogationSentenceData->performance = currentParseTreeGroup->parseTreeMaxWordIndex - currentParseTreeGroup->parseTreeMinWordIndex + 1;
	#else
	int performanceTemp = 0;
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION
	cout << "SANI_SEQUENCE_WORDCONNECTIVITY_VERIFICATION: SANIpropagateOperationsClass::updatePerformanceGroup, traceBackpropParseTree:" << endl;
	bool print = true;
	#else
	bool print = false;
	#endif
	bool performancePreprocess = true;
	if(!traceBackpropParseTree(currentParseTreeGroup, 1, print, performancePreprocess, &performanceTemp, forwardPropogationSentenceData->sentenceContents))
	{
		cout << "SANIpropagateOperationsClass::updatePerformanceGroup fail #1" << endl;
		result = false;
	}
	resetNeuronBackprop(currentParseTreeGroup, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);

	#ifdef SANI_PARSE
	#ifdef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
	forwardPropogationSentenceData->performance = performanceTemp;
	#else
	forwardPropogationSentenceData->performance = 0;
	for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
	{
		GIApreprocessorPlainTextWord* currentWord = (forwardPropogationSentenceData->sentenceContents)->at(i);
		if(currentWord->alreadyFoundMatch)
		{
			forwardPropogationSentenceData->performance = forwardPropogationSentenceData->performance + 1;
			currentWord->alreadyFoundMatch = false;
		}
	}
	#endif
	#endif
	#endif
	
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
	if(result)
	{
		if(forwardPropogationSentenceData->performance != forwardPropogationSentenceData->sentenceContents->size())
		{
	#endif
			cout << "SANIpropagateOperationsClass::updatePerformanceGroup fail #2" << endl;
			result = false;
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
		}
	}
	#endif
			
	return result;
}
#endif

bool SANIpropagateOperationsClass::traceBackpropParseTree(SANIGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents)
{
	bool calculateMaxWeight = false;
	int maxWeightNOTUSED = 0;
	return traceBackpropParseTree(currentParseTreeGroup, level, print, performancePreprocess, performance, sentenceContents, calculateMaxWeight, &maxWeightNOTUSED);

}
bool SANIpropagateOperationsClass::traceBackpropParseTree(SANIGroupParseTree* currentParseTreeGroup, int level, const bool print, const bool performancePreprocess, int* performance, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool calculateMaxWeight, int* maxWeight)
{
	bool result = true;
	
	#ifndef SANI_DONT_SET_NEURON_TRACED
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
	#endif
	
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		if(calculateMaxWeight)
		{
			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
			*maxWeight = *maxWeight + currentParseTreeGroup->groupWeight;
			#else
			if(currentParseTreeGroup->groupWeight > *maxWeight)
			{
				*maxWeight = currentParseTreeGroup->groupWeight;
			}
			#endif
		}
		#endif

		if(performancePreprocess)
		{
			#ifdef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
			*performance = *performance + 1;
			//won't calculate performance perfectly because some groups are reused
			#else
			/*
			if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, currentParseTreeGroup->semanticRelationReturnEntityForwardPropogationSignalData->wordReference))
			{
				//nb this method won't work if subReferenceSets are syntactically identical (and neural net groups are therefore reused); eg the red dog eats a blue apple.
					//"dog" and "apple" will use identical neural groups and so will only count to +1 performance total
			}
			*/
			#endif
		}
				
		if(print)
		{
			//#ifdef SANI_DEBUG_PROPAGATE
			SANInodes.printParseTreeDebugIndentation(level);
			#ifdef SANI_SEQUENCE_GRAMMAR
			cout << "traceBackpropParseTree: currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << ", currentParseTreeGroup->groupRef->groupIndex = " << currentParseTreeGroup->groupRef->groupIndex << endl;
			#else
			cout << "traceBackpropParseTree: currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << ", currentParseTreeGroup->groupTypeName = " << currentParseTreeGroup->groupTypeName << endl;		//<< ", parse word (providing semanticRelationReturnEntity) = ?"
			#endif
			//#endif
		}
		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
			
			//#ifdef SANI_DEBUG_PROPAGATE_EXTRA
			if(print)	
			{
				SANInodes.printComponent(currentComponent, level);	//TEMP for DEBUG
			}
			//#endif
		
			if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
			
				#ifndef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
				if(performancePreprocess)
				{	
					if(GIApreprocessorWordClassObject.isWordInWordlist(sentenceContents, currentComponent->candidateStringMatch))
					{	
						#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_MUTUALLY_EXCLUSIVE
						if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)	//redundant?
						{
							if(currentComponent->candidateStringMatch->alreadyFoundMatch)
							{
								result = false;
								//cout << "duplicate instance of word detected in parsetree - fail to parse sentence" << endl;
								//duplicate instance of word detected in parsetree - fail to parse sentence
							}
							else
							{
						#endif		
								currentComponent->candidateStringMatch->alreadyFoundMatch = true;
								//nb this method won't work if subReferenceSets are syntactically identical (and neural net groups are therefore reused); eg the red dog eats a blue apple.
									//"the" and "a" will use identical neural groups and so will only count to +1 performance total
						#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT_MUTUALLY_EXCLUSIVE
							}
						}
						#endif
					}
				}
				#endif

				if(print)
				{
					SANInodes.printComponent(currentComponent, level+1);	//+1 added @GIA3j5aTEMP66
				}
			}
			#ifndef SANI_HEAVY_OPTIMISED	//CHECKTHIS
			else
			{
			#endif					
				if(currentComponent->parseTreeGroupRef != NULL)
				{												
					//#ifdef SANI_DEBUG_PROPAGATE_EXTRA
					if(print)
					{
						SANInodes.printParseTreeDebugIndentation(level);
						cout << "(currentComponent->parseTreeGroupRef != NULL): currentComponent->parseTreeGroupRef->groupName = " << currentComponent->parseTreeGroupRef->groupName << endl;
					}
					//#endif
					
					if(!traceBackpropParseTree(currentComponent->parseTreeGroupRef, level+1, print, performancePreprocess, performance, sentenceContents, calculateMaxWeight, maxWeight))
					{
						result = false;
					}
				}
				else
				{
					//cout << "(currentComponent->parseTreeGroupRef == NULL)" << endl;
				}
			#ifndef SANI_HEAVY_OPTIMISED
			}
			#endif
		}
	#ifndef SANI_DONT_SET_NEURON_TRACED	
	}
	#endif
	
	return result;
}

#endif





								
								
#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSHOC_STRICT
bool SANIpropagateOperationsClass::isSentenceWordDataFullyConnected(SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	//forwardPropogationWordData->previousWordConnections.push_back(wPrevious);
	bool result = true;
	
	int numberOfWords = forwardPropogationSentenceData->forwardPropogationWordDataArray.size();
	vector<bool> previousWordConnectionsFound(numberOfWords, false);
	previousWordConnectionsFound[numberOfWords-1] = true;	//set last word as found 
	SANIForwardPropogationWordData* lastWordData = ((forwardPropogationSentenceData->forwardPropogationWordDataArray)[numberOfWords-1]);
	isSentenceWordDataFullyConnected(&previousWordConnectionsFound, lastWordData);
	for(int w = 0; w<previousWordConnectionsFound.size(); w++)
	{
		if(!previousWordConnectionsFound[w])
		{
			result = false;
		}
	}
	
	return result;
}

void SANIpropagateOperationsClass::isSentenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, SANIForwardPropogationWordData* currentWordData)
{
	for(int i = 0; i<currentWordData->previousWordConnections.size(); i++)
	{
		SANIForwardPropogationWordData* previousWordData = (currentWordData->previousWordConnections)[i];
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
		cout << "previousWordConnectionsFound == true: previousWordData->w = " << previousWordData->w << endl;
		#endif
		(*previousWordConnectionsFound)[previousWordData->w] = true;
		isSentenceWordDataFullyConnected(previousWordConnectionsFound, previousWordData);
	}
}
#endif


#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE

#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
bool SANIpropagateOperationsClass::componentTests1(SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	if(currentComponent->wordNounVariantType != GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
	{
		componentTests = false;
		if(forwardPropogationSignalData->wordNounVariantType == currentComponent->wordNounVariantType)
		{
			componentTests = true;
		}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_DETECT_IRREGULAR_NOUN_FORMS
		else if(forwardPropogationSignalData->wordNounVariantType == GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_SINGULAR_OR_PLURAL)	
		{
			componentTests = true;
		}
		#endif
	}
	if(currentComponent->wordVerbVariantType != GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
	{
		componentTests = false;
		if(forwardPropogationSignalData->wordVerbVariantType == currentComponent->wordVerbVariantType)
		{
			componentTests = true;
		}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_INTERPRET_PAST_AND_PAST_PARTICIPLE_THE_SAME
		else if((forwardPropogationSignalData->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST) && (currentComponent->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE))
		{
			componentTests = true;
		}
		else if((forwardPropogationSignalData->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE) && (currentComponent->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST))
		{
			componentTests = true;
		}
		#endif
	}
	
	return componentTests;
}
#endif

#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
bool SANIpropagateOperationsClass::componentTests2(SANIGroupNeuralNetwork* group, SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool componentTests = true;
	
	/*
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
	Algorithm:
	#else
	Algorithm:
	previousWordPOStype/existsPreceedingWordPOStype requirement: there exists a previous word POStype that was succesfully parsed by a higher order neuron/group.
		Question: so how will the program know this until the higher order neuron group has been parsed?
		Conclusion: will need to;
			a) record the wordPos type for every activated component in a group
			b) read both;
				i) previous components in current group and 
				ii) read up the tree to see if this condition is met (ie there exists a previous word POStype that was succesfully parsed by a higher order neuron/group)
	#endif
	*/
	if(group->previousWordPOStype != "")
	{
		componentTests = false;
		int previousWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype);
		int translatorSentenceWordIndexMin = REALLY_LARGE_INT;
		GIApreprocessorPlainTextWord* firstWordInCurrentParseTreeGroupParseTree = NULL;
		int numComponentsFound = 0;
		bool pass = false;
		if(!getFirstWordInParseTree(currentParseTreeGroup, &firstWordInCurrentParseTreeGroupParseTree, &translatorSentenceWordIndexMin, &numComponentsFound))
		{
			firstWordInCurrentParseTreeGroupParseTree = forwardPropogationWordData->wordReference;
		}
		else
		{
			pass = true;
		}
		
		if(findPreviousWordInSentence(forwardPropogationSentenceData->sentenceContents, firstWordInCurrentParseTreeGroupParseTree, previousWordPOStype))		
		{
			componentTests = true;
		}
	}
	if(group->existsPreceedingWordPOStype != "")
	{
		componentTests = false;
		int existsPreceedingWordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(group->existsPreceedingWordPOStype);
		int translatorSentenceWordIndexMin = REALLY_LARGE_INT;
		GIApreprocessorPlainTextWord* firstWordInCurrentParseTreeGroupParseTree = NULL;
		int numComponentsFound = 0;
		if(!getFirstWordInParseTree(currentParseTreeGroup, &firstWordInCurrentParseTreeGroupParseTree, &translatorSentenceWordIndexMin, &numComponentsFound))
		{
			firstWordInCurrentParseTreeGroupParseTree = forwardPropogationWordData->wordReference;
		}
		else
		{
		
		}
		
		if(findPreceedingWordInSentence(forwardPropogationSentenceData->sentenceContents, firstWordInCurrentParseTreeGroupParseTree, existsPreceedingWordPOStype))		
		{
			componentTests = true;
		}
	}
	
	return componentTests;
}

bool SANIpropagateOperationsClass::getFirstWordInParseTree(SANIGroupParseTree* currentParseTreeGroup, GIApreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceWordIndexMin, int* numComponentsFound)
{
	bool result = false;
		
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		//if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		//{
			*numComponentsFound = *numComponentsFound + 1;
		//}
		
		int translatorSentenceWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		if(translatorSentenceWordIndex < *translatorSentenceWordIndexMin)
		{
			*translatorSentenceWordIndexMin = translatorSentenceWordIndex;
			*firstWordInCurrentParseTreeGroupParseTree = currentComponent->neuronComponentConnectionActiveWordRecord;
			result = true;
		}
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{							
			if(getFirstWordInParseTree(currentComponent->parseTreeGroupRef, firstWordInCurrentParseTreeGroupParseTree, translatorSentenceWordIndexMin, numComponentsFound))
			{
				result = true;
			}
		}
	}
	
	return result;
}

bool SANIpropagateOperationsClass::findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int previousWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	GIApreprocessorPlainTextWord* previousComponentWord = NULL;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(contextWord == currentWord)
			{
				stillReadingActivatedWords = false;
			}
			else
			{
				previousComponentWord = contextWord;
			}
		}
	}
	if(previousComponentWord != NULL)
	{
		if(checkWordPOStype(previousComponentWord, previousWordPOStype))
		{
			result = true;
		}
	}
			
	return result;
}
bool SANIpropagateOperationsClass::findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIApreprocessorPlainTextWord* currentWord, int existsPreceedingWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(contextWord == currentWord)
			{
				stillReadingActivatedWords = false;
			}
			else
			{
				if(checkWordPOStype(contextWord, existsPreceedingWordPOStype))
				{
					result = true;
				}
			}
		}
	}
	
	return result;
}

bool SANIpropagateOperationsClass::checkWordPOStype(GIApreprocessorPlainTextWord* currentWord, int unambiguousPOSindex)
{
	bool result = false;	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == unambiguousPOSindex)
	{
		result = true;
	}
	#else
	if(!SANInodes.currentWordAmbiguous(currentWord))
	{
		if(SHAREDvars.getBitValue(currentWord->POSambiguityInfo, unambiguousPOSindex))
		{
			result = true;
		}
	}
	#endif
	return result;
}
#endif
#endif



bool SANIpropagateOperationsClass::resetAllNeurons(vector<SANIGroupType*>* SANIGroupTypes, const int groupBoolIndexType)
{
	bool result = true;
	
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[i2];

			bool* boolPointer = NULL;
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_GENERATED)
			{
				boolPointer = &(group->neuronGenerated);
			}

			if(*boolPointer)
			{
				*boolPointer = false;
			}
		}
	}
	
	return result;
}

bool SANIpropagateOperationsClass::resetAllNeuronComponents(vector<SANIGroupType*>* SANIGroupTypes, const int groupBoolIndexType)
{
	bool result = true;
	
	//cout << "SANIGroupTypes->size() = " << SANIGroupTypes->size() << endl;
	
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[i2];

			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE)
			{
				resetGroupActivation(group);
			}
			#ifdef SANI_HEAVY_OPTIMISED
			
			#else
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF)
			{
				#ifdef SANI_DEBUG_PROPAGATE_EXTRA8
				cout << "group = " << group->groupName << endl;
				printGroup(group, 1);
				#endif
				
				resetGroupParseTreeGroupRef(group, true);
			}
			#endif
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED)
			{
				group->neuronPropagated = false;
			}
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE)
			{
				group->neuronPropagatedSave = group->neuronPropagated;
			}
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET)
			{
				group->neuronPropagated = group->neuronPropagatedSave;
			}
			#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROCESSED)
			{
				group->neuronProcessed = false;
			}
			#endif
			#ifdef SANI_LIGHT_OPTIMISED
			#ifdef SANI_SOLIDIFY_NET_BACKPROP
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
			{
				#ifdef SANI_SOLIDIFY_BIO
				group->solidified = false;
				#else
				group->currentParseTreeGroupTemp->solidified = false;
				#endif
			}
			#endif
			#endif
			#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
			if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_DEMARKATEOPTIMUMPATHWAY_RESET)
			{
				SANInodes.resetGroupOptimumPathway(group);
			}
			#endif
	
		}
	}
	
	return result;
}


#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS
bool SANIpropagateOperationsClass::isLastComponentFuzzy(SANIGroupNeuralNetwork* group)
{
	bool lastComponentIsFuzzy = false;

	int indexOfLastComponentInGroup = group->components.size()-1;
	SANIComponentNeuralNetwork* lastComponent = (group->components)[indexOfLastComponentInGroup];
	#ifdef SANI_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_TREAT_OR_AS_FUZZY
	if(lastComponent->optional || SANInodesComponentClassObject.componentHasSubcomponents(lastComponent))
	#else
	if(lastComponent->optional || (lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
	#endif
	{
		lastComponentIsFuzzy = true;
	}
		
	return lastComponentIsFuzzy;	
}
#endif

bool SANIpropagateOperationsClass::resetGroupActivation(SANIGroupNeuralNetwork* group)
{	
	bool result = true;
	
	group->neuronActive = false;	//not used
	resetComponentsActivation(&(group->components));
	
	return result;
}

bool SANIpropagateOperationsClass::resetComponentsActivation(vector<SANIComponentNeuralNetwork*>* components)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		SANIComponentNeuralNetwork* currentComponent = (*components)[i1];
		
		#ifdef SANI_RESET_COMPONENT_NEURONCOMPONENTCONNECTIONACTIVEWORDRECORD
		currentComponent->neuronComponentConnectionActiveWordRecord = NULL;
		#endif
		currentComponent->neuronComponentConnectionActive = false;
		
		#ifdef SANI_SUPPORT_COMPONENTS_SUB
		if(SANInodesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			resetComponentsActivation(&(currentComponent->subComponents));
		}
		#endif
	}
	
	return result;
}

bool SANIpropagateOperationsClass::restoreGroupActivation(SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* groupOrig)
{	
	bool result = true;

	group->neuronActive = groupOrig->neuronActive;
	restoreComponentsActivation(&(group->components), &(groupOrig->components));
	
	return result;
}

bool SANIpropagateOperationsClass::restoreComponentsActivation(vector<SANIComponentNeuralNetwork*>* components, vector<SANIComponentNeuralNetwork*>* componentsOrig)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		SANIComponentNeuralNetwork* currentComponent = (*components)[i1];
		SANIComponentNeuralNetwork* currentComponentOrig = (*componentsOrig)[i1];
		currentComponent->neuronComponentConnectionActive = currentComponentOrig->neuronComponentConnectionActive;
		#ifdef SANI_SUPPORT_COMPONENTS_SUB
		if(SANInodesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			restoreComponentsActivation(&(currentComponent->subComponents), &(currentComponentOrig->subComponents));
		}
		#endif
	}
	
	return result;
}




#ifdef SANI_PARSE
#ifdef SANI_HEAVY
#ifdef SANI_HEAVY_OPTIMISED
bool SANIpropagateOperationsClass::resetGroupParseTreeGroupRef(SANIGroupActivationMemory* activationMemoryGroup, SANIGroupParseTree* currentParseTreeGroup, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;	
	
	#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP			
	for(int i=1; i<activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray.size(); i++)	//i=1; never remove the first activationPathWordFirstActivationMemoryGroupActivationPoint (as this will correspond to activationPathWordFirstActivationMemoryGroup)
	{
		SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroupActivationPointTemp = activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray[i];
		SANIGroupParseTree* activationPathWordFirstParseTreeGroupActivationPointTemp = currentParseTreeGroup->activationPathWordFirstParseTreeGroupActivationPointArray[i];
		
		if(!(activationPathWordFirstActivationMemoryGroupActivationPointTemp->firstLevelActivationPointAdded))
		{
			SANInodes.deleteGroup(activationPathWordFirstActivationMemoryGroupActivationPointTemp);	
			if(!(currentParseTreeGroup->neuronTraced))	//parseTreeGroups in activationPointArray >i=0 can still be added to final parseTree
			{
				SANInodes.deleteGroup(activationPathWordFirstParseTreeGroupActivationPointTemp);
			}
		}
	}
	#endif

	if(deleteExistingParseTreeGroupRef)
	{
		if(!(currentParseTreeGroup->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
		{
			SANInodes.deleteGroup(currentParseTreeGroup);
		}
	}
		
	SANInodes.deleteGroup(activationMemoryGroup);
	
	return result;
}
#else
bool SANIpropagateOperationsClass::resetGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		for(int i=0; i<group->currentParseTreeGroupArray.size(); i++)
		{
			SANIGroupParseTree* tempParseTreeGroup = (group->currentParseTreeGroupArray)[i];
			if(!(tempParseTreeGroup->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
			{
				delete tempParseTreeGroup;
			}
		}
	}
	group->currentParseTreeGroupArray.clear();

	SANIGroupParseTree* tempParseTreeGroup = SANInodes.convertNeuralNetworkGroupToParseTreeGroupNew(group);	//new SANIGroupParseTree(*convertNeuralNetworkGroupToParseTreeGroup(group));
	/*
	NO, as dont wish to copy subcomponents into tempParseTreeGroup;
	SANIGroupParseTree* tempParseTreeGroup = copyGroup(group);
	deleteComponents(&(tempParseTreeGroup->components));
	*/
	tempParseTreeGroup->components.clear();
	group->currentParseTreeGroupArray.push_back(tempParseTreeGroup);		
	
	#ifdef SANI_SAVE_MEMORY_GROUPS
	for(int i=0; i<group->activationMemoryGroupArray.size(); i++)
	{
		SANIGroupActivationMemory* tempMemoryGroup = (group->activationMemoryGroupArray)[i];
		delete tempMemoryGroup;
	}
	group->activationMemoryGroupArray.clear();

	SANIGroupActivationMemory* tempMemoryGroup = SANInodes.convertNeuralNetworkGroupToMemoryActivationGroupNew(group); //copyGroup(convertNeuralNetworkGroupToMemoryActivationGroup(group));
	group->activationMemoryGroupArray.push_back(tempMemoryGroup);
	#endif
	
	return result;
}
#endif
#else
bool SANIpropagateOperationsClass::resetGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		if(group->currentParseTreeGroupTemp != NULL)
		{
			#ifndef SANI_DONT_SET_NEURON_TRACED
			if(!(group->currentParseTreeGroupTemp->neuronTraced))	//fixed GIA3g11aTEMP14
			//if(!(group->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
			{
			#endif
				//delete group->currentParseTreeGroupTemp;	//OLD: don't delete components
				SANInodes.deleteGroup(group->currentParseTreeGroupTemp);	//added GIA3g11aTEMP27
			#ifndef SANI_DONT_SET_NEURON_TRACED
			}
			#endif
		}
	}
	group->currentParseTreeGroupTemp = SANInodes.convertNeuralNetworkGroupToParseTreeGroupNew(group); 	//new SANIGroupParseTree(*convertNeuralNetworkGroupToParseTreeGroup(group));
	group->currentParseTreeGroupTemp->components.clear();	//added 3g1h
	
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY	
	//clear neuronComponentConnectionActiveWordRecordMemory
	if(deleteExistingParseTreeGroupRef)
	{
		group->parseTreeGroupMemory.clear();
		group->parseTreeGroupMemory.push_back(group->currentParseTreeGroupTemp);
		for(int i=0; i<group->components.size(); i++)
		{
			SANIComponent* currentComponent = (group->components)[i];
			currentComponent->neuronComponentConnectionActiveWordRecordMemory.clear();
		}
	}
	#endif
	
	return result;
}
bool SANIpropagateOperationsClass::restoreGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, SANIGroupParseTree* parseTreeGroupOrig)
{
	bool result = true;
	
	SANInodes.deleteGroup(group->currentParseTreeGroupTemp);	//CHECKTHIS - delete newly created currentParseTreeGroupTemp 
	group->currentParseTreeGroupTemp = parseTreeGroupOrig;
	
	return result;
}


#endif
#endif






#ifdef SANI_PARSE

bool SANIpropagateOperationsClass::resetNeuronBackprop(SANIGroupParseTree* currentParseTreeGroup, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;

	
	if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED)
	{
		boolPointer = &(currentParseTreeGroup->neuronTraced);
	}
	/*
	else if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
	{
		boolPointer = &(currentParseTreeGroup->neuronSolidified);
	}
	*/
				
	if(*boolPointer)
	{
		*boolPointer = false;
		
		for(int i1=0; i1<currentParseTreeGroup->components.size(); i1++)
		{
			SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i1];
			if(currentComponent->parseTreeGroupRef != NULL)
			{
				resetNeuronBackprop(currentComponent->parseTreeGroupRef, groupBoolIndexType);
			}
		}
	}
	
	return result;
}

#endif

#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
bool SANIpropagateOperationsClass::resetNeuronForwardProp(SANIGroupNeuralNetwork* group, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;
	if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED)
	{
		boolPointer = &(group->neuronPropagated);
	}
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	else if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED)
	{
		boolPointer = &(group->neuronPreviousWordPOStypeTested);
	}
	#endif
	*/
		
	if(*boolPointer)
	{
		//cout << "resetNeuronForwardProp: group = " << group->groupName << endl;
		*boolPointer = false;
		for(int i=0; i<group->ANNfrontComponentConnectionList.size(); i++)
		{
			SANIComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[i];
			SANIComponentNeuralNetwork* ownerComponent = currentComponent;
			if(currentComponent->isSubcomponent)
			{
				ownerComponent = currentComponent->ownerComponent;
			}
			SANIGroupNeuralNetwork* ownerGroup = ownerComponent->ownerGroup;
			resetNeuronForwardProp(ownerGroup, groupBoolIndexType);
		}
	}
	
	return result;
}

#endif


			


bool SANIpropagateOperationsClass::doesRequireResetGroupActivation(SANIComponentNeuralNetwork* Xcomponent, int indexOfXcomponentInGroup, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationWordData* forwardPropogationWordData)
{
	bool requireResetGroupActivation = false;
	
	#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
	if(Xcomponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
	{
		if(Xcomponent->subComponents.size() > 0)
		{
			if(Xcomponent->neuronComponentConnectionActive)
			{
				SANIComponentNeuralNetwork* subComponent = (Xcomponent->subComponents)[0];
				if(subComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					//expect repeated strings to occur consecutively
					int translatorSentenceWordIndexComparisonInt;
					if(parseSentenceReverse)
					{
						translatorSentenceWordIndexComparisonInt = forwardPropogationWordData->wordReference->translatorSentenceWordIndex + 1;
					}
					else
					{
						translatorSentenceWordIndexComparisonInt = forwardPropogationWordData->wordReference->translatorSentenceWordIndex - 1;
					}
					
					if(subComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex != translatorSentenceWordIndexComparisonInt)
					{
						requireResetGroupActivation = true;
					}
				}
				else
				{
					if(findNextAvailableComponent(indexOfXcomponentInGroup, ownerGroup))
					{
						requireResetGroupActivation = true;
					}
					else
					{
						//FUTURE: what if still need to reset however; what if it was activated incorrectly, or already activated from a previous successful parse (e.g. <group groupName="nounMultiword" semanticRelationFunctionName="multiwordNoun">)?
					}
				}
			}
			else
			{

			}
		}
		else
		{
			cerr << "SANIpropagateOperationsClass::doesRequireResetGroupActivation{} error: Xcomponent->subcomponents.size() == 0" << endl;
		}
	}
	else
	{
	#endif
		requireResetGroupActivation = true;
	#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
	}
	#endif
	
	return requireResetGroupActivation;
}

					

bool SANIpropagateOperationsClass::findNextAvailableComponent(int indexOfXcomponentInGroup, SANIGroupNeuralNetwork* ownerGroup)
{
	bool foundNextAvailableComponent = false;
	bool findingNextAvailableComponent = true;
	
	for(int i=indexOfXcomponentInGroup; i<ownerGroup->components.size(); i++)
	{
		int i2;
		if(parseSentenceReverse)
		{
			i2 = ownerGroup->components.size()-1-i;
		}
		else
		{
			i2 = i;
		}
		
		if(findingNextAvailableComponent)
		{
			bool i2Test = false;
			if(parseSentenceReverse)
			{
				if(i2 < indexOfXcomponentInGroup)
				{
					i2Test = true;
				}
			}
			else
			{
				if(i2 > indexOfXcomponentInGroup)
				{
					i2Test = true;
				}
			}
			
			if(i2Test)
			{
				SANIComponentNeuralNetwork* component = (ownerGroup->components)[i2];
				if(!(component->neuronComponentConnectionActive))
				{
					#ifdef SANI_SUPPORT_COMPONENTS_OPTIONAL
					if(!component->optional)
					{
					#endif
						findingNextAvailableComponent = false;
						foundNextAvailableComponent = true;
					#ifdef SANI_SUPPORT_COMPONENTS_OPTIONAL
					}
					#endif
				}
				else
				{
					findingNextAvailableComponent = false;
				}
			}
		}
	}
	
	return foundNextAvailableComponent;
}

#endif


#ifdef SANI_FREE_MEMORY
bool SANIpropagateOperationsClass::initialiseParseTreeGroupList(vector<SANIGroupType*>* SANIGroupTypes, vector<SANIGroupParseTree*>* parseTreeGroupList)
{	
	bool result = true;
	
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[i2];
			parseTreeGroupList->push_back(group->currentParseTreeGroupTemp);
			//cout << "initialiseParseTreeGroupList" << endl;
		}
	}
	
	return result;
}

bool SANIpropagateOperationsClass::deinitialiseParseTreeGroupList(vector<SANIGroupType*>* SANIGroupTypes, vector<SANIGroupParseTree*>* parseTreeGroupList)
{	
	bool result = true;
	
	for(int i2=0; i2<parseTreeGroupList->size(); i2++)
	{
		SANIGroupParseTree* parseTreeGroupTemp = (*parseTreeGroupList)[i2];
		SANInodes.deleteGroup(parseTreeGroupTemp);
	}
	parseTreeGroupList->clear();
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[i2];
			group->currentParseTreeGroupTemp = NULL;
		}
	}
	
	return result;
}
#endif



