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
 * File Name: SANIposRelTranslatorVariables.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1q1a 25-August-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Part-of-speech Relation Translator Variables
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_POS_REL_TRANSLATOR_VARIABLES
#define HEADER_SANI_POS_REL_TRANSLATOR_VARIABLES

#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif
#ifdef SANI_NEURAL_NETWORK
#include "ANNneuronClass.hpp"
#endif
#ifdef SANI_PARSE_SIMULTANEOUS
#include "GIAtranslatorOperations.hpp"
#endif
#include "LRPpreprocessorSentenceClass.hpp"

#include "SHAREDglobalDefs.hpp"


class SANItranslatorVariablesClass
{
public:

	SANItranslatorVariablesClass(void);
	~SANItranslatorVariablesClass(void);
	
	bool isQuery;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_SENTENCE_INDEXING
	int maxNumberSentences;
	LRPpreprocessorSentence* currentPreprocessorSentenceInList;
	#endif
	
	LRPtranslatorVariablesClass LRPpreprocessorTranslatorVariables;

	#ifdef SANI_NEURAL_NETWORK
	ANNtranslatorVariablesClass* ANNtranslatorVariables;
	#endif
	
	#ifdef SANI_PARSE_SIMULTANEOUS
	GIAtranslatorVariablesClass* GIAtranslatorVariables;
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC
	bool parserAllowed;
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	bool parserDemarkateOptimumPathway;
	#endif
	#endif
	#endif
};


#endif
