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
 * File Name: SANIformation.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1m3c 01-August-2020
 * Requirements: 
 * Description: SANI (Sequentially Activated Neuronal Input neural network) Formation
 * /
 *******************************************************************************/


#include "SANIformation.hpp"

#ifdef SANI


SANIGroupNeuralNetwork* firstInputGroupInNetwork;
#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
SANIGroupNeuralNetwork* firstGroupInInputLayerSectionWordOrig;
unordered_map<string, SANIGroupNeuralNetwork*> inputLayerSectionWordOrigMap;	//for efficient lookup
#else
SANIGroupNeuralNetwork* firstGroupInInputLayerSectionWordPOStype;
#ifdef SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
SANIGroupNeuralNetwork* firstGroupInInputLayerSectionWordNounVariantType;
SANIGroupNeuralNetwork* firstGroupInInputLayerSectionWordVerbVariantType;
#endif
SANIGroupNeuralNetwork* firstGroupInInputLayerSectionExplicitWord;
SANIGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayer;
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
int numberOfInputGroupsInSectionWordOrig;
#else
int numberOfInputGroupsInSectionWordPOStype;
#ifdef SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
int numberOfInputGroupsInSectionNounVariantType;
int numberOfInputGroupsInSectionVerbVariantType;
#endif
int numberOfInputGroupsInSectionExplicitWord;
int numberOfInputGroupsInSectionTokensLayer;

unordered_map<string, SANIGroupNeuralNetwork*> inputLayerSectionExplicitWordMap;
unordered_map<string, SANIGroupNeuralNetwork*> inputLayerSectionTokensLayerMap;
#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
vector<string> explicitWordListLocal;
#endif
#endif

//high level groupType names should be synced with GIAposRelTranslatorClass::generateParseTreeIntro
SANIGroupNeuralNetwork* topLevelGroupInOuputLayerSectionStatements;
SANIGroupNeuralNetwork* topLevelGroupInOuputLayerSectionQuestions;
SANIGroupNeuralNetwork* topLevelGroupInOuputLayerSectionSubjects;


#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
SANIGroupNeuralNetwork* SANIformationClass::getFirstGroupInInputLayerSectionWordOrig()
{
	return firstGroupInInputLayerSectionWordOrig;
}
#else
#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
vector<string>* SANIformationClass::getExplicitWordList()
{
	return &explicitWordListLocal;
}
#endif
unordered_map<string, SANIGroupNeuralNetwork*>* SANIformationClass::getInputLayerSectionExplicitWordMap()
{
	return &inputLayerSectionExplicitWordMap;
}
unordered_map<string, SANIGroupNeuralNetwork*>* SANIformationClass::getInputLayerSectionTokensLayerMap()
{
	return &inputLayerSectionTokensLayerMap;
}
SANIGroupNeuralNetwork* SANIformationClass::getFirstGroupInInputLayerSectionWordPOStype()
{
	return firstGroupInInputLayerSectionWordPOStype;
}
#endif

#ifdef GIA_NEURAL_NETWORK
SANIGroupNeuralNetwork* SANIformationClass::getFirstInputGroupInNetwork()
{
	return firstInputGroupInNetwork;
}
#endif



/*
[front of network]

input layer:                                        				                                                                                 			                                                              InputLayerSectionTokensLayer
                                                                                                                                                                                                                               /                          |                                      \
pre-input layer:        InputLayerSectionWordNounVariantType    InputLayerSectionWordVerbVariantType                                                                                                   InputLayerSectionTokensLayerClass                   InputLayerSectionTokensLayerClass               InputLayerSectionTokensLayerClass        
                                            \                                     /                                                                                                  			             /                        |                      \                                                             /               |              \
pre-input layer:                                     InputLayerSectionWordPOStype                                             InputLayerSectionExplicitWord                        InputLayerSectionTokensLayerClassType    InputLayerSectionTokensLayerClassType       InputLayerSectionTokensLayerClassType        			       

[back of network]
*/



bool SANIformationClass::createSANI(vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	createInputNeuronLayer(SANIrulesTokenLayers, SANIGroupTypes);
	
	#ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY		
	createNeuronLayerIntro(SANIrulesTokenLayers, SANIGroupTypes);
	#endif
	
	#ifdef SANI_DEBUG_CREATE
	cout << "exiting normally" << endl;
	exit(EXIT_ERROR);
	#endif
	
	return result;
}

bool SANIformationClass::createInputNeuronLayer(vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	/*
	create inputLayer - define all neurons for all;
		wordPOStype
			wordNounVariantType
			wordVerbVariantType
		explicit words in SANIrules.xml
		tokens-layer in SANIrules.xml
			classes
				types
	*/


	SANIGroupNeuralNetwork* currentGroupInInputLayerSection = NULL;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	numberOfInputGroupsInSectionWordOrig = 0;
	
	firstGroupInInputLayerSectionWordOrig = new SANIGroupNeuralNetwork();
	firstInputGroupInNetwork = firstGroupInInputLayerSectionWordOrig;
	currentGroupInInputLayerSection = firstGroupInInputLayerSectionWordOrig;
	if(!createInputNeuronLayerSectionWordOrig(&currentGroupInInputLayerSection, &numberOfInputGroupsInSectionWordOrig))
	{
		result = false;
	}	
	#else
	numberOfInputGroupsInSectionWordPOStype = 0;
	numberOfInputGroupsInSectionExplicitWord = 0;
	numberOfInputGroupsInSectionTokensLayer = 0;
	
	firstGroupInInputLayerSectionWordPOStype = new SANIGroupNeuralNetwork();
	firstInputGroupInNetwork = firstGroupInInputLayerSectionWordPOStype;
	currentGroupInInputLayerSection = firstGroupInInputLayerSectionWordPOStype;
	if(!createInputNeuronLayerSectionWordPOStype(&currentGroupInInputLayerSection, &numberOfInputGroupsInSectionWordPOStype))
	{
		result = false;
	}
	
	#ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	firstGroupInInputLayerSectionExplicitWord = new SANIGroupNeuralNetwork();
	currentGroupInInputLayerSection = firstGroupInInputLayerSectionExplicitWord;
	if(!createInputNeuronLayerSectionExplicitWord(&currentGroupInInputLayerSection, &numberOfInputGroupsInSectionExplicitWord, SANIGroupTypes))
	{
		result = false;
	}
	
	firstGroupInInputLayerSectionTokensLayer = new SANIGroupNeuralNetwork();
	currentGroupInInputLayerSection = firstGroupInInputLayerSectionTokensLayer;
	if(!createInputNeuronLayerSectionTokensLayer(SANIrulesTokenLayers, &currentGroupInInputLayerSection, &numberOfInputGroupsInSectionTokensLayer))
	{
		result = false;
	}
	#endif
	#endif


	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
bool SANIformationClass::createInputNeuronLayerSectionWordOrig(SANIGroupNeuralNetwork** currentGroupInInputLayerSectionWordOrig, int* numberOfGroupsInSection)
{
	bool result = true;
	//initialise section as null
	return result;
}
bool SANIformationClass::addInputNeuronLayerSectionWordOrig(GIApreprocessorPlainTextWord* currentWord, SANIGroupNeuralNetwork** currentGroupInInputLayerSection)
{
	bool result = true;
	
	int* numberOfGroupsInSection = &numberOfInputGroupsInSectionWordOrig;

	SANIGroupNeuralNetwork* currentGroupInInputLayerSectionWordOrig = getFirstGroupInInputLayerSectionWordOrig();
	while(currentGroupInInputLayerSectionWordOrig->next != NULL)
	{
		currentGroupInInputLayerSectionWordOrig = currentGroupInInputLayerSectionWordOrig->next;	//point to last group in section
	}
	*currentGroupInInputLayerSection = currentGroupInInputLayerSectionWordOrig;
	
	string wordOrig = currentWord->tagName;
	#ifdef SANI_ANN
	currentGroupInInputLayerSectionWordOrig->neuronReference->GIAentityName = wordOrig;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR
	currentGroupInInputLayerSectionWordOrig->groupTypeIsString = true;
	#endif
	#ifdef SANI_DEBUG_PROPAGATE_ASSIGN_GROUP_INDICES_TO_INPUT_NEURONS
	int newNeuronIndex = SANInodes.assignGroupIndex(currentGroupInInputLayerSectionWordOrig);	
	#ifdef SANI_ANN
	currentGroupInInputLayerSectionWordOrig->initiateANNneuron("groupIndex:" + SHAREDvars.convertIntToString(newNeuronIndex));
	#endif
	#endif
	addGroupToLayer(&currentGroupInInputLayerSectionWordOrig, numberOfGroupsInSection);
	inputLayerSectionWordOrigMap.insert(pair<string, SANIGroupNeuralNetwork*>(wordOrig, currentGroupInInputLayerSectionWordOrig));
	#ifdef SANI_DEBUG_CREATE
	cout << "SANIformationClass::createInputNeuronLayerSectionWordOrig{} addGroupToLayer(currentGroupInInputLayerSectionWordOrig, numberOfGroupsInSection);  i = " << i << endl;
	#endif
	
	
	return result;
}
bool SANIformationClass::findInputNeuronLayerSectionWordOrig(GIApreprocessorPlainTextWord* currentWord, SANIGroupNeuralNetwork** currentGroupInInputLayerSection)
{
	bool result = false;
	
	string wordOrig = currentWord->tagName;
	unordered_map<string, SANIGroupNeuralNetwork*>::iterator iter = inputLayerSectionWordOrigMap.find(wordOrig);
	if(iter != inputLayerSectionWordOrigMap.end())
	{
		result = true;
		*currentGroupInInputLayerSection = iter->second;
	}
	else
	{
		//cerr << "SANIformationClass::findInputNeuronLayerSectionWordOrig{} error: cannot find word in inputLayerSectionWordOrigMap: wordOrig = " << wordOrig << endl;
	}
	
	return result;
}
#else
bool SANIformationClass::createInputNeuronLayerSectionWordPOStype(SANIGroupNeuralNetwork** currentGroupInInputLayerSectionWordPOStype, int* numberOfGroupsInSection)
{
	bool result = true;
	
	#ifdef SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
	numberOfInputGroupsInSectionNounVariantType = 0;
	numberOfInputGroupsInSectionVerbVariantType = 0;
	#endif
	
	for(int i=0; i<GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		#ifdef SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
		if(i == GIA_PREPROCESSOR_POS_TYPE_NOUN)
		{
			firstGroupInInputLayerSectionWordNounVariantType = new SANIGroupNeuralNetwork();
			if(!createInputNeuronLayerSectionNounVariantType(*currentGroupInInputLayerSectionWordPOStype, &firstGroupInInputLayerSectionWordNounVariantType, &numberOfInputGroupsInSectionNounVariantType))
			{
				result = false;
			}
		}
		if(i == GIA_PREPROCESSOR_POS_TYPE_VERB)
		{
			firstGroupInInputLayerSectionWordVerbVariantType = new SANIGroupNeuralNetwork();
			if(!createInputNeuronLayerSectionVerbVariantType(*currentGroupInInputLayerSectionWordPOStype, &firstGroupInInputLayerSectionWordVerbVariantType, &numberOfInputGroupsInSectionVerbVariantType))
			{
				result = false;
			}
		}
		#endif
		#ifdef SANI_ANN
		(*currentGroupInInputLayerSectionWordPOStype)->neuronReference->GIAentityName = GIApreprocessorPOStypeNameArray[i];
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR
		(*currentGroupInInputLayerSectionWordPOStype)->groupTypeIsString = true;
		#endif
		#ifdef SANI_DEBUG_PROPAGATE_ASSIGN_GROUP_INDICES_TO_INPUT_NEURONS
		int newNeuronIndex = SANInodes.assignGroupIndex(*currentGroupInInputLayerSectionWordPOStype);	
		#ifdef SANI_ANN
		(*currentGroupInInputLayerSectionWordPOStype)->initiateANNneuron("groupIndex:" + SHAREDvars.convertIntToString(newNeuronIndex));
		#endif
		#endif
		addGroupToLayer(currentGroupInInputLayerSectionWordPOStype, numberOfGroupsInSection);
		#ifdef SANI_DEBUG_CREATE
		cout << "SANIformationClass::createInputNeuronLayerSectionWordPOStype{} addGroupToLayer(currentGroupInInputLayerSectionWordPOStype, numberOfGroupsInSection);  i = " << i << endl;
		#endif
	}
}

#ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY

#ifdef SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
bool SANIformationClass::createInputNeuronLayerSectionNounVariantType(SANIGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, SANIGroupNeuralNetwork** currentGroupInInputLayerSectionWordNounVariantType, int* numberOfGroupsInSection)
{	
	bool result = true;
	
	for(int i=0; i<GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_NUMBER_OF_TYPES; i++)
	{
		createGroupANNconnectionIO(currentGroupInInputLayerSectionWordPOStype, *currentGroupInInputLayerSectionWordNounVariantType);	//switched GIA3h3a
		#ifdef SANI_ANN
		(*currentGroupInInputLayerSectionWordNounVariantType)->neuronReference->GIAentityName = GIApreprocessorMultiwordReductionNounDatabaseTagBaseTenseFormArray[i];
		#endif
		addGroupToLayer(currentGroupInInputLayerSectionWordNounVariantType, numberOfGroupsInSection);		
	}
	
	return result;
}

bool SANIformationClass::createInputNeuronLayerSectionVerbVariantType(SANIGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, SANIGroupNeuralNetwork** currentGroupInInputLayerSectionWordVerbVariantType, int* numberOfGroupsInSection)
{
	bool result = true;
	
	for(int i=0; i<GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_NUMBER_OF_TYPES; i++)
	{
		createGroupANNconnectionIO(currentGroupInInputLayerSectionWordPOStype, *currentGroupInInputLayerSectionWordVerbVariantType);	//switched GIA3h3a
		#ifdef SANI_ANN
		(*currentGroupInInputLayerSectionWordVerbVariantType)->neuronReference->GIAentityName = GIApreprocessorMultiwordReductionVerbDatabaseTagBaseTenseFormArray[i];
		#endif
		addGroupToLayer(currentGroupInInputLayerSectionWordVerbVariantType, numberOfGroupsInSection);		
	}
	
	return result;
}
#endif

bool SANIformationClass::createInputNeuronLayerSectionExplicitWord(SANIGroupNeuralNetwork** currentGroupInInputLayerSection, int* numberOfGroupsInSection, vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	inputLayerSectionExplicitWordMap.clear();	//added GIA3g11aTEMP62
	
	#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	explicitWordListLocal.clear();
	#endif
	vector<string> explicitWordList;
	
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		for(int j=0; j<(groupType->groups).size(); j++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[j];
			if(!createInputNeuronLayerSectionExplicitWord(&(group->components), false, &explicitWordList))
			{
				result = false;
			}
		}
	}
	for(int i=0; i<explicitWordList.size(); i++)
	{
		#ifdef SANI_ANN
		(*currentGroupInInputLayerSection)->neuronReference->GIAentityName = explicitWordList[i];
		#endif
		inputLayerSectionExplicitWordMap.insert(pair<string, SANIGroupNeuralNetwork*>(explicitWordList[i], *currentGroupInInputLayerSection));
		addGroupToLayer(currentGroupInInputLayerSection, numberOfGroupsInSection);
	}
	
	#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	#ifndef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
	explicitWordListLocal = explicitWordList;
	#endif
	#endif
	
	
	return result;
}
bool SANIformationClass::createInputNeuronLayerSectionExplicitWord(vector<SANIComponentNeuralNetwork*>* components, bool subcomponents, vector<string>* explicitWordList)
{
	bool result = true;
	
	for(int j=0; j<components->size(); j++)
	{
		SANIComponentNeuralNetwork* component = (*components)[j];
		
		if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		{
			if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
			{
				string word = component->word;
				if(!findWordInList(word, explicitWordList))
				{
					explicitWordList->push_back(word);
					#ifdef SANI_DEBUG_CREATE
					cout << "SANIrulesClass::connectComponentsReferences{} explicitWordList->push_back(word) = " << word << endl;
					#endif
					
					#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
					if(component->stringTypeExplicitAddToExplicitWordTempPOS)
					{
						explicitWordListLocal.push_back(word);
						#ifdef SANI_DEBUG_CREATE
						cout << "SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT: explicitWordListLocal.push_back(word) = " << word << endl;
						#endif 
					}
					#endif
				}
			}
		}
		else if((component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR) || (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
		{
			if(subcomponents)
			{
				result = false;
				cerr << "SANIrulesClass::connectComponentsReferences{} error: (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR/REPEAT) && subcomponents" << endl;
				exit(EXIT_ERROR);	
			}
			if(!createInputNeuronLayerSectionExplicitWord(&(component->subComponents), true, explicitWordList))
			{
				result = false;
				cerr << "SANIrulesClass::connectComponentsReferences{} error: !connectComponentsReferences()" << endl;
				exit(EXIT_ERROR);			
			}
		}
	}
	
	return result;
}


bool SANIformationClass::createInputNeuronLayerSectionTokensLayer(vector<XMLparserTag*>* SANIrulesTokenLayers, SANIGroupNeuralNetwork** currentGroupInInputLayerSectionTokensLayer, int* numberOfGroupsInSectionTokensLayer)
{
	bool classTagFound = false;
	bool result = false;
	
	inputLayerSectionTokensLayerMap.clear();	//added GIA3g11aTEMP62
	
	for(int i=0; i<SANIrulesTokenLayers->size(); i++)
	{	
		XMLparserTag* currentLayerTag = SANIrulesTokenLayers->at(i);

		//cout << "i = " << i << endl;
		//cout << "currentLayerTag->name = " << currentLayerTag->name << endl;	//only the first 3 xml rules <tokens> <layer> layers detected

		string layerName = "";
		if(!XMLparserClass.getAttribute(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name, &layerName))
		{
			cerr << "GIAposRelTranslatorHybridClass::executeTxtRelTranslatorDepreciated{} error: (!XMLparserClass.getAttribute(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name, &layerName)" << endl;
			exit(EXIT_ERROR);
		}
		
		XMLparserTag* firstXMLtagInLayer = XMLparserClass.parseTagDownALevel(currentLayerTag, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_layer, &result);
		XMLparserTag* currentXMLtagInLayer = firstXMLtagInLayer;

		SANIGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayerClass = new SANIGroupNeuralNetwork();
		SANIGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClass = firstGroupInInputLayerSectionTokensLayerClass;
		int numberOfGroupsInSectionTokensLayerClass = 0; 
		
		while(currentXMLtagInLayer->nextTag != NULL)
		{
			if(currentXMLtagInLayer->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class)
			{	
				bool sameClass = false;
				string className = "";
				//vector<string>* logicReferenceClassVector = NULL;
				if(currentXMLtagInLayer->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
				{
					className = currentXMLtagInLayer->firstAttribute->value;
				}
				
				XMLparserTag* firstTagInClass = XMLparserClass.parseTagDownALevel(currentXMLtagInLayer, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class, &result);
				XMLparserTag* currentTagInClass = firstTagInClass;
				#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
				//cout << "classNameToFind = " << classNameToFind << endl;
				#endif
				
				SANIGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayerClassType = new SANIGroupNeuralNetwork();
				SANIGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassType = firstGroupInInputLayerSectionTokensLayerClassType;
				int numberOfGroupsInSectionTokensLayerClassType = 0; 
		
				while(currentTagInClass->nextTag != NULL)
				{
					if(currentTagInClass->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type)
					{
						#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
						//cout << "\tSANIrules::isClassTag{} debug: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type" << endl;
						#endif
						string classTypeName = "";
						bool sameType = false;
						if(currentTagInClass->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
						{
							classTypeName = currentTagInClass->firstAttribute->value;
						}

						XMLparserTag* firstTagInTypeTag = XMLparserClass.parseTagDownALevel(currentTagInClass, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type, &result);
						XMLparserTag* currentTagInType = firstTagInTypeTag;
						
						SANIGroupNeuralNetwork* firstGroupInInputLayerSectionTokensLayerClassTypeInstance = new SANIGroupNeuralNetwork();
						SANIGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassTypeInstance = firstGroupInInputLayerSectionTokensLayerClassTypeInstance;
						int numberOfGroupsInSectionTokensLayerClassTypeInstance = 0; 
				
						while(currentTagInType->nextTag != NULL)
						{
							if(currentTagInType->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_instance)
							{
								#ifdef GIA_DEBUG_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
								//cout << "\t\tSANIrules::isClassTag{} debug: GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_instance" << endl;
								#endif
								string instanceName = "";
								if(currentTagInType->firstAttribute->name == GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name)
								{
									instanceName = currentTagInType->firstAttribute->value;
									#ifdef SANI_DEBUG_CREATE
									cout << "SANIrulesClass::createInputNeuronLayerSectionTokensLayer{} instanceName = " << instanceName << endl;
									#endif
								}
								
								currentGroupInInputLayerSectionTokensLayerClassTypeInstance->GIAtokenLayerClassTypeInstanceName = instanceName;
								#ifdef SANI_ANN
								currentGroupInInputLayerSectionTokensLayerClassTypeInstance->neuronReference->GIAentityName = instanceName;
								#endif
								createGroupANNconnectionIO(currentGroupInInputLayerSectionTokensLayerClassTypeInstance, currentGroupInInputLayerSectionTokensLayerClassType);
								addGroupToLayer(&currentGroupInInputLayerSectionTokensLayerClassTypeInstance, &numberOfGroupsInSectionTokensLayerClassTypeInstance);
							}
							
							currentTagInType = currentTagInType->nextTag;
						}
							
						currentGroupInInputLayerSectionTokensLayerClassType->GIAtokenLayerClassTypeName = classTypeName;
						#ifdef SANI_ANN
						currentGroupInInputLayerSectionTokensLayerClassType->neuronReference->GIAentityName = classTypeName;
						#endif
						//cout << "classTypeName = " << classTypeName << endl;
						createGroupANNconnectionIO(currentGroupInInputLayerSectionTokensLayerClassType, currentGroupInInputLayerSectionTokensLayerClass);
						addGroupToLayer(&currentGroupInInputLayerSectionTokensLayerClassType, &numberOfGroupsInSectionTokensLayerClassType);
					}
					
					currentTagInClass = currentTagInClass->nextTag;
				}
				
				currentGroupInInputLayerSectionTokensLayerClass->GIAtokenLayerClassName = className;
				#ifdef SANI_ANN
				currentGroupInInputLayerSectionTokensLayerClass->neuronReference->GIAentityName = className;
				#endif
				//cout << "className = " << className << endl;
				createGroupANNconnectionIO(currentGroupInInputLayerSectionTokensLayerClass, *currentGroupInInputLayerSectionTokensLayer);
				addGroupToLayer(&currentGroupInInputLayerSectionTokensLayerClass, &numberOfGroupsInSectionTokensLayerClass);
			}

			currentXMLtagInLayer = currentXMLtagInLayer->nextTag;
		}
		
		(*currentGroupInInputLayerSectionTokensLayer)->GIAtokenLayerName = layerName;
		#ifdef SANI_ANN
		(*currentGroupInInputLayerSectionTokensLayer)->neuronReference->GIAentityName = layerName;
		#endif
		//cout << "layerName = " << layerName << endl;
		inputLayerSectionTokensLayerMap.insert(pair<string, SANIGroupNeuralNetwork*>(layerName, *currentGroupInInputLayerSectionTokensLayer));
		addGroupToLayer(currentGroupInInputLayerSectionTokensLayer, numberOfGroupsInSectionTokensLayer);
	}
	
	return classTagFound;
}
#endif

#endif


#ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
bool SANIformationClass::createNeuronLayerIntro(vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);

		//parse from highest level groups first 
		//high level groupType names should be synced with GIAposRelTranslatorClass::generateParseTreeIntro
		bool passGroupTests = false;
		SANIGroupNeuralNetwork* topLevelGroup = NULL;
		
		//bool passGroupTests = SANInodesGroupClassObject.isTopLevelGroupType(groupType->groupTypeName, groupType->referenceSetType, isQuery, parseIsolatedSubreferenceSets);	
		if(groupType->referenceSetType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE)
		{
			if(groupType->groupTypeName == SANIGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS])
			{
				/*
				if(topLevelGroupInOuputLayerSectionStatements != NULL)
				{
					cerr << "SANIformationClass::createNeuronLayers{} error: (topLevelGroupInOuputLayerSectionStatements != NULL): more than one (groupType->referenceSetType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE) && (groupType->groupTypeName == SANIGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS]) defined" << endl;
					exit(EXIT_ERROR);
				}
				*/
				passGroupTests = true;
				topLevelGroupInOuputLayerSectionStatements = new SANIGroupNeuralNetwork();
				topLevelGroup = topLevelGroupInOuputLayerSectionStatements;
			}
			else if(groupType->groupTypeName == SANIGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS])
			{
				/*
				if(topLevelGroupInOuputLayerSectionQuestions != NULL)
				{
					cerr << "SANIformationClass::createNeuronLayers{} error: (topLevelGroupInOuputLayerSectionQuestions != NULL): more than one (groupType->referenceSetType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE) && (groupType->groupTypeName == SANIGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS]) defined" << endl;
					exit(EXIT_ERROR);
				}
				*/
				passGroupTests = true;
				topLevelGroupInOuputLayerSectionQuestions = new SANIGroupNeuralNetwork();
				topLevelGroup = topLevelGroupInOuputLayerSectionQuestions;
			}
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
			else if(groupType->groupTypeName == SANIGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS])
			{
				/*
				if(topLevelGroupInOuputLayerSectionSubjects != NULL)
				{
					cerr << "SANIformationClass::createNeuronLayers{} error: (topLevelGroupInOuputLayerSectionSubjects != NULL): more than one (groupType->referenceSetType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE) && (groupType->groupTypeName == SANIGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS]) defined" << endl;
					exit(EXIT_ERROR);
				}
				*/
				passGroupTests = true;
				topLevelGroupInOuputLayerSectionSubjects = new SANIGroupNeuralNetwork();
				topLevelGroup = topLevelGroupInOuputLayerSectionSubjects;
			}
			#endif
		}
		
		if(passGroupTests)
		{
			//top level group found
			#ifdef SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
			ANNneuron* currentGroupNeuronInLayer = NULL;
			ANNneuron* previousGroupNeuronInLayer = NULL;
			#endif	
			for(int j=0; j<(groupType->groups).size(); j++)
			{
				SANIGroupNeuralNetwork* group = (groupType->groups)[j];
				
				//createGroupANNconnectionIO(group, topLevelGroup);
				createGroupANNconnectionIObasic(group, topLevelGroup);
				#ifdef SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
				currentGroupNeuronInLayer = group->neuronReference;
				if(j != 0)
				{
					previousGroupNeuronInLayer->nextNeuron = currentGroupNeuronInLayer;
				}
				#endif
				
				if(!createNeuronLayerGroup(SANIrulesTokenLayers, group))
				{
					result = false;
				}
				
				#ifdef SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
				previousGroupNeuronInLayer = currentGroupNeuronInLayer;
				#endif
			}
		}
	}
	
	return result;
}

bool SANIformationClass::createNeuronLayerGroupType(vector<XMLparserTag*>* SANIrulesTokenLayers, SANIComponentNeuralNetwork* higherLevelComponent, SANIGroupType* groupType)
{
	bool result = true;
	
	/*
	#ifdef SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
	ANNneuron* currentGroupNeuronInLayer = NULL;
	ANNneuron* previousGroupNeuronInLayer = NULL;
	#endif	
	*/
	for(int j=0; j<(groupType->groups).size(); j++)
	{
		SANIGroupNeuralNetwork* group = (groupType->groups)[j];
		
		createGroupANNconnection(group, higherLevelComponent);
		/*
		#ifdef SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
		currentGroupNeuronInLayer = group->neuronReference;
		if(j != 0)
		{
			previousGroupNeuronInLayer->nextNeuron = currentGroupNeuronInLayer;
		}
		#endif
		*/
		
		if(!createNeuronLayerGroup(SANIrulesTokenLayers, group))
		{
			result = false;
		}
		
		/*
		#ifdef SANI_ANN_ADD_LAYER_FOR_EVERY_GROUP_TYPE
		previousGroupNeuronInLayer = currentGroupNeuronInLayer;
		#endif
		*/
	}
	return result;
}


bool SANIformationClass::createNeuronLayerGroup(vector<XMLparserTag*>* SANIrulesTokenLayers, SANIGroupNeuralNetwork* group)
{
	bool result = true;
	
	if(!group->neuronGenerated)	//prevent circular loops
	{
		group->neuronGenerated = true;
		if(!createNeuronLayerComponents(SANIrulesTokenLayers, group, &(group->components), false, NULL))
		{
			result = false;
		}
	}
	
	return result;
}

bool SANIformationClass::createNeuronLayerComponents(vector<XMLparserTag*>* SANIrulesTokenLayers, SANIGroupNeuralNetwork* group, vector<SANIComponentNeuralNetwork*>* components, bool subcomponents, SANIComponentNeuralNetwork* higherLevelComponent)
{
	bool result = true;
	
	for(int j=0; j<components->size(); j++)
	{
		SANIComponentNeuralNetwork* component = (*components)[j];
		
		//#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		//forwardNounVerbVariantRequirementsGroupToComponent(currentParseTreeGroup, newParseComponent);
		//#endif
		
		if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		{
			SANIGroupNeuralNetwork* stringGroup = NULL;
			if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
			{
				string wordPOStypeName = component->wordPOStype;
				int wordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(wordPOStypeName);
				
				stringGroup = getInputGroupLayerSection(firstGroupInInputLayerSectionWordPOStype, wordPOStype);
				#ifdef SANI_DEBUG_CREATE
				cout << "SANIformationClass::createNeuronLayerGroupType{}: getInputGroupLayerSection): wordPOStypeName = " << wordPOStypeName << endl;
				#endif
			}
			else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
			{
				string word = component->word;
				unordered_map<string, SANIGroupNeuralNetwork*>::iterator iter = inputLayerSectionExplicitWordMap.find(word);
				if(iter != inputLayerSectionExplicitWordMap.end())
				{
					stringGroup = iter->second;
					#ifdef SANI_DEBUG_CREATE
					cout << "SANIformationClass::createNeuronLayerGroupType{}: (iter != inputLayerSectionExplicitWordMap.end()): word = " << word << endl;
					#endif	
				}
				else
				{
					cerr << "SANIformationClass::createNeuronLayerComponents{} error: cannot find word in inputLayerSectionExplicitWordMap: word = " << word << endl;
					exit(EXIT_ERROR);
				}
			}
			else if(component->stringType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
			{
				if(findTokensLayerClassType(component->tokenLayer, component->tokenClass, component->tokenType, &stringGroup))
				{
					#ifdef SANI_DEBUG_CREATE
					cout << "SANIformationClass::createNeuronLayerGroupType{}: findTokensLayerClassType: component->tokenClass = " << component->tokenClass << ", component->tokenType = " << component->tokenType << endl;
					#endif				
				}
				else
				{
					cerr << "SANIformationClass::createNeuronLayerComponents{} error: !findTokensLayerClassType(): component->tokenClass = " << component->tokenClass << ", component->tokenType = " << component->tokenType << endl;
					exit(EXIT_ERROR);
				}
			}
			
			createGroupANNconnection(stringGroup, component);
		}
		else if(component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
		{
			if(component->groupRef != NULL)
			{
				createGroupANNconnection(component->groupRef, component);
				
				#ifdef SANI_DEBUG_PROPAGATE_EXTRA8
				cout << "createGroupANNconnection: component->groupRef->groupName = " << component->groupRef->groupName << ", higher level component groupOwner group->groupName = " << group->groupName << endl;
				#endif

				//#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				//forwardNounVerbVariantRequirementsComponentToGroup(currentParseTreeComponent, newParseGroup);
				//#endif
				
				if(createNeuronLayerGroup(SANIrulesTokenLayers, component->groupRef))
				{
					#ifdef SANI_DEBUG_CREATE
					cout << "SANIformationClass::createNeuronLayerComponents{}: createNeuronLayerGroup" << endl;
					#endif
					//foundWordMatch = true;
				}
			}
			else if(component->groupTypeRef != NULL)
			{
				//#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				//forwardNounVerbVariantRequirementsComponentToGroup(currentParseTreeComponent, newParseGroup);
				//#endif
				
				//cout << "group->groupName = " << group->groupName << endl;
				//cout << "\tcomponent->groupTypeRef = " << component->groupTypeRef->groupTypeName << endl;
				
				if(createNeuronLayerGroupType(SANIrulesTokenLayers, component, component->groupTypeRef))
				{
					#ifdef SANI_DEBUG_CREATE
					cout << "SANIformationClass::createNeuronLayerComponents{}: createNeuronLayerGroupType" << endl;
					#endif
					//foundWordMatch = true;
				}
			}
			else
			{
				cerr << "SANIformationClass::createNeuronLayerComponents{} error: (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP) && (component->groupTypeRef/groupRef == NULL)" << endl;
				exit(EXIT_ERROR);
			}
			
		}
		else if((component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR) || (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
		{
			if(subcomponents)
			{
				result = false;
				cerr << "SANIformationClass::createNeuronLayerComponents{} error: (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR/REPEAT) && subcomponents" << endl;
				exit(EXIT_ERROR);
			}
			if(!createNeuronLayerComponents(SANIrulesTokenLayers, group, &(component->subComponents), true, component))	//CHECKTHIS (NB repeat/optional subcomponents are currently added directly to the groupNeuron like the other neurons)
			{
				result = false;
				cerr << "SANIformationClass::createNeuronLayerComponents{} error: !connectComponentsReferences()" << endl;
				exit(EXIT_ERROR);		
			}
		}
	}
	
	return result;
}





bool SANIformationClass::findTokensLayerClassType(string layerName, string layerClassName, string layerClassTypeName, SANIGroupNeuralNetwork** groupFound)
{	
	bool result = false;
	SANIGroupNeuralNetwork* layerClassGroupFound = NULL;
	if(findTokensLayerClass(layerName, layerClassName, &layerClassGroupFound))
	{
		if(layerClassTypeName != "")
		{
			for(int i=0; i<layerClassGroupFound->ANNbackGroupConnectionList.size(); i++)
			{
				SANIGroupNeuralNetwork* layerClassTypeGroup = ((layerClassGroupFound->ANNbackGroupConnectionList)[i]);
				if(layerClassTypeGroup->GIAtokenLayerClassTypeName == layerClassTypeName)
				{
					result = true;
					*groupFound = layerClassTypeGroup;
				}
			}
		}
		else
		{
			*groupFound = layerClassGroupFound;
		}		
	}
	return result;
}

bool SANIformationClass::findTokensLayerClass(string layerName, string layerClassName, SANIGroupNeuralNetwork** layerClassGroupFound)
{
	bool result = false;	
	for(unordered_map<string, SANIGroupNeuralNetwork*>::iterator iter = inputLayerSectionTokensLayerMap.begin(); iter != inputLayerSectionTokensLayerMap.end(); iter++)
	{
		SANIGroupNeuralNetwork* layerGroup = iter->second;
		//cout << "layerGroup->GIAtokenLayerName = " << layerGroup->GIAtokenLayerName << endl;
		//cout << "layerName = " << layerName << endl;
		if(layerGroup->GIAtokenLayerName == layerName)
		{
			for(int i=0; i<layerGroup->ANNbackGroupConnectionList.size(); i++)
			{
				SANIGroupNeuralNetwork* layerClassGroup = ((layerGroup->ANNbackGroupConnectionList)[i]);
				//cout << "layerClassGroup->GIAtokenLayerClassName = " << layerClassGroup->GIAtokenLayerClassName << endl;
				//cout << "layerClassName = " << layerClassName << endl;
				if(layerClassGroup->GIAtokenLayerClassName == layerClassName)
				{
					result = true;
					*layerClassGroupFound = layerClassGroup;
				}
			}
		}
	}
	return result;
}
#endif

			
bool SANIformationClass::findWordInList(const string word, vector<string>* explicitWordList)
{
	bool result = false;
	vector<string>::iterator it = find(explicitWordList->begin(), explicitWordList->end(), word);
	if(it != explicitWordList->end())
	{
		result = true;
	}
	return result;
}

bool SANIformationClass::findWordInGroupMap(const string word, unordered_map<string, SANIGroupNeuralNetwork*>* wordMap, SANIGroupNeuralNetwork** groupFound)
{
	bool result = false;
	unordered_map<string, SANIGroupNeuralNetwork*>::iterator it = wordMap->find(word);
	if(it != wordMap->end())
	{
		result = true;
		*groupFound = it->second;
	}
	return result;
}


				
SANIGroupNeuralNetwork* SANIformationClass::getInputGroupLayerSection(SANIGroupNeuralNetwork* firstGroupInInputLayerSection, int groupIndexInSection)
{
	SANIGroupNeuralNetwork* currentGroupInInputLayerSection = firstGroupInInputLayerSection;
	for(int i=0; i<groupIndexInSection; i++)
	{
		currentGroupInInputLayerSection = currentGroupInInputLayerSection->next;
	}
	return currentGroupInInputLayerSection;
}



void SANIformationClass::addGroupToLayer(SANIGroupNeuralNetwork** currentGroupInLayer, int* numberOfGroupsInSection)
{
	(*currentGroupInLayer)->next = new SANIGroupNeuralNetwork();
	*currentGroupInLayer = (*currentGroupInLayer)->next;
	*numberOfGroupsInSection = *numberOfGroupsInSection + 1;
}

#ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
bool SANIformationClass::createGroupANNconnectionIO(SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelGroup)
{
	//this creates an artificial group connection for SANI only (not used by GIAposRelTranslatorParser)
	group->ANNfrontGroupConnectionList.push_back(higherLevelGroup);
	higherLevelGroup->ANNbackGroupConnectionList.push_back(group);
	
	#ifdef SANI_ANN
	createANNconnection(group, higherLevelGroup);
	#endif
}

bool SANIformationClass::createGroupANNconnectionIObasic(SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelGroup)
{
	//this creates an artificial group connection for SANI only (not used by GIAposRelTranslatorParser)
	group->ANNfrontGroupConnectionList.push_back(higherLevelGroup);
	higherLevelGroup->ANNbackGroupConnectionList.push_back(group);
}
#endif



bool SANIformationClass::createGroupANNconnection(SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* higherLevelComponent)
{
	#ifdef SANI_DEBUG_FORMATION
	cout << "SANIformationClass::createGroupANNconnection, group->groupIndex = " << group->groupIndex << ", higherLevelComponent->ownerGroup = " << higherLevelComponent->ownerGroup->groupName << endl;
	#endif
	
	group->ANNfrontComponentConnectionList.push_back(higherLevelComponent);
	#ifdef SANI_SEQUENCE_GRAMMAR
	higherLevelComponent->ANNbackGroupConnectionList.push_back(group);
	#endif
	
	#ifdef SANI_ANN
	#ifndef SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	createANNconnection(group, higherLevelComponent);
	#endif
	#endif
}


#ifdef SANI_ANN
ANNneuronConnection* SANIformationClass::createANNconnection(SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* higherLevelComponent)
{
	#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
	return createANNconnection(group, higherLevelComponent, higherLevelComponent->componentStrength);
	#else
	return createANNconnection(group, higherLevelComponent, 0);
	#endif
}
ANNneuronConnection* SANIformationClass::createANNconnection(SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* higherLevelComponent, const double connectionStrength)
{
	vector<ANNneuronConnection*>* ANNbackNeuronConnectionList = &(higherLevelComponent->ANNbackNeuronConnectionList);
	SANIGroupNeuralNetwork* higherLevelGroup = higherLevelComponent->ownerGroup;

	ANNneuronConnection* newANNneuronConnection = createANNconnection(group, higherLevelGroup);
	#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
	newANNneuronConnection->GIAconnectionStrength = connectionStrength;
	#endif
				
	ANNbackNeuronConnectionList->push_back(newANNneuronConnection);
	
	return newANNneuronConnection;
}
ANNneuronConnection* SANIformationClass::createANNconnection(SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelGroup)
{
	ANNneuron* neuron1 = group->neuronReference;
	ANNneuron* neuron2 = higherLevelGroup->neuronReference;
	
	ANNneuronConnection* newANNneuronConnection = new ANNneuronConnection();
	newANNneuronConnection->backNeuron = neuron1;
	newANNneuronConnection->frontNeuron = neuron2;
	(neuron1->frontANNneuronConnectionList).push_back(newANNneuronConnection);
	(neuron2->backANNneuronConnectionList).push_back(newANNneuronConnection);
	
	return newANNneuronConnection;
}

#ifdef SANI_SEQUENCE_GRAMMAR	//requires component->ANNbackGroupConnectionList
void SANIformationClass::deleteANNconnections(SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* component)
{				
	SANIGroupNeuralNetwork* groupTarget = component->ownerGroup;

	//code derived from SANIformationClass::createANNconnection;
	for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
	{
		SANIGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
		(groupSource->neuronReference->frontANNneuronConnectionList).clear();
	}
	(groupTarget->neuronReference->backANNneuronConnectionList).clear();

	//code derived from SANIformationClass::createANNconnection;
	vector<ANNneuronConnection*>* ANNbackNeuronConnectionList = &(component->ANNbackNeuronConnectionList);
	for(int l=0; l<ANNbackNeuronConnectionList->size(); l++)
	{
		ANNneuronConnection* ANNbackNeuronConnection = (*ANNbackNeuronConnectionList)[l];
		ANNbackNeuronConnection->frontNeuron = NULL;	//prevent frontNeuron deletion during deconstruction
		delete ANNbackNeuronConnection;
	}
	ANNbackNeuronConnectionList->clear();
}
#endif


#ifdef SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
bool SANIformationClass::createANNconnectivity(vector<SANIGroupType*>* SANIGroupTypes)
{	
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		for(int j=0; j<(groupType->groups).size(); j++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[j];
			#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
			group->neuronReference->GIAneuronStrength = group->groupStrength;
			#endif
			#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION
			bool foundActiveComponent = false;
			#endif
			for(int k=0; k<group->components.size(); k++)
			{
				SANIComponentNeuralNetwork* component = (group->components).at(k);
				for(int l=0; l<component->ANNbackGroupConnectionList.size(); l++)
				{
					SANIGroupNeuralNetwork* groupSource = component->ANNbackGroupConnectionList[l];
					//cout << "createANNconnection" << endl;
					#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_COMPONENT_INDEX
					#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_COMPONENT_INDEX_EXACT
					ANNneuronConnection* connection = createANNconnection(groupSource, component);
					connection->GIAcomponentIndexFirst = !((bool)k);
					#else
					ANNneuronConnection* connection = createANNconnection(groupSource, component);
					connection->GIAcomponentIndex = k;
					#endif
					#else
					createANNconnection(groupSource, component);
					#endif
				}
				#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION
				if(component->neuronComponentConnectionActive)
				{
					foundActiveComponent = true;
				}
				#endif
			}
			
			#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION
			if(group->neuronActive)
			{
				group->neuronReference->activationLevel = ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_FULL;
			}
			else if(foundActiveComponent)
			{
				group->neuronReference->activationLevel = ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_PARTIAL;
			}
			else
			{
				group->neuronReference->activationLevel = ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_INACTIVE;
			}
			#endif
		}
	}

}
#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
bool SANIformationClass::createANNconnectivityReset(vector<SANIGroupType*>* SANIGroupTypes)
{	
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		for(int j=0; j<(groupType->groups).size(); j++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[j];
			for(int k=0; k<group->components.size(); k++)
			{
				SANIComponentNeuralNetwork* component = (group->components).at(k);
				deleteANNconnections(group, component);
			}
		}
	}
}				
#endif
#endif

#endif


#endif