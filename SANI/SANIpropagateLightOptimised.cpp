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
 * File Name: SANIpropagateLightOptimised.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p8a 29-April-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Light Optimised - ~O(n)
 * /
 *******************************************************************************/


#include "SANIpropagateLightOptimised.hpp"
#include "SHAREDvars.hpp"


#ifdef SANI_LIGHT_OPTIMISED



bool SANIpropagateLightOptimisedClass::executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, constEffective vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
{
	bool result = false;

	*performance = 0;
	
	bool sentenceValidActivationPath = false;
	
	//sentence specific variables:
	SANIForwardPropogationSentenceData forwardPropogationSentenceData;
	SANIpropagateOperations.setParseSentenceReverse(true, &forwardPropogationSentenceData);
	forwardPropogationSentenceData.toplevelGroupActivationFound = false;
	//forwardPropogationSentenceData.performance = performance;
	#ifdef SANI_PARSE_SIMULTANEOUS
	forwardPropogationSentenceData.parserEnabled = parserEnabled;
	#endif
	forwardPropogationSentenceData.sentenceContents = sentenceContents;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	forwardPropogationSentenceData.isQuery = SANInodesGroupClassObject.determineIsQuery(sentenceContents);
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	forwardPropogationSentenceData.parseIsolatedSubreferenceSets = parseIsolatedSubreferenceSets;
	#endif
	forwardPropogationSentenceData.SANIGroupTypes = SANIGroupTypes;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		SANIForwardPropogationWordData* forwardPropogationWordData = new SANIForwardPropogationWordData();
		forwardPropogationSentenceData.forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
	}
	
	#ifdef SANI_PARSE_SAVE_PARSE_TREE
	forwardPropogationSentenceData.topLevelParseTreeGroupPropagate = NULL;
	#endif
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
	forwardPropogationSentenceData.parseTreeMaxWeightPropagate = 0;
	#endif					
					
	SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE); //this is required for SANI_LIGHT_OPTIMISED
	SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of SANIpropagateLightOptimisedClass::executePosRelTranslatorNeuralNetwork)
	#ifdef SANI_FREE_MEMORY
	SANIpropagateOperationsParseTree.initialiseParseTreeGroupList(SANIGroupTypes, &(forwardPropogationSentenceData.parseTreeGroupListPropagate));
	#endif

	SANIGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;
	

	for(int i=0; i<sentenceContents->size(); i++)
	{
		int w;
		if(forwardPropogationSentenceData.parseSentenceReverse)
		{
			w = sentenceContents->size()-1-i;
		}
		else
		{
			w = i;
		}
		
		if(!propagateWordThroughNetworkIntro(translatorVariables, w, &forwardPropogationSentenceData, 0, activationPathWordFirstParseTreeGroup))
		{
			result = false;
		}
	}
	
	#ifdef SANI_SOLIDIFY_NET_BACKPROP
	SANIpropagateOperations.resetAllNeuronComponents(forwardPropogationSentenceData.SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY);
	#endif
	
	if(forwardPropogationSentenceData.toplevelGroupActivationFound)
	{
		sentenceValidActivationPath = true;
		result = true;
		
		#ifdef SANI_PARSE_SAVE_PARSE_TREE
		*topLevelParseTreeGroup = forwardPropogationSentenceData.topLevelParseTreeGroupPropagate;
		//cout << "forwardPropogationSentenceData->topLevelParseTreeGroupPropagate" << endl;
		
		//#ifdef SANI_DEBUG_PROPAGATE
		#ifdef SANI_DEBUG_RULES_PRINT_PARSE_PROCESS_POS_TYPES
		cout << "topLevelGroup" << endl;
		printBackpropParseTree(&(forwardPropogationSentenceData.topLevelParseTreeGroupPropagate), 1);
		cout << "end printBackpropParseTree" << endl;
		//cout << "forwardPropogationSentenceData->topLevelParseTreeGroupPropagate->groupName = " << forwardPropogationSentenceData->topLevelParseTreeGroupPropagate->groupName << endl;
		#endif	
		#endif	
		
		#ifdef SANI_PARSE_PERFORMANCE
		*performance = forwardPropogationSentenceData.performance;
		//cout << "forwardPropogationSentenceData.performance = " << forwardPropogationSentenceData.performance << endl;
		#else
		*performance = 1;
		#endif
		
		/*
		cout << "DEBUG: finished executing SANIpropagateLightOptimisedClass::executePosRelTranslatorNeuralNetwork (exiting)" << endl;
		exit(EXIT_ERROR);
		*/
	}
	else
	{
		*performance = 0;		
	}
	
	#ifndef SANI_REPLICATE_TOP_LEVEL_PARSE_TREE	
	if(sentenceValidActivationPath)
	{
		SANIpropagateOperationsParseTree.traceBackpropParseTreeSetTraced(&(forwardPropogationSentenceData.topLevelParseTreeGroupPropagate), 1);	//added GIA3g6aTEMP32 - set all parseTreeGroup groups in final heirachy to neuronTraced to prevent their memory from being deleted during SANIpropagateOperations.resetAllNeuronComponents
		SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);
		SANIpropagateOperationsParseTree.resetNeuronBackprop(&(forwardPropogationSentenceData.topLevelParseTreeGroupPropagate), GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);	//added GIA3g6aTEMP32 
		
		/*
		for(int w=0; w<sentenceContents->size(); w++)
		{
			SANIForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData.forwardPropogationWordDataArray[w]);
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
	cout << "DEBUG: finished executing SANIpropagateLightOptimisedClass::executePosRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	#ifdef SANI_FREE_MEMORY
	SANIpropagateOperationsParseTree.deinitialiseParseTreeGroupList(SANIGroupTypes, &(forwardPropogationSentenceData.parseTreeGroupListPropagate));
	#endif
	#ifdef SANI_FREE_MEMORY
	for(int i2=0; i2<(forwardPropogationSentenceData.parseTreeComponentListExtraPropagate).size(); i2++)
	{
		SANIComponentParseTree* parseTreeComponentTemp = (forwardPropogationSentenceData.parseTreeComponentListExtraPropagate)[i2];
		delete parseTreeComponentTemp;
	}
	(forwardPropogationSentenceData.parseTreeComponentListExtraPropagate).clear();
	#endif
	#ifdef SANI_FREE_MEMORY
	for(int w=0; w<sentenceContents->size(); w++)
	{
		SANIForwardPropogationWordData* forwardPropogationWordData = forwardPropogationSentenceData.forwardPropogationWordDataArray[w];
		delete forwardPropogationWordData;
	}
	#endif
	
	return result;
}

	


bool SANIpropagateLightOptimisedClass::propagateWordThroughNetworkIntro(SANItranslatorVariablesClass* translatorVariables, int w, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup)
{
	bool result = false;

	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	SANIpropagateOperations.resetAllNeuronComponents(forwardPropogationSentenceData->SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_SAVE);
	SANIpropagateOperations.resetAllNeuronComponents(forwardPropogationSentenceData->SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED);
	#endif
	
	//cout << "w = " << w << endl;
	LRPpreprocessorPlainTextWord* currentWord = forwardPropogationSentenceData->sentenceContents->at(w);

	SANInodes.printParseTreeDebugIndentation(layer);
	#ifdef SANI_DEBUG_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
	#ifndef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "currentWord->POSambiguityInfo = " << LRPpreprocessorPOStagger.printPOSambiguityInfo(currentWord->POSambiguityInfo) << endl;
	#endif

	//word specific variables:
	SANIForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);

	forwardPropogationWordData->wordReference = currentWord;
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
	#endif
	
	forwardPropogationWordData->w = w;
	//cout << "forwardPropogationWordData->previousWordConnections.size() = " << forwardPropogationWordData->previousWordConnections.size() << endl;
	
	//group specific variables:
	SANIForwardPropogationSignalData forwardPropogationSignalData;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	forwardPropogationSignalData.wordNounVariantType = currentWord->wordNounVariantGrammaticalTenseForm;
	forwardPropogationSignalData.wordVerbVariantType = currentWord->wordVerbVariantGrammaticalTenseForm;
	#endif
				
	string wordLowerCase = SHAREDvars.convertStringToLowerCase(&(currentWord->tagName));
		
	
			
	SANIGroupNeuralNetwork* inputLayerGroup = NULL;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_STRING_EXPLICIT_CASE_INSENSITIVE
	if(SANIformation.findWordInGroupMap(wordLowerCase, SANIformation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef SANI_DEBUG_PROPAGATE
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "SANIpropagateLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
	}
	#else
	if(SANIformation.findWordInGroupMap(wordLowerCase, SANIformation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef SANI_DEBUG_PROPAGATE
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "SANIpropagateLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
	}
	else if(SANIformation.findWordInGroupMap(currentWord->tagName, SANIformation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef SANI_DEBUG_PROPAGATE
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "SANIpropagateLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
		{
			result = true;
		}
	}
	#endif

	#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	bool currentWordIsExplicitPOStypeTemp = false;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_EXPLICITWORDTEMP)
	{
		currentWordIsExplicitPOStypeTemp = true;
	}
	#else
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, LRP_PREPROCESSOR_POS_TYPE_EXPLICITWORDTEMP))	//CHECKTHIS
	{
		currentWordIsExplicitPOStypeTemp = true;
	}
	#endif
	if(!currentWordIsExplicitPOStypeTemp)
	{
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
						if(currentGroupInInputLayerSectionTokensLayerClassTypeInstance->GIAtokenLayerClassTypeInstanceName == wordLowerCase)	//NB this implementation is synced with SANIrulesClass::isClassTag: assume tokens always comprise wordLowerCase
						{
							string GIAtokenLayerClassName = currentGroupInInputLayerSectionTokensLayerClass->GIAtokenLayerClassName;
							int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;
							if(SHAREDvars.textInTextArray(GIAtokenLayerClassName, LRPpreprocessorPOStypeNameArray, LRP_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES, &wordPOStype))
							{
								forwardPropogationWordData->wordPOStype = wordPOStype;
							}
							else
							{
								cerr << "SANIpropagateLightOptimisedClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
								exit(EXIT_ERROR);
							}

							#ifdef SANI_DEBUG_PROPAGATE
							SANInodes.printParseTreeDebugIndentation(layer);
							cout << "SANIpropagateLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
							#endif
							if(propagateWordThroughNetworkGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClassType, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
							{
								result = true;
							}
							if(propagateWordThroughNetworkGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClass, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))	//what is this for?
							{
								result = true;
							}
						}
					}
				}
			}
		}


		if(!SANInodes.currentWordPOSunknown(currentWord))
		{
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
			int wordPOStype = currentWord->unambiguousPOSindex;
			#else
			#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
			forwardPropogationSignalData.firstPOSval = true;
			#endif
			for(int wordPOStype=0; wordPOStype<LRP_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
			{
				if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, wordPOStype))
				{
			#endif
					if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
					{
						result = true;
					}

			#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
			#else
					#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
					forwardPropogationSignalData.firstPOSval = false;
					#endif
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
				if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
				{
					result = true;
				}
			}
			#else
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
			int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_NOUN;
			if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
			{
				result = true;
			}	
			#endif
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
			if(LRPpreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
			{	
				int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT;
				if(propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
				{
					result = true;
				}
			}
			#endif
			#endif	
		}
		#endif
		
	#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	}
	#endif
	
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	SANIpropagateOperations.resetAllNeuronComponents(forwardPropogationSentenceData->SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROPAGATED_RESET);
	#endif
	
	#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
	SANIpropagateOperations.resetAllNeuronComponents(forwardPropogationSentenceData->SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_PROCESSED);
	#endif
	
		
	return result;
}

	
bool SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroupInit(SANItranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	SANIGroupNeuralNetwork* inputLayerGroup = SANIformation.getInputGroupLayerSection(SANIformation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef SANI_DEBUG_PROPAGATE
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "SANIpropagateLightOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = wordPOStype;
	#endif
	
	if(propagateWordThroughNetworkGroupInit(translatorVariables, w, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}	
	
bool SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroupInit(SANItranslatorVariablesClass* translatorVariables, const int w, SANIGroupNeuralNetwork* inputLayerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	inputLayerGroup->neuronPropagated = false;
	
	if(propagateWordThroughNetworkGroupIntro(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}				
				

bool SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroupIntro(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup)
{
	bool result = false;
	
	if(propagateWordThroughNetworkGroup(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
	{
		result = true;
	}
	
	return result;
}

bool SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroup(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
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
		
		bool preprocessSearchForNonResetActivations = false;
		#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS
		preprocessSearchForNonResetActivations = true;
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, preprocessSearchForNonResetActivations))
		{
			result = true;
		}
		#endif

		preprocessSearchForNonResetActivations = false;
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, preprocessSearchForNonResetActivations))
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
		
bool SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroupSelect(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool preprocessSearchForNonResetActivations)
{
	bool result = false;
	
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY
	//int mostRecentContributionWordIndex = INT_DEFAULT_VALUE;	//not required with current implementation (removeLessRecentContributionIndices takes care of it)
	#endif

	for(int i=0; i<group->SANIfrontComponentConnectionList.size(); i++)
	{
		#ifndef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
		{
		#endif
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
			
			#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
			if(isWordInGroupOptimumPathwayWordList(forwardPropogationSentenceData, currentComponent, forwardPropogationWordData))
			{
			#endif
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
					if(SANIpropagateOperations.componentTests1(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))	//static component tests (component activations irrelevant)
					{
					#endif

						//cout << "currentComponent->componentType = " << currentComponent->componentType << endl;
						if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
						{
							currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
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

						#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT
						#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESTORE
						SANIGroupNeuralNetwork* ownerGroupOrig = SANInodes.copyGroup(ownerGroup);
						SANIGroupParseTree* currentParseTreeGroupOrig = ownerGroupOrig->currentParseTreeGroupTemp;
						#endif

						int indexOfFirstComponentInGroup;
						int indexOfLastComponentInGroup;
						if(forwardPropogationSentenceData->parseSentenceReverse)
						{
							indexOfFirstComponentInGroup = ownerGroup->components.size()-1;
							indexOfLastComponentInGroup = 0;
						}
						else
						{
							indexOfFirstComponentInGroup = 0;
							indexOfLastComponentInGroup = ownerGroup->components.size()-1;  			    
						}
						SANIComponentNeuralNetwork* firstComponent = (ownerGroup->components)[indexOfFirstComponentInGroup];
						SANIComponentNeuralNetwork* lastComponent = (ownerGroup->components)[indexOfLastComponentInGroup];

						int indexOfSecondComponentInGroup = INT_DEFAULT_VALUE;
						SANIComponentNeuralNetwork* secondComponent = NULL;
						if(ownerGroup->components.size() >= 1)
						{
							if(forwardPropogationSentenceData->parseSentenceReverse)
							{
								indexOfSecondComponentInGroup = indexOfFirstComponentInGroup-1;
							}
							else
							{
								indexOfSecondComponentInGroup = 1;
							}
							secondComponent = (ownerGroup->components)[indexOfSecondComponentInGroup];
						}

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

						bool measureSequentialActivationOnly = false;
						bool sequentialActivationFound = false;
						bool existingActivationFoundStartComponent = false;	
						bool resetUnallowed = false;

						#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS
						if(preprocessSearchForNonResetActivations)
						{
							measureSequentialActivationOnly = true;
							sequentialActivationFound = false;
							existingActivationFoundStartComponent = false;
							resetUnallowed = false;
							propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, measureSequentialActivationOnly, &sequentialActivationFound, &existingActivationFoundStartComponent, &resetUnallowed, &debug);
							if(sequentialActivationFound)
							{
								#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_RESET
								ownerGroup->nonResetActivationFoundDuringPreprocess = true;
								#endif
								#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
								if(!existingActivationFoundStartComponent)
								{
									ownerGroup->newActivationFoundDuringPreprocess = true;
									//cout << "SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroupSelect{}: ownerGroup->newActivationFoundDuringPreprocess" << endl;
								}
								#endif
							}
						}
						else
						{
						#endif
							#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
							bool nonResetActivationFoundDuringPreprocessLocal = false;
							bool newActivationFoundDuringPreprocessLocal = false;

							measureSequentialActivationOnly = true;
							sequentialActivationFound = false;
							existingActivationFoundStartComponent = false;
							resetUnallowed = false;	
							propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, measureSequentialActivationOnly, &sequentialActivationFound, &existingActivationFoundStartComponent, &resetUnallowed, &debug);
							if(sequentialActivationFound)
							{
								#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_RESET
								nonResetActivationFoundDuringPreprocessLocal = true;
								#endif
								#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
								if(!existingActivationFoundStartComponent)
								{
									newActivationFoundDuringPreprocessLocal = true;
								}
								#endif
							}
							#endif

							//changed 3g5a (introduce more simple reset detection method);
							bool requireResetGroupActivation = false;
							bool requireResetGroupActivationGroupWasActive = false;
							if(firstComponent == ownerComponent)
							{

								bool firstMandatoryComponentActive = false;
								if(firstComponent->neuronComponentConnectionActive)
								{
									firstMandatoryComponentActive = true;
								}
								#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_ENSURE_FIRST_COMPONENT_ACTIVE_NEW
								else if(secondComponent != NULL)
								{
									if((firstComponent->optional) && (secondComponent->neuronComponentConnectionActive))
									{
										firstMandatoryComponentActive = true;		//changed @GIA3g11aTEMP70
									}
								}
								#endif

								if(firstMandatoryComponentActive)
								{
									if(SANIpropagateOperations.doesRequireResetGroupActivation(forwardPropogationSentenceData, firstComponent, indexOfFirstComponentInGroup, ownerGroup, forwardPropogationWordData))
									{
										requireResetGroupActivation = true;
										//cout << "requireResetGroupActivation" << endl;
									}
								}
							}
							else if(ownerGroup->components.size() >= 1)
							{
								if((secondComponent == ownerComponent) && (firstComponent->optional))
								{
									#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_ENSURE_FIRST_COMPONENT_ACTIVE_NEW
									if((secondComponent->neuronComponentConnectionActive) || (firstComponent->neuronComponentConnectionActive))
									#else
									if(secondComponent->neuronComponentConnectionActive)
									#endif
									{
										if(SANIpropagateOperations.doesRequireResetGroupActivation(forwardPropogationSentenceData, secondComponent, indexOfSecondComponentInGroup, ownerGroup, forwardPropogationWordData))
										{
											requireResetGroupActivation = true;
										}
									}
								}
							}


							#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD
							if(requireResetGroupActivation)
							{	
								#ifdef SANI_DEBUG_PROPAGATE_EXTRA9
								cout << "resetUnallowed = " << resetUnallowed << endl;
								#endif

								#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
								if(!(ownerGroup->neuronPropagated))	//CHECKTHIS
								{
								#endif	
									#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS
									#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_RESET
									if(ownerGroup->nonResetActivationFoundDuringPreprocess)
									{
										//cout << "nonResetActivationFoundDuringPreprocess" << endl;
										requireResetGroupActivation = false;
									}
									#endif
									#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
									if(resetUnallowed)
									{
										//cout << "ownerGroup->resetUnallowed" << endl;
										//exit(EXIT_ERROR);
										requireResetGroupActivation = false;
									}
									#endif
									#endif

									#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
									if(ownerGroup->neuronProcessed)	//note if sequentialActivationFound && existingActivationFoundStartComponent during preprocessSearchForNonResetActivations, then ownerGroup->neuronProcessed
									{
										requireResetGroupActivation = false;
										//cout << "ownerGroup->neuronProcessed" << endl;
									}		
									#endif
								#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
								}
								#endif
							}
							#endif

							if(requireResetGroupActivation)
							{
								requireResetGroupActivationGroupWasActive = ownerGroup->neuronActive;

								#ifdef SANI_DEBUG_PROPAGATE_EXTRA4
								cout << "resetGroupActivation - " << ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;
								#endif
								bool clearParseTreeGroup = false;
								#ifndef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESTORE
								if(!(ownerGroup->neuronActive))
								{
									clearParseTreeGroup = true;
								}
								#endif

								#ifdef SANI_FREE_MEMORY
								clearParseTreeGroup = false;
								#endif
								SANIpropagateOperations.resetGroupActivation(ownerGroup);
								SANIpropagateOperationsParseTree.resetGroupParseTreeGroupRef(ownerGroup, clearParseTreeGroup);
								#ifdef SANI_FREE_MEMORY
								(forwardPropogationSentenceData->parseTreeGroupListPropagate).push_back(ownerGroup->currentParseTreeGroupTemp);
								#endif
							}
							#endif


							/*
							#ifdef SANI_SOLIDIFY_NET_BACKPROP
							SANIGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;
							if(currentParseTreeGroupTemp->solidified)
							{
								cout << "solidified" << endl;
								exit(EXIT_ERROR);
							}
							else
							{
							#endif
							*/

							measureSequentialActivationOnly = false;
							sequentialActivationFound = false;
							existingActivationFoundStartComponent = false;
							resetUnallowed = false;	//irrelevant
							if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, measureSequentialActivationOnly, &sequentialActivationFound, &existingActivationFoundStartComponent, &resetUnallowed, &debug))
							{
								result = true;
								#ifndef SANI_FREE_MEMORY
								#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESTORE
								if(requireResetGroupActivation)
								{
									if(!requireResetGroupActivationGroupWasActive)
									{
										SANInodes.deleteGroup(currentParseTreeGroupOrig);
									}
								}
								#endif
								#endif
							}
							else
							{
								#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESTORE
								if(requireResetGroupActivation)
								{
									#ifdef SANI_DEBUG_PROPAGATE_EXTRA2
									cout << "restoreGroupActivation - " << ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;
									#endif
									SANIpropagateOperations.restoreGroupActivation(ownerGroup, ownerGroupOrig);
									#ifdef SANI_FREE_MEMORY
									ownerGroup->currentParseTreeGroupTemp = currentParseTreeGroupOrig;
									#else
									SANIpropagateOperationsParseTree.restoreGroupParseTreeGroupRef(ownerGroup, currentParseTreeGroupOrig);
									#endif
								}
								#endif
							}

							/*
							#ifdef SANI_SOLIDIFY_NET_BACKPROP
							}
							#endif
							*/

							#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_RESET
							//if(nonResetActivationFoundDuringPreprocessLocal)
							//{
								ownerGroup->nonResetActivationFoundDuringPreprocess = false;
							//}
							#endif
							#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
							if(newActivationFoundDuringPreprocessLocal)
							{
								ownerGroup->newActivationFoundDuringPreprocess = false;
							}
							#endif

						#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS
						}
						#endif

						#ifdef SANI_FREE_MEMORY
						#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESTORE
						SANInodes.deleteGroup(ownerGroupOrig);
						#endif
						#endif

					#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
					}
					#endif
				#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
				}
				#endif
			#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
			}
			#endif
		#ifndef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
		}
		#endif
	}
	
	return result;
}	
	


bool SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool measureSequentialActivationOnly, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent, bool* resetUnallowed, const SANIposRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFoundAndExistingActivationFound = false;

	/*	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	SANInodes.printComponent(currentComponent, layer+1);
	#endif
	*/
	
	/*
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSTHOC	
	vector<SANIForwardPropogationWordData*> previousWordConnectionsOrig = forwardPropogationWordData->previousWordConnections;
	#endif
	*/
	
	SANIComponentNeuralNetwork* previousActiveComponent = NULL;	//NOTUSED
	SANIComponentNeuralNetwork* finalActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;	//NOTUSED
	
	if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, ownerComponent, &(ownerGroup->components), &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &finalActiveComponent, existingActivationFoundStartComponent))
	{	
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
		SANInodes.printParseTreeDebugIndentation(layer+1);
		cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << " - PASS1" << endl;
		SANInodes.printComponent(currentComponent, layer+1);
		#endif
	
		//cout << "existingActivationFoundStartComponent = " << *existingActivationFoundStartComponent << endl;
		bool subComponentsPassed = true;
		bool repeatDetected = false;
		if(currentComponent->isSubcomponent)
		{
			subComponentsPassed = false;
			//*existingActivationFoundStartComponent = false;	//added GIA3g11aTEMP62
			if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
			{
				if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(forwardPropogationWordData, forwardPropogationSentenceData, currentComponent, &(ownerComponent->subComponents), existingActivationFoundStartComponent))
				{
					subComponentsPassed = true;
					#ifdef SANI_DEBUG_PROPAGATE
					//cout << "SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(forwardPropogationWordData, forwardPropogationSentenceData, currentComponent, &(ownerComponent->subComponents), &repeatDetected, existingActivationFoundStartComponent))
				{
					subComponentsPassed = true;
					#ifdef SANI_DEBUG_PROPAGATE
					//cout << "SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
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
			#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
			SANInodes.printParseTreeDebugIndentation(layer+1);
			cout << "3b: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
			#endif
			
			#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
			if(componentTests2(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted))
			{
			#endif
				#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
				SANInodes.printParseTreeDebugIndentation(layer+1);
				cout << "3c: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
				#endif
				
				#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
				//determines if reset is allowed
				/*//OLD method;
				if(measureSequentialActivationOnly)
				{
					*resetUnallowed = false;
					if(!resetAllowed(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, *existingActivationFoundStartComponent, forwardPropogationSentenceData))
					{
						*resetUnallowed = true;
					}
				}
				*/
				*resetUnallowed = false;
				if(resetAllowed(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, *existingActivationFoundStartComponent, forwardPropogationSentenceData, firstActiveComponentInGroup, ownerComponent))
				{
				#endif
					#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
					SANInodes.printParseTreeDebugIndentation(layer+1);
					cout << "3c1: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
					#endif
					
					#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
					if(SANIpropagateOperations.componentWordConnectivityTestsWrapper(forwardPropogationWordData, forwardPropogationSentenceData, ownerGroup, activationPathWordCurrentParseTreeGroup, *existingActivationFoundStartComponent))
					{
					#endif
						#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
						SANInodes.printParseTreeDebugIndentation(layer+1);
						cout << "3c2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
						#endif

						#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
						if(verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree(forwardPropogationSentenceData, ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, *existingActivationFoundStartComponent))
						{
						#endif
							#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
							SANInodes.printParseTreeDebugIndentation(layer+1);
							cout << "3d: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
							#endif
							#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_MAX_NUMBER_OF_WORDS_IN_PARSETREE
							if(existingActivationVerifyMaxNumberWordsInParseTree(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationSentenceData, layer, *existingActivationFoundStartComponent))
							{
							#endif
								#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
								SANInodes.printParseTreeDebugIndentation(layer+1);
								cout << "3d2: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
								#endif
								
								#ifdef SANI_VERIFY_OWNER_GROUP_DOESNT_CONTAIN_NEWLY_ACTIVATED_COMPONENT_IN_ITS_PARSE_TREE
								if(verifyOwnerParseTreeGroupDoesntContainNewActivationComponentParseTree(ownerGroup, activationPathWordCurrentParseTreeGroup, layer))
								{
								#endif
									#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
									SANInodes.printParseTreeDebugIndentation(layer+1);
									cout << "3d3: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
									#endif

									*sequentialActivationFound = true;
									#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
									if(!measureSequentialActivationOnly)
									{
									#endif
										#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
										if(!(*existingActivationFoundStartComponent && ownerGroup->newActivationFoundDuringPreprocess))
										{
										#endif
											if(propagateWordThroughNetworkGroupComponent(translatorVariables, group, currentComponent, ownerGroup, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, groupFrontComponentConnectionListIndex, *existingActivationFoundStartComponent, debug))
											{
												result = true;
											}
										#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_COMPONENT_WHEN_NEW_COMPONENT_AVAILABLE
										}
										else
										{
											cout << "SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroupComponentWrapper{}: existingActivationFoundStartComponent && ownerGroup->newActivationFoundDuringPreprocess" << endl;
										}
										#endif
									#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
									}
									#endif
								#ifdef SANI_VERIFY_OWNER_GROUP_DOESNT_CONTAIN_NEWLY_ACTIVATED_COMPONENT_IN_ITS_PARSE_TREE
								}
								#endif

							#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_MAX_NUMBER_OF_WORDS_IN_PARSETREE
							}
							#endif
						#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
						}
						#endif
					#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
					}
					#endif
				#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
				}
				else
				{
					*resetUnallowed = true;
				}
				#endif
			#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
			}
			#endif
		}
	}
	else
	{
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
		SANInodes.printParseTreeDebugIndentation(layer+1);
		cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << " - FAIL1" << endl;	
		SANInodes.printComponent(currentComponent, layer+1);
		#endif	
	}
		
	#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS_MOD
	if(!resetAllowed(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, *existingActivationFoundStartComponent, forwardPropogationSentenceData, firstActiveComponentInGroup, ownerComponent))
	{
		*resetUnallowed = true;
	}
	#endif
				
	
	/*
	#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSTHOC	
	if(!result)
	{
		forwardPropogationWordData->previousWordConnections = previousWordConnectionsOrig;
	}
	#endif
	*/
	
	return result;
}
					
	
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroupComponent(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* ownerComponent, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFoundStartComponent, const SANIposRelTranslatorDebug* debug)
{
	bool result = true;
	
	ownerComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
	ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	
	if(forwardPropogationWordData->wordReference->translatorSentenceWordIndex < 0)
	{
		cout << "forwardPropogationWordData->wordReference->translatorSentenceWordIndex = " << forwardPropogationWordData->wordReference->translatorSentenceWordIndex << endl;
		exit(EXIT_ERROR);
	}
	
	#ifdef SANI_PARSE
	//create new parseTreeGroup
	//copied from SANIpropagateInverseClass::generateRulesGroupTreeComponents;
	SANIComponentParseTree* newParseComponent = SANInodes.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new SANIComponentParseTree(*SANIpropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
	newParseComponent->componentRef = currentComponent;
	newParseComponent->neuronComponentConnectionActive = true;	//implied
	newParseComponent->neuronComponentConnectionActiveWordRecord = currentComponent->neuronComponentConnectionActiveWordRecord;
	#ifdef SANI_PARSE_GENERATE_PARSE_TREE
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	#endif
	#ifdef SANI_PARSE_SIMULTANEOUS_BIO
	if(newParseComponent->parserForwardPropogationSignalData != NULL)
	{
		delete newParseComponent->parserForwardPropogationSignalData;
	}
	newParseComponent->parserForwardPropogationSignalData = new GIAposRelTranslatorParserForwardPropogationSignalData(forwardPropogationSignalData->parserForwardPropogationSignalData);
	#endif
	#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	newParseComponent->wordIndex = forwardPropogationWordData->w;
	#endif
	newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_STREAMLINED
	forwardPropogationWordData->wordReference->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous words in current parse tree	//!SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC: this will be set later by GIAposRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
	#else
	//DOESNTWORK;
	if(newParseComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		if(newParseComponent->candidateStringMatch != NULL)
		{	
			cout << "forwardPropogationWordData->wordPOStype = " << forwardPropogationWordData->wordPOStype << endl;
			newParseComponent->candidateStringMatch->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//note this is only compatible with PARSE_SIMULTANEOUS because there is no guarantee the candidateStringMatch->wordPOStypeInferred from a good/successful propagation will not be overwritten with the data from a future bad/unsuccessful propagation
		}
	}
	#endif
	#endif
	#ifdef SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	if(currentComponent->semanticRelationReturnConnectionDynamic)
	{
		
	}
	#endif
	SANIGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;


	#ifdef SANI_ALLOW_MULTIPLE_ACTIVATIONS
	if(existingActivationFoundStartComponent)
	{
		#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_TAKE_LAST_ACTIVATION_AS_PARSETREE
		SANIComponentParseTree* oldParseComponent = NULL;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			//currentParseTreeGroupTemp->components.pop_front();
			//cout << "ownerGroup->components.size() = " << ownerGroup->components.size() << endl;
			//cout << "currentParseTreeGroupTemp->components.size() = " << currentParseTreeGroupTemp->components.size() << endl;
			oldParseComponent = currentParseTreeGroupTemp->components.front();
			currentParseTreeGroupTemp->components.erase(currentParseTreeGroupTemp->components.begin());
		}
		else
		{
			oldParseComponent = currentParseTreeGroupTemp->components.back();
			currentParseTreeGroupTemp->components.pop_back();
		}
		//delete oldParseComponent;
		#ifdef SANI_FREE_MEMORY
		(forwardPropogationSentenceData->parseTreeComponentListExtraPropagate).push_back(oldParseComponent);	//mark overwritten component for deletion
		#endif
		#else
		cerr << "SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroupComponent: SANI_ALLOW_MULTIPLE_ACTIVATIONS: !SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_TAKE_LAST_ACTIVATION_AS_PARSETREE is not coded (planned implementation: store multiple parseTrees for each component activation)" << endl; 
		#endif
		
		#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
		if(currentParseTreeGroupTemp->components.size() > 0)
		{
			SANIComponentParseTree* previousParseComponent = NULL;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				previousParseComponent = currentParseTreeGroupTemp->components.front();
				currentParseTreeGroupTemp->parseTreeMinWordIndex = previousParseComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;	//CHECKTHIS
			}
			else
			{
				previousParseComponent = currentParseTreeGroupTemp->components.back();
				currentParseTreeGroupTemp->parseTreeMaxWordIndex = previousParseComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;	//CHECKTHIS
			}
			//SANIpropagateOperationsParseTree.updateParseTreeMaxMinWordIndexOfParent(currentParseTreeGroupTemp, previousParseComponent);
			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
			SANIGroupParseTree* childGroup = previousParseComponent->parseTreeGroupRef;
			if(childGroup != NULL)
			{
				currentParseTreeGroupTemp->parseTreeTotalWeight = currentParseTreeGroupTemp->parseTreeTotalWeight - childGroup->parseTreeTotalWeight;
			}
			#endif
		}
		else
		{
			currentParseTreeGroupTemp->parseTreeMaxWordIndex = INT_DEFAULT_VALUE;
			currentParseTreeGroupTemp->parseTreeMinWordIndex = INT_DEFAULT_VALUE;
			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
			currentParseTreeGroupTemp->parseTreeMaxWeight = 0.0;
			currentParseTreeGroupTemp->parseTreeMinWeight = 0.0;
			currentParseTreeGroupTemp->parseTreeTotalWeight = 0.0;
			#endif
		}
		#endif
	}
	#endif
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		//currentParseTreeGroupTemp->components.push_front(newParseComponent);
		currentParseTreeGroupTemp->components.insert(currentParseTreeGroupTemp->components.begin(), newParseComponent);
	}
	else
	{
		currentParseTreeGroupTemp->components.push_back(newParseComponent);
	}
	#endif
	
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	//cout << "currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex = " << currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex << endl;
	if(currentParseTreeGroupTemp->parseTreeMaxWordIndex == INT_DEFAULT_VALUE)
	{
		currentParseTreeGroupTemp->parseTreeMaxWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		currentParseTreeGroupTemp->parseTreeMinWordIndex = currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		currentParseTreeGroupTemp->parseTreeMaxWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeMinWeight = ownerGroup->groupWeight;
		currentParseTreeGroupTemp->parseTreeTotalWeight = ownerGroup->groupWeight;
		#endif
	}
	SANIpropagateOperationsParseTree.updateParseTreeMaxMinWordIndexOfParent(currentParseTreeGroupTemp, newParseComponent, activationPathWordCurrentParseTreeGroup);
	#endif
	
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA4
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << ", activationSequenceCompleted = " << activationSequenceCompleted << endl;	
	SANInodes.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef SANI_PARSE
	SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = ownerGroup->currentParseTreeGroupTemp;
	#endif
	
	#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_BASIC
	ownerGroup->neuronProcessed = true;
	#endif
	

	#ifdef SANI_SOLIDIFY_NET_BACKPROP
	#ifdef SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
	//b) enforce solidification of previous active component subgroup
	if(activationPathWordCurrentParseTreeGroupOwner->components.size() >= 2)
	{
		int indexOfPreviousActiveComponent;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			indexOfPreviousActiveComponent = 0+1;
		}
		else
		{
			indexOfPreviousActiveComponent = activationPathWordCurrentParseTreeGroupOwner->components.size()-2;
		}
		SANIComponentParseTree* component = activationPathWordCurrentParseTreeGroupOwner->components[indexOfPreviousActiveComponent];
		#ifdef SANI_SOLIDIFY_BIO_WEAK1
		if(component->parseTreeGroupRef != NULL)
		{
			component->parseTreeGroupRef->solidified = true;
		}
		#else
		solidifyNeuralNetBackpropGroupLastComponent(forwardPropogationSentenceData, component->parseTreeGroupRef, layer); 
		#endif
	}
	#else
	#ifdef SANI_SOLIDIFY_BIO_WEAK3
	newParseComponent->solidified = true;
	#else
	newParseComponent->parseTreeGroupRef->solidified = true;
	#endif
	#endif
	#endif
					
	if(activationSequenceCompleted)
	{		
		ownerGroup->neuronActive = true;	//not used
		
		#ifdef SANI_PARSE_SIMULTANEOUS
		if(forwardPropogationSentenceData->parserEnabled)
		{
			//NO: only execute this function when reached the highest layer in activation tree;
			if(!SANIparser.generateSemanticRelationsFromTxtRelationsWrapperNeuralNetwork(translatorVariables, activationPathWordCurrentParseTreeGroupOwner, &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective), layer))
			{
				result = false;
			}
		}
		#endif
		
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA
		SANInodes.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif

		ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData = ownerGroup->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
		SANIForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = &(ownerGroup->semanticRelationReturnEntityForwardPropogationSignalData);

		bool topLevelGroup = SANInodesGroupClassObject.isTopLevelGroupType(ownerGroup->groupTypeName, ownerGroup->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
		if(topLevelGroup)
		{
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSTHOC
			int wComparisonInt;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				wComparisonInt = 0;	//start of sentence found
			}
			else
			{
				wComparisonInt = forwardPropogationSentenceData->sentenceContents->size()-1;	//end of sentence punctuation mark found
			}
			if(forwardPropogationWordData->w == wComparisonInt)
			{
			#endif
				#ifdef SANI_PARSE_PERFORMANCE
				if(SANIpropagateOperationsParseTree.updateAndVerifyPerformanceGroupSentence(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
				{
					//cout << "forwardPropogationSentenceData->performance = " << forwardPropogationSentenceData->performance << endl;
				#endif
				
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					int maxWeight;
					SANIpropagateOperationsParseTree.calculatePerformanceWeightOfParseTree(activationPathWordCurrentParseTreeGroupOwner, &maxWeight);
					/*
					cout << "maxWeight = " << maxWeight << endl;
					cout << "parseTreeMaxWeight2 = " << forwardPropogationSentenceData->parseTreeMaxWeightPropagate << endl;
					*/
					if(maxWeight >= forwardPropogationSentenceData->parseTreeMaxWeightPropagate)
					{
						forwardPropogationSentenceData->parseTreeMaxWeightPropagate = maxWeight;
					#endif
						/*
						cout << "topLevelGroup && SANIpropagateOperations.isSentenceWordDataFullyConnected; TEMP EXIT" << endl;
						exit(EXIT_ERROR);
						*/
	
						//result = true;
						forwardPropogationSentenceData->finishedPassingSentenceWords = true;
	
						forwardPropogationSentenceData->toplevelGroupActivationFound = true;
						
						#ifdef SANI_PARSE_SAVE_PARSE_TREE
						#ifdef SANI_REPLICATE_TOP_LEVEL_PARSE_TREE
						SANInodes.deleteParseTree(forwardPropogationSentenceData->topLevelParseTreeGroupPropagate, 0);
						forwardPropogationSentenceData->topLevelParseTreeGroupPropagate = SANInodes.replicateParseTree(activationPathWordCurrentParseTreeGroupOwner, 0);
						#else
						//OLD: forwardPropogationSentenceData->topLevelParseTreeGroupPropagate = activationPathWordCurrentParseTreeGroupOwner;
						//copy currentParseTreeGroupTemp so it cant be overwritten;
						forwardPropogationSentenceData->topLevelParseTreeGroupPropagate = SANInodes.copyGroup(activationPathWordCurrentParseTreeGroupOwner);
						#endif
						#endif
						
						#ifdef SANI_DEBUG_RULES_PRINT_PARSE_PROCESS_POS_TYPES
						cout << "1topLevelGroup" << endl;
						#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
						cout << "parseTreeMaxWeight1 = " << forwardPropogationSentenceData->parseTreeMaxWeightPropagate << endl;
						#endif
						cout << "forwardPropogationSentenceData->topLevelParseTreeGroupPropagate->groupName = " << activationPathWordCurrentParseTreeGroupOwner->groupName << endl;
						printBackpropParseTree(activationPathWordCurrentParseTreeGroupOwner, 3);
						#endif
						/*
						cout << "topLevelGroup" << endl;
						cout << "finishedPassingSentenceWords (temp exit)" << endl;
						exit(0);
						*/
						
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					}
					else
					{
						//cout << "parseTreeMaxWeight2 = " << forwardPropogationSentenceData->parseTreeMaxWeightPropagate << endl;
					}
					#endif
				#ifdef SANI_PARSE_PERFORMANCE
				}
				#endif
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_POSTHOC
			}
			else
			{
				//cout << endl;
			}
			#endif
		}
		else
		{
			//cout << endl;
		
			#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
			ownerGroup->neuronPropagated = true;
			#endif
			
			//activation sequence completed (and not top level group), propagate next layer up
			if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))
			{
				//result = true;
			}
			
			#ifdef SANI_LIGHT_OPTIMISED_RESET_ONLY_ONCE_FOUND_FIRST_COMPONENT_RESET_ONCE_PER_WORD_SUPPORT_RECURSION
			ownerGroup->neuronPropagated = false;
			#endif
		}
	}

	return result;
}


bool SANIpropagateLightOptimisedClass::propagateWordThroughNetworkGroupComponentPassNextWord(SANItranslatorVariablesClass* translatorVariables, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner)
{
	bool result = false;
	
	int wNext;
	bool wNextComparison = false;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		wNext = forwardPropogationWordData->w - 1;
		if(wNext >= 0)
		{
			wNextComparison = true;
		}
	}
	else
	{
		wNext = forwardPropogationWordData->w + 1;
		if(wNext < forwardPropogationSentenceData->sentenceContents->size())
		{
			wNextComparison = true;
		}
	}
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
	cout << "forwardPropogationWordData->w+1 = " << forwardPropogationWordData->w+1 << endl;
	#endif
	
	bool passThroughNew = false;
	int wNextComparisonInt;
	
	if(wNextComparison)
	{
		if(propagateWordThroughNetworkIntro(translatorVariables, wNext, forwardPropogationSentenceData, layer+1, NULL))
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




#ifdef SANI_SOLIDIFY_NET_BACKPROP
#ifdef SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
#ifdef SANI_SOLIDIFY_BIO_WEAK3
bool SANIpropagateLightOptimisedClass::solidifyNeuralNetBackpropGroupLastComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* group, int level)
{
	bool result = true;

	if(group != NULL)
	{
		group->solidified = true;
		
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA9
		SANInodes.printParseTreeDebugIndentation(level);
		cout << "group->solidified: " << group->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;
		#endif
		
		if(group->components.size() > 0)
		{
			SANIComponentParseTree* lastComponent = getLastActiveComponent(group);
			solidifyNeuralNetBackpropGroupLastComponent(forwardPropogationSentenceData, lastComponent->groupRef, level+1);
		}
	}
		
	return result;
}
#else
bool SANIpropagateLightOptimisedClass::solidifyNeuralNetBackpropGroupLastComponent(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup, int level)
{
	bool result = true;

	if(currentParseTreeGroup != NULL)
	{
		currentParseTreeGroup->solidified = true;
		
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA9
		SANInodes.printParseTreeDebugIndentation(level);
		cout << "currentParseTreeGroup->solidified: " << currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;
		#endif
		
		if(currentParseTreeGroup->components.size() > 0)
		{
			int indexOfLastComponentInGroup;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				indexOfLastComponentInGroup = 0;
			}
			else
			{
				indexOfLastComponentInGroup = currentParseTreeGroup->components.size()-1;
			}
			SANIComponentParseTree* lastComponent = (currentParseTreeGroup->components)[indexOfLastComponentInGroup];
			solidifyNeuralNetBackpropGroupLastComponent(forwardPropogationSentenceData, lastComponent->parseTreeGroupRef, level+1);
		}
	}
		
	return result;
}
#endif
#endif
#endif


#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
bool SANIpropagateLightOptimisedClass::componentTests2(const SANIGroupNeuralNetwork* group, const SANIGroupParseTree* currentParseTreeGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted)
{
	bool componentTests = true;
	
	if(!forwardPropogationSentenceData->parseSentenceReverse || activationSequenceCompleted)
	{
		/*
		#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS_GLOBAL
		Algorithm:
		#else
		Algorithm:
		previousWordPOStype/existsPreceedingWordPOStype requirement: there exists a previous word POStype
		OLD:
		previousWordPOStype/existsPreceedingWordPOStype requirement: there exists a previous word POStype that was succesfully parsed by a higher order neuron/group.
			Question: so how will the program know this until the higher order neuron group has been parsed?
			Conclusion: will need to;
				a) record the wordPos type for every activated component in a group
				b) read both;
					i) previous components in current group and 
					ii) read up the tree to see if this condition is met (ie there exists a previous word POStype that was succesfully parsed by a higher order neuron/group)
		#endif
		*/
		int currentWordIndex = forwardPropogationWordData->w;
		if(currentWordIndex > 0)	//CHECKTHIS
		{
			if(group->previousWordPOStype != "")
			{
				componentTests = false;
				int previousWordPOStype = LRPpreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype);

				if(findPreviousWordInSentence(forwardPropogationSentenceData->sentenceContents, currentWordIndex, previousWordPOStype))		
				{
					//cout << "componentTests = true" << endl;
					componentTests = true;
				}
			}
			if(group->existsPreceedingWordPOStype != "")
			{
				componentTests = false;
				int existsPreceedingWordPOStype = LRPpreprocessorWordClassObject.getPOStypeFromName(group->existsPreceedingWordPOStype);

				if(findPreceedingWordInSentence(forwardPropogationSentenceData->sentenceContents, currentWordIndex, existsPreceedingWordPOStype))		
				{
					componentTests = true;
				}
			}
		}
	}
	
	return componentTests;
}

bool SANIpropagateLightOptimisedClass::findPreviousWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const int currentWordIndex, const int previousWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	LRPpreprocessorPlainTextWord* previousComponentWord = NULL;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		LRPpreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(w == currentWordIndex)
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
bool SANIpropagateLightOptimisedClass::findPreceedingWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const int currentWordIndex, const int existsPreceedingWordPOStype)
{
	bool result = false;
	
	bool stillReadingActivatedWords = true;
	for(int w=0; w<sentenceContents->size(); w++)
	{
		LRPpreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		if(stillReadingActivatedWords)
		{
			if(w == currentWordIndex)
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




#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
//determines if reset is allowed
bool SANIpropagateLightOptimisedClass::resetAllowed(constEffective SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool firstActiveComponentInGroup, const SANIComponentNeuralNetwork* ownerComponent)
{
	bool result = false;
	
	if((ownerGroup->groupName == "general") && (ownerGroup->groupTypeName == "referenceSets"))	//CHECKTHIS magic strings
	{
		if(existingActivatedComponentCapturesLastWordInSentence(ownerGroup, activationPathWordCurrentParseTreeGroup, forwardPropogationWordData, existingActivationFoundStartComponent, forwardPropogationSentenceData, firstActiveComponentInGroup))
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}
	
	#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
	//TODO: check this is the one and only place this check needs to take place
	if(existingActivationFoundStartComponent)
	{
		if(!(forwardPropogationSignalData->firstPOSval))
		{
			if(ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference)
			{
				//component has been activated by previous POS propagation
				result = false;
			}
		}
	}
	#endif
	
	return result;
}

//requires SANI_REVERSE_DIRECTION;
bool SANIpropagateLightOptimisedClass::existingActivatedComponentCapturesLastWordInSentence(constEffective SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool firstActiveComponentInGroup)
{
	bool result = false;

	const SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;

	SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef = NULL;
	SANIComponentParseTree* lastActiveComponentInParseTree = NULL;
	SANIGroupParseTree* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
	

	#ifdef SANI_SOLIDIFY_NET_BACKPROP
	#ifdef SANI_SOLIDIFY_BIO_WEAK3
	if(!(ownerGroup->solidified))
	#else
	if(!(ownerGroupParseTreeGroup->solidified))
	#endif
	{
	#endif
		if(ownerGroupParseTreeGroup->components.size() > 0)
		{
			#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS_MOD
			if(firstActiveComponentInGroup)
			#else
			if(existingActivationFoundStartComponent)
			#endif
			{
				//cout << "firstActiveComponentInGroup" << endl;
				
				int lastActiveComponentInParseTreeIndex;
				#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS_MOD
				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
				}
				else
				{
					lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
				}
				#else
				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					lastActiveComponentInParseTreeIndex = 0;
				}
				else
				{
					lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
				}
				#endif

				SANIComponentParseTree* lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];

				lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;

				if(existingActivatedComponentCapturesLastWordInSentence(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTreeParseTreeGroupRef, lastActiveComponentInParseTree, forwardPropogationWordData, existingActivationFoundStartComponent, forwardPropogationSentenceData))
				{
					result = true;
				}
			}
			else
			{
				//cout << "!firstActiveComponentInGroup" << endl;
				result = true;
			}
		}
		else
		{
			result = true;
		}
	#ifdef SANI_SOLIDIFY_NET_BACKPROP
	}
	else
	{
		//cout << "ownerGroupParseTreeGroup->solidified" << endl;
		result = true;
	}
	#endif
	
	return result;
}

bool SANIpropagateLightOptimisedClass::existingActivatedComponentCapturesLastWordInSentence(const SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, const SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, const SANIComponentParseTree* lastActiveComponentInParseTree, const SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	//cout << "\t SANIpropagateLightOptimisedClass::existingActivatedComponentCapturesLastWordInSentence{}: " << endl;
	
	bool result = false;
	
	bool calculateMaxOrMin;
	int wordIndexMax1;
	int wordIndexMax2;
	const SANIGroupParseTree* parseTreeGroupToFindWordIndexMax1;
	const SANIGroupParseTree* parseTreeGroupToFindWordIndexMax2;
	int wordIndexMaxMin1;
	int wordIndexMaxMin2;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		calculateMaxOrMin = true;
		wordIndexMax1 = -1;
		wordIndexMax2 = -1;
		parseTreeGroupToFindWordIndexMax1 = lastActiveComponentInParseTreeParseTreeGroupRef;
		parseTreeGroupToFindWordIndexMax2 = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
		wordIndexMaxMin1 = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;	//CHECKTHIS
		wordIndexMaxMin2 = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;	//CHECKTHIS
	}
	else
	{
		calculateMaxOrMin = false;
		wordIndexMax1 = REALLY_LARGE_INT;
		wordIndexMax2 = REALLY_LARGE_INT;
		parseTreeGroupToFindWordIndexMax1 = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef;
		parseTreeGroupToFindWordIndexMax2 = lastActiveComponentInParseTreeParseTreeGroupRef;
		wordIndexMaxMin1 = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		wordIndexMaxMin2 = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
	}

	if(parseTreeGroupToFindWordIndexMax1 != NULL)
	{
		if(!SANIpropagateOperationsParseTree.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax1, calculateMaxOrMin, &wordIndexMax1, 0))
		{
			//cout << "SANIpropagateOperationsClass::existingActivatedComponentCapturesLastWordInSentence error{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax1, )" << endl;	
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "SANIpropagateOperationsClass::existingActivatedComponentCapturesLastWordInSentence pass{}: !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax1, )" << endl;	
			//exit(EXIT_ERROR);
		}
	}
	wordIndexMax1 = SHAREDvars.maxInt(wordIndexMax1, wordIndexMaxMin1);
	
	if(parseTreeGroupToFindWordIndexMax2 != NULL)
	{
		if(!SANIpropagateOperationsParseTree.getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax2, calculateMaxOrMin, &wordIndexMax2, 0))
		{
			//newParseComponent->neuronComponentConnectionActiveWordRecord has not yet been defined
			//cout << "SANIpropagateOperationsClass::existingActivatedComponentCapturesLastWordInSentence error{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax2, ) && " << endl;
			//exit(EXIT_ERROR);
		}
		else
		{
			//cout << "SANIpropagateOperationsClass::existingActivatedComponentCapturesLastWordInSentence pass{} !getMinMaxWordIndexInParseTree(parseTreeGroupToFindWordIndexMax2, ) && " << endl;
			//exit(EXIT_ERROR);
		}
	}
	wordIndexMax2 = SHAREDvars.maxInt(wordIndexMax2, wordIndexMaxMin2);

	#ifdef SANI_DEBUG_PROPAGATE_EXTRA9
	cout << "wordIndexMax1 = " << wordIndexMax1 << endl;
	cout << "wordIndexMax2 = " << wordIndexMax2 << endl;
	//cout << "wordIndexMinMin = " << wordIndexMinMin << endl;
	cout << "forwardPropogationSentenceData->sentenceContents->size() = " << forwardPropogationSentenceData->sentenceContents->size() << endl;
	#endif
	
	int wordIndexMaxComparisonInt;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		wordIndexMaxComparisonInt = forwardPropogationSentenceData->sentenceContents->size()-1-1;		//ignore full stop
	}
	else
	{
		wordIndexMaxComparisonInt = 0;
	}
	if(wordIndexMax1 == wordIndexMaxComparisonInt)
	{
		//existing activated component's parseTree contains last word in sentence 
		if(wordIndexMax2 == wordIndexMaxComparisonInt)
		{
			//overwritten/newly activated component's parseTree contains last word in sentence 
			result = true;
		}
		
		/*
		#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS_MOD2
		if(wordIndexMax2 < wordIndexMax1-1)
		{
			result = true;	//entity refers to separate referenceSet:general group
		}
		#endif
		*/
	}
	else
	{
		result = true;
	}

	return result;
}
#endif


#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE

bool SANIpropagateLightOptimisedClass::verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent)
{
	bool result = false;

	if(existingActivationFoundStartComponent)
	{
		const SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = NULL;
		if(activationPathWordCurrentParseTreeGroup != NULL)
		{
			prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
		}
		
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
		cout << "SANIpropagateOperationsClass::verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree: existingActivationFoundStartComponent" << endl;
		#endif
		
		SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef = NULL;
		SANIComponentParseTree* lastActiveComponentInParseTree = NULL;
		SANIGroupParseTree* ownerGroupParseTreeGroup = ownerGroup->currentParseTreeGroupTemp;
		if(ownerGroupParseTreeGroup->components.size() > 0)
		{
			int lastActiveComponentInParseTreeIndex;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				lastActiveComponentInParseTreeIndex = 0;
			}
			else
			{
				lastActiveComponentInParseTreeIndex = ownerGroupParseTreeGroup->components.size() - 1;
			}
			lastActiveComponentInParseTree = (ownerGroupParseTreeGroup->components)[lastActiveComponentInParseTreeIndex];
			if(lastActiveComponentInParseTree->parseTreeGroupRef != NULL)
			{
				lastActiveComponentInParseTreeParseTreeGroupRef = lastActiveComponentInParseTree->parseTreeGroupRef;
			}
			
		#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
			int prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin;
			int prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax;
			int lastActiveComponentInParseTreeParseTreeWordIndexMin;
			int lastActiveComponentInParseTreeParseTreeWordIndexMax;
			if(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef != NULL)
			{
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->parseTreeMinWordIndex;
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax = prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->parseTreeMaxWordIndex;
			}
			else
			{
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
			}
			#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY_WEAK
			if(lastActiveComponentInParseTreeParseTreeGroupRef != NULL)
			{
				lastActiveComponentInParseTreeParseTreeWordIndexMin = lastActiveComponentInParseTreeParseTreeGroupRef->parseTreeMinWordIndex;
				lastActiveComponentInParseTreeParseTreeWordIndexMax = lastActiveComponentInParseTreeParseTreeGroupRef->parseTreeMaxWordIndex;
			}
			else
			{
				lastActiveComponentInParseTreeParseTreeWordIndexMin = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
				lastActiveComponentInParseTreeParseTreeWordIndexMax = lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
			}
			#else
			lastActiveComponentInParseTreeParseTreeWordIndexMin = ownerGroupParseTreeGroup->parseTreeMinWordIndex;
			lastActiveComponentInParseTreeParseTreeWordIndexMax = ownerGroupParseTreeGroup->parseTreeMaxWordIndex;			
			#endif	
			
			/*
			cout << "prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin << endl;
			cout << "prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax << endl;
			cout << "lastActiveComponentInParseTreeParseTreeWordIndexMin = " << lastActiveComponentInParseTreeParseTreeWordIndexMin << endl;
			cout << "lastActiveComponentInParseTreeParseTreeWordIndexMax = " << lastActiveComponentInParseTreeParseTreeWordIndexMax << endl;
			*/
			
			result = false;
			if((prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin <= lastActiveComponentInParseTreeParseTreeWordIndexMin) && (prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax >= lastActiveComponentInParseTreeParseTreeWordIndexMax))
			{
				result = true;
			}
			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
			if((lastActiveComponentInParseTreeParseTreeWordIndexMin == prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMin) && (lastActiveComponentInParseTreeParseTreeWordIndexMax == prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexMax))
			{
				bool foundMatchingSetOfLastActiveAndProspectiveActiveComponentParseTreeWords = true;
				if(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->groupWeight < lastActiveComponentInParseTreeParseTreeGroupRef->groupWeight)
				{
					//cout << "(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->groupWeight < lastActiveComponentInParseTreeParseTreeGroupRef->groupWeight)" << endl;
					result = false;	
				}
			}
			#endif
				
		#else

			vector<int> prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList;
			vector<int> lastActiveComponentInParseTreeParseTreeWordIndexList;
			if(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef != NULL)
			{
				getWordIndicesInParseTree(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, &prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList, 0);
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.push_back(forwardPropogationWordData->wordReference->translatorSentenceWordIndex);
				//prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.push_back(convertWordIndexToSentenceEntityIndex(forwardPropogationWordData->w));
			}
			else
			{
				prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.push_back(forwardPropogationWordData->wordReference->translatorSentenceWordIndex);
			}
			if(lastActiveComponentInParseTreeParseTreeGroupRef != NULL)
			{
				getWordIndicesInParseTree(lastActiveComponentInParseTreeParseTreeGroupRef, &lastActiveComponentInParseTreeParseTreeWordIndexList, 0);	
			}
			else
			{
				lastActiveComponentInParseTreeParseTreeWordIndexList.push_back(lastActiveComponentInParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex);
			}

			result = true;
			
			
			bool foundProspectiveNewlyActiveComponentParseTreeWordForEveryLastActiveComponentParseTreeWord = true;
			for(int i=0; i<lastActiveComponentInParseTreeParseTreeWordIndexList.size(); i++)
			{
				bool foundLastActiveComponentParseTreeWord = false;
				#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
				cout << "lastActiveComponentInParseTreeParseTreeWordIndexList[i] = " << lastActiveComponentInParseTreeParseTreeWordIndexList[i] << endl;
				#endif
				
				for(int j=0; j<prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.size(); j++)
				{
					#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
					cout << "prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] << endl;
					#endif
					
					if(prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] == lastActiveComponentInParseTreeParseTreeWordIndexList[i])
					{
						foundLastActiveComponentParseTreeWord = true;
						//cout << "SANIpropagateOperationsClass::verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree debug: foundLastActiveComponentParseTreeWord; prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] << endl;
					}
				}
				if(!foundLastActiveComponentParseTreeWord)
				{
					result = false;	
					foundProspectiveNewlyActiveComponentParseTreeWordForEveryLastActiveComponentParseTreeWord = false;
				}
			}
		
			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC
			if(foundProspectiveNewlyActiveComponentParseTreeWordForEveryLastActiveComponentParseTreeWord)
			{
				if((prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef != NULL) && (lastActiveComponentInParseTreeParseTreeGroupRef != NULL))
				{
					bool foundLastActiveComponentParseTreeWordForEveryProspectiveNewlyActiveComponentParseTreeWord = true;

					for(int j=0; j<prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList.size(); j++)
					{
						bool foundProspectiveActiveComponentParseTreeWord = false;

						for(int i=0; i<lastActiveComponentInParseTreeParseTreeWordIndexList.size(); i++)
						{
							if(prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] == lastActiveComponentInParseTreeParseTreeWordIndexList[i])
							{
								foundProspectiveActiveComponentParseTreeWord = true;
								//cout << "SANIpropagateOperationsClass::verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree debug: foundLastActiveComponentParseTreeWord; prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] = " << prospectiveNewlyActiveComponentInParseTreeParseTreeWordIndexList[j] << endl;
							}
						}
						if(!foundProspectiveActiveComponentParseTreeWord)
						{
							foundLastActiveComponentParseTreeWordForEveryProspectiveNewlyActiveComponentParseTreeWord = false;
						}
					}
					
					if(foundLastActiveComponentParseTreeWordForEveryProspectiveNewlyActiveComponentParseTreeWord)
					{
						bool foundMatchingSetOfLastActiveAndProspectiveActiveComponentParseTreeWords = true;
						if(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->groupWeight < lastActiveComponentInParseTreeParseTreeGroupRef->groupWeight)
						{
							//cout << "(prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef->groupWeight < lastActiveComponentInParseTreeParseTreeGroupRef->groupWeight)" << endl;
							result = false;	
						}
					}
				}
			}
			#endif
		#endif
		}
		else
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}	
		
	return result;
}

bool SANIpropagateLightOptimisedClass::getWordIndicesInParseTree(const SANIGroupParseTree* currentParseTreeGroup, vector<int>* wordIndexList, const int level)
{
	bool result = true;

	#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
	printParseTreeDebugIndentation(level);
	cout << "SANIpropagateOperationsClass::getWordIndicesInParseTree: " <<  currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;	
	#endif
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		wordIndexList->push_back(currentComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex); 
		
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
		printComponent(currentComponent, level);
		#endif
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{					
			if(getWordIndicesInParseTree(currentComponent->parseTreeGroupRef, wordIndexList, level+1))
			{
				result = true;
			}
		}
	}
	
	return result;	
}
#endif




	

#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_MAX_NUMBER_OF_WORDS_IN_PARSETREE
bool SANIpropagateLightOptimisedClass::existingActivationVerifyMaxNumberWordsInParseTree(const SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer, const bool existingActivationFoundStartComponent)
{
	bool result = true;

	if(existingActivationFoundStartComponent)
	{
		const SANIGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;
		SANIComponentParseTree* oldParseComponent = NULL;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			oldParseComponent = currentParseTreeGroupTemp->components.front();
		}
		else
		{
			oldParseComponent = currentParseTreeGroupTemp->components.back();
		}
		
		int oldParseComponentNumberOfWordsInParseTree = 0;
		if(oldParseComponent->parseTreeGroupRef == NULL)
		{
			oldParseComponentNumberOfWordsInParseTree = 1;
			//cout << "currentParseTreeGroupTemp->components.size() = " << currentParseTreeGroupTemp->components.size() << endl;
			//cerr << "SANIpropagateLightOptimisedClass::existingActivationVerifyMaxNumberWordsInParseTree error: (oldParseComponent->parseTreeGroupRef == NULL)" << endl;
			//exit(EXIT_ERROR);
		}
		else
		{
			oldParseComponentNumberOfWordsInParseTree = countNumberWordsInParseTree(oldParseComponent->parseTreeGroupRef, forwardPropogationSentenceData, layer);
		}
		
		int newParseComponentNumberOfWordsInParseTree = 0;
		if(activationPathWordCurrentParseTreeGroup == NULL)
		{
			newParseComponentNumberOfWordsInParseTree = 1;		
		}
		else
		{
			newParseComponentNumberOfWordsInParseTree = countNumberWordsInParseTree(activationPathWordCurrentParseTreeGroup, forwardPropogationSentenceData, layer);
		}
		
	
		result = false;

		#ifdef SANI_DEBUG_PROPAGATE_EXTRA9
		cout << "newParseComponentNumberOfWordsInParseTree = " << newParseComponentNumberOfWordsInParseTree << endl;
		cout << "oldParseComponentNumberOfWordsInParseTree = " << oldParseComponentNumberOfWordsInParseTree << endl;
		#endif
		
		if(newParseComponentNumberOfWordsInParseTree >= oldParseComponentNumberOfWordsInParseTree)
		{
			result = true;
			#ifdef SANI_DEBUG_PROPAGATE_EXTRA9
			cout << "SANIpropagateLightOptimisedClass::existingActivationVerifyMaxNumberWordsInParseTree debug: newParseComponentNumberOfWordsInParseTree >= oldParseComponentNumberOfWordsInParseTree" << endl;
			//exit(EXIT_ERROR);
			#endif
		}
		else
		{
			#ifdef SANI_DEBUG_PROPAGATE_EXTRA9
			cout << "SANIpropagateLightOptimisedClass::existingActivationVerifyMaxNumberWordsInParseTree debug: newParseComponentNumberOfWordsInParseTree < oldParseComponentNumberOfWordsInParseTree" << endl;
			//exit(EXIT_ERROR);
			#endif
		}
	}
	
	return result;
}

int SANIpropagateLightOptimisedClass::countNumberWordsInParseTree(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer)
{	
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	int performance = 0;
	if(currentParseTreeGroup->components.size() > 0)
	{
		performance = currentParseTreeGroup->parseTreeMaxWordIndex - currentParseTreeGroup->parseTreeMinWordIndex + 1;
		//cout << "performance = " << performance << endl;
	}
	#else
	int performanceOrig = forwardPropogationSentenceData->performance;
	
	updatePerformanceGroupSentence(currentParseTreeGroup, forwardPropogationSentenceData, layer);
	int performance = forwardPropogationSentenceData->performance;

	forwardPropogationSentenceData->performance = performanceOrig;
	#endif
	
	return performance;
}


#endif
	

#ifdef SANI_VERIFY_OWNER_GROUP_DOESNT_CONTAIN_NEWLY_ACTIVATED_COMPONENT_IN_ITS_PARSE_TREE
bool SANIpropagateLightOptimisedClass::verifyOwnerParseTreeGroupDoesntContainNewActivationComponentParseTree(const SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int layer)
{
	bool result = true;

	const SANIGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;
	if((currentParseTreeGroupTemp != NULL) && (activationPathWordCurrentParseTreeGroup != NULL))
	{
		/*
		if(parseTreeContainsGroup(currentParseTreeGroupTemp, activationPathWordCurrentParseTreeGroup, layer))
		{
			result = false;
		}
		*/
		if(parseTreeContainsGroup(activationPathWordCurrentParseTreeGroup, currentParseTreeGroupTemp, layer))
		{
			result = false;
		}
	}	
	
	return result;
}
bool SANIpropagateLightOptimisedClass::parseTreeContainsGroup(const SANIGroupParseTree* currentParseTreeGroup, const SANIGroupParseTree* parseTreeGroupToFind, const int level)
{
	bool result = false;

	if(currentParseTreeGroup == parseTreeGroupToFind)
	{
		result = true;
	}
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
				
		if(currentComponent->parseTreeGroupRef != NULL)
		{
			if(parseTreeContainsGroup(currentComponent->parseTreeGroupRef, parseTreeGroupToFind, level+1))
			{
				result = true;
			}	
		}
	}
	
	return result;
}
#endif

bool SANIpropagateLightOptimisedClass::printBackpropParseTree(SANIGroupParseTree* group, const int level)
{
	#ifdef SANI_DEBUG_PARSE_TREE_PRINT_SUPPORT_RECURSION
	SANIpropagateOperationsParseTree.traceBackpropParseTreePrint(group, 1);
	SANIpropagateOperationsParseTree.resetNeuronBackprop(group, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);	
	#else
	SANInodes.printParseTree(group, level);
	#endif
}




bool SANIpropagateLightOptimisedClass::groupActivationCompleted(const SANIGroupNeuralNetwork* group)
{	
	bool groupActivationCompleted = true;
	
	for(int i2=0; i2<group->components.size(); i2++)
	{
		SANIComponentNeuralNetwork* currentComponent2 = (group->components)[i2];
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
	
#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
bool SANIpropagateLightOptimisedClass::demarkateOptimumPathwayBackprop(const SANIGroupParseTree* currentParseTreeGroup)
{
	bool result = true;

	#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
	printParseTreeDebugIndentation(level);
	cout << "SANIpropagateLightOptimisedClass::demarkateOptimumPathwayBackprop: " <<  currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;	
	#endif
	
	/*
	SANIGroupNeuralNetwork* ownerGroup = currentParseTreeGroup->groupOrig;
	ownerGroup->optimumPathwayWordList.push_back(currentParseTreeGroup->wordIndex);
	*/
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		SANIComponentNeuralNetwork* componentOrig = currentComponent->componentRef;
		componentOrig->optimumPathwayWordList.push_back(currentComponent->wordIndex);
			
		if(currentComponent->parseTreeGroupRef != NULL)
		{					
			if(demarkateOptimumPathwayBackprop(currentComponent->parseTreeGroupRef))
			{
				result = true;
			}
		}
	}
	
	return result;	
}

bool SANIpropagateLightOptimisedClass::isWordInGroupOptimumPathwayWordList(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* currentComponent, SANIForwardPropogationWordData* forwardPropogationWordData)
{
	bool result = false;

	if(forwardPropogationSentenceData->parserEnabled)
	{
		//cout << "currentComponent->optimumPathwayWordList.size() = " << currentComponent->optimumPathwayWordList.size() << endl;
		//cout << "forwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
		for(int i=0; i<currentComponent->optimumPathwayWordList.size(); i++)
		{
			//cout << "currentComponent->optimumPathwayWordList[i] = " << currentComponent->optimumPathwayWordList[i] << endl;
		}
		
		if(find(currentComponent->optimumPathwayWordList.begin(), currentComponent->optimumPathwayWordList.end(), forwardPropogationWordData->w) != currentComponent->optimumPathwayWordList.end())
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}	
	
	return result;	
}
			
#endif


		

#endif
