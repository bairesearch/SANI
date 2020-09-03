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
 * Project: General Intelligence Algorithm
 * Project Version: 1m4b 01-September-2020
 * Requirements: requires plain text file
 * Description: SANI (Sequentially Activated Neuronal Input neural network) Nodes
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_NODES
#define HEADER_SANI_NODES

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"

#ifdef SANI_NODES

//extraction of xml tag/attribute names from SANIrules required for print/debug;
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
			
			
					

class SANInodesClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	
	
	//code from SANIrules;
	public: void setSANIGroupTypesGlobal(vector<SANIGroupType*>* SANIGroupTypes);
	public: vector<SANIGroupType*>* getSANIGroupTypesGlobal();
	public: void setSANIrulesTokenLayersGlobal(vector<XMLparserTag*>* SANIrulesTokenLayers);
	public: vector<XMLparserTag*>* getSANIrulesTokenLayersGlobal();
	
	#ifdef SANI_SEQUENCE_GRAMMAR
	public: void initialiseNewGroupIndex(vector<SANIGroupType*>* SANIGroupTypes);
	public: int* getNewGroupIndex();
	public: SANIGroupType* getSequenceGrammarGroupTypeDefault(vector<SANIGroupType*>* SANIGroupTypes);
	public: int assignGroupIndex(SANIGroupNeuralNetwork* group);
	#endif
	
	public: bool findGroupType(vector<SANIGroupType*>* SANIGroupTypes, const string groupTypeName, SANIGroupType** groupTypeFound);
	public: bool findGroup(vector<SANIGroupType*>* SANIGroupTypes, const string groupTypeName, const string groupName, SANIGroupType** groupTypeFound, SANIGroupNeuralNetwork** groupFound);

	
	#ifdef SANI_REMOVE_LAST_OPTIONAL_COMPONENTS
	public: bool removeLastOptionalComponents(vector<SANIGroupType*>* SANIGroupTypes);
		private: bool removeOptionalComponent(SANIGroupType* groupType, int* groupIndex, const int groupTypeGroupsSizeOrig, SANIGroupNeuralNetwork* group, const int optionalComponentIndex);	
	#endif
	public: SANIGroupNeuralNetwork* copyGroup(SANIGroupNeuralNetwork* group);
		public: bool copyComponents(vector<SANIComponentNeuralNetwork*>* components, vector<SANIComponentNeuralNetwork*>* componentsNew);
			public: bool copyComponent(SANIComponentNeuralNetwork* currentComponent, vector<SANIComponentNeuralNetwork*>* componentsNew);
	public: SANIGroupParseTree* copyGroup(SANIGroupParseTree* group);
		public: bool copyComponents(vector<SANIComponentParseTree*>* components, vector<SANIComponentParseTree*>* componentsNew);
	public: SANIGroupActivationMemory* copyGroup(SANIGroupActivationMemory* group);
	#ifdef SANI
	public: bool updateComponentsOwnerGroupAndIndexes(SANIGroupNeuralNetwork* group, vector<SANIComponentNeuralNetwork*>* components, const bool isSubcomponent, SANIComponentNeuralNetwork* ownerComponent);
	#endif



	//code from SANIpropagateOperations;
	#ifdef SANI
	
	public: bool currentWordAmbiguous(GIApreprocessorPlainTextWord* currentWord);

	public: int convertWordIndexToSentenceEntityIndex(int wordIndex);
	public: int convertSentenceEntityIndexToWordIndex(int sentenceEntityIndex);

	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_WORD_NOUNS_ONLY
	public: bool isWordPOStypeNoun(int wordPOStype);
	#endif
	
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	public: int countParseTreeLeafSize(SANIGroupParseTree* currentParseTreeGroup);
	#endif
	public: int countParseTreeLeafSizeUnoptimised(SANIGroupParseTree* currentParseTreeGroup);
	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
	public: bool adjustNetworkDepth(SANIGroupNeuralNetwork* group);
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	public: bool adjustNetworkDepthReset(SANIGroupNeuralNetwork* group);
	#endif
	public: bool getNeuralNetworkDepth(SANIGroupNeuralNetwork* currentNeuron, int* maxDepth);
		public: bool getNeuralNetworkDepth(SANIComponentNeuralNetwork* component, int* maxDepth);
	public: bool countNeuralNetworkMaxLeafSizeAndDepth(SANIGroupNeuralNetwork* currentGroup, int* maxLeafSize, int* maxDepth);
		public: bool countNeuralNetworkMaxLeafSizeAndDepth(SANIComponentNeuralNetwork* component, int* maxLeafSize, int* maxDepth);
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	public: bool countNeuralNetworkMaxLeafSizeAndDepthReset(SANIGroupNeuralNetwork* currentNeuron);
		public: bool countNeuralNetworkMaxLeafSizeAndDepthReset(SANIComponentNeuralNetwork* component);
	#endif
	#endif

	public: SANIComponentNeuralNetwork* getFirstComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentNeuron, bool fromStart);
	public: SANIComponentParseTree* getFirstComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentNeuron, bool fromStart);

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH	
	public: int calculateDepthFromBinaryTreeLeafSize(int numberOfLeafNodesInBinaryTree);
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_RANDOMISE
	public: double generateRandomNumber();
	#endif

	#endif
	

	public: bool deleteGroup(SANIGroupNeuralNetwork* group);
		public: bool deleteComponents(vector<SANIComponentNeuralNetwork*>* components);
	public: bool deleteGroup(SANIGroupParseTree* group);
		public: bool deleteComponents(vector<SANIComponentParseTree*>* components);
	public: bool deleteGroup(SANIGroupActivationMemory* group);
		
	public: bool printGroup(SANIGroupNeuralNetwork* group, int layer);
		public: bool printComponent(SANIComponentParseTree* component, int layer);
		public: bool printComponent(SANIComponentNeuralNetwork* component, int layer);
	public: bool printParseTreeDebugIndentation(int layer);
	
	public: int calculateMinIndexOfMatchesFound(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset);

	public: void deleteParseTree(SANIGroupParseTree* parseTreeGroupToDelete, int level);
	public: SANIGroupParseTree* replicateParseTree(SANIGroupParseTree* parseTreeGroupToReplicate, int level);

	public: SANIGroupParseTree* convertNeuralNetworkGroupToParseTreeGroup(SANIGroupNeuralNetwork* group);
	public: SANIComponentParseTree* convertNeuralNetworkComponentToParseTreeComponent(SANIComponentNeuralNetwork* component);
	public: SANIGroupActivationMemory* convertNeuralNetworkGroupToMemoryActivationGroup(SANIGroupNeuralNetwork* group);
	public: SANIGroupParseTree* convertNeuralNetworkGroupToParseTreeGroupNew(SANIGroupNeuralNetwork* group);
	public: SANIComponentParseTree* convertNeuralNetworkComponentToParseTreeComponentNew(SANIComponentNeuralNetwork* component);
	public: SANIGroupActivationMemory* convertNeuralNetworkGroupToMemoryActivationGroupNew(SANIGroupNeuralNetwork* group);

	#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	public: bool resetGroupOptimumPathway(SANIGroupNeuralNetwork* group);
	private: bool resetComponentsOptimumPathway(vector<SANIComponentNeuralNetwork*>* components);
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR
	public: int calculateCoverage(SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage);
	public: bool traceBackpropNeuralNetwork(SANIGroupNeuralNetwork* currentNeuron, int level, int previousComponentIndex, int previousComponentType);
	public: bool isNeuronString(SANIGroupNeuralNetwork* currentNeuron);
	public: bool getWordPOStypeFromAmbiguousWord(GIApreprocessorPlainTextWord* currentWord, int* wordPOStype);
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR
	public: bool printParseTreeGroupIndices(SANIGroupParseTree* currentParseTreeGroup, int layer);
	public: string printParseTreeGroupIndicesFlat(SANIGroupParseTree* currentParseTreeGroup);

	public: bool printParseTree(SANIGroupParseTree* currentParseTreeGroup, int layer);
	#endif

		
};

#endif


#endif
