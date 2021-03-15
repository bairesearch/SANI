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
 * File Name: SANIgenerateCompactIdentifyReferenceSets.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p3b 15-March-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Identify Reference Sets - identify and connect reference sets
 * /
 *******************************************************************************/


#include "SANIgenerateCompactIdentifyReferenceSets.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR
	
#ifdef SANI_SEQUENCE_GRAMMAR_LINK_SIMILAR_SUBNETS
bool SANIgenerateCompactIdentifyReferenceSetsClass::linkSimiliarSubnets(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* topLevelParseTreeGroup)
{
	bool result = true;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_SUCCESSFULLY_PARSED_NEURONS_BEFORE_REFERENCE_SET_IDENTIFICATION
	updateNetworkRecency(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal, topLevelParseTreeGroup, 0);
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS
	if(!identifyReferenceSetDelimiters(translatorVariables, forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup))
	{
		result = false;
	}	
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
	if(!findAndLinkReferenceSetCandidates1(forwardPropogationSentenceData, topLevelParseTreeGroup, 0))
	{
		result = false;
	}	
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_SUCCESSFULLY_PARSED_NEURONS_AFTER_REFERENCE_SET_IDENTIFICATION
	updateNetworkRecency(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal, topLevelParseTreeGroup, 0)
	#endif
	
	return result;	
}

#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS
bool SANIgenerateCompactIdentifyReferenceSetsClass::identifyReferenceSetDelimiters(SANItranslatorVariablesClass* translatorVariables, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup)
{
	bool result = true;
	
	SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_COMPONENT_CONNECTION_ACTIVE); //this is required to clear the neuron/component activations of past SANIgenerateCompactClass::generatePosRelTranslatorNeuralNetwork generateSentenceNetworkNodes/testSentenceNetworkNodes propagations

	bool determinerDetected = false;
	
	//determine referenceSetStartCodonType for each word:	
	for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
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
		
		int referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_UNKNOWN;
		string currentWordText = currentWord->tagName;
		
		currentWordText = SHAREDvars.convertStringToLowerCase(&currentWordText);
		//cout << "currentWordText = " << currentWordText << endl;

		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES	//DOING
		demarkatePosUnambiguousEntities(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, currentWord, i);
		#endif
		
		if(LRPpreprocessorWordIdentification.determineIsDeterminer(currentWord))
		{
			determinerDetected = true;
			
			//definite detection
			if(SHAREDvars.textInTextArray(currentWordText, grammaticalDeterminerDefiniteArray, GRAMMATICAL_DETERMINER_DEFINITE_NUMBER_OF_TYPES))
			{
				referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE;
			}
			
			//indefinite detection
			if(SHAREDvars.textInTextArray(currentWordText, grammaticalDeterminerIndefiniteArray, GRAMMATICAL_DETERMINER_INDEFINITE_NUMBER_OF_TYPES))
			{
				referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_INDEFINITE;
				//referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT;
			}
		}
		else
		{
			//definite detection
			if(LRPpreprocessorWordClassObject.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
			{
				//case: Propernouns: Tom (definite) rides a 
				referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE;
			}	
			
			//concept detection
			int grammaticalBaseTenseForm;
			LRPpreprocessorMultiwordReductionWord* nounBaseFound = NULL;
			if(LRPpreprocessorWordIdentification.determineNounPluralVariant(currentWordText, &nounBaseFound, &grammaticalBaseTenseForm))
			{
				if(!determinerDetected)
				{
					//case: possible concept: Dogs are/have
					referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT;
				}
			}		
		}
		
		currentWord->referenceSetStartCodonDeterminerType = referenceSetStartCodonType;
	}
	

	//perform reference set propagation of tuples (size t:0->n) after start codon:
	int sentenceLength = forwardPropogationSentenceData->sentenceContents->size();

	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_OF_REFERENCE_SET_DELIMITERS
	int lastIndexOfPreviousReferenceSet = INT_DEFAULT_VALUE;
	#endif
	
	//for(int w=0; w<forwardPropogationSentenceData->sentenceContents->size(); w++)
	for(int i=0; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
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
		
		string currentWordText = currentWord->tagName;
		#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
		cout << "currentWordText = " << currentWordText << endl;
		#endif
		
		if((currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE) || (currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_INDEFINITE) || (currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_PROPERNOUN) || (currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT))
		{
			#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
			cout << "SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE" << endl;
			#endif
			
			vector<multimap<double, SANIGroupNeuralNetwork*>> propagatedGroupsListPerformanceTupleArray(sentenceLength);	//dynamic array declaration (check Windows C++ compatibility; available on g++ compiler)
	
			//initiate SANI propagation of tuple following 
			int tupleSizeMin = 1;
			int tupleSize = tupleSizeMin;
			int tupleFirstIndex = -1;
			if(currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_PROPERNOUN)
			{
				tupleFirstIndex = i;
			}
			else if(currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT)
			{
				//TODO: need to determine tupleFirstIndex (might be at current index i, e.g. "dogs" or before current index i, e.g. "red dogs")
				cerr << "SANIgenerateCompactIdentifyReferenceSetsClass::identifyReferenceSetDelimiters: case (currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT) not yet coded" << endl;
				exit(EXIT_ERROR);
			}
			else
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_IGNORE_DETERMINER
				tupleFirstIndex = i+1;	//CHECKTHIS: +1: ignore determiner when searching for reference set? the sentence parse tree graph node will contain the determiner however, so there will likely always be a discrepancy between these		
				#else
				tupleFirstIndex = i;
				#endif
			}
			bool stillPropagatingPotentialRefSet = true;
			while(stillPropagatingPotentialRefSet)
			{
				int tupleLastIndex = tupleFirstIndex+tupleSize-1;
				LRPpreprocessorPlainTextWord* nextWordInTuple = forwardPropogationSentenceData->sentenceContents->at(tupleLastIndex);
				
				
				//if(currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_INDEFINITE)
				//{
				//	//stop propagation on detection of an indeterminer
				//	stillPropagatingPotentialRefSet = false;
				//}
				//if(currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE)
				//{
				//	//do not stop reference set propagation on detection of a determiner; eg the dog that rides the apple has a pie
				//}
				
				
				#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
				cout << "\n\nSANIpropagateCompactReferenceSets.performPropagationIndex: tupleFirstIndex = " << tupleFirstIndex << ", tupleLastIndex = " << tupleLastIndex << ", performPropagationIndex: " << forwardPropogationSentenceData->sentenceContents->at(tupleLastIndex)->tagName << endl;
				#endif
				
				SANIpropagateCompactReferenceSets.performPropagationIndex(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, &(propagatedGroupsListPerformanceTupleArray[tupleSize]), tupleFirstIndex, tupleLastIndex);
					//SANIforwardPropagateWithPartialSignals(nextWordInTuple);
					//storeMostLikelyCandidates(currentWord, mostLikelyCandidateRefSets);
				
				tupleSize++; 
				
				#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_IGNORE_EOS
				if(tupleLastIndex+1 == forwardPropogationSentenceData->sentenceContents->size()-1)
				#else
				if(tupleLastIndex+1 == forwardPropogationSentenceData->sentenceContents->size())
				#endif
				{
					stillPropagatingPotentialRefSet = false;	//reached end of sentence for tuple construction
				}
			}
			
			int tupleSizeMax = tupleSize;	//-1
			int referenceSetPhraseStartIndex = tupleFirstIndex;
			int mostLikelyCandidateReferenceSetPhraseLength = 0;
			double mostLikelyCandidateReferenceSetSimilarity = 0.0;
			
						
			//identify nodes in existing KB graph that are most activated (contain most active sub words) by the tuples
			SANIGroupNeuralNetwork* mostLikelyCandidateReferenceSetGroup = NULL;
			if(identifyMostLikelyReferenceSetCandidate(&propagatedGroupsListPerformanceTupleArray, tupleSizeMax, &mostLikelyCandidateReferenceSetGroup, &mostLikelyCandidateReferenceSetPhraseLength, &mostLikelyCandidateReferenceSetSimilarity, topLevelParseTreeGroup))
			{
				cout << "identifyMostLikelyReferenceSetCandidate" << endl;
				
				SANIGroupNeuralNetwork* currentSentenceReferenceSet = NULL;
				int minNumberWordContiguityErrors = REALLY_LARGE_INT; //large number
				if(findCurrentSentenceReferenceSet(forwardPropogationSentenceData, tupleFirstIndex, mostLikelyCandidateReferenceSetPhraseLength, topLevelParseTreeGroup, &currentSentenceReferenceSet, &minNumberWordContiguityErrors, 0))
				{
					//create direct link between mostLikelyCandidateReferenceSetGroup and relevant parse tree graph node
					cout << "createDirectAssociationConnection: currentSentenceReferenceSet = " << currentSentenceReferenceSet->groupIndex << endl;
					cout << "createDirectAssociationConnection: mostLikelyCandidateReferenceSetGroup = " << mostLikelyCandidateReferenceSetGroup->groupIndex << endl;
					createDirectAssociationConnection(currentSentenceReferenceSet, mostLikelyCandidateReferenceSetGroup, mostLikelyCandidateReferenceSetSimilarity);	//TODO: define this function and necessary datastructures
				}
				else
				{
					cout << "SANIgenerateCompactIdentifyReferenceSetsClass::identifyReferenceSetDelimiters: !findCurrentSentenceReferenceSet" << endl;
					exit(EXIT_ERROR);
				}	
						
				#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_OF_REFERENCE_SET_DELIMITERS
				int lastIndexOfReferenceSet = tupleFirstIndex+mostLikelyCandidateReferenceSetPhraseLength-1;
				int firstIndexOfReferenceSet = i;	//tupleFirstIndex-1
				demarkatePosReferenceSetDelimiters(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, lastIndexOfPreviousReferenceSet, firstIndexOfReferenceSet, lastIndexOfReferenceSet);
				lastIndexOfPreviousReferenceSet = lastIndexOfReferenceSet;
				#endif
			}
			else
			{
				
			}
		}
	}

	
	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES
bool SANIgenerateCompactIdentifyReferenceSetsClass::demarkatePosUnambiguousEntities(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, LRPpreprocessorPlainTextWord* currentWord, const int i)
{
	bool result = true;
	
	bool contextWordPOSisAmbiguous = false;
	bool identifiedEveryWordInDatabasePOSpermutationNOTUSED = true;
	uint64_t contextWordPOSambiguityInfo = LRP_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;	//default value
	uchar contextWordUnambiguousPOSindex = INT_DEFAULT_VALUE;	//LRP_PREPROCESSOR_POS_TYPE_UNDEFINED;
	if(!LRPpreprocessorPOStagger.determinePOSambiguityInfo(currentWord, &contextWordPOSambiguityInfo, &contextWordPOSisAmbiguous, &contextWordUnambiguousPOSindex, &identifiedEveryWordInDatabasePOSpermutationNOTUSED))
	{
		result = false;
	}

	int indexOfWord = i;
	int wordPhraseLength = 1;
	SANIGroupNeuralNetwork* referenceSetDelimiter = NULL;
	int minNumberWordContiguityErrors = REALLY_LARGE_INT; //large number
	bool foundRefSetNode = findCurrentSentenceReferenceSet(forwardPropogationSentenceData, indexOfWord, wordPhraseLength, topLevelParseTreeGroup, &referenceSetDelimiter, &minNumberWordContiguityErrors, 0);

	currentWord->POSambiguityInfo = contextWordPOSambiguityInfo;	
	if(!contextWordPOSisAmbiguous)
	{
		currentWord->wordPOStypeInferred = contextWordUnambiguousPOSindex;
		currentWord->unambiguousPOSindex = contextWordUnambiguousPOSindex;
		
		if(foundRefSetNode)
		{
			//cout << "foundRefSetNode" << endl;
			#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
			cout << "SANIgenerateCompactIdentifyReferenceSetsClass::demarkatePosUnambiguousEntities: currentWord = " << currentWord->tagName << ", currentWord->wordPOStypeInferred = " << currentWord->wordPOStypeInferred << endl;
			#endif
					
			/*
			referenceSetDelimiter->posValue = contextWordUnambiguousPOSindex;
			referenceSetDelimiter->posAmbiguousInfo = contextWordPOSambiguityInfo;
			referenceSetDelimiter->isPOSambiguous = contextWordPOSisAmbiguous;
			*/

			#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
			setANNneuronSANIposType(referenceSetDelimiter, contextWordUnambiguousPOSindex);
			#endif
		}	
	}
		
	return result;
}

#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
bool SANIgenerateCompactIdentifyReferenceSetsClass::setANNneuronSANIposType(SANIGroupNeuralNetwork* referenceSetDelimiter, const int contextWordUnambiguousPOSindex)
{
	bool result = true;
	
	/*
	if(contextWordUnambiguousPOSindex == )
	{

	}
	*/
	if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_NOUN)
	{
		referenceSetDelimiter->neuronReference->SANIposType = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_SUBSTANCE;
	}
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_VERB)
	{
		referenceSetDelimiter->neuronReference->SANIposType = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_ACTION;
		//referenceSetDelimiter->GIAisRelationship = true;
	}	
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_PREPOSITION)
	{
		referenceSetDelimiter->neuronReference->SANIposType = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_CONDITION;
		//referenceSetDelimiter->GIAisRelationship = true;
	}
	else
	{
		referenceSetDelimiter->neuronReference->SANIposType = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_UNKNOWN;
	}
	
	return result;
}
#endif
			
#endif
		
#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_OF_REFERENCE_SET_DELIMITERS
bool SANIgenerateCompactIdentifyReferenceSetsClass::demarkatePosReferenceSetDelimiters(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, const int lastIndexOfPreviousReferenceSet, const int firstIndexOfReferenceSet, const int lastIndexOfReferenceSet)
{
	bool result = true;

	#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
	bool edgeIsRefSetToDelimiter = true;
	int referenceSetDelimiterWordIndex1 = lastIndexOfReferenceSet;
	int referenceSetDelimiterWordIndex2 = lastIndexOfReferenceSet+1;
	if(!setANNneuronSANIrefsetConnectionType(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, edgeIsRefSetToDelimiter, referenceSetDelimiterWordIndex1, referenceSetDelimiterWordIndex2))
	{
		result = false;
	}	
	#endif

	if(lastIndexOfPreviousReferenceSet != INT_DEFAULT_VALUE)
	{
		int indexOfReferenceSetDelimiter = lastIndexOfPreviousReferenceSet+1;	//indexOfPreviousReferenceSetDelimiter
		int referenceSetDelimiterPhraseLength = firstIndexOfReferenceSet-lastIndexOfPreviousReferenceSet;
		
		bool referenceSetDelimiterWordFoundAmbiguous = false;
		bool referenceSetDelimiterWordFoundSingleAmbiguous = false;
		int referenceSetDelimiterWordFoundSingleAmbiguousIndex = INT_DEFAULT_VALUE;
		bool foundUnambiguousRelationshipEntity = false;
		for(int i=indexOfReferenceSetDelimiter; i<indexOfReferenceSetDelimiter+referenceSetDelimiterPhraseLength; i++)
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
			LRPpreprocessorPlainTextWord* referenceSetDelimiterWord = forwardPropogationSentenceData->sentenceContents->at(w);

			bool referenceSetDelimiterWordAmbiguous = false;
			if(referenceSetDelimiterWord->unambiguousPOSindex == LRP_SHARED_POS_TYPE_UNDEFINED)	//ie ambiguous
			{
				referenceSetDelimiterWordAmbiguous = true;
			}
			
			if(!referenceSetDelimiterWordAmbiguous)	//unambious
			{
				if(LRPpreprocessorWordIdentification.determineIsVerb(referenceSetDelimiterWord, LRP_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT))
				{
					foundUnambiguousRelationshipEntity = true;
				}
				if(LRPpreprocessorWordIdentification.determineIsPreposition(referenceSetDelimiterWord))
				{
					foundUnambiguousRelationshipEntity = true;
				}
			}
			if(referenceSetDelimiterWord->unambiguousPOSindex == LRP_SHARED_POS_TYPE_UNDEFINED)	//ie ambiguous
			{
				if(!referenceSetDelimiterWordFoundAmbiguous)
				{
					referenceSetDelimiterWordFoundAmbiguous = true;
					referenceSetDelimiterWordFoundSingleAmbiguous = true;
					referenceSetDelimiterWordFoundSingleAmbiguousIndex = w;
				}
				else
				{
					referenceSetDelimiterWordFoundSingleAmbiguous = false;
				}
			}
		}
		
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES
		if(!foundUnambiguousRelationshipEntity)
		{
			//set the ambigous relationship entity to unambious (as it has been inferred as the relationship entity within the referenceSetDelimiter text)
			if(referenceSetDelimiterWordFoundSingleAmbiguous)
			{
				LRPpreprocessorPlainTextWord* referenceSetDelimiterWordAmbiguous = forwardPropogationSentenceData->sentenceContents->at(referenceSetDelimiterWordFoundSingleAmbiguousIndex);
				if(LRPpreprocessorWordIdentification.determineIsVerb(referenceSetDelimiterWordAmbiguous, LRP_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT))
				{
					cout << "SANIgenerateCompactIdentifyReferenceSetsClass::demarkatePosReferenceSetDelimiters: referenceSetDelimiterWordAmbiguous = " << referenceSetDelimiterWordAmbiguous->tagName << ", LRP_PREPROCESSOR_POS_TYPE_VERB" << endl;

					referenceSetDelimiterWordAmbiguous->POSambiguityInfo = SHAREDvars.setBitValue(LRP_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, LRP_PREPROCESSOR_POS_TYPE_VERB, true);
					referenceSetDelimiterWordAmbiguous->wordPOStypeInferred = LRP_PREPROCESSOR_POS_TYPE_VERB;
					referenceSetDelimiterWordAmbiguous->unambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_VERB;
				}
				if(LRPpreprocessorWordIdentification.determineIsPreposition(referenceSetDelimiterWordAmbiguous))
				{
					cout << "SANIgenerateCompactIdentifyReferenceSetsClass::demarkatePosReferenceSetDelimiters: referenceSetDelimiterWordAmbiguous = " << referenceSetDelimiterWordAmbiguous->tagName << ", LRP_PREPROCESSOR_POS_TYPE_PREPOSITION" << endl;

					referenceSetDelimiterWordAmbiguous->POSambiguityInfo = SHAREDvars.setBitValue(LRP_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, LRP_PREPROCESSOR_POS_TYPE_PREPOSITION, true);
					referenceSetDelimiterWordAmbiguous->wordPOStypeInferred = LRP_PREPROCESSOR_POS_TYPE_PREPOSITION;
					referenceSetDelimiterWordAmbiguous->unambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_PREPOSITION;
				}
			}
		}
		#endif

		#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
		bool edgeIsRefSetToDelimiter = false;
		int referenceSetDelimiterWordIndex1 = firstIndexOfReferenceSet-1;
		int referenceSetDelimiterWordIndex2 = firstIndexOfReferenceSet;
		if(!setANNneuronSANIrefsetConnectionType(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, edgeIsRefSetToDelimiter, referenceSetDelimiterWordIndex1, referenceSetDelimiterWordIndex2))
		{
			result = false;
		}	
		#endif
				
		/*
		SANIGroupNeuralNetwork* referenceSetDelimiter = NULL;
		int minNumberWordContiguityErrors = REALLY_LARGE_INT; //large number
		if(findCurrentSentenceReferenceSet(forwardPropogationSentenceData, indexOfReferenceSetDelimiter, referenceSetDelimiterPhraseLength, topLevelParseTreeGroup, &referenceSetDelimiter, &minNumberWordContiguityErrors, 0))
		{		
			referenceSetDelimiter->GIAisRelationship = true;
			referenceSetDelimiter->posValue = wordPOSVal	//either verb or preposition
			referenceSetDelimiter->posAmbiguousInfo = posAmbiguousInfo
			referenceSetDelimiter->isPOSambiguous = false;
		}	
		else
		{
			cout << "SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_OF_REFERENCE_SET_DELIMITERS: SANIgenerateCompactIdentifyReferenceSetsClass::identifyReferenceSetDelimiters: !findCurrentSentenceReferenceSet" << endl;
			exit(EXIT_ERROR);
		}	
		*/
	}
}
#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
bool SANIgenerateCompactIdentifyReferenceSetsClass::setANNneuronSANIrefsetConnectionType(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, const bool edgeIsRefSetToDelimiter, const int referenceSetDelimiterWordIndex1, const int referenceSetDelimiterWordIndex2)
{
		SANIGroupNeuralNetwork* referenceSetDelimiter = NULL;
		SANIComponentNeuralNetwork* referenceSetDelimiterComponent1 = NULL;
		SANIComponentNeuralNetwork* referenceSetDelimiterComponent2 = NULL;
		int minNumberWordContiguityErrors = REALLY_LARGE_INT; //large number
		if(findReferenceSetDelimiter(forwardPropogationSentenceData, referenceSetDelimiterWordIndex1, referenceSetDelimiterWordIndex2, topLevelParseTreeGroup, &referenceSetDelimiter, &referenceSetDelimiterComponent1, &referenceSetDelimiterComponent2, 0))
		{		
			if(edgeIsRefSetToDelimiter)
			{
				referenceSetDelimiterComponent1->refsetConnectionType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_REFSET_DELIMITER_CONNECTIONS_TYPE_REFSET_NORMAL;
				referenceSetDelimiterComponent2->refsetConnectionType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_REFSET_DELIMITER_CONNECTIONS_TYPE_REFSET_DELIMITER;
			}
			else
			{
				referenceSetDelimiterComponent1->refsetConnectionType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_REFSET_DELIMITER_CONNECTIONS_TYPE_REFSET_DELIMITER;
				referenceSetDelimiterComponent2->refsetConnectionType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_REFSET_DELIMITER_CONNECTIONS_TYPE_REFSET_NORMAL;			
			}
		}	
		else
		{
			cout << "SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_OF_REFERENCE_SET_DELIMITERS:SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS: SANIgenerateCompactIdentifyReferenceSetsClass::setANNneuronSANIrefsetConnectionType: !findCurrentSentenceReferenceSet" << endl;
			exit(EXIT_ERROR);
		}		
}
#endif
#endif

				
					
bool SANIgenerateCompactIdentifyReferenceSetsClass::identifyMostLikelyReferenceSetCandidate(vector<multimap<double, SANIGroupNeuralNetwork*>>* propagatedGroupsListPerformanceTupleArray, int tupleSizeMax, SANIGroupNeuralNetwork** mostLikelyCandidateReferenceSetGroup, int* mostLikelyCandidateReferenceSetPhraseLength, double* mostLikelyCandidateReferenceSetSimilarity, SANIGroupParseTree* topLevelParseTreeGroup)
{
	bool result = false;
	
	vector<pair<double, SANIGroupNeuralNetwork*>> propagatedGroupsListPerformanceMaxOfEachTupleArray(tupleSizeMax);	//dynamic array declaration (check Windows C++ compatibility; available on g++ compiler)
	for(int t=1; t<tupleSizeMax; t++)
	{
		propagatedGroupsListPerformanceMaxOfEachTupleArray[t].first = 0.0;	//set performance to zero for every element in the array
	}
	
	for(int t=1; t<tupleSizeMax; t++)
	{
		multimap<double, SANIGroupNeuralNetwork*>* propagatedGroupsListPerformance = &((*propagatedGroupsListPerformanceTupleArray)[t]);
		
		bool stillSearchingForReferenceSetCandidate = true;
		
		for(multimap<double, SANIGroupNeuralNetwork*>::reverse_iterator propagatedGroupsListPerformanceIterator = propagatedGroupsListPerformance->rbegin(); propagatedGroupsListPerformanceIterator != propagatedGroupsListPerformance->rend(); propagatedGroupsListPerformanceIterator++) 
		{
			double maxPerformance = propagatedGroupsListPerformanceIterator->first;	//get first in list [max performance]
			SANIGroupNeuralNetwork* maxPerformanceGroup = propagatedGroupsListPerformanceIterator->second;	//get first in list [max performance]
			//cout << "maxPerformance = " << maxPerformance << endl;
			
			if(stillSearchingForReferenceSetCandidate)
			{
				if(maxPerformance >= SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_MINIMUM_ACTIVATION_SIGNAL)
				{
					//when calculating identifyMostLikelyReferenceSetCandidate, ignore (newly created) parse tree graph node representing the sentence reference set;
					if(!findNeuronInParseTree(topLevelParseTreeGroup, maxPerformanceGroup, 0)) //do not use newHiddenLayerGroupsTemp as it only contains newly generated nodes
					{
						//cout << "maxPerformance = " << maxPerformance << endl;
						
						#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
						cout << "!findNeuronInParseTree" << endl;
						#endif
						stillSearchingForReferenceSetCandidate = false;
						propagatedGroupsListPerformanceMaxOfEachTupleArray[t] = make_pair(maxPerformance, maxPerformanceGroup);
					}
				}
			}
		
			SANIpropagateCompactReferenceSets.clearSANIpropagateCompactReferenceSetsNetworkCache(maxPerformanceGroup);
		}		
	}
	
	double maxPerformance = 0.0;
	for(int t=1; t<tupleSizeMax; t++)
	{
		pair<double, SANIGroupNeuralNetwork*> propagatedGroupsListPerformanceMaxTuple = propagatedGroupsListPerformanceMaxOfEachTupleArray[t];
		if(propagatedGroupsListPerformanceMaxTuple.first > maxPerformance)
		{
			result = true;
			cout << "\tSANIgenerateCompactIdentifyReferenceSetsClass::identifyMostLikelyReferenceSetCandidate: maxPerformance = " << maxPerformance << endl;
			
			maxPerformance = propagatedGroupsListPerformanceMaxTuple.first;
			*mostLikelyCandidateReferenceSetGroup = propagatedGroupsListPerformanceMaxTuple.second;	//get first in list [max performance]
			*mostLikelyCandidateReferenceSetPhraseLength = t;
			*mostLikelyCandidateReferenceSetSimilarity = propagatedGroupsListPerformanceMaxTuple.first;
		}
	}
	
	return result;
}
		
bool SANIgenerateCompactIdentifyReferenceSetsClass::findNeuronInParseTree(SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork* neuronToFind, const int layer)
{	
	bool result = false;
	
	if(currentParseTreeGroup->groupRef == neuronToFind)
	{
		result = true;
	}
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(findNeuronInParseTree(parseTreeComponent->parseTreeGroupRef, neuronToFind, layer+1))
			{
				result = true;
			}
		}
	}

	return result;
}

bool SANIgenerateCompactIdentifyReferenceSetsClass::findCurrentSentenceReferenceSet(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int tupleFirstIndex, const int referenceSetPhraseLength, SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork** currentSentenceReferenceSet, int* minNumberWordContiguityErrors, const int layer)
{
	bool result = false;
	
	int numberWordContiguityErrors = calculateNumberWordContiguityErrors(forwardPropogationSentenceData, tupleFirstIndex, referenceSetPhraseLength, currentParseTreeGroup);
	if(numberWordContiguityErrors < *minNumberWordContiguityErrors)
	{
		result = true;
		*minNumberWordContiguityErrors = numberWordContiguityErrors;
		*currentSentenceReferenceSet = currentParseTreeGroup->groupRef;
	}
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(findCurrentSentenceReferenceSet(forwardPropogationSentenceData, tupleFirstIndex, referenceSetPhraseLength, parseTreeComponent->parseTreeGroupRef, currentSentenceReferenceSet, minNumberWordContiguityErrors, layer+1))
			{
				result = true;
			}
		}
	}
	
	return result;
}

int SANIgenerateCompactIdentifyReferenceSetsClass::calculateNumberWordContiguityErrors(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int tupleFirstIndex, int referenceSetPhraseLength, SANIGroupParseTree* currentParseTreeGroup)
{
	int numberWordContiguityErrors = 0;
	
	int tupleMinWordIndex;
	int tupleMaxWordIndex;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		tupleMaxWordIndex = forwardPropogationSentenceData->sentenceContents->size()-1-tupleFirstIndex;	//CHECKTHIS
		tupleMinWordIndex = tupleMaxWordIndex-referenceSetPhraseLength+1;		//CHECKTHIS
	}
	else
	{
		tupleMinWordIndex = tupleFirstIndex;
		tupleMaxWordIndex = tupleFirstIndex+referenceSetPhraseLength-1;
	}
	
	//cout << "tupleMinWordIndex = " << tupleMinWordIndex << endl;
	//cout << "tupleMaxWordIndex = " << tupleMaxWordIndex << endl;
	
	numberWordContiguityErrors = std::abs(tupleMinWordIndex - currentParseTreeGroup->parseTreeMinWordIndex) + std::abs(tupleMaxWordIndex - currentParseTreeGroup->parseTreeMaxWordIndex);

	return numberWordContiguityErrors;
}

bool SANIgenerateCompactIdentifyReferenceSetsClass::findReferenceSetDelimiter(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int referenceSetDelimiterWordIndex1, const int referenceSetDelimiterWordIndex2, SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork** currentSentenceReferenceSet, SANIComponentNeuralNetwork** referenceSetDelimiterComponent1, SANIComponentNeuralNetwork** referenceSetDelimiterComponent2, const int layer)
{
	bool result = false;

	int previousComponentWordIndex1 = INT_DEFAULT_VALUE;
	SANIComponentParseTree* parseTreeComponentPrevious = NULL;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		
		int parseTreeMinWordIndex = INT_DEFAULT_VALUE;
		int parseTreeMaxWordIndex = INT_DEFAULT_VALUE;
		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			SANIGroupParseTree* parseTreeGroupRef = parseTreeComponent->parseTreeGroupRef;
			parseTreeMinWordIndex = parseTreeGroupRef->parseTreeMinWordIndex;
			parseTreeMaxWordIndex = parseTreeGroupRef->parseTreeMaxWordIndex;
		}
		else
		{
			parseTreeMinWordIndex = parseTreeComponent->wordIndex;
			parseTreeMaxWordIndex = parseTreeComponent->wordIndex;
		}
		
		if(i > 0)
		{
			int wordIndex1 = previousComponentWordIndex1;
			int wordIndex2 = INT_DEFAULT_VALUE;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				wordIndex2 = parseTreeMaxWordIndex;
			}
			else
			{
				wordIndex2 = parseTreeMinWordIndex;
			}	
			if((referenceSetDelimiterWordIndex1 == wordIndex1) && (referenceSetDelimiterWordIndex2 == wordIndex2))
			{
				cout << "SANIgenerateCompactIdentifyReferenceSetsClass::findReferenceSetDelimiter" << endl;
				result = true;
				*currentSentenceReferenceSet = currentParseTreeGroup->groupRef;
				*referenceSetDelimiterComponent1 = parseTreeComponentPrevious->componentRef;
				*referenceSetDelimiterComponent2 = parseTreeComponent->componentRef;
			}
		}	
			
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			previousComponentWordIndex1 = parseTreeMinWordIndex;
		}
		else
		{
			previousComponentWordIndex1 = parseTreeMaxWordIndex;
		}
		parseTreeComponentPrevious = parseTreeComponent;
		
		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(findReferenceSetDelimiter(forwardPropogationSentenceData, referenceSetDelimiterWordIndex1, referenceSetDelimiterWordIndex2, parseTreeComponent->parseTreeGroupRef, currentSentenceReferenceSet, referenceSetDelimiterComponent1, referenceSetDelimiterComponent2, layer+1))
			{
				result = true;
			}
		}
	}
	
	return result;
}

void SANIgenerateCompactIdentifyReferenceSetsClass::createDirectAssociationConnection(SANIGroupNeuralNetwork* currentSentenceReferenceSet, SANIGroupNeuralNetwork* mostLikelyCandidateReferenceSetGroup, double mostLikelyCandidateReferenceSetSimilarity)
{
	//perform uniqueness test before appending association;
	bool directAssociationAlreadyCreated1 = false;
	bool directAssociationAlreadyCreated2 = false;
	double directNeuronAssociationExistingProbability1 = 0.0;
	double directNeuronAssociationExistingProbability2 = 0.0;
	for(multimap<double, SANIGroupNeuralNetwork*>::iterator directNeuronAssociationListIterator = currentSentenceReferenceSet->directNeuronAssociationList.begin(); directNeuronAssociationListIterator != currentSentenceReferenceSet->directNeuronAssociationList.end(); )
	//for(int i=0; i<currentSentenceReferenceSet->directNeuronAssociations.size(); i++)
	{
		SANIGroupNeuralNetwork* directNeuronAssociation = directNeuronAssociationListIterator->second;	//directNeuronAssociations[i];
		if(directNeuronAssociation == mostLikelyCandidateReferenceSetGroup)
		{
			directAssociationAlreadyCreated1 = true;
			directNeuronAssociationExistingProbability1 = directNeuronAssociationListIterator->first;
			currentSentenceReferenceSet->directNeuronAssociationList.erase(directNeuronAssociationListIterator);
		}
		else
		{
			directNeuronAssociationListIterator++;
		}
	}
	for(multimap<double, SANIGroupNeuralNetwork*>::iterator directNeuronAssociationListIterator = mostLikelyCandidateReferenceSetGroup->directNeuronAssociationList.begin(); directNeuronAssociationListIterator != mostLikelyCandidateReferenceSetGroup->directNeuronAssociationList.end(); )
	//for(int i=0; i<mostLikelyCandidateReferenceSetGroup->directNeuronAssociations.size(); i++)
	{
		SANIGroupNeuralNetwork* directNeuronAssociation = directNeuronAssociationListIterator->second;	//directNeuronAssociations[i];
		if(directNeuronAssociation == currentSentenceReferenceSet)
		{
			directAssociationAlreadyCreated2 = true;
			directNeuronAssociationExistingProbability2 = directNeuronAssociationListIterator->first;
			mostLikelyCandidateReferenceSetGroup->directNeuronAssociationList.erase(directNeuronAssociationListIterator);
		}
		else
		{
			directNeuronAssociationListIterator++;
		}
	}
	if((directAssociationAlreadyCreated1 && !directAssociationAlreadyCreated2) || (directAssociationAlreadyCreated2 && !directAssociationAlreadyCreated1))
	{
		cerr << "SANIgenerateCompactIdentifyReferenceSetsClass::createDirectAssociationConnection error: ((directAssociationAlreadyCreated1 && !directAssociationAlreadyCreated2) || (directAssociationAlreadyCreated2 && !directAssociationAlreadyCreated1))" << endl;
		exit(EXIT_ERROR);
	}
	if(directAssociationAlreadyCreated1)
	{
		mostLikelyCandidateReferenceSetSimilarity = mostLikelyCandidateReferenceSetSimilarity + ((directNeuronAssociationExistingProbability1+directNeuronAssociationExistingProbability2)/2);	//note directNeuronAssociationExistingProbability1 should equal directNeuronAssociationExistingProbability2 under current implementation
	}

	//create direct association:
	currentSentenceReferenceSet->directNeuronAssociationList.insert(make_pair(mostLikelyCandidateReferenceSetSimilarity, mostLikelyCandidateReferenceSetGroup));
	mostLikelyCandidateReferenceSetGroup->directNeuronAssociationList.insert(make_pair(mostLikelyCandidateReferenceSetSimilarity, currentSentenceReferenceSet));
}





#endif

#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_SUCCESSFULLY_PARSED_NEURONS
bool SANIgenerateCompactIdentifyReferenceSetsClass::updateNetworkRecency(int timeIndex, SANIGroupParseTree* currentParseTreeGroup, const int layer)
{
	//this function only updates recency of nodes in successfully parsed/generated parse tree
	
	bool result = true;
	
	currentParseTreeGroup->groupRef->timeIndex = timeIndex;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!updateNetworkRecency(timeIndex, parseTreeComponent->parseTreeGroupRef, layer+1))
			{
				result = false;
			}
		}
	}

	return result;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
bool SANIgenerateCompactIdentifyReferenceSetsClass::findAndLinkReferenceSetCandidates1(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup, const int layer)
{
	bool result = true;
	
	if(currentParseTreeGroup->groupRef->newlyGeneratedForSentenceTemp)	//CHECKTHIS: only perform reference set candidate detection for sentence newly generated neurons
	{
		SANIGroupNeuralNetwork* referenceSetCandidate2best = NULL;	//considered best based on some function of number of matches with candidateVector1 and recency
		
		SANIGroupNeuralNetwork* referenceSetCandidate1 = currentParseTreeGroup->groupRef;
		vector<SANIGroupNeuralNetwork*> referenceSetCandidateVector1;
		createReferenceSetCandidateVector(currentParseTreeGroup, &referenceSetCandidateVector1);
		
		findAndLinkReferenceSetCandidates2(forwardPropogationSentenceData, &referenceSetCandidate2best, currentParseTreeGroup, referenceSetCandidate1, &referenceSetCandidateVector1, layer);
		
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_LINK		        	
		if(referenceSetCandidate2best != NULL)
		{
			//link these nodes together
			referenceSetCandidate1->referenceSetCandidateDuplicates.push_back(referenceSetCandidate2best);
			referenceSetCandidate2best->referenceSetCandidateDuplicates.push_back(referenceSetCandidate1);
			
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
			cout << "\nfindAndLinkReferenceSetCandidates1, referenceSetCandidate1: " << endl;
			SANInodes.printParseTree(currentParseTreeGroup, 0);
			cout << "\nfindAndLinkReferenceSetCandidates1, referenceSetCandidate2best: " << endl;
			SANInodes.printNeuralNetwork(referenceSetCandidate2best, 0);
			#endif
		}
		#endif
		referenceSetCandidate1->referenceSetCandidateBestMetric = 0.0;
	}
		
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		//printComponent(parseTreeComponent, layer);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!findAndLinkReferenceSetCandidates1(forwardPropogationSentenceData, parseTreeComponent->parseTreeGroupRef, layer+1))
			{
				result = false;
			}
		}
	}

	return result;
}

bool SANIgenerateCompactIdentifyReferenceSetsClass::findAndLinkReferenceSetCandidates2(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork* referenceSetCandidate1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const int layer)
{
	bool result = true;
	
	bool hasComponentGroupRefs = false;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		//printComponent(parseTreeComponent, layer);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			hasComponentGroupRefs = true;
			
			if(!findAndLinkReferenceSetCandidates2(forwardPropogationSentenceData, referenceSetCandidate2best, parseTreeComponent->parseTreeGroupRef, referenceSetCandidate1, referenceSetCandidateVector1, layer+1))
			{
				result = false;
			}
		}
	}
	
	if(!hasComponentGroupRefs)
	{
		//every component points to an inputLayerNeuron
		SANIGroupNeuralNetwork* referenceSetCandidate2 = currentParseTreeGroup->groupRef;
		if(!findAndLinkReferenceSetCandidates3(forwardPropogationSentenceData, referenceSetCandidate2best, referenceSetCandidate2, referenceSetCandidate1, referenceSetCandidateVector1, layer))
		{
			result = false;
		}	
	}

	return result;
}



bool SANIgenerateCompactIdentifyReferenceSetsClass::findAndLinkReferenceSetCandidates3(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupNeuralNetwork* referenceSetCandidate2, SANIGroupNeuralNetwork* referenceSetCandidate1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const int layer)
{
	bool result = true;

	bool referenceSetCandidate2InReferenceSetCandidateVector1 = false;
	for(int v1=0; v1<referenceSetCandidateVector1->size(); v1++)
	{
		if(referenceSetCandidate2 == (*referenceSetCandidateVector1)[v1])
		{
			referenceSetCandidate2InReferenceSetCandidateVector1 = true;
		}
	}
	if(!referenceSetCandidate2InReferenceSetCandidateVector1)
	{
		//cout << "!referenceSetCandidate2InReferenceSetCandidateVector1" << endl;
		vector<SANIGroupNeuralNetwork*> referenceSetCandidateVector2;
		createReferenceSetCandidateVector(referenceSetCandidate2, &referenceSetCandidateVector2);
		findAndLinkReferenceSetCandidatesCompareVectors(referenceSetCandidate2best, referenceSetCandidate1, referenceSetCandidate2, referenceSetCandidateVector1, &referenceSetCandidateVector2);
	}
		
	for(int i=0; i<referenceSetCandidate2->SANIfrontComponentConnectionList.size(); i++)
	{
		SANIComponentNeuralNetwork* currentComponent = (referenceSetCandidate2->SANIfrontComponentConnectionList)[i];
		SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
		if(ownerGroup != referenceSetCandidate1)	//ensure that referenceSetCandidate2 != referenceSetCandidate1
		{			
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_VERIFY_NOT_NEWLY_CREATED
			if(!(ownerGroup->newlyGeneratedForSentenceTemp)) //only consider referenceSetCandidate2 if neuron has not been newly generated for current sentence	//ownerGroup->timeIndex != forwardPropogationSentenceData->sentenceIndex
			{
			#endif
				findAndLinkReferenceSetCandidates3(forwardPropogationSentenceData, referenceSetCandidate2best, ownerGroup, referenceSetCandidate1, referenceSetCandidateVector1, layer-1);
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_VERIFY_NOT_NEWLY_CREATED
			}
			#endif
		}
	}

	return result;
}

/*
bool SANIgenerateCompactIdentifyReferenceSetsClass::findAndLinkReferenceSetCandidatesWrapperInefficient(vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	if(currentParseTreeGroup->groupRef->newlyGeneratedForSentenceTemp)	//CHECKTHIS (only perform reference set candidate detection for sentence newly generated neurons)
	{
		vector<SANIGroupNeuralNetwork*> referenceSetCandidateVector1;
		createReferenceSetCandidateVector(currentParseTreeGroup, &referenceSetCandidateVector1);
		SANIGroupNeuralNetwork* currentNeuralNetworkGroup = currentParseTreeGroup->groupRef;

		for(int k=0; k<SANIGroupTypes->size(); k++)
		{
			SANIGroupType* groupType = SANIGroupTypes->at(k);
			for(int k2=0; k2<groupType->groups.size(); k2++)
			{
				SANIGroupNeuralNetwork* groupTypeNeuralNetworkGroup = (groupType->groups)[k2];

				vector<SANIGroupNeuralNetwork*> referenceSetCandidateVector2;
				createReferenceSetCandidateVector(groupTypeNeuralNetworkGroup, &referenceSetCandidateVector2);

				SANIGroupNeuralNetwork* referenceSetCandidate2best = NULL;
				findAndLinkReferenceSetCandidatesCompareVectors(&referenceSetCandidate2best, groupTypeNeuralNetworkGroup, currentNeuralNetworkGroup, referenceSetCandidateVector1, referenceSetCandidateVector2);
				#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_LINK
				if(referenceSetCandidate2best != NULL)
				{
					//link these nodes together
					referenceSetCandidate1->referenceSetCandidateDuplicates.push_back(referenceSetCandidate2best);
					referenceSetCandidate2best->referenceSetCandidateDuplicates.push_back(referenceSetCandidate1);
				}
				#endif
			}
		}
	}
		
		
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		//printComponent(parseTreeComponent, layer);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!findAndLinkReferenceSetCandidatesWrapper(SANIGroupTypes, forwardPropogationSentenceData, parseTreeComponent->parseTreeGroupRef, layer+1))
			{
				result = false;
			}
		}
	}

	return result;
}
*/

bool SANIgenerateCompactIdentifyReferenceSetsClass::findAndLinkReferenceSetCandidatesCompareVectors(constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupNeuralNetwork* referenceSetCandidate1, constEffective SANIGroupNeuralNetwork* referenceSetCandidate2, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector2)
{
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_RECENCY
	int totalMatchRecencyV2 = 0;
	int totalParseTreeRecencyV1 = 0;
	#endif
	int numberCandidatesV1 = referenceSetCandidateVector1->size();
	int numberCandidatesV2 = referenceSetCandidateVector2->size();
	int numberMatchesV2 = 0;
	for(int v1=0; v1<referenceSetCandidateVector1->size(); v1++)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_RECENCY
		totalParseTreeRecencyV1 = totalParseTreeRecencyV1 + (*referenceSetCandidateVector1)[v1]->timeIndex;
		#endif
		
		for(int v2=0; v2<referenceSetCandidateVector2->size(); v2++)
		{
			if((*referenceSetCandidateVector1)[v1] == (*referenceSetCandidateVector2)[v2])
			{
				//found a match between referenceSetCandidate1 (in original parse tree) and referenceSetCandidate2 (in entire SANI network)
				numberMatchesV2++;
				#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_RECENCY
				totalMatchRecencyV2 = totalMatchRecencyV2 + (*referenceSetCandidateVector2)[v2]->timeIndex;
				#endif
			}
		}
	}

	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
	//cout << "numberMatchesV2 = " << numberMatchesV2 << endl;
	#endif
	if(numberMatchesV2 >= SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_MIN_MATCHES)
	{
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_RECENCY
		double metric = double(totalMatchRecencyV2)/double(totalParseTreeRecencyV1);
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
		//cout << "(metric > referenceSetCandidate1->referenceSetCandidateBestMetric)" << endl;
		#endif
		if(metric > SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_RECENCY_MATCH_PERCENTAGE_REQUISITE)
		{	
		#else
		double metric = double(numberMatchesV2)/double(numberCandidatesV1);
		if(metric > SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_THRESHOLD_MATCH_PERCENT_REQUISITE)
		{
		#endif	
			if(metric > referenceSetCandidate1->referenceSetCandidateBestMetric)
			{
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
				//cout << "(metric > referenceSetCandidate1->referenceSetCandidateBestMetric)" << endl;
				#endif
				
				*referenceSetCandidate2best = referenceSetCandidate2;
				referenceSetCandidate1->referenceSetCandidateBestMetric = metric;
			}
		}

		/*
		if(numberMatchesV2 > int(double(numberCandidatesV1)*SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_THRESHOLD_PERCENT_TRUE_POSITIVES_V1))
		{
			if(numberMatchesV2 > int(double(numberCandidatesV2)*SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY_THRESHOLD_PERCENT_TRUE_POSITIVES_V2))
			{
				if(metric > referenceSetCandidate1->referenceSetCandidateBestMetric)
				{
					*referenceSetCandidate2best = referenceSetCandidate2;
					referenceSetCandidate1->referenceSetCandidateBestMetric = metric;
				}
			}
		}
		*/
	}
}
								
bool SANIgenerateCompactIdentifyReferenceSetsClass::createReferenceSetCandidateVector(SANIGroupParseTree* currentParseTreeGroup, vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector)
{
	bool result = true;
	
	referenceSetCandidateVector->push_back(currentParseTreeGroup->groupRef);
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{		
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!createReferenceSetCandidateVector(parseTreeComponent->parseTreeGroupRef, referenceSetCandidateVector))
			{
				result = false;
			}
		}
		else
		{
			if(SANInodes.hasComponentTypeString(parseTreeComponent))
			{
				SANIGroupNeuralNetwork* componentSource = parseTreeComponent->componentRef->SANIbackGroupConnectionList[0];
				referenceSetCandidateVector->push_back(componentSource);	//added SANI1m5f
				
				//cout << "(componentSource->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)" << endl;
				//cout << "componentSource->groupIndex = " << componentSource->groupIndex << endl;
			}
		}
	}

	return result;
}
bool SANIgenerateCompactIdentifyReferenceSetsClass::createReferenceSetCandidateVector(SANIGroupNeuralNetwork* currentNeuron, vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector)
{
	bool result = true;
	
	referenceSetCandidateVector->push_back(currentNeuron);
	
	for(int i=0; i<currentNeuron->components.size(); i++)
	{				
		SANIComponentNeuralNetwork* component = (currentNeuron->components)[i];
		for(int j=0; j<component->SANIbackGroupConnectionList.size(); j++)
		{
			SANIGroupNeuralNetwork* componentSource = (component->SANIbackGroupConnectionList)[j];
			//cout << "componentSource->inputLayerNeuron = " << componentSource->inputLayerNeuron << endl;
			
			if(!createReferenceSetCandidateVector(componentSource, referenceSetCandidateVector))
			{
				result = false;
			}
		}
	}

	return result;
}

#endif
#endif


	
#endif
