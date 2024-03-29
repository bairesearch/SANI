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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Operations - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#include "SANIgenerateCompactOperations.hpp"
#include "SHAREDvars.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR


bool SANIgenerateCompactOperationsClass::addComponentToFirstLevelHiddenLayerGroup(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* inputLayerPOSneuron, SANIGroupNeuralNetwork* newNeuronSequenceGroup)
{
	bool result = true;

	addComponentToGroup(forwardPropogationSequenceData, inputLayerPOSneuron, newNeuronSequenceGroup, true, false);
	//SANIformation.createGroupANNconnection(group, higherLevelComponent);

	return result;
}

SANIGroupNeuralNetwork* SANIgenerateCompactOperationsClass::createNewHiddenLayerGroup(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "SANIgenerateCompactOperationsClass::createNewHiddenLayerGroup" << endl;
	#endif
	
	SANIGroupNeuralNetwork* newNeuron = createNewGroup();
	SANInodes.addNeuronToGroupTypes(newNeuron, SANIGroupTypes, false);	
	
	return newNeuron;
}



bool SANIgenerateCompactOperationsClass::addVariableComponentToGroup(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelComponentGroupOwner, bool addToStart)
{
	bool result = true;

	SANIComponentNeuralNetwork* variableComponent = SANInodes.getFirstComponent(forwardPropogationSequenceData, higherLevelComponentGroupOwner, addToStart);
	
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
		addComponentToGroup(forwardPropogationSequenceData, group, variableComponent);
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_PREVENT_DUPLICATE_CONNECTIONS
	}
	#endif
		
	return result;
}


bool SANIgenerateCompactOperationsClass::addComponentToGroup(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelComponentGroupOwner, const bool componentTypeString, bool insertAtStart)
{
	bool result = true;
	
	SANIComponentNeuralNetwork* newComponent = new SANIComponentNeuralNetwork();

	newComponent->ownerGroup = higherLevelComponentGroupOwner;

	//see SANInodes.getFirstComponent
	if(forwardPropogationSequenceData->parseSentenceReverse)
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
		#ifndef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
		newComponent->wordPOStype = group->wordPOStype;
		#endif
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

	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
	if(group->inputLayerNeuronArtificialAmbiguousPOSpermutations)
	{
		for(int wordPOStype=0; wordPOStype<LRP_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		{
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(group->inputLayerNeuronArtificialAmbiguousPOSpermutationsPOSambiguityInfo, wordPOStype))
			{
				SANIGroupNeuralNetwork* inputLayerGroup = SANIformation.getInputGroupLayerSection(SANIformation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
				//cout << "SANIformation.createGroupANNconnection(inputLayerGroup, newComponent), inputLayerGroup->groupIndex = " << inputLayerGroup->groupIndex << ", wordPOStype = " << wordPOStype << endl;
				SANIformation.createGroupANNconnection(inputLayerGroup, newComponent);
			}
		}
		newComponent->POSambiguousInputs = true;
		newComponent->POSambiguousInputsPOSambiguityInfo = group->inputLayerNeuronArtificialAmbiguousPOSpermutationsPOSambiguityInfo;
		delete group;	//CHECKTHIS
	}
	else
	{
	#endif
		SANIformation.createGroupANNconnection(group, newComponent);
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
	}
	#endif
	
	return result;
}

bool SANIgenerateCompactOperationsClass::addComponentToGroup(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* higherLevelComponent)
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
	newGroup->groupTypeName = SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_DEFAULT_NAME;
	
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


bool SANIgenerateCompactOperationsClass::verifyLastSequenceIndex(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIGroupParseTree* parseTreeGroup, const int lastSequenceIndex, bool* adjacent)
{
	bool result = false;
	
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		if(parseTreeGroup->parseTreeMinSequenceIndex >= lastSequenceIndex)
		{
			result = true;
		}
		if(parseTreeGroup->parseTreeMinSequenceIndex == lastSequenceIndex)
		{
			*adjacent = true;
		}
	}
	else
	{
		if(parseTreeGroup->parseTreeMaxSequenceIndex <= lastSequenceIndex)
		{
			result = true;
		}
		if(parseTreeGroup->parseTreeMaxSequenceIndex == lastSequenceIndex)
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


int SANIgenerateCompactOperationsClass::calculateNextIndexInSequence(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	return SANIpropagateCompactGenerateOperations.calculateNextIndexInSequenceProspective(forwardPropogationSequenceData, forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage);
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
bool SANIgenerateCompactOperationsClass::updateHighLevelNeuronHierachy(vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, const int lowestLayerNeuronIndex, SANIGroupNeuralNetwork* grammaticalSentenceNeuronSub, const bool completedIdentifyingSentenceHighLevelNeurons)
{
	bool result = true;
	
	//cout << "\nSANIgenerateCompactOperationsClass::updateHighLevelNeuronHierachy" << endl;
	
	bool foundHigherLevelWithoutNeuronNeuronPrior = false;
	int i = lowestLayerNeuronIndex;
	while(!foundHigherLevelWithoutNeuronNeuronPrior)
	{
		if((i < highLevelNeuronPriorArray->size()) && ((*highLevelNeuronPriorArray)[i] != NULL))
		{			
			SANIGroupNeuralNetwork* highLevelNeuronPriorCurrent = (*highLevelNeuronPriorArray)[i];

			//combine grammaticalSentenceNeuronSub and highLevelNeuronPriorArray[i], to create neuron in layer i+1

			SANIGroupNeuralNetwork* grammaticalSentenceNeuronSubHigher = createNewHiddenLayerGroup(forwardPropogationSequenceData, SANIGroupTypes);
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NETWORK_NODES
			cout << grammaticalSentenceNeuronSubHigher->groupIndex << " ";
			#endif
			bool componentTypeString = SANInodes.calculateComponentTypeString(highLevelNeuronPriorCurrent);
			addComponentToGroup(forwardPropogationSequenceData, highLevelNeuronPriorCurrent, grammaticalSentenceNeuronSubHigher, componentTypeString, false);

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
			addComponentToGroup(forwardPropogationSequenceData, grammaticalSentenceNeuronSub, grammaticalSentenceNeuronSubHigher, componentTypeString, false);

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
				//skip layer until find layer with a neuron prior (as all sequenceElements in sentence have been parsed, the last group found must always be added to a higher level neuron)
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


#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_MARK_AS_UNAMBIGUOUS
bool SANIgenerateCompactOperationsClass::markAmbiguousFirstHiddenLayerNeuronsAsUnambiguous(const SANIGroupParseTree* currentParseTreeGroup, const bool previousUnambiguousDetections)
{
	bool result = true;
			
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentParseComponent = (currentParseTreeGroup->components)[i];

		if(SANInodes.parseTreeComponentOnFirstHiddenLayer(currentParseComponent))
		{
			if(currentParseComponent->parseTreeGroupRef == NULL)
			{
				cerr << "SANIgenerateCompactClass::markAmbiguousFirstHiddenLayerNeuronsAsUnambiguous error: (currentParseComponent->parseTreeGroupRef == NULL)" << endl;
				exit(EXIT_ERROR);
			}

			//currentParseTreeGroup->groupRef->firstHiddenLayerNeuron == true
			SANIComponentNeuralNetwork* currentComponent = currentParseComponent->componentRef;
			if(currentComponent->POSambiguousInputs)
			{
				bool resolvePOSambiguousComponentInputs = false;
				
				if(previousUnambiguousDetections)
				{
					#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_MARK_AS_UNAMBIGUOUS
					if(currentComponent->POSambiguousInputsMarkAsUnambiguous)
					{
						resolvePOSambiguousComponentInputs = true;
						currentComponent->POSambiguousInputsMarkAsUnambiguous = false;
					}
					#else
					cerr << "SANIgenerateCompactOperationsClass::markAmbiguousFirstHiddenLayerNeuronsAsUnambiguous error: previousUnambiguousDetections requires SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_MARK_AS_UNAMBIGUOUS" << endl;
					exit(EXIT_ERROR);	
					#endif
				}
				else
				{
					//pos ambiguous components nodes are only ever resolved when they are parsed with a non-ambiguous sequenceElement (or when only 1 pos value is accepted by the component based on the sentence sequenceElement pos values):
					LRPpreprocessorPlainTextWord* currentWord = currentParseComponent->neuronComponentConnectionActiveSequenceElementRecord->wordReference;	//or currentComponent->neuronComponentConnectionActiveSequenceElementRecord
					bool resolvePOSambiguousComponentInputs = false;
					#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_MARK_AS_UNAMBIGUOUS_VERIFY_POS_UNAMBIGUOUS_WORDS
					if(!LRPpreprocessorPOStagger.isWordPOSambiguous(currentWord))	
					{
						resolvePOSambiguousComponentInputs = true;
					}
					#endif
					#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_MARK_AS_UNAMBIGUOUS_VERIFY_UNIQUE
					else if(SANInodes.isComponentWordPOStypeInferredUnique(currentParseComponent->wordPOStypeInferred, currentComponent->POSambiguousInputsPOSambiguityInfo, currentWord->POSambiguityInfo))
					{
						resolvePOSambiguousComponentInputs = true;
					}
					#endif
				}

				SANIGroupNeuralNetwork* currentComponentBackGroup = currentParseComponent->parseTreeGroupRef->groupRef;	//requires SANI_SEQUENCE_GRAMMAR_PARSE_TREE_SAVE_LEAF_NODES

				if(resolvePOSambiguousComponentInputs)
				{
					cout << "resolvePOSambiguousComponentInputs" << endl;
					
					currentComponent->POSambiguousInputs = false;
					for(int i=0; i<currentComponent->SANIbackGroupConnectionList.size(); )
					{
						SANIGroupNeuralNetwork* currentComponentBackGroup = currentComponent->SANIbackGroupConnectionList[i];
						if(currentComponentBackGroup != currentComponentBackGroup)
						{
							//breakSANIconnection(currentComponentBackGroup, currentComponent);

							//1a.
							//disconnect currentComponentBackGroup -> currentComponent
							int componentRefBackGroupFrontIndexToErase = INT_DEFAULT_VALUE;
							for(int j=0; j<currentComponentBackGroup->SANIfrontComponentConnectionList.size(); j++)
							{
								SANIComponentNeuralNetwork* currentComponent2 = (currentComponentBackGroup->SANIfrontComponentConnectionList)[j];
								if(currentComponent2 == currentComponent)
								{
									componentRefBackGroupFrontIndexToErase = j;
								}
							}
							if(componentRefBackGroupFrontIndexToErase == INT_DEFAULT_VALUE)
							{
								cerr << "SANIgenerateCompactClass::markAmbiguousFirstHiddenLayerNeuronsAsUnambiguous error: (componentRefBackGroupFrontIndexToErase == INT_DEFAULT_VALUE)" << endl;
								exit(EXIT_ERROR);
							}
							currentComponentBackGroup->SANIfrontComponentConnectionList.erase(currentComponentBackGroup->SANIfrontComponentConnectionList.begin() + componentRefBackGroupFrontIndexToErase);

							//1b.
							//disconnect currentComponent -> currentComponentBackGroup
							currentComponent->SANIbackGroupConnectionList.erase(currentComponent->SANIbackGroupConnectionList.begin() + i);
						}
						else
						{
							i++;
						}
					}
				}
			}
		}
		else
		{	
			markAmbiguousFirstHiddenLayerNeuronsAsUnambiguous(currentParseComponent->parseTreeGroupRef, previousUnambiguousDetections);
		}
	}

	return result;
}
#endif


		
#endif
