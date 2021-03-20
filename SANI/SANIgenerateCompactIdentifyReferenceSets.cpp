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
 * Project Version: 1p6a 20-March-2021
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

	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
	bool determinerDetectedPrior = false;
	bool auxiliaryBeingPresentSingularCasePrevious = false;	//"is"
	bool indefiniteDeterminerDetectedPrior = false;
	LRPpreprocessorPlainTextWord* priorDeterminer = NULL;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_PREDETERMINERS
	bool predeterminerDetectedPrevious = false;
	#endif
	
	
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

		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES
		demarkatePosUnambiguousEntities(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, currentWord, i);
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_PRONOUNS
		bool pronounReferenceDetected = false;
		bool pronounDeterminerDetected = false;
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_PREDETERMINERS
		bool predeterminerDetected = false;
		if(LRPpreprocessorWordIdentification.determineIsPredeterminer(currentWord))
		{
			predeterminerDetected = true;
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
			determinerDetectedPrior = true;
			#endif
			referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE;
		}
		#endif
		if(LRPpreprocessorWordIdentification.determineIsDeterminer(currentWord))
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_PREDETERMINERS
			if(!predeterminerDetectedPrevious)
			{
			#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
				determinerDetectedPrior = true;
				#endif

				//definite detection
				if(LRPpreprocessorWordIdentification.determineIsDeterminerDefinite(currentWordText))
				{
					referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE;
				}

				//indefinite detection
				if(LRPpreprocessorWordIdentification.determineIsDeterminerIndefinite(currentWordText))
				{
					#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
					if(auxiliaryBeingPresentSingularCasePrevious)
					{
						referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT_OBJECT;	//case: possible concept: a/the alsation is a dog
						if(indefiniteDeterminerDetectedPrior)
						{
							priorDeterminer->referenceSetStartCodonDeterminerType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT_SUBJECT;	//case: possible concept: an [indefinite] alsation [SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT_SUBJECT] is a dog [SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT_OBJECT]
						}
					}
					else
					{
					#endif
						referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_INDEFINITE;
					#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
					}
					#endif
				}
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_PREDETERMINERS
			}
			#endif
		}
		else
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_PRONOUNS
			//definite detection
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_DEMONSTRATIVE))
			{
				//case: Demonstrative Pronoun: this (definite) robot rides a 
				referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE;
				pronounDeterminerDetected = true;
			}
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_INDEFINITE))
			{
				//case: Indefinite Pronoun: somebody (reference) rides a 
				pronounReferenceDetected = true;
			}	
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_INTERROGATIVE))
			{
				//case: Interrogative Pronoun: whomever (reference) rides a 
				pronounReferenceDetected = true;
			}
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_PERSONAL_OBJECT))
			{
				//case: Object Pronoun: Tom rides it (reference)
				pronounReferenceDetected = true;
			}
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_PERSONAL_SUBJECT))
			{
				//case: Subject Pronoun: He (definite) rides a 
				pronounReferenceDetected = true;
			}
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_POSSESSIVE_ADJECTIVE))
			{
				//case: Possessive Adjective Pronoun: Our (definite) robot rides a 
				referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE;
				pronounDeterminerDetected = true;
			}
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_POSSESSIVE_ALONE))
			{
				//case: Possessive Alone Pronoun: Tom is ours (reference)
				pronounReferenceDetected = true;
			}
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_REFLEXIVE))
			{
				//case: Reflexive Pronoun: Tom rides himself (reference)
				pronounReferenceDetected = true;
			}
			if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(currentWord->POSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_RELATIVE))
			{
				//case: Relative Pronoun: Tom is a man who (determiner) rides a 
				referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE;
				pronounDeterminerDetected = true;
			}
			#endif
			
			/*
			//propernouns are not determiners (reference sets do not follow propernouns)
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_PROPERNOUNS
			//definite detection
			if(LRPpreprocessorWordIdentification.determineIsLikelyPropernoun(currentWord))
			{
				//case: Propernouns: Tom (definite) rides a 
				referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE;
			}	
			#endif
			*/
			
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
			//concept detection
			int grammaticalBaseTenseForm;
			LRPpreprocessorMultiwordReductionWord* nounBaseFound = NULL;
			if(LRPpreprocessorWordIdentification.determineNounPluralVariant(currentWordText, &nounBaseFound, &grammaticalBaseTenseForm))
			{
				if(!determinerDetectedPrior)
				{
					//case: possible concept: Dogs are/have Apples
					referenceSetStartCodonType = SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT_SUBJECT_OR_OBJECT;
				}
			}	
			#endif
		}
		
		currentWord->referenceSetStartCodonDeterminerType = referenceSetStartCodonType;
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_PRONOUNS
		currentWord->pronounDeterminerDetected = pronounDeterminerDetected;
		currentWord->pronounReferenceDetected = pronounReferenceDetected;
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
		if(currentWord->tagName == GRAMMATICAL_AUXILIARY_BEING_PRESENT_SINGULAR)
		{
			auxiliaryBeingPresentSingularCasePrevious = true;
		}
		else
		{
			auxiliaryBeingPresentSingularCasePrevious = false;
		}
		if(referenceSetStartCodonType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_INDEFINITE)
		{
			indefiniteDeterminerDetectedPrior = true;
			priorDeterminer = currentWord;
		}
		else
		{
			indefiniteDeterminerDetectedPrior = false;
			priorDeterminer = NULL;
		}				
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_PREDETERMINERS
		if(predeterminerDetected)
		{
			predeterminerDetectedPrevious = true;
		}
		else
		{
			predeterminerDetectedPrevious = false;
		}
		#endif
	}
	

	//perform reference set propagation of tuples (size t:0->n) after start codon:
	int sentenceLength = forwardPropogationSentenceData->sentenceContents->size();

	int lastIndexOfPreviousReferenceSet = INT_DEFAULT_VALUE;
	int firstIndexOfPreviousReferenceSet = 0;
	
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
		
		if(isReferenceSetStartCodonDeterminerTypeAll(currentWord->referenceSetStartCodonDeterminerType))
		{
			#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
			cout << "SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE" << endl;
			#endif
			
			vector<multimap<double, SANIGroupNeuralNetwork*>> propagatedGroupsListPerformanceTupleArray(sentenceLength);	//dynamic array declaration (check Windows C++ compatibility; available on g++ compiler)
	
			//initiate SANI propagation of tuple following 
			int tupleSizeMin = 1;
			int tupleSize = tupleSizeMin;
			int tupleFirstIndex = -1;
			/*
			if(currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_PROPERNOUN)
			{
				tupleFirstIndex = i;
			}
			*/
			if(currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT_SUBJECT_OR_OBJECT)
			{
				tupleFirstIndex = i;
				//TODO: need to determine tupleFirstIndex (might be at current index i, e.g. "dogs" or before current index i, e.g. "red dogs")
				cerr << "SANIgenerateCompactIdentifyReferenceSetsClass::identifyReferenceSetDelimiters: case (currentWord->referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT_SUBJECT_OR_OBJECT) requires detecting prior adjectives." << endl;
				cerr << "Need to determine tupleFirstIndex (might be at current index i, e.g. \"dogs\" or before current index i, e.g. \"red dogs\")" << endl;
				//exit(EXIT_ERROR);
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
				
				bool detectConcepts = false;
				#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
				if(isReferenceSetStartCodonDeterminerTypeConcept(currentWord->referenceSetStartCodonDeterminerType))
				{
					detectConcepts = true;
				}
				#endif
		
				SANIpropagateCompactReferenceSets.performPropagationIndex(translatorVariables, SANIGroupTypes, forwardPropogationSentenceData, &(propagatedGroupsListPerformanceTupleArray[tupleSize]), tupleFirstIndex, tupleLastIndex, detectConcepts);
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
						
				int lastIndexOfReferenceSet = tupleFirstIndex+mostLikelyCandidateReferenceSetPhraseLength-1;
				int firstIndexOfReferenceSet = i;	//tupleFirstIndex-1
				#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_OF_REFERENCE_SET_DELIMITERS
				demarkatePosReferenceSetDelimiters(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, lastIndexOfPreviousReferenceSet, firstIndexOfReferenceSet, lastIndexOfReferenceSet, currentWord->referenceSetStartCodonDeterminerType);
				#endif
				lastIndexOfPreviousReferenceSet = lastIndexOfReferenceSet;
				firstIndexOfPreviousReferenceSet = firstIndexOfReferenceSet;
			}
			else
			{
				
			}
			
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
			if(!setReferenceConceptNodes(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, currentWord, i, lastIndexOfPreviousReferenceSet, firstIndexOfPreviousReferenceSet))
			{
				result = false;
			}
			#endif	
		}
	}

	
	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
bool SANIgenerateCompactIdentifyReferenceSetsClass::setReferenceConceptNodes(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, LRPpreprocessorPlainTextWord* currentWord, const int startCodonIndex, const int lastIndexOfPreviousReferenceSet, const int firstIndexOfPreviousReferenceSet)
{
	bool result = false;

	if(isReferenceSetStartCodonDeterminerTypeConcept(currentWord->referenceSetStartCodonDeterminerType))
	{
		if(lastIndexOfPreviousReferenceSet != INT_DEFAULT_VALUE)
		{
			//mark all previous reference set SANI word nodes as concept:
			for(int i2=firstIndexOfPreviousReferenceSet; i2<startCodonIndex; i2++)
			{
				SANIGroupNeuralNetwork* nodeContainingWord = NULL;
				if(getSANInodeContainingWord(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, i2, &nodeContainingWord))
				{
					nodeContainingWord->SANIisConceptNeuron = true;
				}
			}
		}

		//mark next referenceset/delimiter SANI word nodes as concept (as no determiners follow):
		if(!detectFollowingCodonDeterminers(forwardPropogationSentenceData, startCodonIndex+1))
		{
			for(int i2=startCodonIndex+1; i2<forwardPropogationSentenceData->sentenceContents->size(); i2++)
			{
				SANIGroupNeuralNetwork* nodeContainingWord = NULL;
				if(getSANInodeContainingWord(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, i2, &nodeContainingWord))
				{
					nodeContainingWord->SANIisConceptNeuron = true;
				}
			}	
		}
	}
	
	return result;
}
#endif



bool SANIgenerateCompactIdentifyReferenceSetsClass::isReferenceSetStartCodonDeterminerTypeAll(const int referenceSetStartCodonDeterminerType)
{
	bool result = false;
	
	if((referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_DEFINITE) || (referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_INDEFINITE) || isReferenceSetStartCodonDeterminerTypeConcept(referenceSetStartCodonDeterminerType))	//|| (referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_PROPERNOUN)
	{
		result = true;
	}
	
	return result;
}
bool SANIgenerateCompactIdentifyReferenceSetsClass::isReferenceSetStartCodonDeterminerTypeConcept(const int referenceSetStartCodonDeterminerType)
{
	bool result = false;
	
	if((referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT_SUBJECT_OR_OBJECT) || (referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT_SUBJECT) || (referenceSetStartCodonDeterminerType == SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_START_CODON_CONCEPT_OBJECT))
	{
		result = true;
	}
	
	return result;
}


bool SANIgenerateCompactIdentifyReferenceSetsClass::detectFollowingCodonDeterminers(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int startIndex)
{
	bool result = false;

	for(int i=startIndex; i<forwardPropogationSentenceData->sentenceContents->size(); i++)
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
		
		if(isReferenceSetStartCodonDeterminerTypeAll(currentWord->referenceSetStartCodonDeterminerType))
		{
			result = true;
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

	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_PROPERNOUNS
	//definite detection
	if(LRPpreprocessorWordIdentification.determineIsLikelyPropernoun(currentWord))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_NAME_PROPERNOUN;
		contextWordPOSisAmbiguous = false;
	}	
	/*
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_FIRST_MALE))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_FIRST_MALE;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_FIRST_FEMALE))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_FIRST_FEMALE;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_FAMILY))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_FAMILY;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_PLACE))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_PLACE;
		contextWordPOSisAmbiguous = false;
	}
	*/
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_AUXILIARIES
	//SANI interprets auxiliaries as unambiguous
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_PREPROCESSOR_POS_TYPE_POSSESSIVEENDING))
	{
		contextWordUnambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_POSSESSIVEENDING;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_PREPROCESSOR_POS_TYPE_AUXILIARY_BEING))
	{
		contextWordUnambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_AUXILIARY_BEING;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_PREPROCESSOR_POS_TYPE_AUXILIARY_HAVING))
	{
		contextWordUnambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_AUXILIARY_HAVING;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_PREPROCESSOR_POS_TYPE_AUXILIARY_DOING))
	{
		contextWordUnambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_AUXILIARY_DOING;
		contextWordPOSisAmbiguous = false;
	}
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_CONJUNCTIONS
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_COORDINATING))
	{
		contextWordUnambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_COORDINATING;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_SUBCOORDINATING))
	{
		contextWordUnambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_SUBCOORDINATING;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_CORRELATIVE_FIRST))
	{
		contextWordUnambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_CORRELATIVE_FIRST;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_CORRELATIVE_SECOND))
	{
		contextWordUnambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_CORRELATIVE_SECOND;
		contextWordPOSisAmbiguous = false;
	}
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_PRONOUNS
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_DEMONSTRATIVE))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_PRONOUN_DEMONSTRATIVE;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_INDEFINITE))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_PRONOUN_INDEFINITE;
		contextWordPOSisAmbiguous = false;
	}	
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_INTERROGATIVE))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_PRONOUN_INTERROGATIVE;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_PERSONAL_OBJECT))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_PRONOUN_PERSONAL_OBJECT;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_PERSONAL_SUBJECT))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_PRONOUN_PERSONAL_SUBJECT;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_POSSESSIVE_ADJECTIVE))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_PRONOUN_POSSESSIVE_ADJECTIVE;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_POSSESSIVE_ALONE))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_PRONOUN_POSSESSIVE_ALONE;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_REFLEXIVE))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_PRONOUN_REFLEXIVE;
		contextWordPOSisAmbiguous = false;
	}
	if(LRPpreprocessorPOStagger.getPOSambiguityInfoBit(contextWordPOSambiguityInfo, LRP_SHARED_POS_TYPE_PRONOUN_RELATIVE))
	{
		contextWordUnambiguousPOSindex = LRP_SHARED_POS_TYPE_PRONOUN_RELATIVE;
		contextWordPOSisAmbiguous = false;
	}
	#endif
	
	currentWord->POSambiguityInfo = contextWordPOSambiguityInfo;	
		
	if(!contextWordPOSisAmbiguous)
	{
		currentWord->wordPOStypeInferred = contextWordUnambiguousPOSindex;
		currentWord->unambiguousPOSindex = contextWordUnambiguousPOSindex;

		SANIGroupNeuralNetwork* nodeContainingWord = NULL;
		if(getSANInodeContainingWord(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, i, &nodeContainingWord))
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_ENFORCE_SINGLE_UNIQUE_DIRECT_INPUT
			if(verifySingleDirectInput(nodeContainingWord, i))
			{
			#endif

				//cout << "getSANInodeContainingWord" << endl;
				#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
				cout << "SANIgenerateCompactIdentifyReferenceSetsClass::demarkatePosUnambiguousEntities: currentWord = " << currentWord->tagName << ", currentWord->wordPOStypeInferred = " << currentWord->wordPOStypeInferred << endl;
				#endif

				/*
				referenceSetDelimiter->posValue = contextWordUnambiguousPOSindex;
				referenceSetDelimiter->posAmbiguousInfo = contextWordPOSambiguityInfo;
				referenceSetDelimiter->isPOSambiguous = contextWordPOSisAmbiguous;
				*/

				#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
				setANNneuronSANIposType(nodeContainingWord, contextWordUnambiguousPOSindex);
				#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_ENFORCE_SINGLE_UNIQUE_DIRECT_INPUT
			}
			#endif
		}	
	}
		
	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_ENFORCE_SINGLE_UNIQUE_DIRECT_INPUT
bool SANIgenerateCompactIdentifyReferenceSetsClass::verifySingleDirectInput(SANIGroupNeuralNetwork* nodeContainingWord, const int currentWordIndex)
{
	bool result = true;
	SANIGroupNeuralNetwork* componentSourceFound = NULL;
	
	for(int i=0; i<nodeContainingWord->components.size(); i++)
	{		
		SANIComponentNeuralNetwork* component = (nodeContainingWord->components).at(i);
		if(SANInodes.hasComponentTypeString(component))
		{
			SANIGroupNeuralNetwork* componentSource = component->SANIbackGroupConnectionList[0];
			if(componentSourceFound == NULL)
			{
				componentSourceFound = componentSource;
			}
			else if(componentSource == componentSourceFound)
			{
			
			}
			else
			{
				result = false;
			}	
		}
	}	
	
	return result;
}
#endif
			
//CHECKTHIS;
bool SANIgenerateCompactIdentifyReferenceSetsClass::getSANInodeContainingWord(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, const int indexOfWord, SANIGroupNeuralNetwork** nodeContainingWord)
{
	bool result = false;

	int wordPhraseLength = 1;
	int minNumberWordContiguityErrors = REALLY_LARGE_INT; //large number
	if(findCurrentSentenceReferenceSet(forwardPropogationSentenceData, indexOfWord, wordPhraseLength, topLevelParseTreeGroup, nodeContainingWord, &minNumberWordContiguityErrors, 0))
	{
		#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
		cout << "SANIgenerateCompactIdentifyReferenceSetsClass::getSANInodeContainingWord, wordText = " << forwardPropogationSentenceData->sentenceContents->at(indexOfWord)->tagName << ", nodeContainingWord->groupIndex = " << (*nodeContainingWord)->groupIndex << endl;
		#endif
		result = true;
	}
	
	return result;
}
		
		
#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
bool SANIgenerateCompactIdentifyReferenceSetsClass::setANNneuronSANIposType(SANIGroupNeuralNetwork* nodeContainingWord, const int contextWordUnambiguousPOSindex)
{
	bool result = true;
	
	if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_NOUN)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}
	if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_VERB)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_ACTION;
		//nodeContainingWord->GIAisRelationship = true;
	}	
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_PREPOSITION)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_CONDITION;
		//nodeContainingWord->GIAisRelationship = true;
	}
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_ADJECTIVE)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_QUALITY;
	}
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_ADVERB)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_QUALITY;
	}
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_PROPERNOUNS
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_NAME_PROPERNOUN)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}	
	/*
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_FIRST_MALE)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_FIRST_FEMALE)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_FAMILY)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_NAME_PROPERNOUN_PLACE)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}
	*/
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_AUXILIARIES
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_POSSESSIVEENDING)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_PROPERTY;
		//nodeContainingWord->GIAisRelationship = true;
	}
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_AUXILIARY_BEING)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_DEFINITION;
		//nodeContainingWord->GIAisRelationship = true;
	}
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_AUXILIARY_HAVING)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_PROPERTY;
		//nodeContainingWord->GIAisRelationship = true;
	}
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_AUXILIARY_DOING)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_ACTION;
		//nodeContainingWord->GIAisRelationship = true;
	}
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_CONJUNCTIONS
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_COORDINATING)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_LOGIC_REFERENCE;
		//nodeContainingWord->GIAisRelationship = true;
	}
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_SUBCOORDINATING)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_LOGIC_REFERENCE;
		//nodeContainingWord->GIAisRelationship = true;
	}
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_CORRELATIVE_FIRST)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_LOGIC_REFERENCE;
		//nodeContainingWord->GIAisRelationship = true;
	}
	else if(contextWordUnambiguousPOSindex == LRP_PREPROCESSOR_POS_TYPE_CONJUNCTION_CORRELATIVE_SECOND)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_LOGIC_REFERENCE;
		//nodeContainingWord->GIAisRelationship = true;
	}	
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_PRONOUNS
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_PRONOUN_DEMONSTRATIVE)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_PRONOUN_INDEFINITE)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_PRONOUN_INTERROGATIVE)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_PRONOUN_PERSONAL_OBJECT)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_PRONOUN_PERSONAL_SUBJECT)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_PRONOUN_POSSESSIVE_ADJECTIVE)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}	
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_PRONOUN_POSSESSIVE_ALONE)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}	
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_PRONOUN_REFLEXIVE)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}	
	else if(contextWordUnambiguousPOSindex == LRP_SHARED_POS_TYPE_PRONOUN_RELATIVE)
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_SUBSTANCE;
		//nodeContainingWord->GIAisRelationship = false;
	}	
	#endif
	else
	{
		nodeContainingWord->neuronReference->SANIentityType = LRP_SHARED_ENTITY_TYPE_UNDEFINED;
	}
	
	return result;
}
#endif
			
#endif
		
#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_OF_REFERENCE_SET_DELIMITERS
bool SANIgenerateCompactIdentifyReferenceSetsClass::demarkatePosReferenceSetDelimiters(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, const int lastIndexOfPreviousReferenceSet, const int firstIndexOfReferenceSet, const int lastIndexOfReferenceSet, const int referenceSetStartCodonDeterminerType)
{
	bool result = true;

	//demarkate previous reference set delimiter:
	
	if(lastIndexOfPreviousReferenceSet != INT_DEFAULT_VALUE)
	{
		int indexOfReferenceSetDelimiter = lastIndexOfPreviousReferenceSet+1;	//indexOfPreviousReferenceSetDelimiter
		int referenceSetDelimiterPhraseLength = firstIndexOfReferenceSet-lastIndexOfPreviousReferenceSet;
		if(!demarkatePosReferenceSetDelimiters(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, indexOfReferenceSetDelimiter, referenceSetDelimiterPhraseLength, referenceSetStartCodonDeterminerType))
		{
			result = false;
		}

		#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
		bool edgeIsRefSetToDelimiter = false;
		int referenceSetDelimiterWordIndex1 = firstIndexOfReferenceSet-1;
		int referenceSetDelimiterWordIndex2 = firstIndexOfReferenceSet;
		if(!setANNneuronSANIrefsetConnectionType(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, edgeIsRefSetToDelimiter, referenceSetDelimiterWordIndex1, referenceSetDelimiterWordIndex2))
		{
			result = false;
		}	
		#endif
	}


	//demarkate current reference set delimiter:
	
	if(!detectFollowingCodonDeterminers(forwardPropogationSentenceData, lastIndexOfReferenceSet+1))
	{
		//assume that the last section of the text contains a reference set delimiter (rather than a reference set object)
		int indexOfReferenceSetDelimiter = lastIndexOfReferenceSet+1;
		int referenceSetDelimiterPhraseLength = forwardPropogationSentenceData->sentenceContents->size() - indexOfReferenceSetDelimiter;
		if(!demarkatePosReferenceSetDelimiters(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, indexOfReferenceSetDelimiter, referenceSetDelimiterPhraseLength, referenceSetStartCodonDeterminerType))
		{
			result = false;
		}	
	}

	#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
	bool edgeIsRefSetToDelimiter = true;
	int referenceSetDelimiterWordIndex1 = lastIndexOfReferenceSet;
	int referenceSetDelimiterWordIndex2 = lastIndexOfReferenceSet+1;
	if(!setANNneuronSANIrefsetConnectionType(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, edgeIsRefSetToDelimiter, referenceSetDelimiterWordIndex1, referenceSetDelimiterWordIndex2))
	{
		result = false;
	}	
	#endif
		
}

bool SANIgenerateCompactIdentifyReferenceSetsClass::demarkatePosReferenceSetDelimiters(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, const int indexOfReferenceSetDelimiter, const int referenceSetDelimiterPhraseLength, const int referenceSetStartCodonDeterminerType)
{
	bool result = true;		
		
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
		if(referenceSetDelimiterWord->unambiguousPOSindex == LRP_SHARED_POS_TYPE_UNDEFINED)	//ie ambiguous	//assumes SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES has already been executed
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

		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
		if(!isReferenceSetStartCodonDeterminerTypeConcept(referenceSetStartCodonDeterminerType))
		{
			if(LRPpreprocessorWordIdentification.determineIsAuxiliaryBeing(referenceSetDelimiterWord))
			{
				//interpret "is" auxiliary as quality rather than definition
				referenceSetDelimiterWord->POSambiguityInfo = LRPpreprocessorPOStagger.setPOSambiguityInfoBit(LRP_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, LRP_SHARED_POS_TYPE_ADJECTIVE, true);
				referenceSetDelimiterWord->wordPOStypeInferred = LRP_SHARED_POS_TYPE_ADJECTIVE;
				referenceSetDelimiterWord->unambiguousPOSindex = LRP_SHARED_POS_TYPE_ADJECTIVE;
			}
		}
		#endif
	}

	if(!foundUnambiguousRelationshipEntity)
	{
		//set the ambigous relationship entity to unambious (as it has been inferred as the relationship entity within the referenceSetDelimiter text)
		if(referenceSetDelimiterWordFoundSingleAmbiguous)
		{
			LRPpreprocessorPlainTextWord* referenceSetDelimiterWordAmbiguous = forwardPropogationSentenceData->sentenceContents->at(referenceSetDelimiterWordFoundSingleAmbiguousIndex);
			if(LRPpreprocessorWordIdentification.determineIsVerb(referenceSetDelimiterWordAmbiguous, LRP_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE_IRRELEVANT))
			{
				#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
				cout << "SANIgenerateCompactIdentifyReferenceSetsClass::demarkatePosReferenceSetDelimiters: referenceSetDelimiterWordAmbiguous = " << referenceSetDelimiterWordAmbiguous->tagName << ", LRP_PREPROCESSOR_POS_TYPE_VERB" << endl;
				#endif
				referenceSetDelimiterWordAmbiguous->POSambiguityInfo = LRPpreprocessorPOStagger.setPOSambiguityInfoBit(LRP_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, LRP_PREPROCESSOR_POS_TYPE_VERB, true);
				referenceSetDelimiterWordAmbiguous->wordPOStypeInferred = LRP_PREPROCESSOR_POS_TYPE_VERB;
				referenceSetDelimiterWordAmbiguous->unambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_VERB;
			}
			if(LRPpreprocessorWordIdentification.determineIsPreposition(referenceSetDelimiterWordAmbiguous))
			{
				#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
				cout << "SANIgenerateCompactIdentifyReferenceSetsClass::demarkatePosReferenceSetDelimiters: referenceSetDelimiterWordAmbiguous = " << referenceSetDelimiterWordAmbiguous->tagName << ", LRP_PREPROCESSOR_POS_TYPE_PREPOSITION" << endl;
				#endif
				referenceSetDelimiterWordAmbiguous->POSambiguityInfo = LRPpreprocessorPOStagger.setPOSambiguityInfoBit(LRP_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN, LRP_PREPROCESSOR_POS_TYPE_PREPOSITION, true);
				referenceSetDelimiterWordAmbiguous->wordPOStypeInferred = LRP_PREPROCESSOR_POS_TYPE_PREPOSITION;
				referenceSetDelimiterWordAmbiguous->unambiguousPOSindex = LRP_PREPROCESSOR_POS_TYPE_PREPOSITION;
			}
		}
	}

	//update ANNneuron pos types;
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
	
		if(!referenceSetDelimiterWordAmbiguous)
		{
			//currentWord->wordPOStypeInferred = contextWordUnambiguousPOSindex;
			//currentWord->unambiguousPOSindex = contextWordUnambiguousPOSindex;

			SANIGroupNeuralNetwork* referenceSetDelimiterNode = NULL;
			if(getSANInodeContainingWord(forwardPropogationSentenceData, SANIGroupTypes, topLevelParseTreeGroup, i, &referenceSetDelimiterNode))
			{
				//cout << "getSANInodeContainingWord" << endl;
				#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
				setANNneuronSANIposType(referenceSetDelimiterNode, referenceSetDelimiterWord->unambiguousPOSindex);
				#endif
			}	
		}
	}

	return result;	
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
				referenceSetDelimiterComponent1->refsetConnectionType = LRP_SHARED_ENTITY_CONNECTION_TYPE_SAMEREFERENCESET;
				referenceSetDelimiterComponent2->refsetConnectionType = LRP_SHARED_ENTITY_CONNECTION_TYPE_DIFFREFERENCESET;
			}
			else
			{
				referenceSetDelimiterComponent1->refsetConnectionType = LRP_SHARED_ENTITY_CONNECTION_TYPE_DIFFREFERENCESET;
				referenceSetDelimiterComponent2->refsetConnectionType = LRP_SHARED_ENTITY_CONNECTION_TYPE_SAMEREFERENCESET;			
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
				#ifdef DEBUG_SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE_ACTIVATION_SIGNAL
				cout << "SANIgenerateCompactIdentifyReferenceSetsClass::findReferenceSetDelimiter" << endl;
				#endif
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
