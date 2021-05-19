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
 * File Name: SANIpropagateHeavyOptimised.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p9b 17-May-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Heavy Optimised - ~O(nlogn)
 * /
 *******************************************************************************/


#include "SANIpropagateHeavyOptimised.hpp"
#include "SHAREDvars.hpp"


#ifdef SANI_HEAVY
#ifdef SANI_HEAVY_OPTIMISED


bool SANIpropagateHeavyOptimisedClass::executePosRelTranslatorNeuralNetwork(const SANItranslatorVariablesClass* translatorVariables, constEffective vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance)
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
	//forwardPropogationSentenceData.parserEnabled = parserEnabled;
	#endif
	forwardPropogationSentenceData.sentenceContents = sentenceContents;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUERIES
	forwardPropogationSentenceData.isQuery = SANInodesGroupClassObject.determineIsQuery(sentenceContents);
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
	forwardPropogationSentenceData.parseIsolatedSubreferenceSets = parseIsolatedSubreferenceSets;
	#endif
	forwardPropogationSentenceData.SANIGroupTypes = SANIGroupTypes;
	
	#ifdef SANI_PARSE_SAVE_PARSE_TREE
	forwardPropogationSentenceData.topLevelParseTreeGroupPropagate = NULL;
	#endif
	
	//memory clean forwardPropogationSentenceData.forwardPropogationWordDataArray [from previous POS permutation];
	for(int i=0; i<forwardPropogationSentenceData.forwardPropogationWordDataArray.size(); i++)
	{
		SANIForwardPropogationWordData* forwardPropogationWordData = forwardPropogationSentenceData.forwardPropogationWordDataArray[i];
		delete forwardPropogationWordData;
	}
	forwardPropogationSentenceData.forwardPropogationWordDataArray.clear();

	for(int w=0; w<sentenceContents->size(); w++)
	{
		SANIForwardPropogationWordData* forwardPropogationWordData = new SANIForwardPropogationWordData();
		forwardPropogationSentenceData.forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
	}
	
	forwardPropogationSentenceData.activationPathWordFirstActivationMemoryGroupArray.resize(sentenceContents->size());
	forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray.resize(sentenceContents->size());

	//SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of SANIpropagateHeavyOptimisedClass::executePosRelTranslatorNeuralNetwork)
	
	//SANIGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;
		
	if(sentenceContents->size() > 0)
	{
		bool expectToSeeConnectionWithPreviousWordTrace = false;
		bool passThrough = false;
		int layer = 0;
		for(int w=0; w<sentenceContents->size(); w++)
		{
			if(!propagateWordThroughNetworkWordGroupIntro(translatorVariables, w, &forwardPropogationSentenceData, layer))
			{
				result = false;
			}
		}
		
		/*
		cout << "propagateWordThroughNetworkWordGroupIntro{} finished" << endl;
		exit(EXIT_ERROR);
		*/
		if(!propagateWordThroughNetworkGroupIntro(translatorVariables, &forwardPropogationSentenceData))
		{
			result = false;
		}
	}
	else
	{
		cerr << "SANIpropagateHeavyOptimisedClass::executePosRelTranslatorNeuralNetwork{} error: sentenceContents->size() == 0" << endl;
		exit(EXIT_ERROR);		
	}
	
	if(forwardPropogationSentenceData.toplevelGroupActivationFound)
	{
		sentenceValidActivationPath = true;
		result = true;
		
		#ifdef SANI_PARSE_SAVE_PARSE_TREE
		*topLevelParseTreeGroup = forwardPropogationSentenceData.topLevelParseTreeGroupPropagate;
		//cout << "forwardPropogationSentenceData->topLevelParseTreeGroupPropagate" << endl;
		
		#ifdef SANI_DEBUG_PROPAGATE
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
		
		//cout << "DEBUG: finished executing SANIpropagateHeavyOptimisedClass::executePosRelTranslatorNeuralNetwork (exiting)" << endl;
		//exit(EXIT_ERROR);
	}
	else
	{
		*performance = 0;		
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(sentenceValidActivationPath)
	{
		SANIpropagateOperationsParseTree.traceBackpropParseTreeSetTraced(forwardPropogationSentenceData.topLevelParseTreeGroupPropagate, 1);	//added GIA3g6aTEMP32 - set all parseTreeGroup groups in final heirachy to neuronTraced to prevent their memory from being deleted during SANIpropagateOperations.resetAllNeuronComponents
	}
	#endif
	for(int w=0; w<sentenceContents->size(); w++)	//start at every w in sentence
	{
		vector<SANIGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupArray = forwardPropogationSentenceData.activationPathWordFirstActivationMemoryGroupArray[w];
		vector<SANIGroupParseTree*> activationPathWordFirstParseTreeGroupArray = forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray[w];
		if(activationPathWordFirstActivationMemoryGroupArray.size() != activationPathWordFirstParseTreeGroupArray.size())
		{	
			cerr << "SANIpropagateHeavyOptimisedClass::executePosRelTranslatorNeuralNetwork error: activationPathWordFirstActivationMemoryGroupArray.size() != activationPathWordFirstParseTreeGroupArray.size()" << endl;
			exit(EXIT_ERROR);
		} 
		for(int i=0; i<activationPathWordFirstActivationMemoryGroupArray.size(); i++) 
		{
			SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroup = activationPathWordFirstActivationMemoryGroupArray[i];
			SANIGroupParseTree* activationPathWordFirstParseTreeGroup = activationPathWordFirstParseTreeGroupArray[i];
			SANIpropagateOperationsParseTree.resetGroupParseTreeGroupRef(activationPathWordFirstActivationMemoryGroup, activationPathWordFirstParseTreeGroup, true);
		}
		activationPathWordFirstActivationMemoryGroupArray.clear();
		activationPathWordFirstParseTreeGroupArray.clear();
	}
	forwardPropogationSentenceData.activationPathWordFirstActivationMemoryGroupArray.clear();
	forwardPropogationSentenceData.activationPathWordFirstParseTreeGroupArray.clear();
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(sentenceValidActivationPath)
	{
		SANIpropagateOperationsParseTree.resetNeuronBackprop(forwardPropogationSentenceData.topLevelParseTreeGroupPropagate, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
	}
	#endif
	
	for(int w=0; w<sentenceContents->size(); w++)
	{
		SANIForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData.forwardPropogationWordDataArray[w]);
		#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
		cout << "w = " << w << endl;
		#endif
	}
	
		
	/*
	#ifdef SANI_DEBUG_PROPAGATE
	cout << "DEBUG: finished executing SANIpropagateHeavyOptimisedClass::executePosRelTranslatorNeuralNetwork (exiting)" << endl;
	exit(EXIT_ERROR);
	#endif
	*/
	
	return result;
}

	


bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkWordGroupIntro(const SANItranslatorVariablesClass* translatorVariables, int w, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = false;

	/*
	more general algorithm;
	(1) start at every w in sentence
		propagate word up network as high as it goes
		store the parse tree group (and memory activation group) in memory
			store also the num words n encapsulated by the parse tree group (for efficiency)
		NO: parse to the right and generate sets of parse groups (limited by previousWordFound), including single words
	*/
		
	//SANIGroupParseTree* activationPathWordFirstParseTreeGroup, SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroup

	LRPpreprocessorPlainTextWord* currentWord = forwardPropogationSentenceData->sentenceContents->at(w);

	SANInodes.printParseTreeDebugIndentation(layer);
	#ifdef SANI_DEBUG_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
	#ifndef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "currentWord->POSambiguityInfo = " << LRPpreprocessorPOStagger.printPOSambiguityInfo(currentWord->POSambiguityInfo) << endl;
	#endif
	
	//prepare word specific variables:
	SANIForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);
	forwardPropogationWordData->wordReference = currentWord;
	forwardPropogationWordData->w = w;
	//cout << "forwardPropogationWordData->previousWordConnections.size() = " << forwardPropogationWordData->previousWordConnections.size() << endl;
	
	//group specific variables:
	SANIForwardPropogationSignalData forwardPropogationSignalData;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
	forwardPropogationSignalData.wordNounVariantType = currentWord->wordNounVariantGrammaticalTenseForm;
	forwardPropogationSignalData.wordVerbVariantType = currentWord->wordVerbVariantGrammaticalTenseForm;
	#endif
				
	string wordLowerCase = SHAREDvars.convertStringToLowerCase(&(currentWord->tagName));
		
	
	if(!SANInodes.currentWordPOSunknown(currentWord))
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		int wordPOStype = currentWord->unambiguousPOSindex;
		#else
		for(int wordPOStype=0; wordPOStype<LRP_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		{
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, wordPOStype))
			{
		#endif
				if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
				{
					result = true;
				}
				
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
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
			if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
			{
				result = true;
			}
		}
		#else
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
		int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_NOUN;
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
		{
			result = true;
		}	
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
		if(LRPpreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
		{		
			//cout << "isMidSentenceUppercaseWordLikelyProperNoun" << endl;
			int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT;
			if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
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
	if(SANIformation.findWordInGroupMap(wordLowerCase, SANIformation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef SANI_DEBUG_PROPAGATE
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
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
		cout << "SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
		{
			result = true;
		}
	}
	else if(SANIformation.findWordInGroupMap(currentWord->tagName, SANIformation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationWordData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef SANI_DEBUG_PROPAGATE
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
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
							cerr << "SANIClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
							exit(EXIT_ERROR);
						}
							
						#ifdef SANI_DEBUG_PROPAGATE
						SANInodes.printParseTreeDebugIndentation(layer);
						cout << "SANIClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
						#endif
						if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClassType, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
						{
							result = true;
						}
						if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClass, &forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))	//what is this for?
						{
							result = true;
						}
					}
				}
			}
		}
	}
		
	return result;
}

	
bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkWordGroupInit(const SANItranslatorVariablesClass* translatorVariables, int w, int wordPOStype, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = false;
	
	SANIGroupNeuralNetwork* inputLayerGroup = SANIformation.getInputGroupLayerSection(SANIformation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef SANI_DEBUG_PROPAGATE
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = wordPOStype;
	#endif
	
	if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer))
	{
		result = true;
	}
	
	return result;
}	
	
bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkWordGroupInit(const SANItranslatorVariablesClass* translatorVariables, int w, SANIGroupNeuralNetwork* inputLayerGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer)
{
	bool result = true;
	
	//cout << "activationPathWordFirstParseTreeGroupNext->inputGroupString" << endl;
	
	SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroup = NULL;
	SANIGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;
	generateMemoryActiveGroup(inputLayerGroup, inputLayerGroup, inputLayerGroup, &activationPathWordFirstActivationMemoryGroup, &activationPathWordFirstParseTreeGroup, false);
	/*
	SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroup = SANInodes.copyGroup(inputLayerGroup);
	activationPathWordFirstActivationMemoryGroup->groupOrig = inputLayerGroup;
	SANIGroupParseTree* activationPathWordFirstParseTreeGroup = new SANIGroupParseTree(*inputLayerGroup);
	*/
	
	forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[w].push_back(activationPathWordFirstActivationMemoryGroup);
	forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w].push_back(activationPathWordFirstParseTreeGroup);
	
	activationPathWordFirstActivationMemoryGroup->layer = layer;
	activationPathWordFirstActivationMemoryGroup->numberWordsInSet = 1;
	activationPathWordFirstActivationMemoryGroup->forwardPropogationSignalData = *forwardPropogationSignalData;
	activationPathWordFirstActivationMemoryGroup->forwardPropogationWordData = *forwardPropogationWordData;	//(forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);
	activationPathWordFirstActivationMemoryGroup->neuronActive = true;
	activationPathWordFirstActivationMemoryGroup->wordGroupNeuron = true;
	activationPathWordFirstActivationMemoryGroup->inputGroupString = true;
		//TODO: ensure activationPathWordFirstActivationMemoryGroup->forwardPropogationWordData is always set correctly
		
	return result;
}				
	

bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupIntro(const SANItranslatorVariablesClass* translatorVariables, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = false;
	
	/*
	more general algorithm;
	(2) repeat until found top level group with all encapsulated words:
		start at every w in sentence
			for each existing parseTreeGroup stored at w
				for each adjacent parseTreeGroup available (that does not skip an encapsulate word), ie stored at w+n (where n is number of words in the left parse group)
					activate their appropriate top level components in the memory group
					see if the second activated group propagates up into the second
	*/
	
	vector<LRPpreprocessorPlainTextWord*>* sentenceContents = forwardPropogationSentenceData->sentenceContents;

	int maxIterationIndex = SANI_MAX_ITERATIONS;
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
	forwardPropogationSentenceData->parseTreeMaxWeightPropagate = 0;
	#endif
	forwardPropogationSentenceData->maxIterationPropagate = false;
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_WEAK
	bool limitedIterations = false;
	#endif
	#endif
	bool foundTopLevelGroup = false;
	bool stillGeneratingSets = true;
	int connectIterationIndex = 1;
	while(!foundTopLevelGroup && stillGeneratingSets)	//repeat until found top level group with all encapsulated words:
	{
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
		//SANInodes.printParseTreeDebugIndentation(layer);
		cout << "\nconnectIterationIndex = " << connectIterationIndex << endl;
		#endif
			
		for(int w=0; w<sentenceContents->size(); w++)	//start at every w in sentence
		{
			/*
			#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
			cout << "w = " << w << endl;
			#endif
			*/
			
			vector<SANIGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupArray = forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[w];
			vector<SANIGroupParseTree*> activationPathWordFirstParseTreeGroupArray = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[w];

			#ifdef SANI_ENFORCE_STRICT_ITERATION_INDICES
			int groupArraySize = activationPathWordFirstActivationMemoryGroupArray.size();
			for(int i=0; i<groupArraySize; i++)
			#else
			for(int i=0; i<activationPathWordFirstActivationMemoryGroupArray.size(); i++) 	//for each existing parseTreeGroup stored at w
			#endif
			{
				SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroup = activationPathWordFirstActivationMemoryGroupArray[i];
				SANIGroupParseTree* activationPathWordFirstParseTreeGroup = activationPathWordFirstParseTreeGroupArray[i];
				
				int wNext = w + activationPathWordFirstActivationMemoryGroup->numberWordsInSet;
				#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
				cout << "w = " << w << endl;
				cout << "wNext = " << wNext << endl;
				#endif
				
				if(wNext < sentenceContents->size())
				{	
					#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
					SANInodes.printParseTreeDebugIndentation(activationPathWordFirstActivationMemoryGroup->layer);
					cout << "propagateWordThroughNetworkGroupIntro activationPathWordFirstActivationMemoryGroup: " <<  activationPathWordFirstActivationMemoryGroup->groupTypeName << ":" << activationPathWordFirstActivationMemoryGroup->groupName << endl;		
					#endif

					//activate their appropriate top level components in the memory group
					int layer = activationPathWordFirstActivationMemoryGroup->layer;
					SANIForwardPropogationWordData* forwardPropogationWordData = &(activationPathWordFirstActivationMemoryGroup->forwardPropogationWordData);
					//cout << "forwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
					SANIForwardPropogationSignalData* forwardPropogationSignalData = &(activationPathWordFirstActivationMemoryGroup->forwardPropogationSignalData);
					SANIForwardPropogationActivationPointData forwardPropogationActivationPointData;
					#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
					forwardPropogationActivationPointData.activationPathWordFirstActivationMemoryGroupActivationPointArray = &(activationPathWordFirstActivationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray);
					forwardPropogationActivationPointData.activationPathWordFirstParseTreeGroupActivationPointArray = &(activationPathWordFirstParseTreeGroup->activationPathWordFirstParseTreeGroupActivationPointArray);
					#else
					vector<SANIGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupActivationPointArray;
					vector<SANIGroupParseTree*> activationPathWordFirstParseTreeGroupActivationPointArray;	
					forwardPropogationActivationPointData.activationPathWordFirstActivationMemoryGroupActivationPointArray = &activationPathWordFirstActivationMemoryGroupActivationPointArray;
					forwardPropogationActivationPointData.activationPathWordFirstParseTreeGroupActivationPointArray = &activationPathWordFirstParseTreeGroupActivationPointArray;
					#endif
					forwardPropogationSentenceData->forwardPropogationActivationPointData = &forwardPropogationActivationPointData;
					forwardPropogationActivationPointData.generateActivationPointArray = true;

					#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
					if(!(activationPathWordFirstActivationMemoryGroup->pointArrayGenerated))
					{
					#endif	
						#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						if(!activationPathWordFirstActivationMemoryGroup->firstLevelActivationPointAdded)
						{
							activationPathWordFirstActivationMemoryGroup->firstLevelActivationPointAdded = true;
						#endif
							//add the first level activationPathWordFirstActivationMemoryGroup to activationPathWordFirstActivationMemoryGroupActivationPointArray (this isn't done by propagateWordThroughNetworkGroup):
							forwardPropogationActivationPointData.activationPathWordFirstActivationMemoryGroupActivationPointArray->push_back(activationPathWordFirstActivationMemoryGroup);
							forwardPropogationActivationPointData.activationPathWordFirstParseTreeGroupActivationPointArray->push_back(activationPathWordFirstParseTreeGroup);
						#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						}
						#endif

						if(activationPathWordFirstActivationMemoryGroup->neuronActive)
						{
							#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
							activationPathWordFirstActivationMemoryGroup->pointArrayGenerated = true;
							#endif
							
							SANIGroupNeuralNetwork* activationPathWordFirstActivationMemoryGroupBase = activationPathWordFirstActivationMemoryGroup;
							if(propagateWordThroughNetworkGroup(translatorVariables, activationPathWordFirstActivationMemoryGroupBase, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup))
							{
								result = true;
							}
						}

					#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP	
					}
					#endif


					vector<SANIGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupArrayNext = forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[wNext];
					vector<SANIGroupParseTree*> activationPathWordFirstParseTreeGroupArrayNext = forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[wNext];
					#ifdef SANI_ENFORCE_STRICT_ITERATION_INDICES
					int groupArrayNextSize = activationPathWordFirstActivationMemoryGroupArrayNext.size();
					for(int i2=0; i2<groupArrayNextSize; i2++)
					#else
					for(int i2=0; i2<activationPathWordFirstActivationMemoryGroupArrayNext.size(); i2++) 	//for each adjacent parseTreeGroup available (that does not skip an encapsulate word), ie stored at w+n (where n is number of words in the left parse group)
					#endif
					{
						SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroupNext = activationPathWordFirstActivationMemoryGroupArrayNext[i2];
						SANIGroupParseTree* activationPathWordFirstParseTreeGroupNext = activationPathWordFirstParseTreeGroupArrayNext[i2];

						#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
						SANInodes.printParseTreeDebugIndentation(activationPathWordFirstActivationMemoryGroupNext->layer);
						cout << "propagateWordThroughNetworkGroupIntro activationPathWordFirstActivationMemoryGroupNext: " <<  activationPathWordFirstActivationMemoryGroupNext->groupTypeName << ":" << activationPathWordFirstActivationMemoryGroupNext->groupName << endl;		
						#endif

						//activate their appropriate top level components in the memory group
						int layerNext = activationPathWordFirstActivationMemoryGroupNext->layer;
						SANIForwardPropogationWordData* forwardPropogationWordDataNext = &(activationPathWordFirstActivationMemoryGroupNext->forwardPropogationWordData);
						//cout << "forwardPropogationWordDataNext->w = " << forwardPropogationWordDataNext->w << endl;
						SANIForwardPropogationSignalData* forwardPropogationSignalDataNext = &(activationPathWordFirstActivationMemoryGroupNext->forwardPropogationSignalData);
						SANIForwardPropogationActivationPointData forwardPropogationActivationPointDataNext;
						#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						forwardPropogationActivationPointDataNext.activationPathWordFirstActivationMemoryGroupActivationPointArray = &(activationPathWordFirstActivationMemoryGroupNext->activationPathWordFirstActivationMemoryGroupActivationPointArray);
						forwardPropogationActivationPointDataNext.activationPathWordFirstParseTreeGroupActivationPointArray = &(activationPathWordFirstParseTreeGroupNext->activationPathWordFirstParseTreeGroupActivationPointArray);
						#else
						vector<SANIGroupActivationMemory*> activationPathWordFirstActivationMemoryGroupActivationPointArrayNext;
						vector<SANIGroupParseTree*> activationPathWordFirstParseTreeGroupActivationPointArrayNext;	
						forwardPropogationActivationPointDataNext.activationPathWordFirstActivationMemoryGroupActivationPointArray = &activationPathWordFirstActivationMemoryGroupActivationPointArrayNext;
						forwardPropogationActivationPointDataNext.activationPathWordFirstParseTreeGroupActivationPointArray = &activationPathWordFirstParseTreeGroupActivationPointArrayNext;
						#endif
						forwardPropogationSentenceData->forwardPropogationActivationPointData = &forwardPropogationActivationPointDataNext;
						forwardPropogationActivationPointDataNext.generateActivationPointArray = true;

						#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						if(!(activationPathWordFirstActivationMemoryGroupNext->pointArrayGenerated))
						{
						#endif
							if(activationPathWordFirstActivationMemoryGroupNext->neuronActive)
							{
								#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
								activationPathWordFirstActivationMemoryGroupNext->pointArrayGenerated = true;
								#endif

								#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
								if(!activationPathWordFirstActivationMemoryGroupNext->firstLevelActivationPointAdded)
								{
									activationPathWordFirstActivationMemoryGroupNext->firstLevelActivationPointAdded = true;
								#endif
									//if(activationPathWordFirstActivationMemoryGroupNext->inputGroupString)
									//{	
									//add the first level activationPathWordFirstActivationMemoryGroupNext to activationPathWordFirstActivationMemoryGroupActivationPointArray (this isn't done by propagateWordThroughNetworkGroup):
									forwardPropogationActivationPointDataNext.activationPathWordFirstActivationMemoryGroupActivationPointArray->push_back(activationPathWordFirstActivationMemoryGroupNext);	//OLD: add original unpropagated group to forwardPropogationActivationPointDataNext (in case of string)
									forwardPropogationActivationPointDataNext.activationPathWordFirstParseTreeGroupActivationPointArray->push_back(activationPathWordFirstParseTreeGroupNext);
									//}
								#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP	
								}
								#endif
								
								if(propagateWordThroughNetworkGroup(translatorVariables, activationPathWordFirstActivationMemoryGroupNext, forwardPropogationSignalDataNext, forwardPropogationWordDataNext, forwardPropogationSentenceData, layerNext, activationPathWordFirstParseTreeGroupNext))
								{
									result = true;
								}
							}
						#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						}
						#endif


						if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
						{
							//see if the second activated set point propagates up immediately into the first				
							if(connectToPreviousActivationGroup(translatorVariables, activationPathWordFirstActivationMemoryGroup, activationPathWordFirstActivationMemoryGroupNext, &forwardPropogationActivationPointData, &forwardPropogationActivationPointDataNext, forwardPropogationSignalDataNext, forwardPropogationWordData, forwardPropogationWordDataNext, forwardPropogationSentenceData, layerNext))
							{
								result = true;
							}
						}
						
						#ifndef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						for(int i=1; i<activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArrayNext.size(); i++)	//i=1; never remove the first activationPathWordFirstActivationMemoryGroupActivationPoint (as this will correspond to activationPathWordFirstActivationMemoryGroup)
						{
							SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroupActivationPointTemp = activationPathWordFirstActivationMemoryGroupActivationPointArrayNext[i];
							SANIGroupParseTree* activationPathWordFirstParseTreeGroupActivationPointTemp = activationPathWordFirstParseTreeGroupActivationPointArrayNext[i];
							if(!activationPathWordFirstActivationMemoryGroupActivationPointTemp->firstLevelActivationPointAdded)
							{
								SANInodes.deleteGroup(activationPathWordFirstActivationMemoryGroupActivationPointTemp);
								if(!(currentParseTreeGroup->neuronTraced))	//parseTreeGroups in activationPointArray >i=0 can still be added to final parseTree
								{
									SANInodes.deleteGroup(activationPathWordFirstParseTreeGroupActivationPointTemp);
								}
							}
						}
						#endif
					}
				}
				
				#ifndef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
				for(int i=1; i<activationMemoryGroup->activationPathWordFirstActivationMemoryGroupActivationPointArray.size(); i++)	//i=1; never remove the first activationPathWordFirstActivationMemoryGroupActivationPoint (as this will correspond to activationPathWordFirstActivationMemoryGroup)
				{
					SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroupActivationPointTemp = activationPathWordFirstActivationMemoryGroupActivationPointArray[i];
					SANIGroupParseTree* activationPathWordFirstParseTreeGroupActivationPointTemp = activationPathWordFirstParseTreeGroupActivationPointArray[i];
					if(!activationPathWordFirstActivationMemoryGroupActivationPointTemp->firstLevelActivationPointAdded)
					{
						SANInodes.deleteGroup(activationPathWordFirstActivationMemoryGroupActivationPointTemp);
						if(!(currentParseTreeGroup->neuronTraced))	//parseTreeGroups in activationPointArray >i=0 can still be added to final parseTree
						{
							SANInodes.deleteGroup(activationPathWordFirstParseTreeGroupActivationPointTemp);
						}
					}
				}
				#endif
			}
		}
		
		if(forwardPropogationSentenceData->finishedPassingSentenceWords)
		{
			stillGeneratingSets = false;
		}
		if(connectIterationIndex >= maxIterationIndex)
		{
			stillGeneratingSets = false;
		}
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
		if(forwardPropogationSentenceData->maxIterationPropagate)
		{
			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_WEAK
			forwardPropogationSentenceData->maxIterationPropagate = false;
			if(!limitedIterations)
			{
				limitedIterations = true;
				maxIterationIndex = connectIterationIndex+SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_WEAK_OFFSET;	//allow 1 more iteration once maxIteration detected
			}
			#else
			stillGeneratingSets = false;
			#endif
		}
		#endif
							
		connectIterationIndex++;
	}
	
	return result;
	
}

//CHECKTHIS; check parameter forwardPropogationWordData
bool SANIpropagateHeavyOptimisedClass::connectToPreviousActivationGroup(const SANItranslatorVariablesClass* translatorVariables, SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroup, SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroupNext, const SANIForwardPropogationActivationPointData* forwardPropogationActivationPointData, const SANIForwardPropogationActivationPointData* forwardPropogationActivationPointDataNext, SANIForwardPropogationSignalData* forwardPropogationSignalDataNext, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationWordData* forwardPropogationWordDataNext, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer)
{
	bool result = false;
	
	for(int i1=0; i1<forwardPropogationActivationPointDataNext->activationPathWordFirstActivationMemoryGroupActivationPointArray->size(); i1++)
	{
		SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroupNextPoint = (*forwardPropogationActivationPointDataNext->activationPathWordFirstActivationMemoryGroupActivationPointArray)[i1];
		SANIGroupParseTree* activationPathWordFirstParseTreeGroupNextPoint = (*forwardPropogationActivationPointDataNext->activationPathWordFirstParseTreeGroupActivationPointArray)[i1];

		if(activationPathWordFirstActivationMemoryGroupNextPoint->neuronActive)	//added 3g8aTEMP22
		{			
			for(int i2=0; i2<activationPathWordFirstActivationMemoryGroupNextPoint->SANIfrontComponentConnectionList.size(); i2++)
			{
				SANIComponentNeuralNetwork* currentComponentNext = (activationPathWordFirstActivationMemoryGroupNextPoint->SANIfrontComponentConnectionList)[i2];
				SANIGroupNeuralNetwork* ownerGroupNext = currentComponentNext->ownerGroup;
				
				for(int i3=0; i3<forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray->size(); i3++)
				{
					SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroupPoint = (*forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray)[i3];
					SANIGroupParseTree* activationPathWordFirstParseTreeGroupPoint = (*forwardPropogationActivationPointData->activationPathWordFirstParseTreeGroupActivationPointArray)[i3];
					SANIGroupNeuralNetwork* groupOrig = activationPathWordFirstActivationMemoryGroupPoint->groupOrig;

					if(groupOrig == ownerGroupNext)
					{
						//found matching group names between set and next set (see if their memory groups are connected)

						#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
						SANInodes.printParseTreeDebugIndentation(layer);
						cout << "connectToPreviousActivationGroup (groupOrig == ownerGroupNext): activationPathWordFirstActivationMemoryGroupPoint: " <<  activationPathWordFirstActivationMemoryGroupPoint->groupTypeName << ":" << activationPathWordFirstActivationMemoryGroupPoint->groupName << endl;		
						SANInodes.printParseTreeDebugIndentation(layer);
						cout << "connectToPreviousActivationGroup (groupOrig == ownerGroupNext): activationPathWordFirstActivationMemoryGroupNextPoint: " <<  activationPathWordFirstActivationMemoryGroupNextPoint->groupTypeName << ":" << activationPathWordFirstActivationMemoryGroupNextPoint->groupName << endl;		
						#endif

						SANIComponentNeuralNetwork* ownerComponentNext = currentComponentNext;
						int componentIndex = currentComponentNext->componentIndex;
						int ownerComponentIndex = componentIndex;
						if(currentComponentNext->isSubcomponent)
						{
							ownerComponentNext = currentComponentNext->ownerComponent;
							ownerComponentIndex = ownerComponentNext->componentIndex;
						}

						SANIGroupActivationMemory* ownerGroup = activationPathWordFirstActivationMemoryGroupPoint;
						SANIComponentNeuralNetwork* ownerComponent = ownerGroup->components[ownerComponentIndex];
						SANIComponentNeuralNetwork* currentComponent = ownerComponent;
						if(currentComponentNext->isSubcomponent)
						{
							currentComponent = ownerComponent->subComponents[componentIndex];
						}

						forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup = true;
						forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray = false;	//CHECKTHIS
						forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW = forwardPropogationWordData->w;
						
						
						//forwardPropogationActivationPointDataNext->connectToPreviousActivationGroup = true;
						
						if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
						{
							if(propagateWordThroughNetworkGroupSelect(translatorVariables, ownerGroup, ownerGroup->groupOrig, currentComponent, forwardPropogationSignalDataNext, forwardPropogationWordDataNext, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroupNextPoint, activationPathWordFirstParseTreeGroupPoint))
							{
								result = true;

								#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
								cout << "connectToPreviousActivationGroup: propagateWordThroughNetworkGroupSelect passed" << endl;
								#endif

								//FUTURE: ideally this should be done (set correctly originally) in SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponent:
								SANIGroupActivationMemory* newGroup = forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[forwardPropogationWordData->w].back();	//forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW
								newGroup->numberWordsInSet = activationPathWordFirstActivationMemoryGroup->numberWordsInSet + activationPathWordFirstActivationMemoryGroupNext->numberWordsInSet;	//CHECKTHIS
								newGroup->forwardPropogationWordData.w = forwardPropogationWordData->w;
								
								#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
								cout << "\tnewGroup: " <<  newGroup->groupTypeName << ":" << newGroup->groupName << endl;	
								cout << "\tnewGroup->pointArrayGenerated: " <<  newGroup->pointArrayGenerated << endl;
								cout << "\tnewGroup->numberWordsInSet = " << newGroup->numberWordsInSet << endl;
								cout << "\tforwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
								cout << "\tnewGroup->forwardPropogationWordData.w = " << newGroup->forwardPropogationWordData.w << endl;
								cout << "\tforwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW = " << forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW << endl;
								#endif
							}
						}
					}
				}
			}
		}
	}
		
	return result;
}
	


bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroup(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, constEffective SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_PROPAGATE
	//cout << "layer = " << layer << endl;
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << group->groupName << " \e[0m" << endl;
	#endif
	
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops
	{
		group->neuronPropagated = true;
	#endif
		
		for(int i=0; i<group->SANIfrontComponentConnectionList.size(); i++)
		{
			if(!(forwardPropogationSentenceData->finishedPassingSentenceWords))
			{
				SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i];
				SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
				SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = SANInodes.convertNeuralNetworkGroupToParseTreeGroupNew(ownerGroup);
				activationPathWordCurrentParseTreeGroupOwner->components.clear();	//dont wish to copy subcomponents into currentParseTreeGroupNew;
				
				if(propagateWordThroughNetworkGroupSelect(translatorVariables, ownerGroup, ownerGroup, currentComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner))
				{
					result = true;
				}
				
				#ifdef SANI_PARSE
				SANInodes.deleteGroup(activationPathWordCurrentParseTreeGroupOwner);
				#endif				
			}
		}
		
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	#endif
	
	return result;
}
	
		
bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupSelect(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* ownerGroup, constEffective SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* currentComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, constEffective SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner)
{
	bool result = false;
	 		
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
		
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA2
		SANInodes.printParseTreeDebugIndentation(layer+1);
		cout << "2a: propagateWordThroughNetworkGroupSelect: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
		//cout << "currentComponent componentType = " << currentComponent->componentType << endl;
		SANInodes.printComponent(currentComponent, layer+1);
		#endif

		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		if(SANIpropagateOperations.componentTests1(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))	//static component tests (component activations irrelevant)
		{
		#endif
		
			//cout << "ownerGroup->groupTypeName = " << ownerGroup->groupTypeName  << endl;
		
			SANIGroupActivationMemory* activationMemoryGroupNew = NULL;
			SANIGroupParseTree* currentParseTreeGroupNew = NULL;
			
			generateMemoryActiveGroup(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, &activationMemoryGroupNew, &currentParseTreeGroupNew, true);			
			
			//required because ownerGroup is often equivalent to ownerGroupOrig:
			activationMemoryGroupNew->forwardPropogationWordData = *forwardPropogationWordData;
			//activationMemoryGroupNew->forwardPropogationSignalData = *forwardPropogationSignalData;	//set from semanticRelationReturnEntityForwardPropogationSignalDataProspective
			activationMemoryGroupNew->layer = layer;
			activationMemoryGroupNew->neuronActive = false;
			//activationMemoryGroupNew->pointArrayGenerated = false;
			activationMemoryGroupNew->firstLevelActivationPointAdded = false;
			activationMemoryGroupNew->activationPathWordFirstActivationMemoryGroupActivationPointArray.clear();
			currentParseTreeGroupNew->activationPathWordFirstParseTreeGroupActivationPointArray.clear();
			
			#ifdef SANI_SAVE_MEMORY_GROUPS
			SANIComponentNeuralNetwork* activationMemoryGroupOwnerComponent = activationMemoryGroupNew->components[ownerComponentIndex];
			SANIComponentNeuralNetwork* activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent;
			if(currentComponentOrig->isSubcomponent)
			{
				if(SANInodesComponentClassObject.componentHasSubcomponents(ownerComponentOrig))
				{
					/*
					cout << "propagateWordThroughNetworkGroupSelect: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
					SANInodes.printComponent(currentComponent, layer+1);
					cout << "componentIndex = " << componentIndex << endl;
					*/
					activationMemoryGroupCurrentComponent = activationMemoryGroupOwnerComponent->subComponents[componentIndex];
				}
			}
			ownerGroup = activationMemoryGroupNew;
			currentComponent = activationMemoryGroupCurrentComponent;
			ownerComponent = activationMemoryGroupOwnerComponent;
			#endif
			#ifdef SANI_PARSE
			activationPathWordCurrentParseTreeGroupOwner = currentParseTreeGroupNew;	//use at top of parseTreeGroup at top of stack
			#endif

			if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
				currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
			}

			SANIposRelTranslatorDebug debug;

			#ifdef SANI_DEBUG_PROPAGATE_EXTRA2
			SANInodes.printParseTreeDebugIndentation(layer+1);
			cout << "2b: propagateWordThroughNetworkGroupSelect: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
			SANInodes.printComponent(currentComponent, layer+1);
			#endif

			if(currentComponent->semanticRelationReturnEntity)
			{
				activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective = *forwardPropogationSignalData;
			}

			int indexOfFirstComponentInGroup = 0;
			int indexOfLastComponentInGroup = ownerGroup->components.size()-1;
			SANIComponentNeuralNetwork* firstComponent = (ownerGroup->components)[indexOfFirstComponentInGroup];
			SANIComponentNeuralNetwork* lastComponent = (ownerGroup->components)[indexOfLastComponentInGroup];

			/*
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
							if(lastComponent->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex == forwardPropogationWordData->w - 1)
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
			*/


			bool deleteNewlyCreatedMemoryGroup = false;

			if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, &debug))
			{
				result = true;
			}
			else
			{
				deleteNewlyCreatedMemoryGroup = true;
			}

			if(deleteNewlyCreatedMemoryGroup)
			{
				#ifdef SANI_PARSE
				SANInodes.deleteGroup(currentParseTreeGroupNew);
				#endif
				#ifdef SANI_SAVE_MEMORY_GROUPS
				SANInodes.deleteGroup(activationMemoryGroupNew);
				#endif
			}

		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
		}
		#endif
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	}
	#endif	
	
	return result;
}	
	


bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(const SANItranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, constEffective SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const SANIposRelTranslatorDebug* debug)
{
	bool result = false;
	
	bool activationSequenceCompleted = false;
	bool sequentialActivationFound = false;
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA3
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "3a: propagateWordThroughNetworkGroup: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;	
	SANInodes.printComponent(currentComponent, layer+1);
	#endif
	
	SANIComponentNeuralNetwork* previousActiveComponent = NULL;	//NOTUSED
	SANIComponentNeuralNetwork* finalActiveComponent = NULL;	//NOTUSED
	bool firstActiveComponentInGroup = false;
	if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, ownerComponent, &(ownerGroup->components), &activationSequenceCompleted, &firstActiveComponentInGroup, &previousActiveComponent, &finalActiveComponent))
	{	
		bool subComponentsPassed = true;
		bool repeatDetected = false;
		if(currentComponent->isSubcomponent)
		{
			subComponentsPassed = false;
			if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR)
			{
				if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady(forwardPropogationWordData, forwardPropogationSentenceData, currentComponent, &(ownerComponent->subComponents)))
				{
					subComponentsPassed = true;
					#ifdef SANI_DEBUG_PROPAGATE
					//cout << "SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(forwardPropogationWordData, forwardPropogationSentenceData, currentComponent, &(ownerComponent->subComponents), &repeatDetected))
				{
					subComponentsPassed = true;
					#ifdef SANI_DEBUG_PROPAGATE
					//cout << "SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady" << endl;
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
			if(!firstActiveComponentInGroup || SANIpropagateOperations.componentTests2(ownerGroupOrig, activationPathWordCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
			{
			#endif
				if(propagateWordThroughNetworkGroupComponent(translatorVariables, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, repeatDetected, activationPathWordCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, debug))
				{
					result = true;
				}
			#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
			}
			#endif
		}
	}
	
	return result;
}
	
	
//precondition: only components (not subcomponents) use special condition flags (wordNounVariantType/wordVerbVariantType)
bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponent(const SANItranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, constEffective SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const SANIposRelTranslatorDebug* debug)
{
	bool result = false;
	
	//cout << "4: SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponent: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
		
	ownerComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
	ownerComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;

	#ifdef SANI_PARSE
	//create new parseTreeGroup
	//copied from SANIpropagateHeavyOptimisedInverseClass::generateRulesGroupTreeComponents;
	SANIComponentParseTree* newParseComponent = SANInodes.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new SANIComponentParseTree(*SANIpropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
	newParseComponent->componentRef = currentComponent;	
	newParseComponent->neuronComponentConnectionActive = true;
	newParseComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
	newParseComponent->parseTreeGroupRef = activationPathWordCurrentParseTreeGroup;
	if(newParseComponent->groupRefName == "")
	{
		newParseComponent->groupRefName = activationPathWordCurrentParseTreeGroup->groupName;
	}
	//currentWord->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous words in current parse tree	//this will be set later by GIAposRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
	newParseComponent->wordPOStypeInferred = forwardPropogationWordData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
	activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);
	#endif

	#ifdef SANI_DEBUG_PROPAGATE_EXTRA4
	//bool sentenceWordDataFullyConnected = SANIpropagateOperations.isSentenceWordDataFullyConnected(forwardPropogationSentenceData);
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroupComponent: " <<  ownerGroupOrig->groupTypeName << ":" << ownerGroupOrig->groupName << endl;
	SANInodes.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "propagateWordThroughNetworkGroupComponent: activationSequenceCompleted = " << activationSequenceCompleted << endl;
	#endif

	if(forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup)
	{
		result = true;
		
		forwardPropogationSentenceData->activationPathWordFirstActivationMemoryGroupArray[forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW].push_back(SANInodes.convertNeuralNetworkGroupToMemoryActivationGroup(ownerGroup));
		forwardPropogationSentenceData->activationPathWordFirstParseTreeGroupArray[forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW].push_back(activationPathWordCurrentParseTreeGroupOwner);	//OLD: activationPathWordCurrentParseTreeGroup
	}
	if(forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray)
	{
		result = true;
	}
				
	if(activationSequenceCompleted)
	{
		//enable connections to fuzzy groups
		if(forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray)
		{
			//result = true;

			//cout << "propagateWordThroughNetworkGroupComponent: ownerGroup->forwardPropogationWordData.w = " << ownerGroup->forwardPropogationWordData.w << endl;
			//cout << "(forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray)" << endl;
			forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray->push_back(SANInodes.convertNeuralNetworkGroupToMemoryActivationGroup(ownerGroup));
			forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstParseTreeGroupActivationPointArray->push_back(activationPathWordCurrentParseTreeGroupOwner);	
		}
		
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA
		SANInodes.printParseTreeDebugIndentation(layer+1);
		cout << "activationSequenceCompleted" << endl;
		#endif

		SANIForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = NULL;

		activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalDataProspective;	//required to pass wordNounVariantType/wordVerbVariantType
		semanticRelationReturnEntityForwardPropogationSignalDataNew = &(activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData);
		SANInodes.convertNeuralNetworkGroupToMemoryActivationGroup(ownerGroup)->forwardPropogationSignalData = activationPathWordCurrentParseTreeGroupOwner->semanticRelationReturnEntityForwardPropogationSignalData;	//set from semanticRelationReturnEntityForwardPropogationSignalDataProspective

		//record variables for SANIpropagateHeavyOptimisedParse;
		ownerGroup->neuronActive = true;
		//activationPathWordCurrentParseTreeGroupOwner->neuronActive = true;	//not used
		
		bool topLevelGroup = SANInodesGroupClassObject.isTopLevelGroupType(ownerGroup->groupTypeName, ownerGroup->groupTypeReferenceSetType, forwardPropogationSentenceData->isQuery, forwardPropogationSentenceData->parseIsolatedSubreferenceSets);
		if(topLevelGroup)
		{	
			//cout << "topLevelGroup forwardPropogationSentenceData->parseIsolatedSubreferenceSets = " << forwardPropogationSentenceData->parseIsolatedSubreferenceSets << endl;
			
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY
			if(forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW == 0) //forwardPropogationWordData->w == 0)	//verify that all words are captured by the tree
			{
			#endif
				if(forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup)
				{
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
					forwardPropogationSentenceData->maxIterationPropagate = true;	//3g8h	//CHECKTHIS //always take successfully parses that require the least number of iterations
					#endif
					#else
					forwardPropogationSentenceData->finishedPassingSentenceWords = true;
					#endif
					
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					int maxWeight = 0;
					SANIpropagateOperationsParseTree.calculatePerformanceWeightOfParseTree(activationPathWordCurrentParseTreeGroupOwner, &maxWeight);
					if(maxWeight >= forwardPropogationSentenceData->parseTreeMaxWeightPropagate)
					{
						forwardPropogationSentenceData->parseTreeMaxWeightPropagate = maxWeight;
					#endif
					
						forwardPropogationSentenceData->toplevelGroupActivationFound = true;
						#ifdef SANI_PARSE_SAVE_PARSE_TREE
						forwardPropogationSentenceData->topLevelParseTreeGroupPropagate = activationPathWordCurrentParseTreeGroupOwner;
						#endif
						#ifdef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE
						if(!SANIpropagateOperationsParseTree.updatePerformanceGroupSentence(activationPathWordCurrentParseTreeGroupOwner, forwardPropogationSentenceData, layer))
						{
							//result = false;
						}
						#endif
						
						#ifdef SANI_DEBUG_PROPAGATE
						cout << "topLevelGroup found" << endl;
						printBackpropParseTree(forwardPropogationSentenceData->topLevelParseTreeGroupPropagate, 1);
						cout << "end printBackpropParseTree" << endl;
						#endif

						/*
						cout << "topLevelGroup" << endl;
						cout << "finishedPassingSentenceWords (temp exit)" << endl;
						exit(0);
						*/
					
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					}
					#endif
				}
				
			#ifdef SANI_ENFORCE_WORD_CONNECTIVITY	
			}
			#endif
		}
		else
		{
			if(!(forwardPropogationSentenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup))	//CHECKTHIS
			{
				//activation sequence completed (and not top level group), propagate next layer up
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))	//OLD: ownerGroupOrig
				{
					result = true;
				}
			}
		}
	}
	else
	{
		if(forwardPropogationSentenceData->forwardPropogationActivationPointData->generateActivationPointArray)
		{
			forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstActivationMemoryGroupActivationPointArray->push_back(SANInodes.convertNeuralNetworkGroupToMemoryActivationGroup(ownerGroup));
			forwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathWordFirstParseTreeGroupActivationPointArray->push_back(activationPathWordCurrentParseTreeGroupOwner);	
		}	
	}
	
	return result;
}







bool SANIpropagateHeavyOptimisedClass::printBackpropParseTree(SANIGroupParseTree* group, const int level)
{
	#ifdef SANI_DEBUG_PARSE_TREE_PRINT_SUPPORT_RECURSION
	SANIpropagateOperationsParseTree.traceBackpropParseTreePrint(group, 1);
	SANIpropagateOperationsParseTree.resetNeuronBackprop(group, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);	
	#else
	SANInodes.printParseTree(group, level);
	#endif
}














//copyParseTreeGroupComponents = false
void SANIpropagateHeavyOptimisedClass::generateMemoryActiveGroup(SANIGroupNeuralNetwork* activationMemoryGroup, constEffective SANIGroupNeuralNetwork* groupOrig, SANIGroupNeuralNetwork* currentParseTreeGroupTemplate, SANIGroupActivationMemory** activationMemoryGroupNew, SANIGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents)
{
	*activationMemoryGroupNew = SANInodes.convertNeuralNetworkGroupToMemoryActivationGroupNew(activationMemoryGroup);
	
	if(copyParseTreeGroupComponents == true)
	{
		cerr << "SANIpropagateHeavyOptimisedClass::generateMemoryActiveGroup error{}: (copyParseTreeGroupComponents == true)" << endl;
		exit(EXIT_ERROR);
	}
	
	(*activationMemoryGroupNew)->groupOrig = groupOrig;
	*currentParseTreeGroupNew = SANInodes.convertNeuralNetworkGroupToParseTreeGroupNew(currentParseTreeGroupTemplate);
}

//copyParseTreeGroupComponents = true
void SANIpropagateHeavyOptimisedClass::generateMemoryActiveGroup(SANIGroupNeuralNetwork* activationMemoryGroup, constEffective SANIGroupNeuralNetwork* groupOrig, SANIGroupParseTree* currentParseTreeGroup, SANIGroupActivationMemory** activationMemoryGroupNew, SANIGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents)
{
	*activationMemoryGroupNew = SANInodes.convertNeuralNetworkGroupToMemoryActivationGroupNew(activationMemoryGroup);
	
	if(copyParseTreeGroupComponents == false)
	{
		cerr << "SANIpropagateHeavyOptimisedClass::generateMemoryActiveGroup error{}: (copyParseTreeGroupComponents == false)" << endl;
		exit(EXIT_ERROR);
	}
	
	(*activationMemoryGroupNew)->groupOrig = groupOrig;
	*currentParseTreeGroupNew = SANInodes.copyGroup(currentParseTreeGroup);
}


void SANIpropagateHeavyOptimisedClass::printComponents(const vector<SANIComponentNeuralNetwork*>* components)
{		
	for(int i=0; i<components->size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (*components)[i];
		SANInodes.printComponent(currentComponent, 0);
		cout << "currentComponent->componentType = " << currentComponent->componentType << endl;
		if(SANInodesComponentClassObject.componentHasSubcomponents(currentComponent))
		{
			printComponents(&(currentComponent->subComponents)); 
		}
	}
}

#endif
#endif
