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
 * File Name: SANInodesGroupClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p9c 17-May-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Nodes Group Class
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_GROUP_CLASS
#define HEADER_SANI_GROUP_CLASS

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif
#include "LRPpreprocessorWordClass.hpp"
#ifdef SANI_FORWARD
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#endif


#ifdef SANI_NODES

#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_GENERATED (1)	//type 1

#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PROPAGATED (3)	//type 2
#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
//#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_FORWARDPROP_NEURON_PREVIOUS_POS_WORD_TYPE_TESTED (4)	//type 2
#endif

#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE (5)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF (6)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED (9)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE (10)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET (11)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROCESSED (12)	//type 3
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_DEMARKATEOPTIMUMPATHWAY_RESET (13)	//type 3

#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED (7)	//type 4
#define GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY (8)	//type 4



#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE

#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_UNKNOWN (0)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_LOGICREFERENCESET (1)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_REFERENCESET (2)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SUBREFERENCESET (3)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_SENTENCE (4)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES (5)
static string SANIGroupsComponentReferenceSetTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_NUMBER_OF_TYPES] = {"unknown", "logicReferenceSet", "referenceSet", "subReferenceSet", "sentence"};

#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_UNKNOWN (0)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETS (1)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_REFERENCESETS (2)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETS (3)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETPARTS (4)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETAPPENDS (5)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETSOPTIONAL (6)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_STATEMENTS (7)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_QUESTIONS (8)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBJECTS (9)
#define GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_NUMBER_OF_TYPES (10)	//this is just a selection of prominent groupTypes (it doesn't include all of them)
static string SANIGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_NUMBER_OF_TYPES] = {"unknown", "logicReferenceSets", "referenceSets", "subReferenceSets", "subReferenceSetsPart", "subReferenceSetsAppend", "logicReferenceSetsOptional", "statements", "questions", "subjects"};

#endif



class SANIposRelTranslatorDebug
{
public:
	SANIposRelTranslatorDebug(void);
	~SANIposRelTranslatorDebug(void);
	
	#ifdef SANI_HEAVY
	int activationMemoryGroupArrayIndex;
	int activationMemoryGroupArraySize;
	#endif
	bool firstComponentActive;
	bool secondComponentActive;
};



#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
class GIAposRelTranslatorParserForwardPropogationSignalData
{
public:
	GIAposRelTranslatorParserForwardPropogationSignalData(void);
	~GIAposRelTranslatorParserForwardPropogationSignalData(void);
	
	GIAentityNode* semanticRelationReturnEntity;
	/*
	#ifdef SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	bool semanticRelationReturnConnectionDynamic;
	#endif
	*/
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
	GIAentityNode* semanticRelationReturnEntitySubject;
	GIAentityNode* semanticRelationReturnEntityDelimiter;
	string semanticRelationReturnFunctionNameRecord;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	vector<GIAentityNode*> semanticRelationReturnFunctionEntityArray;
	#endif
};
#endif


class SANIGroupNeuralNetwork;
class SANIComponentNeuralNetwork;
class SANIComponentParseTree;
class SANIGroupActivationMemory;
class SANIGroupParseTree;

#ifdef SANI_FORWARD
class SANIForwardPropogationSignalData
{
public:
	SANIForwardPropogationSignalData(void);
	~SANIForwardPropogationSignalData(void);
	
	#ifdef SANI_SEQUENCE_GRAMMAR
	#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	int firstIndexInSequence;
	int currentIndexInSequence;
	#endif
	//bool createNewConnections;
	//int maxLayerToCreateNewConnections;
	int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed;
	int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed;	//OLD: +1 (e.g. sentence length); ie word indices must occur before activatedNeuronWithMaxWordIndexCoverageLastWordIndex
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	int wordNounVariantType;
	int wordVerbVariantType;
	#endif
	
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY
	bool foundPreviousActiveWord;
	#endif
	
	#ifdef SANI_PARSE_SIMULTANEOUS
	GIAposRelTranslatorParserForwardPropogationSignalData parserForwardPropogationSignalData;	//semantic relation function parameters
	#endif
	#endif
	
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	bool firstPOSval;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE
	double activationSignalStrength;
	vector<LRPpreprocessorPlainTextWord*> wordsCaptured;
	multimap<double, SANIGroupNeuralNetwork*>* propagatedGroupsListPerformance;	//multimap<int64_t, SANIGroupNeuralNetwork*>* propagatedGroupsListPerformance;
	int maxNumberWordsCaptured;
	int firstWordPropagatedIndex;
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_CONCEPTS
	bool detectConcepts;
	#endif
	#endif
};

class SANIForwardPropogationWordData
{
public:
	SANIForwardPropogationWordData(void);
	~SANIForwardPropogationWordData(void);
		
	 //word specific variables:
	LRPpreprocessorPlainTextWord* wordReference;
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	int wordPOStype;	
	#endif
	
	int w;
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
	bool expectToSeeConnectionWithPreviousWordTrace;
	#endif
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSTHOC
	vector<SANIForwardPropogationWordData*> previousWordConnections;
	#endif
		
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BASIC
	bool foundPreviousActiveWord;	//debug only
	#endif
};

#endif




class SANIGroup
{
public:
	SANIGroup(void);
	~SANIGroup(void);

	//shared variables;
	string groupName;
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_BASE
	int groupWeight;
	#endif
	string groupTypeName;
			
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	int groupTypeReferenceSetType;
	vector<string> semanticRelationFunctionName;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	string semanticRelationFunctionConditionNewName;
	#endif				
					
	//SANIGroup variables;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	string previousWordPOStype;
	string existsPreceedingWordPOStype;
	#endif
	#ifdef SANI_INVERSE
	int numberOfConsecutiveTimesPreviousGroupType;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	int wordVerbVariantTypeDerived;
	int wordNounVariantTypeDerived;	
	#endif
	#endif	
	#endif
};



class SANIGroupNeuralNetwork: public SANIGroup
{
public:
	SANIGroupNeuralNetwork(void);
	~SANIGroupNeuralNetwork(void);
	#ifdef SANI_ANN
	public: void initiateANNneuron(const string groupName);
	#endif
	
	vector<SANIComponentNeuralNetwork*> components;	

	//SANIGroupNeuralNetwork variables;
	#ifdef SANI_FORWARD
	
	#ifdef SANI_SEQUENCE_GRAMMAR
	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
	int networkDepth;
	int networkLeafSize;
	#endif	
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	vector<SANIGroupParseTree*> parseTreeGroupMemory;
	#endif
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	bool marked;
	#endif	
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	bool counted;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
	bool activatedNeuronWithMaxWordIndexCoverageVariableStartComponentTemp;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
	bool activatedNeuronWithMaxWordIndexCoverageVariableEndComponentTemp;
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_GENERATE_DETECT_COVERAGE_SUPPORT_VARIABLE_END_COMPONENT
	bool lowestLayerInArray;
	#endif
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
	bool verified;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
	bool firstHiddenLayerNeuron;
	#endif	
	bool inputLayerNeuron;	//neuron is in first layer of generated network
	bool groupTypeIsString;
	//SANIForwardPropogationWordData* wordDataTemp;
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS_INPUT_NEURONS_STORE_WORD_OBJECTS
	LRPpreprocessorPlainTextWord* wordObject;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	#else
	int wordPOStype;
	#endif
	int groupIndex;
	#ifdef SANI_SEQUENCE_GRAMMAR_WEIGHTS
	double groupStrength;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_TOP_LEVEL_NEURONS
	bool topLevelSentenceNeuron;
	#endif
	#endif
	
	#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS
	#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_RESET
	bool nonResetActivationFoundDuringPreprocess;
	#endif
	#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
	bool newActivationFoundDuringPreprocess;
	#endif
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	//ie #ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	vector<SANIGroupNeuralNetwork*> SANIfrontGroupConnectionList;	//for IO only
	vector<SANIGroupNeuralNetwork*> SANIbackGroupConnectionList;	//for IO only
	#endif
	vector<SANIComponentNeuralNetwork*> SANIfrontComponentConnectionList;
	SANIGroupNeuralNetwork* next;
	#ifdef SANI_ANN
	ANNneuron* neuronReference;
	bool neuronDisplayPositionSet;
	int neuronDisplayPositionX;
	int neuronDisplayPositionY;
	bool neuronConnectivitySet;
	#ifdef SANI_ANN_CENTRE_NEURONS
	int neuronDisplayPositionXcentred;	
	int neuronDisplayPositionYcentred;	
	#endif
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE	//ie #ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	string GIAtokenLayerName;
	string GIAtokenLayerClassName;
	string GIAtokenLayerClassTypeName;
	string GIAtokenLayerClassTypeInstanceName;
	#endif
	bool neuronGenerated;
	bool neuronPropagated;
	#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
	bool neuronProcessed;
	#endif
	bool neuronPropagatedSave;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	//bool neuronPreviousWordPOStypeTested;	//NOT USED
	#endif
	#ifdef SANI_LIGHT
	SANIForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalData;
	SANIForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//temporary	//only update semanticRelationReturnEntityForwardPropogationSignalData upon complete/group activation (prevents ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData from being invalidated by partial reactivations of a group; when accessed by an unrelated instance of SANIparserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwor) 	
	#ifdef SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifdef SANI_SOLIDIFY_BIO_WEAK3
	bool solidified;
	#endif
	#endif
	#endif
	#ifdef SANI_SAVE_MEMORY_GROUPS
	#ifdef SANI_HEAVY_UNOPTIMISED
	vector<SANIGroupActivationMemory*> activationMemoryGroupArray;
	#endif
	#endif
	#ifdef SANI_PARSE
	#ifdef SANI_HEAVY_UNOPTIMISED
	vector<SANIGroupParseTree*> currentParseTreeGroupArray;
	#endif
	#ifdef SANI_LIGHT_NEW
	SANIGroupParseTree* currentParseTreeGroupTemp;	//used fill in currentParseTreeGroupTemp components before adding currentParseTreeGroup to parseTreeGroup network
	#endif
	//#ifdef SANI_LIGHT
	bool neuronActive;	//interpretation: all components are active (unless they are optional / unused or cases)
	//#endif	
	#endif
	#ifdef SANI_REMOVE_LAST_OPTIONAL_COMPONENTS
	SANIGroupNeuralNetwork* optionalComponentsWrapperGroup;
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR
	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_NEWLY_GENERATED_NEURONS_INDIVIDUALLY
	bool newlyGeneratedForSentenceTemp;
	#ifdef SANI_ANN_COLOUR_NODES_FROM_LAST_GENERATED_SENTENCE
	bool generatedForLastSentence;
	#endif
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_CENTRAL_COMPONENTS_LINK
	vector<SANIGroupNeuralNetwork*> phraseCandidateSynonyms;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_LINK
	vector<SANIGroupNeuralNetwork*> referenceSetCandidateDuplicates;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY
	long timeIndex;	//may correspond to sentenceIndex
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
	double referenceSetCandidateBestMetric;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS
	bool startOfSentenceWordDetected;
	bool endOfSentenceWordDetected;
	bool markToErase;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION
	multimap<double, SANIGroupNeuralNetwork*> directNeuronAssociationList;	
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE
	double activationSignalStrength;
	vector<LRPpreprocessorPlainTextWord*> wordsCaptured;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_CONCEPTS
	bool SANIisConceptNeuron;
	#endif
	#endif
	#endif
	
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
	bool inputLayerNeuronArtificialAmbiguousPOSpermutations;
	uint64_t inputLayerNeuronArtificialAmbiguousPOSpermutationsPOSambiguityInfo;	//temporarily store word objects containing POS ambiguity info of artificial POS ambiguous input neuron
	#endif
};


class SANIGroupActivationMemory: public SANIGroupNeuralNetwork
{
public:
	SANIGroupActivationMemory(void);
	~SANIGroupActivationMemory(void);

	
	//vector<SANIComponentNeuralNetwork*> components;	

	//SANIGroupActivationMemory variables;
	#ifdef SANI_FORWARD
	#ifdef SANI_HEAVY
	#ifdef SANI_HEAVY_OPTIMISED
	int numberWordsInSet;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	SANIForwardPropogationSignalData forwardPropogationSignalData;
	#endif
	SANIForwardPropogationWordData forwardPropogationWordData;
	int layer;
	bool inputGroupString;
	#ifdef SANI_SAVE_MEMORY_GROUPS
	SANIGroupNeuralNetwork* groupOrig;	//for memoryActivationGroups only
	#endif
	#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	bool pointArrayGenerated;
	bool firstLevelActivationPointAdded;
	vector<SANIGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupActivationPointArray;
	#endif
	bool wordGroupNeuron;
	#endif
	#ifdef SANI_SAVE_MEMORY_GROUPS
	#ifdef SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
	int lastWordIndexActivated;
	#endif
	#endif
	#endif
	#endif

};


class SANIGroupParseTree: public SANIGroup
{
public:
	SANIGroupParseTree(void);
	~SANIGroupParseTree(void);
		
	vector<SANIComponentParseTree*> components;	

	//SANIGroupParseTree variables;
	#ifdef SANI_FORWARD
	#ifdef SANI_SEQUENCE_GRAMMAR
	SANIGroupNeuralNetwork* groupRef;	//backup of original group (non-parse-tree)
	#ifndef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFY2
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	//bool variableStartComponentFound;
	bool missingOrVariableStartComponentFound;
	#endif
	#endif
	/*
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
	bool missingStartComponentFound;
	#endif
	*/
	#endif
	#ifdef SANI_HEAVY
	#ifdef SANI_HEAVY_OPTIMISED
	#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
	vector<SANIGroupParseTree*> activationPathWordFirstParseTreeGroupActivationPointArray;	
	#endif
	#endif	
	SANIForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalData;
	SANIForwardPropogationSignalData semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//temporary	//only update semanticRelationReturnEntityForwardPropogationSignalData upon complete/group activation (prevents ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData from being invalidated by partial reactivations of a group; when accessed by an unrelated instance of SANIparserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwor) 	
	#ifdef SANI_HEAVY_UNOPTIMISED
	SANIGroupNeuralNetwork* parseTreeGroupRefReverse;
	int parseTreeGroupRefReverseComponentIndex;
	#endif
	#endif
	#ifdef SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	#ifndef SANI_SOLIDIFY_BIO
	bool solidified;
	#endif
	#endif
	bool neuronTraced;
	#ifdef SANI_HEAVY_UNOPTIMISED
	bool neuronActive;	//interpretation: all components are active (unless they are optional / unused or cases)
	#endif
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	int parseTreeMinWordIndex;
	int parseTreeMaxWordIndex;
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC
	double parseTreeMaxWeight;	//NOT USED
	double parseTreeMinWeight;
	double parseTreeTotalWeight;	//NOT USED
	#endif
	#endif
	#endif

};







class SANIGroupType
{
public:
	SANIGroupType(void);
	~SANIGroupType(void);

	string groupTypeName;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	int referenceSetType;
	#endif
	
	#ifdef SANI_ANN
	bool neuronDisplayPositionSet;
	int neuronDisplayPositionX;
	int neuronDisplayPositionY;
	int groupTypeXindex;
	int groupMaxY;
	#endif
		
	vector<SANIGroupNeuralNetwork*> groups;
};



#ifdef SANI_FORWARD
	
class SANIForwardPropogationActivationPointData
{
public:
	SANIForwardPropogationActivationPointData(void);
	~SANIForwardPropogationActivationPointData(void);
	
	bool generateActivationPointArray;
	bool connectToPreviousActivationGroup;
	vector<SANIGroupActivationMemory*>* activationPathWordFirstActivationMemoryGroupActivationPointArray;
	vector<SANIGroupParseTree*>* activationPathWordFirstParseTreeGroupActivationPointArray;
	int layer;	
	int activationPathW;
};

class SANIForwardPropogationSentenceData
{
public:
	SANIForwardPropogationSentenceData(void);
	~SANIForwardPropogationSentenceData(void);
	
	bool parseSentenceReverse;
	
	#ifdef SANI_SEQUENCE_GRAMMAR
	bool recordActivatedNeuronWithMaxWordIndexCoverage;
	SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage;
	bool activatedNeuronWithMaxWordIndexCoveragePartial;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
	bool recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent;
	bool activatedNeuronWithMaxWordIndexCoverageVariableEndComponent;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	bool recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent;
	bool activatedNeuronWithMaxWordIndexCoverageVariableStartComponent;	//ie candidateCoverageMissingOrVariableStartComponent
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS_X_COMP_REQUIRE_MATCHING_DEPTH
	int variableFirstComponentMaxDepth;
	#endif
	#endif
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_HISTORY
	vector<SANIGroupNeuralNetwork*> listOfHighLevelNeuronsCompleteHistory;
	#endif
	/*
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
	bool activatedNeuronWithMaxWordIndexCoverageMissingStartComponent;
	#endif
	*/
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
	bool findingCandidateComponent2;
	SANIComponentNeuralNetwork* candidateComponent2ToFind;
	bool foundCandidateComponent2;
	SANIGroupParseTree* candidateComponent2sourceParseTreeGroup;
	#endif
	vector<SANIGroupNeuralNetwork*>* firstLayer;
	#endif
	
	//sentence specific variables:
	bool toplevelGroupActivationFound;
	#ifdef SANI_PARSE_SIMULTANEOUS
	bool parserEnabled;
	#endif
	#ifdef SANI_PARSE_PERFORMANCE
	int performance;
	#endif
	vector<LRPpreprocessorPlainTextWord*>* sentenceContents;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	bool isQuery;
	#endif
	//#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	bool parseIsolatedSubreferenceSets;
	//#endif
	#endif
	
	bool finishedPassingSentenceWords;
	
	vector<SANIGroupType*>* SANIGroupTypes;

	vector<SANIForwardPropogationWordData*> forwardPropogationWordDataArray;
	
	#ifdef SANI_HEAVY
	#ifdef SANI_HEAVY_UNOPTIMISED
	vector<SANIGroupParseTree*> activationPathWordFirstParseTreeGroupArray;
	#else
	vector<vector<SANIGroupActivationMemory*>> activationPathWordFirstActivationMemoryGroupArray;
	vector<vector<SANIGroupParseTree*>> activationPathWordFirstParseTreeGroupArray;
	#endif
	#endif
	
	SANIForwardPropogationActivationPointData* forwardPropogationActivationPointData;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_ALL_PROPAGATED_NEURONS
	bool updateNeuronRecency;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISALLOW_IDENTICAL_INPUTS
	SANIGroupNeuralNetwork* highLevelNeuronPriorTemp;
	#endif
	
	//moved from SANIpropagate static variables:
	#ifdef SANI_PARSE_SAVE_PARSE_TREE
	SANIGroupParseTree* topLevelParseTreeGroupPropagate;
	#endif
	//#ifdef SANI_HEAVY_OPTIMISED
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
	bool maxIterationPropagate;
	#endif
	//#endif
	//#ifdef SANI_HEAVY_OPTIMISED or SANI_LIGHT_OPTIMISED or SANI_COMPACT
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT	
	int parseTreeMaxWeightPropagate;
	#endif
	//#endif
	//#ifdef SANI_LIGHT_OPTIMISED or SANI_COMPACT
	#ifdef SANI_FREE_MEMORY
	vector<SANIGroupParseTree*> parseTreeGroupListPropagate;
	vector<SANIComponentParseTree*> parseTreeComponentListExtraPropagate;
	#endif
	//#endif
	#ifdef SANI_LIGHT_UNOPTIMISED
	#ifdef SANI_LIGHT_BIO
	vector<SANIGroupParseTree*> parseTreeGroupsArrayPropagate;
	#endif
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS
	bool artificialInputNeuronLinkPosAmbiguousPermuations;
	#endif
	
};

#endif


class SANInodesGroupClass
{
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	#ifdef SANI_INVERSE
	public: void copyParseGroupInfo(SANIGroupParseTree* currentParseTreeGroupTemp, SANIGroupParseTree* currentParseTreeGroup);
	#endif
	public: bool isTopLevelGroupType(const string groupTypeName, const int groupTypeReferenceSetType, const bool isQuery, const bool parseIsolatedSubreferenceSets);	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	public: bool determineIsQuery(vector<LRPpreprocessorPlainTextWord*>* sentenceContents);
	#endif
	#endif
};



#endif

#endif
