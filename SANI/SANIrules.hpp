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
 * File Name: SANIrules.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p9c 17-May-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Rules
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_RULES
#define HEADER_SANI_RULES


#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif

#ifdef USE_SANI	//GIA_POS_REL_TRANSLATOR_RULES

#include "XMLparserClass.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#ifdef SANI_SEQUENCE_GRAMMAR
//#include "SANIgenerateCompact.hpp"
#endif


#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE

#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_WITH_NO_EXTERNAL_PROCESSING_NUMBER_OF_TYPES (2)
static string GIAtxtRelWithNoExternalProcessingGroupNameArray[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_WITH_NO_EXTERNAL_PROCESSING_NUMBER_OF_TYPES] = {"definitionAlias1-withNoExternalProcessing", "definitionAlias2-withNoExternalProcessing"};
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_WITH_EXTERNAL_PROCESSING_NUMBER_OF_TYPES (2)
static string GIAtxtRelWithExternalProcessingGroupNameArray[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_WITH_EXTERNAL_PROCESSING_NUMBER_OF_TYPES] = {"definitionAlias1-withExternalProcessing", "definitionAlias2-withExternalProcessing"};

//extension of GIAsynRelTranslatorGeneric.hpp;
	//should be roughly synced also with GIAsemRelTranslatorDefs.hpp:GIA_SEM_REL_TRANSLATOR_COMMON:GIA_ENTITY_VECTOR_CONNECTION_TYPE_*
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_undefined -1

#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_createSubstanceInstance 1	//NOT CURRENTLY USED
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_createActionInstance 2	//NOT CURRENTLY USED

#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_property 4
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_action 5
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionSubject 6
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionObject 7
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_condition 8


#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definition 11
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAlias 12
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAliasWithAuxiliary 13
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeQuantity 14
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionWithAuxiliary 15
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyWithAuxiliary 16
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionSubject 17
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionObject 18
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertySubject 19
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyObject 20
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionSubject 21
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionObject 22
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopula 23
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopulaSingle 24
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordAuxiliary 25
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordPreposition 26
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordNoun 27
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordAlias 28
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordDate 29
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_numerosityPredeterminer 30
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_numerosityDeterminer 31
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyReverse 32
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionSubjectReverse 33
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionObjectReverse 34
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionSubjectReverse 35
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionObjectReverse 36
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicDefinition 37
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicAction 38
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicCondition 39
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicConclusion 40
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicConjunction 41
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionNew 42
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeMeasure 43
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeMeasurePer 44
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_thingMultiAdjConjunction 45
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_thingMultiConjunction 46
#define GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_NUMBER_OF_TYPES (47)
static string GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_NUMBER_OF_TYPES] = {"unknown", "createSubstanceInstance", "createActionInstance", "NA", "property", "action", "actionSubject", "actionObject", "condition", "NA", "NA", "definition", "definitionAlias", "definitionAliasWithAuxiliary", "attributeQuantity", "definitionWithAuxiliary", "propertyWithAuxiliary", "definitionSubject", "definitionObject", "propertySubject", "propertyObject", "conditionSubject", "conditionObject", "tenseAttributeModalAuxiliaryOrCopula", "tenseAttributeModalAuxiliaryOrCopulaSingle", "multiwordAuxiliary", "multiwordPreposition", "multiwordNoun", "multiwordAlias", "multiwordDate", "numerosityPredeterminer", "numerosityDeterminer", "propertyReverse", "actionSubjectReverse", "actionObjectReverse", "conditionSubjectReverse", "conditionObjectReverse", "logicDefinition", "logicAction", "logicCondition", "logicConclusion", "logicConjunction", "conditionNew", "attributeMeasure", "attributeMeasurePer", "thingMultiAdjConjunction", "thingMultiConjunction"};

#define GIA_POS_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX (0)	//semanticRelationReturnFunctionNameIndexType "subject"/"delimiter" refers to "subject"/"delimiter" of semantic relation function 0

/*
#define GIA_POS_REL_TRANSLATOR_RULES_TAG_txtRelTranslator ((string)"txtRelTranslator")
	#define GIA_POS_REL_TRANSLATOR_RULES_TAG_groups ((string)"groups")
		#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TAG_groupType ((string)"groupType")
			#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUPTYPE_ATTRIBUTE_groupTypeName ((string)"groupTypeName")	//allowed values: subReferenceSetsPart referenceSets subReferenceSetsAppend referenceSets logicReferenceSets
			#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUPTYPE_ATTRIBUTE_referenceSetType ((string)"referenceSetType")
			#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUPTYPE_TAG_group ((string)"group")	//NB X = groupType
				#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_groupName ((string)"groupName")	//enables referencing of specific groups by other groups (not just its group type)
				#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_semanticRelationFunctionName ((string)"semanticRelationFunctionName")	//the semantic relation to be applied to the current component (requires a subject or delimiter and an object)
				#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_semanticRelationFunctionName2 ((string)"semanticRelationFunctionName2")	//the semantic relation to be applied to the current component (requires a subject or delimiter and an object) #2
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_semanticRelationFunctionConditionNewName ((string)"semanticRelationFunctionConditionNewName")	
				#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
				#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_previousWordPOStype ((string)"previousWordPOStype")	//ensures that the previous word was of wordPOStype x (e.g. preposition/verb) before executing this group
				#endif
				#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_EXISTS_PRECEEDING_WORD_POS_TYPE
				#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_existsPreceedingWordPOStype ((string)"existsPreceedingWordPOStype")	//ensures that a previous word was of wordPOStype x (e.g. preposition/verb) before executing this group
				#endif
				#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_BASE
				#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_groupWeight ((string)"groupWeight")
				#endif
				//REDUNDANT (see special case 1 for solution): #define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_ATTRIBUTE_semanticRelationReturnEntityAndConnectToSubject ((string)"semanticRelationReturnEntityAndConnectToSubject")	//requires the following attributes to be specified for one of its components: semanticRelationReturnFunctionName="conditionSubject" semanticRelationReturnEntity="true"
				#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_component ((string)"component")
					#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_referenceSetTypeHybrid ((string)"referenceSetTypeHybrid")
					#endif
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_componentType ((string)"componentType")
					
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupTypeNameRef ((string)"groupTypeName")	//groupTypeRefName
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_groupNameRef ((string)"groupName")	//groupRefName
					
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_stringType ((string)"stringType")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_word ((string)"word")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordPOStype ((string)"wordPOStype")
					#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordVerbVariantType ((string)"wordVerbVariantType")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_wordNounVariantType ((string)"wordNounVariantType")
					#endif
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenLayer ((string)"tokenLayer")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenClass ((string)"tokenClass")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_tokenType ((string)"tokenType")
					#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_missing ((string)"missing")
					#endif
					#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS_EFFICIENT
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_addToExplicitWordTempPOS ((string)"addToExplicitWordTempPOS")	
					#endif
					
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationIndexType ((string)"semanticRelationIndexType")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationIndexType2 ((string)"semanticRelationIndexType2")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnEntity ((string)"semanticRelationReturnEntity")	//specifies that the current component returns the following entity (for subsequent higher level processing)	//used by multiple special cases?
					#ifdef SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnConnectionDynamic ((string)"semanticRelationReturnConnectionDynamic")	//specifies whether the returned semantic relation connection made (to target) will be determined dynamically based on semantic content of possible targets (action/delimiter or noun/object) 			
					#endif
					//special case 1;
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnFunctionName ((string)"semanticRelationReturnFunctionName")		//specifies that a particular semanticRelation function (semanticRelationReturnFunctionName) should be executed/applied by the higher level component on semanticRelationReturnEntity
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationReturnFunctionNameIndexType ((string)"semanticRelationReturnFunctionNameIndexType")	//specifies that the semanticRelation function must be executed on the first preceeding reference to a particular entity type (e.g. "delimiter"/"subject/thing")
					//special case 3 (for semanticRelationIndexType="list" special cases);
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationDelimiterRecord ((string)"semanticRelationDelimiterRecord")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationRecord ((string)"semanticRelationRecord")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationDelimiterConnect ((string)"semanticRelationDelimiterConnect")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_semanticRelationConnect ((string)"semanticRelationConnect")
					//other;
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_optional ((string)"optional")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_true ((string)"true")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_VALUE_false ((string)"false")	//implied by default
					
					#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_queryComparisonVariable ((string)"queryComparisonVariable")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isAuxiliaryQuery ((string)"isAuxiliaryQuery")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isWhichOrEquivalentWhatQuery ((string)"isWhichOrEquivalentWhatQuery")
					#endif
					#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_ALIASES
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isNameQuery ((string)"isNameQuery")
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isName ((string)"isName")
					#endif
					#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isNumberOf ((string)"isNumberOf")
					#endif
					#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_EXPLETIVES
					#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_ATTRIBUTE_isExpletive ((string)"isExpletive")
					#endif
				#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_or ((string)"or")
				#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_repeat ((string)"repeat")
				#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_GROUP_TAG_example ((string)"example")
	#define GIA_POS_REL_TRANSLATOR_RULES_TAG_tokens ((string)"tokens")
		#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_layer ((string)"layer")
			#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_class ((string)"class")
				#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_type ((string)"type")
					#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_TAG_instance ((string)"instance")
			#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_ATTRIBUTE_name ((string)"name")
*/


#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_LAYER_NAME "logicReference"

#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED (0)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION (1)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB (2)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION (3)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_undefined "undefined"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_conjunction "conjunction"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_verb "verb"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_preposition "preposition"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_NUM (4)
static string GIAposRelTranslatorHybridLogicReferenceClasses[GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_NUM] = {GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_undefined, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_conjunction, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_verb, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_preposition};

#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_UNDEFINED (GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED)
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_CONJUNCTION (GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION)
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_VERB (GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB)
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_PREPOSITION (GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION)
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_DEFINITION (4)
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_CONCLUSION (5)
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_undefined (GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_undefined)
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_conjunction (GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_conjunction)
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_verb (GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_verb)
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_preposition (GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_preposition)
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_definition "definition"
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_conclusion "conclusion"
#define GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_NUM (GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_NUM + 2)
static string GIAposRelTranslatorLogicReferenceClasses[GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_NUM] = {GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_undefined, GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_conjunction, GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_verb, GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_preposition, GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_definition, GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_conclusion};
#ifdef GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_DYNAMIC_RELATIONSHIP_ENTITY_TYPES
//static int GIAposRelTranslatorLogicReferenceClassesCrossReferenceEntityType[GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_NUM] = {GIA_ENTITY_TYPE_UNDEFINED, GIA_ENTITY_TYPE_UNDEFINED, GIA_ENTITY_TYPE_ACTION, GIA_ENTITY_TYPE_CONDITION, GIA_ENTITY_TYPE_DEFINITION, GIA_ENTITY_TYPE_CONDITION};
static int GIAposRelTranslatorLogicReferenceClassesCrossReferenceEntityType[GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_NUM] = {GIA_ENTITY_TYPE_UNDEFINED, GIA_ENTITY_TYPE_CONDITION, GIA_ENTITY_TYPE_ACTION, GIA_ENTITY_TYPE_CONDITION, GIA_ENTITY_TYPE_DEFINITION, GIA_ENTITY_TYPE_CONDITION};
#endif

#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED (11)	//used to demarkate components of conjunction logic references 
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_CONFIRMED (12)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT (13)
/*
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPLEMENT (11)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_COMPLEMENT (13)
*/

/*
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_UNKNOWN (0)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_IMPERATIVE (1)	//eg Ride the bike
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_STATEMENT (2)	//eg Tom rides the bike (standard subject/object)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_AND (3)		//eg and Tom rides the bike
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_OR (4)		//or Tom rides the bike
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_CONCLUSION (5)	//eg therefore Tom rides the bike
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_LOGICALCONDITION (6)	//eg therefore Tom rides the bike
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB_TYPE_PROPOSITION (7)	//eg Tom suggested that Tom rides the bike
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_REGARDING (8)	//eg Considering X, Tom rides the bike
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_STANCE (9)	//eg Unlike X, Tom rides the bike
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_QUALIFIER (10)	//eg Tom rides the bike, except on Thursday
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_TIME (11)	//eg Tom rides the bike after 12pm (need to reconcile with standard GIA conditions (semantic relation); these currently conflict)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_MEANSOFACHIEVINGACTION (12)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_PURPOSE (13)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_WITH (14)
*/
//OLD (depreciated):
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown "unknown"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_imperative "imperative"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_statement "statement"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and "and"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or "or"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_conclusion "conclusion"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_logicalCondition "logicalCondition"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB_TYPE_proposition "proposition"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_regarding "regarding"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_stance "stance"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_qualifier "qualifier"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_time "time"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_meansOfAchievingAction "means of achieving action"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_purpose "purpose"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_with "with"
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_NUM (4)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_NUM (4)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB_TYPE_NUM (1)
#define GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_NUM (7)
static string GIAposRelTranslatorHybridLogicReferenceClassUndefinedTypes[GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_NUM] = {GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_imperative, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_statement};
static string GIAposRelTranslatorHybridLogicReferenceClassConjunctionTypes[GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_NUM] = {GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_conclusion, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_logicalCondition};
static string GIAposRelTranslatorHybridLogicReferenceClassVerbTypes[GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB_TYPE_NUM] = {GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_VERB_TYPE_proposition};
static string GIAposRelTranslatorHybridLogicReferenceClassPrepositionTypes[GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_NUM] = {GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_regarding, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_stance, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_qualifier, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_time, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_meansOfAchievingAction, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_purpose, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_with};

#define BOOL_NA (false)

#endif




class SANIrulesClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	private: SANInodesClass SANInodes;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	#ifdef SANI_SEQUENCE_GRAMMAR
	//private: SANIgenerateCompactClass SANIgenerateCompact;
	#endif

	public: bool extractSANIrules(vector<SANIGroupType*>* SANIGroupTypes, vector<XMLparserTag*>* SANIrulesTokenLayers);
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
		public: bool extractSANIGroups(vector<SANIGroupType*>* SANIGroupTypes);
			private: bool parseComponents(XMLparserTag* firstTxtRelTranslatorRulesFirstComponentTag, constEffective SANIGroupNeuralNetwork* groupOwner, vector<SANIComponentNeuralNetwork*>* componentsList, const bool parseSubcomponent, constEffective SANIComponentNeuralNetwork* subComponentOwner);
			private: bool connectGroupsReferences(vector<SANIGroupType*>* SANIGroupTypes);
				private: bool connectComponentsReferences(vector<SANIGroupType*>* SANIGroupTypes, const vector<SANIComponentNeuralNetwork*>* components, const bool subcomponents);
		public: bool extractSANIrulesTokens(vector<XMLparserTag*>* SANIrulesTokenLayers);

	public: bool isClassTagWrapper(const string word, const string layerNameToFind, const string classNameToFind, const string classTypeNameToFind, vector<XMLparserTag*>* SANIrulesTokenLayers);
		public: bool isClassTag(const string word, const string layerNameToFind, const string classNameToFind, string* classTypeNameFound, vector<XMLparserTag*>* SANIrulesTokenLayers);
		public: bool isClassTag(const string word, const string layerNameToFind, const string classNameToFind, const string classTypeNameToFind, vector<XMLparserTag*>* SANIrulesTokenLayers);
	#endif	
		
};


#endif

#endif
