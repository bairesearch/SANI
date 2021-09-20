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
 * Project Version: 1q2b 19-September-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Identify - identify and connect regions
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
bool SANIgenerateCompactIdentifyClass::identifyVariableComponentsAndReferenceSets(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* topLevelParseTreeGroup)
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
	identifyStartEndOfSentenceWordDetected(forwardPropogationSequenceData, topLevelParseTreeGroup);
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS
	if(!identifyVariableComponentsWrapper(translatorVariables, SANIGroupTypes, forwardPropogationSequenceData))
	{
		result = false;
	}
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_LINK_SIMILAR_SUBNETS
	if(!SANIgenerateCompactIdentifyReferenceSets.linkSimiliarSubnets(translatorVariables, SANIGroupTypes, forwardPropogationSequenceData, topLevelParseTreeGroup))
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
bool SANIgenerateCompactIdentifyClass::identifyStartEndOfSentenceWordDetected(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* currentParseTreeGroup)
{	
	bool result = true;
	
	//cout << "currentParseTreeGroup->parseTreeMinSequenceIndex = " << currentParseTreeGroup->parseTreeMinSequenceIndex << endl;
	//cout << "currentParseTreeGroup->parseTreeMaxSequenceIndex = " << currentParseTreeGroup->parseTreeMaxSequenceIndex << endl;
	
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		if(currentParseTreeGroup->parseTreeMinSequenceIndex == 0)
		{
			currentParseTreeGroup->groupRef->endOfSequenceElementDetected = true;
		}
		if(currentParseTreeGroup->parseTreeMaxSequenceIndex == forwardPropogationSequenceData->sentenceContents->size()-1)
		{
			currentParseTreeGroup->groupRef->startOfSequenceElementDetected = true;
		}
	}
	else
	{
		if(currentParseTreeGroup->parseTreeMinSequenceIndex == 0)
		{
			currentParseTreeGroup->groupRef->startOfSequenceElementDetected = true;
			//cout << "currentParseTreeGroup->groupRef->startOfSequenceElementDetected" << endl;
		}
		if(currentParseTreeGroup->parseTreeMaxSequenceIndex == forwardPropogationSequenceData->sentenceContents->size()-1)
		{
			currentParseTreeGroup->groupRef->endOfSequenceElementDetected = true;
			//cout << "currentParseTreeGroup->groupRef->endOfSequenceElementDetected" << endl;
		}
	}

	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING
		if(SANInodes.parseTreeComponentOnFirstHiddenLayer(parseTreeComponent))
		{
			parseTreeComponent->parseTreeGroupRef->groupRef->startOfSequenceElementDetected = currentParseTreeGroup->groupRef->startOfSequenceElementDetected;
			parseTreeComponent->parseTreeGroupRef->groupRef->endOfSequenceElementDetected = currentParseTreeGroup->groupRef->endOfSequenceElementDetected;
		}
		else
		{
		#else
		if(!SANInodes.parseTreeComponentOnFirstHiddenLayer(parseTreeComponent))
		{
		#endif
			if(!identifyStartEndOfSentenceWordDetected(forwardPropogationSequenceData, parseTreeComponent->parseTreeGroupRef))
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
bool SANIgenerateCompactIdentifyClass::identifyVariableComponentsWrapper(const SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	bool result = true;
								
	if(!identifyVariableComponents(SANIGroupTypes, forwardPropogationSequenceData))
	{	
		result = false;
	}
	
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_CENTRAL_COMPONENTS
	if(!findAndLinkCentralVariationCandidates(forwardPropogationSequenceData))
	{	
		result = false;
	}
	#endif
	
	return result;
}

bool SANIgenerateCompactIdentifyClass::identifyVariableComponents(vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	bool result = true;
	
	bool identifyAVariableComponent = false;
	
	for(int k=0; k<newHiddenLayerGroupsTemp.size(); k++)
	{
		SANIGroupNeuralNetwork* generatedNeuron = newHiddenLayerGroupsTemp[k];
		
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ONE_PER_SENTENCE
		if(!identifyAVariableComponent)
		{
		#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
			if(!SANInodes.aComponentHasPOSambiguousInputs(generatedNeuron))
			{
			#endif
		
				//cout << "generatedNeuron
				bool passEdgeRequirements = true;

				int firstComponentIndex;
				if(forwardPropogationSequenceData->parseSentenceReverse)
				{
					firstComponentIndex = generatedNeuron->components.size()-1;
				}
				else
				{
					firstComponentIndex = 0;
				}
				SANIComponentNeuralNetwork* firstComponentOfGeneratedNeuron = generatedNeuron->components[firstComponentIndex];	

				int lastComponentIndex;
				if(forwardPropogationSequenceData->parseSentenceReverse)
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
					if(identifyVariableFirstLastComponents(SANIGroupTypes, forwardPropogationSequenceData, true, generatedNeuron, firstComponentOfGeneratedNeuron, lastComponentOfGeneratedNeuron, firstComponentOfGeneratedNeuronSource, lastComponentOfGeneratedNeuronSource))
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
						if(identifyVariableFirstLastComponents(SANIGroupTypes, forwardPropogationSequenceData, false, generatedNeuron, lastComponentOfGeneratedNeuron, firstComponentOfGeneratedNeuron, lastComponentOfGeneratedNeuronSource, firstComponentOfGeneratedNeuronSource))
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
			#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
			}
			#endif
			
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ONE_PER_SENTENCE
		}
		#endif
	}
	
	return result;
}

bool SANIgenerateCompactIdentifyClass::identifyVariableFirstLastComponents(vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, bool identifyVariableFirstOrLastComponent, SANIGroupNeuralNetwork* generatedNeuron, const SANIComponentNeuralNetwork* variableComponentOfGeneratedNeuron, const SANIComponentNeuralNetwork* nonvariableComponentOfGeneratedNeuron, SANIGroupNeuralNetwork* variableComponentOfGeneratedNeuronSource, SANIGroupNeuralNetwork* nonvariableComponentOfGeneratedNeuronSource)
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
	markFirstComponentSubNeurons(forwardPropogationSequenceData, generatedNeuron);	//changed SANI1n7c - OLD: markSubNeurons(generatedNeuron);
	#endif
						
	bool passEdgeRequirements = true;
	if(identifyVariableFirstOrLastComponent)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ENSURE_VARIABLE_COMPONENT_ENCAPSULATES_EDGE_OF_SENTENCE
		if(!(variableComponentOfGeneratedNeuronSource->startOfSequenceElementDetected))
		{
			passEdgeRequirements = false;
		}
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ENSURE_NONVARIABLE_COMPONENT_ENCAPSULATES_EDGE_OF_SENTENCE
		if(!(nonvariableComponentOfGeneratedNeuronSource->endOfSequenceElementDetected))
		{
			passEdgeRequirements = false;
		}
		#endif	
	}
	else
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ENSURE_VARIABLE_COMPONENT_ENCAPSULATES_EDGE_OF_SENTENCE
		if(!(variableComponentOfGeneratedNeuronSource->endOfSequenceElementDetected))
		{
			passEdgeRequirements = false;
		}
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_ENSURE_NONVARIABLE_COMPONENT_ENCAPSULATES_EDGE_OF_SENTENCE
		if(!(nonvariableComponentOfGeneratedNeuronSource->startOfSequenceElementDetected))
		{
			passEdgeRequirements = false;
		}
		#endif	
	}
	
	if(passEdgeRequirements)
	{
		//cout << "passEdgeRequirements;" << endl;
		//cout << "generatedNeuron->groupIndex = " << generatedNeuron->groupIndex << endl;
		//SANInodes.printNeuralNetwork(generatedNeuron, 0);

		for(int i=0; i<nonvariableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.size(); i++)
		{
			if(!(generatedNeuron->markToErase))	//only perform merge once
			{
				SANIComponentNeuralNetwork* currentComponent = (nonvariableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList)[i];
				SANIGroupNeuralNetwork* candidateMatchGroup = currentComponent->ownerGroup;
				if(candidateMatchGroup != generatedNeuron)
				{								
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VERIFY_NOT_NEWLY_CREATED
					if(!(candidateMatchGroup->newlyGeneratedForSentenceTemp)) //only consider candidateMatchGroup if neuron has not been newly generated for current sentence	//candidateMatchGroup->timeIndex != forwardPropogationSequenceData->sentenceIndex
					{
					#endif
						int candidateMatchGroupVariableComponentIndex2;
						int candidateMatchGroupNonvariableComponentIndex2;
						
						//cout << "candidateMatchGroup->groupIndex = " << candidateMatchGroup->groupIndex << endl;
						
						if(identifyVariableFirstOrLastComponent)
						{
							if(forwardPropogationSequenceData->parseSentenceReverse)
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
							if(forwardPropogationSequenceData->parseSentenceReverse)
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
									//cout << "identifyVariableFirstOrLastComponent" << endl;	
									#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_FIRST_COMPONENTS
									if(!SANIpropagateCompactGenerateOperations.identifyMissingOrVariableStartComponentFound(forwardPropogationSequenceData, &(candidateMatchGroup->components), candidateMatchGroupVariableComponent))	//requires firstComponent
									{
										//cout << "!identifyMissingOrVariableStartComponentFound" << endl;
										passPreconditions = false;
									}
									#endif
									#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_VERIFY_THAT_VARIABLE_EDGE_COMPONENT_SOURCE_POS_IS_NOT_IDENTICAL
									if(SANIpropagateCompactGenerateOperations.variableEdgeComponentSourcePOSisIdenticalWrapper1(forwardPropogationSequenceData, generatedNeuron, candidateMatchGroup, identifyVariableFirstOrLastComponent))
									{
										//cout << "variableEdgeComponentSourcePOSisIdenticalWrapper1" << endl;
										passPreconditions = false;
									}
									else
									{
										//cout << "!variableEdgeComponentSourcePOSisIdenticalWrapper1" << endl;
									}
									#endif
								}
								else
								{
									#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_LAST_COMPONENTS
									if(!SANIpropagateCompactGenerateOperations.identifyMissingOrVariableEndComponentFound(forwardPropogationSequenceData, &(candidateMatchGroup->components), candidateMatchGroupNonvariableComponent))	//requires secondLastComponentIndex (firstComponent)
									{
										passPreconditions = false;
									}
									#endif
									#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_VERIFY_THAT_VARIABLE_EDGE_COMPONENT_SOURCE_POS_IS_NOT_IDENTICAL
									if(SANIpropagateCompactGenerateOperations.variableEdgeComponentSourcePOSisIdenticalWrapper1(forwardPropogationSequenceData, generatedNeuron, candidateMatchGroup, identifyVariableFirstOrLastComponent))
									{
										passPreconditions = false;
									}
									#endif
								}
								//CHECKTHIS: no additional tests required from SANIpropagateOperationsClass::identifySequentialActivationFound
								
								#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
								cout << "found a variable first/last component candidate match" << endl;
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
								
								#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_DUPLICATE_DETECTION_CULL
								if(passPreconditions || duplicateProspectiveVariableComponentDetected)
								#else
								if(passPreconditions && !duplicateProspectiveVariableComponentDetected)
								#endif
								{	
									//cout << "duplicateProspectiveVariableComponentDetected = " << duplicateProspectiveVariableComponentDetected << endl;

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
									//connect variableComponentSource -> candidateMatchGroup [if !duplicateProspectiveVariableComponentDetected]
									//connect variableComponentOfGeneratedNeuronSource <- candidateMatchGroup [if !duplicateProspectiveVariableComponentDetected]
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
									#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_DUPLICATE_DETECTION_CULL
									if(!duplicateProspectiveVariableComponentDetected)
									{
									#endif
										variableComponentOfGeneratedNeuronSource->SANIfrontComponentConnectionList.push_back(candidateMatchGroup->components[candidateMatchGroupVariableComponentIndex2]);
										candidateMatchGroup->components[candidateMatchGroupVariableComponentIndex2]->SANIbackGroupConnectionList.push_back(variableComponentOfGeneratedNeuronSource);
									#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_DUPLICATE_DETECTION_CULL
									}
									#endif

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
									//connect candidateMatchGroup <- generatedNeuronTarget [if !duplicateProspectiveVariableComponentDetected]
									//connect candidateMatchGroup -> generatedNeuronTarget [if !duplicateProspectiveVariableComponentDetected]
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
											#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_DUPLICATE_DETECTION_CULL
											if(!duplicateProspectiveVariableComponentDetected)
											{
											#endif
												generatedNeuronTargetComponent->SANIbackGroupConnectionList.push_back(candidateMatchGroup);
												candidateMatchGroup->SANIfrontComponentConnectionList.push_back(generatedNeuronTargetComponent);
											#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_DUPLICATE_DETECTION_CULL
											}
											#endif
										}
									}

									//4. erase generatedNeuron
									generatedNeuron->markToErase = true;
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
bool SANIgenerateCompactIdentifyClass::findAndLinkCentralVariationCandidates(SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	//algorithm: find nodes that are connected to the same first input neuron and an output neuron in which the second input of the output neuron is the same
	
	SANIpropagateOperations.setParseSentenceReverse(true, forwardPropogationSequenceData);

	for(int k=0; k<newHiddenLayerGroupsTemp.size(); k++)
	{
		SANIGroupNeuralNetwork* generatedNeuron = newHiddenLayerGroupsTemp[k];

		int firstComponentIndex;
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			firstComponentIndex = generatedNeuron->components.size()-1;
		}
		else
		{
			firstComponentIndex = 0;
		}

		vector<SANIGroupNeuralNetwork*> secondComponentOfOutputNeuronSourceVector1;
		populateSecondComponentOfOutputNeuronSourceVector(forwardPropogationSequenceData, generatedNeuron, &secondComponentOfOutputNeuronSourceVector1);
		
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
					populateSecondComponentOfOutputNeuronSourceVector(forwardPropogationSequenceData, ownerGroup, &secondComponentOfOutputNeuronSourceVector2);
					
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
bool SANIgenerateCompactIdentifyClass::populateSecondComponentOfOutputNeuronSourceVector(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIGroupNeuralNetwork* group, vector<SANIGroupNeuralNetwork*>* secondComponentOfOutputNeuronSourceVector)
{
	bool result = true;
	
	for(int i=0; i<group->SANIfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i];
		SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	

		int lastComponentIndex;	//ie secondComponentIndex
		if(forwardPropogationSequenceData->parseSentenceReverse)
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
	


#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
bool SANIgenerateCompactIdentifyClass::markFirstComponentSubNeurons(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* currentNeuron)
{	
	bool result = true;
	
	if(!SANInodes.isNeuronString(currentNeuron))	//CHECKTHIS
	{
		currentNeuron->marked = true;
		SANIComponentNeuralNetwork* firstComponent = SANInodes.getFirstComponent(forwardPropogationSequenceData, currentNeuron, true);
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
