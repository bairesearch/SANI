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
 * File Name: SANIposRelTranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p5a 19-March-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Part-of-speech Relation Translator
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_POS_REL_TRANSLATOR
#define HEADER_SANI_POS_REL_TRANSLATOR

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif

#include "SANIrules.hpp"
#ifdef SANI_FORWARD
#include "SANIformation.hpp"
#include "SANIpropagateOperations.hpp"
#endif
#ifdef SANI_SEQUENCE_GRAMMAR
#include "SANIpropagateCompact.hpp"
#endif
//#ifndef SANI_PARSE_SIMULTANEOUS
#include "SANIposRelTranslatorPermutations.hpp"
//#endif
#ifdef LRP_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
#include "LRPpreprocessorWordIdentification.hpp"
#endif
#include "LRPpreprocessorWordClass.hpp"
#include "SHAREDvars.hpp"

class SANIposRelTranslatorClass
{
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	private: GIAposRelTranslatorHybridClass GIAposRelTranslatorHybrid;
	#endif
	#ifdef SANI_FORWARD
	private: SANIformationClass SANIformation;
	private: SANIpropagateOperationsClass SANIpropagateOperations;	//required for printComponent/printParseTreeDebugIndentation	
	#endif
	//#ifndef SANI_PARSE_SIMULTANEOUS
	private: SANIposRelTranslatorPermutationsClass SANIposRelTranslatorPermutations;
	//#endif
	private: SANIrulesClass SANIrules;
	#ifdef LRP_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	#endif
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;

	private: SHAREDvarsClass SHAREDvars;

	public: bool parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(SANItranslatorVariablesClass* translatorVariables);

};




#endif
