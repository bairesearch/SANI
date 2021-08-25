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
 * File Name: SANInodesGroupClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p12c 07-August-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Nodes Group Class
 * /
 *******************************************************************************/


#include "SANInodesGroupClass.hpp"

#ifdef SANI_NODES



SANIposRelTranslatorDebug::SANIposRelTranslatorDebug(void)
{
	#ifdef SANI_HEAVY
	activationMemoryGroupArrayIndex = INT_DEFAULT_VALUE;
	activationMemoryGroupArraySize = INT_DEFAULT_VALUE;
	#endif
	firstComponentActive = false;
	secondComponentActive = false;	
}
SANIposRelTranslatorDebug::~SANIposRelTranslatorDebug(void)
{

}

#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
GIAposRelTranslatorParserForwardPropogationSignalData::GIAposRelTranslatorParserForwardPropogationSignalData(void)
{
	semanticRelationReturnEntity = NULL;
	/*
	#ifdef SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	semanticRelationReturnConnectionDynamic = false;
	#endif
	*/
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
	semanticRelationReturnEntitySubject = NULL;
	semanticRelationReturnEntityDelimiter = NULL;
	semanticRelationReturnFunctionNameRecord = "";
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	//semanticRelationReturnFunctionEntityArray = NULL
	#endif
}
GIAposRelTranslatorParserForwardPropogationSignalData::~GIAposRelTranslatorParserForwardPropogationSignalData(void)
{

}
#endif

#ifdef SANI_FORWARD
SANIForwardPropogationSignalData::SANIForwardPropogationSignalData(void)
{
	//group specific variables:
	
	#ifdef SANI_SEQUENCE_GRAMMAR
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	firstIndexInSequence = 0;
	currentIndexInSequence = INT_DEFAULT_VALUE;
	#endif
	//createNewConnections = false;
	//maxLayerToCreateNewConnections = 0;
	activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = 0;
	activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = 0;
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	wordNounVariantType = LRP_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	wordVerbVariantType = LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	#endif
	
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY
	foundPreviousActiveWord = false;
	#endif
	
	#ifdef SANI_PARSE_SIMULTANEOUS
	//parserForwardPropogationSignalData = NULL;
	#endif
	#endif
	
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	firstPOSval = true;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE
	activationSignalStrength = 0.0;
	//wordsCaptured = NULL;
	propagatedGroupsListPerformance = NULL;
	maxNumberWordsCaptured = 0;
	firstWordPropagatedIndex = 0;
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_CONCEPTS
	detectConcepts = false;
	#endif
	#endif
}
SANIForwardPropogationSignalData::~SANIForwardPropogationSignalData(void)
{

}

SANIForwardPropogationWordData::SANIForwardPropogationWordData(void)
{
	//word specific variables:
	wordReference = NULL;
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;
	#endif
	
	w = INT_DEFAULT_VALUE;
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
	expectToSeeConnectionWithPreviousWordTrace = false;
	#endif
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSTHOC
	//previousWordConnections = NULL;
	#endif
	
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
	foundPreviousActiveWord = false;
	#endif
}
SANIForwardPropogationWordData::~SANIForwardPropogationWordData(void)
{

}

#endif


SANIGroup::SANIGroup(void)
{
	//shared variables;
	groupName = "";
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_BASE
	groupWeight = 0;
	#endif
	groupTypeName = "";
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	groupTypeReferenceSetType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN;
	semanticRelationFunctionName.resize(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP);
	for(int i=0; i<GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		semanticRelationFunctionName[i] = "";
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	semanticRelationFunctionConditionNewName = "";
	#endif

	//SANIGroup variables;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	previousWordPOStype = "";
	existsPreceedingWordPOStype = "";
	#endif
	#ifdef SANI_INVERSE
	numberOfConsecutiveTimesPreviousGroupType = 0;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	wordVerbVariantTypeDerived = LRP_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;
	wordNounVariantTypeDerived = LRP_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN;	
	#endif
	#endif
	#endif
}
SANIGroup::~SANIGroup(void)
{
}


SANIGroupNeuralNetwork::SANIGroupNeuralNetwork(void)
{
	//components = NULL;
	
	//SANIGroupNeuralNetwork variables;
	#ifdef SANI_FORWARD
	
	#ifdef SANI_SEQUENCE_GRAMMAR
	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
	networkDepth = 0;
	networkLeafSize = 0;
	#endif
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	//parseTreeGroupMemory = NULL;
	#endif
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	marked = false;
	#endif	
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	counted = false;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
	activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp = false;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
	activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	lowestLayerInArray = false;
	#endif
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
	verified = false;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
	firstHiddenLayerNeuron = false;
	#endif	
	inputLayerNeuron = false;
	groupTypeIsString = false;
	//wordDataTemp = NULL;
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS_INPUT_NEURONS_STORE_WORD_OBJECTS
	wordObject = NULL;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	#else
	wordPOStype = INT_DEFAULT_VALUE;
	#endif
	groupIndex = INT_DEFAULT_VALUE;
	#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
	groupStrength = 0;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_TOP_LEVEL_NEURONS
	topLevelSentenceNeuron = false;
	#endif
	#endif
	
	#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS
	#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_RESET
	nonResetActivationFoundDuringPreprocess = false;
	#endif
	#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
	newActivationFoundDuringPreprocess = false;
	#endif
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	//ie #ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	//SANIfrontGroupConnectionList = NULL;
	//SANIbackGroupConnectionList = NULL;
	#endif
	//SANIfrontComponentConnectionList = NULL;
	next = NULL;
	#ifdef SANI_ANN
	initiateANNneuron("");
	neuronDisplayPositionSet = false;
	neuronDisplayPositionX = INT_DEFAULT_VALUE;
	neuronDisplayPositionY = INT_DEFAULT_VALUE;
	neuronConnectivitySet = false;
	#ifdef SANI_ANN_CENTRE_NEURONS
	neuronDisplayPositionXcentred = INT_DEFAULT_VALUE;
	neuronDisplayPositionYcentred = INT_DEFAULT_VALUE;	
	#endif
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	//ie #ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	GIAtokenLayerName = "";
	GIAtokenLayerClassName = "";
	GIAtokenLayerClassTypeName = "";
	GIAtokenLayerClassTypeInstanceName = "";
	#endif
	neuronGenerated = false;
	neuronPropagated = false;
	#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
	neuronProcessed = false;
	#endif
	neuronPropagatedSave = false;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	//neuronPreviousWordPOStypeTested = false;	//NOT USED
	#endif
	#ifdef SANI_LIGHT
	//semanticRelationReturnEntityForwardPropogationSignalData = NULL;
	//semanticRelationReturnEntityForwardPropogationSignalDataProspective = NULL;	
	#ifdef SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifdef SANI_SOLIDIFY_BIO_WEAK3
	solidified = false;
	#endif
	#endif
	#endif
	#ifdef SANI_SAVE_MEMORY_GROUPS
	#ifdef SANI_HEAVY_UNOPTIMISED
	//activationMemoryGroupArray = NULL;
	#endif
	#endif
	#ifdef SANI_PARSE
	#ifdef SANI_HEAVY_UNOPTIMISED
	//currentParseTreeGroupArray = NULL;
	#endif
	#ifdef SANI_LIGHT_NEW
	currentParseTreeGroupTemp = NULL;	//used fill in currentParseTreeGroupTemp components before adding currentParseTreeGroup to parseTreeGroup network
	#endif	
	//#ifdef SANI_LIGHT
	neuronActive = false;
	//#endif	
	#endif
	#ifdef SANI_REMOVE_LAST_OPTIONAL_COMPONENTS
	optionalComponentsWrapperGroup = NULL;
	#endif
		
	#ifdef SANI_SEQUENCE_GRAMMAR
	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_NEWLY_GENERATED_NEURONS_INDIVIDUALLY
	newlyGeneratedForSentenceTemp = false;
	#ifdef SANI_ANN_COLOUR_NODES_FROM_LAST_GENERATED_SENTENCE
	generatedForLastSentence = false;
	#endif
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_CENTRAL_COMPONENTS_LINK
	//phraseCandidateSynonyms = NULL;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_LINK
	//referenceSetCandidateDuplicates = NULL;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY
	timeIndex = 0;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
	referenceSetCandidateBestMetric = 0.0;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS
	startOfSentenceWordDetected = false;
	endOfSentenceWordDetected = false;
	markToErase = false;
	#endif	
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION
	//directNeuronAssociationList = NULL;	
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE
	activationSignalStrength = 0.0;
	//wordsCaptured = NULL;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_CONCEPTS
	SANIisConceptNeuron = false;
	#endif
	#endif
	#endif
	
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
	inputLayerNeuronArtificialAmbiguousPOSpermutations = false;
	inputLayerNeuronArtificialAmbiguousPOSpermutationsPOSambiguityInfo = 0;
	#endif
}
SANIGroupNeuralNetwork::~SANIGroupNeuralNetwork(void)
{
}


#ifdef SANI_ANN
void SANIGroupNeuralNetwork::initiateANNneuron(const string groupName)	
{
	neuronReference = new ANNneuron();
	neuronReference->SANIneuronName = groupName;
	neuronReference->spatialCoordinatesSet3D = true;	//required for draw
	neuronReference->hasBackLayer = true;	//required for draw
}
#endif
			



SANIGroupActivationMemory::SANIGroupActivationMemory(void)
{
	//SANIGroupActivationMemory variables;
	#ifdef SANI_FORWARD
	#ifdef SANI_HEAVY
	#ifdef SANI_HEAVY_OPTIMISED
	numberWordsInSet = 0;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	//forwardPropogationSignalData = NULL;
	#endif
	//forwardPropogationWordData = NULL;
	layer = INT_DEFAULT_VALUE;
	inputGroupString = false;
	#ifdef SANI_SAVE_MEMORY_GROUPS
	groupOrig = NULL;	//for memoryActivationGroups only
	#endif
	#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	pointArrayGenerated = false;
	firstLevelActivationPointAdded = false;
	//activationPathWordFirstActivationMemoryGroupActivationPointArray = NULL;
	#endif
	wordGroupNeuron = false;
	#endif
	#ifdef SANI_SAVE_MEMORY_GROUPS
	#ifdef SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
	lastWordIndexActivated = INT_DEFAULT_VALUE;
	#endif
	#endif
	#endif
	#endif
}
SANIGroupActivationMemory::~SANIGroupActivationMemory(void)
{
}

SANIGroupParseTree::SANIGroupParseTree(void)
{	
	//SANIGroupParseTree variables;
	#ifdef SANI_FORWARD
	#ifdef SANI_SEQUENCE_GRAMMAR
	groupRef = NULL;
	#ifndef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFY2
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	//variableStartComponentFound = false;
	missingOrVariableStartComponentFound = false;
	#endif
	#endif
	/*
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
	missingStartComponentFound = false;
	#endif
	*/
	#endif
	#ifdef SANI_HEAVY
	#ifdef SANI_HEAVY_OPTIMISED
	#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	//activationPathWordFirstParseTreeGroupActivationPointArray = NULL;	
	#endif	
	#endif
	//semanticRelationReturnEntityForwardPropogationSignalData = NULL;
	//semanticRelationReturnEntityForwardPropogationSignalDataProspective = NULL;	
	#ifdef SANI_HEAVY_UNOPTIMISED
	parseTreeGroupRefReverse = NULL;
	parseTreeGroupRefReverseComponentIndex = INT_DEFAULT_VALUE;
	#endif
	#endif
	#ifdef SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifndef SANI_SOLIDIFY_BIO
	solidified = false;
	#endif
	#endif
	neuronTraced = false;
	#ifdef SANI_HEAVY_UNOPTIMISED
	neuronActive = false;
	#endif
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	parseTreeMinWordIndex = INT_DEFAULT_VALUE;
	parseTreeMaxWordIndex = INT_DEFAULT_VALUE;
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC
	parseTreeMaxWeight = 0.0;
	parseTreeMinWeight = 0.0;
	parseTreeTotalWeight = 0.0;
	#endif
	#endif
	#endif
}
SANIGroupParseTree::~SANIGroupParseTree(void)
{
}


SANIGroupType::SANIGroupType(void)
{
	groupTypeName = "";
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	referenceSetType = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES;	//why not GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN?
	#endif
	
	#ifdef SANI_ANN
	neuronDisplayPositionSet = false;
	neuronDisplayPositionX = INT_DEFAULT_VALUE;
	neuronDisplayPositionY = INT_DEFAULT_VALUE;
	groupTypeXindex = INT_DEFAULT_VALUE;
	groupMaxY = 0;
	#endif
}
SANIGroupType::~SANIGroupType(void)
{
	//groups = NULL;
}


#ifdef SANI_FORWARD

SANIForwardPropogationActivationPointData::SANIForwardPropogationActivationPointData(void)
{
	generateActivationPointArray = false;
	connectToPreviousActivationGroup = false;
	activationPathWordFirstActivationMemoryGroupActivationPointArray = NULL;
	activationPathWordFirstParseTreeGroupActivationPointArray = NULL;
	layer = 0;
	activationPathW = INT_DEFAULT_VALUE;
}
SANIForwardPropogationActivationPointData::~SANIForwardPropogationActivationPointData(void)
{

}


SANIForwardPropogationSentenceData::SANIForwardPropogationSentenceData(void)
{
	parseSentenceReverse = false;

	#ifdef SANI_SEQUENCE_GRAMMAR
	recordActivatedNeuronWithMaxWordIndexCoverage = false;
	activatedNeuronWithMaxWordIndexCoverage = NULL;
	activatedNeuronWithMaxWordIndexCoveragePartial = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
	recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent = false;
	activatedNeuronWithMaxWordIndexCoverageVariableEndComponent = false;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent = false;
	activatedNeuronWithMaxWordIndexCoverageVariableStartComponent = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH
	variableFirstComponentMaxDepth = 0;
	#endif
	#endif
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_HISTORY
	//listOfHighLevelNeuronsCompleteHistory = NULL;
	#endif	
	/*
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
	activatedNeuronWithMaxWordIndexCoverageMissingStartComponent = false;
	#endif
	*/
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
	findingCandidateComponent2 = false;
	candidateComponent2ToFind = NULL;
	foundCandidateComponent2 = false;
	candidateComponent2sourceParseTreeGroup = NULL;
	#endif
	firstLayer = NULL;
	#endif
	
	//sentence specific variables:
	toplevelGroupActivationFound = false;
	#ifdef SANI_PARSE_SIMULTANEOUS
	parserEnabled = false;
	#endif
	#ifdef SANI_PARSE_PERFORMANCE	
	performance = 0;
	#endif
	sentenceContents = NULL;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	isQuery = false;
	#endif
	//#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	parseIsolatedSubreferenceSets = false;
	//#endif
	#endif
	
	finishedPassingSentenceWords = false;
	
	SANIGroupTypes = NULL;
	
	//forwardPropogationWordDataArray = NULL;
	
	#ifdef SANI_HEAVY_UNOPTIMISED
	//activationPathWordFirstParseTreeGroupArray = NULL;
	#else
	//activationPathWordFirstActivationMemoryGroupArray = NULL;
	//activationPathWordFirstParseTreeGroupArray = NULL;
	#endif
	
	forwardPropogationActivationPointData = NULL;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_ALL_PROPAGATED_NEURONS
	updateNeuronRecency = false;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISALLOW_IDENTICAL_INPUTS
	highLevelNeuronPriorTemp = NULL;
	#endif
	
	//moved from SANIpropagate static variables:
	#ifdef SANI_PARSE_SAVE_PARSE_TREE
	topLevelParseTreeGroupPropagate = NULL;
	#endif
	//#ifdef SANI_HEAVY_OPTIMISED
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
	maxIterationPropagate = INT_DEFAULT_VALUE;
	#endif
	//#endif
	//#ifdef SANI_HEAVY_OPTIMISED or SANI_LIGHT_OPTIMISED or SANI_COMPACT
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT	
	parseTreeMaxWeightPropagate = INT_DEFAULT_VALUE;
	#endif
	//#endif
	#ifdef SANI_LIGHT_OPTIMISED
	#ifdef SANI_FREE_MEMORY
	//parseTreeGroupListPropagate = NULL;
	//parseTreeComponentListExtraPropagate = NULL;
	#endif
	#endif
	#ifdef SANI_LIGHT_UNOPTIMISED
	#ifdef SANI_LIGHT_BIO
	//parseTreeGroupsArrayPropagate = NULL;
	#endif
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
	artificialInputNeuronLinkPosAmbiguousPermuations = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT
	activatedNeuronWithMaxWordIndexCoverageRequirePosAmbiguousContext = false;
	#endif	
	#endif
	
	simultaneousAmbiguousPOSpropagation = false;
}
SANIForwardPropogationSentenceData::~SANIForwardPropogationSentenceData(void)
{

}

#endif



#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
#ifdef SANI_INVERSE
void SANInodesGroupClass::copyParseGroupInfo(SANIGroupParseTree* currentParseTreeGroupTemp, SANIGroupParseTree* currentParseTreeGroup)	
{
	//NOT ALLOWED as will overwrite previous tree linking to currentParseTreeGroup:  *currentParseTreeGroup = currentParseTreeGroupTemp;
	
	currentParseTreeGroup->semanticRelationFunctionName = currentParseTreeGroupTemp->semanticRelationFunctionName;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	currentParseTreeGroup->semanticRelationFunctionConditionNewName = currentParseTreeGroupTemp->semanticRelationFunctionConditionNewName;
	#endif
	currentParseTreeGroup->groupName = currentParseTreeGroupTemp->groupName;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	currentParseTreeGroup->previousWordPOStype = currentParseTreeGroupTemp->previousWordPOStype;
	currentParseTreeGroup->existsPreceedingWordPOStype = currentParseTreeGroupTemp->existsPreceedingWordPOStype;
	#endif
	currentParseTreeGroup->groupTypeName = currentParseTreeGroupTemp->groupTypeName;
	
	currentParseTreeGroup->components = currentParseTreeGroupTemp->components;	//CHECKTHIS
}
#endif

bool SANInodesGroupClass::isTopLevelGroupType(const string groupTypeName, const int groupTypeReferenceSetType, const bool isQuery, const bool parseIsolatedSubreferenceSets)	
{
	bool topLevelGroup = false;
	
	if(groupTypeReferenceSetType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE)
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
		if(isQuery)
		{
			if(groupTypeName == SANIGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS])
			{
				topLevelGroup = true;
				//cout << "isQuery && 'queries' group found" << endl;
			}
		}
		else
		{
		#endif

			#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
			if(parseIsolatedSubreferenceSets)
			{
				if(groupTypeName == SANIGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS])
				{
					topLevelGroup = true;
				}
			}
			else
			{
			#endif
			
				if(groupTypeName == SANIGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS])
				{
					topLevelGroup = true;
				}
							
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
			}
			#endif

		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
		}
		#endif
	}
	
	return topLevelGroup;
}


#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
bool SANInodesGroupClass::determineIsQuery(vector<LRPpreprocessorPlainTextWord*>* sentenceContents)
{
	bool isQuery = false;
	
	if((*sentenceContents)[sentenceContents->size()-1]->tagName == LRP_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE_QUESTIONMARK)
	{
		isQuery = true;
	}
	
	return isQuery;
}
#endif
#endif



	
#endif
