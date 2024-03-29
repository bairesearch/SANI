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
 * File Name: SANInodes.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Nodes
 * /
 *******************************************************************************/


#include "SANInodes.hpp"


#ifdef SANI_NODES


#ifdef SANI_SEQUENCE_GRAMMAR
static int newNeuronIndex;
#endif

vector<SANIGroupType*>* SANIGroupTypesGlobal;
vector<XMLparserTag*>* SANIrulesTokenLayersGlobal;

void SANInodesClass::setSANIGroupTypesGlobal(vector<SANIGroupType*>* SANIGroupTypes)
{
	SANIGroupTypesGlobal = SANIGroupTypes;
}
vector<SANIGroupType*>* SANInodesClass::getSANIGroupTypesGlobal()
{
	return SANIGroupTypesGlobal;
}
void SANInodesClass::setSANIrulesTokenLayersGlobal(vector<XMLparserTag*>* SANIrulesTokenLayers)
{
	SANIrulesTokenLayersGlobal = SANIrulesTokenLayers;
}
vector<XMLparserTag*>* SANInodesClass::getSANIrulesTokenLayersGlobal()
{
	return SANIrulesTokenLayersGlobal;
}

#ifdef SANI_SEQUENCE_GRAMMAR
void SANInodesClass::initialiseNewGroupIndex(vector<SANIGroupType*>* SANIGroupTypes)
{
	SANIGroupType* groupType = getSequenceGrammarGroupTypeDefault(SANIGroupTypes);
	newNeuronIndex = groupType->groups.size();	//0
}
int* SANInodesClass::getNewGroupIndex()
{
	return &newNeuronIndex;
}
SANIGroupType* SANInodesClass::getSequenceGrammarGroupTypeDefault(vector<SANIGroupType*>* SANIGroupTypes)
{
	SANIGroupType* groupType = getSequenceGrammarGroupType(SANIGroupTypes, SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_DEFAULT_NAME);
	return groupType;
}
#ifdef SANI_SEQUENCE_GRAMMAR_PARSE_TREE_SAVE_LEAF_NODES_ADD_INPUT_NEURONS_TO_GROUPTYPES_ARRAY	
SANIGroupType* SANInodesClass::getSequenceGrammarGroupTypeInputNeurons(vector<SANIGroupType*>* SANIGroupTypes)
{
	SANIGroupType* groupType = getSequenceGrammarGroupType(SANIGroupTypes, SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_INPUT_NEURONS_NAME);
	return groupType;
}
#endif
SANIGroupType* SANInodesClass::getSequenceGrammarGroupType(vector<SANIGroupType*>* SANIGroupTypes, const string groupTypeName)
{
	SANIGroupType* groupType = NULL;
	if(!findGroupType(SANIGroupTypes, groupTypeName, &groupType))
	{
		cerr << "SANInodesClass::getSequenceGrammarGroupTypeDefault error: !findGroupType, groupTypeName = " << groupTypeName << endl;
		exit(EXIT_ERROR);
	}
	return groupType;
}
int SANInodesClass::assignGroupIndex(SANIGroupNeuralNetwork* group)
{	
	int* newNeuronIndexTemp = getNewGroupIndex();
	//cout << "*newNeuronIndexTemp = " << *newNeuronIndexTemp << endl;
	group->groupIndex = *newNeuronIndexTemp;
	*newNeuronIndexTemp = *newNeuronIndexTemp + 1;
	return group->groupIndex;
}
#endif

bool SANInodesClass::findGroupType(vector<SANIGroupType*>* SANIGroupTypes, const string groupTypeName, constEffective SANIGroupType** groupTypeFound)
{
	int groupTypeFoundIndexNOTUSED = INT_DEFAULT_VALUE;
	return findGroupType(SANIGroupTypes, groupTypeName, groupTypeFound, &groupTypeFoundIndexNOTUSED);
}
bool SANInodesClass::findGroupType(vector<SANIGroupType*>* SANIGroupTypes, const string groupTypeName, constEffective SANIGroupType** groupTypeFound, int* groupTypeFoundIndex)
{
	bool result = false;
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		//cout << "groupType->groupTypeName = " << groupType->groupTypeName << endl;
		if(groupType->groupTypeName == groupTypeName)
		{
			*groupTypeFound = groupType;
			*groupTypeFoundIndex = i;
			result = true;
		}
	}
	return result;
}
bool SANInodesClass::findGroup(vector<SANIGroupType*>* SANIGroupTypes, const string groupTypeName, const string groupName, constEffective SANIGroupType** groupTypeFound, SANIGroupNeuralNetwork** groupFound)
{
	bool result = false;
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		if(groupType->groupTypeName == groupTypeName)
		{
			*groupTypeFound = groupType;
			for(int j=0; j<groupType->groups.size(); j++)
			{
				SANIGroupNeuralNetwork* group = (groupType->groups)[j];
				if(group->groupName == groupName)
				{
					*groupFound = group;
					result = true;
				}
			}
		}
	}
	return result;
}





#ifdef SANI_REMOVE_LAST_OPTIONAL_COMPONENTS
bool SANInodesClass::removeLastOptionalComponents(vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		
		int groupTypeGroupsSizeOrig = (groupType->groups).size();
		for(int groupIndex=0; groupIndex<(groupType->groups).size(); groupIndex++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[groupIndex];
			vector<SANIComponentNeuralNetwork*>* components = &(group->components);
					
			if(components->size() > 0)
			{
				#ifdef SANI_REVERSE_DIRECTION
				int lastComponentOptionalComponentIndex = 0;
				#else
				int lastComponentOptionalComponentIndex = components->size()-1;
				#endif
				SANIComponentNeuralNetwork* lastComponent = (*components)[lastComponentOptionalComponentIndex];
				
				#ifdef SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_OR_OPTIONAL_REPEAT
				bool hasOptionalRepeat = false;
				int repeatOptionalComponentIndex = INT_DEFAULT_VALUE;
				for(int q=0; q<group->components.size(); q++)
				{	
					SANIComponentNeuralNetwork* component = group->components[q];
					if((component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT) && (component->optional))
					{
						hasOptionalRepeat = true;
						repeatOptionalComponentIndex = q;
					}
				}
				#endif
		
				if(lastComponent->optional)
				{
					if(components->size() == 1 && lastComponent->optional)
					{
						//group only contains one component and it is optional
						cerr << "SANInodesClass::removeLastOptionalComponents{} warning: (components->size() == 1 && lastComponent->optional) - group only contains one component and it is optional" << endl;
					}
					removeOptionalComponent(groupType, &groupIndex, groupTypeGroupsSizeOrig, group, lastComponentOptionalComponentIndex);
				}
				#ifdef SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_OR_OPTIONAL_REPEAT
				else if(hasOptionalRepeat)
				{
					removeOptionalComponent(groupType, &groupIndex, groupTypeGroupsSizeOrig, group, repeatOptionalComponentIndex);
				}
				#endif
			}
			else
			{
				cerr << "SANInodesClass::removeLastOptionalComponents{} error: " << (components->size() == 0) << endl;
				exit(EXIT_ERROR);
			}
		}
	}
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA7
	cout << "SANI_DEBUG_PROPAGATE_EXTRA7 exit" << endl;
	exit(EXIT_ERROR);
	#endif
	
	return result;
}	

bool SANInodesClass::removeOptionalComponent(SANIGroupType* groupType, int* groupIndex, const int groupTypeGroupsSizeOrig, SANIGroupNeuralNetwork* group, int optionalComponentIndex)
{
	bool result = true;
	
	vector<SANIComponentNeuralNetwork*>* components = &(group->components);
	
	SANIGroupNeuralNetwork* optionalComponentsWrapperGroup = NULL;
	bool wrapperGroupAlreadyDefined = false;
	if(*groupIndex >= groupTypeGroupsSizeOrig)
	{
		wrapperGroupAlreadyDefined = true;
		optionalComponentsWrapperGroup = group->optionalComponentsWrapperGroup;
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA7
		cout << "passed test: (*groupIndex >= groupTypeGroupsSizeOrig)" << endl;
		#endif
	}
			
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA7
	cout << "SANInodesClass::removeOptionalComponent: remove optional component from groupNew1" << endl;
	cout << "SANInodesClass::removeOptionalComponent: set optional component from groupNew1 to optional==false" << endl;
	#endif

	//create groupNew1
	SANIGroupNeuralNetwork* groupNew1 = copyGroup(group);
	groupNew1->groupName = groupNew1->groupName + SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_ARTIFICIAL_GROUP_NAME_APPEND1;
	groupNew1->optionalComponentsWrapperGroup = group;
	//remove optional component from groupNew1;
	vector<SANIComponentNeuralNetwork*>* componentsNew1 = &(groupNew1->components);
	componentsNew1->erase(componentsNew1->begin()+optionalComponentIndex);	//CHECKTHIS
	updateComponentsOwnerGroupAndIndexes(groupNew1, &(groupNew1->components), false, NULL);
	(groupType->groups).push_back(groupNew1);

	//create groupNew2
	SANIGroupNeuralNetwork* groupNew2 = copyGroup(group);
	groupNew2->groupName = groupNew2->groupName + SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_ARTIFICIAL_GROUP_NAME_APPEND2;
	groupNew2->optionalComponentsWrapperGroup = group;
	//set optional component to !optional;
	vector<SANIComponentNeuralNetwork*>* componentsNew2 = &(groupNew2->components);
	SANIComponentNeuralNetwork* optionalComponentNew2 = (*componentsNew2)[optionalComponentIndex];
	optionalComponentNew2->optional = false;
	updateComponentsOwnerGroupAndIndexes(groupNew2, &(groupNew2->components), false, NULL);
	(groupType->groups).push_back(groupNew2);

	SANIComponentNeuralNetwork* artificialGroupOrComponent = NULL;
	if(wrapperGroupAlreadyDefined)
	{
		artificialGroupOrComponent = optionalComponentsWrapperGroup->components[0];
		
		//delete artifical group (reference) from groupTypes and artificialGroupOrComponent->components;
		(groupType->groups).erase((groupType->groups).begin() + *groupIndex);
		(*groupIndex) = (*groupIndex) - 1;
		for(int q=0; q<artificialGroupOrComponent->subComponents.size(); q++)
		{	
			SANIComponentNeuralNetwork* component = artificialGroupOrComponent->subComponents[q];
			if(component->groupRefName == group->groupName)
			{
				(artificialGroupOrComponent->subComponents).erase((artificialGroupOrComponent->subComponents).begin() + q);
				q--;
			}
		}
	}
	else
	{
		//turn group into a wrapper group with an or function pointing to both groups
		optionalComponentsWrapperGroup = group;
		artificialGroupOrComponent = new SANIComponentNeuralNetwork();
		artificialGroupOrComponent->componentIndex = SANI_COMPONENT_INDEX_FIRST;
		#ifdef SANI_FORWARD
		artificialGroupOrComponent->ownerGroup = group;	//enables reverse lookup for ANN
		#endif
		artificialGroupOrComponent->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR;
		components->clear();
		components->push_back(artificialGroupOrComponent);
	}

	//add newly created artificial groups to wrapper group
	SANIComponentNeuralNetwork* artificialGroupComponentNew1 = new SANIComponentNeuralNetwork();
	artificialGroupComponentNew1->groupRefName = groupNew1->groupName;
	artificialGroupComponentNew1->groupTypeRefName = optionalComponentsWrapperGroup->groupTypeName;
	artificialGroupComponentNew1->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
	artificialGroupComponentNew1->componentIndex = artificialGroupOrComponent->subComponents.size();
	artificialGroupComponentNew1->ownerGroup = optionalComponentsWrapperGroup;
	artificialGroupComponentNew1->ownerComponent = artificialGroupOrComponent;
	artificialGroupComponentNew1->isSubcomponent = true;
	artificialGroupComponentNew1->semanticRelationReturnEntity = true;
	artificialGroupOrComponent->subComponents.push_back(artificialGroupComponentNew1);
	SANIComponentNeuralNetwork* artificialGroupComponentNew2 = new SANIComponentNeuralNetwork();
	artificialGroupComponentNew2->groupRefName = groupNew2->groupName;
	artificialGroupComponentNew2->groupTypeRefName = optionalComponentsWrapperGroup->groupTypeName;
	artificialGroupComponentNew2->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
	artificialGroupComponentNew2->componentIndex = artificialGroupOrComponent->subComponents.size();
	artificialGroupComponentNew2->ownerGroup = optionalComponentsWrapperGroup;
	artificialGroupComponentNew2->ownerComponent = artificialGroupOrComponent;
	artificialGroupComponentNew2->isSubcomponent = true;
	artificialGroupComponentNew2->semanticRelationReturnEntity = true;
	artificialGroupOrComponent->subComponents.push_back(artificialGroupComponentNew2);
	for(int c=SANI_COMPONENT_INDEX_FIRST; c<artificialGroupOrComponent->subComponents.size(); c++)
	{
		SANIComponentNeuralNetwork* subcomponent = artificialGroupOrComponent->subComponents[c];
		subcomponent->componentIndex = c;
	}
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA8
	cout << "artificialGroupComponentNew1->groupRefName = " << artificialGroupComponentNew1->groupRefName << endl;
	cout << "artificialGroupComponentNew2->groupRefName = " << artificialGroupComponentNew2->groupRefName << endl;
	#endif
	
	return result;
}

/*
bool SANInodesClass::removeLastOptionalComponents(vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		
		int groupTypeGroupsSizeOrig = (groupType->groups).size();
		for(int j=0; j<(groupType->groups).size(); j++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[j];
			vector<SANIComponentNeuralNetwork*>* components = &(group->components);
			
			SANIGroupNeuralNetwork* optionalComponentsWrapperGroup = NULL;
			bool wrapperGroupAlreadyDefined = false;
			if(j >= groupTypeGroupsSizeOrig)
			{
				wrapperGroupAlreadyDefined = true;
				optionalComponentsWrapperGroup = group->optionalComponentsWrapperGroup;
				#ifdef SANI_DEBUG_PROPAGATE_EXTRA7
				cout << "passed test: (j >= groupTypeGroupsSizeOrig)" << endl;
				#endif
			}
							
			if(components->size() > 0)
			{
				#ifdef SANI_REVERSE_DIRECTION
				SANIComponentNeuralNetwork* lastComponent = (*components)[0];
				#else
				SANIComponentNeuralNetwork* lastComponent = (*components)[components->size()-1];
				#endif
				if(components->size() == 1 && lastComponent->optional)
				{
					//group only contains one component and it is optional
					cerr << "SANInodesClass::removeLastOptionalComponents{} warning: (components->size() == 1 && lastComponent->optional) - group only contains one component and it is optional" << endl;
				}
				if(lastComponent->optional)
				{

					#ifdef SANI_DEBUG_PROPAGATE_EXTRA7
					cout << "SANInodesClass::removeLastOptionalComponents: remove last component from groupNew1" << endl;
					cout << "SANInodesClass::removeLastOptionalComponents: set last component from groupNew1 to optional==false" << endl;
					#endif

					//create groupNew1
					SANIGroupNeuralNetwork* groupNew1 = copyGroup(group);
					groupNew1->groupName = groupNew1->groupName + SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_ARTIFICIAL_GROUP_NAME_APPEND1;
					groupNew1->optionalComponentsWrapperGroup = group;
					//remove last component from groupNew1;
					vector<SANIComponentNeuralNetwork*>* componentsNew1 = &(groupNew1->components);
					#ifdef SANI_REVERSE_DIRECTION
					//componentsNew1.pop_front();
					componentsNew1->erase(componentsNew1->begin());
					#else
					componentsNew1->pop_back();
					#endif
					updateComponentsOwnerGroupAndIndexes(groupNew1, &(groupNew1->components), false, NULL);
					(groupType->groups).push_back(groupNew1);

					//create groupNew2
					SANIGroupNeuralNetwork* groupNew2 = copyGroup(group);
					groupNew2->groupName = groupNew2->groupName + SANI_REMOVE_LAST_OPTIONAL_COMPONENTS_ARTIFICIAL_GROUP_NAME_APPEND2;
					groupNew2->optionalComponentsWrapperGroup = group;
					//set last component to !optional;
					vector<SANIComponentNeuralNetwork*>* componentsNew2 = &(groupNew2->components);
					#ifdef SANI_REVERSE_DIRECTION
					SANIComponentNeuralNetwork* lastComponentNew2 = (*componentsNew2)[0];
					#else
					SANIComponentNeuralNetwork* lastComponentNew2 = (*componentsNew2)[components->size()-1];
					#endif
					lastComponentNew2->optional = false;
					updateComponentsOwnerGroupAndIndexes(groupNew2, &(groupNew2->components), false, NULL);
					(groupType->groups).push_back(groupNew2);

					SANIComponentNeuralNetwork* artificialGroupOrComponent = NULL;
					if(wrapperGroupAlreadyDefined)
					{
						artificialGroupOrComponent = optionalComponentsWrapperGroup->components[0];
						
						//delete artifical group (reference) from groupTypes and artificialGroupOrComponent->components;
						(groupType->groups).erase((groupType->groups).begin() + j);
						j--;
						for(int q=0; q<artificialGroupOrComponent->subComponents.size(); q++)
						{	
							SANIComponentNeuralNetwork* component = artificialGroupOrComponent->subComponents[q];
							if(component->groupRefName == group->groupName)
							{
								(artificialGroupOrComponent->subComponents).erase((artificialGroupOrComponent->subComponents).begin() + q);
								q--;
							}
						}
					}
					else
					{
						//turn group into a wrapper group with an or function pointing to both groups
						optionalComponentsWrapperGroup = group;
						artificialGroupOrComponent = new SANIComponentNeuralNetwork();
						artificialGroupOrComponent->componentIndex = SANI_COMPONENT_INDEX_FIRST;
						#ifdef SANI_FORWARD
						artificialGroupOrComponent->ownerGroup = group;	//enables reverse lookup for ANN
						#endif
						artificialGroupOrComponent->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR;
						components->clear();
						components->push_back(artificialGroupOrComponent);
					}

					//add newly created artificial groups to wrapper group
					SANIComponentNeuralNetwork* artificialGroupComponentNew1 = new SANIComponentNeuralNetwork();
					artificialGroupComponentNew1->groupRefName = groupNew1->groupName;
					artificialGroupComponentNew1->groupTypeRefName = optionalComponentsWrapperGroup->groupTypeName;
					artificialGroupComponentNew1->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
					artificialGroupComponentNew1->componentIndex = artificialGroupOrComponent->subComponents.size();
					artificialGroupComponentNew1->ownerGroup = optionalComponentsWrapperGroup;
					artificialGroupComponentNew1->ownerComponent = artificialGroupOrComponent;
					artificialGroupComponentNew1->isSubcomponent = true;
					artificialGroupComponentNew1->semanticRelationReturnEntity = true;
					artificialGroupOrComponent->subComponents.push_back(artificialGroupComponentNew1);
					SANIComponentNeuralNetwork* artificialGroupComponentNew2 = new SANIComponentNeuralNetwork();
					artificialGroupComponentNew2->groupRefName = groupNew2->groupName;
					artificialGroupComponentNew2->groupTypeRefName = optionalComponentsWrapperGroup->groupTypeName;
					artificialGroupComponentNew2->componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
					artificialGroupComponentNew2->componentIndex = artificialGroupOrComponent->subComponents.size();
					artificialGroupComponentNew2->ownerGroup = optionalComponentsWrapperGroup;
					artificialGroupComponentNew2->ownerComponent = artificialGroupOrComponent;
					artificialGroupComponentNew2->isSubcomponent = true;
					artificialGroupComponentNew2->semanticRelationReturnEntity = true;
					artificialGroupOrComponent->subComponents.push_back(artificialGroupComponentNew2);
					for(int c=SANI_COMPONENT_INDEX_FIRST; c<artificialGroupOrComponent->subComponents.size(); c++)
					{
						SANIComponentNeuralNetwork* subcomponent = artificialGroupOrComponent->subComponents[c];
						subcomponent->componentIndex = c;
					}
					
					#ifdef SANI_DEBUG_PROPAGATE_EXTRA8
					cout << "artificialGroupComponentNew1->groupRefName = " << artificialGroupComponentNew1->groupRefName << endl;
					cout << "artificialGroupComponentNew2->groupRefName = " << artificialGroupComponentNew2->groupRefName << endl;
					#endif
				}
			}
			else
			{
				cerr << "SANInodesClass::removeLastOptionalComponents{} error: " << (components->size() == 0) << endl;
				exit(EXIT_ERROR);
			}
		}
	}
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA7
	cout << "SANI_DEBUG_PROPAGATE_EXTRA7 exit" << endl;
	exit(EXIT_ERROR);
	#endif
	
	return result;
}
*/

#endif
	
SANIGroupNeuralNetwork* SANInodesClass::copyGroup(const SANIGroupNeuralNetwork* group)
{		
	SANIGroupNeuralNetwork* newGroup = new SANIGroupNeuralNetwork(*group);
	newGroup->components.clear();
	copyComponents(&(group->components), &(newGroup->components));

	return newGroup;
}
bool SANInodesClass::copyComponents(const vector<SANIComponentNeuralNetwork*>* components, vector<SANIComponentNeuralNetwork*>* componentsNew)
{	
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (*components)[i];
		copyComponent(currentComponent, componentsNew);
		/*
		SANIComponentNeuralNetwork* newComponent = new SANIComponentNeuralNetwork(*currentComponent);
		componentsNew->push_back(newComponent);
		#ifdef SANI_SUPPORT_COMPONENTS_SUB
		if(SANInodesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			newComponent->subComponents.clear();
			copyComponents(&(currentComponent->subComponents), &(newComponent->subComponents)); 
		}
		#endif
		*/
	}
	
	return result;
}

bool SANInodesClass::copyComponent(const SANIComponentNeuralNetwork* currentComponent, vector<SANIComponentNeuralNetwork*>* componentsNew)
{	
	bool result = true;
	
	SANIComponentNeuralNetwork* newComponent = new SANIComponentNeuralNetwork(*currentComponent);
	componentsNew->push_back(newComponent);
	#ifdef SANI_SUPPORT_COMPONENTS_SUB
	if(SANInodesComponentClassObject.componentHasSubcomponents(currentComponent))
	{
		newComponent->subComponents.clear();
		copyComponents(&(currentComponent->subComponents), &(newComponent->subComponents)); 
	}
	#endif
	
	return result;
}

SANIGroupParseTree* SANInodesClass::copyGroup(const SANIGroupParseTree* group)
{		
	SANIGroupParseTree* newGroup = new SANIGroupParseTree(*group);
	newGroup->components.clear();
	copyComponents(&(group->components), &(newGroup->components));

	return newGroup;
}
bool SANInodesClass::copyComponents(const vector<SANIComponentParseTree*>* components, vector<SANIComponentParseTree*>* componentsNew)
{	
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		SANIComponentParseTree* currentComponent = (*components)[i];
		SANIComponentParseTree* newComponent = new SANIComponentParseTree(*currentComponent);
		componentsNew->push_back(newComponent);
	}
	
	return result;
}


SANIGroupActivationMemory* SANInodesClass::copyGroup(const SANIGroupActivationMemory* group)
{		
	SANIGroupActivationMemory* newGroup = new SANIGroupActivationMemory(*group);
	newGroup->components.clear();
	copyComponents(&(group->components), &(newGroup->components));

	return newGroup;
}



#ifdef SANI_FORWARD
bool SANInodesClass::updateComponentsOwnerGroupAndIndexes(constEffective SANIGroupNeuralNetwork* group, const vector<SANIComponentNeuralNetwork*>* components, const bool isSubcomponent, constEffective SANIComponentNeuralNetwork* ownerComponent)
{
	bool result = true;
	
	for(int c=SANI_COMPONENT_INDEX_FIRST; c<components->size(); c++)
	{
		SANIComponentNeuralNetwork* component = (*components)[c];
		component->componentIndex = c;
		component->ownerGroup = group;
		#ifdef SANI_SUPPORT_COMPONENTS_SUB
		if(isSubcomponent)
		{
			component->ownerComponent = ownerComponent;	//added GIA3i3c
		}
		if(SANInodesComponentClassObject.componentHasSubcomponents(component))
		{
			updateComponentsOwnerGroupAndIndexes(group, &(component->subComponents), true, component);
		}
		#endif
	}
	
	return result;
}
#endif	






#ifdef SANI_FORWARD

//this function returns true if original POS type was undefined/unknown (old function name: currentWordAmbiguous)
bool SANInodesClass::currentWordPOSunknown(const LRPpreprocessorPlainTextWord* currentWord)
{
	bool result = false;
	
	#ifdef SANI_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_UNDEFINED)
	#else
	if(currentWord->POSambiguityInfo == LRP_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN)
	#endif
	{
		result = true;
	}
	
	return result;
}


int SANInodesClass::convertSequenceIndexToSentenceEntityIndex(const int sequenceIndex)
{
	int sentenceEntityIndex = sequenceIndex + SANI_SEQUENCE_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return sentenceEntityIndex;
}
int SANInodesClass::convertSentenceEntityIndexToSequenceIndex(const int sentenceEntityIndex)
{
	int sequenceIndex = sentenceEntityIndex - SANI_SEQUENCE_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return sequenceIndex;
}


#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT_NOUNS_ONLY
bool SANInodesClass::isWordPOStypeNoun(const int wordPOStype)
{
	bool result = false;
	
	if(wordPOStype == LRP_SHARED_POS_TYPE_NOUN)	//what about LRP_SHARED_POS_TYPE_PRONOUN_DEMONSTRATIVE etc from GIAsemRelTranslatorDefs.hpp?
	{
		result = true;
	}
	//sync with (ie based on) <group groupName="nounMultiwordPropernoun"> [includes: <group groupName="nounMultiwordName">]
	else if(wordPOStype == LRP_SHARED_POS_TYPE_PROPERNOUN_FIRST_MALE)
	{
		result = true;
	}
	else if(wordPOStype == LRP_SHARED_POS_TYPE_PROPERNOUN_FIRST_FEMALE)
	{
		result = true;
	}
	else if(wordPOStype == LRP_SHARED_POS_TYPE_PROPERNOUN_FAMILY)
	{
		result = true;
	}
	else if(wordPOStype == LRP_SHARED_POS_TYPE_PROPERNOUN_PLACE)
	{
		result = true;
	}	
	//sync with (ie based on) <group groupName="pronoun">		
	else if(wordPOStype == LRP_SHARED_POS_TYPE_PRONOUN_INDEFINITE)
	{
		result = true;
	}
	else if(wordPOStype == LRP_SHARED_POS_TYPE_PRONOUN_PERSONAL_OBJECT)
	{
		result = true;
	}
	else if(wordPOStype == LRP_SHARED_POS_TYPE_PRONOUN_PERSONAL_SUBJECT)
	{
		result = true;
	}
	else if(wordPOStype == LRP_SHARED_POS_TYPE_PRONOUN_REFLEXIVE)
	{
		result = true;
	}	
	
	else if(wordPOStype == LRP_SHARED_POS_TYPE_VERB)
	{
		result = true;
	}
	
	return result;
}
#endif

#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
int SANInodesClass::countParseTreeLeafSize(const SANIGroupParseTree* currentParseTreeGroup)
{
	int size = 0;
	
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	if(currentParseTreeGroup != NULL)
	{
		size = currentParseTreeGroup->parseTreeMaxSequenceIndex - currentParseTreeGroup->parseTreeMinSequenceIndex + 1;
	}
	#else
	size = countParseTreeLeafSizeUnoptimised(currentParseTreeGroup);
	/*
	if(currentParseTreeGroup != NULL)
	{			
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];

			size++;
						
			size = size + countParseTreeLeafSize(currentComponent->parseTreeGroupRef);
		}
	}
	*/	
	#endif

	
	return size;
}
#endif
int SANInodesClass::countParseTreeLeafSizeUnoptimised(const SANIGroupParseTree* currentParseTreeGroup)
{
	int size = 0;
		
	if(currentParseTreeGroup != NULL)
	{	
		#ifdef SANI_SEQUENCE_GRAMMAR
		if(currentParseTreeGroup->groupRef->groupTypeIsString)
		{
			cout << "currentParseTreeGroup->groupRef->groupTypeIsString" << endl;
		}
		if(currentParseTreeGroup->groupRef->inputLayerNeuron)
		{
			cout << "currentParseTreeGroup->groupRef->inputLayerNeuron" << endl;
		}
		#endif
			
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
						
			size = size + countParseTreeLeafSizeUnoptimised(currentComponent->parseTreeGroupRef);
		}
	}
	
	return size;
}

#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH

bool SANInodesClass::adjustNetworkDepth(SANIGroupNeuralNetwork* group)
{
	bool result = true;
	
	int maxDepth = group->networkDepth;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	if(!(group->counted))
	{
		group->counted = true;
	#endif

		for(int l=0; l<group->SANIfrontComponentConnectionList.size(); l++)
		{
			SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[l];
			SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;

			ownerGroup->networkDepth = max(ownerGroup->networkDepth, maxDepth);

			adjustNetworkDepth(ownerGroup);

		}
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	}
	#endif
	
	return result;
}
#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
bool SANInodesClass::adjustNetworkDepthReset(SANIGroupNeuralNetwork* group)
{
	bool result = true;
	
	if(group->counted)
	{
		group->counted = false;

		for(int l=0; l<group->SANIfrontComponentConnectionList.size(); l++)
		{
			SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[l];
			SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;

			adjustNetworkDepthReset(ownerGroup);
		}
	}
	
	return result;
}

#endif

		
bool SANInodesClass::getNeuralNetworkDepth(SANIGroupNeuralNetwork* currentNeuron, int* maxDepth)
{
	bool result = true;	

	*maxDepth = currentNeuron->networkDepth;
	
	return result;
}
bool SANInodesClass::getNeuralNetworkDepth(const SANIComponentNeuralNetwork* component, int* maxDepth)
{
	bool result = true;
				
	int componentMaxDepth = 0;	
	
	for(int l=0; l<component->SANIbackGroupConnectionList.size(); l++)
	{
		const SANIGroupNeuralNetwork* groupSource = component->SANIbackGroupConnectionList[l];
		componentMaxDepth = max(componentMaxDepth, groupSource->networkDepth);
	}
	*maxDepth = componentMaxDepth;
	
	return result;
}

bool SANInodesClass::countNeuralNetworkMaxLeafSizeAndDepth(SANIGroupNeuralNetwork* currentNeuron, int* maxLeafSize, int* maxDepth)
{
	bool result = true;	

	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	if(!(currentNeuron->counted))
	{
		currentNeuron->counted = true;
	#endif
	
		int maxDepthStart = *maxDepth + 1;
		*maxDepth = maxDepthStart;
	
		if(currentNeuron->inputLayerNeuron)
		{
			*maxLeafSize = *maxLeafSize + 1;
		}
		else
		{
			for(int i=0; i<currentNeuron->components.size(); i++)
			{
				int c = i;

				SANIComponentNeuralNetwork* component = (currentNeuron->components)[c];

				int leafSize = 0;
				int depth = maxDepthStart;
				countNeuralNetworkMaxLeafSizeAndDepth(component, &leafSize, &depth);
				*maxLeafSize = *maxLeafSize + leafSize;
				*maxDepth = max(*maxDepth, depth);
			}
		}
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	}
	#endif
	
	return result;
}
bool SANInodesClass::countNeuralNetworkMaxLeafSizeAndDepth(const SANIComponentNeuralNetwork* component, int* maxLeafSize, int* maxDepth)
{
	bool result = true;
				
	int componentMaxLeafSize = 0;
	int componentMaxDepth = *maxDepth;	
	int maxDepthStart = *maxDepth;
		
	for(int l=0; l<component->SANIbackGroupConnectionList.size(); l++)
	{
		SANIGroupNeuralNetwork* groupSource = component->SANIbackGroupConnectionList[l];
		int leafSize = 0;
		int depth = maxDepthStart; 
		countNeuralNetworkMaxLeafSizeAndDepth(groupSource, &leafSize, &depth);
		componentMaxLeafSize = max(componentMaxLeafSize, leafSize);
		componentMaxDepth = max(componentMaxDepth, depth);
	}
	*maxLeafSize = componentMaxLeafSize;
	*maxDepth = componentMaxDepth;
	
	return result;
}
#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
bool SANInodesClass::countNeuralNetworkMaxLeafSizeAndDepthReset(SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = true;	

	if(currentNeuron->counted)
	{
		currentNeuron->counted = false;
	
		if(currentNeuron->inputLayerNeuron)
		{
		}
		else
		{
			for(int i=0; i<currentNeuron->components.size(); i++)
			{
				int c = i;
				SANIComponentNeuralNetwork* component = (currentNeuron->components)[c];
				
				countNeuralNetworkMaxLeafSizeAndDepthReset(component);
			}
		}
	}
	
	return result;
}
bool SANInodesClass::countNeuralNetworkMaxLeafSizeAndDepthReset(const SANIComponentNeuralNetwork* component)
{
	bool result = true;
				
	for(int l=0; l<component->SANIbackGroupConnectionList.size(); l++)
	{
		SANIGroupNeuralNetwork* groupSource = component->SANIbackGroupConnectionList[l];
		countNeuralNetworkMaxLeafSizeAndDepthReset(groupSource);
	}
	
	return result;
}
#endif
#endif

SANIComponentNeuralNetwork* SANInodesClass::getFirstComponent(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* currentNeuron, bool fromStart)
{
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		fromStart = !fromStart;
	}
	int componentIndex;
	if(fromStart)
	{
		componentIndex = SANI_COMPONENT_INDEX_FIRST;
	}
	else
	{
		componentIndex = SANI_COMPONENT_INDEX_FIRST+currentNeuron->components.size()-1;
	}
	SANIComponentNeuralNetwork* firstComponent = currentNeuron->components[componentIndex];
	
	return firstComponent;
}

SANIComponentParseTree* SANInodesClass::getFirstComponent(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* currentNeuron, bool fromStart)
{
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		fromStart = !fromStart;
	}
	int componentIndex;
	if(fromStart)
	{
		componentIndex = SANI_COMPONENT_INDEX_FIRST;
	}
	else
	{
		componentIndex = SANI_COMPONENT_INDEX_FIRST+currentNeuron->components.size()-1;
	}
	SANIComponentParseTree* firstComponent = currentNeuron->components[componentIndex];
	
	return firstComponent;
}



#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH	
int SANInodesClass::calculateDepthFromBinaryTreeLeafSize(const int numberOfLeafNodesInBinaryTree)
{
	//see http://courses.cs.vt.edu/~cs3114/Fall09/wmcquain/Notes/T03a.BinaryTreeTheorems.pdf
	
	/*
	//assume full and complete tree:
	
	//numberOfLeafNodesInBinaryTree = (n+1)/2
	int nodesInBinaryTree = numberOfLeafNodesInBinaryTree*2 - 1;
	
	//nodesInBinaryTree = 2^(h+1)-1.
	int maxDepth = floor(log2(nodesInBinaryTree));
	*/
	
	//assume full but incomplete tree (ie elongated tree):
	int maxDepth = numberOfLeafNodesInBinaryTree - 1;
	
	return maxDepth;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_RANDOMISE
double SANInodesClass::generateRandomNumber()
{
	double randomNumberBetween0And1 = rand()/double(RAND_MAX);
}
#endif


#endif


bool SANInodesClass::deleteGroup(const SANIGroupNeuralNetwork* group)
{
	deleteComponents(&(group->components));
	delete group;
}
bool SANInodesClass::deleteComponents(const vector<SANIComponentNeuralNetwork*>* components)
{
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (*components)[i];
		#ifdef SANI_SUPPORT_COMPONENTS_SUB
		if(SANInodesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			deleteComponents(&(currentComponent->subComponents));
		}
		#endif
		delete currentComponent;
	}
	
	return result;
}	

bool SANInodesClass::deleteGroup(const SANIGroupParseTree* group)
{
	deleteComponents(&(group->components));
	delete group;
}
bool SANInodesClass::deleteComponents(const vector<SANIComponentParseTree*>* components)
{
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		SANIComponentParseTree* currentComponent = (*components)[i];
		delete currentComponent;
	}
	
	return result;
}	

bool SANInodesClass::deleteGroup(const SANIGroupActivationMemory* group)
{
	deleteComponents(&(group->components));
	delete group;
}	



			
bool SANInodesClass::printGroup(const SANIGroupNeuralNetwork* group, const int layer)
{
	for(int i=0; i<group->components.size(); i++)
	{
		SANIComponentNeuralNetwork* component = (group->components)[i];
		printComponent(component, layer);
	}	
}

bool SANInodesClass::printComponent(const SANIComponentParseTree* component, const int layer)
{
	bool result = true;
	
	printParseTreeDebugIndentation(layer);
	string componentTagRegenerated = "";
	#ifndef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP
	if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordPOStype + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SHAREDvars.convertIntToString(component->wordPOStypeInferred) + CHAR_TAG_CLOSE;
		}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
		else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_word + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->word + CHAR_TAG_CLOSE;
		}
		else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenLayer + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenLayer + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenClass + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenClass + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenType + CHAR_TAG_CLOSE;
		}
		#endif
		
		if(component->candidateStringMatch != NULL)
		{
			componentTagRegenerated = componentTagRegenerated + ", candidateStringMatch = " + component->candidateStringMatch->tagName;
		}
		cout << componentTagRegenerated << endl;
	}
	else if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupTypeNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupTypeRefName + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupRefName + STRING_SPACE + CHAR_TAG_CLOSE;	
		#else
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + CHAR_TAG_CLOSE;		
		#endif
		cout << componentTagRegenerated << endl;
	}
	#else
	if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_NEURON)
	{
		if(hasComponentTypeString(component))
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordPOStype + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SHAREDvars.convertIntToString(component->wordPOStypeInferred) + CHAR_TAG_CLOSE;
			if(component->candidateStringMatch != NULL)
			{
				componentTagRegenerated = componentTagRegenerated + ", candidateStringMatch = " + component->candidateStringMatch->tagName;
			}		
		}
		else
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + CHAR_TAG_CLOSE;		
		}
		cout << componentTagRegenerated << endl;
	}	
	#endif
	
	return result;
}

bool SANInodesClass::printComponent(const SANIComponentNeuralNetwork* component, const int layer)
{
	bool result = true;
	
	printParseTreeDebugIndentation(layer);
	string componentTagRegenerated = "";
	#ifndef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP	
	if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordPOStype + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->wordPOStype + CHAR_TAG_CLOSE;
		}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
		else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_word + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->word + CHAR_TAG_CLOSE;
		}
		else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
		{
			componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenLayer + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenLayer + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenClass + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenClass + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->tokenType + CHAR_TAG_CLOSE;
		}
		#endif

		if(component->candidateStringMatch != NULL)
		{
			componentTagRegenerated = componentTagRegenerated + ", candidateStringMatch = " + component->candidateStringMatch->tagName;
		}
		cout << componentTagRegenerated << endl;
	}
	else if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupTypeNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupTypeRefName + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupNameRef + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->groupRefName + STRING_SPACE + CHAR_TAG_CLOSE;	
		#else
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + CHAR_TAG_CLOSE;	
		#endif
		cout << componentTagRegenerated << endl;
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
	#ifdef SANI_SUPPORT_COMPONENTS_OR
	else if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_or + STRING_SPACE + CHAR_TAG_CLOSE;
		cout << componentTagRegenerated << endl;
		for(int i=0; i<component->subComponents.size(); i++)
		{
			SANIComponentNeuralNetwork* subcomponent = (component->subComponents)[i];
			printComponent(subcomponent, layer+1);
		}
	}
	#endif
	#ifdef SANI_SUPPORT_COMPONENTS_REPEAT
	else if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_repeat + STRING_SPACE + CHAR_TAG_CLOSE;
		cout << componentTagRegenerated << endl;
		for(int i=0; i<component->subComponents.size(); i++)
		{
			SANIComponentNeuralNetwork* subcomponent = (component->subComponents)[i];
			printComponent(subcomponent, layer+1);
		}
	}
	#endif
	#endif
	#else
	if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_NEURON)
	{
		componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + CHAR_TAG_CLOSE;	
		cout << componentTagRegenerated << endl;
	}	
	#endif
	
	return result;
}

bool SANInodesClass::printParseTreeDebugIndentation(const int layer)
{
	bool result = true;
	
	for(int i=0; i<layer; i++)
	{
		cout << "\t";
	}
	
	return result;
}

int SANInodesClass::calculateMinIndexOfMatchesFound(vector<LRPpreprocessorPlainTextWord*>* sentenceContentsSubset)
{	
	int minIndexOfMatchesFound = -1;
	for(int w=0; w<sentenceContentsSubset->size(); w++)
	{
		LRPpreprocessorPlainTextWord* currentWord = sentenceContentsSubset->at(w);
		if(currentWord->alreadyFoundMatch)
		{	
			minIndexOfMatchesFound = w;
		}
	}
	
	return minIndexOfMatchesFound;
}


void SANInodesClass::deleteParseTree(const SANIGroupParseTree* parseTreeGroupToDelete, const int level)
{
	if(parseTreeGroupToDelete != NULL)
	{
		for(int i=0; i<parseTreeGroupToDelete->components.size(); i++)
		{
			SANIComponentParseTree* currentComponent = (parseTreeGroupToDelete->components)[i];

			if(currentComponent->parseTreeGroupRef != NULL)
			{
				deleteParseTree(currentComponent->parseTreeGroupRef, level+1);
			}
		}

		deleteGroup(parseTreeGroupToDelete);
	}
}

SANIGroupParseTree* SANInodesClass::replicateParseTree(const SANIGroupParseTree* parseTreeGroupToReplicate, const int level)
{
	bool result = false;

	SANIGroupParseTree* parseTreeGroupNew = copyGroup(parseTreeGroupToReplicate);
	
	for(int i=0; i<parseTreeGroupToReplicate->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (parseTreeGroupToReplicate->components)[i];
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{
			SANIGroupParseTree* componentParseTreeGroupNew = replicateParseTree(currentComponent->parseTreeGroupRef, level+1);
			(parseTreeGroupNew->components)[i]->parseTreeGroupRef = componentParseTreeGroupNew;
		}
	}
	
	return parseTreeGroupNew;
}

SANIGroupParseTree* SANInodesClass::convertNeuralNetworkGroupToParseTreeGroup(constEffective SANIGroupNeuralNetwork* group)
{
	constEffective SANIGroup* groupBase = group;
	return (static_cast<SANIGroupParseTree*>(groupBase));
}
SANIComponentParseTree* SANInodesClass::convertNeuralNetworkComponentToParseTreeComponent(constEffective SANIComponentNeuralNetwork* component)
{
	constEffective SANIComponent* componentBase = component;
	return (static_cast<SANIComponentParseTree*>(componentBase));
}
SANIGroupActivationMemory* SANInodesClass::convertNeuralNetworkGroupToMemoryActivationGroup(constEffective SANIGroupNeuralNetwork* group)
{
	return (static_cast<SANIGroupActivationMemory*>(group));
}


SANIGroupParseTree* SANInodesClass::convertNeuralNetworkGroupToParseTreeGroupNew(SANIGroupNeuralNetwork* group)
{
	SANIGroup* groupBase = group;
	SANIGroupParseTree* groupNew = new SANIGroupParseTree();
	SANIGroup* groupNewBase = groupNew;
	*groupNewBase = *groupBase;
	#ifdef SANI_SEQUENCE_GRAMMAR
	groupNew->groupRef = group;
	#endif
	return groupNew;
}
SANIComponentParseTree* SANInodesClass::convertNeuralNetworkComponentToParseTreeComponentNew(SANIComponentNeuralNetwork* component)	//why isnt this required?
{
	SANIComponent* componentBase = component;
	SANIComponentParseTree* componentNew = new SANIComponentParseTree();
	SANIComponent* componentNewBase = componentNew;
	*componentNewBase = *componentBase;
	#ifdef SANI_SEQUENCE_GRAMMAR_ALWAYS_STORE_COMPONENT_REFS
	componentNew->componentRef = component;
	#endif	
	return componentNew;
}
SANIGroupActivationMemory* SANInodesClass::convertNeuralNetworkGroupToMemoryActivationGroupNew(const SANIGroupNeuralNetwork* group)
{
	SANIGroupActivationMemory* groupNew = new SANIGroupActivationMemory();
	SANIGroupNeuralNetwork* groupNewBase = groupNew;
	*groupNewBase = *group;
	groupNew->components.clear();
	copyComponents(&(group->components), &(groupNew->components));
	return groupNew;
}


#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
bool SANInodesClass::resetGroupOptimumPathway(const SANIGroupNeuralNetwork* group)
{	
	bool result = true;
	
	resetComponentsOptimumPathway(&(group->components));
	
	return result;
}

bool SANInodesClass::resetComponentsOptimumPathway(const vector<SANIComponentNeuralNetwork*>* components)
{	
	bool result = true;
	
	for(int i1=0; i1<components->size(); i1++)
	{
		SANIComponentNeuralNetwork* currentComponent = (*components)[i1];
		
		currentComponent->optimumPathwayWordList.clear();
		
		if(SANInodesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			resetComponentsOptimumPathway(&(currentComponent->subComponents));
		}
	}
	
	return result;
}
#endif



#ifdef SANI_SEQUENCE_GRAMMAR
int SANInodesClass::calculateCoverage(const SANIGroupParseTree* activatedNeuronWithMaxSequenceIndexCoverage)
{
	int activatedNeuronSequenceIndexCoverage = activatedNeuronWithMaxSequenceIndexCoverage->parseTreeMaxSequenceIndex - activatedNeuronWithMaxSequenceIndexCoverage->parseTreeMinSequenceIndex + 1;
	return activatedNeuronSequenceIndexCoverage;
}

bool SANInodesClass::traceBackpropNeuralNetwork(const SANIGroupNeuralNetwork* currentNeuron, const int level, const int previousComponentIndex, const int previousComponentType)
{
	printParseTreeDebugIndentation(level);
	if(isNeuronString(currentNeuron))
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
		#ifdef SANI_SEQUENCE_GRAMMAR_STORE_SEQUENCEELEMENT_OBJECTS
		cout << "SANInodesClass::traceBackpropNeuralNetwork: prevCompIndex = " << previousComponentIndex << ", prevCompType = " << previousComponentType << ", currentNeuron->sequenceElementObject->wordReference->tagName = " << currentNeuron->sequenceElementObject->wordReference->tagName << endl;
		#else
		cout << "SANInodesClass::traceBackpropNeuralNetwork: prevCompIndex = " << previousComponentIndex << ", prevCompType = " << previousComponentType << ", currentNeuron->groupIndex = " << currentNeuron->groupIndex << endl;
		#endif
		#else
		cout << "SANInodesClass::traceBackpropNeuralNetwork: prevCompIndex = " << previousComponentIndex << ", prevCompType = " << previousComponentType << ", currentNeuron->wordPOStype = " << currentNeuron->wordPOStype << endl;
		#endif
	}
	else
	{
		cout << "SANInodesClass::traceBackpropNeuralNetwork: prevCompIndex = " << previousComponentIndex << ", prevCompType = " << previousComponentType << ", currentNeuron->groupIndex = " << currentNeuron->groupIndex << endl;
	}
	
	for(int i=0; i<currentNeuron->components.size(); i++)
	{
		int c = i;
		
		SANIComponentNeuralNetwork* component = (currentNeuron->components)[c];
					
		for(int l=0; l<component->SANIbackGroupConnectionList.size(); l++)
		{
			SANIGroupNeuralNetwork* groupSource = component->SANIbackGroupConnectionList[l];
			traceBackpropNeuralNetwork(groupSource, level+1, i, component->componentType);
		}	
	}
		
}

bool SANInodesClass::isNeuronString(const SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = false;
	if(currentNeuron->groupTypeIsString)
	{
		//groupType==string detected (FUTURE: set this)
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}

#endif	


#ifdef SANI_SEQUENCE_GRAMMAR
bool SANInodesClass::printParseTreeGroupIndices(const SANIGroupParseTree* currentParseTreeGroup, const int layer)
{
	bool result = true;
	
	printParseTreeDebugIndentation(layer);
	cout << currentParseTreeGroup->groupRef->groupIndex << endl;
			
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];

		if(hasComponentTypeString(currentComponent))
		{
			printParseTreeDebugIndentation(layer+1);
			cout << "componentIndex = " << currentComponent->componentIndex << ", i" << currentComponent->componentRef->ownerGroup->groupIndex << endl;
		}
		else
		{	
			if(currentComponent->parseTreeGroupRef != NULL)
			{
				printParseTreeGroupIndices(currentComponent->parseTreeGroupRef, layer+1);
			}
		}
	}

	return result;
}

string SANInodesClass::printParseTreeGroupIndicesFlat(const SANIGroupParseTree* currentParseTreeGroup)
{
	string groupIndicesString = "";
	
	groupIndicesString = groupIndicesString + SHAREDvars.convertIntToString(currentParseTreeGroup->groupRef->groupIndex) + " ";
			
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];

		if(hasComponentTypeString(currentComponent))
		{
			groupIndicesString = groupIndicesString + "i" + SHAREDvars.convertIntToString(currentComponent->componentRef->ownerGroup->groupIndex) + " ";
		}
		else
		{	
			if(currentComponent->parseTreeGroupRef != NULL)
			{
				groupIndicesString = groupIndicesString + printParseTreeGroupIndicesFlat(currentComponent->parseTreeGroupRef);
			}
		}
	}

	return groupIndicesString;
}

#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	
bool SANInodesClass::printNeuralNetwork(const SANIGroupNeuralNetwork* currentNeuron, const int layer)
{
	bool result = true;

	printParseTreeDebugIndentation(layer);
	cout << "currentNeuron->groupIndex = " << currentNeuron->groupIndex << endl;
	
	for(int i=0; i<currentNeuron->components.size(); i++)
	{				
		SANIComponentNeuralNetwork* component = (currentNeuron->components)[i];
		
		printComponent(component, layer);
		
		for(int j=0; j<component->SANIbackGroupConnectionList.size(); j++)
		{
			SANIGroupNeuralNetwork* componentSource = (component->SANIbackGroupConnectionList)[j];
			
			if(!(componentSource->inputLayerNeuron))	//optional
			{
				if(!printNeuralNetwork(componentSource, layer+1))
				{
					result = false;
				}
			}
			
		}
	}	

	return result;
}
#else
bool SANInodesClass::printNeuralNetwork(const SANIGroupNeuralNetwork* currentNeuron, const int layer)
{
	bool result = true;

	if(currentNeuron->inputLayerNeuron)
	{
		printNeuralNetworkInputNeuron(currentNeuron, layer);	
	}
	else
	{
		printParseTreeDebugIndentation(layer);
		cout << "currentNeuron->groupIndex = " << currentNeuron->groupIndex << endl;

		for(int i=0; i<currentNeuron->components.size(); i++)
		{				
			SANIComponentNeuralNetwork* component = (currentNeuron->components)[i];

			printComponent(component, layer);

			for(int j=0; j<component->SANIbackGroupConnectionList.size(); j++)
			{
				SANIGroupNeuralNetwork* componentSource = (component->SANIbackGroupConnectionList)[j];
				if(!printNeuralNetwork(componentSource, layer+1))
				{
					result = false;
				}
			}
		}
	}	

	return result;
}
#endif
#ifndef GIA_POS_REL_TRANSLATOR_RULES_USE	
bool SANInodesClass::printNeuralNetworkInputNeuron(const SANIGroupNeuralNetwork* inputNeuron, const int layer)
{
	printParseTreeDebugIndentation(layer);
	cout << "inputNeuron->groupIndex = " << inputNeuron->groupIndex << endl;
	
	//CHECKTHIS
	/*
	printParseTreeDebugIndentation(layer);

	string componentTagRegenerated = "";
	componentTagRegenerated = componentTagRegenerated + CHAR_TAG_OPEN + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentTypes[component->componentType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + SANIGroupsComponentStringTypes[component->stringType] + STRING_SPACE + GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordPOStype + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + component->wordPOStype + CHAR_TAG_CLOSE;
	if(component->candidateStringMatch != NULL)
	{
		componentTagRegenerated = componentTagRegenerated + ", candidateStringMatch = " + component->candidateStringMatch->tagName;
	}
	cout << componentTagRegenerated << endl;
	*/
}
#endif


#endif


bool SANInodesClass::printParseTree(const SANIGroupParseTree* currentParseTreeGroup, const int layer)
{
	bool result = true;
	
	#ifdef SANI_SEQUENCE_GRAMMAR
	printParseTreeDebugIndentation(layer);
	cout << "currentParseTreeGroup->groupRef->groupIndex = " << currentParseTreeGroup->groupRef->groupIndex << endl;
	#endif
	
	/*
	if(currentParseTreeGroup->components.size() == 1)
	{
		cout << "currentParseTreeGroup->components.size() == 1" << endl;
	}
	*/
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		printComponent(parseTreeComponent, layer);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!printParseTree(parseTreeComponent->parseTreeGroupRef, layer+1))
			{
				result = false;
			}
		}
	}

	return result;
}




bool SANInodesClass::hasComponentTypeString(const SANIComponentNeuralNetwork* component)
{
	bool result = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP
	if(component->neuralNetworkComponentHasTypeString)
	{
		result = true;
	}
	/*
	for(int j=0; j<component->SANIbackGroupConnectionList.size(); j++)
	{
		SANIGroupNeuralNetwork* componentSource = (component->SANIbackGroupConnectionList)[j];
		if(componentSource->inputLayerNeuron)
		{
			result = true;
		}
	}
	*/
	#else	
	if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		result = true;
	}
	#endif
	return result;
}

bool SANInodesClass::hasComponentTypeString(const SANIComponentParseTree* component)
{
	bool result = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP
	if(component->parseTreeComponentTypeString)
	#else
	if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	#endif
	{
		result = true;
	}
	return result;
}		

#ifdef SANI_SEQUENCE_GRAMMAR
bool SANInodesClass::calculateComponentTypeString(const SANIGroupNeuralNetwork* componentSource)
{
	bool componentTypeString = false;
	if(componentSource->inputLayerNeuron)
	{
		componentTypeString = true;
	}
	return componentTypeString;
	/*
	int componentType;
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP
	componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_NEURON;
	#else
	componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP;
	if(componentSource->inputLayerNeuron)
	{
		componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING;
	}
	#endif
	return componentType;
	*/
}
#endif

bool SANInodesClass::parseTreeComponentOnFirstHiddenLayer(const SANIComponentParseTree* parseTreeComponent)	//parseTreeComponentIsString
{
	bool result = false;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_PARSE_TREE_SAVE_LEAF_NODES_COMPATIBILITY
	if(hasComponentTypeString(parseTreeComponent))
	#else
	if(parseTreeComponent->parseTreeGroupRef == NULL)
	#endif
	{
		result = true;
	}
	
	return result;
}
bool SANInodesClass::parseTreeNodeInputLayer(const SANIGroupParseTree* parseTreeNode)
{
	bool result = false;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_PARSE_TREE_SAVE_LEAF_NODES_COMPATIBILITY
	if(parseTreeNode->groupRef->inputLayerNeuron)
	#else
	if(parseTreeNode == NULL)
	#endif
	{
		result = true;
	}
	
	return result;
}


#ifdef SANI_SEQUENCE_GRAMMAR
void SANInodesClass::addNeuronToGroupTypes(SANIGroupNeuralNetwork* newNeuron, vector<SANIGroupType*>* SANIGroupTypes, const bool inputNeuron)
{	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_GROUP_INDICES
	cout << "addNeuronToGroupTypes::addNeuronToGroupTypes" << endl;
	#endif
	
	SANIGroupType* groupType = NULL;
	#ifdef SANI_SEQUENCE_GRAMMAR_PARSE_TREE_SAVE_LEAF_NODES_ADD_INPUT_NEURONS_TO_GROUPTYPES_ARRAY
	if(inputNeuron)
	{
		groupType = getSequenceGrammarGroupTypeInputNeurons(SANIGroupTypes);	//ie getSequenceGrammarGroupType(SANIGroupTypes, SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_INPUT_NEURONS_NAME);
	}
	else
	{
	#endif
		groupType = getSequenceGrammarGroupTypeDefault(SANIGroupTypes);	//ie getSequenceGrammarGroupType(SANIGroupTypes, SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_DEFAULT_NAME);
	#ifdef SANI_SEQUENCE_GRAMMAR_PARSE_TREE_SAVE_LEAF_NODES_ADD_INPUT_NEURONS_TO_GROUPTYPES_ARRAY
	}
	#endif
	groupType->groups.push_back(newNeuron);
}
void SANInodesClass::createGroupType(vector<SANIGroupType*>* SANIGroupTypes, const string groupTypeName)
{
	SANIGroupType* groupType = new SANIGroupType();
	groupType->groupTypeName = groupTypeName;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	groupType->referenceSetType = SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_REFERENCE_SET_TYPE;
	#endif
	SANIGroupTypes->push_back(groupType);	
}
#endif

//#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_MARK_AS_UNAMBIGUOUS_VERIFY_UNIQUE or SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_VERIFY_UNIQUE
bool SANInodesClass::isComponentWordPOStypeInferredUnique(const int componentWordPOStypeInferred, const uint64_t componentPOSambiguousInputsPOSambiguityInfo, const uint64_t currentWordPOSambiguityInfo)
{
	bool result = true;

	for(int wordPOStype=0; wordPOStype<LRP_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
	{
		if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(componentPOSambiguousInputsPOSambiguityInfo, wordPOStype))
		{
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWordPOSambiguityInfo, wordPOStype))
			{
				if(componentWordPOStypeInferred != wordPOStype)
				{
					result = false;
				}
			}
		}
	}
				
	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
bool SANInodesClass::aComponentHasPOSambiguousInputs(const SANIGroupNeuralNetwork* neuron)
{
	bool result = false;

	//if(firstHiddenLayerNeuron)
	for(int i=0; i<neuron->components.size(); i++)
	{				
		SANIComponentNeuralNetwork* component = (neuron->components)[i];
		if(component->POSambiguousInputs)
		{
			result = true;
		}
	}
				
	return result;
}
#endif


#endif
