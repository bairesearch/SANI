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
 * File Name: SANIgenerateCompactIdentify.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p9a 17-May-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Generate Compact Identify - identify and connect regions
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_GENERATE_COMPACT_IDENTIFY
#define HEADER_SANI_GENERATE_COMPACT_IDENTIFY

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
#include "SANIposRelTranslatorVariables.hpp"
#include "SANIpropagateOperations.hpp"
#include "SANIpropagateCompactOperations.hpp"
#include "SANIpropagateCompactGenerateOperations.hpp"
#ifdef SANI_SEQUENCE_GRAMMAR_LINK_SIMILAR_SUBNETS
#include "SANIgenerateCompactIdentifyReferenceSets.hpp"
#endif



#ifdef SANI_SEQUENCE_GRAMMAR

class SANIgenerateCompactIdentifyClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: SANIpropagateCompactOperationsClass SANIpropagateCompactOperations;
	private: SANIpropagateCompactGenerateOperationsClass SANIpropagateCompactGenerateOperations;
	#ifdef SANI_SEQUENCE_GRAMMAR_LINK_SIMILAR_SUBNETS
	private: SANIgenerateCompactIdentifyReferenceSetsClass SANIgenerateCompactIdentifyReferenceSets;
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_NEWLY_GENERATED_NEURONS
	public: bool addGroupToHiddenLayerGroupsTemp(SANIGroupNeuralNetwork* newGroup);
	public: bool clearHiddenLayerGroupsTemp();
	public: bool setGeneratedForLastSentence(const SANItranslatorVariablesClass* translatorVariables);
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY
	public: bool identifyVariableComponentsAndReferenceSets(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* topLevelParseTreeGroup);
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_DETECTED_EDGE_OF_SENTENCE
		private: bool identifyStartEndOfSentenceWordDetected(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup);
		#endif	
	#endif

	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS
	public: bool identifyVariableComponentsWrapper(const SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
		private: bool identifyVariableComponents(vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
			private: bool identifyVariableFirstLastComponents(vector<SANIGroupType*>* SANIGroupTypes, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool identifyVariableFirstOrLastComponent, SANIGroupNeuralNetwork* generatedNeuron, const SANIComponentNeuralNetwork* variableComponentOfGeneratedNeuron, const SANIComponentNeuralNetwork* nonvariableComponentOfGeneratedNeuron, SANIGroupNeuralNetwork* variableComponentOfGeneratedNeuronSource, SANIGroupNeuralNetwork* nonvariableComponentOfGeneratedNeuronSource);
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_CENTRAL_COMPONENTS
		private: bool findAndLinkCentralVariationCandidates(SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
			private: bool populateSecondComponentOfOutputNeuronSourceVector(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIGroupNeuralNetwork* group, vector<SANIGroupNeuralNetwork*>* secondComponentOfOutputNeuronSourceVector);
		#endif
	#endif
	
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	private: bool markFirstComponentSubNeurons(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* currentNeuron);
		public: bool markSubNeurons(SANIGroupNeuralNetwork* currentNeuron);
	public: bool markSubNeuronsReset(SANIGroupNeuralNetwork* currentNeuron);
	#endif
	#endif
};

#endif

#endif
