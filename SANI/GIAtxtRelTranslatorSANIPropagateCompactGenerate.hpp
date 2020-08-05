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
 * File Name: GIAtxtRelTranslatorSANIPropagateCompactGenerate.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k1a 05-March-2020
 * Requirements: 
 * Description: Textual Relation Translator SANI (Sequentially Activated Neuronal Input neural network) Propagate Compact - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_COMPACT_GENERATE
#define HEADER_GIA_TXT_REL_TRANSLATOR_SANI_PROPAGATE_COMPACT_GENERATE

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIAtxtRelTranslatorRulesGroupClass.hpp"
#include "GIAtxtRelTranslatorRulesComponentClass.hpp"
#include "GIAtxtRelTranslatorSANIPropagateOperations.hpp"
#include "GIAtxtRelTranslatorSANIPropagateCompact.hpp"
#include "GIAtxtRelTranslatorSANIFormation.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "ANNdisplay.hpp"



#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR

class GIAtxtRelTranslatorSANIPropagateCompactGenerateClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIAtxtRelTranslatorRulesGroupClass GIAtxtRelTranslatorRulesGroupClassObject;
	private: GIAtxtRelTranslatorRulesComponentClass GIAtxtRelTranslatorRulesComponentClassObject;
	private: GIAtxtRelTranslatorSANIPropagateOperationsClass GIAtxtRelTranslatorSANIPropagateOperations;
	private: GIAtxtRelTranslatorSANIPropagateCompactClass GIAtxtRelTranslatorSANIPropagateCompact;
	private: GIAtxtRelTranslatorSANIFormationClass GIAtxtRelTranslatorSANIFormation;

	private: ANNdisplayClass ANNdisplay;

	public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool createNewConnections);
		public: bool executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance, const bool createNewConnection);
			private: bool findAndReconcileVariation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
				private: bool findAndReconcileIncrementalVariation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData);
					#ifndef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
					private: GIAtxtRelTranslatorRulesGroupNeuralNetwork* splitNeuron(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* partiallyActivatedNeuronWithMaxWordIndexCoverage);
					#endif
					private: bool addNeuronToList(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* listOfHighLevelNeurons, GIAtxtRelTranslatorRulesGroupNeuralNetwork* neuron, int* indexInSequence);
					private: bool connectListOfHighLevelNeuronsToNewNeuron(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* listOfHighLevelNeurons, GIAtxtRelTranslatorRulesGroupNeuralNetwork** grammaticalSentenceNeuron, const bool createTopLevelNeuron);
					#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_COMPONENT_DETECT_LOCAL_VARATION
					private: bool verifyAndMarkNeuronAsCandidateWrapper(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* candidateNeuronBaseParseTree, GIAtxtRelTranslatorRulesComponentNeuralNetwork** candidateComponent2);
						private: bool verifyAndMarkNeuronAsCandidate(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentNeuron, GIAtxtRelTranslatorRulesComponentNeuralNetwork** candidateComponent2);
					#endif
					private: bool createOrAppendFirstLevelHiddenLayerGroup(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentLayerNeuronGroupStart, bool* creatingNewNeuronSequence, int* neuronSequenceIndex, GIAtxtRelTranslatorRulesGroupNeuralNetwork** newNeuronSequenceGroup, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* listOfHighLevelNeurons);
					#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
					private: bool directWireLowLevelPOSneuronToGrammaticalSentenceNeuron(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* singleComponentNeuronCandidate, GIAtxtRelTranslatorRulesGroupNeuralNetwork* grammaticalSentenceNeuron);
					#endif

	private: bool addComponentToFirstLevelHiddenLayerGroup(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerPOSneuron, GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronSequenceGroup);
	private: GIAtxtRelTranslatorRulesGroupNeuralNetwork* createNewHiddenLayerGroup(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes);
	//private: GIAtxtRelTranslatorRulesGroupNeuralNetwork* createNewFirstLevelHiddenLayerGroup(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerPOSneuron);
	private: GIAtxtRelTranslatorRulesGroupNeuralNetwork* splitGroupAtLastActivatedComponent(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorRulesGroupNeuralNetwork* neuronToSplit, int indexToSplitVector);
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
	private: GIAtxtRelTranslatorRulesGroupNeuralNetwork* splitGroupAtLastActivatedComponentUnordered(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorRulesGroupNeuralNetwork* neuronToSplit, int indexToSplitVector1, int indexToSplitVector2);
	#endif	

	private: bool addComponentToGroup(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupNeuralNetwork* higherLevelComponentGroupOwner, int componentType, bool insertAtStart);
	private: bool addComponentToGroup(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent);
		private: GIAtxtRelTranslatorRulesGroupNeuralNetwork* createNewGroup();

	private: bool findGroupDirectlyAbove(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group1, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group2);

	private: bool verifyLastWordIndex(GIAtxtRelTranslatorSANIForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroup, int lastWordIndex, bool* adjacent);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
	private: bool addSubneuronsToList(vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* listOfHighLevelNeuronsCompleteHistory, GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentNeuron);
	#endif
};

#endif

#endif