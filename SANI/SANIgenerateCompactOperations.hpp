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
 * File Name: SANIgenerateCompactOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Operations - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_GENERATE_COMPACT_OPERATIONS
#define HEADER_SANI_GENERATE_COMPACT_OPERATIONS

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "LRPpreprocessorSentenceClass.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "LRPpreprocessorPOStagger.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperations.hpp"
#include "SANIpropagateOperationsParseTree.hpp"
#include "SANIpropagateCompactGenerateOperations.hpp"	//required for SANIpropagateCompactGenerateOperationsClass::calculateNextIndexInSequenceProspective
#include "SANIpropagateCompact.hpp"
#include "SANIgenerateCompactIdentify.hpp"
#include "SANIformation.hpp"




#ifdef SANI_SEQUENCE_GRAMMAR

class SANIgenerateCompactOperationsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: LRPpreprocessorPOStaggerClass LRPpreprocessorPOStagger;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: SANIpropagateOperationsParseTreeClass SANIpropagateOperationsParseTree;
	private: SANIpropagateCompactGenerateOperationsClass SANIpropagateCompactGenerateOperations;
	private: SANIpropagateCompactClass SANIpropagateCompact;
	private: SANIformationClass SANIformation;
	private: SANIgenerateCompactIdentifyClass SANIgenerateCompactIdentify;
	

	public: bool addComponentToFirstLevelHiddenLayerGroup(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* inputLayerPOSneuron, SANIGroupNeuralNetwork* newNeuronSequenceGroup);
	public: SANIGroupNeuralNetwork* createNewHiddenLayerGroup(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupType*>* SANIGroupTypes);

	public: bool addVariableComponentToGroup(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelComponentGroupOwner, bool addToStart);
	public: bool addComponentToGroup(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelComponentGroupOwner, const bool componentTypeString, bool insertAtStart);
	public: bool addComponentToGroup(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* higherLevelComponent);
		public: SANIGroupNeuralNetwork* createNewGroup();

	private: bool findGroupDirectlyAbove(const SANIGroupNeuralNetwork* group1, const SANIGroupNeuralNetwork* group2);

	private: bool verifyLastSequenceIndex(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIGroupParseTree* parseTreeGroup, const int lastSequenceIndex, bool* adjacent);
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING

	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_DO_NOT_HAVE_CONNECTIONS_TO_ANY_NEW_CANDIDATE
	public: bool addParentNeuronsToList(vector<SANIGroupNeuralNetwork*>* listOfHighLevelNeuronsCompleteHistory, SANIGroupNeuralNetwork* currentNeuron);
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	public: bool addParentNeuronsToListReset(SANIGroupNeuralNetwork* currentNeuron);
	#endif
	
	#endif
	#endif
		
	public: int calculateNextIndexInSequence(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
	
	#ifdef SANI_SEQUENCE_GRAMMAR_ADD_NEW_NEURONS_TO_SYMMETRICAL_TREE
	public: bool findLowestLayerNeuron(const vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** lowestLayerNeuron, int* lowestLayerNeuronIndex);
	public: bool findHighestLayerNeuron(const vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, SANIGroupNeuralNetwork** highestLayerNeuron, int* highestLayerNeuronIndex);
	public: bool updateHighLevelNeuronHierachy(vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIGroupNeuralNetwork*>* highLevelNeuronPriorArray, const int lowestLayerNeuronIndex, SANIGroupNeuralNetwork* grammaticalSentenceNeuronSub, const bool completedIdentifyingSentenceHighLevelNeurons);
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_MARK_AS_UNAMBIGUOUS
	public: bool markAmbiguousFirstHiddenLayerNeuronsAsUnambiguous(const SANIGroupParseTree* currentParseTreeGroup, const bool previousUnambiguousDetections);
	#endif	
};

#endif

#endif
