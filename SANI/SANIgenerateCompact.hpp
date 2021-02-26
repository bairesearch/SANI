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
 * File Name: SANIgenerateCompact.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1o5b 25-February-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_GENERATE_COMPACT
#define HEADER_SANI_GENERATE_COMPACT

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
#include "SANIpropagateOperations.hpp"
#include "SANIpropagateOperationsParseTree.hpp"
#include "SANIpropagateCompact.hpp"
#include "SANIgenerateCompactIdentify.hpp"
#include "SANIformation.hpp"
#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
#include "SANIgenerateCompactSectionedVariable.hpp"
#else
#include "SANIgenerateCompactSectionedUnique.hpp"
#endif
#else
#include "SANIgenerateCompactContinuous.hpp"
#endif




#ifdef SANI_SEQUENCE_GRAMMAR

class SANIgenerateCompactClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: SANIpropagateOperationsParseTreeClass SANIpropagateOperationsParseTree;
	private: SANIpropagateCompactClass SANIpropagateCompact;
	private: SANIformationClass SANIformation;
	private: SANIgenerateCompactIdentifyClass SANIgenerateCompactIdentify;
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_INCREMENTALLY_SECTIONED
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	private: SANIgenerateCompactSectionedVariableClass SANIgenerateCompactSectionedVariable;
	#else
	private: SANIgenerateCompactSectionedUniqueClass SANIgenerateCompactSectionedUnique;
	#endif
	#else
	private: SANIgenerateCompactContinuousClass SANIgenerateCompactContinuous;
	#endif

	private: ANNdisplayClass ANNdisplay;

	public: bool generatePosRelTranslatorNeuralNetworkWrapper(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const bool createNewConnections);
		public: bool generatePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, bool parseIsolatedSubreferenceSets, bool parserEnabled, int* performance, const bool createNewConnections);
			bool generateSentenceNetworkNodes(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
			bool testSentenceNetworkNodes(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree** topLevelParseTreeGroup);			
	
};

#endif

#endif
