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
 * File Name: SANIgenerateCompactOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1o5a 25-February-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Operations - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#include "SANIgenerateCompactOperations.hpp"
#include "SHAREDvars.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR


bool SANIgenerateCompactOperationsClass::addComponentToFirstLevelHiddenLayerGroup(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* inputLayerPOSneuron, SANIGroupNeuralNetwork* newNeuronSequenceGroup)
{
	bool result = true;

	addComponentToGroup(forwardPropogationSentenceData, inputLayerPOSneuron, newNeuronSequenceGroup, true, false);
	//SANIformation.createGroupANNconnection(group, higherLevelComponent);

	return result;
}

SANIGroupNeuralNetwork* SANIgenerateCompactOperationsClass::createNewHiddenLayerGroup(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIgenerateCompactOperationsClass::createNewHiddenLayerGroup" << endl;
	#endif
	
	SANIGroupNeuralNetwork* newNeuron = createNewGroup();
	SANIGroupType* groupType = SANInodes.getSequenceGrammarGroupTypeDefault(SANIGroupTypes);	
	groupType->groups.push_back(newNeuron);
	
	return newNeuron;
}



bool SANIgenerateCompactOperationsClass::addVariableComponentToGroup(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelComponentGroupOwner, bool addToStart)
{
	bool result = true;

	SANIComponentNeuralNetwork* variableComponent = SANInodes.getFirstComponent(forwardPropogationSentenceData, higherLevelComponentGroupOwner, addToStart);
	
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING
	if(addToStart)
	{
		if(SANInodes.hasComponentTypeString(variableComponent))	//(variableComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		{
			cerr << "SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING: SANIgenerateCompactOperationsClass::addVariableComponentToGroup error: SANInodes.hasComponentTypeString(variableComponent)" << endl;
			exit(EXIT_ERROR);
		}
	}
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS_NON_STRING
	if(!addToStart)
	{
		if(SANInodes.hasComponentTypeString(variableComponent))	//(variableComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		{
			cerr << "SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS_NON_STRING: SANIgenerateCompactOperationsClass::addVariableComponentToGroup error: SANInodes.hasComponentTypeString(variableComponent)" << endl;
			exit(EXIT_ERROR);
		}
	}
	#endif
					
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_PREVENT_DUPLICATE_CONNECTIONS
	bool duplicateConnectionFound = false;
	for(int j=0; j<variableComponent->SANIbackGroupConnectionList.size(); j++)
	{
		SANIGroupNeuralNetwork* subGroup = (variableComponent->SANIbackGroupConnectionList)[j];
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


bool SANIgenerateCompactOperationsClass::addComponentToGroup(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelComponentGroupOwner, const bool componentTypeString, bool insertAtStart)
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
			cerr << "\nSANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS: SANIgenerateCompactOperationsClass::addComponentToGroup error:" << endl;
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

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	//should never be true
	newComponent->groupRefName = group->groupName;
	newComponent->groupTypeRefName = group->groupTypeName;
	#endif

	/*
	this is only for initialisation (SANIformation) and parseTreeGroup components?;
	newComponent->groupRef = group;	//newComponent->ownerGroup instead
	newComponent->groupTypeRef = group->groupType;
	*/
	if(componentTypeString)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP
		newComponent->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_NEURON;
		newComponent->neuralNetworkComponentHasTypeString = true;
		#else
		newComponent->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING;
		newComponent->stringType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS;
		newComponent->wordPOStype = group->wordPOStype;	
		#endif
	}
	else
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP
		newComponent->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_NEURON;
		#else
		newComponent->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
		#endif
	}

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIgenerateCompactOperationsClass::addComponentToGroup: higherLevelComponentGroupOwner->groupIndex = " << higherLevelComponentGroupOwner->groupIndex << ", newComponent->componentIndex = " << newComponent->componentIndex << endl;
	#endif

	SANIformation.createGroupANNconnection(group, newComponent);

	return result;
}

bool SANIgenerateCompactOperationsClass::addComponentToGroup(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* higherLevelComponent)
{
	bool result = true;

	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP
	if(group->inputLayerNeuron)
	{
		higherLevelComponent->neuralNetworkComponentHasTypeString = true;
	}
	#endif
	
	SANIformation.createGroupANNconnection(group, higherLevelComponent);
	
	return result;
}


SANIGroupNeuralNetwork* SANIgenerateCompactOperationsClass::createNewGroup()
{
	SANIGroupNeuralNetwork* newGroup = new SANIGroupNeuralNetwork();
	newGroup->groupName = SANI_SEQUENCE_GRAMMAR_GROUP_NAME;
	newGroup->groupTypeName = SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_NAME;
	
	int newNeuronIndex = SANInodes.assignGroupIndex(newGroup);

	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_NEWLY_GENERATED_NEURONS
	SANIgenerateCompactIdentify.addGroupToHiddenLayerGroupsTemp(newGroup);
	#endif
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIgenerateCompactOperationsClass::createNewGroup - groupIndex = " << newNeuronIndex << endl;
	#endif
	#ifdef SANI_ANN
	string formatString = string("%0") + SHAREDvars.convertIntToString(SANI_ANN_GROUP_INDEX_DISPLAY_FORMAT_NUMBER_CHARACTERS) + "d";
	newGroup->initiateANNneuron("groupIndex:" + SHAREDvars.convertIntToString(newNeuronIndex, formatString));	//SANI_SEQUENCE_GRAMMAR_NEURON_NAME	//newGroup->groupTypeName + ":" + newGroup->groupName
	#endif
	
	return newGroup;
}

bool SANIgenerateCompactOperationsClass::findGroupDirectlyAbove(const SANIGroupNeuralNetwork* group1, const SANIGroupNeuralNetwork* group2)
{
	bool result = false;
	
	for(int i=0; i<group1->SANIfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (group1->SANIfrontComponentConnectionList)[i];

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


bool SANIgenerateCompactOperationsClass::verifyLastWordIndex(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIGroupParseTree* parseTreeGroup, const int lastWordIndex, bool* adjacent)
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

#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_DO_NOT_HAVE_CONNECTIONS_TO_ANY_NEW_CANDIDATE
bool SANIgenerateCompactOperationsClass::addParentNeuronsToList(vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeuronsCompleteHistory, SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = true;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	if(!currentNeuron->counted)
	{
		currentNeuron->counted = true;
	#endif

		#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
		if(currentNeuron->verified)
		{
			cout << "SANIgenerateCompactOperationsClass::addParentNeuronsToList error: currentNeuron has already been parsed (circular loop detected)" << endl;
			exit(EXIT_ERROR);
		}
		currentNeuron->verified = true;
		#endif

		listOfHighLevelNeuronsCompleteHistory->push_back(currentNeuron);

		for(int j=0; j<currentNeuron->SANIfrontComponentConnectionList.size(); j++)
		{
			SANIComponentNeuralNetwork* parentComponent = (currentNeuron->SANIfrontComponentConnectionList)[j];
			SANIGroupNeuralNetwork* higherLevelGroup = parentComponent->ownerGroup;

			addParentNeuronsToList(listOfHighLevelNeuronsCompleteHistory, higherLevelGroup);
		}

		#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
		currentNeuron->verified = false;
		#endif
		
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	}
	#endif
	
	return result;
}
#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
bool SANIgenerateCompactOperationsClass::addParentNeuronsToListReset(SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = true;
	
	if(currentNeuron->counted)
	{
		currentNeuron->counted = false;
		
		for(int j=0; j<currentNeuron->SANIfrontComponentConnectionList.size(); j++)
		{
			SANIComponentNeuralNetwork* parentComponent = (currentNeuron->SANIfrontComponentConnectionList)[j];
			SANIGroupNeuralNetwork* higherLevelGroup = parentComponent->ownerGroup;

			addParentNeuronsToListReset(higherLevelGroup);
		}
	}
	
	return result;
}
#endif
#endif
#endif


int SANIgenerateCompactOperationsClass::calculateNextIndexInSequence(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	return SANIpropagateCompactGenerateOperations.calculateNextIndexInSequenceProspective(forwardPropogationSentenceData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage);
}




#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
bool SANIgenerateCompactOperationsClass::findLowestLayerNeuron(const vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** lowestLayerNeuron, int* lowestLayerNeuronIndex)
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
bool SANIgenerateCompactOperationsClass::findHighestLayerNeuron(const vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** highestLayerNeuron, int* highestLayerNeuronIndex)
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
bool SANIgenerateCompactOperationsClass::updateHighLevelNeuronHierachy(vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, const int lowestLayerNeuronIndex, SANIGroupNeuralNetwork* grammaticalSentenceNeuronSub, const bool completedIdentifyingSentenceHighLevelNeurons)
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
			bool componentTypeString = SANInodes.calculateComponentTypeString(highLevelNeuronPriorCurrent);
			addComponentToGroup(forwardPropogationSentenceData, highLevelNeuronPriorCurrent, grammaticalSentenceNeuronSubHigher, componentTypeString, false);

			#ifdef SANI_DEBUG_FORMATION2
			SANInodes.printParseTreeDebugIndentation(i);
			cout << "(*highLevelNeuronPriorArray)[i] != NULL), i = " << i << endl;
			SANInodes.printParseTreeDebugIndentation(i);
			cout << "adding highLevelNeuronPriorCurrent->groupIndex = " << highLevelNeuronPriorCurrent->groupIndex << " to grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
			#endif
			
			#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
			grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, highLevelNeuronPriorCurrent->networkDepth + 1);
			#endif

			componentTypeString = SANInodes.calculateComponentTypeString(grammaticalSentenceNeuronSub);
			addComponentToGroup(forwardPropogationSentenceData, grammaticalSentenceNeuronSub, grammaticalSentenceNeuronSubHigher, componentTypeString, false);

			#ifdef SANI_DEBUG_FORMATION2
			SANInodes.printParseTreeDebugIndentation(i);
			cout << "adding grammaticalSentenceNeuronSub->groupIndex = " << grammaticalSentenceNeuronSub->groupIndex << " to grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
			#endif
			
			#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
			grammaticalSentenceNeuronSubHigher->networkDepth = max(grammaticalSentenceNeuronSubHigher->networkDepth, grammaticalSentenceNeuronSub->networkDepth + 1);
			#endif

			grammaticalSentenceNeuronSub = grammaticalSentenceNeuronSubHigher;


			#ifdef SANI_DEBUG_FORMATION
			cout << "\t\t SANIgenerateCompactOperationsClass::connectListOfHighLevelNeuronsToNewNeuronLimitNumComponentsSection: grammaticalSentenceNeuronSubHigher->groupIndex = " << grammaticalSentenceNeuronSubHigher->groupIndex << endl;
			#endif

			#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
			SANInodes.adjustNetworkDepth(grammaticalSentenceNeuronSubHigher);
			#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
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
