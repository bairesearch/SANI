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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Operations - generic functions
 * /
 *******************************************************************************/


#include "SANIpropagateOperations.hpp"


#ifdef SANI_FORWARD



void SANIpropagateOperationsClass::setParseSentenceReverseStrict(bool parseSentenceReverseNew, SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	forwardPropogationSequenceData->parseSentenceReverse = parseSentenceReverseNew;
}

void SANIpropagateOperationsClass::setParseSentenceReverse(const bool normalDirection, SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
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
	forwardPropogationSequenceData->parseSentenceReverse = parseSentenceReverseNew;
}

bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, testComponent, components, activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, &existingActivationFoundNOTUSED);
}
#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
#ifdef SANI_SUPPORT_COMPONENTS_OR
bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(forwardPropogationSequenceElementData, forwardPropogationSequenceData, testComponent, components, &existingActivationFoundNOTUSED);
}
#endif
#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components, bool* repeatDetected)
{
	bool existingActivationFoundNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(forwardPropogationSequenceElementData, forwardPropogationSequenceData, testComponent, components, repeatDetected, &existingActivationFoundNOTUSED);
}
#endif
#endif

bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent, bool* existingActivationFoundStartComponent)
{
	bool missingStartComponentsFoundNOTUSED = false;
	bool missingOrVariableStartComponentFoundNOTUSED = false;
	bool missingOrVariableEndComponentFoundNOTUSED = false;
	int numberOfInactiveComponentsRemaining = 0;
	SANIGroupParseTree* activationPathWordCurrentParseTreeGroupNOTUSED = NULL;
	SANIGroupNeuralNetwork* ownerGroupNOTUSED = NULL;
	bool componentWordConnectivityTestsResultNOTUSED = false;
	bool existingActivationFoundEndComponentNOTUSED = false;
	return propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, testComponent, components, activationPathWordCurrentParseTreeGroupNOTUSED, activationSequenceCompleted, firstActiveComponentInGroup, previousActiveComponent, finalActiveComponent, existingActivationFoundStartComponent, &existingActivationFoundEndComponentNOTUSED, &missingStartComponentsFoundNOTUSED, &missingOrVariableStartComponentFoundNOTUSED, &missingOrVariableEndComponentFoundNOTUSED, &numberOfInactiveComponentsRemaining, ownerGroupNOTUSED, componentWordConnectivityTestsResultNOTUSED);
}

bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, bool* activationSequenceCompleted, bool* firstActiveComponentInGroup, SANIComponentNeuralNetwork** previousActiveComponent, SANIComponentNeuralNetwork** finalActiveComponent, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, bool* missingStartComponentFound, bool* missingOrVariableStartComponentFound, bool* missingOrVariableEndComponentFound, int* numberOfInactiveComponentsRemaining, SANIGroupNeuralNetwork* ownerGroup, const bool componentWordConnectivityTestsResult)
{	
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	*numberOfInactiveComponentsRemaining = 0;
		
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY
	*previousActiveComponent = NULL;
	#endif
		
	for(int i=0; i<components->size(); i++)
	{		
		int c;
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			c = components->size()-1-i;
		}
		else
		{
			c = i;
		}

		SANIComponentNeuralNetwork* currentComponent = (*components)[c];

		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY
		if(currentComponent->neuronComponentConnectionActive)
		{
			*finalActiveComponent = currentComponent;
		}
		#endif
						
		if(stillParsingActiveComponents)
		{
			if(currentComponent == testComponent)
			{
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY
				if(*previousActiveComponent == NULL)
				{
					*firstActiveComponentInGroup = true;
				}
				#endif
				
				stillParsingActiveComponents = false;

				identifySequentialActivationFound(forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, components, activationPathSequenceElementCurrentParseTreeGroup, i, currentComponent, *previousActiveComponent, &sequentialActivationFound, existingActivationFoundStartComponent, existingActivationFoundEndComponent, ownerGroup, componentWordConnectivityTestsResult);
				//cout << "sequentialActivationFound = " << sequentialActivationFound << endl;
				
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
				SANIpropagateCompactGenerateOperations.identifyMissingOrVariableEndComponentFound(forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, components, activationPathSequenceElementCurrentParseTreeGroup, i, currentComponent, *previousActiveComponent, missingOrVariableEndComponentFound);
				#endif
			}
			else
			{
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY
				if(currentComponent->neuronComponentConnectionActive)
				{
					*previousActiveComponent = currentComponent;
					//cout << "*previousActiveComponent = currentComponent, currentComponent->componentIndex = " << currentComponent->componentIndex << endl;
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
					SANIpropagateCompactGenerateOperations.identifyMissingOrVariableStartComponentFound(forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, components, activationPathSequenceElementCurrentParseTreeGroup, i, currentComponent, *previousActiveComponent, missingOrVariableStartComponentFound);
					#endif
					
					identifyMissingStartComponentFound(forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, components, activationPathSequenceElementCurrentParseTreeGroup, i, currentComponent, *previousActiveComponent, missingStartComponentFound, &stillParsingActiveComponents);
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
							//cout << "1 group not suitable for activation, following component is active: sequentialActivationFound = false" << endl;
						}
						#else
						//group not suitable for activation, following component is active
						sequentialActivationFound = false;
						//cout << "2 group not suitable for activation, following component is active: sequentialActivationFound = false" << endl;
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
		//cout << "(*missingStartComponentFound) && !(*missingOrVariableStartComponentFound)): sequentialActivationFound = false" << endl;
	}
	#endif
	
	if(sequentialActivationFound)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		bool passMissingStartComponentTests = true;
		if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableStartComponent)
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

	//cout << "sequentialActivationFound = " << sequentialActivationFound << endl;
	
	return sequentialActivationFound;
}



bool SANIpropagateOperationsClass::identifySequentialActivationFound(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIComponentNeuralNetwork*>* components, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, SANIGroupNeuralNetwork* ownerGroup, bool componentWordConnectivityTestsResult)
{
	bool result = true;
	
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY_VERIFY_PREVIOUS_COMPONENT_NOT_ACTIVATED_BY_SAME_WORD_DIFFERENT_POS_PROPAGATION
	bool previousComponentWasActivatedBySameWordDifferentPOSpropagation = false;
	if(previousActiveComponent != NULL)
	{
		if((previousActiveComponent)->neuronComponentConnectionActiveSequenceElementRecord == forwardPropogationSequenceElementData)
		{
			previousComponentWasActivatedBySameWordDifferentPOSpropagation = true;
			//cout << "previousComponentWasActivatedBySameWordDifferentPOSpropagation" << endl;
			/*
			cout << "previousActiveComponent->componentIndex = " << previousActiveComponent->componentIndex << endl;
			cout << "forwardPropogationSequenceElementData->sequenceIndex = " << forwardPropogationSequenceElementData->sequenceIndex << endl;
			cout << "forwardPropogationSequenceElementData->wordReference->tagName = " << forwardPropogationSequenceElementData->wordReference->tagName << endl;
			cout << "(previousActiveComponent)->neuronComponentConnectionActiveSequenceElementRecord->tagName = " << (previousActiveComponent)->neuronComponentConnectionActiveSequenceElementRecord->tagName << endl;	
			cout << "(previousActiveComponent)->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex = " << (previousActiveComponent)->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex << endl;	
			*/
		}
	}
	if(!previousComponentWasActivatedBySameWordDifferentPOSpropagation)
	{
	#endif	
				
	if(currentComponent->neuronComponentConnectionActive)
	{	
		//cout << "(currentComponent->neuronComponentConnectionActive)" << endl;
		
		#ifdef SANI_SEQUENCE_GRAMMAR
		SANIpropagateCompactOperations.identifySequentialActivationFound(forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, components, activationPathSequenceElementCurrentParseTreeGroup, i, currentComponent, previousActiveComponent, sequentialActivationFound, existingActivationFoundStartComponent, existingActivationFoundEndComponent, ownerGroup, componentWordConnectivityTestsResult);
		#else		
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
			#ifdef SANI_ALLOW_MULTIPLE_ACTIVATIONS
			*sequentialActivationFound = true;
			*existingActivationFoundStartComponent = true;	
			#endif
		#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
		}
		#endif
		#endif
	}
	else
	{
		*sequentialActivationFound = true;
		//cout << "*sequentialActivationFound" << endl;
	}
	
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY_VERIFY_PREVIOUS_COMPONENT_NOT_ACTIVATED_BY_SAME_WORD_DIFFERENT_POS_PROPAGATION
	}
	#endif
	
	return result;
}

bool SANIpropagateOperationsClass::identifyMissingStartComponentFound(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIComponentNeuralNetwork*>* components, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* missingStartComponentFound, bool* stillParsingActiveComponents)
{
	bool result = true;
	
	if(!(currentComponent->neuronComponentConnectionActive))
	{
		#ifdef SANI_SEQUENCE_GRAMMAR
		SANIpropagateCompactOperations.identifyMissingStartComponentFound(forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, components, activationPathSequenceElementCurrentParseTreeGroup, i, currentComponent, previousActiveComponent, missingStartComponentFound, stillParsingActiveComponents);
		#else
		*stillParsingActiveComponents = false;	
		#endif
	}
	
	return result;		
}



#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
#ifdef SANI_SUPPORT_COMPONENTS_OR					
bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components, bool* existingActivationFoundStartComponent)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	for(int i=0; i<components->size(); i++)
	{
		int c;
		if(forwardPropogationSequenceData->parseSentenceReverse)
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
#endif
#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
//precondition: repeat sequences can contain only 1 component
bool SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components, bool* repeatDetected, const bool* existingActivationFoundStartComponent)
{
	bool sequentialActivationFound = false;
	
	bool stillParsingActiveComponents = true;
	for(int i=0; i<components->size(); i++)
	{
		int c;
		if(forwardPropogationSequenceData->parseSentenceReverse)
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
					int translatorSentenceSequenceIndexComparisonInt;
					if(forwardPropogationSequenceData->parseSentenceReverse)
					{
						translatorSentenceSequenceIndexComparisonInt = forwardPropogationSequenceElementData->sequenceIndex + 1;
					}
					else
					{
						translatorSentenceSequenceIndexComparisonInt = forwardPropogationSequenceElementData->sequenceIndex - 1;
					}
					if(component->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex == translatorSentenceSequenceIndexComparisonInt)
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
					//if(component->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex == forwardPropogationSequenceElementData->sequenceIndex)
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
#endif
#endif



#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
#ifdef SANI_HEAVY
bool SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup)
{
	bool result = false;

	bool existingActivationFoundNOTUSED = false;

	const SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathSequenceElementCurrentParseTreeGroup;
	
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() > 0)
	{
		int lastActiveComponentInParseTreeIndex = activationPathWordCurrentParseTreeGroupOwner->components.size() - 1;
		SANIComponentParseTree* lastActiveComponentInParseTree = (activationPathWordCurrentParseTreeGroupOwner->components)[lastActiveComponentInParseTreeIndex];
		SANIComponent* lastActiveComponentNOTUSED = NULL;
		SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
		
		SANIGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRefNOTUSED = NULL;
		SANIGroupNeuralNetwork* ownerGroupNOTUSED = NULL;
		result = componentWordConnectivityTests(forwardPropogationSequenceElementData, forwardPropogationSequenceData, ownerGroupNOTUSED, prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentNOTUSED, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, previousActiveComponentInParseTreeParseTreeGroupRefNOTUSED, existingActivationFoundNOTUSED);
	}
	else
	{
		result = true;
	}
	
	return result;
}
#else
#ifdef SANI_LIGHT_UNOPTIMISED
bool SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup)
{
	bool existingActivationFoundStartComponent = false;
	bool existingActivationFoundEndComponent = false;
	return componentWordConnectivityTestsWrapper(forwardPropogationSequenceElementData, forwardPropogationSequenceData, ownerGroup, ownerGroup->currentParseTreeGroupTemp, activationPathSequenceElementCurrentParseTreeGroup, existingActivationFoundStartComponent, existingActivationFoundEndComponent);
}
#else
bool SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const bool existingActivationFoundStartComponent)
{
	bool existingActivationFoundEndComponent = false;
	return componentWordConnectivityTestsWrapper(forwardPropogationSequenceElementData, forwardPropogationSequenceData, ownerGroup, ownerGroup->currentParseTreeGroupTemp, activationPathSequenceElementCurrentParseTreeGroup, existingActivationFoundStartComponent, existingActivationFoundEndComponent);
}
#endif
bool SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const bool existingActivationFoundStartComponent, const bool existingActivationFoundEndComponent)
{
	bool result = false;

	const SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathSequenceElementCurrentParseTreeGroup;

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
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_LAST_COMPONENTS_REMEMBER_FIRST_COMPONENT_SEQUENCE_INDEX
	else if(existingActivationFoundEndComponent)
        {
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS		
        	if(ownerGroupParseTreeGroup->components.size() == 2)	//assumes 2 active components in parsetree
		#else
		if(ownerGroupParseTreeGroup->components.size() == ownerGroup->components.size())
		#endif
		{	
			#ifdef SANI_PARSE_SAVE_PARSE_TREE
			int lastActiveComponentInParseTreeIndex;
			if(forwardPropogationSequenceData->parseSentenceReverse)
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
				lastActiveComponentInParseTreeIndex = 1;
				#else
				lastActiveComponentInParseTreeIndex = ownerGroup->components.size()-1;
				#endif
			}
			else
			{
				lastActiveComponentInParseTreeIndex = 0;
			}
			SANIComponentParseTree* lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];
			lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
			//note lastActiveComponentInParseTreeParseTreeGroupRef will be empty in the case of a component->type == STRING
			
			//added GIA3k13a for SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY;
			int lastActiveComponentIndex = lastActiveComponentInParseTree->componentIndex;
			SANIComponent* lastActiveComponent = ownerGroup->components[lastActiveComponentIndex];
			#else
			cerr << "SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper currently requires SANI_PARSE_SAVE_PARSE_TREE" << endl;
			exit(EXIT_ERROR);
			#endif
			
			if(componentWordConnectivityTests(forwardPropogationSequenceElementData, forwardPropogationSequenceData, ownerGroup, prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponent, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, previousActiveComponentInParseTreeParseTreeGroupRef, existingActivationFoundStartComponent))
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
			cerr << "SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_LAST_COMPONENTS_REMEMBER_FIRST_COMPONENT_SEQUENCE_INDEX: SANIpropagateOperationsClass::componentWordConnectivityTestsWrapper error: (ownerGroupParseTreeGroup->components.size() != 2)" << endl;
			cerr << "ownerGroupParseTreeGroup->components.size() = " << ownerGroupParseTreeGroup->components.size() << endl;
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
			
			if(forwardPropogationSequenceData->parseSentenceReverse)
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
			
			//added GIA3k13a for SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY;
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
			
			if(componentWordConnectivityTests(forwardPropogationSequenceElementData, forwardPropogationSequenceData, ownerGroup, prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponent, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, previousActiveComponentInParseTreeParseTreeGroupRef, existingActivationFoundStartComponent))
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

//note if !SANI_PARSE_GENERATE_PARSE_TREE, lastActiveComponentInParseTreeParseTreeGroupRef will be NULL, so must rely on lastActiveComponentInParseTree->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex
bool SANIpropagateOperationsClass::componentWordConnectivityTests(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, SANIComponent* lastActiveComponent, const SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, const SANIComponentParseTree* lastActiveComponentInParseTree, const SANIGroupParseTree* previousActiveComponentInParseTreeParseTreeGroupRef, const bool existingActivationFoundStartComponent)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SEQUENCEINDEX_VERIFICATION
	cout << "SANIpropagateOperationsClass::componentWordConnectivityTests:" << endl;
	#endif
	
	int sequenceIndexMax = -1;
	int sequenceIndexMin = REALLY_LARGE_INT;
	const SANIGroupParseTree* parseTreeGroupToFindSequenceIndexMin;
	const SANIGroupParseTree* parseTreeGroupToFindSequenceIndexMax;
	int sequenceIndexMinMin;
	int sequenceIndexMaxMax;
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		parseTreeGroupToFindSequenceIndexMin = lastActiveComponentInParseTreeParseTreeGroupRef;
		parseTreeGroupToFindSequenceIndexMax = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
		sequenceIndexMinMin = lastActiveComponentInParseTree->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;	//CHECKTHIS
		sequenceIndexMaxMax = forwardPropogationSequenceElementData->sequenceIndex;	//CHECKTHIS
	}
	else
	{
		parseTreeGroupToFindSequenceIndexMin = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
		parseTreeGroupToFindSequenceIndexMax = lastActiveComponentInParseTreeParseTreeGroupRef;
		sequenceIndexMinMin = forwardPropogationSequenceElementData->sequenceIndex;
		sequenceIndexMaxMax = lastActiveComponentInParseTree->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
				
		//cout << "sequenceIndexMinMin (forwardPropogationSequenceElementData->sequenceIndex) = " << sequenceIndexMinMin << endl;
		//cout << "sequenceIndexMaxMax (lastActiveComponentInParseTree->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex) = " << sequenceIndexMaxMax << endl;
	}

	if(parseTreeGroupToFindSequenceIndexMax != NULL)
	{
		//cout << "parseTreeGroupToFindSequenceIndexMax->parseTreeMinSequenceIndex (lastActiveComponentInParseTreeParseTreeGroupRef) = " << parseTreeGroupToFindSequenceIndexMax->parseTreeMinSequenceIndex << endl;		
		//cout << "parseTreeGroupToFindSequenceIndexMax->parseTreeMaxSequenceIndex (lastActiveComponentInParseTreeParseTreeGroupRef) = " << parseTreeGroupToFindSequenceIndexMax->parseTreeMaxSequenceIndex << endl;
	
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SEQUENCEINDEX_VERIFICATION
		cout << "parseTreeGroupToFindSequenceIndexMax != NULL" << endl;
		cout << "parseTreeGroupToFindSequenceIndexMax->groupRef->groupIndex = " << parseTreeGroupToFindSequenceIndexMax->groupRef->groupIndex << endl;
		#endif
		if(!SANIpropagateOperationsParseTree.getMinMaxSequenceIndexInParseTree(parseTreeGroupToFindSequenceIndexMax, true, &sequenceIndexMax, 0))
		{
		}
	}
	sequenceIndexMax = SHAREDvars.maxInt(sequenceIndexMax, sequenceIndexMaxMax);

	if(parseTreeGroupToFindSequenceIndexMin != NULL)
	{
		//cout << "parseTreeGroupToFindSequenceIndexMin->parseTreeMinSequenceIndex (prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef) = " << parseTreeGroupToFindSequenceIndexMin->parseTreeMinSequenceIndex << endl;
		//cout << "parseTreeGroupToFindSequenceIndexMin->parseTreeMaxSequenceIndex (prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef) = " << parseTreeGroupToFindSequenceIndexMin->parseTreeMaxSequenceIndex << endl;

		if(!SANIpropagateOperationsParseTree.getMinMaxSequenceIndexInParseTree(parseTreeGroupToFindSequenceIndexMin, false, &sequenceIndexMin, 0))
		{
		}
	}
	sequenceIndexMin = SHAREDvars.minInt(sequenceIndexMin, sequenceIndexMinMin);

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SEQUENCEINDEX_VERIFICATION
	cout << "ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;	
	cout << "sequenceIndexMax = " << sequenceIndexMax << endl;
	cout << "sequenceIndexMin = " << sequenceIndexMin << endl;
	//cout << "sequenceIndexMaxMax = " << sequenceIndexMaxMax << endl;
	//cout << "sequenceIndexMinMin = " << sequenceIndexMinMin << endl;
	cout << "existingActivationFoundStartComponent = " << existingActivationFoundStartComponent << endl;
	#endif
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	int sequenceIndexProspectiveFirst;
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		sequenceIndexProspectiveFirst = sequenceIndexMax;
	}
	else
	{
		sequenceIndexProspectiveFirst = sequenceIndexMin;
	}
	if(SANIpropagateOperationsParseTree.componentWordConnectivityTestsCompareToMemory(forwardPropogationSequenceData, sequenceIndexProspectiveFirst, ownerGroup, lastActiveComponent))	//OR: (sequenceIndexProspectiveFirst, [!stringComponent || SANI_PARSE_GENERATE_PARSE_TREE: lastActiveComponentInParseTreeParseTreeGroupRef->groupRef,] lastActiveComponentInParseTree->componentRef)
	#else
	if(sequenceIndexMin-sequenceIndexMax == 1)
	#endif
	{
		result = true;
	}
	#ifndef SANI_SEQUENCE_GRAMMAR
	#ifdef SANI_ALLOW_MULTIPLE_ACTIVATIONS
	else if(existingActivationFoundStartComponent)
	{
		//assumes SANI_REVERSE_DIRECTION:
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC_STRICT_MUTUALLY_EXCLUSIVE_BIO
		if(previousActiveComponentInParseTreeParseTreeGroupRef != NULL)
		{
			int sequenceIndexMinPrevious = previousActiveComponentInParseTreeParseTreeGroupRef->parseTreeMinSequenceIndex;
			if(sequenceIndexMinPrevious-sequenceIndexMax == 1)
			{
				result = true;
			}
		}
		else
		{
			if(sequenceIndexMin <= sequenceIndexMax)
			{
				result = true;
			}
		}
		#else
		if(sequenceIndexMin <= sequenceIndexMax)
		{
			result = true;
		}
		#endif
	}
	#endif
	#endif

	return result;
}

#endif





								
								
#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC_STRICT
bool SANIpropagateOperationsClass::isSequenceWordDataFullyConnected(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	//forwardPropogationSequenceElementData->previousSequenceElementConnections.push_back(wPrevious);
	bool result = true;
	
	int numberOfWords = forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray.size();
	vector<bool> previousWordConnectionsFound(numberOfWords, false);
	previousWordConnectionsFound[numberOfWords-1] = true;	//set last sequenceElement as found 
	SANIForwardPropogationSequenceElementData* lastWordData = ((forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray)[numberOfWords-1]);
	isSequenceWordDataFullyConnected(&previousWordConnectionsFound, lastWordData);
	for(int w = 0; w<previousWordConnectionsFound.size(); w++)
	{
		if(!previousWordConnectionsFound[w])
		{
			result = false;
		}
	}
	
	return result;
}

void SANIpropagateOperationsClass::isSequenceWordDataFullyConnected(vector<bool>* previousWordConnectionsFound, const SANIForwardPropogationSequenceElementData* currentSequenceElementData)
{
	for(int i = 0; i<currentSequenceElementData->previousSequenceElementConnections.size(); i++)
	{
		SANIForwardPropogationSequenceElementData* previouSequenceIndexData = (currentSequenceElementData->previousSequenceElementConnections)[i];
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
		cout << "previousWordConnectionsFound == true: previouSequenceIndexData->sequenceIndex = " << previouSequenceIndexData->sequenceIndex << endl;
		#endif
		(*previousWordConnectionsFound)[previouSequenceIndexData->sequenceIndex] = true;
		isSequenceWordDataFullyConnected(previousWordConnectionsFound, previouSequenceIndexData);
	}
}
#endif


#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE

#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
bool SANIpropagateOperationsClass::componentTests1(const SANIComponentNeuralNetwork* currentComponent, const SANIGroupNeuralNetwork* group, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	bool componentTests = true;
	
	if(currentComponent->wordNounVariantType != LRP_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
	{
		componentTests = false;
		if(forwardPropogationSignalData->wordNounVariantType == currentComponent->wordNounVariantType)
		{
			componentTests = true;
		}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_DETECT_IRREGULAR_NOUN_FORMS
		else if(forwardPropogationSignalData->wordNounVariantType == LRP_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_SINGULAR_OR_PLURAL)	
		{
			componentTests = true;
		}
		#endif
	}
	if(currentComponent->wordVerbVariantType != LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
	{
		componentTests = false;
		if(forwardPropogationSignalData->wordVerbVariantType == currentComponent->wordVerbVariantType)
		{
			componentTests = true;
		}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_INTERPRET_PAST_AND_PAST_PARTICIPLE_THE_SAME
		else if((forwardPropogationSignalData->wordVerbVariantType == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST) && (currentComponent->wordVerbVariantType == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE))
		{
			componentTests = true;
		}
		else if((forwardPropogationSignalData->wordVerbVariantType == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE) && (currentComponent->wordVerbVariantType == LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST))
		{
			componentTests = true;
		}
		#endif
	}
	
	return componentTests;
}
#endif

#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
bool SANIpropagateOperationsClass::componentTests2(const SANIGroupNeuralNetwork* group, const SANIGroupParseTree* currentParseTreeGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	bool componentTests = true;
	
	/*
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
	Algorithm:
	#else
	Algorithm:
	previousWordPOStype/existsPreceedingWordPOStype requirement: there exists a previous sequenceElement POStype that was succesfully parsed by a higher order neuron/group.
		Question: so how will the program know this until the higher order neuron group has been parsed?
		Conclusion: will need to;
			a) record the sequenceElementPos type for every activated component in a group
			b) read both;
				i) previous components in current group and 
				ii) read up the tree to see if this condition is met (ie there exists a previous sequenceElement POStype that was succesfully parsed by a higher order neuron/group)
	#endif
	*/
	if(group->previousWordPOStype != "")
	{
		componentTests = false;
		int previousWordPOStype = LRPpreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype);
		int translatorSentenceSequenceIndexMin = REALLY_LARGE_INT;
		LRPpreprocessorPlainTextWord* firstWordInCurrentParseTreeGroupParseTree = NULL;
		int numComponentsFound = 0;
		bool pass = false;
		if(!getFirstWordInParseTree(currentParseTreeGroup, &firstWordInCurrentParseTreeGroupParseTree, &translatorSentenceSequenceIndexMin, &numComponentsFound))
		{
			firstWordInCurrentParseTreeGroupParseTree = forwardPropogationSequenceElementData->wordReference;
		}
		else
		{
			pass = true;
		}
		
		if(findPreviousWordInSentence(forwardPropogationSequenceData->sentenceContents, firstWordInCurrentParseTreeGroupParseTree, previousWordPOStype))		
		{
			componentTests = true;
		}
	}
	if(group->existsPreceedingWordPOStype != "")
	{
		componentTests = false;
		int existsPreceedingWordPOStype = LRPpreprocessorWordClassObject.getPOStypeFromName(group->existsPreceedingWordPOStype);
		int translatorSentenceSequenceIndexMin = REALLY_LARGE_INT;
		LRPpreprocessorPlainTextWord* firstWordInCurrentParseTreeGroupParseTree = NULL;
		int numComponentsFound = 0;
		if(!getFirstWordInParseTree(currentParseTreeGroup, &firstWordInCurrentParseTreeGroupParseTree, &translatorSentenceSequenceIndexMin, &numComponentsFound))
		{
			firstWordInCurrentParseTreeGroupParseTree = forwardPropogationSequenceElementData->wordReference;
		}
		else
		{
		
		}
		
		if(findPreceedingWordInSentence(forwardPropogationSequenceData->sentenceContents, firstWordInCurrentParseTreeGroupParseTree, existsPreceedingWordPOStype))		
		{
			componentTests = true;
		}
	}
	
	return componentTests;
}

bool SANIpropagateOperationsClass::getFirstWordInParseTree(const SANIGroupParseTree* currentParseTreeGroup, LRPpreprocessorPlainTextWord** firstWordInCurrentParseTreeGroupParseTree, int* translatorSentenceSequenceIndexMin, int* numComponentsFound)
{
	bool result = false;
		
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		//if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		//{
			*numComponentsFound = *numComponentsFound + 1;
		//}
		
		int sequenceIndex = currentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
		if(sequenceIndex < *translatorSentenceSequenceIndexMin)
		{
			*translatorSentenceSequenceIndexMin = sequenceIndex;
			*firstWordInCurrentParseTreeGroupParseTree = currentComponent->neuronComponentConnectionActiveSequenceElementRecord->wordReference;
			result = true;
		}
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{							
			if(getFirstWordInParseTree(currentComponent->parseTreeGroupRef, firstWordInCurrentParseTreeGroupParseTree, translatorSentenceSequenceIndexMin, numComponentsFound))
			{
				result = true;
			}
		}
	}
	
	return result;
}

#ifndef SANI_SEQUENCE_GRAMMAR
bool SANIpropagateOperationsClass::findPreviousWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const LRPpreprocessorPlainTextWord* currentWord, const int previousWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	LRPpreprocessorPlainTextWord* previousComponentWord = NULL;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		LRPpreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
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
bool SANIpropagateOperationsClass::findPreceedingWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const LRPpreprocessorPlainTextWord* currentWord, const int existsPreceedingWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		LRPpreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
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

bool SANIpropagateOperationsClass::checkWordPOStype(const LRPpreprocessorPlainTextWord* currentWord, const int unambiguousPOSindex)
{
	bool result = false;	
	#ifdef SANI_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == unambiguousPOSindex)
	{
		result = true;
	}
	#else
	if(!SANInodes.currentWordPOSunknown(currentWord))
	{
		if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, unambiguousPOSindex))
		{
			result = true;
		}
	}
	#endif
	return result;
}
#endif

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
				
				SANIpropagateOperationsParseTree.resetGroupParseTreeGroupRef(group, true);
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
			#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_SEQUENCEELEMENT_BASIC
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
bool SANIpropagateOperationsClass::isLastComponentFuzzy(const SANIGroupNeuralNetwork* group)
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

bool SANIpropagateOperationsClass::resetComponentsActivation(const vector<SANIComponentNeuralNetwork*>* components)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		SANIComponentNeuralNetwork* currentComponent = (*components)[i1];
		
		#ifdef SANI_RESET_COMPONENT_NEURON_COMPONENT_CONNECTION_ACTIVE_SEQUENCEELEMENT_RECORD
		currentComponent->neuronComponentConnectionActiveSequenceElementRecord = NULL;
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

bool SANIpropagateOperationsClass::restoreComponentsActivation(const vector<SANIComponentNeuralNetwork*>* components, const vector<SANIComponentNeuralNetwork*>* componentsOrig)
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



#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
bool SANIpropagateOperationsClass::resetNeuronForwardProp(const SANIGroupNeuralNetwork* group, const int groupBoolIndexType)
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
		for(int i=0; i<group->SANIfrontComponentConnectionList.size(); i++)
		{
			SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i];
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


			


bool SANIpropagateOperationsClass::doesRequireResetGroupActivation(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIComponentNeuralNetwork* Xcomponent, const int indexOfXcomponentInGroup, const SANIGroupNeuralNetwork* ownerGroup, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData)
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
					int translatorSentenceSequenceIndexComparisonInt;
					if(forwardPropogationSequenceData->parseSentenceReverse)
					{
						translatorSentenceSequenceIndexComparisonInt = forwardPropogationSequenceElementData->sequenceIndex + 1;
					}
					else
					{
						translatorSentenceSequenceIndexComparisonInt = forwardPropogationSequenceElementData->sequenceIndex - 1;
					}
					
					if(subComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex != translatorSentenceSequenceIndexComparisonInt)
					{
						requireResetGroupActivation = true;
					}
				}
				else
				{
					if(findNextAvailableComponent(forwardPropogationSequenceData, indexOfXcomponentInGroup, ownerGroup))
					{
						requireResetGroupActivation = true;
					}
					else
					{
						//FUTURE: what if still need to reset however; what if it was activated incorrectly, or already activated from a previous successful parse (e.g. <group groupName="nounMultisequenceElement" semanticRelationFunctionName="multisequenceElementNoun">)?
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

					

bool SANIpropagateOperationsClass::findNextAvailableComponent(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int indexOfXcomponentInGroup, const SANIGroupNeuralNetwork* ownerGroup)
{
	bool foundNextAvailableComponent = false;
	bool findingNextAvailableComponent = true;
	
	for(int i=indexOfXcomponentInGroup; i<ownerGroup->components.size(); i++)
	{
		int i2;
		if(forwardPropogationSequenceData->parseSentenceReverse)
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
			if(forwardPropogationSequenceData->parseSentenceReverse)
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


