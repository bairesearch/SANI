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
 * File Name: SANIposRelTranslatorPermutations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1o3b 16-November-2020
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Part-of-speech Relation Translator Permutations
 * /
 *******************************************************************************/


#include "SANIposRelTranslatorPermutations.hpp"

#ifdef USE_SANI	//GIA_POS_REL_TRANSLATOR_RULES_GIA3

#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
static int performanceMaxPermutationIndexRecord;	//temp var (per sentence)
static bool parseIsolatedSubreferenceSetsRecord;
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
#include "SANIpropagateCompact.hpp"
#include "SANIneuralNetworkOperations.hpp"

#endif

bool SANIposRelTranslatorPermutationsClass::executePosRelTranslatorWrapper(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;

	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
	cout << "SANIpropagateCompactClass().initialiseIncrementalGeneration" << endl;
	SANIpropagateCompactClass().initialiseIncrementalGeneration();
	cout << "GIAneuralNetworkOperationsClass().initialiseIncrementalGeneration" << endl;
	GIAneuralNetworkOperationsClass().initialiseIncrementalGeneration();
	#endif
	
	/*
	algorithm (v0.1);
		For every word in sentence
			calculate pos ambiguity info (possible pos types)
		Generate all possible pos sequences representing sentence
		For every possible pos sequence representing sentence
			identify the set of logic/reference set groups supporting (successfully parsing) the pos sequence
				interpret unknown words (pos types) as wild cards
				take the logic/reference set group that most successfully parses the sentence

		In the future use a neural net to train the system to identify new rule groups (or upgrade/refine rule groups)
	*/
	
	//cout << "SANIposRelTranslatorPermutationsClass::executePosRelTranslatorWrapper" << endl;


	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_SENTENCE_INDEXING
	//code from GIAtranslatorClass::parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences;
	//count maxNumberSentences
	LRPpreprocessorSentence* currentSentenceInList = translatorVariables->LRPpreprocessorTranslatorVariables.firstLRPpreprocessorSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		if(currentSentenceInList->sentenceIndexOriginal > translatorVariables->maxNumberSentences)
		{
			translatorVariables->maxNumberSentences = currentSentenceInList->sentenceIndexOriginal;
		}
		currentSentenceInList = currentSentenceInList->next;
	}	
	#endif
	
	LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList = translatorVariables->LRPpreprocessorTranslatorVariables.firstLRPpreprocessorSentenceInList;
	while(currentLRPpreprocessorSentenceInList->next != NULL)
	{	
		#ifdef SANI_SEQUENCE_GRAMMAR_STORE_SENTENCE_INDEXING
		translatorVariables->currentPreprocessorSentenceInList = currentLRPpreprocessorSentenceInList;
		#endif
		
		#ifdef SANI_DEBUG_RULES_PRINT_SENTENCES
		cout << "SANIposRelTranslatorPermutationsClass::executePosRelTranslator{}: sentence " << currentLRPpreprocessorSentenceInList->sentenceIndexOriginal << ", sentenceContents = " << LRPpreprocessorWordClassObject.printWordListString(LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList)) << endl;
		#endif
		
		if(!executePosRelTranslatorWrapper2(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList))
		{
			result = false;
		}
				
		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
	}
	
	#ifndef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC
	if(!transferParseTreePOStypeInferredToWordList(translatorVariables))	//is this required without GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START?
	{
		result = false;
	}
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR
	#ifdef SANI_DEBUG_COUNT_GENERATED_NEURONS 
	SANIGroupType* groupType = SANInodes.getSequenceGrammarGroupTypeDefault(SANIGroupTypes);
	cout << "SANI_DEBUG_COUNT_GENERATED_NEURONS: groupType->groups.size() = " << groupType->groups.size() << endl;
	#endif
	#endif
	
	return result;
}

bool SANIposRelTranslatorPermutationsClass::executePosRelTranslatorWrapper2(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList)
{
	bool result = true;

	/*
	algorithm (v0.1);
		For every word in sentence
			calculate pos ambiguity info (possible pos types)
		Generate all possible pos sequences representing sentence
		For every possible pos sequence representing sentence
			identify the set of logic/reference set groups supporting (successfully parsing) the pos sequence
				interpret unknown words (pos types) as wild cards
				take the logic/reference set group that most successfully parses the sentence

		In the future use a neural net to train the system to identify new rule groups (or upgrade/refine rule groups)
	*/
	
	//cout << "SANIposRelTranslatorPermutationsClass::executePosRelTranslatorWrapper2" << endl;

	vector<uint64_t> POSambiguityInfoPermutation;
	vector<LRPpreprocessorPlainTextWord*>* sentenceContents = LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList);

	#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	vector<string>* explicitWordList = SANIformation.getExplicitWordList();
	#else
	vector<string>* explicitWordList = NULL;
	#endif

	if(!LRPpreprocessorPOStagger.determinePOSambiguityInfoWrapper(sentenceContents, &POSambiguityInfoPermutation, explicitWordList))
	{
		result = false;
	}

	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_SUBNETS_BASED_ON_POS_UNAMBIGUOUS_SEGMENTS
	if(!executePosRelTranslatorOnPOSunambiguousSentenceSubsets(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList, &POSambiguityInfoPermutation))
	{
		result = false;
	}	
	#else
	if(!executePosRelTranslator(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList, &POSambiguityInfoPermutation))
	{
		result = false;
	}
	#endif

	//cout << "2 currentLRPpreprocessorSentenceInList->firstParseTreeGroup = " << currentLRPpreprocessorSentenceInList->firstParseTreeGroup->groupName << endl;

	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_SUBNETS_BASED_ON_POS_UNAMBIGUOUS_SEGMENTS
bool SANIposRelTranslatorPermutationsClass::executePosRelTranslatorOnPOSunambiguousSentenceSubsets(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, const vector<uint64_t>* POSambiguityInfoPermutation)
{
	bool result = true;
	
	vector<LRPpreprocessorPlainTextWord*>* sentenceContents = LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList);
	vector<LRPpreprocessorPlainTextWord*> sentenceContentsSubset;
	vector<uint64_t> POSambiguityInfoPermutationSubset;
	for(int w = 0; w<sentenceContents->size(); w++)
	{
		LRPpreprocessorPlainTextWord* word = (*sentenceContents)[w];
		uint64_t POSambiguityInfo = (*POSambiguityInfoPermutation)[w];
		
		uchar unambiguousPOSinfoIndex;
		bool treatWordAsAmbiguousIfNullPOSvalue = true;	//CHECKTHIS
		if(LRPpreprocessorPOStaggerDatabase.determinePOSambiguityInfoIsAmbiguous(POSambiguityInfo, &unambiguousPOSinfoIndex, treatWordAsAmbiguousIfNullPOSvalue))
		{
			bool passedPrelimRequirements = false;
			if(sentenceContentsSubset.size() >= SANI_SEQUENCE_GRAMMAR_GENERATE_SUBNETS_BASED_ON_POS_UNAMBIGUOUS_SEGMENTS_MIN_LENGTH)	//(sentenceContentsSubset.size() > 0)
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_CONSECUTIVE_POS
				if(!detectLongConsecutivePOS(&POSambiguityInfoPermutationSubset))
				{
				#endif
					passedPrelimRequirements = true;
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_CONSECUTIVE_POS
				}
				#endif
			}
			
			if(passedPrelimRequirements)
			{
				//cout << "sentenceContentsSubset.size() = " << sentenceContentsSubset.size() << endl;
				if(!executePosRelTranslatorOnPOSunambiguousSentenceSubset(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList, &sentenceContentsSubset, &POSambiguityInfoPermutationSubset))
				{
					result = false;
				}
			}
			else
			{
				sentenceContentsSubset.clear();
				POSambiguityInfoPermutationSubset.clear();
			}
		}
		else
		{
			sentenceContentsSubset.push_back(word);
			POSambiguityInfoPermutationSubset.push_back(POSambiguityInfo);
		}
	}
	if(sentenceContentsSubset.size() >= SANI_SEQUENCE_GRAMMAR_GENERATE_SUBNETS_BASED_ON_POS_UNAMBIGUOUS_SEGMENTS_MIN_LENGTH)	//(sentenceContentsSubset.size() > 0)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_CONSECUTIVE_POS
		if(!detectLongConsecutivePOS(&POSambiguityInfoPermutationSubset))
		{
		#endif
			//cout << "sentenceContentsSubset.size() = " << sentenceContentsSubset.size() << endl;
			if(!executePosRelTranslatorOnPOSunambiguousSentenceSubset(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList, &sentenceContentsSubset, &POSambiguityInfoPermutationSubset))
			{
				result = false;
			}
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_CONSECUTIVE_POS
		}
		#endif
	}
	
	//restore word indices
	for(int w = 0; w<sentenceContents->size(); w++)
	{
		LRPpreprocessorPlainTextWord* word = (*sentenceContents)[w];
		word->translatorSentenceWordIndex = w;
	}
	
	return result;
}
bool SANIposRelTranslatorPermutationsClass::executePosRelTranslatorOnPOSunambiguousSentenceSubset(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, vector<LRPpreprocessorPlainTextWord*>* sentenceContentsSubset, vector<uint64_t>* POSambiguityInfoPermutationSubset)
{
	bool result = true;
	
	//set sentenceContents;
	vector<LRPpreprocessorPlainTextWord*> sentenceContentsBackup = *(LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList));	//currentLRPpreprocessorSentenceInList->sentenceContentsLRP
	*(LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList)) = *sentenceContentsSubset;
	
	//temporarily modify word indices
	for(int w=0; w<sentenceContentsSubset->size(); w++)
	{
		LRPpreprocessorPlainTextWord* word = (*sentenceContentsSubset)[w];
		word->translatorSentenceWordIndex = w;
	}
	
	if(!executePosRelTranslator(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList, POSambiguityInfoPermutationSubset))
	{
		result = false;
	}
	
	//restore sentenceContents;
	*(LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList)) = sentenceContentsBackup;
	
	sentenceContentsSubset->clear();
	POSambiguityInfoPermutationSubset->clear();
	
	return result;
}	
#endif



bool SANIposRelTranslatorPermutationsClass::executePosRelTranslator(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, vector<uint64_t>* POSambiguityInfoPermutation)
{
	bool result = true;

	#ifdef SANI_DEBUG_RULES_PRINT_SENTENCES
	cout << endl;
	LRPpreprocessorSentenceClassObject.printSentence(LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList), true);
	#endif

	vector<LRPpreprocessorPlainTextWord*>* sentenceContents = LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList);

	/*
	algorithm (v0.1);
		For every word in sentence
			calculate pos ambiguity info (possible pos types)
		Generate all possible pos sequences representing sentence
		For every possible pos sequence representing sentence
			identify the set of logic/reference set groups supporting (successfully parsing) the pos sequence
				interpret unknown words (pos types) as wild cards
				take the logic/reference set group that most successfully parses the sentence

		In the future use a neural net to train the system to identify new rule groups (or upgrade/refine rule groups)
	*/
	
	#ifdef SANI_FORWARD
	SANIGroupParseTree* firstParseTreeGroup = NULL;	//not used by SANI (write directly to currentLRPpreprocessorSentenceInList->firstParseTreeGroup)
	#else
	SANIGroupParseTree* firstParseTreeGroup = new SANIGroupParseTree();
	currentLRPpreprocessorSentenceInList->firstParseTreeGroup = firstParseTreeGroup;
	#endif
		
	int performance = 0; 
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	int minIndexOfMatchesFoundBackupOptimum = SANInodes.calculateMinIndexOfMatchesFound(sentenceContents);
	vector<vector<uint64_t>*> POSambiguityInfoUnambiguousPermutationArray;
	vector<uint64_t>* POSambiguityInfoUnambiguousPermutationNew = new vector<uint64_t>(POSambiguityInfoPermutation->size());
	POSambiguityInfoUnambiguousPermutationArray.push_back(POSambiguityInfoUnambiguousPermutationNew);
	LRPpreprocessorPOStagger.generatePOSambiguityInfoUnambiguousPermutationArray(&POSambiguityInfoUnambiguousPermutationArray, POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationNew, 0);
	int iOptimum = 0;
	#ifndef SANI_SEQUENCE_GRAMMAR_GENERATE_SUBNETS_BASED_ON_POS_UNAMBIGUOUS_SEGMENTS
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_CONSECUTIVE_POS
	for(int i=0; i<POSambiguityInfoUnambiguousPermutationArray.size();)
	{
		vector<uint64_t>* POSambiguityInfoPermutationTemp = (POSambiguityInfoUnambiguousPermutationArray)[i];
		if(detectLongConsecutivePOS(POSambiguityInfoPermutationTemp))
		{
			POSambiguityInfoUnambiguousPermutationArray.erase(POSambiguityInfoUnambiguousPermutationArray.begin()+i);
		}
		else
		{
			i++;
		}
	}
	#endif
	#endif
	#endif
	bool foundParse = false;
	

	
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	if(translatorVariables->parserDemarkateOptimumPathway)
	{
	#endif
		bool parseIsolatedSubreferenceSets1 = false;
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS_OPTIMISED
		LRPpreprocessorSentenceClassObject.calculateParseIsolatedSubreferenceSets1(currentLRPpreprocessorSentenceInList, &parseIsolatedSubreferenceSets1);
		#endif

		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		if(generateParseTreeIntroWrapper(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSets1, &POSambiguityInfoUnambiguousPermutationArray, &iOptimum, &minIndexOfMatchesFoundBackupOptimum))	
		#else
		if(generateParseTreeIntroWrapper(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSets1, POSambiguityInfoPermutation))
		#endif
		{
			foundParse = true;
			#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
			parseIsolatedSubreferenceSetsRecord = parseIsolatedSubreferenceSets1;
			#endif
		}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
		if(!foundParse)
		{
			bool parseIsolatedSubreferenceSets2 = true;
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS_OPTIMISED
			if(LRPpreprocessorSentenceClassObject.calculateParseIsolatedSubreferenceSets2(currentLRPpreprocessorSentenceInList, &parseIsolatedSubreferenceSets2))
			{
			#endif
				#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
				if(generateParseTreeIntroWrapper(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSets2, &POSambiguityInfoUnambiguousPermutationArray, &iOptimum, &minIndexOfMatchesFoundBackupOptimum))	
				#else
				if(generateParseTreeIntroWrapper(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSets2, POSambiguityInfoPermutation))
				#endif
				{
					foundParse = true;
					#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
					parseIsolatedSubreferenceSetsRecord = parseIsolatedSubreferenceSets2;
					#endif
				}
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS_OPTIMISED
			}
			#endif
		}
		#endif
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	}
	else
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		if(generateParseTreeIntroWrapper(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSetsRecord, &POSambiguityInfoUnambiguousPermutationArray, &iOptimum, &minIndexOfMatchesFoundBackupOptimum))	
		#else
		if(generateParseTreeIntroWrapper(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes, currentLRPpreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSetsRecord, POSambiguityInfoPermutation))
		#endif
		{
			foundParse = true;
		}
	}
	#endif
	
	//#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(foundParse)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR
		cout << "SANIposRelTranslatorPermutationsClass::executePosRelTranslator{}: Successfully parsed sentence " << currentLRPpreprocessorSentenceInList->sentenceIndexOriginal << ", ";
		LRPpreprocessorSentenceClassObject.printSentence(LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList), true);
		//cout << "sentenceContents = " << LRPpreprocessorWordClassObject.printWordListString(LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList)) << endl;
		#endif
	}
	else
	{
		#ifndef SANI_FORWARD
		SANIpropagateInverse.clearAllWordsAlreadyFoundMatchInComponent(sentenceContents, minIndexOfMatchesFoundBackupOptimum);	//redundant?	
		#endif
		cerr << "SANIposRelTranslatorPermutationsClass::executePosRelTranslator{}: Failed to parse sentence " << currentLRPpreprocessorSentenceInList->sentenceIndexOriginal << ", ";
		LRPpreprocessorSentenceClassObject.printSentence(LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList), true);
		//cerr << "SANIposRelTranslatorPermutationsClass::executePosRelTranslator{}: Failed to parse sentence " << currentLRPpreprocessorSentenceInList->sentenceIndexOriginal << ", sentenceContents = " << LRPpreprocessorWordClassObject.printWordListString(LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList)) << endl;
		//exit(EXIT_ERROR);
	}
	//#endif

	return result;
}


#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
bool SANIposRelTranslatorPermutationsClass::generateParseTreeIntroWrapper(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroup, int* performance, bool parseIsolatedSubreferenceSets, const vector<vector<uint64_t>*>* POSambiguityInfoUnambiguousPermutationArray, int* iOptimum, int* minIndexOfMatchesFoundBackupOptimum)
#else
bool SANIposRelTranslatorPermutationsClass::generateParseTreeIntroWrapper(SANItranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroup, int* performance, bool parseIsolatedSubreferenceSets, const vector<uint64_t>* POSambiguityInfoPermutation)
#endif
{
	#ifdef SANI_SEQUENCE_GRAMMAR_THROW_SENTENCE_FAIL_ERROR_IF_ANY_POS_PERMUTATION_FAILS
	bool result = true;
	#else
	bool result = false;
	#endif
	
	vector<LRPpreprocessorPlainTextWord*>* sentenceContents = LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList);

	#ifdef SANI_FORWARD
	int performanceMaxPermutationIndex = INT_DEFAULT_VALUE;
	int performanceMax = 0;
	#endif
	
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	if(translatorVariables->parserDemarkateOptimumPathway)
	{
	#endif
	
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_MAX_POS_PERMUTATIONS
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_MAX_POS_PERMUTATIONS_FOR_PARTICULAR_SENTENCE
		int maxPosPermutations = POSambiguityInfoUnambiguousPermutationArray->size();
		if(currentLRPpreprocessorSentenceInList->sentenceIndexOriginal == SANI_DEBUG_SEQUENCE_GRAMMAR_MAX_POS_PERMUTATIONS_FOR_PARTICULAR_SENTENCE_INDEX)
		{
			cout << "\n\n SANI_DEBUG_SEQUENCE_GRAMMAR_MAX_POS_PERMUTATIONS_FOR_PARTICULAR_SENTENCE" << endl;
			maxPosPermutations = SANI_DEBUG_SEQUENCE_GRAMMAR_MAX_POS_PERMUTATIONS_NUM;
		}
		for(int i=0; i<maxPosPermutations; i++)
		#else
		for(int i=0; i<SANI_DEBUG_SEQUENCE_GRAMMAR_MAX_POS_PERMUTATIONS_NUM; i++)
		#endif
		#else
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SINGLE_POS_PERMUTATION
		for(int i=0; i<1; i++)
		#else
		for(int i=0; i<POSambiguityInfoUnambiguousPermutationArray->size(); i++)
		#endif
		#endif
		{
			vector<uint64_t>* POSambiguityInfoPermutationTemp = (*POSambiguityInfoUnambiguousPermutationArray)[i];
			LRPpreprocessorPOStagger.setSentenceContentsWordsUnambiguousPOSindex(sentenceContents, POSambiguityInfoPermutationTemp);
	
			#ifdef SANI_DEBUG_PROPAGATE
			cout << "POSambiguityInfoUnambiguousPermutationArray index = " << i << endl;
			//LRPpreprocessorPOStagger.printPOSambiguityInfoPermutationAssumeUnambiguous(POSambiguityInfoPermutationTemp);	//printPOSambiguityInfoPermutation
			#endif
			#ifdef SANI_DEBUG_RULES_PRINT_PARSE_PROCESS3
			cout << "POSambiguityInfoUnambiguousPermutationArray index = " << i << endl;
			LRPpreprocessorPOStagger.printPOSambiguityInfoPermutationAssumeUnambiguous(POSambiguityInfoPermutationTemp);		
			#endif
	
			#ifdef SANI_FORWARD
			SANIGroupParseTree* firstParseTreeGroupTemp = NULL;
			#else
			SANIGroupParseTree* firstParseTreeGroupTemp = new SANIGroupParseTree();		
			#endif
	
			int minIndexOfMatchesFoundBackup2 = SANInodes.calculateMinIndexOfMatchesFound(sentenceContents);
		#else
		for(int w=0; w<sentenceContents->size(); w++)
		{
			(sentenceContents->at(w))->POSambiguityInfo = (*POSambiguityInfoPermutation)[w];
		}
		#ifdef SANI_FORWARD
		SANIGroupParseTree* firstParseTreeGroupTemp = NULL;
		#else
		SANIGroupParseTree* firstParseTreeGroupTemp = firstParseTreeGroup;
		#endif
		#endif
		

			int performanceTemp = 0;
			bool passedTemp = false;
			
			#ifdef SANI_FORWARD
			bool parserEnabled = false;
			#ifdef SANI_SEQUENCE_GRAMMAR
			bool createNewConnections = true; 
			if(SANIgenerateCompact.generatePosRelTranslatorNeuralNetwork(translatorVariables, SANIGroupTypes, sentenceContents, &firstParseTreeGroupTemp, parseIsolatedSubreferenceSets, parserEnabled, &performanceTemp, createNewConnections))
			#else
			if(SANIpropagate.executePosRelTranslatorNeuralNetwork(translatorVariables, SANIGroupTypes, sentenceContents, &firstParseTreeGroupTemp, parseIsolatedSubreferenceSets, parserEnabled, &performanceTemp))
			#endif
			#else	
			if(SANIpropagateInverse.executePosRelTranslatorNeuralNetworkInverse(SANIrulesTokenLayers, SANIGroupTypes, sentenceContents, firstParseTreeGroupTemp, &performanceTemp, parseIsolatedSubreferenceSets))
			#endif
			{
				passedTemp =  true;
				#ifndef SANI_SEQUENCE_GRAMMAR_THROW_SENTENCE_FAIL_ERROR_IF_ANY_POS_PERMUTATION_FAILS
				result = true;
				#endif
				
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_PRINT_SUCCESSFULLY_TRACED_OR_GENERATED_PARSE_TREES
				SANIpropagateOperations.printParseTree(firstParseTreeGroupTemp, 0);
				#endif
				
				#ifdef SANI_DEBUG_RULES_PRINT_SENTENCES
				cout << "executePosRelTranslatorNeuralNetwork passed" << endl;	
				#endif
				#ifdef SANI_DEBUG_RULES_PRINT_PARSE_PROCESS
				cout << "passedTemp" << endl;
				#endif
				#ifndef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START	
				*performance = performanceTemp;
				#endif
				
				//cout << "performanceTemp = " << performanceTemp << endl;
				//exit(EXIT_ERROR);
			}
			else
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_THROW_SENTENCE_FAIL_ERROR_IF_ANY_POS_PERMUTATION_FAILS
				result = false;
				#endif
				
				#ifdef SANI_DEBUG_RULES_PRINT_SENTENCES_FAIL_ONLY
				cout << "executePosRelTranslatorNeuralNetwork failed" << endl;
				#endif
				#ifdef SANI_DEBUG_RULES_PRINT_SENTENCES
				cout << "executePosRelTranslatorNeuralNetwork failed" << endl;
				#endif		
			}
	
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		
		#ifdef SANI_FORWARD
			if(updatePerformanceNeuralNetwork(performanceTemp, performance, currentLRPpreprocessorSentenceInList, firstParseTreeGroupTemp, passedTemp, i, &performanceMaxPermutationIndex))
			{
				*iOptimum = i;	
			}
			
			//cout << "currentLRPpreprocessorSentenceInList->firstParseTreeGroup = " << currentLRPpreprocessorSentenceInList->firstParseTreeGroup->groupName << endl;
	
		}
		#else
			if(SANIpropagateInverse.updatePerformance(performanceTemp, performance, firstParseTreeGroup, firstParseTreeGroupTemp, passedTemp, minIndexOfMatchesFoundBackupOptimum, sentenceContents, minIndexOfMatchesFoundBackup2, NULL))
			{
				*iOptimum = i;	
			}
		}
		if(result)
		{
			//cout << "performance = " << performance << endl;
			SANIpropagateInverse.restoreAllWordsAlreadyFoundMatchInComponent(sentenceContents, *performance);
		}
		#endif
		
		#endif
			
	
		#ifdef SANI_PARSE_SIMULTANEOUS
		if(result)
		{
			
			//regenerate parse tree with optimum path
			bool parserEnabled = true;
			vector<uint64_t>* POSambiguityInfoPermutationTemp = (*POSambiguityInfoUnambiguousPermutationArray)[performanceMaxPermutationIndex];
			LRPpreprocessorPOStagger.setSentenceContentsWordsUnambiguousPOSindex(sentenceContents, POSambiguityInfoPermutationTemp);
			int performanceTemp = 0;
			
			#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_UNOPTIMISED
			if(translatorVariables->parserAllowed)
			{
			#endif
				#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
				parserEnabled = false; 
				performanceTemp = 0;
				//derive optimum pathway (store in parseTree)
				if(!SANIpropagate.executePosRelTranslatorNeuralNetwork(translatorVariables, SANIGroupTypes, sentenceContents, &firstParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performanceTemp))
				{
					result = false;	
				}
				//mark optimum pathway
				if(!SANIpropagate.demarkateOptimumPathwayBackprop(firstParseTreeGroup))
				{
					result = false;
				}
				#ifdef SANI_FREE_MEMORY
				SANInodes.deleteParseTree(firstParseTreeGroup, 0);
				#endif
				#endif
				
				parserEnabled = true;
			#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_UNOPTIMISED
			}
			else
			{
				parserEnabled = false;
			}
			#endif
			#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
			performanceMaxPermutationIndexRecord = performanceMaxPermutationIndex;
			#else
			result = false;
			performanceTemp = 0;
			if(SANIpropagate.executePosRelTranslatorNeuralNetwork(translatorVariables, SANIGroupTypes, sentenceContents, &firstParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performanceTemp))
			{
				result = true;	
				*performance = sentenceContents->size();	//hard set to maximum performance
			}
	
			#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
			SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_DEMARKATEOPTIMUMPATHWAY_RESET);
			#endif
			
			#endif
	
		}
		else
		{
			performanceMaxPermutationIndexRecord = INT_DEFAULT_VALUE;
		}
		#endif
		
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	}
	else
	{
		if(performanceMaxPermutationIndexRecord != INT_DEFAULT_VALUE)
		{
			bool parserEnabled = true;
			vector<uint64_t>* POSambiguityInfoPermutationTemp = (*POSambiguityInfoUnambiguousPermutationArray)[performanceMaxPermutationIndexRecord];
			LRPpreprocessorPOStagger.setSentenceContentsWordsUnambiguousPOSindex(sentenceContents, POSambiguityInfoPermutationTemp);
			int performanceTemp = 0;
			
			result = false;	
			if(SANIpropagate.executePosRelTranslatorNeuralNetwork(translatorVariables, SANIGroupTypes, sentenceContents, &firstParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performanceTemp))
			{
				result = true;	
				*performance = sentenceContents->size();	//hard set to maximum performance
			}
			
			#ifdef SANI_DEBUG_PRINT_FINAL2
			if(firstParseTreeGroup != NULL)
			{
				cout << "(firstParseTreeGroup != NULL)" << endl;
				SANIpropagateOperations.printParseTree(firstParseTreeGroup, 0);
			}
			#endif
		}
	}
	#endif

	return result;
}	

	
#ifdef SANI_FORWARD
//based on SANIpropagateInverseClass::updatePerformanceNeuralNetwork
bool SANIposRelTranslatorPermutationsClass::updatePerformanceNeuralNetwork(int performanceTemp, int* performance, LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, SANIGroupParseTree* firstParseTreeGroupTemp, const bool passedTemp, int permutationIndex, int* performanceMaxPermutationIndex)
{
	bool result = false;
	
	if(passedTemp)
	{
		if(performanceTemp > *performance)
		{
			result = true;
			*performance = performanceTemp;
			#ifdef SANI_FREE_MEMORY
			if(currentLRPpreprocessorSentenceInList->firstParseTreeGroup != NULL)
			{
				SANInodes.deleteParseTree(currentLRPpreprocessorSentenceInList->firstParseTreeGroup, 0);
			}
			#endif
			#ifdef SANI_PARSE_SAVE_PARSE_TREE
			currentLRPpreprocessorSentenceInList->firstParseTreeGroup = firstParseTreeGroupTemp;
			#endif
			*performanceMaxPermutationIndex = permutationIndex;
			
			//cout << "firstParseTreeGroup->groupName = " << firstParseTreeGroupTemp->groupName << endl;
			//exit(EXIT_ERROR);
		}
	}

	#ifdef SANI_PARSE_SAVE_PARSE_TREE
	if(!result)
	{
		//delete all subgroups/components recursively in currentParseTreeGroupTemp
		if(firstParseTreeGroupTemp != NULL)
		{
			//deleteAllSubgroupsRecurse(firstParseTreeGroupTemp, 1);	//CHECKTHIS
		}
	}
	#endif
	
	return result;
}

//based on SANIpropagateInverseClass::deleteAllSubgroupsRecurse
//note it assumes subcomponents
bool SANIposRelTranslatorPermutationsClass::deleteAllSubgroupsRecurse(SANIGroupParseTree* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			SANIComponentParseTree* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
			if(currentParseTreeComponent->parseTreeGroupRef != NULL)
			{
				#ifdef SANI_DEBUG_RULES_PRINT_PARSE_PROCESS
				SANIpropagateOperations.printParseTreeDebugIndentation(layer);
				cout << "deleteAllSubgroupsRecurse" << endl;
				#endif
				
				if(!deleteAllSubgroupsRecurse(currentParseTreeComponent->parseTreeGroupRef, layer+1))
				{
					result = false;
				}
			}
			
			delete currentParseTreeComponent;
		}
		delete currentParseTreeGroup;
	}
	
	return result;
}

#endif



#ifndef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC
bool SANIposRelTranslatorPermutationsClass::transferParseTreePOStypeInferredToWordList(const SANItranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef SANI_DEBUG_RULES
	cout << "printParseTreeDebug: " << endl;
	#endif
	
	LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList = translatorVariables->LRPpreprocessorTranslatorVariables.firstLRPpreprocessorSentenceInList;
	while(currentLRPpreprocessorSentenceInList->next != NULL)
	{
		vector<LRPpreprocessorPlainTextWord*>* sentenceContents = LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList);

		//this will replace the sentenceContents word->wordPOStypeInferred values with their ideal value as stored in the parse tree (in the case where the ideal word->wordPOStypeInferred values were overwritten by a more recent bad parse):
		
		#ifdef SANI_FORWARD
		if(currentLRPpreprocessorSentenceInList->firstParseTreeGroup != NULL)
		{
		#endif
			const SANIGroupParseTree* firstParseTreeGroup = currentLRPpreprocessorSentenceInList->firstParseTreeGroup;
			int layer = SANI_POS_REL_TRANSLATOR_RULES_LAYER_START;
			#ifdef SANI_DEBUG_RULES
			cout << "firstParseTreeGroup: groupTypeName = " << firstParseTreeGroup->groupTypeName << ", groupName = " << firstParseTreeGroup->groupName << endl;
			#endif
			if(!transferParseTreePOStypeInferredToWordList(firstParseTreeGroup, layer))
			{
				result = false;
			}

			#ifdef SANI_DEBUG_RULES
			for(int w=0; w<sentenceContents->size(); w++)
			{
				LRPpreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
				cout << "LRPpreprocessorPOStypeNameArray[contextWord->wordPOStypeInferred] = " << LRPpreprocessorPOStypeNameArray[contextWord->wordPOStypeInferred] << endl;
			}
			#endif
		#ifdef SANI_FORWARD
		}
		#endif
		
		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
	}
	#ifdef SANI_DEBUG_RULES
	cout << "\n" << endl;
	#endif

	return result;
}

bool SANIposRelTranslatorPermutationsClass::transferParseTreePOStypeInferredToWordList(const SANIGroupParseTree* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANInodes.printParseTreeDebugIndentation(layer);
		
		//#ifdef SANI_DEBUG_RULES
		//cout << "transferParseTreePOStypeInferredToWordList currentParseTreeGroup: " <<  currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;		
		//#endif
		//cout << "(currentParseTreeGroup->components).size() = " << (currentParseTreeGroup->components).size() << endl;
		//cout << "i = " << i << endl;
		
		SANIComponentParseTree* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
		
		//if(currentParseTreeComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)	//redundant
		//{
			if(currentParseTreeComponent->candidateStringMatch != NULL)
			{
				//this will replace the sentenceContents word->wordPOStypeInferred with the ideal value as stored in the parse tree (in the case where the ideal word->wordPOStypeInferred value was overwritten by more recent bad parse):
				currentParseTreeComponent->candidateStringMatch->wordPOStypeInferred = currentParseTreeComponent->wordPOStypeInferred;
				//cout << "currentParseTreeComponent->candidateStringMatch->wordPOStypeInferred = " << currentParseTreeComponent->candidateStringMatch->wordPOStypeInferred << endl;
			}
		//}
		
		#ifdef SANI_DEBUG_RULES
		SANIpropagateOperations.printComponent(currentParseTreeComponent, layer);
		#endif
		if(currentParseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!transferParseTreePOStypeInferredToWordList(currentParseTreeComponent->parseTreeGroupRef, layer+1))
			{
				result = false;
			}
		}
	}
	
	return result;
}
#endif


#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_CONSECUTIVE_POS
bool SANIposRelTranslatorPermutationsClass::detectLongConsecutivePOS(const vector<uint64_t>* POSambiguityInfoPermutation)
{
	bool foundLongConsecutivePOS = false;
	
	int consecutivePOScount = 1;
	uint64_t consecutivePOStype = INT_DEFAULT_VALUE;
	
	for(int i=0; i<POSambiguityInfoPermutation->size(); i++)
	{
		uint64_t currentPOStype = (*POSambiguityInfoPermutation)[i];
		
		if(currentPOStype == consecutivePOStype)
		{
			consecutivePOScount++;
			if(consecutivePOScount > SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_CONSECUTIVE_POS_MAX_ALLOWED)
			{
				foundLongConsecutivePOS = true;
				
				//cout << "SANIposRelTranslatorPermutationsClass::detectLongConsecutivePOS" << endl;
				//LRPpreprocessorPOStagger.printPOSambiguityInfo(currentPOStype);
			}
		}
		else
		{
			consecutivePOScount = 1;
		}
		
		consecutivePOStype = currentPOStype;
	}
	
	return foundLongConsecutivePOS;
}
#endif
				
				

#endif


