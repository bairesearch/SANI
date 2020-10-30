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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1n3b 21-October-2020
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
#include "SANIpropagateCompactGenerate.hpp"
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
#ifdef SANI_SEQUENCE_GRAMMAR
	private: SANIpropagateCompactGenerateClass SANIpropagateCompactGenerate;
#else
	#ifdef SANI_HEAVY
	#ifdef SANI_HEAVY_UNOPTIMISED
	private: SANIpropagateHeavyUnoptimisedClass SANIpropagate;
	#else
	private: SANIpropagateHeavyOptimisedClass SANIpropagate;
	#endif
	#else
	#ifdef SANI_LIGHT_UNOPTIMISED
	private: SANIpropagateLightUnoptimisedClass SANIpropagate;	
	#else
	private: SANIpropagateLightOptimisedClass SANIpropagate;	
	#endif
	#endif
#endif
	#else
	private: SANIpropagateInverseClass SANIpropagateInverse;	
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


	public: bool executeTxtRelTranslatorWrapper(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes);
		public: bool executeTxtRelTranslatorWrapper2(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList);
		#ifndef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC
		private: bool transferParseTreePOStypeInferredToWordList(SANItranslatorVariablesClass* translatorVariables);
			private: bool transferParseTreePOStypeInferredToWordList(SANIGroupParseTree* currentParseTreeGroup, int layer);
		#endif
		public: bool executeTxtRelTranslator(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, vector<uint64_t>* POSambiguityInfoPermutation);
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
			private: bool generateParseTreeIntroWrapper(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets, vector<vector<uint64_t>*>* POSambiguityInfoUnambiguousPermutationArray, int* iOptimum, int* minIndexOfMatchesFoundBackupOptimum);
			#else
			private: bool generateParseTreeIntroWrapper(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets, vector<uint64_t>* POSambiguityInfoPermutation);
			#endif
				#ifdef SANI_FORWARD
				private: bool updatePerformanceNeuralNetwork(const int performanceTemp, int* performance, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroupTemp, const bool passedTemp, const int permutationIndex, int* performanceMaxPermutationIndex);
				private: bool deleteAllSubgroupsRecurse(SANIGroupParseTree* currentParseTreeGroup, int layer);
				#endif
			

};
#endif




#endif
