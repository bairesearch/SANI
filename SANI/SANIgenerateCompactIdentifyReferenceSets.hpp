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
 * File Name: SANIgenerateCompactIdentifyReferenceSets.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p1b 04-March-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Identify Reference Sets - identify and connect reference sets
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_GENERATE_COMPACT_IDENTIFY_REFERENCESETS
#define HEADER_SANI_GENERATE_COMPACT_IDENTIFY_REFERENCESETS

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif
#include "SHAREDvars.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANInodesGroupClass.hpp"
#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE
#include "SANIpropagateCompactReferenceSets.hpp"
#endif
#include "LRPpreprocessorWordIdentification.hpp"
#include "LRPpreprocessorWordClass.hpp"

//copied from GIAsynRelTranslatorDefs.hpp [needs to be moved to LRP]
#define GRAMMATICAL_DETERMINER_DEFINITE "the"
#define GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR "a"
#define GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL "some"
#define GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR_FIRST_LETTER_VOWEL "an"
#define GRAMMATICAL_DETERMINER_DEFINITE_EACH "each"
#define GRAMMATICAL_DETERMINER_DEFINITE_EVERY "every"
#define GRAMMATICAL_DETERMINER_INDEFINITE_ALL "all"
//not required for Stanford CoreNLP as "an" lemma is "a" (but is required for Relex)
#define GRAMMATICAL_DETERMINER_INDEFINITE_NUMBER_OF_TYPES (2)
static string grammaticalDeterminerIndefiniteArray[GRAMMATICAL_DETERMINER_INDEFINITE_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR, GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR_FIRST_LETTER_VOWEL};	//NB this intentionally discludes GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL "some" as this is handled the same as a definite determinier by GIA2 POS tag system
#define GRAMMATICAL_DETERMINER_DEFINITE_NUMBER_OF_TYPES (3)
static string grammaticalDeterminerDefiniteArray[GRAMMATICAL_DETERMINER_DEFINITE_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_DEFINITE, GRAMMATICAL_DETERMINER_DEFINITE_EACH, GRAMMATICAL_DETERMINER_DEFINITE_EVERY};	//NB this intentionally discludes GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL "some" as this is handled the same as a definite determinier by GIA2 POS tag system



#ifdef SANI_SEQUENCE_GRAMMAR

class SANIgenerateCompactIdentifyReferenceSetsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_PROPAGATE
	private: SANIpropagateCompactReferenceSetsClass SANIpropagateCompactReferenceSets;
	#endif
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LINK_SIMILAR_SUBNETS
	public: bool linkSimiliarSubnets(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* topLevelParseTreeGroup);
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS
		private: bool identifyReferenceSetDelimiters(SANItranslatorVariablesClass* translatorVariables, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup);
			private: void createDirectAssociationConnection(SANIGroupNeuralNetwork* currentSentenceReferenceSet, SANIGroupNeuralNetwork* mostLikelyCandidateReferenceSetGroup, double mostLikelyCandidateReferenceSetSimilarity);
			private: bool findCurrentSentenceReferenceSet(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int tupleFirstIndex, int referenceSetPhraseLength, SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork** currentSentenceReferenceSet, int* minNumberWordContiguityErrors, const int layer);
				private: int calculateNumberWordContiguityErrors(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int tupleFirstIndex, int referenceSetPhraseLength, SANIGroupParseTree* currentParseTreeGroup);
			private: bool identifyMostLikelyReferenceSetCandidate(vector<multimap<double, SANIGroupNeuralNetwork*>>* propagatedGroupsListPerformanceTupleArray, int tupleSizeMax, SANIGroupNeuralNetwork** mostLikelyCandidateReferenceSetGroup, int* mostLikelyCandidateReferenceSetPhraseLength, double* mostLikelyCandidateReferenceSetSimilarity, SANIGroupParseTree* topLevelParseTreeGroup);
				private: bool findNeuronInParseTree(SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork* neuronToFind, const int layer);
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_SUCCESSFULLY_PARSED_NEURONS
		private: bool updateNetworkRecency(int timeIndex, SANIGroupParseTree* currentParseTreeGroup, const int layer);
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
		private: bool findAndLinkReferenceSetCandidates1(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup, const int layer);
			private: bool findAndLinkReferenceSetCandidates2(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork* referenceSetCandidate1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const int layer);
				private: bool findAndLinkReferenceSetCandidates3(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupNeuralNetwork* referenceSetCandidate2, SANIGroupNeuralNetwork* referenceSetCandidate1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const int layer);
					private: bool findAndLinkReferenceSetCandidatesCompareVectors(constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupNeuralNetwork* referenceSetCandidate1, constEffective SANIGroupNeuralNetwork* referenceSetCandidate2, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector2);
					private: bool createReferenceSetCandidateVector(SANIGroupParseTree* currentParseTreeGroup, vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector);
					private: bool createReferenceSetCandidateVector(SANIGroupNeuralNetwork* currentNeuron, vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector);			
		#endif
	#endif
};

#endif

#endif
