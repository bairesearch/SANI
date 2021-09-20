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
 * Project Version: 1q2b 19-September-2021
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
#include "SANIpropagateOperations.hpp"
#include "LRPpreprocessorWordIdentification.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "LRPpreprocessorPOStagger.hpp"




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
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorPOStaggerClass LRPpreprocessorPOStagger;

	
	#ifdef SANI_SEQUENCE_GRAMMAR_LINK_SIMILAR_SUBNETS
	public: bool linkSimiliarSubnets(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* topLevelParseTreeGroup);
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS
		private: bool identifyReferenceSetDelimiters(SANItranslatorVariablesClass* translatorVariables, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup);
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_VIA_DETERMINERS_SUPPORT_CONCEPTS
			private: bool setReferenceConceptNodes(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, LRPpreprocessorPlainTextWord* currentWord, const int startCodonIndex, const int lastIndexOfPreviousReferenceSet, const int firstIndexOfPreviousReferenceSet);
			#endif
			private: bool isReferenceSetStartCodonDeterminerTypeAll(const int referenceSetStartCodonDeterminerType);
			private: bool isReferenceSetStartCodonDeterminerTypeConcept(const int referenceSetStartCodonDeterminerType);
			private: bool detectFollowingCodonDeterminers(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int startIndex);
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES
			private: bool demarkatePosUnambiguousEntities(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, LRPpreprocessorPlainTextWord* currentWord, const int i);
				#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_UNAMBIGUOUS_ENTITIES_ENFORCE_SINGLE_UNIQUE_DIRECT_INPUT
				private: bool verifySingleDirectInput(SANIGroupNeuralNetwork* nodeContainingWord, const int currentSequenceIndex);
				#endif
				private: bool getSANInodeContainingWord(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, const int indexOfWord, SANIGroupNeuralNetwork** nodeContainingWord);
				#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
				private: bool setANNneuronSANIposType(SANIGroupNeuralNetwork* nodeContainingWord, const int contextWordUnambiguousPOSindex);
				#endif
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_DEMARKATE_POS_OF_REFERENCE_SET_DELIMITERS
			private: bool demarkatePosReferenceSetDelimiters(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, const int lastIndexOfPreviousReferenceSet, const int firstIndexOfReferenceSet, const int lastIndexOfReferenceSet, const int referenceSetStartCodonDeterminerType);
				private: bool demarkatePosReferenceSetDelimiters(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, const int indexOfReferenceSetDelimiter, const int referenceSetDelimiterPhraseLength, const int referenceSetStartCodonDeterminerType);			
				#ifdef SANI_ANN_COLOUR_CONNECTIONS_BASED_ON_POS
				private: bool setANNneuronSANIrefsetConnectionType(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes, SANIGroupParseTree* topLevelParseTreeGroup, const bool edgeIsRefSetToDelimiter, const int referenceSetDelimiterSequenceIndex1, const int referenceSetDelimiterSequenceIndex2);
				#endif
			#endif
			private: bool identifyMostLikelyReferenceSetCandidate(vector<multimap<double, SANIGroupNeuralNetwork*>>* propagatedGroupsListPerformanceTupleArray, const int tupleSizeMax, SANIGroupNeuralNetwork** mostLikelyCandidateReferenceSetGroup, int* mostLikelyCandidateReferenceSetPhraseLength, double* mostLikelyCandidateReferenceSetSimilarity, SANIGroupParseTree* topLevelParseTreeGroup);
				private: bool findNeuronInParseTree(SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork* neuronToFind, const int layer);
			private: bool findCurrentSentenceReferenceSet(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int tupleFirstIndex, const int referenceSetPhraseLength, SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork** currentSentenceReferenceSet, int* minNumberWordContiguityErrors, const int layer);
				private: int calculateNumberWordContiguityErrors(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int tupleFirstIndex, const int referenceSetPhraseLength, SANIGroupParseTree* currentParseTreeGroup);
			private: bool findReferenceSetDelimiter(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int referenceSetDelimiterSequenceIndex1, const int referenceSetDelimiterSequenceIndex2, SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork** currentSentenceReferenceSet, SANIComponentNeuralNetwork** referenceSetDelimiterComponent1, SANIComponentNeuralNetwork** referenceSetDelimiterComponent2, const int layer);	
			private: void createDirectAssociationConnection(SANIGroupNeuralNetwork* currentSentenceReferenceSet, SANIGroupNeuralNetwork* mostLikelyCandidateReferenceSetGroup, double mostLikelyCandidateReferenceSetSimilarity);
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_STORE_RECENCY_UPDATE_SUCCESSFULLY_PARSED_NEURONS
		private: bool updateNetworkRecency(int timeIndex, SANIGroupParseTree* currentParseTreeGroup, const int layer);
		#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_REFERENCE_SET_IDENTIFICATION_WITHOUT_SEQUENTIALITY
		private: bool findAndLinkReferenceSetCandidates1(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* currentParseTreeGroup, const int layer);
			private: bool findAndLinkReferenceSetCandidates2(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupParseTree* currentParseTreeGroup, SANIGroupNeuralNetwork* referenceSetCandidate1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const int layer);
				private: bool findAndLinkReferenceSetCandidates3(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupNeuralNetwork* referenceSetCandidate2, SANIGroupNeuralNetwork* referenceSetCandidate1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const int layer);
					private: bool findAndLinkReferenceSetCandidatesCompareVectors(constEffective SANIGroupNeuralNetwork** referenceSetCandidate2best, SANIGroupNeuralNetwork* referenceSetCandidate1, constEffective SANIGroupNeuralNetwork* referenceSetCandidate2, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector1, const vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector2);
					private: bool createReferenceSetCandidateVector(SANIGroupParseTree* currentParseTreeGroup, vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector);
					private: bool createReferenceSetCandidateVector(SANIGroupNeuralNetwork* currentNeuron, vector<SANIGroupNeuralNetwork*>* referenceSetCandidateVector);			
		#endif
	#endif
};

#endif

#endif
