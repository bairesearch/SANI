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
 * File Name: SANIpropagateLightUnoptimised.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Light Unoptimised - ~O(n^2)
 * /
 *******************************************************************************/


#include "SANIpropagateLightUnoptimised.hpp"


#ifdef SANI_LIGHT_UNOPTIMISED


bool SANIpropagateLightUnoptimisedClass::executePosRelTranslatorNeuralNetwork(const SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
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
	
	#ifdef SANI_PARSE_SAVE_PARSE_TREE
	forwardPropogationSequenceData.topLevelParseTreeGroupPropagate = NULL;
	#endif
	#ifdef SANI_LIGHT_BIO
	(forwardPropogationSequenceData.parseTreeGroupsArrayPropagate).clear();
	#endif

	SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of SANIpropagateLightUnoptimisedClass::executePosRelTranslatorNeuralNetwork)
	
	SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup = NULL;
		
	if(sentenceContents->size() > 0)
	{
		int w;
		if(forwardPropogationSequenceData.parseSentenceReverse)
		{
			w = sentenceContents->size()-1;
		}
		else
		{
			w = 0;
		}
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSequenceData, 0, activationPathSequenceElementFirstParseTreeGroup))
		{
			result = false;
		}
	}
	else
	{
		cerr << "SANIpropagateLightUnoptimisedClass::executePosRelTranslatorNeuralNetwork{} error: sentenceContents->size() == 0" << endl;
		exit(EXIT_ERROR);		
	}
	
	if(forwardPropogationSequenceData.toplevelGroupActivationFound)
	{
		sentenceValidActivationPath = true;
		result = true;
		
		#ifdef SANI_PARSE_SAVE_PARSE_TREE
		*topLevelParseTreeGroup = forwardPropogationSequenceData.topLevelParseTreeGroupPropagate;
		//cout << "forwardPropogationSequenceData.topLevelParseTreeGroupPropagate" << endl;
		
		//#ifdef SANI_DEBUG_PROPAGATE
		cout << "topLevelGroup" << endl;
		printBackpropParseTree(forwardPropogationSequenceData.topLevelParseTreeGroupPropagate, 1);
		cout << "end printBackpropParseTree" << endl;
		//cout << "forwardPropogationSequenceData.topLevelParseTreeGroupPropagate->groupName = " << forwardPropogationSequenceData.topLevelParseTreeGroupPropagate->groupName << endl;
		//#endif	
		#endif	
		
		#ifdef SANI_PARSE_PERFORMANCE
		*performance = forwardPropogationSequenceData.performance;
		//cout << "forwardPropogationSequenceData.performance = " << forwardPropogationSequenceData.performance << endl;
		#else
		*performance = 1;
		#endif
		
		/*
		cout << "DEBUG: finished executing SANIpropagateLightUnoptimisedClass::executePosRelTranslatorNeuralNetwork (exiting)" << endl;
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
		
		/*
		for(int w=0; w<sentenceContents->size(); w++)
		{
			SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = (forwardPropogationSequenceData.forwardPropogationSequenceElementDataArray[w]);
		}
		*/
	}
	else
	{
		SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);
	}
	#endif
	
	//TODO: resetSemanticRelationReturnEntityForwardPropogationSignalDataPackage(sentenceContents)
	
	/*
	#ifdef SANI_DEBUG_PROPAGATE
	cout << "DEBUG: finished executing SANIpropagateLightUnoptimisedClass::executePosRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	return result;
}

	


bool SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro(const SANItranslatorVariablesClass* translatorVariables, int w, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup)
{
	bool result = false;

	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	SANIpropagateOperations.resetAllNeuronComponents(forwardPropogationSequenceData->SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE);
	SANIpropagateOperations.resetAllNeuronComponents(forwardPropogationSequenceData->SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED);
	#endif
	
	//cout << "w = " << w << endl;
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

	forwardPropogationSequenceElementData->wordReference = currentWord;
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationSequenceElementData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
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
				if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
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
			if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
			{
				result = true;
			}
		}
		#else
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
		int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_NOUN;
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
		{
			result = true;
		}	
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
		if(LRPpreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
		{	
			int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT;
			if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
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
		cout << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
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
		cout << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
		{
			result = true;
		}
	}
	else if(SANIformation.findWordInGroupMap(currentWord->tagName, SANIformation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationSequenceElementData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef SANI_DEBUG_PROPAGATE
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
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
							cerr << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
							exit(EXIT_ERROR);
						}
							
						#ifdef SANI_DEBUG_PROPAGATE
						SANInodes.printParseTreeDebugIndentation(layer);
						cout << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
						#endif
						if(propagateWordThroughNetworkGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClassType, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
						{
							result = true;
						}
						if(propagateWordThroughNetworkGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClass, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))	//what is this for?
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

	
bool SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupInit(const SANItranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup)
{
	bool result = false;
	
	SANIGroupNeuralNetwork* inputLayerGroup = SANIformation.getInputGroupLayerSection(SANIformation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef SANI_DEBUG_PROPAGATE
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationSequenceElementData->wordPOStype = wordPOStype;
	#endif
	
	if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}	
	
bool SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupInit(const SANItranslatorVariablesClass* translatorVariables, const int w, SANIGroupNeuralNetwork* inputLayerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup)
{
	bool result = false;
	
	inputLayerGroup->neuronPropagated = false;
	
	if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}				
				

bool SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupIntro(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup)
{
	bool result = false;
	
	if(propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}

bool SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroup(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_PROPAGATE
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;
	#endif
			
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops
	{
		group->neuronPropagated = true;
	#endif
		
		//vector<GIAactivationPathMostRecentContribution> frontLayerActivationPathMostRecentContribution;
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup))
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
		
bool SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupSelect(const SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup)
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
			
			#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
			if(!ownerGroup->neuronPropagated)	//prevent circular loops
			{
			#endif	
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
						
					if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
					{
						currentComponent->candidateStringMatch = forwardPropogationSequenceElementData->wordReference;
					}

					SANIposRelTranslatorDebug debug;
					#ifdef SANI_DEBUG_PROPAGATE_EXTRA4
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

					if(currentComponent->semanticRelationReturnEntity)
					{
						ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
					}

					bool deleteNewlyCreatedMemoryGroup = false;
					if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup, i, &debug))
					{
						result = true;
					}

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
	


bool SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(const SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const SANIposRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	SANInodes.printComponent(currentComponent, layer+1);
	#endif
	
	/*
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC	
	vector<SANIForwardPropogationSequenceElementData*> previousWordConnectionsOrig = forwardPropogationSequenceElementData->previousSequenceElementConnections;
	#endif
	*/
	
	SANIComponentNeuralNetwork* previousActiveComponent = NULL;	//NOTUSED
	SANIComponentNeuralNetwork* lastActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;	//NOTUSED
	if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, ownerComponent, &(ownerGroup->components), &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &lastActiveComponent))
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
					//cout << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(forwardPropogationSequenceElementData, forwardPropogationSequenceData, currentComponent, &(ownerComponent->subComponents), &repeatDetected))
				{
					subComponentsPassed = true;
					#ifdef SANI_DEBUG_PROPAGATE
					//cout << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
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
			
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "3b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	#endif
			#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
			if(componentTests2(ownerGroup, activationPathSequenceElementCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData))
			{
			#endif
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "3c: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	#endif
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
				if(SANIpropagateOperations.componentWordConnectivityTestsWrapper(forwardPropogationSequenceElementData, forwardPropogationSequenceData, ownerGroup, activationPathSequenceElementCurrentParseTreeGroup))
				{
				#endif
					if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, activationSequenceCompleted, layer, activationPathSequenceElementCurrentParseTreeGroup, groupFrontComponentConnectionListIndex, debug))
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
	
	/*
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC	
	if(!result)
	{
		forwardPropogationSequenceElementData->previousSequenceElementConnections = previousWordConnectionsOrig;
	}
	#endif
	*/
	
	return result;
}
	
	
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent(const SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* ownerComponent, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool activationSequenceCompleted, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const SANIposRelTranslatorDebug* debug)
{
	bool result = false;
	
	#ifdef SANI_LIGHT_NEW_CODE
	#ifdef SANI_LIGHT_MEM
	SANIGroupNeuralNetwork* ownerGroupOrig = SANInodes.copyGroup(ownerGroup);
	#else
	SANIGroupNeuralNetwork* ownerGroupOrig = NULL;
	#endif
	#endif

	ownerComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
	ownerComponent->neuronComponentConnectionActiveSequenceElementRecord = forwardPropogationSequenceElementData;
	currentComponent->neuronComponentConnectionActiveSequenceElementRecord = forwardPropogationSequenceElementData;

	#ifdef SANI_PARSE
	//create new parseTreeGroup
	//copied from SANIpropagateInverseClass::generateRulesGroupTreeComponents;
	SANIComponentParseTree* newParseComponent = SANInodes.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new SANIComponentParseTree(*SANIpropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
	newParseComponent->componentRef = currentComponent;
	newParseComponent->neuronComponentConnectionActive = true;
	newParseComponent->neuronComponentConnectionActiveSequenceElementRecord = currentComponent->neuronComponentConnectionActiveSequenceElementRecord;
	newParseComponent->parseTreeGroupRef = activationPathSequenceElementCurrentParseTreeGroup;
	//currentWord->wordPOStypeInferred = forwardPropogationSequenceElementData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous sequenceElements in current parse tree	//this will be set later by GIAposRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
	newParseComponent->wordPOStypeInferred = forwardPropogationSequenceElementData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
	SANIGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;
	#ifdef SANI_LIGHT_BIO2
	//newParseComponent->componentRef = currentComponent;
	newParseComponent->groupFrontComponentConnectionListIndex = groupFrontComponentConnectionListIndex;
	newParseComponent->numberOfWordsCurrentlyParsed = forwardPropogationSequenceElementData->sequenceIndex;
	#endif
	#ifdef SANI_LIGHT_BIO
	if(currentParseTreeGroupTemp->components.size() == 0)
	{
		(forwardPropogationSequenceData->parseTreeGroupsArrayPropagate).push_back(currentParseTreeGroupTemp);
	}
	#endif
	/*
	#ifdef SANI_SOLIDIFY_NET_BACKPROP
	newParseComponent->parseTreeGroupRef->solidified = true;
	#endif
	*/
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		//currentParseTreeGroupTemp->components.push_front(newParseComponent);
		currentParseTreeGroupTemp->components.insert(currentParseTreeGroupTemp->components.begin(), newParseComponent);
	}
	else
	{
		currentParseTreeGroupTemp->components.push_back(newParseComponent);
	}
	#endif
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA4
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", firstComponentActive = " << debug->firstComponentActive << endl;	
	SANInodes.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef SANI_PARSE
	SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroupTemp;
	#endif

	/*
	if(ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)
	{
		cerr << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error (ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)" << endl;
		exit(EXIT_ERROR);
	}
	*/
				
	if(activationSequenceCompleted)
	{		
		ownerGroup->neuronActive = true;	//not used
		
		#ifdef SANI_LIGHT_NEW_CODE
		SANIpropagateOperations.resetGroupActivation(ownerGroup);
		#ifdef SANI_PARSE
		//create new currentParseTreeGroupTemp for ownerGroup;
		//cout << "SANIpropagateOperationsParseTree.resetGroupParseTreeGroupRef" << endl;
		SANIpropagateOperationsParseTree.resetGroupParseTreeGroupRef(ownerGroup, false);
		/*
		if(ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)
		{
			cerr << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: (ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)" << endl;
			exit(EXIT_ERROR);
		}
		*/
		#endif
		#endif	
		
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA
		SANInodes.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif

		ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData = ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
		SANIForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData);

		bool topLevelGroup = SANInodesGroupClassObject.isTopLevelGroupType(ownerGroup->groupTypeName, ownerGroup->groupTypeReferenceSetType, forwardPropogationSequenceData->isQuery, forwardPropogationSequenceData->parseIsolatedSubreferenceSets);
		if(topLevelGroup)
		{
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC
			int wComparisonInt;
			if(forwardPropogationSequenceData->parseSentenceReverse)
			{
				wComparisonInt = 0;	//start of sentence found
			}
			else
			{
				wComparisonInt = forwardPropogationSequenceData->sentenceContents->size()-1;	//end of sentence punctuation mark found
			}
			if(forwardPropogationSequenceElementData->sequenceIndex == wComparisonInt)	//start of sentence found
			{
			#endif
				if(SANIpropagateOperationsParseTree.updateAndVerifyPerformanceGroupSentence(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSequenceData, layer))
				{
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
					/*
					cout << "topLevelGroup" << endl;
					cout << "finishedPassingSequenceElements (temp exit)" << endl;
					exit(0);
					*/
				}
				else
				{
					//fail
					restoreGroupActivations(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSequenceElementData, forwardPropogationSequenceData, true);
				}	

			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC
			}
			else
			{
				//activation sequence completed (and top level group), propagate next sequenceElement
				if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner))
				{
					result = true;
				}
				else
				{
					//ownerGroupOrig->neuronPropagated = false;
					restoreGroupActivations(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSequenceElementData, forwardPropogationSequenceData, true);
					
					if(ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)
					{
						cerr << "4 (ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)" << endl;
						exit(EXIT_ERROR);
					}
				}
				
				/*
				#ifdef SANI_REVERSE_DIRECTION
				cerr << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: !(forwardPropogationSequenceElementData->sequenceIndex == 0)" << endl;
				#else
				cerr << "SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponent error: !(forwardPropogationSequenceElementData->sequenceIndex == forwardPropogationSequenceData->sentenceContents.size()-1)" << endl;
				#endif
				cerr << "forwardPropogationSequenceElementData->sequenceIndex = " << forwardPropogationSequenceElementData->sequenceIndex << endl;
				exit(EXIT_ERROR);
				*/
			}
			#endif
		}
		else
		{
			//activation sequence completed (and not top level group), propagate next layer up
			if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationSequenceElementData, forwardPropogationSequenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))
			{
				result = true;
			}
			else
			{
				//ownerGroupOrig->neuronPropagated = false;
				restoreGroupActivations(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSequenceElementData, forwardPropogationSequenceData, true);
				
				if(ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)
				{
					cerr << "5 (ownerGroup->currentParseTreeGroupTemp->groupName != ownerGroup->groupName)" << endl;
					exit(EXIT_ERROR);
				}

			}
		}
	}
	else
	{
		//activation sequence completed, propagate next sequenceElement
		if(propagateWordThroughNetworkGroupComponentPassNextWord(translatorVariables, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner))
		{
			result = true;
		}
		else
		{
			//#ifdef SANI_LIGHT_MEM
			restoreGroupActivations(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSequenceElementData, forwardPropogationSequenceData, false);
			//#endif
		}
	}
	
	#ifdef SANI_LIGHT_NEW_CODE
	#ifdef SANI_LIGHT_MEM
	SANInodes.deleteGroup(ownerGroupOrig);
	#endif
	#endif


	return result;
}


void SANIpropagateLightUnoptimisedClass::restoreGroupActivations(SANIGroupNeuralNetwork* ownerGroup, SANIGroupNeuralNetwork* ownerGroupOrig, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool restoreCurrentParseTreeGroupTemp)
{
	#ifdef SANI_LIGHT_NEW_CODE
	if(restoreCurrentParseTreeGroupTemp)
	{
		SANIpropagateOperationsParseTree.restoreGroupParseTreeGroupRef(ownerGroup, activationPathWordCurrentParseTreeGroupOwner);	//restore original group from memory
	}
	//cout << "ownerGroup->currentParseTreeGroupTemp = " << ownerGroup->currentParseTreeGroupTemp->groupName << endl;
	
	#ifdef SANI_LIGHT_MEM
	SANIComponentParseTree* latestParseTreeComponent = NULL;
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		//ownerGroup->currentParseTreeGroupTemp.pop_front();
		latestParseTreeComponent = ownerGroup->currentParseTreeGroupTemp->components[0];
		ownerGroup->currentParseTreeGroupTemp->components.erase(ownerGroup->currentParseTreeGroupTemp->components.begin());
	}
	else
	{
		latestParseTreeComponent = ownerGroup->currentParseTreeGroupTemp->components[ownerGroup->currentParseTreeGroupTemp->components.size()-1];
		ownerGroup->currentParseTreeGroupTemp->components.pop_back();
	}
	delete latestParseTreeComponent;
	SANIpropagateOperations.restoreGroupActivation(ownerGroup, ownerGroupOrig);
	//NOTNECESSARY: SANIpropagateOperationsParseTree.resetGroupParseTreeGroupRef(ownerGroup, true);
	#endif
		
	#else
	
	#ifdef SANI_LIGHT_BIO
	for(int i2=0; i2<(forwardPropogationSequenceData->parseTreeGroupsArrayPropagate).size(); i2++)
	{
		SANIGroupParseTree* parseTreeGroup = (forwardPropogationSequenceData->parseTreeGroupsArrayPropagate)[i2];
		if(parseTreeGroup == activationPathWordCurrentParseTreeGroupOwner)	//this check is only allowed if SANIpropagateLightUnoptimisedClass::restoreGroupActivations is always executed when if(!propagateWordThroughNetworkGroup) / if(!propagateWordThroughNetworkGroupComponentPassNextWord)
		{
			for(int i1=0; i1<parseTreeGroup->components.size(); i1++)
			{
				SANIComponentParseTree* parseTreeComponent = (parseTreeGroup->components)[i1];
				int parseTreeComponentSequenceIndex = parseTreeComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
				
				bool parseTreeComponentSequenceIndexComparisonBool = false;
				if(forwardPropogationSequenceData->parseSentenceReverse)
				{
					if(parseTreeComponentSequenceIndex <= forwardPropogationSequenceElementData->sequenceIndex)
					{
						parseTreeComponentSequenceIndexComparisonBool = true;
					}
				}
				else
				{
					if(parseTreeComponentSequenceIndex >= forwardPropogationSequenceElementData->sequenceIndex)
					{
						parseTreeComponentSequenceIndexComparisonBool = true;
					}
				}
				if(parseTreeComponentSequenceIndexComparisonBool)
				{
					SANIComponentParseTree* component = parseTreeComponent->componentRef;
					component->neuronComponentConnectionActive = false;
					parseTreeGroup->components.erase(parseTreeGroup->components.begin() + i1); //delete parseTreeComponent from parseTreeGroup
					i1--;
					delete parseTreeComponent;
				}
			}
			if(parseTreeGroup->components.size() == 0)
			{
				(forwardPropogationSequenceData->parseTreeGroupsArrayPropagate).erase((forwardPropogationSequenceData->parseTreeGroupsArrayPropagate).begin() + i2); //delete parseTreeGroup from array
				i2--;
				//NOTNECESSARY: SANIpropagateOperationsParseTree.resetGroupParseTreeGroupRef(ownerGroup, true);
			}
		}
	}
	#else
	
	#endif
	
	
	#endif
}

bool SANIpropagateLightUnoptimisedClass::propagateWordThroughNetworkGroupComponentPassNextWord(const SANItranslatorVariablesClass* translatorVariables, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner)
{
	bool result = false;
	
	int wNext;
	bool wNextComparison = false;
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		wNext = forwardPropogationSequenceElementData->sequenceIndex - 1;
		if(wNext >= 0)
		{
			wNextComparison = true;
		}
	}
	else
	{
		wNext = forwardPropogationSequenceElementData->sequenceIndex + 1;
		if(wNext < forwardPropogationSequenceData->sentenceContents->size())
		{
			wNextComparison = true;
		}
	}
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
	cout << "forwardPropogationSequenceElementData->sequenceIndex+1 = " << forwardPropogationSequenceElementData->sequenceIndex+1 << endl;
	#endif
	
	bool passThroughNew = false;
	if(wNextComparison)
	{
		if(propagateWordThroughNetworkIntro(translatorVariables, wNext, forwardPropogationSequenceData, layer+1, NULL))
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






#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
bool SANIpropagateLightUnoptimisedClass::componentTests2(const SANIGroupNeuralNetwork* group, const SANIGroupParseTree* currentParseTreeGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	bool componentTests = true;
	
	/*
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
	Algorithm:
	#else
	Algorithm:
	previousWordPOStype/existsPreceedingWordPOStype requirement: there exists a previous sequenceElement POStype
	OLD:
	previousWordPOStype/existsPreceedingWordPOStype requirement: there exists a previous sequenceElement POStype that was succesfully parsed by a higher order neuron/group.
		Question: so how will the program know this until the higher order neuron group has been parsed?
		Conclusion: will need to;
			a) record the sequenceElementPos type for every activated component in a group
			b) read both;
				i) previous components in current group and 
				ii) read up the tree to see if this condition is met (ie there exists a previous sequenceElement POStype that was succesfully parsed by a higher order neuron/group)
	#endif
	*/
	int currentSequenceIndex = forwardPropogationSequenceElementData->sequenceIndex;
	if(currentSequenceIndex > 0)	//CHECKTHIS
	{
		if(group->previousWordPOStype != "")
		{
			componentTests = false;
			int previousWordPOStype = LRPpreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype);

			if(findPreviousWordInSentence(forwardPropogationSequenceData->sentenceContents, currentSequenceIndex, previousWordPOStype))		
			{
				//cout << "componentTests = true" << endl;
				componentTests = true;
			}
		}
		if(group->existsPreceedingWordPOStype != "")
		{
			componentTests = false;
			int existsPreceedingWordPOStype = LRPpreprocessorWordClassObject.getPOStypeFromName(group->existsPreceedingWordPOStype);

			if(findPreceedingWordInSentence(forwardPropogationSequenceData->sentenceContents, currentSequenceIndex, existsPreceedingWordPOStype))		
			{
				componentTests = true;
			}
		}
	}
	
	return componentTests;
}

bool SANIpropagateLightUnoptimisedClass::findPreviousWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const int currentSequenceIndex, const int previousWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	LRPpreprocessorPlainTextWord* previousComponentWord = NULL;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		LRPpreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(w == currentSequenceIndex)
			{
				stillReadingActivatedWords = false;
			}
			else
			{
				previousComponentWord = contextWord;
			}
		}
	}
	if(previousComponentWord != NULL)
	{
		if(SANIpropagateOperations.checkWordPOStype(previousComponentWord, previousWordPOStype))
		{
			result = true;
		}
	}
			
	return result;
}
bool SANIpropagateLightUnoptimisedClass::findPreceedingWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const int currentSequenceIndex, const int existsPreceedingWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		LRPpreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(w == currentSequenceIndex)
			{
				stillReadingActivatedWords = false;
			}
			else
			{
				if(SANIpropagateOperations.checkWordPOStype(contextWord, existsPreceedingWordPOStype))
				{
					result = true;
				}
			}
		}
	}
	
	return result;
}
#endif





bool SANIpropagateLightUnoptimisedClass::printBackpropParseTree(SANIGroupParseTree* group, const int level)
{
	#ifdef SANI_DEBUG_PARSE_TREE_PRINT_SUPPORT_RECURSION
	SANIpropagateOperationsParseTree.traceBackpropParseTreePrint(group, 1);
	SANIpropagateOperationsParseTree.resetNeuronBackprop(group, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);	
	#else
	SANInodes.printParseTree(group, level);
	#endif
}




bool SANIpropagateLightUnoptimisedClass::groupActivationCompleted(const SANIGroupNeuralNetwork* group)
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
				






#endif
