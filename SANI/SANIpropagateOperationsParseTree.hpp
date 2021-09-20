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
 * File Name: SANIpropagateOperationsParseTree.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1q2b 19-September-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Operations Parse Tree - generic functions
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_OPERATIONS_PARSE_TREE
#define HEADER_SANI_PROPAGATE_OPERATIONS_PARSE_TREE


#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif


#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "LRPpreprocessorSentenceClass.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"



class SANIpropagateOperationsParseTreeClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;

	#ifdef SANI_FORWARD
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
	public: bool getMinMaxSequenceIndexInParseTree(const SANIGroupParseTree* currentParseTreeGroup, const bool findMaxOrMinSequenceIndex, int* sequenceIndexMaxOrMin, int level);
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
	public: bool componentWordConnectivityTestsCompareToMemory(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int sequenceIndexProspectiveFirst, SANIGroupNeuralNetwork* ownerGroup, SANIComponent* lastActiveComponent);
	#endif
	#endif
		
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
	public: bool calculatePerformanceWeightOfParseTree(SANIGroupParseTree* currentParseTreeGroup, int* maxWeight);
		public: bool setPerformanceWeightOfParseTree(SANIGroupParseTree* currentParseTreeGroup, int maxWeight);
	#endif

	#ifdef SANI_PARSE
	#ifdef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	public: bool updatePerformance(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer);
	#endif
		public: bool updateAndVerifyPerformanceGroupSentence(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer);
		public: bool updatePerformanceGroupSentence(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer);
		public: bool verifyPerformanceGroupSentence(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer);
			public: bool updateAndOrVerifyPerformanceGroup(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer, const bool updatePerformance, const bool verifyPerformance, const bool partialSentenceSequence, const SANIForwardPropogationSignalData* forwardPropogationSignalData);
	#endif
		public: bool traceBackpropParseTreeSetTraced(SANIGroupParseTree* currentParseTreeGroup, int level);
		#ifdef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
		public: bool traceBackpropParseTreePerformance(SANIGroupParseTree* currentParseTreeGroup, int level, int* performance);
		#else
		public: bool traceBackpropParseTreeSequenceIndexCoverage(SANIGroupParseTree* currentParseTreeGroup, int level, vector<LRPpreprocessorPlainTextWord*>* sentenceContents);
		#endif
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		public: bool traceBackpropParseTreeCalculateWeight(SANIGroupParseTree* currentParseTreeGroup, int level, int* maxWeight);
		#endif
		#ifdef SANI_DEBUG_PARSE_TREE_PRINT_SUPPORT_RECURSION
		public: bool traceBackpropParseTreePrint(SANIGroupParseTree* currentParseTreeGroup, int level);
		#endif
	#endif
		
	#ifdef SANI_PARSE
	#ifdef SANI_HEAVY
	#ifdef SANI_HEAVY_OPTIMISED
	public: bool resetGroupParseTreeGroupRef(SANIGroupActivationMemory* activationMemoryGroup, SANIGroupParseTree* currentParseTreeGroup, const bool deleteExistingParseTreeGroupRef);
	#else
	public: bool resetGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef);
	#endif
	#else
	public: bool resetGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef);
	public: bool restoreGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, SANIGroupParseTree* parseTreeGroupOrig);
	#endif
	#endif
	
	#ifdef SANI_PARSE
	public: bool resetNeuronBackprop(SANIGroupParseTree* currentParseTreeGroup, const int groupBoolIndexType);
	#endif
		
	#ifdef SANI_FREE_MEMORY
	public: bool initialiseParseTreeGroupList(vector<SANIGroupType*>* SANIGroupTypes, vector<SANIGroupParseTree*>* parseTreeGroupList);
	public: bool deinitialiseParseTreeGroupList(vector<SANIGroupType*>* SANIGroupTypes, vector<SANIGroupParseTree*>* parseTreeGroupList);
	#endif
	
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	public: void updateParseTreeMaxMinSequenceIndexOfParent(SANIGroupParseTree* parentGroup, SANIComponentParseTree* parentComponent, SANIGroupParseTree* childGroup);
		#ifdef SANI_SEQUENCE_GRAMMAR
		public: void updateParseTreeMaxMinSequenceIndexOfParentBasic(SANIGroupParseTree* parentGroup, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIGroupParseTree* childGroup);
		#endif
	#endif
	
	
	#endif
	

};


#endif
