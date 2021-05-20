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
 * File Name: SANIpropagateInverse.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p10c 20-May-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Inverse
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_INVERSE
#define HEADER_SANI_PROPAGATE_INVERSE

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif

#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3	//or USE_SANI
#ifdef SANI_INVERSE

#ifdef USE_GIA
//#include "GIAtranslatorOperations.hpp"
#endif
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperations.hpp"
#include "SANIposRelTranslatorVariables.hpp"
#include "SANIrules.hpp"
#include "LRPpreprocessorPOStagger.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "LRPpreprocessorSentenceClass.hpp"
#include "SHAREDvars.hpp"

class SANIpropagateInverseClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;	//required for printComponent/printParseTreeDebugIndentation
	private: SANIrulesClass SANIrules;
	private: LRPpreprocessorPOStaggerClass LRPpreprocessorPOStagger;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: SHAREDvarsClass SHAREDvars;

	public: bool executePosRelTranslatorNeuralNetworkInverse(vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets);
		private: bool generateParseTreeGroupType(vector<XMLparserTag*>* SANIrulesTokenLayers, const SANIGroupType* groupType, vector<LRPpreprocessorPlainTextWord*>* sentenceContentsSubset, SANIGroupParseTree* currentParseTreeGroup, int* performance, int layer, const string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType, SANIComponentParseTree* previousParseTreeComponent);
			private: bool generateParseTreeGroup(vector<XMLparserTag*>* SANIrulesTokenLayers, SANIGroupNeuralNetwork* group, vector<LRPpreprocessorPlainTextWord*>* sentenceContentsSubset, SANIGroupParseTree* currentParseTreeGroup, int* performance, int layer, const string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
				private: bool generateRulesGroupTreeComponents(vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIComponentNeuralNetwork*>* components, vector<LRPpreprocessorPlainTextWord*>* sentenceContentsSubset, SANIGroupParseTree* currentParseTreeGroup, int* performance, const bool subcomponents, const int subcomponentsType, const bool subcomponentsOptional, int layer, const string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
					private: bool generateRulesGroupTreeComponent(vector<XMLparserTag*>* SANIrulesTokenLayers, SANIComponentNeuralNetwork* component, vector<LRPpreprocessorPlainTextWord*>* sentenceContentsSubset, SANIComponentParseTree* currentParseTreeComponent, int* performance, int layer, const string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		private: bool forwardNounVerbVariantRequirementsComponentToGroup(SANIComponentParseTree* component, SANIGroupParseTree* newParseGroup);
		private: bool forwardNounVerbVariantRequirementsGroupToComponent(SANIGroupParseTree* currentParseGroup, SANIComponentParseTree* currentComponent);
		#endif
		private: bool findStringMatch(vector<XMLparserTag*>* SANIrulesTokenLayers, SANIComponentNeuralNetwork* component, LRPpreprocessorPlainTextWord* currentWord, SANIComponentParseTree* currentParseTreeComponent);
		private: bool verifyPOStype(const LRPpreprocessorPlainTextWord* currentWord, const uchar wordPOStype);
		public: bool updatePerformance(int performanceTemp, int* performance, SANIGroupParseTree* currentParseTreeGroup, SANIGroupParseTree* currentParseTreeGroupTemp, const bool passedTemp, int* minIndexOfMatchesFoundBackupOptimum, vector<LRPpreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup, SANIComponentParseTree* previousParseTreeComponent);
			private: bool deleteAllSubgroupsRecurse(const SANIGroupParseTree* currentParseTreeGroup, int layer);
			private: bool deleteParseComponent(const SANIComponentParseTree* currentParseTreeComponent);
		public: void clearAllWordsAlreadyFoundMatchInComponent(vector<LRPpreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup);
		public: void restoreAllWordsAlreadyFoundMatchInComponent(vector<LRPpreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundNew);

};
#endif
#endif



#endif
