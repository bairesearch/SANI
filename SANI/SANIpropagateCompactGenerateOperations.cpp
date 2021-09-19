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
 * File Name: SANIpropagateCompactGenerateOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1q2a 19-September-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Compact Generate Operations - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#include "SANIpropagateCompactGenerateOperations.hpp"
#include "SHAREDvars.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR

#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
bool SANIpropagateCompactGenerateOperationsClass::verifyMissingOrVariableStartComponentFound(const SANItranslatorVariablesClass* translatorVariables, const int layer, SANIGroupNeuralNetwork* ownerGroup, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, const SANIComponentNeuralNetwork* previousActiveComponent, const SANIComponentNeuralNetwork* finalActiveComponent, const bool* existingActivationFoundStartComponent, const bool* missingStartComponentFound, bool* variableStartComponentFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup)
{
	bool result = true;
	
	//consider moving componentWordConnectivityTests test into SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady setting of missingOrVariableStartComponentFound
	if(componentWordConnectivityTests)
	{
		*sequentialActivationFound = true;	//redundant
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_VARIABLE
		if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)
		{
			if(!*missingStartComponentFound)
			{
				if(*missingOrVariableStartComponentFound)
				{
					//guaranteed: testComponent->w != 0 [ie second+ component]

					*missingOrVariableStartComponentFound = false;	//the first component found was variable and correct
					if(numberOfInactiveComponentsRemaining == 0)
					{
						if(*activationSequenceCompleted != true)
						{
							cout << "SANIpropagateCompactGenerateOperationsClass::propagateWordThroughNetworkGroupComponentWrapper warning: (numberOfInactiveComponentsRemaining == 0) && (activationSequenceCompleted != true)" << endl;
						}
						*activationSequenceCompleted = true;	//this be already marked as true by SANIpropagateOperationsClass::propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady
					}
				}
			}
		}
		#endif
	}

	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)
	{
		if(*missingOrVariableStartComponentFound)
		{
			//guaranteed: (missingStartComponentFound || missingStartComponentFound) && testComponent->w != 0 [ie second+ component]

			//test verifyActivatedNeuronWithMaxSequenceIndexAllowed:
			bool firstLastSequenceIndexTestHypotheticalWithoutVariableStartComponent = false; 
			SANIGroupParseTree activatedNeuronCandidate;	//activatedNeuronCandidate is only used for both verifyActivatedNeuronWithMaxSequenceIndexAllowed purposes and updating ownerGroup->currentParseTreeGroupTemp->parseTreeMinSequenceIndex/parseTreeMaxSequenceIndex in the case a variable first component is detected
			activatedNeuronCandidate.parseTreeMaxSequenceIndex = forwardPropogationSequenceElementData->sequenceIndex;
			activatedNeuronCandidate.parseTreeMinSequenceIndex = forwardPropogationSequenceElementData->sequenceIndex;
			//SANIpropagateOperationsParseTree.updateParseTreeMaxMinSequenceIndexOfParentBasic will update activatedNeuronCandidate based on activationPathSequenceElementCurrentParseTreeGroup only (existing group activation parseTreeMaxSequenceIndex/parseTreeMinSequenceIndex will be disgarded as the first component is variable)			
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_VARIABLE
			if(firstActiveComponentInGroup || ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1)))	//ie if missing start component || variable start component	//ie no components have previously been activated (firstActiveComponentInGroup) || 1 component has previously been activated ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))
			{
			#endif
				SANIpropagateOperationsParseTree.updateParseTreeMaxMinSequenceIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationSequenceElementData, activationPathSequenceElementCurrentParseTreeGroup);
				if(verifyActivatedNeuronWithMaxSequenceIndexAllowed(forwardPropogationSequenceData, forwardPropogationSignalData, &activatedNeuronCandidate, true))
				{
					firstLastSequenceIndexTestHypotheticalWithoutVariableStartComponent = true;
				}
				else
				{
					*missingOrVariableStartComponentFound = false;
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFIED
					*sequentialActivationFound = false;
					#endif
				}
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_VARIABLE
			}
			else
			{
				cerr << "potential algorithm error; if(missingOrVariableStartComponentFound); the following should always be true: if(firstActiveComponentInGroup || ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))" << endl;
				exit(EXIT_ERROR);
			}
			#endif

			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_VARIABLE
			if(firstLastSequenceIndexTestHypotheticalWithoutVariableStartComponent)
			{
				if(*missingOrVariableStartComponentFound)
				{
					if(!(*sequentialActivationFound))
					{
						if((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))	//only 1 component has been activated	//ie (!*missingStartComponentFound)
						{
							//variable first component detected
							//sequenceElement indices do not align (ie !sequentialActivationFound), so assume first component was miscellaneously activated (mark it as variable) 

							*variableStartComponentFound = true;
							if(sequentialActivationConnectivityTests(translatorVariables, testComponent, ownerGroup, &activatedNeuronCandidate, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, activationSequenceCompleted, layer, activationPathSequenceElementCurrentParseTreeGroup, *existingActivationFoundStartComponent, variableStartComponentFound))	//CHECKTHIS
							{
								/*update sequentialActivationFound value->true under the assumption the first component is being modified with a new (variable) connection [therefore sequenceIndex alignment with second component is not required]. 
								This is only required for propagateWordThroughNetworkGroupComponent:updateActivatedNeuronWithMaxSequenceIndexCoverage to execute [ie the detection/designation of a fully activatedNeuronWithMaxSequenceIndexCoverage with variable first component]*/
								*sequentialActivationFound = true;	
								
								//ownerGroup->currentParseTreeGroupTemp->variableStartComponentFound = true;
								ownerGroup->currentParseTreeGroupTemp->parseTreeMinSequenceIndex = activatedNeuronCandidate.parseTreeMinSequenceIndex;
								ownerGroup->currentParseTreeGroupTemp->parseTreeMaxSequenceIndex = activatedNeuronCandidate.parseTreeMaxSequenceIndex;
							}
							else
							{
								*missingOrVariableStartComponentFound = false;
							}
						}
					}	
				}
			}
			#endif
		}

		#ifndef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFY2
		if(*missingOrVariableStartComponentFound)
		{
			ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound = true;
		}
		/*
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
		if(missingStartComponentFound)
		{
			ownerGroup->currentParseTreeGroupTemp->missingStartComponentFound = true;
		}
		#endif
		*/
		#endif
	}
	#endif
	
	return result;
}
//note verifyActivatedNeuronWithMaxSequenceIndexAllowed tests are not required without SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
bool SANIpropagateCompactGenerateOperationsClass::verifyMissingOrVariableStartComponentFoundAllowedWordIndices(const SANItranslatorVariablesClass* translatorVariables, const int layer, const SANIGroupNeuralNetwork* ownerGroup, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, const SANIComponentNeuralNetwork* previousActiveComponent, const SANIComponentNeuralNetwork* finalActiveComponent, const bool* existingActivationFoundStartComponent, const bool* missingStartComponentFound, const bool* variableStartComponentFound, const bool componentWordConnectivityTests, const bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup)
{
	bool result = true;
	
	//#ifndef SANI_SEQUENCE_GRAMMAR_ORIG
	//moved GIA3j5aTEMP36 from old SANIpropagateCompactGenerateOperationsClass::propagateWordThroughNetworkGroupComponent:if(activationSequenceCompleted), verifyActivatedNeuronWithMaxSequenceIndexCoverage checks
	//CHECKTHIS - the activation tree being fed into the missing or variable start component must encapsulate the entire sequenceElement index segment from the start of the global propagation required sequenceIndex (activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed):
	bool strictStartingCondition = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_VARIABLE
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFY2
	if(*missingStartComponentFound || missingOrVariableStartComponentFound)
	#else
	if(*missingStartComponentFound || (ownerGroup->currentParseTreeGroupTemp->missingOrVariableStartComponentFound))	
	#endif
	#else
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFIED
	if(*missingOrVariableStartComponentFound)
	#else
	if(*missingStartComponentFound)
	#endif
	#endif
	{
		strictStartingCondition = true;	
	}
	SANIGroupParseTree activatedNeuronCandidate = *(ownerGroup->currentParseTreeGroupTemp);	//activatedNeuronCandidate is only used for verifyActivatedNeuronWithMaxSequenceIndexAllowed purposes
	if(firstActiveComponentInGroup)
	{
		activatedNeuronCandidate.parseTreeMaxSequenceIndex = forwardPropogationSequenceElementData->sequenceIndex;
		activatedNeuronCandidate.parseTreeMinSequenceIndex = forwardPropogationSequenceElementData->sequenceIndex;
		//SANIpropagateOperationsParseTree.updateParseTreeMaxMinSequenceIndexOfParentBasic will update activatedNeuronCandidate based on activationPathSequenceElementCurrentParseTreeGroup only
	}
	SANIpropagateOperationsParseTree.updateParseTreeMaxMinSequenceIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationSequenceElementData, activationPathSequenceElementCurrentParseTreeGroup);
	if(!verifyActivatedNeuronWithMaxSequenceIndexAllowed(forwardPropogationSequenceData, forwardPropogationSignalData, &activatedNeuronCandidate, strictStartingCondition))
	{
		(*sequentialActivationFound) = false;
	}	
	//#endif
	
	return result;
}
#endif
#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_LAST_COMPONENTS
bool SANIpropagateCompactGenerateOperationsClass::propagateVariableEndComponentRemoveLastParseTreeComponent(const SANItranslatorVariablesClass* translatorVariables, const int layer, SANIGroupNeuralNetwork* ownerGroup, const SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, bool* activationSequenceCompleted, const SANIComponentNeuralNetwork* previousActiveComponent, const SANIComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFoundEndComponent, const bool componentWordConnectivityTests, bool* sequentialActivationFound, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup)
{
	bool result = true;

	if(componentWordConnectivityTests)
	{
		*sequentialActivationFound = true;	//redundant
	}
		
	if(*existingActivationFoundEndComponent)
	{
		//test verifyActivatedNeuronWithMaxSequenceIndexAllowed:
		//cout << "propagateVariableEndComponentRemoveLastParseTreeComponent" << endl;
		
		bool firstLastSequenceIndexTestHypotheticalWithVariableEndComponent = false; 
				
		SANIGroupParseTree activatedNeuronCandidate;	//activatedNeuronCandidate is only used for both verifyActivatedNeuronWithMaxSequenceIndexAllowed purposes and updating ownerGroup->currentParseTreeGroupTemp->parseTreeMinSequenceIndex/parseTreeMaxSequenceIndex in the case a variable first component is detected

		if(ownerGroup->currentParseTreeGroupTemp->components.size() == 0)
		{
			cerr << "SANIpropagateCompactGenerateOperationsClass::propagateVariableEndComponentRemoveLastParseTreeComponent error: (ownerGroup->currentParseTreeGroupTemp->components.size() == 0)" << endl;
			exit(EXIT_ERROR);
		}
		
		//this code is derived from SANIpropagateCompactGenerateOperationsClass::verifyMissingOrVariableStartComponentFound;
		#ifdef SANI_PARSE_SAVE_PARSE_TREE
		//get first component in parseTreeGroup:
		SANIComponentParseTree* firstComponentParseTree = SANInodes.getFirstComponent(forwardPropogationSequenceData, ownerGroup->currentParseTreeGroupTemp, true);
		if(!SANInodes.parseTreeComponentOnFirstHiddenLayer(firstComponentParseTree))
		{
			activatedNeuronCandidate.parseTreeMaxSequenceIndex = firstComponentParseTree->parseTreeGroupRef->parseTreeMaxSequenceIndex;
			activatedNeuronCandidate.parseTreeMinSequenceIndex = firstComponentParseTree->parseTreeGroupRef->parseTreeMinSequenceIndex;		
		}
		else
		{
			activatedNeuronCandidate.parseTreeMaxSequenceIndex = firstComponentParseTree->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
			activatedNeuronCandidate.parseTreeMinSequenceIndex = firstComponentParseTree->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;	
		}
		#else
		//SANIComponentNeuralNetwork* firstComponent = SANInodes.getFirstComponent(forwardPropogationSequenceData, ownerGroup, true);
		cerr << "SANIpropagateCompactGenerateOperationsClass::propagateVariableEndComponentRemoveLastParseTreeComponent currently requires SANI_PARSE_SAVE_PARSE_TREE" << endl;
		exit(EXIT_ERROR);
		#endif
		
		SANIpropagateOperationsParseTree.updateParseTreeMaxMinSequenceIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationSequenceElementData, activationPathSequenceElementCurrentParseTreeGroup);
		if(verifyActivatedNeuronWithMaxSequenceIndexAllowed(forwardPropogationSequenceData, forwardPropogationSignalData, &activatedNeuronCandidate, true))
		{
			firstLastSequenceIndexTestHypotheticalWithVariableEndComponent = true;
		}
		else
		{
			*existingActivationFoundEndComponent = false;
			*sequentialActivationFound = false;
		}

		if(firstLastSequenceIndexTestHypotheticalWithVariableEndComponent)
		{
			//this code is derived from SANIpropagateCompactGenerateOperationsClass::propagateWordThroughNetworkGroupComponent (first section) and could be moved there (but would require rederivation of activatedNeuronCandidate)
			if(*existingActivationFoundEndComponent)
			{
				*activationSequenceCompleted = true;	//added GIA3l8e

				//readjust ownerGroup->currentParseTreeGroupTemp including parseTreeMaxSequenceIndex/parseTreeMinSequenceIndex (remove last component) 

				#ifdef SANI_DEBUG_PROPAGATE
				cout << "******** reduceGroupParseTreeGroupRef, groupIndex = " << ownerGroup->groupIndex << endl;
				#endif
				SANIGroupParseTree* parseTreeGroupRecord = ownerGroup->currentParseTreeGroupTemp;	//record reference to ownerGroup->currentParseTreeGroupTemp before resetting

				//SANIpropagateOperations.resetGroupActivation(ownerGroup);	//not required as last component values will shortly be overwritten
				SANIpropagateOperationsParseTree.resetGroupParseTreeGroupRef(ownerGroup, false);	

				*(ownerGroup->currentParseTreeGroupTemp) = *(parseTreeGroupRecord);	//replicate	//CHECKTHIS
				//or ownerGroup->currentParseTreeGroupTemp = new SANIGroupParseTree(*(parseTreeGroupRecord));	//NO: not allowed because ownerGroup->currentParseTreeGroupTemp has already been added to ownerGroup->parseTreeGroupMemory?

				SANIGroupParseTree* currentParseTreeGroupTemp = ownerGroup->currentParseTreeGroupTemp;	//new ownerGroup->currentParseTreeGroupTemp
				currentParseTreeGroupTemp->components.clear();
				SANIComponentParseTree* newFirstParseComponent = new SANIComponentParseTree(*SANInodes.getFirstComponent(forwardPropogationSequenceData, parseTreeGroupRecord, true)); 	//replicate first parse tree component
				if(forwardPropogationSequenceData->parseSentenceReverse)
				{
					//currentParseTreeGroupTemp->components.push_front(newParseComponent);
					currentParseTreeGroupTemp->components.insert(currentParseTreeGroupTemp->components.begin(), newFirstParseComponent);
				}
				else
				{
					currentParseTreeGroupTemp->components.push_back(newFirstParseComponent);
				}

				#ifdef SANI_FREE_MEMORY
				(forwardPropogationSequenceData->parseTreeGroupListPropagate).push_back(ownerGroup->currentParseTreeGroupTemp);
				#endif	
				#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
				//ownerGroup->parseTreeGroupMemory.push_back(ownerGroup->currentParseTreeGroupTemp);	//not required (as memory is only currently used for groups with first component active)
				#endif

				ownerGroup->currentParseTreeGroupTemp->parseTreeMinSequenceIndex = activatedNeuronCandidate.parseTreeMinSequenceIndex;
				ownerGroup->currentParseTreeGroupTemp->parseTreeMaxSequenceIndex = activatedNeuronCandidate.parseTreeMaxSequenceIndex;
			}
		}
	}
	
	return result;
}
#endif	
	

bool SANIpropagateCompactGenerateOperationsClass::updateActivatedNeuronWithMaxSequenceIndexCoverageWrapper(SANIGroupNeuralNetwork* ownerGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool activationSequenceCompleted, const bool sequentialActivationFound, const bool missingStartComponentFound, bool missingOrVariableStartComponentFound, bool missingOrVariableEndComponentFound)
{
	bool result = true;
	
	if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)
	{
		if(!activationSequenceCompleted)
		{
			if(sequentialActivationFound)
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
				//rule #2;
				if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableEndComponent)
				{					
					if(missingOrVariableEndComponentFound)
					{						
						if(calculateNextIndexInSequenceProspective(forwardPropogationSequenceData, ownerGroup->currentParseTreeGroupTemp) < forwardPropogationSequenceData->sentenceContents->size()-1)
						{		
							//cout << "SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS: updateActivatedNeuronWithMaxSequenceIndexCoverage" << endl;
							bool candidateCoveragePartial = false;
							updateActivatedNeuronWithMaxSequenceIndexCoverage(ownerGroup, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, missingStartComponentFound, missingOrVariableStartComponentFound, missingOrVariableEndComponentFound, false);
						}
					}
				}
				#endif
				
				#ifndef SANI_SEQUENCE_GRAMMAR_IGNORE_PARTIAL_DETECTIONS
				bool candidateCoveragePartial = true;
				updateActivatedNeuronWithMaxSequenceIndexCoverage(ownerGroup, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData, forwardPropogationSequenceElementData, forwardPropogationSequenceData, missingStartComponentFound, false, false, candidateCoveragePartial);
				#endif
			}
		}
	}
	
	return result;
}
	
bool SANIpropagateCompactGenerateOperationsClass::updateActivatedNeuronWithMaxSequenceIndexCoverage(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool missingStartComponentFound, bool missingOrVariableStartComponentFound, bool missingOrVariableEndComponentFound, bool candidateCoveragePartial)
{
	bool result = true;
		
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFY2
	bool activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponentSet = missingOrVariableStartComponentFound;
	#else
	bool activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponentSet;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFY
	activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponentSet = currentParseTreeGroupTemp->missingOrVariableStartComponentFound;
	#else
	if(candidateCoveragePartial)
	{
		activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponentSet = currentParseTreeGroupTemp->missingOrVariableStartComponentFound;
	}
	else
	{
		activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponentSet = missingOrVariableStartComponentFound;
	}
	#endif
	#endif
	#endif
	
	bool testWordIndicesAllowed = true;
	bool testMinNumComponents = true;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	if(activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponentSet)
	{
		testWordIndicesAllowed = true;
		testMinNumComponents = false;
	}
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
	if(missingOrVariableEndComponentFound)
	{
		testWordIndicesAllowed = true;
		testMinNumComponents = false;
	}
	#endif
			
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISALLOW_IDENTICAL_INPUTS	
	if(ownerGroup != forwardPropogationSequenceData->highLevelNeuronPriorTemp)
	{
	#endif
		if(verifyActivatedNeuronWithMaxSequenceIndexCoverage(forwardPropogationSequenceData, forwardPropogationSignalData, forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage, currentParseTreeGroupTemp, testWordIndicesAllowed, testMinNumComponents, candidateCoveragePartial, missingOrVariableStartComponentFound, missingOrVariableEndComponentFound))
		{		
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_SEQUENCE_INDEX_COVERAGE
			cout << "verifyActivatedNeuronWithMaxSequenceIndexCoverage" << endl;
			#endif

			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_MISSING
			if(!missingStartComponentFound)
			{
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_VARIABLE
			if(!missingOrVariableStartComponentFound)
			{
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
			if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableEndComponent || !missingOrVariableEndComponentFound)
			{
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
			if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableStartComponent || !missingOrVariableStartComponentFound)
			{
			#endif
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_SEQUENCE_INDEX_COVERAGE
				cout << "(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableStartComponent || !missingOrVariableStartComponentFound)" << endl;
				#endif

				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE
				int variableComponentMaxDepth = 0;
				int variableComponentMaxLeafSize = 0;
				if(missingOrVariableEndComponentFound)
				{
					SANIComponentNeuralNetwork* lastComponent = SANInodes.getFirstComponent(forwardPropogationSequenceData, ownerGroup, false);
					if(lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
					{
						cerr << "SANIpropagateCompactGenerateOperationsClass::updateActivatedNeuronWithMaxSequenceIndexCoverage error: (lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)" << endl;
						exit(EXIT_ERROR);
					}
					#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
					SANInodes.getNeuralNetworkDepth(lastComponent, &variableComponentMaxDepth);
					#else
					SANIpropagateOperations.countNeuralNetworkMaxLeafSizeAndDepth(lastComponent, &variableComponentMaxLeafSize, &variableComponentMaxDepth);
					#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
					SANIpropagateOperations.countNeuralNetworkMaxLeafSizeAndDepthReset(lastComponent);
					#endif
					#endif
				}
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_SEQUENCE_INDEX_COVERAGE
				cout << "\n\t\tvariableComponentMaxDepth = " << variableComponentMaxDepth << endl;
				cout << "\t\tforwardPropogationSentenceData->variableFirstComponentMaxDepth = " << forwardPropogationSequenceData->variableFirstComponentMaxDepth << endl;
				cout << "\t\tmissingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;			
				//cout << "SANInodes.countParseTreeLeafSize(currentParseTreeGroupTemp) = " << SANInodes.countParseTreeLeafSize(currentParseTreeGroupTemp) << endl;
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE_MATCHING_DEPTH
				if(!missingOrVariableEndComponentFound || variableComponentMaxDepth == forwardPropogationSequenceData->variableFirstComponentMaxDepth)
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE_SIZEABLE_SUBTREE
				if(!missingOrVariableEndComponentFound || variableComponentMaxDepth >= SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE_SIZEABLE_SUBTREE_MIN_NEURONS)
				#endif
				{
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_LAST_COMP_REQUIRE_SIZEABLE_SUBTREE
				if(!missingOrVariableStartComponentFound || SANInodes.countParseTreeLeafSize(currentParseTreeGroupTemp) >= SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_LAST_COMP_REQUIRE_SIZEABLE_SUBTREE_MIN_NEURONS)
				{
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE
				int variableComponentMaxDepth = 0;
				int variableComponentMaxLeafSize = 0;
				if(missingOrVariableStartComponentFound)
				{
					SANIComponentNeuralNetwork* startComponent = SANInodes.getFirstComponent(forwardPropogationSequenceData, ownerGroup, true);
					/*
					if(startComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
					{
						cerr << "SANIpropagateCompactGenerateOperationsClass::updateActivatedNeuronWithMaxSequenceIndexCoverage error: (startComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)" << endl;
						//cout << "missingOrVariableEndComponentFound = " << missingOrVariableEndComponentFound << endl;
						exit(EXIT_ERROR);
					}
					*/
					#ifdef SANI_SEQUENCE_GRAMMAR_RECORD_DEPTH
					SANInodes.getNeuralNetworkDepth(startComponent, &variableComponentMaxDepth);
					#else
					SANIpropagateOperations.countNeuralNetworkMaxLeafSizeAndDepth(startComponent, &variableComponentMaxLeafSize, &variableComponentMaxDepth);
					#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
					SANIpropagateOperations.countNeuralNetworkMaxLeafSizeAndDepthReset(startComponent);
					#endif
					#endif
				}
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_SEQUENCE_INDEX_COVERAGE
				cout << "\n\t\tvariableComponentMaxDepth = " << variableComponentMaxDepth << endl;
				cout << "\t\tforwardPropogationSentenceData->variableFirstComponentMaxDepth = " << forwardPropogationSequenceData->variableFirstComponentMaxDepth << endl;
				cout << "\t\tmissingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;			
				//cout << "SANInodes.countParseTreeLeafSize(currentParseTreeGroupTemp) = " << SANInodes.countParseTreeLeafSize(currentParseTreeGroupTemp) << endl;
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE_MATCHING_DEPTH
				if(!missingOrVariableStartComponentFound || variableComponentMaxDepth == forwardPropogationSequenceData->variableFirstComponentMaxDepth)	//ORIGSPEC //intermediary: variableComponentMaxDepth < forwardPropogationSequenceData->variableFirstComponentMaxDepth
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE_SIZEABLE_SUBTREE
				if(!missingOrVariableStartComponentFound || variableComponentMaxDepth >= SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE_SIZEABLE_SUBTREE_MIN_NEURONS)
				#endif
				{
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_RANDOMISE
				double randomProb = SANIpropagateOperations.generateRandomNumber();
				if(randomProb > SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_RANDOMISE_PROB)
				{
				#endif
					#ifdef SANI_SEQUENCE_GRAMMAR_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC_STRICT_AFTER_SIMULTANEOUS_POS_PROPAGATION
					bool updatePerformance = false;
					bool verifyPerformance = true;
					bool sequenceElementConnectivityVerification = SANIpropagateOperationsParseTree.updateAndOrVerifyPerformanceGroup(currentParseTreeGroupTemp, forwardPropogationSequenceData, 0, updatePerformance, verifyPerformance, true, forwardPropogationSignalData);
					if(sequenceElementConnectivityVerification)
					{
					#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT
						bool foundPosUnambiguousContext = true;
						if(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageRequirePosAmbiguousContext)
						{
							foundPosUnambiguousContext = false;
							if(verifyPosUnambiguousContext(forwardPropogationSequenceElementData, forwardPropogationSequenceData, currentParseTreeGroupTemp))
							{
								foundPosUnambiguousContext = true;
							}
						}
						if(foundPosUnambiguousContext)
						{
						#endif	
							//cout << "\t2 candidateCoveragePartial = " << candidateCoveragePartial << endl;

							//cout << "missingStartComponentFound = " << missingStartComponentFound << endl;

							#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
							cout << "++++++++++++++++++++++++++++++++++++++++++ forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage" << endl;
							cout << "\tactivatedNeuronWithMaxSequenceIndexCoverageVariableStartComponentSet = " << activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponentSet << endl;		
							#endif
							//cout << "\tactivatedNeuronWithMaxSequenceIndexCoverageVariableStartComponentSet = " << activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponentSet << endl;		
							#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_SEQUENCE_INDEX_COVERAGE
							cout << "\tcandidateCoveragePartial = " << candidateCoveragePartial << endl;
							cout << "\tforwardPropogationWordData->w = " << forwardPropogationSequenceElementData->sequenceIndex << endl;
							#endif
							//cout << "candidateCoveragePartial = " << candidateCoveragePartial << endl;

							forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverage = currentParseTreeGroupTemp;
							forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoveragePartial = candidateCoveragePartial;			
							#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
							forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponent = activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponentSet;
							#endif
							#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
							forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableEndComponent = missingOrVariableEndComponentFound;
							#endif
							/*
							#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
							forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageMissingStartComponent = activatedNeuronWithMaxSequenceIndexCoverageMissingStartComponentSet;	//NOTUSED
							#endif
							*/
						#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT
						}
						#endif
					#ifdef SANI_SEQUENCE_GRAMMAR_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC_STRICT_AFTER_SIMULTANEOUS_POS_PROPAGATION
					}
					#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_RANDOMISE
				}
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE
				}
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_LAST_COMP_REQUIRE_SIZEABLE_SUBTREE
				}
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE
				}
				#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
			}
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
			}
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_VARIABLE
			}
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_ENFORCE_FIRST_COMPONENT_NOT_MISSING
			}
			#endif
		}
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISALLOW_IDENTICAL_INPUTS
	}
	else
	{
		//cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISALLOW_IDENTICAL_INPUTS: (ownerGroup == forwardPropogationSequenceData->highLevelNeuronPriorTemp)" << endl;
		//exit(EXIT_ERROR);
	}
	#endif
	
	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT
bool SANIpropagateCompactGenerateOperationsClass::verifyPosUnambiguousContext(const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIGroupParseTree* activatedNeuronCandidate)
{
	bool foundPosUnambiguousContext = true;
	
	int activatedNeuronCandidateMinSequenceIndex = activatedNeuronCandidate->parseTreeMinSequenceIndex;
	int activatedNeuronCandidateMaxSequenceIndex = activatedNeuronCandidate->parseTreeMaxSequenceIndex;
	int startOfSentenceSequenceIndex = 0;
	int endOfSentenceSequenceIndex = forwardPropogationSequenceData->sentenceContents->size()-1;
	int numberOfUnambiguousContextWordsStart = 0;
	int numberOfUnambiguousContextWordsEnd = 0;
	
	bool findingUnambiguousContextWords = true;	
	for(int w=activatedNeuronCandidateMinSequenceIndex; w<=activatedNeuronCandidateMaxSequenceIndex; w++)
	{
		LRPpreprocessorPlainTextWord* currentWord = (*(forwardPropogationSequenceData->sentenceContents))[w];
		
		if(findingUnambiguousContextWords)
		{
			if(!LRPpreprocessorPOStagger.isWordPOSambiguous(currentWord))
			{
				numberOfUnambiguousContextWordsStart++;
			}
			else
			{
				findingUnambiguousContextWords = false;
			}
		}
	}
	findingUnambiguousContextWords = true;
	for(int w=activatedNeuronCandidateMaxSequenceIndex; w>=activatedNeuronCandidateMinSequenceIndex; w--)
	{
		LRPpreprocessorPlainTextWord* currentWord = (*(forwardPropogationSequenceData->sentenceContents))[w];
		
		if(findingUnambiguousContextWords)
		{
			if(!LRPpreprocessorPOStagger.isWordPOSambiguous(currentWord))
			{
				numberOfUnambiguousContextWordsEnd++;
			}
			else
			{
				findingUnambiguousContextWords = false;
			}
		}
	}
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT_IGNORE_OUT_OF_SENTENCE_BOUNDS
	int minimumNumberOfUnambiguousContextWordsStartRequired = SHAREDvars.minInt(activatedNeuronCandidateMinSequenceIndex-startOfSentenceSequenceIndex, SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT_BOUNDARY);
	int minimumNumberOfUnambiguousContextWordsEndRequired = SHAREDvars.minInt(endOfSentenceSequenceIndex-activatedNeuronCandidateMaxSequenceIndex, SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT_BOUNDARY);
	#else
	int minimumNumberOfUnambiguousContextWordsStartRequired = SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT_BOUNDARY;
	int minimumNumberOfUnambiguousContextWordsEndRequired = SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_REQUIRE_POS_UNAMBIGUOUS_CONTEXT_BOUNDARY;
	#endif
	
	if(numberOfUnambiguousContextWordsStart < minimumNumberOfUnambiguousContextWordsStartRequired)
	{
		foundPosUnambiguousContext = false;
	}
	if(numberOfUnambiguousContextWordsEnd < minimumNumberOfUnambiguousContextWordsEndRequired)
	{
		foundPosUnambiguousContext = false;
	}
		
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_VERIFY
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_MARK_AS_UNAMBIGUOUS
	clearPOSambiguousInputsMarkAsUnambiguous(activatedNeuronCandidate);	//clear existing POSambiguousInputsMarkAsUnambiguous values
	#endif
	for(int w=activatedNeuronCandidateMinSequenceIndex; w<=activatedNeuronCandidateMaxSequenceIndex; w++)
	{
		LRPpreprocessorPlainTextWord* currentWord = (*(forwardPropogationSequenceData->sentenceContents))[w];
		if(!verifyThatComponentInputIsNotPosAmbiguous(activatedNeuronCandidate, currentWord))
		{
			foundPosUnambiguousContext = false;
		}
	}
	#endif
	
	if(foundPosUnambiguousContext)
	{
		cout << "SANIpropagateCompactGenerateOperationsClass::verifyPosUnambiguousContext: foundPosUnambiguousContext" << endl;
	}
	
	return foundPosUnambiguousContext;
}
#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_VERIFY
bool SANIpropagateCompactGenerateOperationsClass::verifyThatComponentInputIsNotPosAmbiguous(const SANIGroupParseTree* currentParseTreeGroup, const LRPpreprocessorPlainTextWord* currentWord)
{
	bool result = true;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		SANIComponentNeuralNetwork* currentComponent = parseTreeComponent->componentRef;
			
		if(SANInodes.parseTreeComponentOnFirstHiddenLayer(parseTreeComponent))
		{
			if(LRPpreprocessorPOStagger.isWordPOSambiguous(currentWord))
			{
				if(currentComponent->POSambiguousInputs)
				{
					#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_VERIFY_UNIQUE
					if(SANInodes.isComponentWordPOStypeInferredUnique(parseTreeComponent->wordPOStypeInferred, currentComponent->POSambiguousInputsPOSambiguityInfo, currentWord->POSambiguityInfo))
					{
						#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_MARK_AS_UNAMBIGUOUS
						#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_MARK_AS_UNAMBIGUOUS_VERIFY_UNIQUE
						currentComponent->POSambiguousInputsMarkAsUnambiguous = true;
						#endif
						#endif
					}
					else
					{
					#endif
						result = false;
					#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_VERIFY_UNIQUE
					}
					#endif
				}
				#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_VERIFY_POS_UNAMBIGUOUS_SUBNET
				else
				{
					//result = true
				}
				#endif
			}
			else
			{
				if(currentComponent->POSambiguousInputs)
				{
					#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_MARK_AS_UNAMBIGUOUS
					#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_MARK_AS_UNAMBIGUOUS_VERIFY_POS_UNAMBIGUOUS_WORDS
					currentComponent->POSambiguousInputsMarkAsUnambiguous = true;
					#endif
					#endif
				}
			}
		}
		else
		{
			if(!verifyThatComponentInputIsNotPosAmbiguous(parseTreeComponent->parseTreeGroupRef, currentWord))
			{
				result = false;
			}
		}
	}

	return result;
}
#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_POS_AMBIGUOUS_PERMUTATIONS_ALLOW_TO_BE_MATCHED_MARK_AS_UNAMBIGUOUS
bool SANIpropagateCompactGenerateOperationsClass::clearPOSambiguousInputsMarkAsUnambiguous(const SANIGroupParseTree* currentParseTreeGroup)
{
	bool result = true;
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		SANIComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		if(SANInodes.parseTreeComponentOnFirstHiddenLayer(parseTreeComponent))
		{
			SANIComponentNeuralNetwork* currentComponent = parseTreeComponent->componentRef;
			if(currentComponent->POSambiguousInputsMarkAsUnambiguous)
			{
				currentComponent->POSambiguousInputsMarkAsUnambiguous = false;
			}
		}
		else
		{
			if(!clearPOSambiguousInputsMarkAsUnambiguous(parseTreeComponent->parseTreeGroupRef))
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



bool SANIpropagateCompactGenerateOperationsClass::verifyActivatedNeuronWithMaxSequenceIndexCoverage(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIGroupParseTree* activatedNeuronBaseline, SANIGroupParseTree* activatedNeuronCandidate, const bool testWordIndicesAllowed, const bool testMinNumComponents, const bool candidateCoveragePartial, const bool missingOrVariableStartComponentFound, const bool missingOrVariableEndComponentFound)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SEQUENCEINDEX_VERIFICATION
	cout << "verifyActivatedNeuronWithMaxSequenceIndexCoverage START" << endl;
	#endif
	#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
	cout << "verifyActivatedNeuronWithMaxSequenceIndexCoverage START" << endl;
	#endif
					
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_ARE_NOT_USED_BY_ANY_NEW_CANDIDATE
	bool candidateNeuronInCompleteHistory = false;
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
	if(activatedNeuronCandidate->groupRef->marked)
	{
		candidateNeuronInCompleteHistory = true;
		//cout << "candidateNeuronInCompleteHistory" << endl;
	}
	#endif
	#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_HISTORY
	for(int i=0; i<forwardPropogationSequenceData->listOfHighLevelNeuronsCompleteHistory.size(); i++)
	{
		if(activatedNeuronCandidate->groupRef == forwardPropogationSequenceData->listOfHighLevelNeuronsCompleteHistory[i])
		{
			candidateNeuronInCompleteHistory = true;
		}
	}
	#endif
	if(!candidateNeuronInCompleteHistory)
	{
	#endif		
		#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
		cout << "!candidateNeuronInCompleteHistory" << endl;
		#endif

		//if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)	//assumed true
		//{
		#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
		if(verifyNeverSplitGroupBetweenTwoIdenticalComponents(forwardPropogationSequenceData, activatedNeuronCandidate, candidateCoveragePartial))
		{
		#endif			
			#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
			cout << "pass verifyNeverSplitGroupBetweenTwoIdenticalComponents" << endl;
			#endif
			
			int activatedNeuronCandidateCoverage = 0;
			int activatedNeuronCandidateMaxWeight = 0;
			#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			bool activatedNeuronCandidateMinNumComponentsTest = false;
			#endif
			if(activatedNeuronCandidate != NULL)
			{
				activatedNeuronCandidateCoverage = SANInodes.calculateCoverage(activatedNeuronCandidate);
				#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
				SANIpropagateOperationsParseTree.calculatePerformanceWeightOfParseTree(activatedNeuronCandidate, &activatedNeuronCandidateMaxWeight);
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
				//cout << "activatedNeuronCandidate->components.size() = " << activatedNeuronCandidate->components.size() << endl;
				if(activatedNeuronCandidate->components.size() >= SANI_SEQUENCE_GRAMMAR_MIN_NUM_COMPONENTS)
				{
					activatedNeuronCandidateMinNumComponentsTest = true;
				}
				#endif
			}
			int activatedNeuronBaselineCoverage = 0;
			int activatedNeuronBaselineMaxWeight = 0;
			if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)
			{
				if(activatedNeuronBaseline != NULL)
				{
					activatedNeuronBaselineCoverage = SANInodes.calculateCoverage(activatedNeuronBaseline);
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					//activatedNeuronBaselineMaxWeight = activatedNeuronBaselineCoverage->parseTreeMaxWeight;
					SANIpropagateOperationsParseTree.calculatePerformanceWeightOfParseTree(activatedNeuronBaseline, &activatedNeuronBaselineMaxWeight);
					#endif
				}
			}

			bool firstLastSequenceIndexTest = false;
			if(testWordIndicesAllowed)
			{	
				bool strictStartingCondition = true;
				firstLastSequenceIndexTest = verifyActivatedNeuronWithMaxSequenceIndexAllowed(forwardPropogationSequenceData, forwardPropogationSignalData, activatedNeuronCandidate, strictStartingCondition);
			}
			else
			{
				firstLastSequenceIndexTest = true;
			}
			
			#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			if(!testMinNumComponents || activatedNeuronCandidateMinNumComponentsTest)
			{
			#endif			
				if(firstLastSequenceIndexTest)
				{	
					bool passBaselineComparisonTests = true;
					if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverage)
					{						
						passBaselineComparisonTests = false;
		
						#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
						cout << "(firstSequenceIndexTest && lastSequenceIndexTest)" << endl;
						cout << "activatedNeuronCandidateCoverage = " << activatedNeuronCandidateCoverage << endl;
						cout << "activatedNeuronBaselineCoverage = " << activatedNeuronBaselineCoverage << endl;		
						#endif
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SEQUENCEINDEX_VERIFICATION
						cout << "(firstSequenceIndexTest && lastSequenceIndexTest)" << endl;
						cout << "activatedNeuronCandidateCoverage = " << activatedNeuronCandidateCoverage << endl;
						cout << "activatedNeuronBaselineCoverage = " << activatedNeuronBaselineCoverage << endl;
						#endif

						bool passBaselineCoverageTests = false;
						#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS_PREFERENCE_THEM
						if(missingOrVariableEndComponentFound)
						{	
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;
							}
						}	
						#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_PREFERENCE_THEM
						if(missingOrVariableStartComponentFound)
						{	
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;
							}
						}
						#endif
						if(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)
						{
							passBaselineCoverageTests = true;
						}
						#ifndef SANI_SEQUENCE_GRAMMAR_IGNORE_PARTIAL_DETECTIONS
						if(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoveragePartial && !(candidateCoveragePartial) && !(missingOrVariableStartComponentFound) && !(missingOrVariableEndComponentFound))
						{
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;	//allow fully active activatedNeuronCandidate to override partially active activatedNeuronBaseline, assuming they have the same coverage	//CHECKTHIS
							}
						}
						#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
						if(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableStartComponent && !(missingOrVariableStartComponentFound) && !(missingOrVariableEndComponentFound))	//CHECKTHIS: !(candidateCoveragePartial)
						{
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;	//allow activatedNeuronCandidate without missingOrVariableStartComponentFound to override activatedNeuronBaseline with missingOrVariableStartComponentFound, assuming they have the same coverage	//CHECKTHIS
							}
						}
						#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
						if(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoverageVariableEndComponent && !(missingOrVariableStartComponentFound) && !(missingOrVariableEndComponentFound))	//CHECKTHIS: !(candidateCoveragePartial)
						{
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;	//allow activatedNeuronCandidate without missingOrVariableEndComponentFound to override activatedNeuronBaseline with missingOrVariableEndComponentFound, assuming they have the same coverage	//CHECKTHIS
							}
						}
						#endif
						

						if(passBaselineCoverageTests)
						{
							#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
							cout << "(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)" << endl;
							#endif
							#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SEQUENCEINDEX_VERIFICATION
							cout << "(activatedNeuronCandidateCoverage > activatedNeuronBaselineCoverage)" << endl;
							#endif
							#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
							#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_FIX
							if((activatedNeuronCandidateCoverage != activatedNeuronBaselineCoverage) || (activatedNeuronCandidateMaxWeight >= activatedNeuronBaselineMaxWeight))
							#else
							if(activatedNeuronCandidateMaxWeight >= activatedNeuronBaselineMaxWeight)
							#endif
							{
								//SANIpropagateOperationsParseTree.setPerformanceWeightOfParseTree(activatedNeuronBaselineCoverage, maxWeight);
								//activatedNeuronBaselineCoverage->parseTreeMaxWeight = maxWeight;
							#endif
								passBaselineComparisonTests = true;
							#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
							}
							#endif
						}

					}
					if(passBaselineComparisonTests)
					{
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SEQUENCEINDEX_VERIFICATION
						cout << "verifyActivatedNeuronWithMaxSequenceIndexCoverage PASSED" << endl;
						#endif
						#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
						cout << "verifyActivatedNeuronWithMaxSequenceIndexCoverage PASSED" << endl;
						#endif
						result = true;
					}

				}
			#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			}
			#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
		}
		#endif
		//}
	#ifdef SANI_SEQUENCE_GRAMMAR_GENERATE_ENSURE_ALL_NEURONS_IN_SUBGROUP_ARE_NOT_USED_BY_ANY_NEW_CANDIDATE
	}
	#endif
		
	return result;
}
	
bool SANIpropagateCompactGenerateOperationsClass::verifyActivatedNeuronWithMaxSequenceIndexAllowed(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIGroupParseTree* activatedNeuronCandidate, const bool strictStartingCondition)
{
	bool result = false;

	int activatedNeuronCandidateMinSequenceIndex = activatedNeuronCandidate->parseTreeMinSequenceIndex;
	int activatedNeuronCandidateMaxSequenceIndex = activatedNeuronCandidate->parseTreeMaxSequenceIndex;

	bool firstSequenceIndexTest = false;
	bool lastSequenceIndexTest = false;

	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		if(strictStartingCondition)
		{
			if(activatedNeuronCandidateMaxSequenceIndex == forwardPropogationSignalData->activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed)
			{
				firstSequenceIndexTest = true;
			}
		}
		else
		{
			if(activatedNeuronCandidateMaxSequenceIndex <= forwardPropogationSignalData->activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed)
			{
				firstSequenceIndexTest = true;
			}		
		}
		if(activatedNeuronCandidateMinSequenceIndex >= forwardPropogationSignalData->activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed)
		{
			lastSequenceIndexTest = true;
		}
	}
	else
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_SEQUENCEINDEX_VERIFICATION
		cout << "\tactivatedNeuronCandidateMinSequenceIndex = " << activatedNeuronCandidateMinSequenceIndex << endl;
		cout << "\tactivatedNeuronCandidateMaxSequenceIndex = " << activatedNeuronCandidateMaxSequenceIndex << endl;
		cout << "\tforwardPropogationSignalData->activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed = " << forwardPropogationSignalData->activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed << endl;
		cout << "\tforwardPropogationSignalData->activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed = " << forwardPropogationSignalData->activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed << endl;
		#endif
		if(strictStartingCondition)
		{
			if(activatedNeuronCandidateMinSequenceIndex == forwardPropogationSignalData->activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed)
			{
				firstSequenceIndexTest = true;
			}
		}
		else
		{
			if(activatedNeuronCandidateMinSequenceIndex >= forwardPropogationSignalData->activatedNeuronWithMaxSequenceIndexCoverageFirstSequenceIndexAllowed)
			{
				firstSequenceIndexTest = true;
			}		
		}
		if(activatedNeuronCandidateMaxSequenceIndex <= forwardPropogationSignalData->activatedNeuronWithMaxSequenceIndexCoverageLastSequenceIndexAllowed)
		{
			lastSequenceIndexTest = true;
		}
	}

	if(firstSequenceIndexTest && lastSequenceIndexTest)
	{
		result = true;
	}	

	return result;
}
		
#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
bool SANIpropagateCompactGenerateOperationsClass::verifyNeverSplitGroupBetweenTwoIdenticalComponents(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* activatedNeuronWithMaxSequenceIndexCoverage, const bool candidateCoveragePartial)
{
	bool result = false;
	#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
	cout << "verifyNeverSplitGroupBetweenTwoIdenticalComponents START" << endl;
	#endif
	
	if(verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSequenceData, activatedNeuronWithMaxSequenceIndexCoverage))
	{
		bool pass = true;
		#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
		cout << "verifyActivatedNeuronsAtLeastOneBasic" << endl;
		#endif
		
		//condition1: dont select a matched neuron activatedNeuronWithMaxSequenceIndexCoverage if it stops half way between a repeated section of POS type x in the sequenceElement sequence
		if(neuronFirstOrLastActivatedComponentSequenceIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(forwardPropogationSequenceData, activatedNeuronWithMaxSequenceIndexCoverage))
		{			
			//prevent activatedNeuronWithMaxSequenceIndexCoverage from being selected by verifyActivatedNeuronWithMaxSequenceIndexCoverage
			pass = false;
			#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
			cout << "neuronFirstOrLastActivatedComponentSequenceIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence"<< endl;		
			#endif
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
			cout << "SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition1: activatedNeuronWithMaxSequenceIndexCoverage, activatedNeuronWithMaxSequenceIndexCoveragePartial = " << activatedNeuronWithMaxSequenceIndexCoverage->activatedNeuronWithMaxSequenceIndexCoveragePartial << endl;
			//exit(EXIT_ERROR);
			#endif
		}
			
		if(candidateCoveragePartial)
		{			
			//condition2: dont select a matched neuron activatedNeuronWithMaxSequenceIndexCoverage and split it if the final activated component groupRef (ie of the matched sequence equals) in activatedNeuronWithMaxSequenceIndexCoverage equals the next unactivated component groupRef in activatedNeuronWithMaxSequenceIndexCoverage
			int lastActivatedIndex = INT_DEFAULT_VALUE;
			int firstUnactivatedIndex = INT_DEFAULT_VALUE;
			bool partialActivationConfirmed = identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(forwardPropogationSequenceData, activatedNeuronWithMaxSequenceIndexCoverage, &lastActivatedIndex, &firstUnactivatedIndex);
			
			if(partialActivationConfirmed)	//OLD: partial activation test required in case where activatedNeuronWithMaxSequenceIndexCoverage is recorded by SANIpropagateCompactGenerateOperationsClass::propagateWordThroughNetworkGroupComponentWrapper but is subsequently fully activated
			{	
				if(lastActivatedIndex == firstUnactivatedIndex)
				{
					cout << "(lastActivatedIndex == firstUnactivatedIndex)" << endl;
					exit(EXIT_ERROR);
				}
				SANIComponentNeuralNetwork* lastActivatedComponent = (activatedNeuronWithMaxSequenceIndexCoverage->groupRef)->components[lastActivatedIndex];		//BAD: activatedNeuronWithMaxSequenceIndexCoverage->components[lastActivatedIndex]->componentRef;
				SANIComponentNeuralNetwork* firstNonActivatedComponent = (activatedNeuronWithMaxSequenceIndexCoverage->groupRef)->components[firstUnactivatedIndex];	//BAD: activatedNeuronWithMaxSequenceIndexCoverage->components[firstUnactivatedIndex]->componentRef;
				#ifdef SANI_SEQUENCE_GRAMMAR
				#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS_ADVANCED
				bool foundComponentSourceMatch = false;
				for(int i=0; i<lastActivatedComponent->SANIbackGroupConnectionList.size(); i++)
				{
					SANIGroupNeuralNetwork* lastActivatedComponentSource = lastActivatedComponent->SANIbackGroupConnectionList[i];
					for(int j=0; j<firstNonActivatedComponent->SANIbackGroupConnectionList.size(); j++)
					{
						SANIGroupNeuralNetwork* firstNonActivatedComponentSource = firstNonActivatedComponent->SANIbackGroupConnectionList[j];
						if(lastActivatedComponentSource == firstNonActivatedComponentSource)
						{
							foundComponentSourceMatch = true;
						}
					}					
				}
				if(foundComponentSourceMatch)
				#else
				//assume lastActivatedComponent/firstNonActivatedComponent->groupRef == null, so cannot be compared
				//assume !SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS (ie SANIbackGroupConnectionList contains single neurons) 
				SANIGroupNeuralNetwork* lastActivatedComponentSource = lastActivatedComponent->SANIbackGroupConnectionList[0];
				SANIGroupNeuralNetwork* firstNonActivatedComponentSource = firstNonActivatedComponent->SANIbackGroupConnectionList[0];
				int lastActivatedComponentSourceSize = lastActivatedComponent->SANIbackGroupConnectionList.size();
				int firstNonActivatedComponentSourceSize = firstNonActivatedComponent->SANIbackGroupConnectionList.size();
				#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS_SUPPORT_VARIABLE_FIRST_COMPONENTS
				if(!((lastActivatedComponentSourceSize == 1) && (firstNonActivatedComponentSourceSize == 1)) || (lastActivatedComponentSource == firstNonActivatedComponentSource))	//updated SANI1m6e, old @SANI1m2b: if(((lastActivatedComponentSourceSize == 1) && (firstNonActivatedComponentSourceSize == 1)) && (lastActivatedComponentSource == firstNonActivatedComponentSource))
				#else
				if((lastActivatedComponentSource == firstNonActivatedComponentSource))
				#endif
				#endif
				#else
				if(lastActivatedComponent->groupRef == firstNonActivatedComponent->groupRef)
				#endif
				{
					//cout << "(lastActivatedComponent->groupRef == firstNonActivatedComponent->groupRef)" << endl;
					
					//cout << "lastActivatedComponent->groupRef->groupIndex = " << lastActivatedComponent->groupRef->groupIndex;
					//cout << "firstNonActivatedComponent->groupRef->groupIndex = " << firstNonActivatedComponent->groupRef->groupIndex;
					
					//prevent activatedNeuronWithMaxSequenceIndexCoverage from being selected by verifyActivatedNeuronWithMaxSequenceIndexCoverage
					pass = false;	
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
					cout << "SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition2" << endl;
					#endif
				}
			}
		}
		
		if(pass)
		{
			result = true;
		}
	}
	
	return result;
}

bool SANIpropagateCompactGenerateOperationsClass::neuronFirstOrLastActivatedComponentSequenceIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIGroupParseTree* activatedNeuronWithMaxSequenceIndexCoverage)
{
	bool result = false;
	
	//condition1: dont select a matched neuron activatedNeuronWithMaxSequenceIndexCoverage if it stops half way between a repeated section of POS type x in the sequenceElement sequence
	int currentMaxSequenceIndexInSequence;
	int nextSequenceIndexInSequence;
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		currentMaxSequenceIndexInSequence = activatedNeuronWithMaxSequenceIndexCoverage->parseTreeMinSequenceIndex;
		nextSequenceIndexInSequence = activatedNeuronWithMaxSequenceIndexCoverage->parseTreeMinSequenceIndex - 1;
	}
	else
	{
		currentMaxSequenceIndexInSequence = activatedNeuronWithMaxSequenceIndexCoverage->parseTreeMaxSequenceIndex;
		nextSequenceIndexInSequence = activatedNeuronWithMaxSequenceIndexCoverage->parseTreeMaxSequenceIndex + 1;
	}
	int currentMinSequenceIndexInSequence;
	int previousSequenceIndexInSequence;
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		currentMinSequenceIndexInSequence = activatedNeuronWithMaxSequenceIndexCoverage->parseTreeMaxSequenceIndex;
		previousSequenceIndexInSequence = activatedNeuronWithMaxSequenceIndexCoverage->parseTreeMaxSequenceIndex + 1;
	}
	else
	{
		currentMinSequenceIndexInSequence = activatedNeuronWithMaxSequenceIndexCoverage->parseTreeMinSequenceIndex;
		previousSequenceIndexInSequence = activatedNeuronWithMaxSequenceIndexCoverage->parseTreeMinSequenceIndex - 1;
	}
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
	cout << "\n\n activatedNeuronWithMaxSequenceIndexCoverage->components.size() = " << activatedNeuronWithMaxSequenceIndexCoverage->components.size() << endl;	
	cout << "currentMaxSequenceIndexInSequence = " << currentMaxSequenceIndexInSequence << endl;
	cout << "nextSequenceIndexInSequence = " << nextSequenceIndexInSequence << endl;
	cout << "currentMinSequenceIndexInSequence = " << currentMinSequenceIndexInSequence << endl;
	cout << "previousSequenceIndexInSequence = " << previousSequenceIndexInSequence << endl;
	#endif
	
	if(((currentMaxSequenceIndexInSequence < forwardPropogationSequenceData->firstLayer->size()) && (currentMaxSequenceIndexInSequence >= 0)) && ((nextSequenceIndexInSequence < forwardPropogationSequenceData->firstLayer->size()) && (nextSequenceIndexInSequence >= 0)))
	{//boundary test
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		/*
		vector<SANIGroupNeuralNetwork*>* currentMaxSequenceIndexInSequenceFirstLayerNeurons = (*forwardPropogationSequenceData->firstLayer)[currentMaxSequenceIndexInSequence];
		vector<SANIGroupNeuralNetwork*>* nextSequenceIndexInSequenceFirstLayerNeurons = (*forwardPropogationSequenceData->firstLayer)[nextSequenceIndexInSequence];			
		for(int i=0; i<currentMaxSequenceIndexInSequenceFirstLayerNeurons.size(); i++)
		{
			SANIGroupNeuralNetwork* currentMinSequenceIndexInSequenceFirstLayerNeuron = (*currentMinSequenceIndexInSequenceFirstLayerNeurons)[i];
			SANIGroupNeuralNetwork* nextSequenceIndexInSequenceFirstLayerNeuron = (*nextSequenceIndexInSequenceFirstLayerNeurons)[i];			
		*/	
			cerr << "!SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS:SANI_SEQUENCE_GRAMMAR_VARIABLE_COMPONENTS:SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS error: neuronFirstOrLastActivatedComponentSequenceIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence has not yet been coded for SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
			exit(EXIT_ERROR);
		#else
			SANIGroupNeuralNetwork* currentMaxSequenceIndexInSequenceFirstLayerNeuron = (*forwardPropogationSequenceData->firstLayer)[currentMaxSequenceIndexInSequence];
			SANIGroupNeuralNetwork* nextSequenceIndexInSequenceFirstLayerNeuron = (*forwardPropogationSequenceData->firstLayer)[nextSequenceIndexInSequence];	
			if(currentMaxSequenceIndexInSequenceFirstLayerNeuron == nextSequenceIndexInSequenceFirstLayerNeuron)
			{
				result = true;
			}
		#endif
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		/*
		}
		*/
		#endif
	}
	if(((currentMinSequenceIndexInSequence < forwardPropogationSequenceData->firstLayer->size()) && (currentMinSequenceIndexInSequence >= 0)) && ((previousSequenceIndexInSequence < forwardPropogationSequenceData->firstLayer->size()) && (previousSequenceIndexInSequence >= 0)))
	{//boundary test
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		/*
		vector<SANIGroupNeuralNetwork*>* currentMinSequenceIndexInSequenceFirstLayerNeurons = (*forwardPropogationSequenceData->firstLayer)[currentMinSequenceIndexInSequence];
		vector<SANIGroupNeuralNetwork*>* previousSequenceIndexInSequenceFirstLayerNeurons = (*forwardPropogationSequenceData->firstLayer)[previousSequenceIndexInSequence];
		
		for(int i=0; i<currentMaxSequenceIndexInSequenceFirstLayerNeurons.size(); i++)
		{
			SANIGroupNeuralNetwork* currentMinSequenceIndexInSequenceFirstLayerNeuron = (*currentMinSequenceIndexInSequenceFirstLayerNeurons)[i];
			SANIGroupNeuralNetwork* previousSequenceIndexInSequenceFirstLayerNeuron = (*previousSequenceIndexInSequenceFirstLayerNeurons)[i];			
		*/	
			cerr << "!SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS:SANI_SEQUENCE_GRAMMAR_VARIABLE_COMPONENTS:SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS error: neuronFirstOrLastActivatedComponentSequenceIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence has not yet been coded for SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
			exit(EXIT_ERROR);		
		#else		
			SANIGroupNeuralNetwork* currentMinSequenceIndexInSequenceFirstLayerNeuron = (*forwardPropogationSequenceData->firstLayer)[currentMinSequenceIndexInSequence];
			SANIGroupNeuralNetwork* previousSequenceIndexInSequenceFirstLayerNeuron = (*forwardPropogationSequenceData->firstLayer)[previousSequenceIndexInSequence];	
			if(currentMinSequenceIndexInSequenceFirstLayerNeuron == previousSequenceIndexInSequenceFirstLayerNeuron)
			{
				result = true;
			}
		#endif
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		/*
		}
		*/
		#endif
	}
	
	return result;
}
#endif


bool SANIpropagateCompactGenerateOperationsClass::verifyActivatedNeuronsAtLeastOneBasic(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIGroupParseTree* activatedNeuronWithMaxSequenceIndexCoverage)
{
	bool result = false;
	
	if(activatedNeuronWithMaxSequenceIndexCoverage != NULL)
	{
		result = true;
	}
	
	return result;
}

bool SANIpropagateCompactGenerateOperationsClass::verifyActivatedNeuronsAtLeastOne(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIGroupParseTree* activatedNeuronWithMaxSequenceIndexCoverage, const SANIGroupNeuralNetwork* newNeuronSequenceGroup1)
{
	bool result = false;
	
	if(verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSequenceData, activatedNeuronWithMaxSequenceIndexCoverage))
	{
		bool pass = true;
		if(forwardPropogationSequenceData->activatedNeuronWithMaxSequenceIndexCoveragePartial)
		{			
			#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
			if(newNeuronSequenceGroup1 == activatedNeuronWithMaxSequenceIndexCoverage->groupRef) 
			{
				//intrasentenceMatch = true;
				pass = false;
			}
			#endif
		}
		if(pass)
		{
			result = true;
		}
	}
	
	return result;
}


#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
//CHECKTHIS;
//ordered (do not assume normal order): "first" and "last" definitions respect (forwardPropogationSequenceData->parseSentenceReverse)
//partialActivationConfirmed: tests whether there is at least 1 unactivated component succeeding the activated component (not for SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION case where missingStartComponentFound, but all remaining components are active)
bool SANIpropagateCompactGenerateOperationsClass::identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex)
{
	bool partialActivationConfirmed = false;
	
	int firstActivatedIndexUnordered;
	int lastActivatedIndexUnordered;
	if(identifyComponentIndexFirstAndLastActivatedIndexUnordered(forwardPropogationSequenceData, parseTreeGroupNeuron, &firstActivatedIndexUnordered, &lastActivatedIndexUnordered))
	{
		SANIGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;

		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			*lastActivatedIndex = firstActivatedIndexUnordered;
			*firstUnactivatedIndex = firstActivatedIndexUnordered-1;			
			if(*firstUnactivatedIndex >= 0)
			{
				partialActivationConfirmed = true;
			}
		}
		else
		{
			*lastActivatedIndex = lastActivatedIndexUnordered;
			*firstUnactivatedIndex = lastActivatedIndexUnordered+1;
			if(*firstUnactivatedIndex < groupOrig->components.size())
			{
				partialActivationConfirmed = true;
			}
		}
	}
	
	return partialActivationConfirmed;
}

//unordered (assume normal order): "first" and "last" definitions do not respect (forwardPropogationSequenceData->parseSentenceReverse)
bool SANIpropagateCompactGenerateOperationsClass::identifyComponentIndexFirstAndLastActivatedIndexUnordered(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* firstActivatedIndex, int* lastActivatedIndex)
{
	bool result = false;
	
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
		SANIComponentNeuralNetwork* firstActivatedComponent = parseTreeGroupNeuron->components[0]->componentRef;
		*firstActivatedIndex = firstActivatedComponent->componentIndex;
		*lastActivatedIndex = *firstActivatedIndex + numberOfActivatedComponents-1;
		
		result = true;
	}
	
	return result;
}
#else

//preconditions: note no components->size() boundary check required as identifyComponentIndexFirstUnactivatedIndex can only be executed on partiallyActivatedNeuronWithMaxSequenceIndexCoverage (never fullyActivatedNeuronWithMaxSequenceIndexCoverage)
//partialActivationConfirmed: tests whether there is at least 1 unactivated component succeeding the activated component (not for SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION case where missingStartComponentFound, but all remaining components are active)
bool SANIpropagateCompactGenerateOperationsClass::identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex)
{
	bool partialActivationConfirmed = false;
	
	int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
	SANIGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;
	if(parseTreeGroupNeuron->components.size() < groupOrig->components.size())
	{
		partialActivationConfirmed = true;
		
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			*lastActivatedIndex = groupOrig->components.size()-numberOfActivatedComponents;
			*firstUnactivatedIndex = groupOrig->components.size()-1-numberOfActivatedComponents;
		}
		else
		{
			*lastActivatedIndex = numberOfActivatedComponents-1;
			*firstUnactivatedIndex = numberOfActivatedComponents;
		}
	}
	
	return partialActivationConfirmed;
}

//unordered (assume normal order): "first" and "last" definitions do not respect (forwardPropogationSequenceData->parseSentenceReverse)
bool SANIpropagateCompactGenerateOperationsClass::identifyComponentIndexLastActivatedIndexUnordered(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex)
{
	bool result = false;
	if(parseTreeGroupNeuron->components.size() > 0)
	{
		int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
		SANIComponentNeuralNetwork* firstActivatedComponent = parseTreeGroupNeuron->components[0]->componentRef;
		int firstActivatedIndex = firstActivatedComponent->componentIndex;
		if(firstActivatedIndex == 0)
		{
			result = true;
			*lastActivatedIndex = *firstActivatedIndex + numberOfActivatedComponents-1;
		}
		else
		{
			cerr << "!SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION: SANIpropagateCompactClass::identifyComponentIndexLastActivatedIndexUnordered error:  (firstActivatedIndex != 0)" << endl;
			exit(EXIT_ERROR);
		}
	}
	return result;
}
#endif











#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
bool SANIpropagateCompactGenerateOperationsClass::identifyMissingOrVariableStartComponentFound(const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int i, const SANIComponentNeuralNetwork* currentComponent, const SANIComponentNeuralNetwork* previousActiveComponent, bool* missingOrVariableStartComponentFound)
{
	bool result = true;
	
	//note it doesn't matter if the start component is active or inactive. Even if it has been activated miscellaneously (ie without maintaining sequenceElement index connectivity), it can be reset.
	if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableStartComponent)
	{
		if(i == 0)
		{
			bool variableFirstComponentTypeRequirements = false;
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_MISSING
			if(!(currentComponent->neuronComponentConnectionActive))
			{
				variableFirstComponentTypeRequirements = true;
			}
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_VARIABLE
			if(currentComponent->neuronComponentConnectionActive)
			{
				variableFirstComponentTypeRequirements = true;
			}
			#endif
			if(variableFirstComponentTypeRequirements)
			{	
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS
				if(identifyMissingOrVariableStartComponentFound(forwardPropogationSequenceData, components, currentComponent))
				{
				#else
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING
				if(currentComponent->componentType != GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)	//added GIA3j5aTEMP65
				{
				#endif
				#endif
					//cout << "missingOrVariableStartComponentFound = true" << endl;
					*missingOrVariableStartComponentFound = true;
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS
				}
				#else
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_NON_STRING
				}
				#endif
				#endif
			}	
		}
		else
		{
			*missingOrVariableStartComponentFound = false;	//removed @GIA3j5aTEMP18
		}
	}
	
	return result;
}
#endif
#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_FIRST_COMPONENTS_SHARED
bool SANIpropagateCompactGenerateOperationsClass::identifyMissingOrVariableStartComponentFound(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIComponentNeuralNetwork* currentComponent)
{
	bool result = false;
	
	bool testFirstComponentNotString = false;
	bool testLastComponentNotString = false;
	bool testFirstComponentSubgroupsUnique = false;
	bool testLastComponentSubgroupsUnique = false;
	bool testNotDualLowerLevelConnections = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_FIRST_COMPONENTS_NON_STRING
	testFirstComponentNotString = true;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_FIRST_COMPONENTS
	testLastComponentNotString = true;
	#endif
	#endif
	
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_FIRST_COMPONENTS_ALL_NEURONS_IN_FIRST_COMPONENT_SUBGROUPS_ARE_UNIQUE
	testFirstComponentSubgroupsUnique = true;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_FIRST_COMPONENTS_ALL_NEURONS_IN_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE
	testLastComponentSubgroupsUnique = true;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_DISABLE_MULTIPLE_INPUTS_IF_HAS_IDENTICAL_COMPONENTS
	testNotDualLowerLevelConnections = true;
	#endif
	if(calculateVariableComponentPassCriteria(forwardPropogationSequenceData, currentComponent, components, testFirstComponentNotString, testLastComponentNotString, testFirstComponentSubgroupsUnique, testLastComponentSubgroupsUnique, testNotDualLowerLevelConnections))
	{
		//cout << "missingOrVariableStartComponentFound = true" << endl;
		result = true;
	}
	
	return result;
}	
#endif		

#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
bool SANIpropagateCompactGenerateOperationsClass::identifyMissingOrVariableEndComponentFound(const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, vector<SANIComponentNeuralNetwork*>* components, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int i, const SANIComponentNeuralNetwork* currentComponent, const SANIComponentNeuralNetwork* previousActiveComponent, bool* missingOrVariableEndComponentFound)
{
	bool result = true;
	
	if(forwardPropogationSequenceData->recordActivatedNeuronWithMaxSequenceIndexCoverageSupportVariableEndComponent)
	{	
		int secondLastComponentIndex;
		#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
		secondLastComponentIndex = 0;
		#else
		//ensure neuron minimum number of components >= 2
		if(components->size() < 2)
		{
			cerr << "SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS: identifyMissingOrVariableEndComponentFound error - (components->size() < 2)" << endl;
			exit(EXIT_ERROR); 
		}
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			secondLastComponentIndex = 1;
		}
		else
		{
			secondLastComponentIndex = components->size()-2;
		}
		#endif
		if(i == secondLastComponentIndex)
		{
			bool variableFirstComponentTypeRequirements = false;
			if(!(currentComponent->neuronComponentConnectionActive))	//consider removing this requirement to allow groups identified for last variable components creation to have their activations reset
			{
				variableFirstComponentTypeRequirements = true;
			}
			if(variableFirstComponentTypeRequirements)
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS
				if(identifyMissingOrVariableEndComponentFound(forwardPropogationSequenceData, components, currentComponent))
				{
				#endif
					*missingOrVariableEndComponentFound = true;
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_COMPONENTS
				}
				#endif
			}	
		}
		else
		{
			*missingOrVariableEndComponentFound = false;
		}
	}
	
	return result;
}
#endif
#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_LAST_COMPONENTS_SHARED
bool SANIpropagateCompactGenerateOperationsClass::identifyMissingOrVariableEndComponentFound(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIComponentNeuralNetwork* currentComponent)
{
	bool result = false;
	
	bool testFirstComponentNotString = false;
	bool testLastComponentNotString = false;
	bool testFirstComponentSubgroupsUnique = false;
	bool testLastComponentSubgroupsUnique = false;
	bool testNotDualLowerLevelConnections = false;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_LAST_COMPONENTS_NON_STRING
	testFirstComponentNotString = true;
	testLastComponentNotString = true;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_LAST_COMPONENTS_ALL_NEURONS_IN_FIRST_COMPONENT_SUBGROUPS_ARE_UNIQUE
	testFirstComponentSubgroupsUnique = true;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_LAST_COMPONENTS_ALL_NEURONS_IN_LAST_COMPONENT_SUBGROUPS_ARE_UNIQUE
	testLastComponentSubgroupsUnique = true;
	#endif
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_DISABLE_MULTIPLE_INPUTS_IF_HAS_IDENTICAL_COMPONENTS
	testNotDualLowerLevelConnections = true;
	#endif
	if(calculateVariableComponentPassCriteria(forwardPropogationSequenceData, currentComponent, components, testFirstComponentNotString, testLastComponentNotString, testFirstComponentSubgroupsUnique, testLastComponentSubgroupsUnique, testNotDualLowerLevelConnections))
	{
		//cout << "missingOrVariableEndComponentFound = true" << endl;
		result = true;
	}
	
	return result;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_SHARED
bool SANIpropagateCompactGenerateOperationsClass::calculateVariableComponentPassCriteria(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIComponentNeuralNetwork* component, const vector<SANIComponentNeuralNetwork*>* components, const bool testFirstComponentNotString, const bool testLastComponentNotString, const bool testFirstComponentSubgroupsUnique, const bool testLastComponentSubgroupsUnique, const bool testNotDualLowerLevelConnections)
{
	bool passCriteria = true;

	SANIComponentNeuralNetwork* secondComponent = NULL;
	
	//cout << "components->size() = " << components->size() << endl;
	
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		secondComponent = (*components)[0];	//(*components)[c-1];	//as i==0
	}
	else
	{
		secondComponent = (*components)[1];	//(*components)[c+1];	//as i==0
	}
			
	if(testFirstComponentNotString)
	{
		if(SANInodes.hasComponentTypeString(component))	//added GIA3j5aTEMP65
		{
			passCriteria = false;
			//cout << "fail1" << endl;
		}
	}
	if(testLastComponentNotString)
	{
		if(SANInodes.hasComponentTypeString(secondComponent))
		{
			passCriteria = false;
			//cout << "fail2" << endl;
		}
	}	
	if(testFirstComponentSubgroupsUnique)
	{
		for(int j=0; j<component->SANIbackGroupConnectionList.size(); j++)
		{
			SANIGroupNeuralNetwork* subGroup = (component->SANIbackGroupConnectionList)[j];
			#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
			if(groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient(subGroup))
			{	
				passCriteria = false;
				//cout << "fail3" << endl;
			}
			#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
			groupContainsNeuronWithinProspectiveAlternateSubgroupReset(subGroup);
			#endif
			#endif
		}
	}
	if(testLastComponentSubgroupsUnique)
	{	
		#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS
		if(secondComponent->SANIbackGroupConnectionList.size() > 1)
		{
			cerr << "testLastComponentSubgroupsUnique; SANIpropagateCompactGenerateOperationsClass::calculateVariableComponentPassCriteria error: (secondComponent->SANIbackGroupConnectionList.size() > 1)" << endl;
			exit(EXIT_ERROR);
		}
		#endif
		
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_LAST_COMPONENTS_SHARED
		for(int j=0; j<secondComponent->SANIbackGroupConnectionList.size(); j++)
		{
			SANIGroupNeuralNetwork* secondComponentSubGroup = (secondComponent->SANIbackGroupConnectionList)[j];
		#else
			SANIGroupNeuralNetwork* secondComponentSubGroup = (secondComponent->SANIbackGroupConnectionList)[0];	//assume second component only has 1 reference
		#endif
			#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
			if(groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient(secondComponentSubGroup))
			{	
				passCriteria = false;
				//cout << "fail4" << endl;
				//cout << "groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient" << endl;
			}
			#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
			groupContainsNeuronWithinProspectiveAlternateSubgroupReset(secondComponentSubGroup);
			#endif
			#endif
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_LAST_COMPONENTS_SHARED
		}
		#endif
	}
	
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_DISABLE_MULTIPLE_INPUTS_IF_HAS_IDENTICAL_COMPONENTS
	if(testNotDualLowerLevelConnections)
	{
		if(SANIpropagateCompactOperations.findValidDualLowerLevelConnection(forwardPropogationSequenceData, components, component, false))		
		{	
			passCriteria = false;
			//cout << "fail5" << endl;
		}
	}
	#endif
	
	return passCriteria;
}
#endif

#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING_EFFICIENT
bool SANIpropagateCompactGenerateOperationsClass::groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient(SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = false;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	if(!currentNeuron->counted)
	{
		currentNeuron->counted = true;
	#endif
		if(!SANInodes.isNeuronString(currentNeuron))	//CHECKTHIS
		{
			#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
			if(currentNeuron->verified)
			{
				cout << "SANIpropagateCompactGenerateOperationsClass::groupContainsNeuronWithinProspectiveAlternateSubgroup error: currentNeuron has already been parsed (circular loop detected)" << endl;
				exit(EXIT_ERROR);
			}
			currentNeuron->verified = true;
			#endif

			if(currentNeuron->marked)
			{
				//cout << "groupContainsNeuronWithinProspectiveAlternateSubgroup" << endl;
				result = true;	
			}	
			if(!result)
			{
				for(int i=0; i<currentNeuron->components.size(); i++)
				{
					SANIComponentNeuralNetwork* currentComponent = currentNeuron->components[i];
					for(int j=0; j<currentComponent->SANIbackGroupConnectionList.size(); j++)
					{
						SANIGroupNeuralNetwork* subGroup = (currentComponent->SANIbackGroupConnectionList)[j];
						if(groupContainsNeuronWithinProspectiveAlternateSubgroupEfficient(subGroup))
						{
							result = true;
						}
					}
				}
			}

			#ifdef SANI_SEQUENCE_GRAMMAR_VERIFY_NO_CIRCULAR
			currentNeuron->verified = false;
			#endif
		}
	#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
	}
	#endif
	
	return result;
}
#ifdef SANI_SEQUENCE_GRAMMAR_OPTIMISE_FOR_DIVERGENT_CONVERGENT_PATHWAYS
bool SANIpropagateCompactGenerateOperationsClass::groupContainsNeuronWithinProspectiveAlternateSubgroupReset(SANIGroupNeuralNetwork* currentNeuron)
{
	bool result = true;
	
	if(currentNeuron->counted)
	{
		currentNeuron->counted = false;
		if(!SANInodes.isNeuronString(currentNeuron))
		{
			for(int i=0; i<currentNeuron->components.size(); i++)
			{
				SANIComponentNeuralNetwork* currentComponent = currentNeuron->components[i];
				for(int j=0; j<currentComponent->SANIbackGroupConnectionList.size(); j++)
				{
					SANIGroupNeuralNetwork* subGroup = (currentComponent->SANIbackGroupConnectionList)[j];
					groupContainsNeuronWithinProspectiveAlternateSubgroupReset(subGroup);
				}
			}
		}
	}
	
	return result;
}
#endif
#endif



int SANIpropagateCompactGenerateOperationsClass::calculateNextIndexInSequenceProspective(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIGroupParseTree* activatedNeuronWithMaxSequenceIndexCoverageProspective)
{
	int indexInSequence = 0;
	
	if(forwardPropogationSequenceData->parseSentenceReverse)
	{
		indexInSequence = forwardPropogationSequenceData->sentenceContents->size()-1 - (activatedNeuronWithMaxSequenceIndexCoverageProspective->parseTreeMinSequenceIndex - 1);
	}
	else
	{
		indexInSequence = activatedNeuronWithMaxSequenceIndexCoverageProspective->parseTreeMaxSequenceIndex + 1;
	}
	
	return indexInSequence;
}


#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_VERIFY_THAT_VARIABLE_EDGE_COMPONENT_SOURCE_POS_IS_NOT_IDENTICAL
bool SANIpropagateCompactGenerateOperationsClass::variableEdgeComponentSourcePOSisIdenticalWrapper1(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* generatedNeuron, SANIGroupNeuralNetwork* candidateMatchGroup, bool identifyVariableFirstOrLastComponent)
{
	bool result = false;
	
	//find every existing variable edge (first/last) component pos
	
	SANIComponentNeuralNetwork* newVariableComponent = SANInodes.getFirstComponent(forwardPropogationSequenceData, generatedNeuron, identifyVariableFirstOrLastComponent);
	SANIGroupNeuralNetwork* neuronWithNewVariableComponent = (newVariableComponent->SANIbackGroupConnectionList)[0];	//generated neuron doesn't have variable components
	
	SANIComponentNeuralNetwork* edgeComponent = SANInodes.getFirstComponent(forwardPropogationSequenceData, candidateMatchGroup, identifyVariableFirstOrLastComponent);
	for(int l2=0; l2<edgeComponent->SANIbackGroupConnectionList.size(); l2++)
	{
		SANIGroupNeuralNetwork* edgeComponentSource = edgeComponent->SANIbackGroupConnectionList[l2];
		if(variableEdgeComponentSourcePOSisIdenticalWrapper2(forwardPropogationSequenceData, neuronWithNewVariableComponent, edgeComponentSource, identifyVariableFirstOrLastComponent))
		{
			result = true;
		}
	}
	
	return result;
}

bool SANIpropagateCompactGenerateOperationsClass::variableEdgeComponentSourcePOSisIdenticalWrapper2(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* neuronWithNewVariableComponent, SANIGroupNeuralNetwork* edgeNeuron, bool identifyVariableFirstOrLastComponent)
{
	bool result = false;
	
	//find every existing variable edge (first/last) component pos
	
	if(edgeNeuron->inputLayerNeuron)	//groupTypeIsString
	{
		if(variableEdgeComponentSourcePOSisIdentical(forwardPropogationSequenceData, edgeNeuron, neuronWithNewVariableComponent, identifyVariableFirstOrLastComponent))
		{
			result = true;
		}
	}
	
	if(edgeNeuron->components.size() > 0)
	{
		SANIComponentNeuralNetwork* edgeComponent = SANInodes.getFirstComponent(forwardPropogationSequenceData, edgeNeuron, !identifyVariableFirstOrLastComponent);

		for(int l2=0; l2<edgeComponent->SANIbackGroupConnectionList.size(); l2++)
		{
			SANIGroupNeuralNetwork* edgeComponentSource = edgeComponent->SANIbackGroupConnectionList[l2];
			if(variableEdgeComponentSourcePOSisIdenticalWrapper2(forwardPropogationSequenceData, neuronWithNewVariableComponent, edgeComponentSource, identifyVariableFirstOrLastComponent))
			{
				result = true;
			}
		}
	}
	
	return result;
}

bool SANIpropagateCompactGenerateOperationsClass::variableEdgeComponentSourcePOSisIdentical(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const SANIGroupNeuralNetwork* neuronToDetect, SANIGroupNeuralNetwork* edgeNeuron, bool identifyVariableFirstOrLastComponent)
{
	bool result = false;
		
	if(neuronToDetect == edgeNeuron)
	{
		result = true;	
	}
	
	if(edgeNeuron->components.size() > 0)
	{
		SANIComponentNeuralNetwork* edgeComponent = SANInodes.getFirstComponent(forwardPropogationSequenceData, edgeNeuron, !identifyVariableFirstOrLastComponent);

		for(int l2=0; l2<edgeComponent->SANIbackGroupConnectionList.size(); l2++)
		{
			SANIGroupNeuralNetwork* edgeComponentSource = edgeComponent->SANIbackGroupConnectionList[l2];
			if(variableEdgeComponentSourcePOSisIdentical(forwardPropogationSequenceData, neuronToDetect, edgeComponentSource, identifyVariableFirstOrLastComponent))
			{
				result = true;
			}
		}
	}
	
	return result;
}
#endif





		
#endif
