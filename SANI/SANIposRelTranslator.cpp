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
 * File Name: SANIposRelTranslator.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p7a 24-March-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Part-of-speech Relation Translator
 * /
 *******************************************************************************/


#include "SANIposRelTranslator.hpp"


	
	
bool SANIposRelTranslatorClass::parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(SANItranslatorVariablesClass* translatorVariables)
{
	bool result = true;

	LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList = NULL;


	#ifdef LRP_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	if(!LRPpreprocessorWordIdentification.createWordIndexListFromLRPfiles())
	{
		result = false;
	}
	#endif

	vector<SANIGroupType*>* SANIGroupTypes = new vector<SANIGroupType*>;
	vector<XMLparserTag*>* SANIrulesTokenLayers = new vector<XMLparserTag*>;
	if(!SANIrules.extractSANIrules(SANIGroupTypes, SANIrulesTokenLayers))
	{
		result = false;
	}
	
	//cout << "SANIGroupTypes->size() = " << SANIGroupTypes->size() << endl;
	
	#ifdef SANI_FORWARD
	if(!SANIformation.createSANI(SANIrulesTokenLayers, SANIGroupTypes))
	{
		result = false;
	}
	#endif

	
	#ifdef LRP_DEFINE_WORD_TRANSLATOR_SENTENCE_ENTITY_INDEX_AT_START
	currentLRPpreprocessorSentenceInList = translatorVariables->LRPpreprocessorTranslatorVariables.firstLRPpreprocessorSentenceInList;
	while(currentLRPpreprocessorSentenceInList->next != NULL)
	{
		vector<LRPpreprocessorPlainTextWord*>* sentenceContents = LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList);
		for(int w=0; w<sentenceContents->size(); w++)
		{	
			LRPpreprocessorPlainTextWord* currentWord = sentenceContents->at(w);
			currentWord->translatorSentenceEntityIndex = LRPpreprocessorWordClassObject.convertSentenceContentsIndexToEntityIndex(w);
			#ifdef SANI_FORWARD
			currentWord->translatorSentenceWordIndex = w;
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_DETERMINE_POS_AMIGUITY_INFO_AT_START
			LRPpreprocessorPOStagger.recordPOSambiguityInfo(currentWord);
			#endif
		}
		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
	}
	#endif	
	
	#ifdef SANI_PARSE_SIMULTANEOUS
	/*
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	translatorVariables->parserDemarkateOptimumPathway = true;	//note actual demarkateOptimumPathwayBackprop isnt required to be executed (it is done by GIAtranslatorClass::convertSentenceRelationsIntoGIAnetworkNodesWrapper for the given sentence), but everything else is (ie code require to extract 
	if(!GIAposRelTranslatorPermutations.executePosRelTranslatorWrapper(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes))
	{
		result = false;
	}
	translatorVariables->parserDemarkateOptimumPathway = false;
	#endif
	*/
	#else
	if(!SANIposRelTranslatorPermutations.executePosRelTranslatorWrapper(translatorVariables, SANIrulesTokenLayers, SANIGroupTypes))
	{
		result = false;
	}
	#endif
	
	
	#ifdef SANI_SEQUENCE_GRAMMAR
	#ifdef SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	//cout << "createANNconnectivity" << endl;
	SANIformation.createANNconnectivity(SANIGroupTypes);
	#endif
	#endif

	return result;
}


