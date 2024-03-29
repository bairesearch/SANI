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
 * File Name: SANIpropagateHeavyUnoptimised.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Heavy Unoptimised - ~O(n^2)
 * /
 *******************************************************************************/


#include "SANIpropagateHeavyUnoptimised.hpp"
#include "SHAREDvars.hpp"

#ifdef SANI_HEAVY
#ifdef SANI_HEAVY_UNOPTIMISED


bool SANIpropagateHeavyUnoptimisedClass::executePosRelTranslatorNeuralNetwork(const SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;
				
	//sentence specific variables:
	SANIForwardPropogationSequenceData forwardPropogationSequenceData;
	SANIpropagateOperations.setParseSentenceReverse(true, &forwardPropogationSequenceData);
	forwardPropogationSequenceData.toplevelGroupActivationFound = false;
	//forwardPropogationSequenceData.performance = performance;
	/*//unsupported;
	#ifdef SANI_PARSE_SIMULTANEOUS    
	forwardPropogationSequenceData.parserEnabled = parserEnabled;
	#endif
	*/
	forwardPropogationSequenceData.sentenceContents = sentenceContents;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	forwardPropogationSequenceData.isQuery = SANInodesGroupClassObject.determineIsQuery(sentenceContents);
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	forwardPropogationSequenceData.parseIsolatedSubreferenceSets = parseIsolatedSubreferenceSets;
	#endif
	forwardPropogationSequenceData.SANIGroupTypes = SANIGroupTypes;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = new SANIForwardPropogationSequenceElementData();
		forwardPropogationSequenceData.forwardPropogationSequenceElementDataArray.push_back(forwardPropogationSequenceElementData);
	}
	forwardPropogationSequenceData.activationPathSequenceElementFirstParseTreeGroupArray.resize(sentenceContents->size());

	#ifdef SANI_PARSE_SAVE_PARSE_TREE
	forwardPropogationSequenceData.topLevelParseTreeGroupPropagate = NULL;
	#endif
	
	SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of SANIpropagateHeavyUnoptimisedClass::executePosRelTranslatorNeuralNetwork)
	
	SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup = NULL;
		
	if(sentenceContents->size() > 0)
	{
		int w = 0;
		bool expectToSeeConnectionWithPreviousWordTrace = false;
		bool passThrough = false;
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSequenceData, 0, activationPathSequenceElementFirstParseTreeGroup, expectToSeeConnectionWithPreviousWordTrace, passThrough))
		{
			result = false;
		}
	}
	else
	{
		cerr << "SANIpropagateHeavyUnoptimisedClass::executePosRelTranslatorNeuralNetwork{} error: sentenceContents->size() == 0" << endl;
		exit(EXIT_ERROR);		
	}
	
	if(forwardPropogationSequenceData.toplevelGroupActivationFound)
	{
		sentenceValidActivationPath = true;
		result = true;
		
		#ifdef SANI_PARSE_SAVE_PARSE_TREE
		*topLevelParseTreeGroup = forwardPropogationSequenceData.topLevelParseTreeGroupPropagate;
		//cout << "forwardPropogationSequenceData->topLevelParseTreeGroupPropagate" << endl;
		
		//#ifdef SANI_DEBUG_PROPAGATE
		cout << "topLevelGroup" << endl;
		printBackpropParseTree(forwardPropogationSequenceData.topLevelParseTreeGroupPropagate, 1);
		cout << "end printBackpropParseTree" << endl;
		//cout << "forwardPropogationSequenceData->topLevelParseTreeGroupPropagate->groupName = " << forwardPropogationSequenceData->topLevelParseTreeGroupPropagate->groupName << endl;
		//#endif	
		#endif	
		
		#ifdef SANI_PARSE_PERFORMANCE
		*performance = forwardPropogationSequenceData.performance;
		//cout << "forwardPropogationSequenceData.performance = " << forwardPropogationSequenceData.performance << endl;
		#else
		*performance = 1;
		#endif
		
		/*
		cout << "DEBUG: finished executing SANIpropagateHeavyUnoptimisedClass::executePosRelTranslatorNeuralNetwork (exiting)" << endl;
		exit(EXIT_ERROR);
		*/
	}
	else
	{
		*performance = 0;		
	}
	
	#ifdef SANI_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(sentenceValidActivationPath)
	{
		SANIpropagateOperationsParseTree.traceBackpropParseTreeSetTraced(forwardPropogationSequenceData.topLevelParseTreeGroupPropagate, 1);	//added GIA3g6aTEMP32 - set all parseTreeGroup groups in final heirachy to neuronTraced to prevent their memory from being deleted during SANIpropagateOperations.resetAllNeuronComponents
		SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);
		SANIpropagateOperationsParseTree.resetNeuronBackprop(forwardPropogationSequenceData.topLevelParseTreeGroupPropagate, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);	//added GIA3g6aTEMP32 
		
		for(int w=0; w<sentenceContents->size(); w++)
		{
			SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = (forwardPropogationSequenceData.forwardPropogationSequenceElementDataArray[w]);
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
			cout << "w = " << w << ", forwardPropogationSequenceElementData->foundPreviousActiveWord = " << forwardPropogationSequenceElementData->foundPreviousActiveWord << endl;
			#endif
		}
	}
	else
	{
		SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);		
	}
	#endif
	
	//TODO: resetSemanticRelationReturnEntityForwardPropogationSignalDataPackage(sentenceContents)
	
	/*
	#ifdef SANI_DEBUG_PROPAGATE
	cout << "DEBUG: finished executing SANIpropagateHeavyUnoptimisedClass::executePosRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	return result;
}

	


bool SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro(const SANItranslatorVariablesClass* translatorVariables, int w, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup, const bool expectToSeeConnectionWithPreviousWordTrace, const bool passThrough)
{
	bool result = false;

	
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	SANIpropagateOperations.resetAllNeuronComponents(forwardPropogationSequenceData->SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE);
	SANIpropagateOperations.resetAllNeuronComponents(forwardPropogationSequenceData->SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED);
	#endif
	
	LRPpreprocessorPlainTextWord* currentWord = forwardPropogationSequenceData->sentenceContents->at(w);

	SANInodes.printParseTreeDebugIndentation(layer);
	#ifdef SANI_DEBUG_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
	#ifndef SANI_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "currentWord->POSambiguityInfo = " << LRPpreprocessorPOStagger.printPOSambiguityInfo(currentWord->POSambiguityInfo) << endl;
	#endif

	//sequenceElement specific variables:
	SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = (forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray[w]);
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC	
	forwardPropogationSequenceElementData->previousSequenceElementConnections.clear();
	#endif
	forwardPropogationSequenceElementData->wordReference = currentWord;
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	//forwardPropogationSequenceElementData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
	#endif
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC
	forwardPropogationSequenceElementData->expectToSeeConnectionWithPreviousWordTrace = expectToSeeConnectionWithPreviousWordTrace;
	#endif
	
	forwardPropogationSequenceElementData->sequenceIndex = w;
	//cout << "forwardPropogationSequenceElementData->previousSequenceElementConnections.size() = " << forwardPropogationSequenceElementData->previousSequenceElementConnections.size() << endl;
	
	//group specific variables:
	SANIForwardPropogationSignalData forwardPropogationSignalData;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	forwardPropogationSignalData.wordNounVariantType = currentWord->wordNounVariantGrammaticalTenseForm;
	forwardPropogationSignalData.wordVerbVariantType = currentWord->wordVerbVariantGrammaticalTenseForm;
	#endif
				
	string sequenceElementLowerCase = SHAREDvars.convertStringToLowerCase(&(currentWord->tagName));
		
	
	if(!SANInodes.currentWordPOSunknown(currentWord))
	{
		#ifdef SANI_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		int wordPOStype = currentWord->unambiguousPOSindex;
		#else
		for(int wordPOStype=0; wordPOStype<LRP_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		{
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, wordPOStype))
			{
		#endif
				if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))
				{
					result = true;
				}
				
		#ifdef SANI_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		#else
			}
		}
		#endif
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
	else
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_WILDCARDS
		for(int wordPOStype=0; wordPOStype<LRP_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		{
			if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))
			{
				result = true;
			}
		}
		#else
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
		int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_NOUN;
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))
		{
			result = true;
		}	
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
		if(LRPpreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
		{	
			int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT;
			if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))
			{
				result = true;
			}
		}
		#endif
		#endif	
	}
	#endif
	
			
	
	
	SANIGroupNeuralNetwork* inputLayerGroup = NULL;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_STRING_EXPLICIT_CASE_INSENSITIVE
	if(SANIformation.findWordInGroupMap(sequenceElementLowerCase, SANIformation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationSequenceElementData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef SANI_DEBUG_PROPAGATE
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))
		{
			result = true;
		}
	}
	#else
	if(SANIformation.findWordInGroupMap(sequenceElementLowerCase, SANIformation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationSequenceElementData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef SANI_DEBUG_PROPAGATE
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))
		{
			result = true;
		}
	}
	else if(SANIformation.findWordInGroupMap(currentWord->tagName, SANIformation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationSequenceElementData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef SANI_DEBUG_PROPAGATE
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))
		{
			result = true;
		}
	}
	#endif
		
	for(unordered_map<string,SANIGroupNeuralNetwork*>::iterator iter1 = SANIformation.getInputLayerSectionTokensLayerMap()->begin(); iter1 != SANIformation.getInputLayerSectionTokensLayerMap()->end(); iter1++)
	{
		SANIGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayer = iter1->second;
		for(int i=0; i<currentGroupInInputLayerSectionTokensLayer->SANIbackGroupConnectionList.size(); i++)
		{
			SANIGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClass = (currentGroupInInputLayerSectionTokensLayer->SANIbackGroupConnectionList)[i];
			for(int i=0; i<currentGroupInInputLayerSectionTokensLayerClass->SANIbackGroupConnectionList.size(); i++)
			{
				SANIGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassType = (currentGroupInInputLayerSectionTokensLayerClass->SANIbackGroupConnectionList)[i];
				for(int i=0; i<currentGroupInInputLayerSectionTokensLayerClassType->SANIbackGroupConnectionList.size(); i++)
				{
					SANIGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassTypeInstance = (currentGroupInInputLayerSectionTokensLayerClassType->SANIbackGroupConnectionList)[i];
					if(currentGroupInInputLayerSectionTokensLayerClassTypeInstance->GIAtokenLayerClassTypeInstanceName == sequenceElementLowerCase)	//NB this implementation is synced with SANIrulesClass::isClassTag: assume tokens always comprise sequenceElementLowerCase
					{
						string GIAtokenLayerClassName = currentGroupInInputLayerSectionTokensLayerClass->GIAtokenLayerClassName;
						int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;
						if(SHAREDvars.textInTextArray(GIAtokenLayerClassName, LRPpreprocessorPOStypeNameArray, LRP_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES, &wordPOStype))
						{
							forwardPropogationSequenceElementData->wordPOStype = wordPOStype;
						}
						else
						{
							cerr << "SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
							exit(EXIT_ERROR);
						}
							
						#ifdef SANI_DEBUG_PROPAGATE
						SANInodes.printParseTreeDebugIndentation(layer);
						cout << "SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
						#endif
						if(propagateWordThroughNetworkGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClassType, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))
						{
							result = true;
						}
						if(propagateWordThroughNetworkGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClass, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))	//what is this for?
						{
							result = true;
						}
					}
				}
			}
		}
	}

	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	SANIpropagateOperations.resetAllNeuronComponents(forwardPropogationSequenceData->SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET);
	#endif
		
	return result;
}

	
bool SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupInit(const SANItranslatorVariablesClass* translatorVariables, int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	SANIGroupNeuralNetwork* inputLayerGroup = SANIformation.getInputGroupLayerSection(SANIformation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef SANI_DEBUG_PROPAGATE
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationSequenceElementData->wordPOStype = wordPOStype;
	#endif
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC_CONSECUTIVE_WORDS_SAME_POSTYPE
	if(w > 0)
	{
		int currentWordPosType = forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray[w]->wordPOStype;
		int previousWordPosType = forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray[w-1]->wordPOStype;
		if((currentWordPosType == previousWordPosType) && (currentWordPosType != LRP_PREPROCESSOR_POS_TYPE_UNDEFINED))
		{
			forwardPropogationSequenceElementData->expectToSeeConnectionWithPreviousWordTrace = true;
		}
	}
	#endif
	
	if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))
	{
		result = true;
	}
	
	return result;
}	
	
bool SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupInit(const SANItranslatorVariablesClass* translatorVariables, int w, SANIGroupNeuralNetwork* inputLayerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	inputLayerGroup->neuronPropagated = false;
	
	forwardPropogationSequenceData->activationPathSequenceElementFirstParseTreeGroupArray[w] = SANInodes.convertNeuralNetworkGroupToParseTreeGroupNew(inputLayerGroup);	//new SANIGroupParseTree(*SANIpropagateOperations.convertNeuralNetworkGroupToParseTreeGroup(inputLayerGroup));	//CHECKTHIS
	activationPathSequenceElementFirstParseTreeGroup = forwardPropogationSequenceData->activationPathSequenceElementFirstParseTreeGroupArray[w];
	if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))
	{
		result = true;
	}
	else
	{
		delete forwardPropogationSequenceData->activationPathSequenceElementFirstParseTreeGroupArray[w];
	}
	
	return result;
}				
				

bool SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupIntro(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	if(propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup, passThrough))
	{
		result = true;
	}
	
	return result;
}

bool SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroup(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_PROPAGATE
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
	if(!passThrough)
	{
	#endif
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
	}
	#endif
	#endif
	/*
	if(passThrough)
	{
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;		
	}
	*/
			
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops
	{
		group->neuronPropagated = true;
	#endif
		
		//vector<GIAactivationPathMostRecentContribution> frontLayerActivationPathMostRecentContribution;
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup, passThrough))
		{
			result = true;
		}
		
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	else
	{
		cout << "FAIL: neuronPropagated" << endl;
		//result = true;
	}
	#endif
	
	return result;
}
		
bool SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupSelect(const SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const bool passThrough)
{
	bool result = false;
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY
	//int mostRecentContributionSequenceIndex = INT_DEFAULT_VALUE;	//not required with current implementation (removeLessRecentContributionIndices takes care of it)
	#endif
	 
	for(int i=0; i<group->SANIfrontComponentConnectionList.size(); i++)
	{
		if(!(forwardPropogationSequenceData->finishedPassingSequenceElements))
		{
			SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i];
			SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	
			SANIComponentNeuralNetwork* ownerComponent = currentComponent;
			int componentIndex = currentComponent->componentIndex;	
			int ownerComponentIndex = componentIndex;
			if(currentComponent->isSubcomponent)
			{
				ownerComponent = currentComponent->ownerComponent;
				ownerComponentIndex = ownerComponent->componentIndex;
			}
			SANIComponentNeuralNetwork* ownerComponentOrig = ownerComponent;
			SANIComponentNeuralNetwork* currentComponentOrig = currentComponent;
			
			#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			if(!ownerGroup->neuronPropagated)	//prevent circular loops
			{
			#endif	
				SANIGroupNeuralNetwork* ownerGroupOrig = ownerGroup;

				#ifdef SANI_DEBUG_PROPAGATE_EXTRA2
				SANInodes.printParseTreeDebugIndentation(layer+1);
				cout << "2a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
				//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
				SANInodes.printComponent(currentComponent, layer+1);
				#endif

				#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				if(SANIpropagateOperations.componentTests1(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData))	//static component tests (component activations irrelevant)
				{
				#endif
				
					#ifdef SANI_SUPPORT_RECURSION
					//this is required to support SANIrules.xml recursive groups, e.g. logicReferenceSets: 
					int activationMemoryGroupArraySizeOrig = ownerGroupOrig->activationMemoryGroupArray.size();
					int currentParseTreeGroupArraySizeOrig = ownerGroupOrig->currentParseTreeGroupArray.size();
					int index = 0;
					for(int activationMemoryGroupArrayIndex=ownerGroupOrig->activationMemoryGroupArray.size()-1; activationMemoryGroupArrayIndex>=0; activationMemoryGroupArrayIndex--)
					{
						index++;
					#endif	
						int activationMemoryGroupArraySize = ownerGroupOrig->activationMemoryGroupArray.size();
						
						#ifdef SANI_PARSE
						#ifdef SANI_SUPPORT_RECURSION
						//cout << "activationMemoryGroupArrayIndex = " << activationMemoryGroupArrayIndex << endl;
						//cout << "ownerGroup->currentParseTreeGroupArray.size() = " << ownerGroupOrig->currentParseTreeGroupArray.size() << endl;
						SANIGroupParseTree* currentParseTreeGroup = ownerGroupOrig->currentParseTreeGroupArray[activationMemoryGroupArrayIndex];
						#else
						SANIGroupParseTree* currentParseTreeGroup = ownerGroupOrig->currentParseTreeGroupArray.back();
						#endif
						#endif
						#ifdef SANI_SAVE_MEMORY_GROUPS
						#ifdef SANI_SUPPORT_RECURSION
						SANIGroupActivationMemory* activationMemoryGroup = ownerGroupOrig->activationMemoryGroupArray[activationMemoryGroupArrayIndex];
						#else
						SANIGroupActivationMemory* activationMemoryGroup = ownerGroupOrig->activationMemoryGroupArray.back();
						#endif
						#endif
						
						/*
						cout << "activationMemoryGroup->components.size() = " << activationMemoryGroup->components.size() << endl;
						cout << "activationMemoryGroup->lastSequenceIndexActivated = " << activationMemoryGroup->lastSequenceIndexActivated << endl;
						cout << "forwardPropogationSequenceElementData->sequenceIndex = " << forwardPropogationSequenceElementData->sequenceIndex << endl;
						*/
						
						#ifdef SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
						if(activationMemoryGroup->lastSequenceIndexActivated != forwardPropogationSequenceElementData->sequenceIndex)
						{
						#endif
							#ifdef SANI_SOLIDIFY_NET_BACKPROP
							if(!(currentParseTreeGroup->solidified))	//this saves time by considering only recently activated groups
							{
							#endif
								#ifdef SANI_PARSE
								SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = NULL;
								#endif
								#ifdef SANI_SAVE_MEMORY_GROUPS
								SANIGroupActivationMemory* activationMemoryGroupNew = NULL;
								#ifdef SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
								int activationMemoryGroupLastSequenceIndexActivatedOrig = INT_DEFAULT_VALUE;
								#endif
								#endif
								#ifdef SANI_PARSE
								SANIGroupParseTree* currentParseTreeGroupNew = NULL;
								#endif
								
								SANIGroupActivationMemory* activationMemoryGroupTemp = NULL;
								SANIGroupParseTree* currentParseTreeGroupTemp = NULL;
								#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
								if(!passThrough)
								{
								#endif
									#ifdef SANI_SAVE_MEMORY_GROUPS
									activationMemoryGroupNew = SANInodes.copyGroup(activationMemoryGroup);
									ownerGroupOrig->activationMemoryGroupArray.push_back(activationMemoryGroupNew);
									#ifdef SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
									activationMemoryGroupLastSequenceIndexActivatedOrig = activationMemoryGroup->lastSequenceIndexActivated;
									activationMemoryGroup->lastSequenceIndexActivated = forwardPropogationSequenceElementData->sequenceIndex;
									//NOTREQUIRED: activationMemoryGroupNew->lastSequenceIndexActivated = forwardPropogationSequenceElementData->sequenceIndex; 
									#endif
									#endif
									#ifdef SANI_PARSE
									currentParseTreeGroupNew = SANInodes.copyGroup(currentParseTreeGroup);	//create a new parse tree group (in case of 1. first encounter with group or 2. recursion into different group)
									ownerGroupOrig->currentParseTreeGroupArray.push_back(currentParseTreeGroupNew);
									#endif				

									activationMemoryGroupTemp = activationMemoryGroupNew;
									currentParseTreeGroupTemp = currentParseTreeGroupNew;
									
									#ifdef SANI_SOLIDIFY_NET_BACKPROP
									if(activationMemoryGroupArrayIndex != 0)	//never solidify first group in array (non-activated template)
									{
										currentParseTreeGroup->solidified = true;
									}
									#endif

								#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
								}
								else
								{
									activationMemoryGroupTemp = activationMemoryGroup;
									currentParseTreeGroupTemp = currentParseTreeGroup;

									#ifdef SANI_SOLIDIFY_NET_BACKPROP
									currentParseTreeGroup->solidified = true;	//prevent recursion during passThrough
									#endif
								}
								#endif
								
								#ifdef SANI_SAVE_MEMORY_GROUPS		
								SANIComponentNeuralNetwork* activationMemoryGroupOwnerComponent = activationMemoryGroupTemp->components[ownerComponentIndex];
								SANIComponentNeuralNetwork* activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent;
								if(currentComponentOrig->isSubcomponent)
								{
									if(SANInodesComponentClassObject.componentHasSubcomponents(ownerComponentOrig))
									{
										activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent->subComponents[componentIndex];
									}
									else
									{
										cerr << "SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupSelect{} error: (currentComponent->isSubcomponent) && !(activationMemoryGroupOwnerComponent->hasSubComponents)" << endl;
										cout << "index = " << index << endl;
										cout << "ownerComponentIndex = " << ownerComponentIndex << endl;
										cout << "componentIndex = " << componentIndex << endl;
										SANInodes.printParseTreeDebugIndentation(layer+1);
										cout << "propagateWordThroughNetworkGroup: " <<  group->groupTypeName << ":" << group->groupName << endl;	
										SANInodes.printParseTreeDebugIndentation(layer+1);
										cout << "propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;
										cout << "currentComponent->componentType = " << currentComponent->componentType << endl;
										cout << "ownerComponent->componentType = " << ownerComponent->componentType << endl;
										SANInodes.printComponent(currentComponent, layer+1);	
										SANInodes.printComponent(ownerComponent, layer+1);
										exit(EXIT_ERROR);
									}
								}
								ownerGroup = activationMemoryGroupTemp;
								currentComponent = activationMemoryGroupCurrentComponent;
								ownerComponent = activationMemoryGroupOwnerComponent;
								#endif
								#ifdef SANI_PARSE
								activationPathWordCurrentParseTreeGroupOwner = currentParseTreeGroupTemp;	//use at top of parseTreeGroup at top of stack
								#endif
									
													
								if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
								{
									currentComponent->candidateStringMatch = forwardPropogationSequenceElementData->wordReference;
								}


								SANIposRelTranslatorDebug debug;
								#ifdef SANI_DEBUG_PROPAGATE_EXTRA4
								debug.activationMemoryGroupArrayIndex = activationMemoryGroupArrayIndex;
								debug.activationMemoryGroupArraySize = activationMemoryGroupArraySize;
								debug.firstComponentActive = ownerGroup->components[0]->neuronComponentConnectionActive;
								if(ownerGroup->components.size() >= 2)
								{
									debug.secondComponentActive = ownerGroup->components[1]->neuronComponentConnectionActive;
								}
								#endif



								#ifdef SANI_DEBUG_PROPAGATE_EXTRA2
								SANInodes.printParseTreeDebugIndentation(layer+1);
								cout << "2b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
								//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
								SANInodes.printComponent(currentComponent, layer+1);
								#endif

								//int currentMemoryGroupArrayNewlyCreatedIndex = ownerGroup->currentParseTreeGroupArray.size()-1;	//CHECKTHIS; is this required for groupArray deletion?


								if(currentComponent->semanticRelationReturnEntity)
								{
									activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
								}

								int indexOfFirstComponentInGroup = 0;
								int indexOfLastComponentInGroup = ownerGroup->components.size()-1;
								SANIComponentNeuralNetwork* firstComponent = (ownerGroup->components)[indexOfFirstComponentInGroup];
								SANIComponentNeuralNetwork* lastComponent = (ownerGroup->components)[indexOfLastComponentInGroup];

								#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS
								bool lastComponentFuzzyAndCompatible = false;
								#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
								bool lastComponentOptionalAndInactiveAndCompatible = false;
								bool lastComponentRepeatAndActivatedByPreviousWordAndCompatible = false;
								if(SANIpropagateOperations.isLastComponentFuzzy(ownerGroup))
								{
									if(lastComponent == ownerComponent)
									{
										if(lastComponent->neuronComponentConnectionActive)
										{
											if(lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
											{
												if(lastComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex == (forwardPropogationSequenceElementData->sequenceIndex - 1))
												{
													lastComponentRepeatAndActivatedByPreviousWordAndCompatible = true;
													lastComponentFuzzyAndCompatible = true;
													//cout << "lastComponentRepeatAndActivatedByPreviousWord" << endl;
												}
											}
										}
										else
										{
											if(lastComponent->optional)
											{
												lastComponentOptionalAndInactiveAndCompatible = true;
												lastComponentFuzzyAndCompatible = true;
												//cout << "lastComponentFuzzyAndCompatible" << endl;
											}
										}
									}
									
								}
								#endif
								#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
								if(SANIpropagateOperations.isLastComponentFuzzy(ownerGroup))
								{
									if(lastComponent == ownerComponent)
									{
										if(lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
										{
											lastComponentFuzzyAndCompatible = true;
										}
									}
									else 
									{
										lastComponentFuzzyAndCompatible = true;
									}
								}
								#endif
								#else
								bool lastComponentFuzzyAndCompatible = false;
								#endif


								#ifndef SANI_SUPPORT_RECURSION
								/*
								if(groupActivationCompleted(ownerGroup))
								{
									//cout << "groupActivationCompleted(ownerGroup)" << endl;
								}
								if(groupActivationCompleted(ownerGroup) && !lastComponentFuzzyAndCompatible)
								{
									//cout << "SANIpropagateOperations.resetGroupActivation: ownerGroup = " << ownerGroup->groupName << endl;
									SANIpropagateOperations.resetGroupActivation(ownerGroup);
									currentParseTreeGroupNew->components.clear();	//reset parseTreeGroup
								}
								*/
								
								//changed 3g5a (introduce more simple reset detection method);
								bool requireResetGroupActivation = false;
								if(firstComponent == ownerComponent)
								{
									if(SANIpropagateOperations.doesRequireResetGroupActivation(forwardPropogationSequenceData, firstComponent, indexOfFirstComponentInGroup, ownerGroup, forwardPropogationSequenceElementData))
									{
										requireResetGroupActivation = true;
									}
								}
								else if(ownerGroup->components.size() >= 1)
								{
									int indexOfSecondComponentInGroup = 1;
									SANIComponentNeuralNetwork* secondComponent = (ownerGroup->components)[indexOfSecondComponentInGroup];

									if((secondComponent == ownerComponent) && (firstComponent->optional))
									{
										if(SANIpropagateOperations.doesRequireResetGroupActivation(forwardPropogationSequenceData, secondComponent, indexOfSecondComponentInGroup, ownerGroup, forwardPropogationSequenceElementData))
										{
											requireResetGroupActivation = true;
										}
									}
								}

								if(requireResetGroupActivation)
								{
									#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
									cout << "requireResetGroupActivation" << endl;
									#endif
									SANIpropagateOperations.resetGroupActivation(ownerGroup);
									currentParseTreeGroupNew->components.clear();	//reset parseTreeGroup
								}
								#endif


								bool deleteNewlyCreatedMemoryGroup = false;

								if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough, &debug))
								{
									result = true;
								}
								else
								{
									deleteNewlyCreatedMemoryGroup = true;
								}



								#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
								if(!passThrough)
								{
								#endif
									if(deleteNewlyCreatedMemoryGroup)
									{
										//cout << "deleteNewlyCreatedMemoryGroup" << endl;

										/*
										if(ownerGroupOrig->currentParseTreeGroupArray.size() != sizeOrig+1)
										{
											cerr << "(ownerGroupOrig->currentParseTreeGroupArray.size() != sizeOrig+1)" << endl;
											cout << "currentParseTreeGroupArraySizeOrig = " << currentParseTreeGroupArraySizeOrig << endl;
											cout << "activationMemoryGroupArraySizeOrig = " << activationMemoryGroupArraySizeOrig << endl;
											cout << "ownerGroupOrig->currentParseTreeGroupArray.size() = " << ownerGroupOrig->currentParseTreeGroupArray.size() << endl;
											cout << "ownerGroupOrig->activationMemoryGroupArray.size() = " << ownerGroupOrig->activationMemoryGroupArray.size() << endl;
											exit(EXIT_ERROR);
										}
										*/

										#ifdef SANI_PARSE
										SANInodes.deleteGroup(currentParseTreeGroupNew);
										ownerGroupOrig->currentParseTreeGroupArray.pop_back();
										#endif
										#ifdef SANI_SAVE_MEMORY_GROUPS
										SANInodes.deleteGroup(activationMemoryGroupNew);
										ownerGroupOrig->activationMemoryGroupArray.pop_back();
										#endif
									}

									#ifdef SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
									activationMemoryGroup->lastSequenceIndexActivated = activationMemoryGroupLastSequenceIndexActivatedOrig;
									#endif
								#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
								}
								#endif

							#ifdef SANI_SOLIDIFY_NET_BACKPROP
								#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
								if(!passThrough)
								{
								#endif
									if(activationMemoryGroupArrayIndex != 0)
									{
										currentParseTreeGroup->solidified = false;
									}
								#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
								}
								else
								{
									currentParseTreeGroup->solidified = false;
								}
								#endif
							}
							#endif

						#ifdef SANI_SAVE_MEMORY_GROUPS_PREVENT_CIRCULAR_CONNECTION_LOOPS
						}
						#endif

					#ifdef SANI_SUPPORT_RECURSION	
					}
					#endif
				#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				}
				#endif
			#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			}
			#endif	
		}
	}
	
	return result;
}	
	


bool SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(const SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, const SANIposRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	SANInodes.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC	
	vector<SANIForwardPropogationSequenceElementData*> previousWordConnectionsOrig = forwardPropogationSequenceElementData->previousSequenceElementConnections;
	#endif
	
	SANIComponentNeuralNetwork* previousActiveComponent = NULL;	//NOTUSED
	SANIComponentNeuralNetwork* finalActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;
	if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, ownerComponent, &(ownerGroup->components), &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &finalActiveComponent))
	{	
		bool subComponentsPassed = true;
		bool repeatDetected = false;
		if(currentComponent->isSubcomponent)
		{
			subComponentsPassed = false;
			if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
			{
				if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(forwardPropogationSequenceElementData, forwardPropogationSequenceData, currentComponent, &(ownerComponent->subComponents)))
				{
					subComponentsPassed = true;
					#ifdef SANI_DEBUG_PROPAGATE
					//cout << "SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(forwardPropogationSequenceElementData, forwardPropogationSequenceData, currentComponent, &(ownerComponent->subComponents), &repeatDetected))
				{
					subComponentsPassed = true;
					#ifdef SANI_DEBUG_PROPAGATE
					//cout << "SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
					#endif
				}
			}
			else
			{
				subComponentsPassed = true;
			}
		}
		if(subComponentsPassed)
		{
			sequentialActivationFound = true;
			
			#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
			if(!firstActiveComponentInGroup || SANIpropagateOperations.componentTests2(ownerGroupOrig, activationPathSequenceElementCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData))
			{
			#endif
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
				if(SANIpropagateOperations.componentWordConnectivityTestsWrapper(forwardPropogationSequenceElementData, forwardPropogationSequenceData, activationPathWordCurrentParseTreeGroupOwner, activationPathSequenceElementCurrentParseTreeGroup))
				{
				#endif
					if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, activationSequenceCompleted, layer, repeatDetected, activationPathSequenceElementCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough, debug))
					{
						result = true;
					}
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
				}
				#endif
			#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
			}
			#endif
		}
	}
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC	
	if(!result)
	{
		forwardPropogationSequenceElementData->previousSequenceElementConnections = previousWordConnectionsOrig;
	}
	#endif
	
	return result;
}
	
	
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponent(const SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, const SANIGroupNeuralNetwork* ownerGroup, SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, const SANIposRelTranslatorDebug* debug)
{
	bool result = false;
	
	SANIForwardPropogationSequenceElementData* forwardPropogationWordDataPreviousRecord = NULL;
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() > 0)
	{
		SANIComponentParseTree* previousActiveComponent = activationPathWordCurrentParseTreeGroupOwner->components[activationPathWordCurrentParseTreeGroupOwner->components.size()-1];
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC
		int wPrevious2 = forwardPropogationSequenceElementData->sequenceIndex - 1;
		if(wPrevious2 >= 0)	//redundant
		{
			if(previousActiveComponent->neuronComponentConnectionActiveSequenceElementRecord->wordReference == (*(forwardPropogationSequenceData->sentenceContents))[wPrevious2])
			{
				forwardPropogationSignalData->foundPreviousActiveWord = true;
				#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
				cout << "foundPreviousActiveWord" << endl;
				#endif
			}
		}
		else
		{
			cerr << "wPrevious2 = " << wPrevious2 << endl;
			exit(EXIT_ERROR);
		}
		#endif
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
		if(!passThrough)
		{
		#endif
			int wPrevious = previousActiveComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
			if(wPrevious >= 0)	//redundant
			{
				SANIForwardPropogationSequenceElementData* forwardPropogationWordDataPrevious = ((forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray)[wPrevious]);
				#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
				cout << "foundPreviousActiveWord, forwardPropogationWordDataPrevious->w = " << forwardPropogationWordDataPrevious->w << endl;
				#endif
				//cout << "forwardPropogationWordDataPrevious->w = " << forwardPropogationWordDataPrevious->w << endl;
				//cout << "forwardPropogationSequenceElementData->previousSequenceElementConnections.size() = " << forwardPropogationSequenceElementData->previousSequenceElementConnections.size() << endl;
				#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
				if(lastComponentFuzzyAndCompatible)
				{
					*forwardPropogationWordDataPreviousRecord = forwardPropogationWordDataPrevious;
					/*
					if(find(forwardPropogationWordDataPrevious->previousSequenceElementConnections.begin(), forwardPropogationWordDataPrevious->previousSequenceElementConnections.end(), forwardPropogationSequenceElementData) == forwardPropogationWordDataPrevious->previousSequenceElementConnections.end())
					{
						forwardPropogationWordDataPrevious->previousSequenceElementConnections.push_back(forwardPropogationSequenceElementData);
					}	
					*/
				}
				else
				{
				#endif
					if(find(forwardPropogationSequenceElementData->previousSequenceElementConnections.begin(), forwardPropogationSequenceElementData->previousSequenceElementConnections.end(), forwardPropogationWordDataPrevious) == forwardPropogationSequenceElementData->previousSequenceElementConnections.end())
					{
						forwardPropogationSequenceElementData->previousSequenceElementConnections.push_back(forwardPropogationWordDataPrevious);
					}
				#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
				}
				#endif
			}
			else
			{
				cerr << "wPrevious = " << wPrevious << endl;
				exit(EXIT_ERROR);
			}
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
		}
		#endif
		#endif					
	}
	#endif
		
		
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
	if(!passThrough)
	{	
	#endif
		
		//cout << "forwardPropogationSequenceElementData->wordReference = " << forwardPropogationSequenceElementData->wordReference->tagName << endl;
		/*
		if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
		{
			cout << "(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)" << endl;
		}
		*/
		
		ownerComponent->neuronComponentConnectionActive = true;
		currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
		ownerComponent->neuronComponentConnectionActiveSequenceElementRecord = forwardPropogationSequenceElementData;
		currentComponent->neuronComponentConnectionActiveSequenceElementRecord = forwardPropogationSequenceElementData;

		/*
		#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC
		vector<SANIForwardPropogationSequenceElementData*> previousWordConnectionsOrig = forwardPropogationSequenceElementData->previousSequenceElementConnections;
		#endif
		#endif
		*/

		#ifdef SANI_PARSE
		//create new parseTreeGroup
		//copied from SANIpropagateInverseClass::generateRulesGroupTreeComponents;
		SANIComponentParseTree* newParseComponent = SANInodes.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new SANIComponentParseTree(*SANIpropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
		newParseComponent->componentRef = currentComponent;
		newParseComponent->neuronComponentConnectionActive = true;
		newParseComponent->neuronComponentConnectionActiveSequenceElementRecord = forwardPropogationSequenceElementData;
		newParseComponent->parseTreeGroupRef = activationPathSequenceElementCurrentParseTreeGroup;
		//currentWord->wordPOStypeInferred = forwardPropogationSequenceElementData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous sequenceElements in current parse tree	//this will be set later by GIAposRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
		newParseComponent->wordPOStypeInferred = forwardPropogationSequenceElementData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
		activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);

		#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		//store parseTreeGroupRefReverse/parseTreeGroupRefReverseComponentIndex orig (is this req?);
		vector<SANIGroupParseTree*> parseTreeGroupRefParseTreeGroupRefReverseOrig;
		vector<int> parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig;
		#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
		SANIGroupParseTree* activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseOrig = activationPathSequenceElementCurrentParseTreeGroup->parseTreeGroupRefReverse;
		int activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseComponentIndexOrig = activationPathSequenceElementCurrentParseTreeGroup->parseTreeGroupRefReverseComponentIndex;
		activationPathSequenceElementCurrentParseTreeGroup->parseTreeGroupRefReverse = activationPathWordCurrentParseTreeGroupOwner;
		activationPathSequenceElementCurrentParseTreeGroup->parseTreeGroupRefReverseComponentIndex = activationPathWordCurrentParseTreeGroupOwner->components.size()-1;
		if(activationSequenceCompleted)
		{
		#endif
			//update the parseTreeGroupRefReverse/parseTreeGroupRefReverseComponentIndex of every component->parseTreeGroupRef in activationPathWordCurrentParseTreeGroupOwner (with the latest activationPathWordCurrentParseTreeGroupOwner memory container):
				//note this will be reexecuted accordingly if lastComponentFuzzyAndCompatible detected:
			for(int i=0; i<activationPathWordCurrentParseTreeGroupOwner->components.size(); i++)
			{
				SANIComponentParseTree* component = activationPathWordCurrentParseTreeGroupOwner->components[i];
				SANIGroupParseTree* parseTreeGroupRef = component->parseTreeGroupRef;
				//store parseTreeGroupRefReverse/parseTreeGroupRefReverseComponentIndex orig (is this req?);
				parseTreeGroupRefParseTreeGroupRefReverseOrig.push_back(parseTreeGroupRef->parseTreeGroupRefReverse);
				parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig.push_back(parseTreeGroupRef->parseTreeGroupRefReverseComponentIndex);
				parseTreeGroupRef->parseTreeGroupRefReverse = activationPathWordCurrentParseTreeGroupOwner;
				parseTreeGroupRef->parseTreeGroupRefReverseComponentIndex = i;
			}
		#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
		}
		#endif
		SANIComponentParseTree* higherLevelComponent = NULL;
		SANIGroupParseTree* higherLevelComponentParseTreeGroupRefOrig = NULL;
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC
		//bool foundPreviousActiveWordOrig = forwardPropogationSignalData->foundPreviousActiveWord;
		#endif
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC	
		vector<SANIForwardPropogationSequenceElementData*> previousWordConnectionsOrig;
		#endif
		#endif
		if(activationSequenceCompleted)
		{
			if(lastComponentFuzzyAndCompatible)
			{
				if(activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse != NULL)
				{
					//cout << "lastComponentFuzzyAndCompatible" << endl;
					//exit(EXIT_ERROR);

					//replace parseTreeGroupRef of previously activated parent component
					/*
					cout << "activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse->groupName = " << activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse->groupName << endl;
					cout << "activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverseComponentIndex = " << activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverseComponentIndex << endl;
					cout << "activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse->components.size() = " << activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse->components.size() << endl;
					*/
					higherLevelComponent = activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverse->components[activationPathWordCurrentParseTreeGroupOwner->parseTreeGroupRefReverseComponentIndex];
					/*
					cout << "higherLevelComponent->groupRefName = " << higherLevelComponent->groupRefName << endl;
					cout << "done" << endl;
					*/
					higherLevelComponentParseTreeGroupRefOrig = higherLevelComponent->parseTreeGroupRef;
					higherLevelComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroupOwner;

					#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY
					#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC
					//forwardPropogationSignalData->foundPreviousActiveWord = true;
					#endif
					#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC
					previousWordConnectionsOrig = forwardPropogationWordDataPreviousRecord->previousSequenceElementConnections;
					if(find(forwardPropogationWordDataPreviousRecord->previousSequenceElementConnections.begin(), forwardPropogationWordDataPreviousRecord->previousSequenceElementConnections.end(), forwardPropogationSequenceElementData) == forwardPropogationWordDataPreviousRecord->previousSequenceElementConnections.end())
					{
						forwardPropogationWordDataPreviousRecord->previousSequenceElementConnections.push_back(forwardPropogationSequenceElementData);
						//cout << "forwardPropogationWordDataPreviousRecord->previousSequenceElementConnections.push_back(forwardPropogationSequenceElementData); [forwardPropogationWordDataPreviousRecord] forwardPropogationSequenceElementData->wordReference->tagName = " << forwardPropogationSequenceElementData->wordReference->tagName << endl;
						//exit(EXIT_ERROR);
					}
					#endif
					#endif					
				}
			}
		}
		#endif
		#endif

		#ifdef SANI_SOLIDIFY_NET_BACKPROP
		#ifdef SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
		//b) enforce solidification of previous active component subgroup
		if(activationPathWordCurrentParseTreeGroupOwner->components.size() >= 2)
		{
			int indexOfPreviousActiveComponent = activationPathWordCurrentParseTreeGroupOwner->components.size()-2;
			SANIComponentParseTree* component = activationPathWordCurrentParseTreeGroupOwner->components[indexOfPreviousActiveComponent];
			solidifyNeuralNetBackpropGroupLastComponent(component->parseTreeGroupRef, layer); 
		}
		#else
		newParseComponent->parseTreeGroupRef->solidified = true;
		#endif
		#endif
		
		
		#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		if(lastComponentFuzzyAndCompatible && activationSequenceCompleted)
		{
			#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
			SANInodes.printParseTreeDebugIndentation(layer+1);
			cout << "propagateWordThroughNetworkGroupComponent: lastComponentFuzzyAndCompatible && activationSequenceCompleted" << endl;
			#endif

			//both a) propagate up and  b) pass next sequenceElement

			//activation sequence completed (and not top level group), propagate next layer up
			if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroupOrig, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough))
			{
				result = true;
			}	
			else
			{
				/*
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC	
				forwardPropogationSequenceElementData->previousSequenceElementConnections = previousWordConnectionsOrig;
				#endif
				*/

				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData.foundPreviousActiveWord = forwardPropogationSignalData->foundPreviousActiveWord;
				#endif

				//record variables for SANIParse;
				activationPathWordCurrentParseTreeGroupOwner->neuronActive = true;	//not used	

				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroupOrig, &(activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData), forwardPropogationSequenceElementData, forwardPropogationSequenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner, passThrough))
				{
					result = true;
				}
			}	
		}
		#endif
		#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		if(lastComponentFuzzyAndCompatible)
		{
			#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
			SANInodes.printParseTreeDebugIndentation(layer+1);
			cout << "propagateWordThroughNetworkGroupComponent: lastComponentFuzzyAndCompatible" << endl;
			#endif
			if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroupOrig, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough))
			{
				result = true;
			}
		}
		#endif
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
	}
	#endif
			
		
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA4
	//bool sentenceWordDataFullyConnected = SANIpropagateOperations.isSequenceWordDataFullyConnected(forwardPropogationSequenceData);
	bool sentenceWordDataFullyConnected = false;
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroupOrig->groupTypeName << ":" << ownerGroupOrig->groupName << ", activationMemoryGroupArrayIndex = " << debug->activationMemoryGroupArrayIndex << ", firstComponentActive = " << debug->firstComponentActive << ", activationMemoryGroupArraySize = " << debug->activationMemoryGroupArraySize << ", SANIpropagateOperations.isSequenceWordDataFullyConnected(forwardPropogationSequenceData) = " << sentenceWordDataFullyConnected << endl;	
	SANInodes.printComponent(currentComponent, layer+1);
	#endif
	
	
	bool passTemp = false;
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
	if(passThrough)
	{
		passTemp = true;
	}
	else
	{
	#endif
		#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		if(!(lastComponentFuzzyAndCompatible && activationSequenceCompleted))
		{
		#endif
		#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		if(!lastComponentFuzzyAndCompatible)
		{
		#endif
			passTemp = true;
		#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		}
		#endif
		#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
		}
		#endif	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
	}
	#endif
	if(passTemp)
	{
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
		SANInodes.printParseTreeDebugIndentation(layer+1);
		cout << "propagateWordThroughNetworkGroupComponent: !lastComponentFuzzyAndCompatible, activationSequenceCompleted = " << activationSequenceCompleted << endl;
		#endif
		if(activationSequenceCompleted)
		{
			//cout << "propagateWordThroughNetworkGroupComponent: repeatDetected = " << repeatDetected << endl;

			#ifdef SANI_DEBUG_PROPAGATE_EXTRA
			SANInodes.printParseTreeDebugIndentation(layer+1);
			cout << "activationSequenceCompleted" << endl;
			#endif

			SANIForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = NULL;
			
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
			if(!passThrough)
			{	
			#endif
			
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData.foundPreviousActiveWord = forwardPropogationSignalData->foundPreviousActiveWord;
				#endif
				semanticRelationReturnEntityForwardPropogationSignalDataNew = &(activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData);

				//record variables for SANIParse;
				activationPathWordCurrentParseTreeGroupOwner->neuronActive = true;	//not used
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
			}
			else
			{
				semanticRelationReturnEntityForwardPropogationSignalDataNew = forwardPropogationSignalData;	//only used for forwardPropogationSignalData->foundPreviousActiveWord
			}
			#endif
			
			//cout << "DEBUG printBackpropParseTree:" << endl;
			//printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 1);

			bool topLevelGroup = SANInodesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeName, ownerGroupOrig->groupTypeReferenceSetType, forwardPropogationSequenceData->isQuery, forwardPropogationSequenceData->parseIsolatedSubreferenceSets);
			if(topLevelGroup)
			{
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
				if(passThrough)
				{	
					result = true;
				}
				else
				{
				#endif
					//cout << "topLevelGroup" << endl;
					//exit(0);

					#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC
					if(forwardPropogationSequenceElementData->sequenceIndex == forwardPropogationSequenceData->sentenceContents->size()-1)	//end of sentence punctuation mark found
					{
					#endif
						#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC_STRICT_OLD
						if(SANIpropagateOperations.isSequenceWordDataFullyConnected(forwardPropogationSequenceData))
						{
						#endif
					
							/*
							cout << "topLevelGroup && SANIpropagateOperations.isSequenceWordDataFullyConnected; TEMP EXIT" << endl;
							exit(EXIT_ERROR);
							*/

							result = true;
							forwardPropogationSequenceData->finishedPassingSequenceElements = true;

							forwardPropogationSequenceData->toplevelGroupActivationFound = true;
							#ifdef SANI_PARSE_SAVE_PARSE_TREE
							forwardPropogationSequenceData->topLevelParseTreeGroupPropagate = activationPathWordCurrentParseTreeGroupOwner;
							#endif
							#ifdef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE
							if(!SANIpropagateOperationsParseTree.updatePerformanceGroupSentence(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSequenceData, layer))
							{
								//result = false;
							}
							#endif

							/*
							cout << "topLevelGroup" << endl;
							cout << "finishedPassingSequenceElements (temp exit)" << endl;
							exit(0);
							*/

						#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC_STRICT_OLD
						}
						else
						{
							result = false;
						}
						#endif
					#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC
					}
					else
					{
						cerr << "SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: !(forwardPropogationSequenceElementData->sequenceIndex == forwardPropogationSequenceData->sentenceContents.size()-1)" << endl;
						exit(EXIT_ERROR);
					}
					#endif
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
				}	
				#endif
			}
			else
			{
				//activation sequence completed (and not top level group), propagate next layer up
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroupOrig, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationSequenceElementData, forwardPropogationSequenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner, passThrough))
				{
					result = true;
				}
				else
				{
					//ownerGroupOrig->neuronPropagated = false;
				}
			}
		}
		else
		{
			/*
			SANInodes.printParseTreeDebugIndentation(layer+1);
			cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroupOrig->groupTypeName << ":" << ownerGroupOrig->groupName << endl;	
			SANInodes.printComponent(currentComponent, layer+1);
			*/

			//cout << "propagateWordThroughNetworkGroupComponentPassNextWord" << endl;
			if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, ownerGroupOrig, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, lastComponentFuzzyAndCompatible, passThrough))
			{
				result = true;
			}
		}
	}


	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
	if(!passThrough)
	{	
	#endif
		#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD1
		if(!result)
		{
			#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
			activationPathSequenceElementCurrentParseTreeGroup->parseTreeGroupRefReverse = activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseOrig;
			activationPathSequenceElementCurrentParseTreeGroup->parseTreeGroupRefReverseComponentIndex = activationPathWordCurrentParseTreeGroupParseTreeGroupRefReverseComponentIndexOrig;
			if(activationSequenceCompleted)
			{
			#endif
				for(int i=0; i<activationPathWordCurrentParseTreeGroupOwner->components.size(); i++)
				{
					SANIComponentParseTree* component = activationPathWordCurrentParseTreeGroupOwner->components[i];
					SANIGroupParseTree* parseTreeGroupRef = component->parseTreeGroupRef;
					parseTreeGroupRef->parseTreeGroupRefReverse = parseTreeGroupRefParseTreeGroupRefReverseOrig[i];
					parseTreeGroupRef->parseTreeGroupRefReverseComponentIndex = parseTreeGroupRefParseTreeGroupRefReverseComponentIndexOrig[i];
				}
			#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_EFFICIENT
			}
			#endif
			if(activationSequenceCompleted)
			{
				if(lastComponentFuzzyAndCompatible && (higherLevelComponent != NULL))
				{
					higherLevelComponent->parseTreeGroupRef = higherLevelComponentParseTreeGroupRefOrig;

					#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY
					#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC
					//forwardPropogationSignalData->foundPreviousActiveWord = foundPreviousActiveWordOrig;
					#endif
					#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC	
					forwardPropogationWordDataPreviousRecord->previousSequenceElementConnections = previousWordConnectionsOrig;
					#endif
					#endif
				}
			}
		}
		#endif
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
	}	
	#endif
	
	return result;
}


#ifdef SANI_SOLIDIFY_NET_BACKPROP
#ifdef SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
bool SANIpropagateHeavyUnoptimisedClass::solidifyNeuralNetBackpropGroupLastComponent(SANIGroupParseTree* currentParseTreeGroup, const int level)
{
	bool result = true;

	currentParseTreeGroup->solidified = true;
	
	if(currentParseTreeGroup->components.size() > 0)
	{
		int indexOfLastComponentInGroup = currentParseTreeGroup->components.size()-1;
		SANIComponentParseTree* lastComponent = (currentParseTreeGroup->components)[indexOfLastComponentInGroup];
		solidifyNeuralNetBackpropGroupLastComponent(lastComponent->parseTreeGroupRef, level+1);
	}
		
	return result;
}
#endif
#endif



/*
bool SANIpropagateHeavyUnoptimisedClass::findComponentRef(SANIGroupParseTree* currentParseTreeGroup, SANIComponentParseTree* componentRef, SANIComponentParseTree** componentFound)
{
	bool result = false;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
		if(currentParseTreeComponent->componentRef == componentRef)
		{
			result = true;
			*componentFound = currentParseTreeComponent;
		}
	}
	
	return result;
}
*/


bool SANIpropagateHeavyUnoptimisedClass::propagateWordThroughNetworkGroupComponentPassNextWord(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* ownerGroupOrig, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough)
{
	bool result = false;
	bool pass = true;
	bool expectToSeeConnectionWithPreviousWordTrace = BOOL_IRRELEVANT;
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT
	if(passThrough)
	{
		if(forwardPropogationSignalData->foundPreviousActiveWord)	//FUTURE GIA? - change to; found any previous active sequenceElement
		{
			pass = false;
			result = true;	//foundPreviousActiveWord in hypothetical activation path 
			//cout << "forwardPropogationSignalData->foundPreviousActiveWord = " << forwardPropogationSignalData->foundPreviousActiveWord << endl;
		}
		else
		{
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT_MULTIPLE_ACTIVATION_HYPOTHESES
			bool topLevelGroup = SANInodesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeName, ownerGroupOrig->groupTypeReferenceSetType, forwardPropogationSequenceData->isQuery, forwardPropogationSequenceData->parseIsolatedSubreferenceSets);
			if(!topLevelGroup)
			{
				//hypothesise that activation sequence is completed (and not top level group), propagate next layer up
				bool passThroughNew = true;
				
				//cout << "propagateWordThroughNetworkGroup START: [SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT_MULTIPLE_ACTIVATION_HYPOTHESES]" << endl;
				SANIForwardPropogationSignalData* forwardPropogationSignalDataNew = forwardPropogationSignalData; //SANIForwardPropogationSignalData forwardPropogationSignalDataNew = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//CHECK THIS
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroupOrig, forwardPropogationSignalDataNew, forwardPropogationSequenceElementData, forwardPropogationSequenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner, passThrough))
				{
					//cout << "propagateWordThroughNetworkGroup pass [SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT_MULTIPLE_ACTIVATION_HYPOTHESES]" << endl;
					pass = false;
					result = true;
				}
				else
				{
					//cout << "propagateWordThroughNetworkGroup fail [SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT_MULTIPLE_ACTIVATION_HYPOTHESES]" << endl;
					pass = false;
					result = false;
				}
			}
			else
			{
				pass = false;
				result = false;			
			}				
			#else
			pass = false;
			result = false;	//irrelevant
			#endif
		}
	}
	else
	{
		if(forwardPropogationSequenceElementData->sequenceIndex > 0)	//first sequenceElement does not require previousWord connectivity
		{
			if(forwardPropogationSignalData->foundPreviousActiveWord)	//FUTURE GIA? - change to; found any previous active sequenceElement
			{
				pass = true;
			}
			else
			{	
				//activate passThrough
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT_NOUNS_ONLY
				if(SANInodes.isWordPOStypeNoun(forwardPropogationSequenceElementData->wordPOStype))
				{
				#endif
				
					bool topLevelGroup = SANInodesGroupClassObject.isTopLevelGroupType(ownerGroupOrig->groupTypeName, ownerGroupOrig->groupTypeReferenceSetType, forwardPropogationSequenceData->isQuery, forwardPropogationSequenceData->parseIsolatedSubreferenceSets);
					if(!topLevelGroup)
					{
						//hypothesise that activation sequence is completed (and not top level group), propagate next layer up
						bool passThroughNew = true;
						
						/*
						if(lastComponentFuzzyAndCompatible)
						{
							cout << "propagateWordThroughNetworkGroup: lastComponentFuzzyAndCompatible, ownerGroupOrig->groupName = " << ownerGroupOrig->groupName << endl;
						}
						*/
						
						//cout << "propagateWordThroughNetworkGroup START:" << endl;
						SANIForwardPropogationSignalData* forwardPropogationSignalDataNew = forwardPropogationSignalData;	//SANIForwardPropogationSignalData forwardPropogationSignalDataNew = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//CHECK THIS
						if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroupOrig, forwardPropogationSignalDataNew, forwardPropogationSequenceElementData, forwardPropogationSequenceData, (layer+1), activationPathSequenceElementCurrentParseTreeGroup, passThroughNew))
						{
							//cout << "propagateWordThroughNetworkGroup pass" << endl;
							//exit(EXIT_ERROR);
							
							pass = true;
						}
						else
						{
							//cout << "propagateWordThroughNetworkGroup fail" << endl;
							pass = false;
							result = false;
						}
					}
					else
					{
						pass = false;
						result = false;		
					}
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFY_HYPOTHETICAL_PROPOGATION_FINDS_PREVIOUS_SEQUENCEELEMENT_NOUNS_ONLY
				}
				else
				{
					pass = true;
				}
				#endif
			}		
		}
		else
		{
			pass = true;
		}
	}
	#endif
		
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
	#ifdef SANI_SUPPORT_FUZZY_LAST_COMPONENTS_METHOD2
	if(lastComponentFuzzyAndCompatible)
	{
		forwardPropogationSignalData->foundPreviousActiveWord = true;	
	}
	#endif
	#endif
	//cout << "forwardPropogationSequenceElementData->expectToSeeConnectionWithPreviousWordTrace = " << forwardPropogationSequenceElementData->expectToSeeConnectionWithPreviousWordTrace << ", forwardPropogationSignalData->foundPreviousActiveWord = " << forwardPropogationSignalData->foundPreviousActiveWord << endl;
	if(forwardPropogationSequenceElementData->expectToSeeConnectionWithPreviousWordTrace)
	{
		pass = false;
		if(forwardPropogationSignalData->foundPreviousActiveWord)
		{
			pass = true;
		}
		else
		{
			//cout << "!pass" << endl;
		}
	}
	if(pass)
	{
		SANInodes.printParseTreeDebugIndentation(layer+1);
		//cout << "forwardPropogationSequenceElementData->sequenceIndex = " << forwardPropogationSequenceElementData->sequenceIndex << endl;
		
		//cout << "pass = " << pass << endl;
		//cout << "propagateWordThroughNetworkGroupComponentPassNextWord: ownerGroupOrig = " << ownerGroupOrig->groupName << ", ownerGroupOrig->groupTypeName = " << ownerGroupOrig->groupTypeName << endl;
				
		//pass next sequenceElement though network
		expectToSeeConnectionWithPreviousWordTrace = false;
		forwardPropogationSequenceElementData->foundPreviousActiveWord = forwardPropogationSignalData->foundPreviousActiveWord;
		
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
		if(forwardPropogationSignalData->foundPreviousActiveWord)
		{
			expectToSeeConnectionWithPreviousWordTrace = false;
		}
		else
		{
			if(forwardPropogationSequenceElementData->sequenceIndex > 0)
			{
				SANIForwardPropogationSequenceElementData* forwardPropogationWordDataPrevious = (forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray[forwardPropogationSequenceElementData->sequenceIndex-1]);
				if(forwardPropogationWordDataPrevious->foundPreviousActiveWord)
				{
					expectToSeeConnectionWithPreviousWordTrace = false;	//allow single (max 1) isolated sequenceElements
				}
				else
				{
					expectToSeeConnectionWithPreviousWordTrace = true;
				}
			}
			else
			{
				expectToSeeConnectionWithPreviousWordTrace = false;
			}
		}
		#endif
	}
	#endif
		
	if(pass)
	{
		int w = forwardPropogationSequenceElementData->sequenceIndex + 1;
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
		cout << "forwardPropogationSequenceElementData->sequenceIndex+1 = " << forwardPropogationSequenceElementData->sequenceIndex+1 << endl;
		#endif
		
		bool passThroughNew = false;
		if(propagateWordThroughNetworkIntro(translatorVariables, w, forwardPropogationSequenceData, layer+1, activationPathWordCurrentParseTreeGroupOwner, expectToSeeConnectionWithPreviousWordTrace, passThroughNew))
		{
			result = true;
		}
		else
		{
			//ownerGroupOrig->neuronPropagated = false;
		}
	}
		
	return result;
}











bool SANIpropagateHeavyUnoptimisedClass::printBackpropParseTree(SANIGroupParseTree* group, const int level)
{
	#ifdef SANI_DEBUG_PARSE_TREE_PRINT_SUPPORT_RECURSION
	SANIpropagateOperationsParseTree.traceBackpropParseTreePrint(group, 1);
	SANIpropagateOperationsParseTree.resetNeuronBackprop(group, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);	
	#else
	SANInodes.printParseTree(group, level);
	#endif
}




bool SANIpropagateHeavyUnoptimisedClass::groupActivationCompleted(const SANIGroupNeuralNetwork* group)
{	
	bool groupActivationCompleted = true;
	
	for(int i2=0; i2<group->components.size(); i2++)
	{
		SANIComponentNeuralNetwork* currentComponent2 = (group->components)[i2];
		/*
		cout << "\tcurrentComponent2->neuronComponentConnectionActive = " << currentComponent2->neuronComponentConnectionActive << endl;
		cout << "\tcurrentComponent2->optional = " << currentComponent2->optional << endl;
		cout << "\tcurrentComponent2->hasSubComponents = " << SANInodesComponentClassObject.componentHasSubcomponents(currentComponent2) << endl;
		*/
		if(!(currentComponent2->neuronComponentConnectionActive))
		{
			if(!(currentComponent2->optional))
			{
				groupActivationCompleted = false;
			}
		}
	}
	
	return groupActivationCompleted;
}

#ifdef SANI_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_DYNAMIC_RESET_INCLUDE_PARTIALLY_OR_FULLY_ACTIVATED_GROUPS
bool SANIpropagateHeavyUnoptimisedClass::groupPartiallyOrFullyActivated(const SANIGroupNeuralNetwork* group)
{	
	bool groupPartiallyActivated = false;
	
	for(int i2=0; i2<group->components.size(); i2++)
	{
		SANIComponentNeuralNetwork* currentComponent2 = (group->components)[i2];
		if(currentComponent2->neuronComponentConnectionActive)
		{
			groupPartiallyActivated = true;
		}
	}
	
	return groupPartiallyActivated;
}
#endif
				










/*
#ifdef SANI_SAVE_MEMORY_GROUPS
SANIGroupNeuralNetwork* SANIpropagateHeavyUnoptimisedClass::copyGroup(SANIGroupNeuralNetwork* group)
{		
	SANIGroupNeuralNetwork* newGroup = SANInodes.copyGroup(group);
	
	//not required (redundant), since only base (empty) groups or activationMemory groups are ever copied;
	newGroup->activationMemoryGroupArray.clear();
	newGroup->currentParseTreeGroupArray.clear();
	
	return newGroup;
}
#endif
*/

#endif


#endif
