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
 * File Name: SANIpropagateCompactOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1q2a 19-September-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Compact Operations - generic functions
 * /
 *******************************************************************************/


#include "SANIpropagateCompactOperations.hpp"


#ifdef SANI_SEQUENCE_GRAMMAR

bool SANIpropagateCompactOperationsClass::identifySequentialActivationFound(const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent, bool* existingActivationFoundEndComponent, SANIGroupNeuralNetwork* ownerGroup, const bool componentWordConnectivityTestsResult)
{
	bool result = true;
	
	//can be depreciated;
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	bool previousComponentWasActivatedBySameWordDifferentPOSpropagation = false;
	if(previousActiveComponent != NULL)
	{
		if((previousActiveComponent)->neuronComponentConnectionActiveSequenceElementRecord == forwardPropogationSequenceElementData)
		{
			previousComponentWasActivatedBySameWordDifferentPOSpropagation = true;
		}
	}
	if(!previousComponentWasActivatedBySameWordDifferentPOSpropagation)
	{
	#endif		
	#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_SEQUENCE_INDEX_OF_EXISTING_COMPONENT
	bool upperNeuronContainsSequenceIndexOfProspectiveComponentTest = false;
	if(!(currentComponent->ownerGroup->neuronActive))
	{
		if(upperNeuronContainsSequenceIndexOfProspectiveComponent(forwardPropogationSequenceData, forwardPropogationSequenceElementData, currentComponent, currentComponent->ownerGroup))
		{
			upperNeuronContainsSequenceIndexOfProspectiveComponentTest = true;
		}
	}
	if(!upperNeuronContainsSequenceIndexOfProspectiveComponentTest)
	{
	#endif
		//can be depreciated;
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_ACTIVATION_IF_REPEATED_SEQUENCE_MISMATCH_DETECTED
		bool allowFirstComponentActivation = true;
		if(i == 0)	//start component in group
		{
			int sequenceElementTranslatorSentenceSequenceIndex = forwardPropogationSequenceElementData->sequenceIndex;
			//if repeated POS instance detected in sentence starting from forwardPropogationSequenceElementData, and first section [lowest level input] of second component is not equivalent to same input group as first component, then disallow activation[/reactivation] of first component
			if(repeatedSequenceDetected(forwardPropogationSequenceData, forwardPropogationSequenceElementData, activationPathSequenceElementCurrentParseTreeGroup, sequenceElementTranslatorSentenceSequenceIndex))
			{
				if(!findValidDualLowerLevelConnectionAlternate(forwardPropogationSequenceData, components, currentComponent))
				{
					allowFirstComponentActivation = false;
					//cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_ACTIVATION_IF_REPEATED_SEQUENCE_MISMATCH_DETECTED: repeatedSequenceDetected && !findValidDualLowerLevelConnectionAlternate: !allowComponentActivation" << endl;
				}
			}
			//if repeated POS instance not detected in sentence starting from forwardPropogationSequenceElementData, and first section [lowest level input] of second component is equivalent to same input group as first component, then disallow activation[/reactivation] of first component 
			if(!repeatedSequenceDetected(forwardPropogationSequenceData, forwardPropogationSequenceElementData, activationPathSequenceElementCurrentParseTreeGroup, sequenceElementTranslatorSentenceSequenceIndex))
			{
				if(findValidDualLowerLevelConnectionAlternate(forwardPropogationSequenceData, components, currentComponent))
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
					#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_LAST_COMPONENTS_REMEMBER_FIRST_COMPONENT_SEQUENCE_INDEX
					int secondLastComponentIndex;
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
					secondLastComponentIndex = 0;
					#else
					//TODO: ensure neuron minimum number of components >= 2
					if(forwardPropogationSequenceData->parseSentenceReverse)
					{
						secondLastComponentIndex = 1;
					}
					else
					{
						secondLastComponentIndex = components->size()-2;
					}
					#endif
					if(i != secondLastComponentIndex)
					{
						if(previousActiveComponent != NULL)	//alternate test does not support !SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS: if(!(*missingStartComponentFound))
						{
							*sequentialActivationFound = true;
							*existingActivationFoundEndComponent = true;
							
							//cout << "existingActivationFoundEndComponent" << endl;

							if(ownerGroup->currentParseTreeGroupTemp == NULL)
							{
								cerr << "SANIpropagateCompactOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady error: (ownerGroup->currentParseTreeGroupTemp == NULL)" << endl;
								exit(EXIT_ERROR);
							}
							else
							{
								if(ownerGroup->currentParseTreeGroupTemp->components.size() == 0)
								{
									cerr << "SANIpropagateCompactOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady error: (ownerGroup->currentParseTreeGroupTemp->components.size() == 0)" << endl;
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
						int sequenceElementTranslatorSentenceSequenceIndex = currentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
						if(findValidDualLowerLevelConnection(forwardPropogationSequenceData, components, currentComponent, true))
						{
							if(repeatedSequenceDetected(forwardPropogationSequenceData, forwardPropogationSequenceElementData, currentComponent->ownerGroup->currentParseTreeGroupTemp, sequenceElementTranslatorSentenceSequenceIndex))
							{
								#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED_AND_SEQUENCE_INDEX_NOT_ALREADY_ENCAPSULATED_BY_HIGHER_LEVEL_GROUP_PREVIOUS_COMPONENT
								if(!upperNeuronContainsSequenceIndexOfProspectiveComponent(forwardPropogationSequenceData, currentComponent->neuronComponentConnectionActiveSequenceElementRecord, currentComponent, currentComponent->ownerGroup))
								{
								#endif
									allowFirstComponentReset = false;
									//cout << "1allowFirstComponentReset = false;" << endl;
								#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED_AND_SEQUENCE_INDEX_NOT_ALREADY_ENCAPSULATED_BY_HIGHER_LEVEL_GROUP_PREVIOUS_COMPONENT
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
							if(consecutiveSequenceDetected(forwardPropogationSequenceData, forwardPropogationSequenceElementData, components, currentComponent))
							{
								//cout << "!allowFirstComponentReset: consecutiveSequenceDetected - currentComponent->ownerGroup->groupIndex = " << currentComponent->ownerGroup->groupIndex << endl;
								//cout << "!allowFirstComponentReset; consecutiveSequenceDetected" << endl;
								allowFirstComponentReset = false;
								//cout << "2allowFirstComponentReset = false;" << endl;
							}
							#endif
							#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF
							if(findValidDualLowerLevelConnection(forwardPropogationSequenceData, components, currentComponent, true))
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
							//always reset, as existing first activated component is not connected (by sequenceElementIndices) to prospective activated next component
						}
						#endif
						#endif
						/*//alternate method;
						#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED_AND_SEQUENCE_INDEX_NOT_ALREADY_ENCAPSULATED_BY_HIGHER_LEVEL_GROUP_PREVIOUS_COMPONENT
						if(currentComponent->neuronComponentConnectionActive)
						{
							if(upperNeuronContainsSequenceIndexOfProspectiveComponent(forwardPropogationSequenceData, currentComponent->neuronComponentConnectionActiveSequenceElementRecord, currentComponent, currentComponent->ownerGroup))
							{
								allowFirstComponentReset = true;
							}
						}
						#endif
						*/						
						/*
						#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_SEQUENCE_INDEX_OF_EXISTING_COMPONENT
						if(!(currentComponent->ownerGroup->neuronComponentConnectionActive))	//?
						{
							if(upperNeuronContainsSequenceIndexOfProspectiveComponent(forwardPropogationSequenceData, forwardPropogationSequenceElementData->sequenceElementRecord, currentComponent, currentComponent->ownerGroup))
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
								if(currentComponent->neuronComponentConnectionActiveSequenceElementRecord == forwardPropogationSequenceElementData)
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
	#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_WRITE_IF_UPPER_NEURON_ALREADY_CONTAINS_SEQUENCE_INDEX_OF_EXISTING_COMPONENT
	}
	#endif
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	}
	#endif
	
	return result;
}

bool SANIpropagateCompactOperationsClass::identifyMissingStartComponentFound(const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int i, const SANIComponentNeuralNetwork* currentComponent, const SANIComponentNeuralNetwork* previousActiveComponent, bool* missingStartComponentFound, bool* stillParsingActiveComponents)
{
	bool result = true;
	
	if(!(currentComponent->neuronComponentConnectionActive))
	{

		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_MISSING
		if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableStartComponent)
		{							
			if(previousActiveComponent == NULL)
			{
				//support activation of group components with missing start components
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING_COMPREHENSIVE
				if(!SANInodes.hasComponentTypeString(currentComponent)) //(currentComponent->componentType != GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
				#endif
					*missingStartComponentFound = true;
					//cout << "2missingStartComponentFound" << endl;
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING_COMPREHENSIVE
				}
				else
				{
					//cout << "SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING_COMPREHENSIVE: (currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)" << endl;
					*stillParsingActiveComponents = false;	
				}
				#endif

			}
			else
			{	
				if((i == 0) && (!SANInodes.hasComponentTypeString(currentComponent)))	//(currentComponent->componentType != GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
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



#ifdef SANI_SEQUENCE_GRAMMAR
bool SANIpropagateCompactOperationsClass::upperNeuronContainsSequenceIndexOfProspectiveComponent(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIForwardPropogationSequenceElementData* neuronComponentConnectionActiveSequenceElementRecord, const SANIComponentNeuralNetwork* component, const SANIGroupNeuralNetwork* group)
{
	bool result = false;
	
	for(int i=0; i<group->SANIfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i];
		SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
		//cout << "SANIpropagateCompactOperationsClass::upperNeuronContainsSequenceIndexOfProspectiveComponent: ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
	
		//method2;
		SANIComponentNeuralNetwork* previousComponent = NULL;
		int c = currentComponent->componentIndex;
		int cPrevious;
		bool foundPreviousComponent = false;
		if(forwardPropogationSequenceData->parseSentenceReverse)
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
				if(previousComponent->neuronComponentConnectionActiveSequenceElementRecord == neuronComponentConnectionActiveSequenceElementRecord)
				{
					result = true;
					//cout << "upperNeuronContainsSequenceIndexOfProspectiveComponent - forwardPropogationSequenceElementData has already been propagated to an upper group (prevent usage duplication)" << endl;
					//cout << "previousComponent->neuronComponentConnectionActiveSequenceElementRecord->w = " << previousComponent->neuronComponentConnectionActiveSequenceElementRecord->w << endl;
					//cout << "neuronComponentConnectionActiveSequenceElementRecord->w = " << neuronComponentConnectionActiveSequenceElementRecord->w << endl;
					//exit(EXIT_ERROR);
				}
			}
		}
			
		/*
		//method1;
		SANIGroupParseTree* ownerGroupParseTree = ownerGroup->currentParseTreeGroupTemp;
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			if(ownerGroupParseTree->currentParseTreeGroupTemp->parseTreeMinSequenceIndex == neuronComponentConnectionActiveSequenceElementRecord->w)	//<=	//component->neuronComponentConnectionActiveSequenceElementRecord->w
			{
				result = true;
				cout << "upperNeuronContainsSequenceIndexOfProspectiveComponent 1 - forwardPropogationSequenceElementData has already been propagated to an upper group (prevent usage duplication)" << endl;
			}
		}
		else
		{
			if(ownerGroupParseTree->currentParseTreeGroupTemp->parseTreeMaxSequenceIndex == neuronComponentConnectionActiveSequenceElementRecord->w)	//>=
			{	
				result = true;
				cout << "upperNeuronContainsSequenceIndexOfProspectiveComponent 2 - forwardPropogationSequenceElementData has already been propagated to an upper group (prevent usage duplication)" << endl;
			}
		}
		*/

		if(upperNeuronContainsSequenceIndexOfProspectiveComponent(forwardPropogationSequenceData, neuronComponentConnectionActiveSequenceElementRecord, component, ownerGroup))
		{
			result = true;
		}	

	}
							
	return result;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_RESET_IF_REPEATED_SEQUENCE_DETECTED
//assumes component refers to current component [forwardPropogationSequenceElementData->sequenceIndex is last sequenceElement index in this component], and testing against next component in group
bool SANIpropagateCompactOperationsClass::repeatedSequenceDetected(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIGroupParseTree* currentParseTreeGroupTemp, const int sequenceElementTranslatorSentenceSequenceIndex)
{
	bool result = false;
	
	int componentSequenceIndexCoverage = 0;
	
	if(currentParseTreeGroupTemp != NULL)
	{
		componentSequenceIndexCoverage = currentParseTreeGroupTemp->parseTreeMaxSequenceIndex - currentParseTreeGroupTemp->parseTreeMinSequenceIndex + 1;
	}
	else
	{
		componentSequenceIndexCoverage = 1;	//groupType=string
	}

	vector<LRPpreprocessorPlainTextWord*> componentWordArray1;
	vector<LRPpreprocessorPlainTextWord*> componentWordArray2;

	int componentWmin1 = INT_DEFAULT_VALUE;
	int componentWmax1 = INT_DEFAULT_VALUE;
	int componentWmin2 = INT_DEFAULT_VALUE;
	int componentWmax2 = INT_DEFAULT_VALUE;
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		componentWmax1 = sequenceElementTranslatorSentenceSequenceIndex+(componentSequenceIndexCoverage-1);
		componentWmin1 = sequenceElementTranslatorSentenceSequenceIndex;
		componentWmax2 = sequenceElementTranslatorSentenceSequenceIndex-1;
		componentWmin2 = sequenceElementTranslatorSentenceSequenceIndex-1-(componentSequenceIndexCoverage-1);
	}
	else
	{
		componentWmin1 = sequenceElementTranslatorSentenceSequenceIndex-(componentSequenceIndexCoverage-1);
		componentWmax1 = sequenceElementTranslatorSentenceSequenceIndex;
		componentWmin2 = sequenceElementTranslatorSentenceSequenceIndex+1;
		componentWmax2 = sequenceElementTranslatorSentenceSequenceIndex+1+(componentSequenceIndexCoverage-1);
	}
	
	if(((componentWmin1 >= 0) && (componentWmin2 >= 0)) && ((componentWmax1 < forwardPropogationSequenceData->sentenceContents->size()) && (componentWmax2 < forwardPropogationSequenceData->sentenceContents->size())))
	{
		result = true;
		
		for(int w = componentWmin1; w<=componentWmax1; w++)
		{
			LRPpreprocessorPlainTextWord* currentWord = (*(forwardPropogationSequenceData->sentenceContents))[w];	
			componentWordArray1.push_back(currentWord);
		}
		for(int w = componentWmin2; w<=componentWmax2; w++)
		{
			LRPpreprocessorPlainTextWord* currentWord = (*(forwardPropogationSequenceData->sentenceContents))[w];	
			componentWordArray2.push_back(currentWord);
		}
		for(int i=0; i<componentWmax1-componentWmin1+1; i++)
		{
			//must sync code with SANIpropagateCompactClass::propagateWordThroughNetworkIntro !currentWordPOSunknown exceptions
			
			#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
			if(componentWordArray1[i]->tagName != componentWordArray2[i]->tagName)
			{
				result = false;
			}
			#else
			if(!SANInodes.currentWordPOSunknown(componentWordArray1[i]) && !SANInodes.currentWordPOSunknown(componentWordArray2[i]))
			{
				if(componentWordArray1[i]->unambiguousPOSindex != componentWordArray2[i]->unambiguousPOSindex)
				{
					result = false;
				}
			}
			else
			{				
				#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
				if(SANInodes.currentWordPOSunknown(componentWordArray1[i]))
				{	
					int sequenceElementPOStype1 = INT_DEFAULT_VALUE;
					bool pass = LRPpreprocessorPOStagger.getWordPOStypeFromWordPOSunknown(componentWordArray1[i], &sequenceElementPOStype1);
					if(pass)
					{
						if(componentWordArray2[i]->unambiguousPOSindex != sequenceElementPOStype1)
						{
							result = false;
						}
					}
					else
					{
						result = false;
					}
				}		
				else if(SANInodes.currentWordPOSunknown(componentWordArray2[i]))
				{
					int sequenceElementPOStype2 = INT_DEFAULT_VALUE;
					bool pass = LRPpreprocessorPOStagger.getWordPOStypeFromWordPOSunknown(componentWordArray2[i], &sequenceElementPOStype2);
					if(pass)
					{
						if(componentWordArray1[i]->unambiguousPOSindex != sequenceElementPOStype2)
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
					//both sequenceElements are ambiguous

					int sequenceElementPOStype1 = INT_DEFAULT_VALUE;
					bool pass1 = LRPpreprocessorPOStagger.getWordPOStypeFromWordPOSunknown(componentWordArray1[i], &sequenceElementPOStype1);
					int sequenceElementPOStype2 = INT_DEFAULT_VALUE;
					bool pass2 = LRPpreprocessorPOStagger.getWordPOStypeFromWordPOSunknown(componentWordArray2[i], &sequenceElementPOStype2);
					if(pass1 && pass2)
					{
						if(sequenceElementPOStype1 != sequenceElementPOStype2)
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


#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_NEXT_SEQUENCE_IS_SAME_AS_CURRENT_SEQUENCE
bool SANIpropagateCompactOperationsClass::consecutiveSequenceDetected(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const vector<SANIComponentNeuralNetwork*>* components, const SANIComponentNeuralNetwork* component)
{
	bool result = false;
	
	if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)
	{			
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			if(component->ownerGroup->currentParseTreeGroupTemp->parseTreeMinSequenceIndex == forwardPropogationSequenceElementData->sequenceIndex+1)
			{
				//group is being repeated:
				result = true;
				//cout << "consecutiveSequenceDetected 1" << endl;
			}
		}
		else
		{
			if(component->ownerGroup->currentParseTreeGroupTemp->parseTreeMaxSequenceIndex == forwardPropogationSequenceElementData->sequenceIndex-1)
			{	
				//group is being repeated:
				result = true;
				//cout << "consecutiveSequenceDetected 2" << endl;
			}
		}
	}
	else
	{
		//cout << "SANIpropagateCompactOperationsClass::consecutiveSequenceDetected warning: !(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)" << endl;
	}
							
	return result;
}
#endif



#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_PREVENT_ACTIVATION_IF_REPEATED_SEQUENCE_MISMATCH_DETECTED
bool SANIpropagateCompactOperationsClass::findValidDualLowerLevelConnectionAlternate(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIComponentNeuralNetwork* component)
{	
	bool validDualLowerLevelConnectionFound = false;
	bool secondComponentFound = false;
	bool firstComponentFound = false;
	for(int i2=0; i2<components->size(); i2++)
	{
		int c2;
		if(forwardPropogationSequenceData->parseSentenceReverse)
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
				for(int l=0; l<component->SANIbackGroupConnectionList.size(); l++)
				{
					const SANIGroupNeuralNetwork* groupSource = component->SANIbackGroupConnectionList[l];

					for(int l2=0; l2<component2->SANIbackGroupConnectionList.size(); l2++)
					{
						SANIGroupNeuralNetwork* groupSource2 = component2->SANIbackGroupConnectionList[l2];

						if(findGroup1InFirstSectionOfGroup2BackConnection(forwardPropogationSequenceData, groupSource2, groupSource))
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

bool SANIpropagateCompactOperationsClass::findGroup1InFirstSectionOfGroup2BackConnection(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* currentGroup2, const SANIGroupNeuralNetwork* group1ToFound)
{
	bool result = false;
	
	if(currentGroup2 == group1ToFound)
	{
		result = true;	
	}
	
	if(currentGroup2->components.size() > 0)
	{
		int c2;
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			c2 = currentGroup2->components.size()-1;
		}
		else
		{
			c2 = 0;
		}
		SANIComponentNeuralNetwork* component2 = (currentGroup2->components)[c2];

		for(int l2=0; l2<component2->SANIbackGroupConnectionList.size(); l2++)
		{
			SANIGroupNeuralNetwork* groupSource2 = component2->SANIbackGroupConnectionList[l2];
			if(findGroup1InFirstSectionOfGroup2BackConnection(forwardPropogationSequenceData, groupSource2, group1ToFound))
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
bool SANIpropagateCompactOperationsClass::findValidDualLowerLevelConnection(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIComponentNeuralNetwork* component, const bool assumeFirstComponentActive)
{	
	bool validDualLowerLevelConnectionFound = false;
	bool secondComponentFound = false;
	bool firstComponentFound = false;
	for(int i2=0; i2<components->size(); i2++)
	{
		int c2;
		if(forwardPropogationSequenceData->parseSentenceReverse)
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
					for(int l2=0; l2<component2->SANIbackGroupConnectionList.size(); l2++)
					{
						SANIGroupNeuralNetwork* groupSource2 = component2->SANIbackGroupConnectionList[l2];

						for(int l=0; l<component->SANIbackGroupConnectionList.size(); l++)
						{
							const SANIGroupNeuralNetwork* groupSource = component->SANIbackGroupConnectionList[l];

							if(groupSource2 == groupSource)
							{
								#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_AND_SATISIFIES_SEQUENCE_INDEX
								if(component->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex == forwardPropogationSequenceElementData->sequenceIndex-1)	//or component->sequenceIndex
								{
								#endif
									validDualLowerLevelConnectionFound = true;
									//cout << "validDualLowerLevelConnectionFound" << endl;
									//cout << "\n\n\n\n SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_RECURSIVE: SANIpropagateCompactOperationsClass::findValidDualLowerLevelConnection validDualLowerLevelConnectionFound" << endl;
									//exit(EXIT_ERROR);
								#ifdef SANI_SEQUENCE_GRAMMAR_PREVENT_RESET_IF_FIRST_INACTIVE_COMPONENT_GROUPREF_IS_SAME_AS_FUTURE_ACTIVE_COMPONENT_GROUPREF_AND_SATISIFIES_SEQUENCE_INDEX
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
bool SANIpropagateCompactOperationsClass::findGroup2InForwardConnectionBranchOfGroup1(const SANIGroupNeuralNetwork* group2, const SANIGroupNeuralNetwork* currentGroup)
{
	bool result = false;
	
	for(int i=0; i<currentGroup->SANIfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (currentGroup->SANIfrontComponentConnectionList)[i];
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



#endif
