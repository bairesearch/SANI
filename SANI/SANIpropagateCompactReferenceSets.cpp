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
 * File Name: SANIpropagateCompactReferenceSets.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p9a 17-May-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Compact Reference Sets - ~O(n)
 * /
 *******************************************************************************/


#include "SANIpropagateCompactReferenceSets.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE

bool SANIpropagateCompactReferenceSetsClass::executePosRelTranslatorNeuralNetworkInitialise(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;

	executePosRelTranslatorNeuralNetworkStart(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData);
	
	return result;
}

bool SANIpropagateCompactReferenceSetsClass::executePosRelTranslatorNeuralNetworkDeinitialise(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
	
	executePosRelTranslatorNeuralNetworkEnd(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData);
	
	return result;
}

bool SANIpropagateCompactReferenceSetsClass::executePosRelTranslatorNeuralNetworkStart(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = false;
	
	//sentence specific variables:
	SANIpropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK_PROPAGATION
	forwardPropogationSentenceData->SANIGroupTypes = SANIGroupTypes;
	#endif
	
	for(int w=0; w<forwardPropogationSentenceData->sentenceContents->size(); w++)
	{	
		SANIForwardPropogationWordData* forwardPropogationWordData = new SANIForwardPropogationWordData();
		forwardPropogationSentenceData->forwardPropogationWordDataArray.push_back(forwardPropogationWordData);
	}
	
	return result;
}

bool SANIpropagateCompactReferenceSetsClass::executePosRelTranslatorNeuralNetworkEnd(const SANItranslatorVariablesClass* translatorVariables, const vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
	
	#ifdef SANI_FREE_MEMORY
	for(int w=0; w<forwardPropogationSentenceData->sentenceContents->size(); w++)
	{
		SANIForwardPropogationWordData* forwardPropogationWordData = forwardPropogationSentenceData->forwardPropogationWordDataArray[w];
		delete forwardPropogationWordData;
	}
	#endif
	
	return result;
}

bool SANIpropagateCompactReferenceSetsClass::performPropagationIndex(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, multimap<double, SANIGroupNeuralNetwork*>* propagatedGroupsListPerformance, const int firstWordPropagatedIndex, const int indexInSequence, const bool detectConcepts)
{
	int firstLayerNeuronIndex;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		firstLayerNeuronIndex = forwardPropogationSentenceData->sentenceContents->size()-1-indexInSequence;
	}
	else
	{
		firstLayerNeuronIndex = indexInSequence;
	}	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR	
	cout << "\n\n \e[33m SANIpropagateCompactReferenceSetsClass::performPropagationIndex: i = " << i << ", firstLayerNeuronIndex = " << firstLayerNeuronIndex << " - " << (*(forwardPropogationSentenceData->sentenceContents))[firstLayerNeuronIndex]->tagName << " \e[0m" << endl;
	#endif

	//cout << "SANIpropagateCompactReferenceSetsClass::performPropagationIndex: indexInSequence = " << indexInSequence << ", firstWordPropagatedIndex = " << firstWordPropagatedIndex << endl;
	
	//group specific variables:
	SANIForwardPropogationSignalData forwardPropogationSignalData;
	forwardPropogationSignalData.firstWordPropagatedIndex = firstWordPropagatedIndex;	//CHECKTHIS: note this has not been adjusted for forwardPropogationSentenceData->parseSentenceReverse, but it is not currently used so it should not be a problem
	forwardPropogationSignalData.maxNumberWordsCaptured = indexInSequence-firstWordPropagatedIndex+1;	//this is used by SANIpropagateCompactReferenceSets for determining ideal number of wordsCaptured by reference set candidate
	
	forwardPropogationSignalData.propagatedGroupsListPerformance = propagatedGroupsListPerformance;

	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_CONCEPTS
	forwardPropogationSignalData.detectConcepts = detectConcepts;
	#endif
	
	if(propagateWordThroughNetworkIntro(translatorVariables, firstLayerNeuronIndex, &forwardPropogationSignalData, forwardPropogationSentenceData))
	{

	}
}


bool SANIpropagateCompactReferenceSetsClass::propagateWordThroughNetworkIntro(SANItranslatorVariablesClass* translatorVariables, const int w, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
	
	LRPpreprocessorPlainTextWord* currentWord = (*(forwardPropogationSentenceData->sentenceContents))[w];

	SANIForwardPropogationWordData* forwardPropogationWordData = (forwardPropogationSentenceData->forwardPropogationWordDataArray[w]);
	forwardPropogationWordData->wordReference = currentWord;
	#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
	forwardPropogationWordData->wordPOStype = LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;	//set by propagateWordThroughNetworkIntro
	#endif
	forwardPropogationWordData->w = w;

	#ifdef SANI_DEBUG_PROPAGATE
	cout << "currentWord = " << currentWord->tagName << endl;
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	int wordPOStypeNOTUSED = INT_DEFAULT_VALUE;
	if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStypeNOTUSED, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
	{
		result = false;
	}	
	#else
	if(!SANInodes.currentWordPOSunknown(currentWord))
	{
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		int wordPOStype = currentWord->unambiguousPOSindex;
		#else
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		forwardPropogationSignalData->firstPOSval = false;
		#endif
		for(int wordPOStype=0; wordPOStype<LRP_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES; wordPOStype++)
		{
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, wordPOStype))
			{
		#endif	
				if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
				{
					result = false;
				}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		#else
				#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
				forwardPropogationSignalData->firstPOSval = false;
				#endif
			}
		}
		#endif
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
	else
	{
		int wordPOStype = INT_DEFAULT_VALUE;
		bool pass = LRPpreprocessorPOStagger.getWordPOStypeFromWordPOSunknown(currentWord, &wordPOStype);
		if(pass)
		{
			if(!propagateWordThroughNetworkGroupInit(translatorVariables, w, wordPOStype, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData))
			{
				result = false;
			}		
		}
	}
	#endif
	#endif
	
	return result;
}

bool SANIpropagateCompactReferenceSetsClass::propagateWordThroughNetworkGroupInit(SANItranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData)
{
	bool result = true;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	LRPpreprocessorPlainTextWord* currentWord = forwardPropogationWordData->wordReference;
	SANIGroupNeuralNetwork* inputLayerGroup;
	if(!SANIformation.findInputNeuronLayerSectionWordOrig(currentWord, &inputLayerGroup))
	{
		SANIformation.addInputNeuronLayerSectionWordOrig(currentWord, &inputLayerGroup);
	}
	#else
	SANIGroupNeuralNetwork* inputLayerGroup = SANIformation.getInputGroupLayerSection(SANIformation.getFirstGroupInInputLayerSectionWordPOStype(), wordPOStype);	
	#endif
	
	forwardPropogationWordData->wordPOStype = wordPOStype;
	/*
	//moved to SANIformation: @SANI1p6a
	#ifdef SANI_SEMANTIC_NETWORK
	inputLayerGroup->wordObject = forwardPropogationWordData->wordReference;
	#else
	inputLayerGroup->wordPOStype = wordPOStype;
	#endif
	*/

	#ifdef SANI_DEBUG_PROPAGATE
	cout << "SANIpropagateCompactReferenceSetsClass::propagateWordThroughNetworkIntro: GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS, wordPOStype = " << wordPOStype << endl;
	#endif

	#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION_INPUT_NEURONS
	inputLayerGroup->neuronActive = true;
	inputLayerGroup->neuronReference->activationLevel = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_FULL;
	#endif

	SANIGroupParseTree* activationPathWordFirstParseTreeGroup = NULL;	//not currently used by SANIpropagateCompactReferenceSetsClass
	int layer = 0;
	propagateWordThroughNetworkGroup(translatorVariables, inputLayerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordFirstParseTreeGroup);

	#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_ACTIVATION_INPUT_NEURONS
	inputLayerGroup->neuronActive = false;
	inputLayerGroup->neuronReference->activationLevel = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_INACTIVE;
	#endif

			
	return result;
}	

bool SANIpropagateCompactReferenceSetsClass::propagateWordThroughNetworkGroup(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_PROPAGATE
	SANInodes.printParseTreeDebugIndentation(layer);
	cout << "\e[32m 1: propagateWordThroughNetworkGroup: " << group->groupTypeName << ":" << "group->groupName << group->groupIndex = " << group->groupIndex << " \e[0m" << endl;
	#endif
			
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
	if(!group->neuronPropagated)	//prevent circular loops	//only required for recursion (not yet supported by SANI_SEQUENCE_GRAMMAR)
	{
		group->neuronPropagated = true;
	#endif
	
		if(propagateWordThroughNetworkGroupSelect(translatorVariables, group, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup))
		{
			result = true;
		}
		
	#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
		group->neuronPropagated = false;
	}
	#endif
	
	return result;
}
		
bool SANIpropagateCompactReferenceSetsClass::propagateWordThroughNetworkGroupSelect(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = false;

	//now search for existing sequence in network
	
	for(int i=0; i<group->SANIfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i];
		SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;

		int componentIndex = currentComponent->componentIndex;	

		#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
		if(!ownerGroup->neuronPropagated)	//prevent circular loops
		{
		#endif	
		
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_CONCEPTS
			if(!(forwardPropogationSignalData->detectConcepts) || ownerGroup->SANIisConceptNeuron)
			{
			#endif			

				#ifdef SANI_DEBUG_PROPAGATE_EXTRA2_REFSETS
				SANInodes.printParseTreeDebugIndentation(layer+1);	
				cout << "2a: propagateWordThroughNetworkGroup: ownerGroup->groupIndex = " <<  ownerGroup->groupIndex << ", currentComponent->componentIndex = " << currentComponent->componentIndex << endl;
				#endif

				if(group->inputLayerNeuron)	//if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					currentComponent->candidateStringMatch = forwardPropogationWordData->wordReference;
				}

				bool sequentialActivationFound = false;
				bool existingActivationFoundStartComponent = false;
				if(propagateWordThroughNetworkGroupComponentWrapper(translatorVariables, group, currentComponent, ownerGroup, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, layer, activationPathWordCurrentParseTreeGroup, i, &sequentialActivationFound, &existingActivationFoundStartComponent))
				{
					result = true;
				}

			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_CONCEPTS
			}
			#endif
			
		#ifdef SANI_PREVENT_CIRCULAR_CONNECTION_LOOPS
		}
		#endif
	}
	
	return result;
}	
	

bool SANIpropagateCompactReferenceSetsClass::propagateWordThroughNetworkGroupComponentWrapper(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent)
{
	bool result = false;
	
	int numberComponentsActivated = 0;
	int totalNumberComponents = ownerGroup->components.size();
	double componentActivationSignal = 0.0;
	
	bool firstComponentInGroup = false;
	bool successorComponentAlreadyActivated = false;
	if(propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, currentComponent, &(ownerGroup->components), ownerGroup, &successorComponentAlreadyActivated, &firstComponentInGroup))
	{
		*sequentialActivationFound = true;
	}
	
	//determine if group reset required;
	if(!sequentialActivationFound)
	{
		//still allow propagation
		if(successorComponentAlreadyActivated && firstComponentInGroup)
		{
			//irrespective: if(currentComponent->activated)
			
			//reactivate entire group;
			clearSANIpropagateCompactReferenceSetsNetworkCache(ownerGroup);
			*sequentialActivationFound = true;
		}		
		else
		{
			//do not reset activation of group
			//do not reactivate group, allow components to be activated out of order (though activation signal will be lowered)
		}	
	}

	//update component activation details	
	currentComponent->neuronComponentConnectionActive = true;
	currentComponent->neuronComponentConnectionActiveWordRecord = forwardPropogationWordData->wordReference;
		
	calculateNeuronActivationSignal(forwardPropogationSignalData, forwardPropogationSentenceData, ownerGroup, currentComponent, &componentActivationSignal, &numberComponentsActivated);

	double fractionOfComponentsActivated = 0.0;
	bool componentActivationOrderSequential = false;
	double fractionOfComponentWordsCaptured = 0.0;
	bool componentWordsOrderSequential = false;
	double neuronPopularity = 0.0;
	double neuronRecencyAge = 0.0;
	
	
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_ACTIVATED_USE
	fractionOfComponentsActivated = numberComponentsActivated/totalNumberComponents;
	//cout << "fractionOfComponentsActivated = " << fractionOfComponentsActivated << endl;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_ACTIVATION_SEQUENTIALITY_USE
	componentActivationOrderSequential = *sequentialActivationFound;
	//cout << "componentActivationOrderSequential = " << componentActivationOrderSequential << endl;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_WORDS_CAPTURED_USE
	fractionOfComponentWordsCaptured = ownerGroup->wordsCaptured.size()/double(forwardPropogationSignalData->maxNumberWordsCaptured);
	//cout << "ownerGroup->wordsCaptured.size() = " << ownerGroup->wordsCaptured.size() << endl;
	//cout << "forwardPropogationSignalData->maxNumberWordsCaptured = " << forwardPropogationSignalData->maxNumberWordsCaptured << endl;
	//cout << "fractionOfComponentWordsCaptured = " << fractionOfComponentWordsCaptured << endl;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_WORDS_SEQUENTIALITY_USE
	componentWordsOrderSequential = verifyWordsCapturedSequentiality(forwardPropogationSentenceData, forwardPropogationSignalData, ownerGroup);
	//cout << "componentWordsOrderSequential = " << componentWordsOrderSequential << endl;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_NEURON_POPULARITY_USE
	neuronPopularity = ownerGroup->groupStrength;
	//cout << "neuronPopularity = " << neuronPopularity << endl;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_RECENCY_USE
	neuronRecencyAge = translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal - ownerGroup->timeIndex;
	//cout << "neuronRecencyAge = " << neuronRecencyAge << endl;
	#endif
	
	double activationSignalStrength = calculateActivationSignalStrength(componentActivationSignal, fractionOfComponentsActivated, componentActivationOrderSequential, fractionOfComponentWordsCaptured, componentWordsOrderSequential, neuronPopularity, neuronRecencyAge);
	ownerGroup->activationSignalStrength = activationSignalStrength;
	
	SANIForwardPropogationSignalData* semanticRelationReturnEntityForwardPropogationSignalDataNew = forwardPropogationSignalData;	//do not need to create a copy of the signal data, but could do this if necessary
	semanticRelationReturnEntityForwardPropogationSignalDataNew->activationSignalStrength = activationSignalStrength;
	semanticRelationReturnEntityForwardPropogationSignalDataNew->wordsCaptured = ownerGroup->wordsCaptured;
	//int64_t activationSignalStrengthLong = convertActivationSignalStrengthToLong(activationSignalStrength);
	//TODO: test if ownerGroup is already added to propagatedGroupsListPerformance (so do not replicate values in map)
	semanticRelationReturnEntityForwardPropogationSignalDataNew->propagatedGroupsListPerformance->insert(make_pair(activationSignalStrength, ownerGroup));	//semanticRelationReturnEntityForwardPropogationSignalDataNew->propagatedGroupsListPerformance->insert(make_pair<int64_t, SANIGroupNeuralNetwork*>(activationSignalStrengthLong, ownerGroup));
	
	SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner = NULL;	//not currently used by SANIpropagateCompactReferenceSetsClass
	
	//CHECKTHIS: propagate next layer up, regardless of whether activation sequence completed
	if(propagateWordThroughNetworkGroup(translatorVariables, ownerGroup, semanticRelationReturnEntityForwardPropogationSignalDataNew, forwardPropogationWordData, forwardPropogationSentenceData, (layer+1), activationPathWordCurrentParseTreeGroupOwner))
	{
		//result = true;
	}
				
	return result;
}

bool SANIpropagateCompactReferenceSetsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIGroupNeuralNetwork* ownerGroup, bool* successorComponentAlreadyActivated, bool* firstComponentInGroup)
{	
	
	bool sequentialActivationFound = false;
	bool stillParsingActiveComponents = true;
	
	vector<LRPpreprocessorPlainTextWord*> wordsCaptured;
		
	for(int i=0; i<components->size(); i++)
	{		
		int c;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			c = components->size()-1-i;
		}
		else
		{
			c = i;
		}

		SANIComponentNeuralNetwork* currentComponent = (*components)[c];

		if(stillParsingActiveComponents)
		{
			if(currentComponent == testComponent)
			{
				if(i == 0)
				{
					*firstComponentInGroup = true;
				}
				
				if(currentComponent->neuronComponentConnectionActive)
				{
					sequentialActivationFound = true;	//allow overwrite activation
				}
				else
				{
					sequentialActivationFound = true;
				}
			}
		}
		else
		{
			if(sequentialActivationFound)
			{
				if(currentComponent->neuronComponentConnectionActive)
				{
					sequentialActivationFound = false;	//a component is already activated after testComponent
					*successorComponentAlreadyActivated = true;
				}
			}
		}
		
		if(!(currentComponent->neuronComponentConnectionActive))
		{
			stillParsingActiveComponents = false;
		}
	}
	
	ownerGroup->wordsCaptured = wordsCaptured;
	
	return sequentialActivationFound;
}





void SANIpropagateCompactReferenceSetsClass::calculateNeuronActivationSignal(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* testComponent, double* componentActivationSignal, int* numberComponentsActivated)
{
	for(int i=0; i<ownerGroup->components.size(); i++)
	{		
		int c;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			c = ownerGroup->components.size()-1-i;
		}
		else
		{
			c = i;
		}

		SANIComponentNeuralNetwork* currentComponent = (ownerGroup->components)[c];

		if(currentComponent->neuronComponentConnectionActive)
		{
			*numberComponentsActivated = *numberComponentsActivated + 1;
			if(currentComponent != testComponent)
			{				
				if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					*componentActivationSignal = *componentActivationSignal + SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_INPUT_SIGNAL;
					appendToWordsCaptured(&(ownerGroup->wordsCaptured), currentComponent->candidateStringMatch);
				}
				else
				{
					//cout << "currentComponent->activationSignalStrength = " << currentComponent->activationSignalStrength << endl;
					*componentActivationSignal = *componentActivationSignal + currentComponent->activationSignalStrength;

					appendToWordsCaptured(&(ownerGroup->wordsCaptured), currentComponent);
				}
			}
		}
		if(currentComponent == testComponent)
		{
			*numberComponentsActivated = *numberComponentsActivated + 1;

			if(currentComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
			{
				*componentActivationSignal = *componentActivationSignal + SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_INPUT_SIGNAL;
				appendToWordsCaptured(&(ownerGroup->wordsCaptured), currentComponent->candidateStringMatch);
			}
			else
			{
				currentComponent->activationSignalStrength = forwardPropogationSignalData->activationSignalStrength;
				*componentActivationSignal = *componentActivationSignal + currentComponent->activationSignalStrength;
			
				currentComponent->wordsCaptured = forwardPropogationSignalData->wordsCaptured;
				appendToWordsCaptured(&(ownerGroup->wordsCaptured), currentComponent);
			}
		}
	}
}

void SANIpropagateCompactReferenceSetsClass::appendToWordsCaptured(vector<LRPpreprocessorPlainTextWord*>* wordsCaptured, const SANIComponentNeuralNetwork* currentComponent)
{	
	//wordsCaptured.insert(wordsCaptured.end(), currentComponent->wordsCaptured.begin(), currentComponent->wordsCaptured.end());	//append

	//perform uniqueness test before appending words;
	for(int i=0; i<currentComponent->wordsCaptured.size(); i++)
	{
		LRPpreprocessorPlainTextWord* wordCaptured = currentComponent->wordsCaptured[i];
		
		appendToWordsCaptured(wordsCaptured, wordCaptured);
	}
}

void SANIpropagateCompactReferenceSetsClass::appendToWordsCaptured(vector<LRPpreprocessorPlainTextWord*>* wordsCaptured, LRPpreprocessorPlainTextWord* wordCaptured)
{			
	if(find(wordsCaptured->begin(), wordsCaptured->end(), wordCaptured) == wordsCaptured->end())	//element not in vector
	{
		wordsCaptured->push_back(wordCaptured);
		#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
		cout << "SANIpropagateCompactReferenceSetsClass::appendToWordsCaptured - adding to wordsCaptured: " << wordCaptured->tagName << endl;
		#endif
	}
	else
	{
		#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
		cout << "SANIpropagateCompactReferenceSetsClass::appendToWordsCaptured - wordCaptured already added" << endl;
		#endif
	}
}



double SANIpropagateCompactReferenceSetsClass::calculateActivationSignalStrength(double componentActivationSignal, double fractionOfComponentsActivated, bool componentActivationOrderSequential, double fractionOfComponentWordsCaptured, bool componentWordsOrderSequential, double neuronPopularity, double neuronRecencyAge)
{
	#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
	cout << "\nSANIpropagateCompactReferenceSetsClass::calculateActivationSignalStrength" << endl;
	cout << "componentActivationSignal = " << componentActivationSignal << endl;
	#endif
	
	double fractionOfComponentsActivatedModifier = 1.0;
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_ACTIVATED_USE
	fractionOfComponentsActivatedModifier = fractionOfComponentsActivated*SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_ACTIVATED;
	#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
	cout << "fractionOfComponentsActivatedModifier = " << fractionOfComponentsActivatedModifier << endl;
	#endif
	#endif
	
	double componentActivationOrderSequentialModifier = 1.0;
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_ACTIVATION_SEQUENTIALITY_USE
	if(!componentActivationOrderSequential)
	{
		componentActivationOrderSequentialModifier = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_ACTIVATION_SEQUENTIALITY;
	}
	#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
	cout << "componentActivationOrderSequentialModifier = " << componentActivationOrderSequentialModifier << endl;
	#endif
	#endif
	
	double fractionOfComponentWordsCapturedModifier = 1.0;
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_WORDS_CAPTURED_USE
	fractionOfComponentWordsCapturedModifier = fractionOfComponentWordsCaptured*SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_WORDS_CAPTURED;
	#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
	cout << "fractionOfComponentWordsCapturedModifier = " << fractionOfComponentWordsCapturedModifier << endl;
	#endif
	#endif
	
	double componentWordsOrderSequentialModifier = 1.0;
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_WORDS_SEQUENTIALITY_USE
	if(!componentWordsOrderSequential)
	{
		componentWordsOrderSequentialModifier = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_WORDS_SEQUENTIALITY;
	}
	#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
	cout << "componentWordsOrderSequentialModifier = " << componentWordsOrderSequentialModifier << endl;
	#endif
	#endif
	
	double neuronPopularityModifier = 1.0;
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_NEURON_POPULARITY_USE
	neuronPopularityModifier = neuronPopularity*SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_NEURON_POPULARITY;
	#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
	cout << "neuronPopularityModifier = " << neuronPopularityModifier << endl;
	#endif
	#endif
	
	double neuronRecencyAgeModifier = 1.0;
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_RECENCY_USE
	double neuronRecencyAgeModifier = 1.0/(neuronRecencyAge*SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_COMPONENTS_RECENCY);
	#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
	cout << "neuronRecencyAgeModifier = " << neuronRecencyAgeModifier << endl;
	#endif
	#endif
		
	double activationSignalStrengthModifier = fractionOfComponentsActivatedModifier*componentActivationOrderSequentialModifier*fractionOfComponentWordsCapturedModifier*componentWordsOrderSequentialModifier*neuronPopularityModifier*neuronRecencyAgeModifier;
	double activationSignalStrength = componentActivationSignal * activationSignalStrengthModifier;
	
	#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
	cout << "activationSignalStrength = " << activationSignalStrength << endl;
	#endif
	
	return activationSignalStrength;
}

bool SANIpropagateCompactReferenceSetsClass::verifyWordsCapturedSequentiality(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIGroupNeuralNetwork* ownerGroup)
{
	bool result = true;
	
	int currentWordIndexInSentence = 0;
	for(int i=0; i<ownerGroup->wordsCaptured.size(); i++)
	{
		LRPpreprocessorPlainTextWord* currentWordCaptured = ownerGroup->wordsCaptured[i];

		bool foundWordCapturedInSentenceSequentially = false;
		for(int i=currentWordIndexInSentence; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
		{
			int w;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				w = forwardPropogationSentenceData->sentenceContents->size()-1-i;
			}
			else
			{
				w = i;
			}
			LRPpreprocessorPlainTextWord* currentWord = forwardPropogationSentenceData->sentenceContents->at(w);
			if(!foundWordCapturedInSentenceSequentially)
			{
				if(currentWord == currentWordCaptured)
				{
					foundWordCapturedInSentenceSequentially = true;
					currentWordIndexInSentence = i+1;
				}
			}
		}

		if(!foundWordCapturedInSentenceSequentially)
		{
			result = false;
		}
	}
	
	return result;
}


/*
int64_t SANIpropagateCompactReferenceSetsClass::convertActivationSignalStrengthToLong(double activationSignalStrength)
{
	int64_t activationSignalStrengthLong = activationSignalStrength * (std::pow(10, SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL_STRENGTH_MAX_SIGNIFICANT_FIGURES));
	return activationSignalStrengthLong;
}
*/

bool SANIpropagateCompactReferenceSetsClass::clearSANIpropagateCompactReferenceSetsNetworkCache(SANIGroupNeuralNetwork* neuron)
{
	SANIpropagateOperations.resetGroupActivation(neuron);

	neuron->activationSignalStrength = 0.0;
	neuron->wordsCaptured.clear();
	
	for(int i=0; i<neuron->components.size(); i++)
	{	
		SANIComponentNeuralNetwork* currentComponent = (neuron->components)[i];
		currentComponent->activationSignalStrength = 0.0;
		currentComponent->wordsCaptured.clear();
	}
}


#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
bool SANIpropagateCompactReferenceSetsClass::printSANInetworkSVG(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	string outputTextSVGFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + "IncrementalGeneration" + SHAREDvars.convertIntToString(incrementalGenerationIndex) + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
	cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK: outputTextSVGFileName = " << outputTextSVGFileName << endl;
	
	#ifdef SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	SANIformation.createANNconnectivity(SANIGroupTypes);
	#endif
	
	GIAneuralNetworkOperationsClass().generateNeuralNetFromSANInet(translatorVariables);	//generate GIA NLP neural network

	int rasterImageWidth = 0;
	int rasterImageHeight = 0;
	bool ANNdisplayInOpenGL = false;
	bool ANNuseInputXMLFile = false;
	string ANNinputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputXMLFile = false;
	string ANNoutputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputLDRFile = false;
	string ANNoutputLDRFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
	bool ANNuseOutputSVGFile = true;
	string ANNoutputSVGFileName = outputTextSVGFileName;
	bool ANNuseOutputPPMFile = false;
	string ANNoutputPPMFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
	bool ANNuseOutputPPMFileRaytraced = false;
	string ANNoutputPPMFileNameRaytraced = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
	string ANNoutputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;
	bool ANNuseSprites = true;
	
	//small:
	int widthSVG = 3840;	//1920
	int heightSVG = 2160;	//1080	//1400
	
	//large/medium:
	//int widthSVG = 14400;
	//int heightSVG = 2430;
	
	ANNdisplay.outputNeuralNetworkToVectorGraphicsAndRaytrace(translatorVariables->firstInputNeuronInNetwork, ANNuseSprites, ANNuseOutputPPMFileRaytraced, ANNdisplayInOpenGL, ANNuseOutputLDRFile, ANNuseOutputSVGFile, ANNuseOutputPPMFile, ANNoutputLDRFileName, ANNoutputSVGFileName, ANNoutputPPMFileName, ANNoutputPPMFileNameRaytraced, ANNoutputTALFileName, rasterImageWidth, rasterImageHeight, widthSVG, heightSVG);

	#ifdef SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
	SANIformation.createANNconnectivityReset(SANIGroupTypes);
	#endif
	
	incrementalGenerationIndex++;
	
	return result;
}
#endif



#endif
