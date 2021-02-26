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
 * File Name: SANInodesComponentClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1o5c 25-February-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Nodes Component Class
 * /
 *******************************************************************************/


#include "SANInodesComponentClass.hpp"

#ifdef SANI_NODES


SANIComponent::SANIComponent(void)
{
	#ifdef SANI_SEQUENCE_GRAMMAR
	//SANIbackGroupConnectionList = NULL;
	#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
	componentStrength = 0;
	#endif
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	referenceSetTypeHybrid = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_REFERENCE_SET_TYPE_HYBRID_UNKNOWN;
	#endif
	groupTypeRefName = "";
	groupRefName = "";
	#endif
	
	componentType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_UNKNOWN;
	componentIndex = INT_DEFAULT_VALUE;
	
	stringType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_UNKNOWN;
	word = "";
	wordPOStype = "";
	
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	tokenLayer = "";
	tokenClass = "";
	tokenType = "";
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
	missing = false;
	#endif
	#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
	stringTypeExplicitAddToExplicitWordTempPOS = false;
	#endif
		
	semanticRelationIndexType.resize(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP);
	for(int i=0; i<GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		semanticRelationIndexType[i] = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN;
	}
	semanticRelationReturnEntity = false;
	#ifdef SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	semanticRelationReturnConnectionDynamic = false;
	#endif
	
	semanticRelationReturnFunctionName = "";
	semanticRelationReturnFunctionNameIndexType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN;
	
	semanticRelationRecord = false;
	semanticRelationConnect = false;
	
	optional = false;
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
	queryComparisonVariable = false;
	isAuxiliaryQuery = false;
	isWhichOrEquivalentWhatQuery = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_ALIASES
	isNameQuery = false;
	isName = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
	isNumberOf = false;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	wordVerbVariantType = LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	wordNounVariantType = LRP_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	#endif		
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_EXPLETIVES
	isExpletive = false;
	#endif	

	#ifdef SANI_PARSE_SIMULTANEOUS_BIO
	parserForwardPropogationSignalData = NULL;
	#endif
	#endif
	

	#ifdef SANI_FORWARD
	neuronComponentConnectionActive = false;
	neuronComponentConnectionActiveWordRecord = NULL;
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	//neuronComponentConnectionActiveWordRecordMemory = NULL;
	#endif
	#endif
	candidateStringMatch = NULL;
	groupTypeRef = NULL;
	groupRef = NULL;
	
}
SANIComponent::~SANIComponent(void)
{
}


SANIComponentNeuralNetwork::SANIComponentNeuralNetwork(void)
{
	#ifdef SANI_SUPPORT_COMPONENTS_SUB
	//subComponents = NULL;
	#endif
	
	#ifdef SANI_FORWARD
	ownerGroup = NULL;
	#ifdef SANI_SUPPORT_COMPONENTS_SUB
	isSubcomponent = false;
	#endif
	ownerComponent = NULL;
	#ifdef SANI_ANN
	//ANNbackNeuronConnectionList = NULL;
	#endif
	#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	//optimumPathwayWordList = NULL;
	#endif
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP	
	neuralNetworkComponentHasTypeString = false;
	#endif
}
SANIComponentNeuralNetwork::~SANIComponentNeuralNetwork(void)
{
}


SANIComponentParseTree::SANIComponentParseTree(void)
{
	//parse tree variables:
	#ifdef SANI_PARSE_GENERATE_PARSE_TREE
	parseTreeGroupRef = NULL;
	#endif
	componentRef = NULL;
	wordPOStypeInferred = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;
	#ifdef SANI_LIGHT_BIO2
	groupFrontComponentConnectionListIndex = INT_DEFAULT_VALUE;
	numberOfWordsCurrentlyParsed = INT_DEFAULT_VALUE;
	#endif
	#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	wordIndex = INT_DEFAULT_VALUE;	//CHECKTHIS
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_VARIABLE_COMPONENTS_STRING_OR_GROUP	
	parseTreeComponentTypeString = false;
	#endif
}
SANIComponentParseTree::~SANIComponentParseTree(void)
{
}


#ifdef SANI_SUPPORT_COMPONENTS_SUB
bool SANInodesComponentClass::componentHasSubcomponents(const SANIComponent* component)
{
	bool hasSubcomponents = false;
	if((component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR) || (component->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
	{
		hasSubcomponents = true;
	}
	return hasSubcomponents;
}
#endif


#endif



