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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1m3b 01-August-2020
 * Requirements: requires plain text file
 * Description: SANI (Sequentially Activated Neuronal Input neural network) Inverse
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_INVERSE
#define HEADER_SANI_PROPAGATE_INVERSE

#include "GIAglobalDefs.hpp"

#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
#ifdef GIA_POS_REL_TRANSLATOR_INVERSE_NEURAL_NETWORK

#include "GIAtranslatorOperations.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperations.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "SHAREDvars.hpp"

class SANIpropagateInverseClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;	//required for printComponent/printParseTreeDebugIndentation
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: SHAREDvarsClass SHAREDvars;

	public: bool generateParseTreeIntro(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets);
		private: bool generateParseTreeGroupType(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, SANIGroupType* groupType, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, SANIGroupParseTree* currentParseTreeGroup, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType, SANIComponentParseTree* previousParseTreeComponent);
			private: bool generateParseTreeGroup(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, SANIGroupNeuralNetwork* group, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, SANIGroupParseTree* currentParseTreeGroup, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
				private: bool generateRulesGroupTreeComponents(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, vector<SANIComponentNeuralNetwork*>* components, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, SANIGroupParseTree* currentParseTreeGroup, int* performance, bool subcomponents, int subcomponentsType, bool subcomponentsOptional, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
					private: bool generateRulesGroupTreeComponent(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, SANIComponentNeuralNetwork* component, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, SANIComponentParseTree* currentParseTreeComponent, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType);
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		private: bool forwardNounVerbVariantRequirementsComponentToGroup(SANIComponentParseTree* component, SANIGroupParseTree* newParseGroup);
		private: bool forwardNounVerbVariantRequirementsGroupToComponent(SANIGroupParseTree* currentParseGroup, SANIComponentParseTree* currentComponent);
		#endif
		private: bool findStringMatch(vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers, SANIComponentNeuralNetwork* component, GIApreprocessorPlainTextWord* currentWord, SANIComponentParseTree* currentParseTreeComponent);
		private: bool verifyPOStype(GIApreprocessorPlainTextWord* currentWord, unsigned char wordPOStype);
		public: bool updatePerformance(const int performanceTemp, int* performance, SANIGroupParseTree* currentParseTreeGroup, SANIGroupParseTree* currentParseTreeGroupTemp, const bool passedTemp, int* minIndexOfMatchesFoundBackupOptimum, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup, SANIComponentParseTree* previousParseTreeComponent);
			private: bool deleteAllSubgroupsRecurse(SANIGroupParseTree* currentParseTreeGroup, int layer);
			private: bool deleteParseComponent(SANIComponentParseTree* currentParseTreeComponent);
		public: void clearAllWordsAlreadyFoundMatchInComponent(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup);
		public: void restoreAllWordsAlreadyFoundMatchInComponent(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundNew);
	private: void defineSubstancesBasedOnNetworkAndDeterminerInfo(GIAtranslatorVariablesClass* translatorVariables);
	private: int getEntityArrayMaxIndex(GIAtranslatorVariablesClass* translatorVariables);

	private: bool printParseTreeDebugIndentation(int layer);

};
#endif
#endif



#endif
