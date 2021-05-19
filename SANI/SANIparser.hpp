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
 * File Name: SANIparser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p9c 17-May-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Parser
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PARSER
#define HEADER_SANI_PARSER

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif

#ifdef USE_GIA
#ifdef SANI_PARSER

//#include "GIAposRelTranslatorParser.hpp"
#include "SANIposRelTranslatorVariables.hpp"
#include "GIAposRelTranslatorParserOperations.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIrules.hpp"
#include "LRPpreprocessorWordClass.hpp"

class SANIparserClass
{
	private: SHAREDvarsClass SHAREDvars;
	//private: GIAposRelTranslatorParserClass GIAposRelTranslatorParser;
	private: GIAposRelTranslatorParserOperationsClass GIAposRelTranslatorParserOperations;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIrulesClass SANIrules;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	
	#ifdef SANI_PARSE_SIMULTANEOUS
	public: bool generateSemanticRelationsFromTxtRelationsWrapperNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, SANIGroupParseTree* firstParseTreeGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, int layer);
		public: bool generateSemanticRelationsFromTxtRelationsNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, SANIGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorParserForwardPropogationSignalData* parserForwardPropogationSignalData, int layer);
	#endif
};

#endif
#endif

#endif
