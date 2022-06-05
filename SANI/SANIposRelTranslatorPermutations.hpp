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
 * File Name: SANIposRelTranslatorPermutations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Part-of-speech Relation Translator Permutations
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_POS_REL_TRANSLATOR_PERMUTATIONS
#define HEADER_SANI_POS_REL_TRANSLATOR_PERMUTATIONS

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif

#ifdef USE_SANI	//GIA_POS_REL_TRANSLATOR_RULES_GIA3

//#include "SHAREDvars.hpp"
#include "SANIposRelTranslatorVariables.hpp"
//#include "SANIrules.hpp"
#include "SANInodes.hpp"
#ifdef SANI_FORWARD
#include "SANIformation.hpp"
#ifdef SANI_SEQUENCE_GRAMMAR
#include "SANIgenerateCompact.hpp"
#else
#ifdef SANI_HEAVY
#ifdef SANI_HEAVY_UNOPTIMISED
#include "SANIpropagateHeavyUnoptimised.hpp"
#else
#include "SANIpropagateHeavyOptimised.hpp"
#endif
#else
#ifdef SANI_LIGHT_UNOPTIMISED
#include "SANIpropagateLightUnoptimised.hpp"
#else
#include "SANIpropagateLightOptimised.hpp"
#endif
#endif
#endif
#else
#include "SANIpropagateInverse.hpp"
#endif
#include "SANIpropagateOperations.hpp"
#include "LRPpreprocessorPOStagger.hpp"
#include "LRPpreprocessorPOStaggerDatabase.hpp"
#include "LRPpreprocessorWordClass.hpp"

//#include "GIAtranslator.hpp"
#include "SHAREDvars.hpp"

class SANIposRelTranslatorPermutationsClass
{
	//private: SHAREDvarsClass SHAREDvars;
	private: SANInodesClass SANInodes;
	#ifdef SANI_FORWARD
	private: SANIformationClass SANIformation;
	#endif
#ifdef SANI_SEQUENCE_GRAMMAR
	private: SANIgenerateCompactClass SANIgenerateCompact;
#else
#ifdef SANI_FORWARD
	#ifdef SANI_HEAVY
	#ifdef SANI_HEAVY_UNOPTIMISED
	private: SANIpropagateHeavyUnoptimisedClass SANIpropagateHeavyUnoptimised;
	#else
	private: SANIpropagateHeavyOptimisedClass SANIpropagateHeavyOptimised;
	#endif
	#else
	#ifdef SANI_LIGHT_UNOPTIMISED
	private: SANIpropagateLightUnoptimisedClass SANIpropagateLightUnoptimised; 
	#else
	private: SANIpropagateLightOptimisedClass SANIpropagateLightOptimised; 
	#endif
	#endif
#else
	private: SANIpropagateInverseClass SANIpropagateInverse;	
#endif
#endif
	private: SANIpropagateOperationsClass SANIpropagateOperations;	//required for printComponent/printParseTreeDebugIndentation
	/*
	private: SANIrulesClass SANIrules;
	*/
	private: LRPpreprocessorPOStaggerClass LRPpreprocessorPOStagger;
	private: LRPpreprocessorPOStaggerDatabaseClass LRPpreprocessorPOStaggerDatabase;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	
	private: SHAREDvarsClass SHAREDvars;


	public: bool executePosRelTranslatorWrapper(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes);
		public: bool executePosRelTranslatorWrapper2(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList);
			#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_SUBNETS_BASED_ON_POS_UNAMBIGUOUS_SEGMENTS
			private: bool executePosRelTranslatorOnPOSunambiguousSentenceSubsets(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, const vector<uint64_t>* POSambiguityInfoPermutation);
				private: bool executePosRelTranslatorOnPOSunambiguousSentenceSubset(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, vector<LRPpreprocessorPlainTextWord*>* sentenceContentsSubset, vector<uint64_t>* POSambiguityInfoPermutationSubset);
			#endif
					public: bool executePosRelTranslator(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, vector<uint64_t>* POSambiguityInfoPermutation);
						#ifdef SANI_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
						private: bool generateParseTreeIntroWrapper(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroup, int* performance, bool parseIsolatedSubreferenceSets, const vector<uint64_t>* POSambiguityInfoPermutation, const vector<vector<uint64_t>*>* POSambiguityInfoUnambiguousPermutationArray, int* iOptimum, int* minIndexOfMatchesFoundBackupOptimum);
						#else
						private: bool generateParseTreeIntroWrapper(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroup, int* performance, bool parseIsolatedSubreferenceSets, const vector<uint64_t>* POSambiguityInfoPermutation);
						#endif
							#ifdef SANI_FORWARD
							private: bool updatePerformanceNeuralNetwork(int performanceTemp, int* performance, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroupTemp, const bool passedTemp, int permutationIndex, int* performanceMaxPermutationIndex);
							private: bool deleteAllSubgroupsRecurse(SANIGroupParseTree* currentParseTreeGroup, int layer);
							#endif
		#ifndef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC
		private: bool transferParseTreePOStypeInferredToWordList(const SANItranslatorVariablesClass* translatorVariables);
			private: bool transferParseTreePOStypeInferredToWordList(const SANIGroupParseTree* currentParseTreeGroup, int layer);
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_CONSECUTIVE_POS
		bool detectLongConsecutivePOS(const vector<uint64_t>* POSambiguityInfoPermutation);
		#endif

};
#endif




#endif
