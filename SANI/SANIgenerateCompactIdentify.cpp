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
 * File Name: SANIgenerateCompactIdentify.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1o5b 25-February-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact - identify and connect regions
 * /
 *******************************************************************************/


#include "SANIgenerateCompactIdentify.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR


#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_NEWLY_GENERATED_NEURONS
vector<SANIGroupNeuralNetwork*> newHiddenLayerGroupsTemp;

bool SANIgenerateCompactIdentifyClass::addGroupToHiddenLayerGroupsTemp(SANIGroupNeuralNetwork* newGroup)
{
	newHiddenLayerGroupsTemp.push_back(newGroup);
	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_NEWLY_GENERATED_NEURONS_INDIVIDUALLY
	newGroup->newlyGeneratedForSentenceTemp = true;
	#endif
}

bool SANIgenerateCompactIdentifyClass::clearHiddenLayerGroupsTemp()
{
	//cout << "newHiddenLayerGroupsTemp.size() = " << newHiddenLayerGroupsTemp.size() << endl; 
	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_NEWLY_GENERATED_NEURONS_INDIVIDUALLY
	for(int k=0; k<newHiddenLayerGroupsTemp.size(); k++)
	{
		SANIGroupNeuralNetwork* currentNeuron = newHiddenLayerGroupsTemp[k];
		currentNeuron->newlyGeneratedForSentenceTemp = false;
	}
	#endif
	newHiddenLayerGroupsTemp.clear();
}

bool SANIgenerateCompactIdentifyClass::setGeneratedForLastSentence(const SANItranslatorVariablesClass* translatorVariables)
{
	if(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal == translatorVariables->maxNumberSentences)
	{
		for(int k=0; k<newHiddenLayerGroupsTemp.size(); k++)
		{
			SANIGroupNeuralNetwork* currentNeuron = newHiddenLayerGroupsTemp[k];
			currentNeuron->generatedForLastSentence = true;
		}
	}
}

#endif



#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY
bool SANIgenerateCompactIdentifyClass::identifyVariableComponentsAndReferenceSets(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* topLevelParseTreeGroup)
{
	bool result = true;
	
	/*
	//moved below;
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT	//assumes SANI_SEQUENCE_GRAMMAR_RECORD_NEWLY_GENERATED_NEURONS
	for(int k=0; k<newHiddenLayerGroupsTemp.size(); k++)
	{
		SANIGroupNeuralNetwork* currentNeuron = newHiddenLayerGroupsTemp[k];	
		markSubNeurons(currentNeuron);
	}
	#endif
	*/
	
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_DETECTED_EDGE_OF_SENTENCE
	identifyStartEndOfSentenceWordDetected(forwardPropogationSentenceData, topLevelParseTreeGroup);
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS
	if(!identifyVariableComponentsWrapper(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, topLevelParseTreeGroup))
	{
		result = false;
	}
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_LINK_SIMILAR_SUBNETS
	if(!linkSimiliarSubnets(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, topLevelParseTreeGroup))
	{
		result = false;
	}
	#endif
	
	/*
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	for(int k=0; k<newHiddenLayerGroupsTemp.size(); k++)
	{
		SANIGroupNeuralNetwork* currentNeuron = newHiddenLayerGroupsTemp[k];	
		markSubNeuronsReset(currentNeuron);
	}
	#endif
	*/
	
	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_DETECTED_EDGE_OF_SENTENCE
bool SANIgenerateCompactIdentifyClass::identifyStartEndOfSentenceWordDetected(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup)
{	
	bool result = true;
	
	//cout << "currentParseTreeGroup->parseTreeMinWordIndex = " << currentParseTreeGroup->parseTreeMinWordIndex << endl;
	//cout << "currentParseTreeGroup->parseTreeMaxWordIndex = " << currentParseTreeGroup->parseTreeMaxWordIndex << endl;
	
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		if(currentParseTreeGroup->parseTreeMinWordIndex == 0)
		{
			currentParseTreeGroup->groupRef->endOfSentenceWordDetected = true;
		}
		if(currentParseTreeGroup->parseTreeMaxWordIndex == forwardPropogationSentenceData->sentenceContents->size()-1)
		{
			currentParseTreeGroup->groupRef->startOfSentenceWordDetected = true;
		}
	}
	else
	{
		if(currentParseTreeGroup->parseTreeMinWordIndex == 0)
		{
			currentParseTreeGroup->groupRef->startOfSentenceWordDetected = true;
			//cout << "currentParseTreeGroup->groupRef->startOfSentenceWordDetected" << endl;
		}
		if(currentParseTreeGroup->parseTreeMaxWordIndex == forwardPropogationSentenceData->sentenceContents->size()-1)
		{
			currentParseTreeGroup->groupRef->endOfSentenceWordDetected = true;
			//cout << "currentParseTreeGroup->groupRef->endOfSentenceWordDetected" << endl;
		}
	}

	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{			
			if(!identifyStartEndOfSentenceWordDetected(forwardPropogationSentenceData, parseTreeComponent->parseTreeGroupRef))
			{
				result = false;
			}
		}
	}
	
	return result;
}
#endif

#endif


#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS
bool SANIgenerateCompactIdentifyClass::identifyVariableComponentsWrapper(const SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIGroupParseTree* topLevelParseTreeGroup)
{
	bool result = true;
								
	if(!identifyVariableComponents(SANIGroupTypes, forwardPropogationSentenceData))
	{	
		result = false;
	}
	
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_CENTRAL_COMPONENTS
	if(!findAndLinkCentralVariationCandidates(forwardPropogationSentenceData))
	{	
		result = false;
	}
	#endif
	
	return result;
}

bool SANIgenerateCompactIdentifyClass::identifyVariableComponents(vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
	
	bool identifyAVariableComponent = false;
	
	for(int k=0; k<newHiddenLayerGroupsTemp.size(); k++)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ONE_PER_SENTENCE
		if(!identifyAVariableComponent)
		{
		#endif
		
			SANIGroupNeuralNetwork* generatedNeuron = newHiddenLayerGroupsTemp[k];
			//cout << "generatedNeuron
			bool passEdgeRequirements = true;

			int firstComponentIndex;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				firstComponentIndex = generatedNeuron->components.size()-1;
			}
			else
			{
				firstComponentIndex = 0;
			}
			SANIComponentNeuralNetwork* firstComponentOfGeneratedNeuron = generatedNeuron->components[firstComponentIndex];	

			int lastComponentIndex;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				lastComponentIndex = 0;
			}
			else
			{
				lastComponentIndex = generatedNeuron->components.size()-1;
			}
			SANIComponentNeuralNetwork* lastComponentOfGeneratedNeuron = generatedNeuron->components[lastComponentIndex];

			if(firstComponentOfGeneratedNeuron->SANIbackGroupConnectionList.size() > 1)
			{
				cerr << "SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_FIRST_COMPONENTS: SANIgenerateCompactIdentifyClass::identifyVariableFirstComponents error: (firstComponentOfGeneratedNeuron->SANIbackGroupConnectionList.size() > 1); SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_GENERATE_FIRST_COMPONENTS is not supported" << endl;
				exit(EXIT_ERROR);
			}
			if(lastComponentOfGeneratedNeuron->SANIbackGroupConnectionList.size() > 1)
			{
				cerr << "SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_FIRST_COMPONENTS: SANIgenerateCompactIdentifyClass::identifyVariableFirstComponents error: (lastComponentOfGeneratedNeuron->SANIbackGroupConnectionList.size() > 1); SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS is not supported" << endl;
				exit(EXIT_ERROR);
			}
			/*
			if(generatedNeuron->SANIfrontComponentConnectionList.size() > 1)
			{
				cerr << "SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_FIRST_COMPONENTS: SANIgenerateCompactIdentifyClass::identifyVariableFirstComponents error: (generatedNeuron->SANIfrontComponentConnectionList.size() > 1)" << endl;
				exit(EXIT_ERROR);
			}
			*/
			SANIGroupNeuralNetwork* firstComponentOfGeneratedNeuronSource = (firstComponentOfGeneratedNeuron->SANIbackGroupConnectionList)[0];	//generated neuron doesn't have variable components
			SANIGroupNeuralNetwork* lastComponentOfGeneratedNeuronSource = (lastComponentOfGeneratedNeuron->SANIbackGroupConnectionList)[0];	//generated neuron doesn't have variable components

			bool identifyVariableFirstComponent = false;
			bool identifyVariableLastComponent = false;
	
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_FIRST_COMPONENTS
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_FIRST_COMPONENTS_NON_STRING
			if(!SANInodes.hasComponentTypeString(firstComponentOfGeneratedNeuron))	//firstComponentOfGeneratedNeuron->componentType != GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING
			{
			if(!SANInodes.hasComponentTypeString(lastComponentOfGeneratedNeuron))	//lastComponentOfGeneratedNeuron->componentType != GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING
			{
			#endif	
				if(identifyVariableFirstLastComponents(SANIGroupTypes, forwardPropogationSentenceData, true, generatedNeuron, firstComponentOfGeneratedNeuron, lastComponentOfGeneratedNeuron, firstComponentOfGeneratedNeuronSource, lastComponentOfGeneratedNeuronSource))
				{
					identifyAVariableComponent = true;
					identifyVariableFirstComponent = true;
					#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_NEWLY_GENERATED_NEURONS_UPDATE_DURING_IDENTIFY
					newHiddenLayerGroupsTemp.erase(newHiddenLayerGroupsTemp.begin() + k);	//CHECKTHIS
					k--;	//generatedNeuron has been merged (deleted)
					#endif

				}
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_FIRST_COMPONENTS_NON_STRING
			}
			}
			#endif	
			#endif

			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_LAST_COMPONENTS
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_LAST_COMPONENTS_NON_STRING
			if(!SANInodes.hasComponentTypeString(firstComponentOfGeneratedNeuron))	//firstComponentOfGeneratedNeuron->componentType != GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING
			{
			if(!SANInodes.hasComponentTypeString(lastComponentOfGeneratedNeuron))	//lastComponentOfGeneratedNeuron->componentType != GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING
			{
			#endif	
				if(!identifyVariableFirstComponent)
				{
					if(identifyVariableFirstLastComponents(SANIGroupTypes, forwardPropogationSentenceData, false, generatedNeuron, lastComponentOfGeneratedNeuron, firstComponentOfGeneratedNeuron, lastComponentOfGeneratedNeuronSource, firstComponentOfGeneratedNeuronSource))
					{
						identifyAVariableComponent = true;
						identifyVariableLastComponent = true;
						#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_NEWLY_GENERATED_NEURONS_UPDATE_DURING_IDENTIFY
						newHiddenLayerGroupsTemp.erase(newHiddenLayerGroupsTemp.begin() + k);	//CHECKTHIS
						k--;	//generatedNeuron has been merged (deleted)
						#endif
					}
				}
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_LAST_COMPONENTS_NON_STRING
			}
			}
			#endif
			#endif
			
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ONE_PER_SENTENCE
		}
		#endif
	}
	
	return result;
}

bool SANIgenerateCompactIdentifyClass::identifyVariableFirstLastComponents(vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool identifyVariableFirstOrLastComponent, SANIGroupNeuralNetwork* generatedNeuron, const SANIComponentNeuralNetwork* variableComponentOfGeneratedNeuron, const SANIComponentNeuralNetwork* nonvariableComponentOfGeneratedNeuron, SANIGroupNeuralNetwork* variableComponentOfGeneratedNeuronSource, SANIGroupNeuralNetwork* nonvariableComponentOfGeneratedNeuronSource)
{
	bool result = false;
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	/*
	//verify no neurons already marked;
	SANIGroupType* groupType = SANInodes.getSequenceGrammarGroupTypeDefault(SANIGroupTypes);
	for(int k2=0; k2<groupType->groups.size(); k2++)
	{
		SANIGroupNeuralNetwork* groupTypeNeuralNetworkGroup = (groupType->groups)[k2];
		if(groupTypeNeuralNetworkGroup->marked)
		{
			cerr << "SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT SANIgenerateCompactIdentifyClass::identifyVariableFirstLastComponents error: groupTypeNeuralNetworkGroup->marked" << endl;
			exit(EXIT_ERROR);
		}
	}
	*/
	//required for SANIpropagateOperationsClass::identifyMissingOrVariableStart/EndComponentFound:calculateVariableComponentPassCriteria;
	markFirstComponentSubNeurons(forwardPropogationSentenceData, generatedNeuron);	//changed SANI1n7c - OLD: markSubNeurons(generatedNeuron);
	#endif
						
	bool passEdgeRequirements = true;
	if(identifyVariableFirstOrLastComponent)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ENSURE_VARIABLE_COMPONENT_ENCAPSULATES_EDGE_OF_SENTENCE
		if(!(variableComponentOfGeneratedNeuronSource->startOfSentenceWordDetected))
		{
			passEdgeRequirements = false;
		}
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ENSURE_NONVARIABLE_COMPONENT_ENCAPSULATES_EDGE_OF_SENTENCE
		if(!(nonvariableComponentOfGeneratedNeuronSource->endOfSentenceWordDetected))
		{
			passEdgeRequirements = false;
		}
		#endif	
	}
	else
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ENSURE_VARIABLE_COMPONENT_ENCAPSULATES_EDGE_OF_SENTENCE
		if(!(variableComponentOfGeneratedNeuronSource->endOfSentenceWordDetected))
		{
			passEdgeRequirements = false;
		}
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ENSURE_NONVARIABLE_COMPONENT_ENCAPSULATES_EDGE_OF_SENTENCE
		if(!(nonvariableComponentOfGeneratedNeuronSource->startOfSentenceWordDetected))
		{
			passEdgeRequirements = false;
		}
		#endif	
	}
	
	if(passEdgeRequirements)
	{
		//cout << "passEdgeRequirements" << endl;
		
		for(int i=0; i<nonvariableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.size(); i++)
		{
			if(!(generatedNeuron->markToErase))	//only perform merge once
			{
				SANIComponentNeuralNetwork* currentComponent = (nonvariableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList)[i];
				SANIGroupNeuralNetwork* candidateMatchGroup = currentComponent->ownerGroup;
				if(candidateMatchGroup != generatedNeuron)
				{		
					//cout << "(candidateMatchGroup != generatedNeuron)" << endl;
						
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VERIFY_NOT_NEWLY_CREATED
					if(!(candidateMatchGroup->newlyGeneratedForSentenceTemp)) //only consider candidateMatchGroup if neuron has not been newly generated for current sentence	//candidateMatchGroup->timeIndex != forwardPropogationSentenceData->sentenceIndex
					{
					#endif
						int candidateMatchGroupVariableComponentIndex2;
						int candidateMatchGroupNonvariableComponentIndex2;
						if(identifyVariableFirstOrLastComponent)
						{
							if(forwardPropogationSentenceData->parseSentenceReverse)
							{
								candidateMatchGroupVariableComponentIndex2 = candidateMatchGroup->components.size()-1;
								candidateMatchGroupNonvariableComponentIndex2 = 0;
							}
							else
							{
								candidateMatchGroupVariableComponentIndex2 = 0;
								candidateMatchGroupNonvariableComponentIndex2 = candidateMatchGroup->components.size()-1;
							}
						}
						else
						{
							if(forwardPropogationSentenceData->parseSentenceReverse)
							{
								candidateMatchGroupVariableComponentIndex2 = 0;
								candidateMatchGroupNonvariableComponentIndex2 = candidateMatchGroup->components.size()-1;
							}
							else
							{
								candidateMatchGroupVariableComponentIndex2 = candidateMatchGroup->components.size()-1;
								candidateMatchGroupNonvariableComponentIndex2 = 0;
							}
						}

						SANIComponentNeuralNetwork* candidateMatchGroupVariableComponent = candidateMatchGroup->components[candidateMatchGroupVariableComponentIndex2];
						SANIComponentNeuralNetwork* candidateMatchGroupNonvariableComponent = candidateMatchGroup->components[candidateMatchGroupNonvariableComponentIndex2];
						if(candidateMatchGroupNonvariableComponent->SANIbackGroupConnectionList.size() == 1)	//ensure than candidateMatch non-variable component has only 1 child (ie SANI doesnt support neurons with more than 1 variable component; either first or last)
						{
							if(currentComponent->componentIndex == candidateMatchGroupNonvariableComponentIndex2)
							{								
								//based on SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady:
								bool passPreconditions = true;
								if(identifyVariableFirstOrLastComponent)
								{
									#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_FIRST_COMPONENTS
									if(!SANIpropagateCompactGenerateOperations.identifyMissingOrVariableStartComponentFound(forwardPropogationSentenceData, &(candidateMatchGroup->components), candidateMatchGroupVariableComponent))	//requires firstComponent
									{
										passPreconditions = false;
									}
									#endif
									#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_VERIFY_THAT_VARIABLE_EDGE_COMPONENT_SOURCE_POS_IS_NOT_IDENTICAL
									if(SANIpropagateCompactGenerateOperations.variableEdgeComponentSourcePOSisIdenticalWrapper1(forwardPropogationSentenceData, generatedNeuron, candidateMatchGroup, identifyVariableFirstOrLastComponent))
									{
										//cout << "variableEdgeComponentSourcePOSisIdenticalWrapper1" << endl;
										passPreconditions = false;
									}
									/*
									else
									{
										cout << "!variableEdgeComponentSourcePOSisIdenticalWrapper1" << endl;
									}
									*/
									#endif
								}
								else
								{
									#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_LAST_COMPONENTS
									if(!SANIpropagateCompactGenerateOperations.identifyMissingOrVariableEndComponentFound(forwardPropogationSentenceData, &(candidateMatchGroup->components), candidateMatchGroupNonvariableComponent))	//requires secondLastComponentIndex (firstComponent)
									{
										passPreconditions = false;
									}
									#endif
									#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_VERIFY_THAT_VARIABLE_EDGE_COMPONENT_SOURCE_POS_IS_NOT_IDENTICAL
									if(SANIpropagateCompactGenerateOperations.variableEdgeComponentSourcePOSisIdenticalWrapper1(forwardPropogationSentenceData, generatedNeuron, candidateMatchGroup, identifyVariableFirstOrLastComponent))
									{
										passPreconditions = false;
									}
									#endif
								}
								//CHECKTHIS: no additional tests required from SANIpropagateOperationsClass::identifySequentialActivationFound
								if(passPreconditions)
								{
									#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
									//cout << "\nfound a variable first/last component candidate match" << endl;
									/*
									cout << "generatedNeuron->groupIndex = " << generatedNeuron->groupIndex << endl;
									cout << "candidateMatchGroup->groupIndex = " << candidateMatchGroup->groupIndex << endl;
									//cout << "variableComponentOfGeneratedNeuronSource->newlyGeneratedForSentenceTemp = " << variableComponentOfGeneratedNeuronSource->newlyGeneratedForSentenceTemp << ", groupIndex = " << variableComponentOfGeneratedNeuronSource->groupIndex << endl;
									cout << "variableComponentOfGeneratedNeuronSource->groupIndex = " << variableComponentOfGeneratedNeuronSource->groupIndex << endl;
									cout << "nonvariableComponentOfGeneratedNeuronSource->groupIndex = " << nonvariableComponentOfGeneratedNeuronSource->groupIndex << endl;								
									for(int j=0; j<candidateMatchGroupVariableComponent->SANIbackGroupConnectionList.size(); j++)
									{
										SANIGroupNeuralNetwork* candidateMatchGroupVariableComponentSource = (candidateMatchGroupVariableComponent->SANIbackGroupConnectionList)[j];
										cout << "candidateMatchGroupVariableComponentSource->groupIndex = " << candidateMatchGroupVariableComponentSource->groupIndex << endl;
									}
									for(int j=0; j<candidateMatchGroupNonvariableComponent->SANIbackGroupConnectionList.size(); j++)
									{
										SANIGroupNeuralNetwork* candidateMatchGroupNonvariableComponentSource = (candidateMatchGroupNonvariableComponent->SANIbackGroupConnectionList)[j];
										cout << "candidateMatchGroupNonvariableComponentSource->groupIndex = " << candidateMatchGroupNonvariableComponentSource->groupIndex << endl;
									}
									*/
									#endif
																			
									//verify that variableComponentOfGeneratedNeuronSource is not already connected to candidateMatchGroupVariableComponent;
									bool duplicateProspectiveVariableComponentDetected = false;
									for(int j=0; j<candidateMatchGroupVariableComponent->SANIbackGroupConnectionList.size(); j++)
									{
										SANIGroupNeuralNetwork* candidateMatchGroupVariableComponentSource = (candidateMatchGroupVariableComponent->SANIbackGroupConnectionList)[j];
										if(candidateMatchGroupVariableComponentSource == variableComponentOfGeneratedNeuronSource)
										{
											duplicateProspectiveVariableComponentDetected = true;
										}
									}
									if(!duplicateProspectiveVariableComponentDetected)
									{										
										
										//found a variable [first/]last component candidate match
										//now merge newly generated neuron with existing candidateMatchGroup

										#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP
										if(SANInodes.calculateComponentTypeString(variableComponentOfGeneratedNeuronSource))
										{
											candidateMatchGroupVariableComponent->neuralNetworkComponentHasTypeString = true;	//CHECKTHIS
										}
										#endif
										
										//1a.
										//disconnect variableComponentSource -> generatedNeuron
										//connect variableComponentSource -> candidateMatchGroup
										//connect variableComponentOfGeneratedNeuronSource <- candidateMatchGroup
										int variableComponentOfGeneratedNeuronSourceFrontIndexToErase = INT_DEFAULT_VALUE;
										for(int j=0; j<variableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.size(); j++)
										{
											SANIComponentNeuralNetwork* currentComponent2 = (variableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList)[j];
											SANIGroupNeuralNetwork* candidateMatchGroup2 = currentComponent2->ownerGroup;
											//cout << "\tgeneratedNeuron->groupIndex = " << generatedNeuron->groupIndex << endl;
											//cout << "\tcandidateMatchGroup2->groupIndex = " << candidateMatchGroup2->groupIndex << endl;
											if(candidateMatchGroup2 == generatedNeuron)
											{
												//delete this (variableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList)[j]
												variableComponentOfGeneratedNeuronSourceFrontIndexToErase = j;
											}
										}
										if(variableComponentOfGeneratedNeuronSourceFrontIndexToErase == INT_DEFAULT_VALUE)
										{
											cerr << "SANIgenerateCompactIdentifyClass::identifyVariableFirstLastComponents error: (variableComponentOfGeneratedNeuronSourceFrontIndexToErase == INT_DEFAULT_VALUE)" << endl;
											exit(EXIT_ERROR);
										}
										//cout << "variableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.size() = " << variableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.size() << endl;
										variableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.erase(variableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.begin() + variableComponentOfGeneratedNeuronSourceFrontIndexToErase);
										variableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.push_back(candidateMatchGroup->components[candidateMatchGroupVariableComponentIndex2]);
										candidateMatchGroup->components[candidateMatchGroupVariableComponentIndex2]->SANIbackGroupConnectionList.push_back(variableComponentOfGeneratedNeuronSource);

										//2.
										//disconnect nonvariableComponentSource -> generatedNeuron	
										//NO [ALREADYDONE]: connect nonvariableComponentSource -> candidateMatchGroup	
										//NO [ALREADYDONE: connect nonvariableComponentSource <- candidateMatchGroup
										int nonvariableComponentOfGeneratedNeuronSourceFrontIndexToErase = INT_DEFAULT_VALUE;
										for(int j=0; j<nonvariableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.size(); j++)
										{
											SANIComponentNeuralNetwork* currentComponent2 = (nonvariableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList)[j];
											SANIGroupNeuralNetwork* candidateMatchGroup2 = currentComponent2->ownerGroup;
											if(candidateMatchGroup2 == generatedNeuron)
											{	
												//delete this (nonvariableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList)[j]
												nonvariableComponentOfGeneratedNeuronSourceFrontIndexToErase = j;

												//cout << "candidateMatchGroup2->groupIndex = " << candidateMatchGroup2->groupIndex << endl;
												//cout << "nonvariableComponentOfGeneratedNeuronSourceFrontIndexToErase = " << nonvariableComponentOfGeneratedNeuronSourceFrontIndexToErase << endl;
											}
										}
										if(nonvariableComponentOfGeneratedNeuronSourceFrontIndexToErase == INT_DEFAULT_VALUE)
										{
											cerr << "SANIgenerateCompactIdentifyClass::identifyVariableFirstLastComponents error: (nonvariableComponentOfGeneratedNeuronSourceFrontIndexToErase == INT_DEFAULT_VALUE)" << endl;
											exit(EXIT_ERROR);
										}
										nonvariableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.erase(nonvariableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.begin() + nonvariableComponentOfGeneratedNeuronSourceFrontIndexToErase);	//CHECKTHIS

										//3.
										//disconnect generatedNeuron <- generatedNeuronTarget
										//connect candidateMatchGroup <- generatedNeuronTarget
										//connect candidateMatchGroup -> generatedNeuronTarget
										for(int l=0; l<generatedNeuron->SANIfrontComponentConnectionList.size(); l++)
										{
											SANIComponentNeuralNetwork* generatedNeuronTargetComponent = generatedNeuron->SANIfrontComponentConnectionList[l];
											SANIGroupNeuralNetwork* generatedNeuronTargetGroup = generatedNeuronTargetComponent->ownerGroup;

											int generatedNeuronTargetGroupBackIndexToErase = INT_DEFAULT_VALUE;
											for(int j=0; j<generatedNeuronTargetComponent->SANIbackGroupConnectionList.size(); j++)
											{
												SANIGroupNeuralNetwork* generatedNeuronTargetGroupComponentSource = (generatedNeuronTargetComponent->SANIbackGroupConnectionList)[j];
												//cout << "generatedNeuron->groupIndex = " << generatedNeuron->groupIndex << endl;
												//cout << "generatedNeuronTargetGroupComponentSource->groupIndex = " << generatedNeuronTargetGroupComponentSource->groupIndex << endl;
												if(generatedNeuronTargetGroupComponentSource == generatedNeuron)
												{
													//delete this (generatedNeuronTargetGroup->SANIbackGroupConnectionList)[j];
													generatedNeuronTargetGroupBackIndexToErase = j;
												}
											}

											if(generatedNeuronTargetGroupBackIndexToErase != INT_DEFAULT_VALUE)
											{
												generatedNeuronTargetComponent->SANIbackGroupConnectionList.erase(generatedNeuronTargetComponent->SANIbackGroupConnectionList.begin() + generatedNeuronTargetGroupBackIndexToErase);
												generatedNeuronTargetComponent->SANIbackGroupConnectionList.push_back(candidateMatchGroup);
												candidateMatchGroup->SANIfrontComponentConnectionList.push_back(generatedNeuronTargetComponent);
											}
										}

										//4. erase generatedNeuron
										generatedNeuron->markToErase = true;
									}
								}
							}
						}
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VERIFY_NOT_NEWLY_CREATED
					}
					#endif
				}
			}
		}
	}
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	markSubNeuronsReset(generatedNeuron);
	#endif
	
	if(passEdgeRequirements)
	{			
		if(generatedNeuron->markToErase)
		{
			//cout << "SANIgenerateCompactIdentifyClass::identifyVariableFirstLastComponents" << endl;
			
			result = true;

			SANIGroupType* groupType = SANInodes.getSequenceGrammarGroupTypeDefault(SANIGroupTypes);	
			groupType->groups.erase(remove(groupType->groups.begin(), groupType->groups.end(), generatedNeuron), groupType->groups.end());	//find and erase element

			delete generatedNeuron->neuronReference;
			delete generatedNeuron;
		}
	}

	return result;
}


#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_CENTRAL_COMPONENTS
bool SANIgenerateCompactIdentifyClass::findAndLinkCentralVariationCandidates(SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	//algorithm: find nodes that are connected to the same first input neuron and an output neuron in which the second input of the output neuron is the same
	
	SANIpropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);

	for(int k=0; k<newHiddenLayerGroupsTemp.size(); k++)
	{
		SANIGroupNeuralNetwork* generatedNeuron = newHiddenLayerGroupsTemp[k];

		int firstComponentIndex;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			firstComponentIndex = generatedNeuron->components.size()-1;
		}
		else
		{
			firstComponentIndex = 0;
		}

		vector<SANIGroupNeuralNetwork*> secondComponentOfOutputNeuronSourceVector1;
		populateSecondComponentOfOutputNeuronSourceVector(forwardPropogationSentenceData, generatedNeuron, &secondComponentOfOutputNeuronSourceVector1);
		
		SANIComponentNeuralNetwork* firstComponent = generatedNeuron->components[firstComponentIndex];
		for(int j=0; j<firstComponent->SANIbackGroupConnectionList.size(); j++)
		{
			SANIGroupNeuralNetwork* firstComponentSource = (firstComponent->SANIbackGroupConnectionList)[j];
			
			for(int i=0; i<firstComponentSource->SANIfrontComponentConnectionList.size(); i++)
			{
				SANIComponentNeuralNetwork* currentComponent = (firstComponentSource->SANIfrontComponentConnectionList)[i];
				SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	
				
				if(ownerGroup != generatedNeuron)
				{
					vector<SANIGroupNeuralNetwork*> secondComponentOfOutputNeuronSourceVector2;
					populateSecondComponentOfOutputNeuronSourceVector(forwardPropogationSentenceData, ownerGroup, &secondComponentOfOutputNeuronSourceVector2);
					
					for(int v1=0; v1<secondComponentOfOutputNeuronSourceVector1.size(); v1++)
					{
						for(int v2=0; v2<secondComponentOfOutputNeuronSourceVector2.size(); v2++)
						{
							if(secondComponentOfOutputNeuronSourceVector1[v1] == secondComponentOfOutputNeuronSourceVector2[v2])
							{
								//ownerGroup represents a central variation of generatedNeuron (ie start and end sequences are identical)
								
								#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_CENTRAL_COMPONENTS_LINK
								//link these nodes together
								secondComponentOfOutputNeuronSourceVector1[v1]->phraseCandidateSynonyms.push_back(secondComponentOfOutputNeuronSourceVector2[v2]);
								secondComponentOfOutputNeuronSourceVector2[v2]->phraseCandidateSynonyms.push_back(secondComponentOfOutputNeuronSourceVector1[v1]);
								#endif
							}
						}
					}
				}
			}
		}	
	}
}
bool SANIgenerateCompactIdentifyClass::populateSecondComponentOfOutputNeuronSourceVector(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIGroupNeuralNetwork* group, vector<SANIGroupNeuralNetwork*>* secondComponentOfOutputNeuronSourceVector)
{
	bool result = true;
	
	for(int i=0; i<group->SANIfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i];
		SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	

		int lastComponentIndex;	//ie secondComponentIndex
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			lastComponentIndex = 0;
		}
		else
		{
			lastComponentIndex = ownerGroup->components.size()-1;
		}

		SANIComponentNeuralNetwork* secondComponentOfOutputNeuron = ownerGroup->components[lastComponentIndex];
		for(int j=0; j<secondComponentOfOutputNeuron->SANIbackGroupConnectionList.size(); j++)
		{
			SANIGroupNeuralNetwork* secondComponentOfOutputNeuronSource = (secondComponentOfOutputNeuron->SANIbackGroupConnectionList)[j];
			secondComponentOfOutputNeuronSourceVector->push_back(secondComponentOfOutputNeuronSource);
		}
	}
	
	return result;
}
#endif

#endif
	
#ifdef SANI_SEQUENCE_GRAMMAR_LINK_SIMILAR_SUBNETS
bool SANIgenerateCompactIdentifyClass::linkSimiliarSubnets(SANItranslatorVariablesClass* translatorVariables, const vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* topLevelParseTreeGroup)
{
	bool result = true;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_SUCCESSFULLY_PARSED_NEURONS_BEFORE_REFERENCE_SET_IDENTIFICATION
	updateNetworkRecency(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal, topLevelParseTreeGroup, 0);
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
	if(!findAndLinkReferenceSetCandidates1(forwardPropogationSentenceData, topLevelParseTreeGroup, 0))
	{
		result = false;
	}	
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_SUCCESSFULLY_PARSED_NEURONS_AFTER_REFERENCE_SET_IDENTIFICATION
	updateNetworkRecency(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal, topLevelParseTreeGroup, 0)
	#endif
	
	return result;	
}

#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_SUCCESSFULLY_PARSED_NEURONS
bool SANIgenerateCompactIdentifyClass::updateNetworkRecency(int timeIndex, SANIGroupParseTree* currentParseTreeGroup, const int layer)
{
	//this function only updates recency of nodes in successfully parsed/generated parse tree
	
	bool result = true;
	
	currentParseTreeGroup->groupRef->timeIndex = timeIndex;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!updateNetworkRecency(timeIndex, parseTreeComponent->parseTreeGroupRef, layer+1))
			{
				result = false;
			}
		}
	}

	return result;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
bool SANIgenerateCompactIdentifyClass::findAndLinkReferenceSetCandidates1(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup, const int layer)
{
	bool result = true;
	
	if(currentParseTreeGroup->groupRef->newlyGeneratedForSentenceTemp)	//CHECKTHIS: only perform reference set candidate detection for sentence newly generated neurons
	{
		SANIGroupNeuralNetwork* referenceSetCandidate2best = NULL;	//considered best based on some function of number of matches with candidateVector1 and recency
		
		SANIGroupNeuralNetwork* referenceSetCandidate1 = currentParseTreeGroup->groupRef;
		vector<SANIGroupNeuralNetwork*> referenceSetCandidateVector1;
		createReferenceSetCandidateVector(currentParseTreeGroup, &referenceSetCandidateVector1);
		
		findAndLinkReferenceSetCandidates2(forwardPropogationSentenceData, &referenceSetCandidate2best, currentParseTreeGroup, referenceSetCandidate1, &referenceSetCandidateVector1, layer);
		
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_LINK		        	
		if(referenceSetCandidate2best != NULL)
		{
			//link these nodes together
			referenceSetCandidate1->referenceSetCandidateDuplicates.push_back(referenceSetCandidate2best);
			referenceSetCandidate2best->referenceSetCandidateDuplicates.push_back(referenceSetCandidate1);
			
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
			cout << "\nfindAndLinkReferenceSetCandidates1, referenceSetCandidate1: " << endl;
			SANInodes.printParseTree(currentParseTreeGroup, 0);
			cout << "\nfindAndLinkReferenceSetCandidates1, referenceSetCandidate2best: " << endl;
			SANInodes.printNeuralNetwork(referenceSetCandidate2best, 0);
			#endif
		}
		#endif
		referenceSetCandidate1->referenceSetCandidateBestMetric = 0.0;
	}
		
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		//printComponent(parseTreeComponent, layer);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!findAndLinkReferenceSetCandidates1(forwardPropogationSentenceData, parseTreeComponent->parseTreeGroupRef, layer+1))
			{
				result = false;
			}
		}
	}

	return result;
}

bool SANIgenerateCompactIdentifyClass::findAndLinkReferenceSetCandidates2(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork* referenceSetCandidate1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const int layer)
{
	bool result = true;
	
	bool hasComponentGroupRefs = false;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		//printComponent(parseTreeComponent, layer);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			hasComponentGroupRefs = true;
			
			if(!findAndLinkReferenceSetCandidates2(forwardPropogationSentenceData, referenceSetCandidate2best, parseTreeComponent->parseTreeGroupRef, referenceSetCandidate1, referenceSetCandidateVector1, layer+1))
			{
				result = false;
			}
		}
	}
	
	if(!hasComponentGroupRefs)
	{
		//every component points to an inputLayerNeuron
		SANIGroupNeuralNetwork* referenceSetCandidate2 = currentParseTreeGroup->groupRef;
		if(!findAndLinkReferenceSetCandidates3(forwardPropogationSentenceData, referenceSetCandidate2best, referenceSetCandidate2, referenceSetCandidate1, referenceSetCandidateVector1, layer))
		{
			result = false;
		}	
	}

	return result;
}



bool SANIgenerateCompactIdentifyClass::findAndLinkReferenceSetCandidates3(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupNeuralNetwork* referenceSetCandidate2, SANIGroupNeuralNetwork* referenceSetCandidate1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const int layer)
{
	bool result = true;

	bool referenceSetCandidate2InReferenceSetCandidateVector1 = false;
	for(int v1=0; v1<referenceSetCandidateVector1->size(); v1++)
	{
		if(referenceSetCandidate2 == (*referenceSetCandidateVector1)[v1])
		{
			referenceSetCandidate2InReferenceSetCandidateVector1 = true;
		}
	}
	if(!referenceSetCandidate2InReferenceSetCandidateVector1)
	{
		//cout << "!referenceSetCandidate2InReferenceSetCandidateVector1" << endl;
		vector<SANIGroupNeuralNetwork*> referenceSetCandidateVector2;
		createReferenceSetCandidateVector(referenceSetCandidate2, &referenceSetCandidateVector2);
		findAndLinkReferenceSetCandidatesCompareVectors(referenceSetCandidate2best, referenceSetCandidate1, referenceSetCandidate2, referenceSetCandidateVector1, &referenceSetCandidateVector2);
	}
		
	for(int i=0; i<referenceSetCandidate2->SANIfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (referenceSetCandidate2->SANIfrontComponentConnectionList)[i];
		SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
		if(ownerGroup != referenceSetCandidate1)	//ensure that referenceSetCandidate2 != referenceSetCandidate1
		{			
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_VERIFY_NOT_NEWLY_CREATED
			if(!(ownerGroup->newlyGeneratedForSentenceTemp)) //only consider referenceSetCandidate2 if neuron has not been newly generated for current sentence	//ownerGroup->timeIndex != forwardPropogationSentenceData->sentenceIndex
			{
			#endif
				findAndLinkReferenceSetCandidates3(forwardPropogationSentenceData, referenceSetCandidate2best, ownerGroup, referenceSetCandidate1, referenceSetCandidateVector1, layer-1);
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_VERIFY_NOT_NEWLY_CREATED
			}
			#endif
		}
	}

	return result;
}

/*
bool SANIgenerateCompactIdentifyClass::findAndLinkReferenceSetCandidatesWrapperInefficient(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	if(currentParseTreeGroup->groupRef->newlyGeneratedForSentenceTemp)	//CHECKTHIS (only perform reference set candidate detection for sentence newly generated neurons)
	{
		vector<SANIGroupNeuralNetwork*> referenceSetCandidateVector1;
		createReferenceSetCandidateVector(currentParseTreeGroup, &referenceSetCandidateVector1);
		SANIGroupNeuralNetwork* currentNeuralNetworkGroup = currentParseTreeGroup->groupRef;

		for(int k=0; k<SANIGroupTypes->size(); k++)
		{
			SANIGroupType* groupType = SANIGroupTypes->at(k);
			for(int k2=0; k2<groupType->groups.size(); k2++)
			{
				SANIGroupNeuralNetwork* groupTypeNeuralNetworkGroup = (groupType->groups)[k2];

				vector<SANIGroupNeuralNetwork*> referenceSetCandidateVector2;
				createReferenceSetCandidateVector(groupTypeNeuralNetworkGroup, &referenceSetCandidateVector2);

				SANIGroupNeuralNetwork* referenceSetCandidate2best = NULL;
				findAndLinkReferenceSetCandidatesCompareVectors(&referenceSetCandidate2best, groupTypeNeuralNetworkGroup, currentNeuralNetworkGroup, referenceSetCandidateVector1, referenceSetCandidateVector2);
				#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_LINK
				if(referenceSetCandidate2best != NULL)
				{
					//link these nodes together
					referenceSetCandidate1->referenceSetCandidateDuplicates.push_back(referenceSetCandidate2best);
					referenceSetCandidate2best->referenceSetCandidateDuplicates.push_back(referenceSetCandidate1);
				}
				#endif
			}
		}
	}
		
		
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		//printComponent(parseTreeComponent, layer);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!findAndLinkReferenceSetCandidatesWrapper(SANIGroupTypes, forwardPropogationSentenceData, parseTreeComponent->parseTreeGroupRef, layer+1))
			{
				result = false;
			}
		}
	}

	return result;
}
*/

bool SANIgenerateCompactIdentifyClass::findAndLinkReferenceSetCandidatesCompareVectors(constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupNeuralNetwork* referenceSetCandidate1, constEffective SANIGroupNeuralNetwork* referenceSetCandidate2, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector2)
{
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_RECENCY
	int totalMatchRecencyV2 = 0;
	int totalParseTreeRecencyV1 = 0;
	#endif
	int numberCandidatesV1 = referenceSetCandidateVector1->size();
	int numberCandidatesV2 = referenceSetCandidateVector2->size();
	int numberMatchesV2 = 0;
	for(int v1=0; v1<referenceSetCandidateVector1->size(); v1++)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_RECENCY
		totalParseTreeRecencyV1 = totalParseTreeRecencyV1 + (*referenceSetCandidateVector1)[v1]->timeIndex;
		#endif
		
		for(int v2=0; v2<referenceSetCandidateVector2->size(); v2++)
		{
			if((*referenceSetCandidateVector1)[v1] == (*referenceSetCandidateVector2)[v2])
			{
				//found a match between referenceSetCandidate1 (in original parse tree) and referenceSetCandidate2 (in entire SANI network)
				numberMatchesV2++;
				#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_RECENCY
				totalMatchRecencyV2 = totalMatchRecencyV2 + (*referenceSetCandidateVector2)[v2]->timeIndex;
				#endif
			}
		}
	}

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
	//cout << "numberMatchesV2 = " << numberMatchesV2 << endl;
	#endif
	if(numberMatchesV2 >= SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_MIN_MATCHES)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_RECENCY
		double metric = double(totalMatchRecencyV2)/double(totalParseTreeRecencyV1);
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
		//cout << "(metric > referenceSetCandidate1->referenceSetCandidateBestMetric)" << endl;
		#endif
		if(metric > SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_RECENCY_MATCH_PERCENTAGE_REQUISITE)
		{	
		#else
		double metric = double(numberMatchesV2)/double(numberCandidatesV1);
		if(metric > SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_THRESHOLD_MATCH_PERCENT_REQUISITE)
		{
		#endif	
			if(metric > referenceSetCandidate1->referenceSetCandidateBestMetric)
			{
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
				//cout << "(metric > referenceSetCandidate1->referenceSetCandidateBestMetric)" << endl;
				#endif
				
				*referenceSetCandidate2best = referenceSetCandidate2;
				referenceSetCandidate1->referenceSetCandidateBestMetric = metric;
			}
		}

		/*
		if(numberMatchesV2 > int(double(numberCandidatesV1)*SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_THRESHOLD_PERCENT_TRUE_POSITIVES_V1))
		{
			if(numberMatchesV2 > int(double(numberCandidatesV2)*SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_THRESHOLD_PERCENT_TRUE_POSITIVES_V2))
			{
				if(metric > referenceSetCandidate1->referenceSetCandidateBestMetric)
				{
					*referenceSetCandidate2best = referenceSetCandidate2;
					referenceSetCandidate1->referenceSetCandidateBestMetric = metric;
				}
			}
		}
		*/
	}
}
								
bool SANIgenerateCompactIdentifyClass::createReferenceSetCandidateVector(SANIGroupParseTree* currentParseTreeGroup, vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector)
{
	bool result = true;
	
	referenceSetCandidateVector->push_back(currentParseTreeGroup->groupRef);
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{		
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!createReferenceSetCandidateVector(parseTreeComponent->parseTreeGroupRef, referenceSetCandidateVector))
			{
				result = false;
			}
		}
		else
		{
			if(SANInodes.hasComponentTypeString(parseTreeComponent))
			{
				SANIGroupNeuralNetwork* componentSource = parseTreeComponent->componentRef->SANIbackGroupConnectionList[0];
				referenceSetCandidateVector->push_back(componentSource);	//added SANI1m5f
				
				//cout << "(componentSource->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)" << endl;
				//cout << "componentSource->groupIndex = " << componentSource->groupIndex << endl;
			}
		}
	}

	return result;
}
bool SANIgenerateCompactIdentifyClass::createReferenceSetCandidateVector(SANIGroupNeuralNetwork* currentNeuron, vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector)
{
	bool result = true;
	
	referenceSetCandidateVector->push_back(currentNeuron);
	
	for(int i=0; i<currentNeuron->components.size(); i++)
	{				
		SANIComponentNeuralNetwork* component = (currentNeuron->components)[i];
		for(int j=0; j<component->SANIbackGroupConnectionList.size(); j++)
		{
			SANIGroupNeuralNetwork* componentSource = (component->SANIbackGroupConnectionList)[j];
			//cout << "componentSource->inputLayerNeuron = " << componentSource->inputLayerNeuron << endl;
			
			if(!createReferenceSetCandidateVector(componentSource, referenceSetCandidateVector))
			{
				result = false;
			}
		}
	}

	return result;
}

#endif
#endif


#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
bool SANIgenerateCompactIdentifyClass::markFirstComponentSubNeurons(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentNeuron)
{	
	bool result = true;
	
	if(!SANInodes.isNeuronString(currentNeuron))	//CHECKTHIS
	{
		currentNeuron->marked = true;
		SANIComponentNeuralNetwork* firstComponent = SANInodes.getFirstComponent(forwardPropogationSentenceData, currentNeuron, true);
		for(int j=0; j<firstComponent->SANIbackGroupConnectionList.size(); j++)
		{
			SANIGroupNeuralNetwork* subGroup = (firstComponent->SANIbackGroupConnectionList)[j];
			markSubNeurons(subGroup);
		}
	}
	
	return result;
}
bool SANIgenerateCompactIdentifyClass::markSubNeurons(SANIGroupNeuralNetwork* currentNeuron)
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
				cout << "SANIgenerateCompactSectionedClass::addSubNeuronsToList error: currentNeuron has already been parsed (circular loop detected)" << endl;
				exit(EXIT_ERROR);
			}
			currentNeuron->verified = true;
			#endif

			for(int i=0; i<currentNeuron->components.size(); i++)
			{
				SANIComponentNeuralNetwork* currentComponent = currentNeuron->components[i];
				for(int j=0; j<currentComponent->SANIbackGroupConnectionList.size(); j++)
				{
					SANIGroupNeuralNetwork* subGroup = (currentComponent->SANIbackGroupConnectionList)[j];
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
bool SANIgenerateCompactIdentifyClass::markSubNeuronsReset(SANIGroupNeuralNetwork* currentNeuron)
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
				for(int j=0; j<currentComponent->SANIbackGroupConnectionList.size(); j++)
				{
					SANIGroupNeuralNetwork* subGroup = (currentComponent->SANIbackGroupConnectionList)[j];
					markSubNeuronsReset(subGroup);
				}
			}
		}
	}
	
	return result;
}
#endif
#endif

	
#endif
