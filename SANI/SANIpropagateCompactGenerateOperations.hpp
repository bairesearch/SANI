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
 * File Name: SANIpropagateCompactGenerateOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1o2a 08-November-2020
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Compact Generate Operations - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_COMPACT_GENERATE_OPERATIONS
#define HEADER_SANI_PROPAGATE_COMPACT_GENERATE_OPERATIONS

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
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIposRelTranslatorVariables.hpp"
#include "SANIpropagateCompactOperations.hpp"
#include "SANIpropagateOperationsParseTree.hpp"


#ifdef SANI_SEQUENCE_GRAMMAR

class SANIpropagateCompactGenerateOperationsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateCompactOperationsClass SANIpropagateCompactOperations;
	private: SANIpropagateOperationsParseTreeClass SANIpropagateOperationsParseTree;


	//from SANIpropagateCompactClass:
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	public: bool verifyMissingOrVariableStartComponentFound(SANItranslatorVariablesClass* translatorVariables, int layer, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, SANIComponentNeuralNetwork* previousActiveComponent, SANIComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFoundStartComponent, bool* missingStartComponentFound, bool* variableStartComponentFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
	public: bool verifyMissingOrVariableStartComponentFoundAllowedWordIndices(SANItranslatorVariablesClass* translatorVariables, int layer, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, SANIComponentNeuralNetwork* previousActiveComponent, SANIComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFoundStartComponent, bool* missingStartComponentFound, bool* variableStartComponentFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_LAST_COMPONENTS
	public: bool propagateVariableEndComponentRemoveLastParseTreeComponent(SANItranslatorVariablesClass* translatorVariables, int layer, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, SANIComponentNeuralNetwork* previousActiveComponent, SANIComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFoundEndComponent, const bool componentWordConnectivityTests, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
	#endif
	public: bool updateActivatedNeuronWithMaxWordIndexCoverageWrapper(SANIGroupNeuralNetwork* ownerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, const bool sequentialActivationFound, const bool missingStartComponentFound, const bool missingOrVariableStartComponentFound, const bool missingOrVariableEndComponentFound);
		public: bool updateActivatedNeuronWithMaxWordIndexCoverage(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool missingStartComponentFound, const bool missingOrVariableStartComponentFound, const bool missingOrVariableEndComponentFound, const bool candidateCoveragePartial);			
			private: bool verifyActivatedNeuronWithMaxWordIndexCoverage(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIGroupParseTree* activatedNeuronBaseline, SANIGroupParseTree* activatedNeuronCandidate, const bool testWordIndicesAllowed, const bool testMinNumComponents, const bool candidateCoveragePartial, const bool missingOrVariableStartComponentFound, const bool missingOrVariableEndComponentFound);
				private: bool verifyActivatedNeuronWithMaxWordIndexAllowed(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIGroupParseTree* activatedNeuronCandidate, const bool strictStartingCondition);
				#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
				private: bool verifyNeverSplitGroupBetweenTwoIdenticalComponents(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, const bool candidateCoveragePartial);		
					private: bool neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage);
				#endif
					public: bool verifyActivatedNeuronsAtLeastOneBasic(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage);
					public: bool verifyActivatedNeuronsAtLeastOne(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, SANIGroupNeuralNetwork* newNeuronSequenceGroup1);
					#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
					public: bool identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex);
						public: bool identifyComponentIndexFirstAndLastActivatedIndexUnordered(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* firstActivatedIndex, int* lastActivatedIndex);
					#else
					public: bool identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex);
						public: bool identifyComponentIndexLastActivatedIndexUnordered(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex);	
					#endif
					
	//from SANIpropagateOperations:
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	public: bool identifyMissingOrVariableStartComponentFound(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIComponentNeuralNetwork*>* components, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* missingOrVariableStartComponentFound);
	#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_FIRST_COMPONENTS_SHARED
		public: bool identifyMissingOrVariableStartComponentFound(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIComponentNeuralNetwork*>* components, SANIComponentNeuralNetwork* currentComponent);
		#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
	public: bool identifyMissingOrVariableEndComponentFound(SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIComponentNeuralNetwork*>* components, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, SANIComponentNeuralNetwork* currentComponent, SANIComponentNeuralNetwork* previousActiveComponent, bool* missingOrVariableEndComponentFound);
	#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_LAST_COMPONENTS_SHARED
		public: bool identifyMissingOrVariableEndComponentFound(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIComponentNeuralNetwork*>* components, SANIComponentNeuralNetwork* currentComponent);
		#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_SHARED
			private: bool calculateVariableComponentPassCriteria(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* component, vector<SANIComponentNeuralNetwork*>* components, const bool testFirstComponentNotString, const bool testLastComponentNotString, const bool testFirstComponentSubgroupsUnique, const bool testLastComponentSubgroupsUnique, const bool testNotDualLowerLevelConnections);	
			#endif
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	public: bool groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient(SANIGroupNeuralNetwork* currentNeuron);
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	public: bool groupContainsNeuronWithinProspectiveAlternateSubgroupReset(SANIGroupNeuralNetwork* currentNeuron);
	#endif
	#endif

	public: int calculateNextIndexInSequenceProspective(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverageProspective);

	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_VERIFY_THAT_VARIABLE_EDGE_COMPONENT_SOURCE_POS_IS_NOT_IDENTICAL
	public: bool variableEdgeComponentSourcePOSisIdenticalWrapper1(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* generatedNeuron, SANIGroupNeuralNetwork* candidateMatchGroup, bool identifyVariableFirstOrLastComponent);
		private: bool variableEdgeComponentSourcePOSisIdenticalWrapper2(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* neuronWithNewVariableComponent, SANIGroupNeuralNetwork* edgeNeuron, bool identifyVariableFirstOrLastComponent);
			private: bool variableEdgeComponentSourcePOSisIdentical(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* neuronToDetect, SANIGroupNeuralNetwork* edgeNeuron, bool identifyVariableFirstOrLastComponent);
	#endif
};

#endif

#endif
