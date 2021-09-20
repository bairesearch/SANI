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
 * Project Version: 1q2b 19-September-2021
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
	
	#ifdef SANI_PARSE_SAVE_PARSE_TREE
	forwardPropogationSequenceData.topLevelParseTreeGroupPropagate = NULL;
	#endif
	
	//memory clean forwardPropogationSequenceData.forwardPropogationSequenceElementDataArray [from previous POS permutation];
	for(int i=0; i<forwardPropogationSequenceData.forwardPropogationSequenceElementDataArray.size(); i++)
	{
		SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = forwardPropogationSequenceData.forwardPropogationSequenceElementDataArray[i];
		delete forwardPropogationSequenceElementData;
	}
	forwardPropogationSequenceData.forwardPropogationSequenceElementDataArray.clear();

	for(int w=0; w<sentenceContents->size(); w++)
	{
		SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = new SANIForwardPropogationSequenceElementData();
		forwardPropogationSequenceData.forwardPropogationSequenceElementDataArray.push_back(forwardPropogationSequenceElementData);
	}
	
	forwardPropogationSequenceData.activationPathSequenceElementFirstActivationMemoryGroupArray.resize(sentenceContents->size());
	forwardPropogationSequenceData.activationPathSequenceElementFirstParseTreeGroupArray.resize(sentenceContents->size());

	//SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_PARSE_TREE_GROUP_REF);	//this is required to initialise currentParseTreeGroup for every group (only required for first execution of SANIpropagateHeavyOptimisedClass::executePosRelTranslatorNeuralNetwork)
	
	//SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup = NULL;
		
	if(sentenceContents->size() > 0)
	{
		bool expectToSeeConnectionWithPreviousWordTrace = false;
		bool passThrough = false;
		int layer = 0;
		for(int w=0; w<sentenceContents->size(); w++)
		{
			if(!propagateWordThroughNetworkWordGroupIntro(translatorVariables, w, &forwardPropogationSequenceData, layer))
			{
				result = false;
			}
		}
		
		/*
		cout << "propagateWordThroughNetworkWordGroupIntro{} finished" << endl;
		exit(EXIT_ERROR);
		*/
		if(!propagateWordThroughNetworkGroupIntro(translatorVariables, &forwardPropogationSequenceData))
		{
			result = false;
		}
	}
	else
	{
		cerr << "SANIpropagateHeavyOptimisedClass::executePosRelTranslatorNeuralNetwork{} error: sentenceContents->size() == 0" << endl;
		exit(EXIT_ERROR);		
	}
	
	if(forwardPropogationSequenceData.toplevelGroupActivationFound)
	{
		sentenceValidActivationPath = true;
		result = true;
		
		#ifdef SANI_PARSE_SAVE_PARSE_TREE
		*topLevelParseTreeGroup = forwardPropogationSequenceData.topLevelParseTreeGroupPropagate;
		//cout << "forwardPropogationSequenceData->topLevelParseTreeGroupPropagate" << endl;
		
		#ifdef SANI_DEBUG_PROPAGATE
		cout << "topLevelGroup" << endl;
		printBackpropParseTree(&(forwardPropogationSequenceData.topLevelParseTreeGroupPropagate), 1);
		cout << "end printBackpropParseTree" << endl;
		//cout << "forwardPropogationSequenceData->topLevelParseTreeGroupPropagate->groupName = " << forwardPropogationSequenceData->topLevelParseTreeGroupPropagate->groupName << endl;
		#endif	
		#endif	
		
		#ifdef SANI_PARSE_PERFORMANCE
		*performance = forwardPropogationSequenceData.performance;
		//cout << "forwardPropogationSequenceData.performance = " << forwardPropogationSequenceData.performance << endl;
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
	
	#ifdef SANI_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(sentenceValidActivationPath)
	{
		SANIpropagateOperationsParseTree.traceBackpropParseTreeSetTraced(forwardPropogationSequenceData.topLevelParseTreeGroupPropagate, 1);	//added GIA3g6aTEMP32 - set all parseTreeGroup groups in final heirachy to neuronTraced to prevent their memory from being deleted during SANIpropagateOperations.resetAllNeuronComponents
	}
	#endif
	for(int w=0; w<sentenceContents->size(); w++)	//start at every w in sentence
	{
		vector<SANIGroupActivationMemory*> activationPathSequenceElementFirstActivationMemoryGroupArray = forwardPropogationSequenceData.activationPathSequenceElementFirstActivationMemoryGroupArray[w];
		vector<SANIGroupParseTree*> activationPathSequenceElementFirstParseTreeGroupArray = forwardPropogationSequenceData.activationPathSequenceElementFirstParseTreeGroupArray[w];
		if(activationPathSequenceElementFirstActivationMemoryGroupArray.size() != activationPathSequenceElementFirstParseTreeGroupArray.size())
		{	
			cerr << "SANIpropagateHeavyOptimisedClass::executePosRelTranslatorNeuralNetwork error: activationPathSequenceElementFirstActivationMemoryGroupArray.size() != activationPathSequenceElementFirstParseTreeGroupArray.size()" << endl;
			exit(EXIT_ERROR);
		} 
		for(int i=0; i<activationPathSequenceElementFirstActivationMemoryGroupArray.size(); i++) 
		{
			SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroup = activationPathSequenceElementFirstActivationMemoryGroupArray[i];
			SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup = activationPathSequenceElementFirstParseTreeGroupArray[i];
			SANIpropagateOperationsParseTree.resetGroupParseTreeGroupRef(activationPathSequenceElementFirstActivationMemoryGroup, activationPathSequenceElementFirstParseTreeGroup, true);
		}
		activationPathSequenceElementFirstActivationMemoryGroupArray.clear();
		activationPathSequenceElementFirstParseTreeGroupArray.clear();
	}
	forwardPropogationSequenceData.activationPathSequenceElementFirstActivationMemoryGroupArray.clear();
	forwardPropogationSequenceData.activationPathSequenceElementFirstParseTreeGroupArray.clear();
	#ifdef SANI_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(sentenceValidActivationPath)
	{
		SANIpropagateOperationsParseTree.resetNeuronBackprop(forwardPropogationSequenceData.topLevelParseTreeGroupPropagate, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
	}
	#endif
	
	for(int w=0; w<sentenceContents->size(); w++)
	{
		SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = (forwardPropogationSequenceData.forwardPropogationSequenceElementDataArray[w]);
		#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BASIC_MAX_1_CONSECUTIVE_ISOLATED_WORDS
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

	


bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkWordGroupIntro(const SANItranslatorVariablesClass* translatorVariables, int w, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer)
{
	bool result = false;

	/*
	more general algorithm;
	(1) start at every w in sentence
		propagate sequenceElement up network as high as it goes
		store the parse tree group (and memory activation group) in memory
			store also the num sequenceElements n encapsulated by the parse tree group (for efficiency)
		NO: parse to the right and generate sets of parse groups (limited by previousWordFound), including single sequenceElements
	*/
		
	//SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup, SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroup

	LRPpreprocessorPlainTextWord* currentWord = forwardPropogationSequenceData->sentenceContents->at(w);

	SANInodes.printParseTreeDebugIndentation(layer);
	#ifdef SANI_DEBUG_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
	#ifndef SANI_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "currentWord->POSambiguityInfo = " << LRPpreprocessorPOStagger.printPOSambiguityInfo(currentWord->POSambiguityInfo) << endl;
	#endif
	
	//prepare sequenceElement specific variables:
	SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = (forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray[w]);
	forwardPropogationSequenceElementData->wordReference = currentWord;
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
				if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer))
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
			if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer))
			{
				result = true;
			}
		}
		#else
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
		int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_NOUN;
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer))
		{
			result = true;
		}	
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
		if(LRPpreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
		{		
			//cout << "isMidSentenceUppercaseWordLikelyProperNoun" << endl;
			int wordPOStype = LRP_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT;
			if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, wordPOStype, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer))
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
		cout << "SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer))
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
		cout << "SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer))
		{
			result = true;
		}
	}
	else if(SANIformation.findWordInGroupMap(currentWord->tagName, SANIformation.getInputLayerSectionExplicitWordMap(), &inputLayerGroup))
	{
		forwardPropogationSequenceElementData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	
		#ifdef SANI_DEBUG_PROPAGATE
		SANInodes.printParseTreeDebugIndentation(layer);
		cout << "SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT" << endl;
		#endif
		if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer))
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
							cerr << "SANIClass::propagateWordThroughNetworkIntro{} error: wordPOStype cannot be derived from GIAtokenLayerClassName, GIAtokenLayerClassName = " << GIAtokenLayerClassName << endl;
							exit(EXIT_ERROR);
						}
							
						#ifdef SANI_DEBUG_PROPAGATE
						SANInodes.printParseTreeDebugIndentation(layer);
						cout << "SANIClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS" << endl;
						#endif
						if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClassType, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer))
						{
							result = true;
						}
						if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, currentGroupInInputLayerSectionTokensLayerClass, &forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer))	//what is this for?
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

	
bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkWordGroupInit(const SANItranslatorVariablesClass* translatorVariables, int w, int wordPOStype, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer)
{
	bool result = false;
	
	SANIGroupNeuralNetwork* inputLayerGroup = SANIformation.getInputGroupLayerSection(SANIformation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);
	#ifdef SANI_DEBUG_PROPAGATE
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationSequenceElementData->wordPOStype = wordPOStype;
	#endif
	
	if(propagateWordThroughNetworkWordGroupInit(translatorVariables, w, inputLayerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer))
	{
		result = true;
	}
	
	return result;
}	
	
bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkWordGroupInit(const SANItranslatorVariablesClass* translatorVariables, int w, SANIGroupNeuralNetwork* inputLayerGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer)
{
	bool result = true;
	
	//cout << "activationPathSequenceElementFirstParseTreeGroupNext->inputGroupString" << endl;
	
	SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroup = NULL;
	SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup = NULL;
	generateMemoryActiveGroup(inputLayerGroup, inputLayerGroup, inputLayerGroup, &activationPathSequenceElementFirstActivationMemoryGroup, &activationPathSequenceElementFirstParseTreeGroup, false);
	/*
	SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroup = SANInodes.copyGroup(inputLayerGroup);
	activationPathSequenceElementFirstActivationMemoryGroup->groupOrig = inputLayerGroup;
	SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup = new SANIGroupParseTree(*inputLayerGroup);
	*/
	
	forwardPropogationSequenceData->activationPathSequenceElementFirstActivationMemoryGroupArray[w].push_back(activationPathSequenceElementFirstActivationMemoryGroup);
	forwardPropogationSequenceData->activationPathSequenceElementFirstParseTreeGroupArray[w].push_back(activationPathSequenceElementFirstParseTreeGroup);
	
	activationPathSequenceElementFirstActivationMemoryGroup->layer = layer;
	activationPathSequenceElementFirstActivationMemoryGroup->numberSequenceElementsInSet = 1;
	activationPathSequenceElementFirstActivationMemoryGroup->forwardPropogationSignalData = *forwardPropogationSignalData;
	activationPathSequenceElementFirstActivationMemoryGroup->forwardPropogationSequenceElementData = *forwardPropogationSequenceElementData;	//(forwardPropogationSequenceData->forwardPropogationSequenceElementDataArray[w]);
	activationPathSequenceElementFirstActivationMemoryGroup->neuronActive = true;
	activationPathSequenceElementFirstActivationMemoryGroup->sequenceElementGroupNeuron = true;
	activationPathSequenceElementFirstActivationMemoryGroup->inputGroupString = true;
		//TODO: ensure activationPathSequenceElementFirstActivationMemoryGroup->forwardPropogationSequenceElementData is always set correctly
		
	return result;
}				
	

bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupIntro(const SANItranslatorVariablesClass* translatorVariables, SANIForwardPropogationSequenceData* forwardPropogationSequenceData)
{
	bool result = false;
	
	/*
	more general algorithm;
	(2) repeat until found top level group with all encapsulated sequenceElements:
		start at every w in sentence
			for each existing parseTreeGroup stored at w
				for each adjacent parseTreeGroup available (that does not skip an encapsulate sequenceElement), ie stored at w+n (where n is number of sequenceElements in the left parse group)
					activate their appropriate top level components in the memory group
					see if the second activated group propagates up into the second
	*/
	
	vector<LRPpreprocessorPlainTextWord*>* sentenceContents = forwardPropogationSequenceData->sentenceContents;

	int maxIterationIndex = SANI_MAX_ITERATIONS;
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
	forwardPropogationSequenceData->parseTreeMaxWeightPropagate = 0;
	#endif
	forwardPropogationSequenceData->maxIterationPropagate = false;
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_WEAK
	bool limitedIterations = false;
	#endif
	#endif
	bool foundTopLevelGroup = false;
	bool stillGeneratingSets = true;
	int connectIterationIndex = 1;
	while(!foundTopLevelGroup && stillGeneratingSets)	//repeat until found top level group with all encapsulated sequenceElements:
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
			
			vector<SANIGroupActivationMemory*> activationPathSequenceElementFirstActivationMemoryGroupArray = forwardPropogationSequenceData->activationPathSequenceElementFirstActivationMemoryGroupArray[w];
			vector<SANIGroupParseTree*> activationPathSequenceElementFirstParseTreeGroupArray = forwardPropogationSequenceData->activationPathSequenceElementFirstParseTreeGroupArray[w];

			#ifdef SANI_ENFORCE_STRICT_ITERATION_INDICES
			int groupArraySize = activationPathSequenceElementFirstActivationMemoryGroupArray.size();
			for(int i=0; i<groupArraySize; i++)
			#else
			for(int i=0; i<activationPathSequenceElementFirstActivationMemoryGroupArray.size(); i++) 	//for each existing parseTreeGroup stored at w
			#endif
			{
				SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroup = activationPathSequenceElementFirstActivationMemoryGroupArray[i];
				SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup = activationPathSequenceElementFirstParseTreeGroupArray[i];
				
				int wNext = w + activationPathSequenceElementFirstActivationMemoryGroup->numberSequenceElementsInSet;
				#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
				cout << "w = " << w << endl;
				cout << "wNext = " << wNext << endl;
				#endif
				
				if(wNext < sentenceContents->size())
				{	
					#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
					SANInodes.printParseTreeDebugIndentation(activationPathSequenceElementFirstActivationMemoryGroup->layer);
					cout << "propagateWordThroughNetworkGroupIntro activationPathSequenceElementFirstActivationMemoryGroup: " <<  activationPathSequenceElementFirstActivationMemoryGroup->groupTypeName << ":" << activationPathSequenceElementFirstActivationMemoryGroup->groupName << endl;		
					#endif

					//activate their appropriate top level components in the memory group
					int layer = activationPathSequenceElementFirstActivationMemoryGroup->layer;
					SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData = &(activationPathSequenceElementFirstActivationMemoryGroup->forwardPropogationSequenceElementData);
					//cout << "forwardPropogationSequenceElementData->sequenceIndex = " << forwardPropogationSequenceElementData->sequenceIndex << endl;
					SANIForwardPropogationSignalData* forwardPropogationSignalData = &(activationPathSequenceElementFirstActivationMemoryGroup->forwardPropogationSignalData);
					SANIForwardPropogationActivationPointData forwardPropogationActivationPointData;
					#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
					forwardPropogationActivationPointData.activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray = &(activationPathSequenceElementFirstActivationMemoryGroup->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray);
					forwardPropogationActivationPointData.activationPathSequenceElementFirstParseTreeGroupActivationPointArray = &(activationPathSequenceElementFirstParseTreeGroup->activationPathSequenceElementFirstParseTreeGroupActivationPointArray);
					#else
					vector<SANIGroupActivationMemory*> activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray;
					vector<SANIGroupParseTree*> activationPathSequenceElementFirstParseTreeGroupActivationPointArray;	
					forwardPropogationActivationPointData.activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray = &activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray;
					forwardPropogationActivationPointData.activationPathSequenceElementFirstParseTreeGroupActivationPointArray = &activationPathSequenceElementFirstParseTreeGroupActivationPointArray;
					#endif
					forwardPropogationSequenceData->forwardPropogationActivationPointData = &forwardPropogationActivationPointData;
					forwardPropogationActivationPointData.generateActivationPointArray = true;

					#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
					if(!(activationPathSequenceElementFirstActivationMemoryGroup->pointArrayGenerated))
					{
					#endif	
						#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						if(!activationPathSequenceElementFirstActivationMemoryGroup->firstLevelActivationPointAdded)
						{
							activationPathSequenceElementFirstActivationMemoryGroup->firstLevelActivationPointAdded = true;
						#endif
							//add the first level activationPathSequenceElementFirstActivationMemoryGroup to activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray (this isn't done by propagateWordThroughNetworkGroup):
							forwardPropogationActivationPointData.activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray->push_back(activationPathSequenceElementFirstActivationMemoryGroup);
							forwardPropogationActivationPointData.activationPathSequenceElementFirstParseTreeGroupActivationPointArray->push_back(activationPathSequenceElementFirstParseTreeGroup);
						#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						}
						#endif

						if(activationPathSequenceElementFirstActivationMemoryGroup->neuronActive)
						{
							#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
							activationPathSequenceElementFirstActivationMemoryGroup->pointArrayGenerated = true;
							#endif
							
							SANIGroupNeuralNetwork* activationPathSequenceElementFirstActivationMemoryGroupBase = activationPathSequenceElementFirstActivationMemoryGroup;
							if(propagateWordThroughNetworkGroup(translatorVariables, activationPathSequenceElementFirstActivationMemoryGroupBase, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroup))
							{
								result = true;
							}
						}

					#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP	
					}
					#endif


					vector<SANIGroupActivationMemory*> activationPathSequenceElementFirstActivationMemoryGroupArrayNext = forwardPropogationSequenceData->activationPathSequenceElementFirstActivationMemoryGroupArray[wNext];
					vector<SANIGroupParseTree*> activationPathSequenceElementFirstParseTreeGroupArrayNext = forwardPropogationSequenceData->activationPathSequenceElementFirstParseTreeGroupArray[wNext];
					#ifdef SANI_ENFORCE_STRICT_ITERATION_INDICES
					int groupArrayNextSize = activationPathSequenceElementFirstActivationMemoryGroupArrayNext.size();
					for(int i2=0; i2<groupArrayNextSize; i2++)
					#else
					for(int i2=0; i2<activationPathSequenceElementFirstActivationMemoryGroupArrayNext.size(); i2++) 	//for each adjacent parseTreeGroup available (that does not skip an encapsulate sequenceElement), ie stored at w+n (where n is number of sequenceElements in the left parse group)
					#endif
					{
						SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroupNext = activationPathSequenceElementFirstActivationMemoryGroupArrayNext[i2];
						SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroupNext = activationPathSequenceElementFirstParseTreeGroupArrayNext[i2];

						#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
						SANInodes.printParseTreeDebugIndentation(activationPathSequenceElementFirstActivationMemoryGroupNext->layer);
						cout << "propagateWordThroughNetworkGroupIntro activationPathSequenceElementFirstActivationMemoryGroupNext: " <<  activationPathSequenceElementFirstActivationMemoryGroupNext->groupTypeName << ":" << activationPathSequenceElementFirstActivationMemoryGroupNext->groupName << endl;		
						#endif

						//activate their appropriate top level components in the memory group
						int layerNext = activationPathSequenceElementFirstActivationMemoryGroupNext->layer;
						SANIForwardPropogationSequenceElementData* forwardPropogationWordDataNext = &(activationPathSequenceElementFirstActivationMemoryGroupNext->forwardPropogationSequenceElementData);
						//cout << "forwardPropogationWordDataNext->w = " << forwardPropogationWordDataNext->w << endl;
						SANIForwardPropogationSignalData* forwardPropogationSignalDataNext = &(activationPathSequenceElementFirstActivationMemoryGroupNext->forwardPropogationSignalData);
						SANIForwardPropogationActivationPointData forwardPropogationActivationPointDataNext;
						#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						forwardPropogationActivationPointDataNext.activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray = &(activationPathSequenceElementFirstActivationMemoryGroupNext->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray);
						forwardPropogationActivationPointDataNext.activationPathSequenceElementFirstParseTreeGroupActivationPointArray = &(activationPathSequenceElementFirstParseTreeGroupNext->activationPathSequenceElementFirstParseTreeGroupActivationPointArray);
						#else
						vector<SANIGroupActivationMemory*> activationPathSequenceElementFirstActivationMemoryGroupActivationPointArrayNext;
						vector<SANIGroupParseTree*> activationPathSequenceElementFirstParseTreeGroupActivationPointArrayNext;	
						forwardPropogationActivationPointDataNext.activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray = &activationPathSequenceElementFirstActivationMemoryGroupActivationPointArrayNext;
						forwardPropogationActivationPointDataNext.activationPathSequenceElementFirstParseTreeGroupActivationPointArray = &activationPathSequenceElementFirstParseTreeGroupActivationPointArrayNext;
						#endif
						forwardPropogationSequenceData->forwardPropogationActivationPointData = &forwardPropogationActivationPointDataNext;
						forwardPropogationActivationPointDataNext.generateActivationPointArray = true;

						#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						if(!(activationPathSequenceElementFirstActivationMemoryGroupNext->pointArrayGenerated))
						{
						#endif
							if(activationPathSequenceElementFirstActivationMemoryGroupNext->neuronActive)
							{
								#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
								activationPathSequenceElementFirstActivationMemoryGroupNext->pointArrayGenerated = true;
								#endif

								#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
								if(!activationPathSequenceElementFirstActivationMemoryGroupNext->firstLevelActivationPointAdded)
								{
									activationPathSequenceElementFirstActivationMemoryGroupNext->firstLevelActivationPointAdded = true;
								#endif
									//if(activationPathSequenceElementFirstActivationMemoryGroupNext->inputGroupString)
									//{	
									//add the first level activationPathSequenceElementFirstActivationMemoryGroupNext to activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray (this isn't done by propagateWordThroughNetworkGroup):
									forwardPropogationActivationPointDataNext.activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray->push_back(activationPathSequenceElementFirstActivationMemoryGroupNext);	//OLD: add original unpropagated group to forwardPropogationActivationPointDataNext (in case of string)
									forwardPropogationActivationPointDataNext.activationPathSequenceElementFirstParseTreeGroupActivationPointArray->push_back(activationPathSequenceElementFirstParseTreeGroupNext);
									//}
								#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP	
								}
								#endif
								
								if(propagateWordThroughNetworkGroup(translatorVariables, activationPathSequenceElementFirstActivationMemoryGroupNext, forwardPropogationSignalDataNext, forwardPropogationWordDataNext, forwardPropogationSequenceData, layerNext, activationPathSequenceElementFirstParseTreeGroupNext))
								{
									result = true;
								}
							}
						#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						}
						#endif


						if(!(forwardPropogationSequenceData->finishedPassingSequenceElements))
						{
							//see if the second activated set point propagates up immediately into the first				
							if(connectToPreviousActivationGroup(translatorVariables, activationPathSequenceElementFirstActivationMemoryGroup, activationPathSequenceElementFirstActivationMemoryGroupNext, &forwardPropogationActivationPointData, &forwardPropogationActivationPointDataNext, forwardPropogationSignalDataNext, forwardPropogationSequenceElementData, forwardPropogationWordDataNext, forwardPropogationSequenceData, layerNext))
							{
								result = true;
							}
						}
						
						#ifndef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
						for(int i=1; i<activationMemoryGroup->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArrayNext.size(); i++)	//i=1; never remove the first activationPathSequenceElementFirstActivationMemoryGroupActivationPoint (as this will correspond to activationPathSequenceElementFirstActivationMemoryGroup)
						{
							SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroupActivationPointTemp = activationPathSequenceElementFirstActivationMemoryGroupActivationPointArrayNext[i];
							SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroupActivationPointTemp = activationPathSequenceElementFirstParseTreeGroupActivationPointArrayNext[i];
							if(!activationPathSequenceElementFirstActivationMemoryGroupActivationPointTemp->firstLevelActivationPointAdded)
							{
								SANInodes.deleteGroup(activationPathSequenceElementFirstActivationMemoryGroupActivationPointTemp);
								if(!(currentParseTreeGroup->neuronTraced))	//parseTreeGroups in activationPointArray >i=0 can still be added to final parseTree
								{
									SANInodes.deleteGroup(activationPathSequenceElementFirstParseTreeGroupActivationPointTemp);
								}
							}
						}
						#endif
					}
				}
				
				#ifndef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP
				for(int i=1; i<activationMemoryGroup->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray.size(); i++)	//i=1; never remove the first activationPathSequenceElementFirstActivationMemoryGroupActivationPoint (as this will correspond to activationPathSequenceElementFirstActivationMemoryGroup)
				{
					SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroupActivationPointTemp = activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray[i];
					SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroupActivationPointTemp = activationPathSequenceElementFirstParseTreeGroupActivationPointArray[i];
					if(!activationPathSequenceElementFirstActivationMemoryGroupActivationPointTemp->firstLevelActivationPointAdded)
					{
						SANInodes.deleteGroup(activationPathSequenceElementFirstActivationMemoryGroupActivationPointTemp);
						if(!(currentParseTreeGroup->neuronTraced))	//parseTreeGroups in activationPointArray >i=0 can still be added to final parseTree
						{
							SANInodes.deleteGroup(activationPathSequenceElementFirstParseTreeGroupActivationPointTemp);
						}
					}
				}
				#endif
			}
		}
		
		if(forwardPropogationSequenceData->finishedPassingSequenceElements)
		{
			stillGeneratingSets = false;
		}
		if(connectIterationIndex >= maxIterationIndex)
		{
			stillGeneratingSets = false;
		}
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
		if(forwardPropogationSequenceData->maxIterationPropagate)
		{
			#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_WEAK
			forwardPropogationSequenceData->maxIterationPropagate = false;
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

//CHECKTHIS; check parameter forwardPropogationSequenceElementData
bool SANIpropagateHeavyOptimisedClass::connectToPreviousActivationGroup(const SANItranslatorVariablesClass* translatorVariables, SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroup, SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroupNext, const SANIForwardPropogationActivationPointData* forwardPropogationActivationPointData, const SANIForwardPropogationActivationPointData* forwardPropogationActivationPointDataNext, SANIForwardPropogationSignalData* forwardPropogationSignalDataNext, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceElementData* forwardPropogationWordDataNext, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer)
{
	bool result = false;
	
	for(int i1=0; i1<forwardPropogationActivationPointDataNext->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray->size(); i1++)
	{
		SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroupNextPoint = (*forwardPropogationActivationPointDataNext->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray)[i1];
		SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroupNextPoint = (*forwardPropogationActivationPointDataNext->activationPathSequenceElementFirstParseTreeGroupActivationPointArray)[i1];

		if(activationPathSequenceElementFirstActivationMemoryGroupNextPoint->neuronActive)	//added 3g8aTEMP22
		{			
			for(int i2=0; i2<activationPathSequenceElementFirstActivationMemoryGroupNextPoint->SANIfrontComponentConnectionList.size(); i2++)
			{
				SANIComponentNeuralNetwork* currentComponentNext = (activationPathSequenceElementFirstActivationMemoryGroupNextPoint->SANIfrontComponentConnectionList)[i2];
				SANIGroupNeuralNetwork* ownerGroupNext = currentComponentNext->ownerGroup;
				
				for(int i3=0; i3<forwardPropogationActivationPointData->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray->size(); i3++)
				{
					SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroupPoint = (*forwardPropogationActivationPointData->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray)[i3];
					SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroupPoint = (*forwardPropogationActivationPointData->activationPathSequenceElementFirstParseTreeGroupActivationPointArray)[i3];
					SANIGroupNeuralNetwork* groupOrig = activationPathSequenceElementFirstActivationMemoryGroupPoint->groupOrig;

					if(groupOrig == ownerGroupNext)
					{
						//found matching group names between set and next set (see if their memory groups are connected)

						#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
						SANInodes.printParseTreeDebugIndentation(layer);
						cout << "connectToPreviousActivationGroup (groupOrig == ownerGroupNext): activationPathSequenceElementFirstActivationMemoryGroupPoint: " <<  activationPathSequenceElementFirstActivationMemoryGroupPoint->groupTypeName << ":" << activationPathSequenceElementFirstActivationMemoryGroupPoint->groupName << endl;		
						SANInodes.printParseTreeDebugIndentation(layer);
						cout << "connectToPreviousActivationGroup (groupOrig == ownerGroupNext): activationPathSequenceElementFirstActivationMemoryGroupNextPoint: " <<  activationPathSequenceElementFirstActivationMemoryGroupNextPoint->groupTypeName << ":" << activationPathSequenceElementFirstActivationMemoryGroupNextPoint->groupName << endl;		
						#endif

						SANIComponentNeuralNetwork* ownerComponentNext = currentComponentNext;
						int componentIndex = currentComponentNext->componentIndex;
						int ownerComponentIndex = componentIndex;
						if(currentComponentNext->isSubcomponent)
						{
							ownerComponentNext = currentComponentNext->ownerComponent;
							ownerComponentIndex = ownerComponentNext->componentIndex;
						}

						SANIGroupActivationMemory* ownerGroup = activationPathSequenceElementFirstActivationMemoryGroupPoint;
						SANIComponentNeuralNetwork* ownerComponent = ownerGroup->components[ownerComponentIndex];
						SANIComponentNeuralNetwork* currentComponent = ownerComponent;
						if(currentComponentNext->isSubcomponent)
						{
							currentComponent = ownerComponent->subComponents[componentIndex];
						}

						forwardPropogationSequenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup = true;
						forwardPropogationSequenceData->forwardPropogationActivationPointData->generateActivationPointArray = false;	//CHECKTHIS
						forwardPropogationSequenceData->forwardPropogationActivationPointData->activationPathW = forwardPropogationSequenceElementData->sequenceIndex;
						
						
						//forwardPropogationActivationPointDataNext->connectToPreviousActivationGroup = true;
						
						if(!(forwardPropogationSequenceData->finishedPassingSequenceElements))
						{
							if(propagateWordThroughNetworkGroupSelect(translatorVariables, ownerGroup, ownerGroup->groupOrig, currentComponent, forwardPropogationSignalDataNext, forwardPropogationWordDataNext, forwardPropogationSequenceData, layer, activationPathSequenceElementFirstParseTreeGroupNextPoint, activationPathSequenceElementFirstParseTreeGroupPoint))
							{
								result = true;

								#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
								cout << "connectToPreviousActivationGroup: propagateWordThroughNetworkGroupSelect passed" << endl;
								#endif

								//FUTURE: ideally this should be done (set correctly originally) in SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponent:
								SANIGroupActivationMemory* newGroup = forwardPropogationSequenceData->activationPathSequenceElementFirstActivationMemoryGroupArray[forwardPropogationSequenceElementData->sequenceIndex].back();	//forwardPropogationSequenceData->forwardPropogationActivationPointData->activationPathW
								newGroup->numberSequenceElementsInSet = activationPathSequenceElementFirstActivationMemoryGroup->numberSequenceElementsInSet + activationPathSequenceElementFirstActivationMemoryGroupNext->numberSequenceElementsInSet;	//CHECKTHIS
								newGroup->forwardPropogationSequenceElementData.sequenceIndex = forwardPropogationSequenceElementData->sequenceIndex;
								
								#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
								cout << "\tnewGroup: " <<  newGroup->groupTypeName << ":" << newGroup->groupName << endl;	
								cout << "\tnewGroup->pointArrayGenerated: " <<  newGroup->pointArrayGenerated << endl;
								cout << "\tnewGroup->numberSequenceElementsInSet = " << newGroup->numberSequenceElementsInSet << endl;
								cout << "\tforwardPropogationWordData->w = " << forwardPropogationSequenceElementData->sequenceIndex << endl;
								cout << "\tnewGroup->forwardPropogationSequenceElementData.w = " << newGroup->forwardPropogationSequenceElementData.w << endl;
								cout << "\tforwardPropogationSentenceData->forwardPropogationActivationPointData->activationPathW = " << forwardPropogationSequenceData->forwardPropogationActivationPointData->activationPathW << endl;
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
	


bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroup(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup)
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
			if(!(forwardPropogationSequenceData->finishedPassingSequenceElements))
			{
				SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i];
				SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
				SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = SANInodes.convertNeuralNetworkGroupToParseTreeGroupNew(ownerGroup);
				activationPathWordCurrentParseTreeGroupOwner->components.clear();	//dont wish to copy subcomponents into currentParseTreeGroupNew;
				
				if(propagateWordThroughNetworkGroupSelect(translatorVariables, ownerGroup, ownerGroup, currentComponent, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner))
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
	
		
bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupSelect(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* ownerGroup, constEffective SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* currentComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner)
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
		if(SANIpropagateOperations.componentTests1(ownerComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData))	//static component tests (component activations irrelevant)
		{
		#endif
		
			//cout << "ownerGroup->groupTypeName = " << ownerGroup->groupTypeName  << endl;
		
			SANIGroupActivationMemory* activationMemoryGroupNew = NULL;
			SANIGroupParseTree* currentParseTreeGroupNew = NULL;
			
			generateMemoryActiveGroup(ownerGroup, ownerGroupOrig, activationPathWordCurrentParseTreeGroupOwner, &activationMemoryGroupNew, &currentParseTreeGroupNew, true);			
			
			//required because ownerGroup is often equivalent to ownerGroupOrig:
			activationMemoryGroupNew->forwardPropogationSequenceElementData = *forwardPropogationSequenceElementData;
			//activationMemoryGroupNew->forwardPropogationSignalData = *forwardPropogationSignalData;	//set from semanticRelationReturnEntityForwardPropogationSignalDataProspective
			activationMemoryGroupNew->layer = layer;
			activationMemoryGroupNew->neuronActive = false;
			//activationMemoryGroupNew->pointArrayGenerated = false;
			activationMemoryGroupNew->firstLevelActivationPointAdded = false;
			activationMemoryGroupNew->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray.clear();
			currentParseTreeGroupNew->activationPathSequenceElementFirstParseTreeGroupActivationPointArray.clear();
			
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
				currentComponent->candidateStringMatch = forwardPropogationSequenceElementData->wordReference;
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
							if(lastComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex == forwardPropogationSequenceElementData->sequenceIndex - 1)
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

			if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, layer, activationPathSequenceElementCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, &debug))
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
	


bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponentWrapper(const SANItranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, constEffective SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const SANIposRelTranslatorDebug* debug)
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
					//cout << "SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroup: propagateWordThroughNetworkGroupVerifyComponentSequenceOrActivationReady" << endl;
					#endif
				}
			}
			else if(ownerComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT)
			{
				if(SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceRepeatActivationReady(forwardPropogationSequenceElementData, forwardPropogationSequenceData, currentComponent, &(ownerComponent->subComponents), &repeatDetected))
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
			if(!firstActiveComponentInGroup || SANIpropagateOperations.componentTests2(ownerGroupOrig, activationPathSequenceElementCurrentParseTreeGroup, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData))
			{
			#endif
				if(propagateWordThroughNetworkGroupComponent(translatorVariables, currentComponent, ownerGroup, ownerGroupOrig, ownerComponent, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, activationSequenceCompleted, layer, repeatDetected, activationPathSequenceElementCurrentParseTreeGroup, activationPathWordCurrentParseTreeGroupOwner, debug))
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
bool SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponent(const SANItranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, constEffective SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const SANIposRelTranslatorDebug* debug)
{
	bool result = false;
	
	//cout << "4: SANIpropagateHeavyOptimisedClass::propagateWordThroughNetworkGroupComponent: " <<  ownerGroup->groupTypeName << ":" << ownerGroup->groupName << endl;		
		
	ownerComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActive = true;	//if hasSubComponents
	ownerComponent->neuronComponentConnectionActiveSequenceElementRecord = forwardPropogationSequenceElementData;
	currentComponent->neuronComponentConnectionActiveSequenceElementRecord = forwardPropogationSequenceElementData;

	#ifdef SANI_PARSE
	//create new parseTreeGroup
	//copied from SANIpropagateHeavyOptimisedInverseClass::generateRulesGroupTreeComponents;
	SANIComponentParseTree* newParseComponent = SANInodes.convertNeuralNetworkComponentToParseTreeComponentNew(currentComponent);	//new SANIComponentParseTree(*SANIpropagateOperations.convertNeuralNetworkComponentToParseTreeComponent(currentComponent));	//copy rules component
	newParseComponent->componentRef = currentComponent;	
	newParseComponent->neuronComponentConnectionActive = true;
	newParseComponent->neuronComponentConnectionActiveSequenceElementRecord = forwardPropogationSequenceElementData;
	newParseComponent->parseTreeGroupRef = activationPathSequenceElementCurrentParseTreeGroup;
	if(newParseComponent->groupRefName == "")
	{
		newParseComponent->groupRefName = activationPathSequenceElementCurrentParseTreeGroup->groupName;
	}
	//currentWord->wordPOStypeInferred = forwardPropogationSequenceElementData->wordPOStype;	//NO: this is required to quickly check wordPOStypeInferred of previous sequenceElements in current parse tree	//this will be set later by GIAposRelTranslatorClass::transferParseTreePOStypeInferredToWordList based on parseComponent->wordPOStypeInferred
	newParseComponent->wordPOStypeInferred = forwardPropogationSequenceElementData->wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
	activationPathWordCurrentParseTreeGroupOwner->components.push_back(newParseComponent);
	#endif

	#ifdef SANI_DEBUG_PROPAGATE_EXTRA4
	//bool sentenceWordDataFullyConnected = SANIpropagateOperations.isSequenceWordDataFullyConnected(forwardPropogationSequenceData);
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "4: propagateWordThroughNetworkGroupComponent: " <<  ownerGroupOrig->groupTypeName << ":" << ownerGroupOrig->groupName << endl;
	SANInodes.printComponent(currentComponent, layer+1);
	#endif
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA5
	SANInodes.printParseTreeDebugIndentation(layer+1);
	cout << "propagateWordThroughNetworkGroupComponent: activationSequenceCompleted = " << activationSequenceCompleted << endl;
	#endif

	if(forwardPropogationSequenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup)
	{
		result = true;
		
		forwardPropogationSequenceData->activationPathSequenceElementFirstActivationMemoryGroupArray[forwardPropogationSequenceData->forwardPropogationActivationPointData->activationPathW].push_back(SANInodes.convertNeuralNetworkGroupToMemoryActivationGroup(ownerGroup));
		forwardPropogationSequenceData->activationPathSequenceElementFirstParseTreeGroupArray[forwardPropogationSequenceData->forwardPropogationActivationPointData->activationPathW].push_back(activationPathWordCurrentParseTreeGroupOwner);	//OLD: activationPathSequenceElementCurrentParseTreeGroup
	}
	if(forwardPropogationSequenceData->forwardPropogationActivationPointData->generateActivationPointArray)
	{
		result = true;
	}
				
	if(activationSequenceCompleted)
	{
		//enable connections to fuzzy groups
		if(forwardPropogationSequenceData->forwardPropogationActivationPointData->generateActivationPointArray)
		{
			//result = true;

			//cout << "propagateWordThroughNetworkGroupComponent: ownerGroup->forwardPropogationSequenceElementData.w = " << ownerGroup->forwardPropogationSequenceElementData.w << endl;
			//cout << "(forwardPropogationSequenceData->forwardPropogationActivationPointData->generateActivationPointArray)" << endl;
			forwardPropogationSequenceData->forwardPropogationActivationPointData->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray->push_back(SANInodes.convertNeuralNetworkGroupToMemoryActivationGroup(ownerGroup));
			forwardPropogationSequenceData->forwardPropogationActivationPointData->activationPathSequenceElementFirstParseTreeGroupActivationPointArray->push_back(activationPathWordCurrentParseTreeGroupOwner);	
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
		
		bool topLevelGroup = SANInodesGroupClassObject.isTopLevelGroupType(ownerGroup->groupTypeName, ownerGroup->groupTypeReferenceSetType, forwardPropogationSequenceData->isQuery, forwardPropogationSequenceData->parseIsolatedSubreferenceSets);
		if(topLevelGroup)
		{	
			//cout << "topLevelGroup forwardPropogationSequenceData->parseIsolatedSubreferenceSets = " << forwardPropogationSequenceData->parseIsolatedSubreferenceSets << endl;
			
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY
			if(forwardPropogationSequenceData->forwardPropogationActivationPointData->activationPathW == 0) //forwardPropogationSequenceElementData->sequenceIndex == 0)	//verify that all sequenceElements are captured by the tree
			{
			#endif
				if(forwardPropogationSequenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup)
				{
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS
					forwardPropogationSequenceData->maxIterationPropagate = true;	//3g8h	//CHECKTHIS //always take successfully parses that require the least number of iterations
					#endif
					#else
					forwardPropogationSequenceData->finishedPassingSequenceElements = true;
					#endif
					
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					int maxWeight = 0;
					SANIpropagateOperationsParseTree.calculatePerformanceWeightOfParseTree(activationPathWordCurrentParseTreeGroupOwner, &maxWeight);
					if(maxWeight >= forwardPropogationSequenceData->parseTreeMaxWeightPropagate)
					{
						forwardPropogationSequenceData->parseTreeMaxWeightPropagate = maxWeight;
					#endif
					
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
						
						#ifdef SANI_DEBUG_PROPAGATE
						cout << "topLevelGroup found" << endl;
						printBackpropParseTree(forwardPropogationSequenceData->topLevelParseTreeGroupPropagate, 1);
						cout << "end printBackpropParseTree" << endl;
						#endif

						/*
						cout << "topLevelGroup" << endl;
						cout << "finishedPassingSequenceElements (temp exit)" << endl;
						exit(0);
						*/
					
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					}
					#endif
				}
				
			#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY	
			}
			#endif
		}
		else
		{
			if(!(forwardPropogationSequenceData->forwardPropogationActivationPointData->connectToPreviousActivationGroup))	//CHECKTHIS
			{
				//activation sequence completed (and not top level group), propagate next layer up
				if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationSequenceElementData, forwardPropogationSequenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))	//OLD: ownerGroupOrig
				{
					result = true;
				}
			}
		}
	}
	else
	{
		if(forwardPropogationSequenceData->forwardPropogationActivationPointData->generateActivationPointArray)
		{
			forwardPropogationSequenceData->forwardPropogationActivationPointData->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray->push_back(SANInodes.convertNeuralNetworkGroupToMemoryActivationGroup(ownerGroup));
			forwardPropogationSequenceData->forwardPropogationActivationPointData->activationPathSequenceElementFirstParseTreeGroupActivationPointArray->push_back(activationPathWordCurrentParseTreeGroupOwner);	
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
