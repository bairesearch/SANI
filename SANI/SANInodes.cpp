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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1m5f 01-September-2020
 * Requirements: requires plain text file
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
	SANIGroupType* groupType = NULL;
	if(!findGroupType(SANIGroupTypes, SANI_SEQUENCE_GRAMMAR_GROUP_TYPE_NAME, &groupType))
	{
		cerr << "SANInodesClass::getSequenceGrammarGroupTypeDefault error: !findGroupType" << endl;
		exit(EXIT_ERROR);
	}
	return groupType;
}
int SANInodesClass::assignGroupIndex(SANIGroupNeuralNetwork* group)
{	
	int* newNeuronIndex = getNewGroupIndex();
	group->groupIndex = *newNeuronIndex;
	*newNeuronIndex = *newNeuronIndex + 1;
	return *newNeuronIndex;
}
#endif

bool SANInodesClass::findGroupType(vector<SANIGroupType*>* SANIGroupTypes, const string groupTypeName, SANIGroupType** groupTypeFound)
{
	bool result = false;
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		//cout << "groupType->groupTypeName = " << groupType->groupTypeName << endl;
		if(groupType->groupTypeName == groupTypeName)
		{
			*groupTypeFound = groupType;
			result = true;
		}
	}
	return result;
}
bool SANInodesClass::findGroup(vector<SANIGroupType*>* SANIGroupTypes, const string groupTypeName, const string groupName, SANIGroupType** groupTypeFound, SANIGroupNeuralNetwork** groupFound)
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

bool SANInodesClass::removeOptionalComponent(SANIGroupType* groupType, int* groupIndex, const int groupTypeGroupsSizeOrig, SANIGroupNeuralNetwork* group, const int optionalComponentIndex)
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
		#ifdef SANI
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
						#ifdef SANI
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
	
SANIGroupNeuralNetwork* SANInodesClass::copyGroup(SANIGroupNeuralNetwork* group)
{		
	SANIGroupNeuralNetwork* newGroup = new SANIGroupNeuralNetwork(*group);
	newGroup->components.clear();
	copyComponents(&(group->components), &(newGroup->components));

	return newGroup;
}
bool SANInodesClass::copyComponents(vector<SANIComponentNeuralNetwork*>* components, vector<SANIComponentNeuralNetwork*>* componentsNew)
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

bool SANInodesClass::copyComponent(SANIComponentNeuralNetwork* currentComponent, vector<SANIComponentNeuralNetwork*>* componentsNew)
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

SANIGroupParseTree* SANInodesClass::copyGroup(SANIGroupParseTree* group)
{		
	SANIGroupParseTree* newGroup = new SANIGroupParseTree(*group);
	newGroup->components.clear();
	copyComponents(&(group->components), &(newGroup->components));

	return newGroup;
}
bool SANInodesClass::copyComponents(vector<SANIComponentParseTree*>* components, vector<SANIComponentParseTree*>* componentsNew)
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


SANIGroupActivationMemory* SANInodesClass::copyGroup(SANIGroupActivationMemory* group)
{		
	SANIGroupActivationMemory* newGroup = new SANIGroupActivationMemory(*group);
	newGroup->components.clear();
	copyComponents(&(group->components), &(newGroup->components));

	return newGroup;
}



#ifdef SANI
bool SANInodesClass::updateComponentsOwnerGroupAndIndexes(SANIGroupNeuralNetwork* group, vector<SANIComponentNeuralNetwork*>* components, const bool isSubcomponent, SANIComponentNeuralNetwork* ownerComponent)
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






#ifdef SANI

bool SANInodesClass::currentWordAmbiguous(GIApreprocessorPlainTextWord* currentWord)
{
	bool result = false;
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == GIA_PREPROCESSOR_POS_TYPE_UNDEFINED)
	#else
	if(currentWord->POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN)
	#endif
	{
		result = true;
	}
	
	return result;
}


int SANInodesClass::convertWordIndexToSentenceEntityIndex(int wordIndex)
{
	int sentenceEntityIndex = wordIndex + SANI_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return sentenceEntityIndex;
}
int SANInodesClass::convertSentenceEntityIndexToWordIndex(int sentenceEntityIndex)
{
	int wordIndex = sentenceEntityIndex - SANI_WORD_INDEX_W_TO_ENTITY_INDEX_OFFSET;
	return wordIndex;
}


#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
bool SANInodesClass::isWordPOStypeNoun(int wordPOStype)
{
	bool result = false;
	
	if(wordPOStype == GIA_SHARED_POS_TYPE_NOUN)	//what about GIA_SHARED_POS_TYPE_PRONOUN_DEMONSTRATIVE etc from GIAsemRelTranslatorDefs.hpp?
	{
		result = true;
	}
	//sync with (ie based on) <group groupName="nounMultiwordPropernoun"> [includes: <group groupName="nounMultiwordName">]
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_FIRST_MALE)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_FIRST_FEMALE)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_FAMILY)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PROPERNOUN_PLACE)
	{
		result = true;
	}	
	//sync with (ie based on) <group groupName="pronoun">		
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_INDEFINITE)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_PERSONAL_OBJECT)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_PERSONAL_SUBJECT)
	{
		result = true;
	}
	else if(wordPOStype == GIA_SHARED_POS_TYPE_PRONOUN_REFLEXIVE)
	{
		result = true;
	}	
	
	else if(wordPOStype == GIA_SHARED_POS_TYPE_VERB)
	{
		result = true;
	}
	
	return result;
}
#endif

#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
int SANInodesClass::countParseTreeLeafSize(SANIGroupParseTree* currentParseTreeGroup)
{
	int size = 0;
	
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	if(currentParseTreeGroup != NULL)
	{
		size = currentParseTreeGroup->parseTreeMaxWordIndex - currentParseTreeGroup->parseTreeMinWordIndex + 1;
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
int SANInodesClass::countParseTreeLeafSizeUnoptimised(SANIGroupParseTree* currentParseTreeGroup)
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
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	if(!(group->counted))
	{
		group->counted = true;
	#endif

		for(int l=0; l<group->ANNfrontComponentConnectionList.size(); l++)
		{
			SANIComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[l];
			SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;

			ownerGroup->networkDepth = max(ownerGroup->networkDepth, maxDepth);

			adjustNetworkDepth(ownerGroup);

		}
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	}
	#endif
	
	return result;
}
#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
bool SANInodesClass::adjustNetworkDepthReset(SANIGroupNeuralNetwork* group)
{
	bool result = true;
	
	if(group->counted)
	{
		group->counted = false;

		for(int l=0; l<group->ANNfrontComponentConnectionList.size(); l++)
		{
			SANIComponentNeuralNetwork* currentComponent = (group->ANNfrontComponentConnectionList)[l];
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
bool SANInodesClass::getNeuralNetworkDepth(SANIComponentNeuralNetwork* component, int* maxDepth)
{
	bool result = true;
				
	int componentMaxDepth = 0;	
	
	for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
	{
		SANIGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
		componentMaxDepth = max(componentMaxDepth, groupSource->networkDepth);
	}
	*maxDepth = componentMaxDepth;
	
	return result;
}

bool SANInodesClass::countNeuralNetworkMaxLeafSizeAndDepth(SANIGroupNeuralNetwork* currentNeuron, int* maxLeafSize, int* maxDepth)
{
	bool result = true;	

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
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
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	}
	#endif
	
	return result;
}
bool SANInodesClass::countNeuralNetworkMaxLeafSizeAndDepth(SANIComponentNeuralNetwork* component, int* maxLeafSize, int* maxDepth)
{
	bool result = true;
				
	int componentMaxLeafSize = 0;
	int componentMaxDepth = *maxDepth;	
	int maxDepthStart = *maxDepth;
		
	for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
	{
		SANIGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
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
#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
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
bool SANInodesClass::countNeuralNetworkMaxLeafSizeAndDepthReset(SANIComponentNeuralNetwork* component)
{
	bool result = true;
				
	for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
	{
		SANIGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
		countNeuralNetworkMaxLeafSizeAndDepthReset(groupSource);
	}
	
	return result;
}
#endif
#endif

SANIComponentNeuralNetwork* SANInodesClass::getFirstComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentNeuron, bool fromStart)
{
	if(forwardPropogationSentenceData->parseSentenceReverse)
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

SANIComponentParseTree* SANInodesClass::getFirstComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentNeuron, bool fromStart)
{
	if(forwardPropogationSentenceData->parseSentenceReverse)
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
int SANInodesClass::calculateDepthFromBinaryTreeLeafSize(int numberOfLeafNodesInBinaryTree)
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


bool SANInodesClass::deleteGroup(SANIGroupNeuralNetwork* group)
{
	deleteComponents(&(group->components));
	delete group;
}
bool SANInodesClass::deleteComponents(vector<SANIComponentNeuralNetwork*>* components)
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

bool SANInodesClass::deleteGroup(SANIGroupParseTree* group)
{
	deleteComponents(&(group->components));
	delete group;
}
bool SANInodesClass::deleteComponents(vector<SANIComponentParseTree*>* components)
{
	bool result = true;
	
	for(int i=0; i<components->size(); i++)
	{
		SANIComponentParseTree* currentComponent = (*components)[i];
		delete currentComponent;
	}
	
	return result;
}	

bool SANInodesClass::deleteGroup(SANIGroupActivationMemory* group)
{
	deleteComponents(&(group->components));
	delete group;
}	



			
bool SANInodesClass::printGroup(SANIGroupNeuralNetwork* group, int layer)
{
	for(int i=0; i<group->components.size(); i++)
	{
		SANIComponentNeuralNetwork* component = (group->components)[i];
		printComponent(component, layer);
	}	
}

bool SANInodesClass::printComponent(SANIComponentParseTree* component, int layer)
{
	bool result = true;
	
	printParseTreeDebugIndentation(layer);
	string componentTagRegenerated = "";
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
	
	return result;
}

bool SANInodesClass::printComponent(SANIComponentNeuralNetwork* component, int layer)
{
	bool result = true;
	
	printParseTreeDebugIndentation(layer);
	string componentTagRegenerated = "";
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
	
	return result;
}

bool SANInodesClass::printParseTreeDebugIndentation(int layer)
{
	bool result = true;
	
	for(int i=0; i<layer; i++)
	{
		cout << "\t";
	}
	
	return result;
}

int SANInodesClass::calculateMinIndexOfMatchesFound(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset)
{	
	int minIndexOfMatchesFound = -1;
	for(int w=0; w<sentenceContentsSubset->size(); w++)
	{
		GIApreprocessorPlainTextWord* currentWord = sentenceContentsSubset->at(w);
		if(currentWord->alreadyFoundMatch)
		{	
			minIndexOfMatchesFound = w;
		}
	}
	
	return minIndexOfMatchesFound;
}


void SANInodesClass::deleteParseTree(SANIGroupParseTree* parseTreeGroupToDelete, int level)
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

SANIGroupParseTree* SANInodesClass::replicateParseTree(SANIGroupParseTree* parseTreeGroupToReplicate, int level)
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

SANIGroupParseTree* SANInodesClass::convertNeuralNetworkGroupToParseTreeGroup(SANIGroupNeuralNetwork* group)
{
	SANIGroup* groupBase = group;
	return (static_cast<SANIGroupParseTree*>(groupBase));
}
SANIComponentParseTree* SANInodesClass::convertNeuralNetworkComponentToParseTreeComponent(SANIComponentNeuralNetwork* component)
{
	SANIComponent* componentBase = component;
	return (static_cast<SANIComponentParseTree*>(componentBase));
}
SANIGroupActivationMemory* SANInodesClass::convertNeuralNetworkGroupToMemoryActivationGroup(SANIGroupNeuralNetwork* group)
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
SANIGroupActivationMemory* SANInodesClass::convertNeuralNetworkGroupToMemoryActivationGroupNew(SANIGroupNeuralNetwork* group)
{
	SANIGroupActivationMemory* groupNew = new SANIGroupActivationMemory();
	SANIGroupNeuralNetwork* groupNewBase = groupNew;
	*groupNewBase = *group;
	groupNew->components.clear();
	copyComponents(&(group->components), &(groupNew->components));
	return groupNew;
}


#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
bool SANInodesClass::resetGroupOptimumPathway(SANIGroupNeuralNetwork* group)
{	
	bool result = true;
	
	resetComponentsOptimumPathway(&(group->components));
	
	return result;
}

bool SANInodesClass::resetComponentsOptimumPathway(vector<SANIComponentNeuralNetwork*>* components)
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
int SANInodesClass::calculateCoverage(SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
{
	int activatedNeuronWordIndexCoverage = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex - activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex + 1;
	return activatedNeuronWordIndexCoverage;
}

bool SANInodesClass::traceBackpropNeuralNetwork(SANIGroupNeuralNetwork* currentNeuron, int level, int previousComponentIndex, int previousComponentType)
{
	printParseTreeDebugIndentation(level);
	if(isNeuronString(currentNeuron))
	{
		cout << "SANInodesClass::traceBackpropNeuralNetwork: prevCompIndex = " << previousComponentIndex << ", prevCompType = " << previousComponentType << ", currentNeuron->wordPOStype = " << currentNeuron->wordPOStype << endl;
	}
	else
	{
		cout << "SANInodesClass::traceBackpropNeuralNetwork: prevCompIndex = " << previousComponentIndex << ", prevCompType = " << previousComponentType << ", currentNeuron->groupIndex = " << currentNeuron->groupIndex << endl;
	}
	
	for(int i=0; i<currentNeuron->components.size(); i++)
	{
		int c = i;
		
		SANIComponentNeuralNetwork* component = (currentNeuron->components)[c];
					
		for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
		{
			SANIGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
			traceBackpropNeuralNetwork(groupSource, level+1, i, component->componentType);
		}	
	}
		
}

bool SANInodesClass::isNeuronString(SANIGroupNeuralNetwork* currentNeuron)
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

bool SANInodesClass::getWordPOStypeFromAmbiguousWord(GIApreprocessorPlainTextWord* currentWord, int* wordPOStype)
{
	bool result = false;
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
	if(GIApreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
	{	
		*wordPOStype = GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT;
		result = true;
	}
	else
	{
	#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
		*wordPOStype = GIA_PREPROCESSOR_POS_TYPE_NOUN;
		result = true;		
		#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
	}
	#endif
	
	return result;
}

#endif	


#ifdef SANI_SEQUENCE_GRAMMAR
bool SANInodesClass::printParseTreeGroupIndices(SANIGroupParseTree* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	printParseTreeDebugIndentation(layer);
	cout << currentParseTreeGroup->groupRef->groupIndex << endl;
			
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];

		if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
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

string SANInodesClass::printParseTreeGroupIndicesFlat(SANIGroupParseTree* currentParseTreeGroup)
{
	string groupIndicesString = "";
	
	groupIndicesString = groupIndicesString + SHAREDvars.convertIntToString(currentParseTreeGroup->groupRef->groupIndex) + " ";
			
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];

		if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
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

bool SANInodesClass::printParseTree(SANIGroupParseTree* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	printParseTreeDebugIndentation(layer);
	cout << "currentParseTreeGroup->groupRef->groupIndex = " << currentParseTreeGroup->groupRef->groupIndex << endl;
	
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

bool SANInodesClass::printNeuralNetwork(SANIGroupNeuralNetwork* currentNeuron, int layer)
{
	bool result = true;

	printParseTreeDebugIndentation(layer);
	cout << "currentNeuron->groupIndex = " << currentNeuron->groupIndex << endl;
	
	for(int i=0; i<currentNeuron->components.size(); i++)
	{				
		SANIComponentNeuralNetwork* component = (currentNeuron->components)[i];
		
		printComponent(component, layer);
		
		for(int j=0; j<component->ANNbackGroupConnectionList.size(); j++)
		{
			SANIGroupNeuralNetwork* componentSource = (component->ANNbackGroupConnectionList)[j];
			
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

#endif




#endif
