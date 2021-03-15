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
 * Project Version: 1p3b 15-March-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Compact Generate Operations - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#include "SANIpropagateCompactGenerateOperations.hpp"
#include "SHAREDvars.hpp"

#ifdef SANI_SEQUENCE_GRAMMAR

#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
bool SANIpropagateCompactGenerateOperationsClass::verifyMissingOrVariableStartComponentFound(const SANItranslatorVariablesClass* translatorVariables, const int layer, SANIGroupNeuralNetwork* ownerGroup, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, const SANIComponentNeuralNetwork* previousActiveComponent, const SANIComponentNeuralNetwork* finalActiveComponent, const bool* existingActivationFoundStartComponent, const bool* missingStartComponentFound, bool* variableStartComponentFound, const bool componentWordConnectivityTests, bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = true;
	
	//consider moving componentWordConnectivityTests test into SANIpropagateOperations.propagateWordThroughNetworkGroupVerifyComponentSequenceActivationReady setting of missingOrVariableStartComponentFound
	if(componentWordConnectivityTests)
	{
		*sequentialActivationFound = true;	//redundant
		#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_VARIABLE
		if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
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
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
	{
		if(*missingOrVariableStartComponentFound)
		{
			//guaranteed: (missingStartComponentFound || missingStartComponentFound) && testComponent->w != 0 [ie second+ component]

			//test verifyActivatedNeuronWithMaxWordIndexAllowed:
			bool firstLastWordIndexTestHypotheticalWithoutVariableStartComponent = false; 
			SANIGroupParseTree activatedNeuronCandidate;	//activatedNeuronCandidate is only used for both verifyActivatedNeuronWithMaxWordIndexAllowed purposes and updating ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex/parseTreeMaxWordIndex in the case a variable first component is detected
			activatedNeuronCandidate.parseTreeMaxWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
			activatedNeuronCandidate.parseTreeMinWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
			//SANIpropagateOperationsParseTree.updateParseTreeMaxMinWordIndexOfParentBasic will update activatedNeuronCandidate based on activationPathWordCurrentParseTreeGroup only (existing group activation parseTreeMaxWordIndex/parseTreeMinWordIndex will be disgarded as the first component is variable)			
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_VARIABLE
			if(firstActiveComponentInGroup || ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1)))	//ie if missing start component || variable start component	//ie no components have previously been activated (firstActiveComponentInGroup) || 1 component has previously been activated ((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))
			{
			#endif
				SANIpropagateOperationsParseTree.updateParseTreeMaxMinWordIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup);
				if(verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, &activatedNeuronCandidate, true))
				{
					firstLastWordIndexTestHypotheticalWithoutVariableStartComponent = true;
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
			if(firstLastWordIndexTestHypotheticalWithoutVariableStartComponent)
			{
				if(*missingOrVariableStartComponentFound)
				{
					if(!(*sequentialActivationFound))
					{
						if((previousActiveComponent != NULL) && (ownerGroup->currentParseTreeGroupTemp->components.size() == 1))	//only 1 component has been activated	//ie (!*missingStartComponentFound)
						{
							//variable first component detected
							//word indices do not align (ie !sequentialActivationFound), so assume first component was miscellaneously activated (mark it as variable) 

							*variableStartComponentFound = true;
							if(sequentialActivationConnectivityTests(translatorVariables, testComponent, ownerGroup, &activatedNeuronCandidate, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, activationSequenceCompleted, layer, activationPathWordCurrentParseTreeGroup, *existingActivationFoundStartComponent, variableStartComponentFound))	//CHECKTHIS
							{
								/*update sequentialActivationFound value->true under the assumption the first component is being modified with a new (variable) connection [therefore wordIndex alignment with second component is not required]. 
								This is only required for propagateWordThroughNetworkGroupComponent:updateActivatedNeuronWithMaxWordIndexCoverage to execute [ie the detection/designation of a fully activatedNeuronWithMaxWordIndexCoverage with variable first component]*/
								*sequentialActivationFound = true;	
								
								//ownerGroup->currentParseTreeGroupTemp->variableStartComponentFound = true;
								ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex = activatedNeuronCandidate.parseTreeMinWordIndex;
								ownerGroup->currentParseTreeGroupTemp->parseTreeMaxWordIndex = activatedNeuronCandidate.parseTreeMaxWordIndex;
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
//note verifyActivatedNeuronWithMaxWordIndexAllowed tests are not required without SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
bool SANIpropagateCompactGenerateOperationsClass::verifyMissingOrVariableStartComponentFoundAllowedWordIndices(const SANItranslatorVariablesClass* translatorVariables, const int layer, const SANIGroupNeuralNetwork* ownerGroup, const SANIComponentNeuralNetwork* testComponent, const vector<SANIComponentNeuralNetwork*>* components, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool* activationSequenceCompleted, const bool firstActiveComponentInGroup, const SANIComponentNeuralNetwork* previousActiveComponent, const SANIComponentNeuralNetwork* finalActiveComponent, const bool* existingActivationFoundStartComponent, const bool* missingStartComponentFound, const bool* variableStartComponentFound, const bool componentWordConnectivityTests, const bool* missingOrVariableStartComponentFound, const int numberOfInactiveComponentsRemaining, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = true;
	
	//#ifndef SANI_SEQUENCE_GRAMMAR_ORIG
	//moved GIA3j5aTEMP36 from old SANIpropagateCompactGenerateOperationsClass::propagateWordThroughNetworkGroupComponent:if(activationSequenceCompleted), verifyActivatedNeuronWithMaxWordIndexCoverage checks
	//CHECKTHIS - the activation tree being fed into the missing or variable start component must encapsulate the entire word index segment from the start of the global propagation required wordIndex (activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed):
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
	SANIGroupParseTree activatedNeuronCandidate = *(ownerGroup->currentParseTreeGroupTemp);	//activatedNeuronCandidate is only used for verifyActivatedNeuronWithMaxWordIndexAllowed purposes
	if(firstActiveComponentInGroup)
	{
		activatedNeuronCandidate.parseTreeMaxWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		activatedNeuronCandidate.parseTreeMinWordIndex = forwardPropogationWordData->wordReference->translatorSentenceWordIndex;
		//SANIpropagateOperationsParseTree.updateParseTreeMaxMinWordIndexOfParentBasic will update activatedNeuronCandidate based on activationPathWordCurrentParseTreeGroup only
	}
	SANIpropagateOperationsParseTree.updateParseTreeMaxMinWordIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup);
	if(!verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, &activatedNeuronCandidate, strictStartingCondition))
	{
		(*sequentialActivationFound) = false;
	}	
	//#endif
	
	return result;
}
#endif
#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_VARIABLE_LAST_COMPONENTS
bool SANIpropagateCompactGenerateOperationsClass::propagateVariableEndComponentRemoveLastParseTreeComponent(const SANItranslatorVariablesClass* translatorVariables, const int layer, SANIGroupNeuralNetwork* ownerGroup, const SANIComponentNeuralNetwork* testComponent, vector<SANIComponentNeuralNetwork*>* components, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, bool* activationSequenceCompleted, const SANIComponentNeuralNetwork* previousActiveComponent, const SANIComponentNeuralNetwork* finalActiveComponent, bool* existingActivationFoundEndComponent, const bool componentWordConnectivityTests, bool* sequentialActivationFound, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup)
{
	bool result = true;

	if(componentWordConnectivityTests)
	{
		*sequentialActivationFound = true;	//redundant
	}
		
	if(*existingActivationFoundEndComponent)
	{
		//test verifyActivatedNeuronWithMaxWordIndexAllowed:
		//cout << "propagateVariableEndComponentRemoveLastParseTreeComponent" << endl;
		
		bool firstLastWordIndexTestHypotheticalWithVariableEndComponent = false; 
				
		SANIGroupParseTree activatedNeuronCandidate;	//activatedNeuronCandidate is only used for both verifyActivatedNeuronWithMaxWordIndexAllowed purposes and updating ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex/parseTreeMaxWordIndex in the case a variable first component is detected

		if(ownerGroup->currentParseTreeGroupTemp->components.size() == 0)
		{
			cerr << "SANIpropagateCompactGenerateOperationsClass::propagateVariableEndComponentRemoveLastParseTreeComponent error: (ownerGroup->currentParseTreeGroupTemp->components.size() == 0)" << endl;
			exit(EXIT_ERROR);
		}
		
		//this code is derived from SANIpropagateCompactGenerateOperationsClass::verifyMissingOrVariableStartComponentFound;
		#ifdef SANI_PARSE_SAVE_PARSE_TREE
		//get first component in parseTreeGroup:
		SANIComponentParseTree* firstComponentParseTree = SANInodes.getFirstComponent(forwardPropogationSentenceData, ownerGroup->currentParseTreeGroupTemp, true);
		if(firstComponentParseTree->parseTreeGroupRef != NULL)
		{
			activatedNeuronCandidate.parseTreeMaxWordIndex = firstComponentParseTree->parseTreeGroupRef->parseTreeMaxWordIndex;
			activatedNeuronCandidate.parseTreeMinWordIndex = firstComponentParseTree->parseTreeGroupRef->parseTreeMinWordIndex;		
		}
		else
		{
			activatedNeuronCandidate.parseTreeMaxWordIndex = firstComponentParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;
			activatedNeuronCandidate.parseTreeMinWordIndex = firstComponentParseTree->neuronComponentConnectionActiveWordRecord->translatorSentenceWordIndex;	
		}
		#else
		//SANIComponentNeuralNetwork* firstComponent = SANInodes.getFirstComponent(forwardPropogationSentenceData, ownerGroup, true);
		cerr << "SANIpropagateCompactGenerateOperationsClass::propagateVariableEndComponentRemoveLastParseTreeComponent currently requires SANI_PARSE_SAVE_PARSE_TREE" << endl;
		exit(EXIT_ERROR);
		#endif
		
		SANIpropagateOperationsParseTree.updateParseTreeMaxMinWordIndexOfParentBasic(&activatedNeuronCandidate, forwardPropogationWordData, activationPathWordCurrentParseTreeGroup);
		if(verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, &activatedNeuronCandidate, true))
		{
			firstLastWordIndexTestHypotheticalWithVariableEndComponent = true;
		}
		else
		{
			*existingActivationFoundEndComponent = false;
			*sequentialActivationFound = false;
		}

		if(firstLastWordIndexTestHypotheticalWithVariableEndComponent)
		{
			//this code is derived from SANIpropagateCompactGenerateOperationsClass::propagateWordThroughNetworkGroupComponent (first section) and could be moved there (but would require rederivation of activatedNeuronCandidate)
			if(*existingActivationFoundEndComponent)
			{
				*activationSequenceCompleted = true;	//added GIA3l8e

				//readjust ownerGroup->currentParseTreeGroupTemp including parseTreeMaxWordIndex/parseTreeMinWordIndex (remove last component) 

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
				SANIComponentParseTree* newFirstParseComponent = new SANIComponentParseTree(*SANInodes.getFirstComponent(forwardPropogationSentenceData, parseTreeGroupRecord, true)); 	//replicate first parse tree component
				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					//currentParseTreeGroupTemp->components.push_front(newParseComponent);
					currentParseTreeGroupTemp->components.insert(currentParseTreeGroupTemp->components.begin(), newFirstParseComponent);
				}
				else
				{
					currentParseTreeGroupTemp->components.push_back(newFirstParseComponent);
				}

				#ifdef SANI_FREE_MEMORY
				(forwardPropogationSentenceData->parseTreeGroupListPropagate).push_back(ownerGroup->currentParseTreeGroupTemp);
				#endif	
				#ifdef SANI_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
				//ownerGroup->parseTreeGroupMemory.push_back(ownerGroup->currentParseTreeGroupTemp);	//not required (as memory is only currently used for groups with first component active)
				#endif

				ownerGroup->currentParseTreeGroupTemp->parseTreeMinWordIndex = activatedNeuronCandidate.parseTreeMinWordIndex;
				ownerGroup->currentParseTreeGroupTemp->parseTreeMaxWordIndex = activatedNeuronCandidate.parseTreeMaxWordIndex;
			}
		}
	}
	
	return result;
}
#endif	
	

bool SANIpropagateCompactGenerateOperationsClass::updateActivatedNeuronWithMaxWordIndexCoverageWrapper(SANIGroupNeuralNetwork* ownerGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, const bool sequentialActivationFound, const bool missingStartComponentFound, bool missingOrVariableStartComponentFound, bool missingOrVariableEndComponentFound)
{
	bool result = true;
	
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
	{
		if(!activationSequenceCompleted)
		{
			if(sequentialActivationFound)
			{
				#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
				//rule #2;
				if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent)
				{					
					if(missingOrVariableEndComponentFound)
					{						
						if(calculateNextIndexInSequenceProspective(forwardPropogationSentenceData, ownerGroup->currentParseTreeGroupTemp) < forwardPropogationSentenceData->sentenceContents->size()-1)
						{		
							//cout << "SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS: updateActivatedNeuronWithMaxWordIndexCoverage" << endl;
							bool candidateCoveragePartial = false;
							updateActivatedNeuronWithMaxWordIndexCoverage(ownerGroup, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, missingStartComponentFound, missingOrVariableStartComponentFound, missingOrVariableEndComponentFound, false);
						}
					}
				}
				#endif
				
				#ifndef SANI_SEQUENCE_GRAMMAR_IGNORE_PARTIAL_DETECTIONS
				bool candidateCoveragePartial = true;
				updateActivatedNeuronWithMaxWordIndexCoverage(ownerGroup, ownerGroup->currentParseTreeGroupTemp, forwardPropogationSignalData, forwardPropogationWordData, forwardPropogationSentenceData, missingStartComponentFound, false, false, candidateCoveragePartial);
				#endif
			}
		}
	}
	
	return result;
}
	
bool SANIpropagateCompactGenerateOperationsClass::updateActivatedNeuronWithMaxWordIndexCoverage(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* currentParseTreeGroupTemp, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool missingStartComponentFound, bool missingOrVariableStartComponentFound, bool missingOrVariableEndComponentFound, bool candidateCoveragePartial)
{
	bool result = true;
		
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFY2
	bool activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = missingOrVariableStartComponentFound;
	#else
	bool activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS_SIMPLIFY
	activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = currentParseTreeGroupTemp->missingOrVariableStartComponentFound;
	#else
	if(candidateCoveragePartial)
	{
		activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = currentParseTreeGroupTemp->missingOrVariableStartComponentFound;
	}
	else
	{
		activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = missingOrVariableStartComponentFound;
	}
	#endif
	#endif
	#endif
	
	bool testWordIndicesAllowed = true;
	bool testMinNumComponents = true;
	#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
	if(activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet)
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
	if(ownerGroup != forwardPropogationSentenceData->highLevelNeuronPriorTemp)
	{
	#endif
		if(verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSignalData, forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage, currentParseTreeGroupTemp, testWordIndicesAllowed, testMinNumComponents, candidateCoveragePartial, missingOrVariableStartComponentFound, missingOrVariableEndComponentFound))
		{		
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
			cout << "verifyActivatedNeuronWithMaxWordIndexCoverage" << endl;
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
			if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent || !missingOrVariableEndComponentFound)
			{
			#endif
			#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
			if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent || !missingOrVariableStartComponentFound)
			{
			#endif
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
				cout << "(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent || !missingOrVariableStartComponentFound)" << endl;
				#endif

				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE
				int variableComponentMaxDepth = 0;
				int variableComponentMaxLeafSize = 0;
				if(missingOrVariableEndComponentFound)
				{
					SANIComponentNeuralNetwork* lastComponent = SANInodes.getFirstComponent(forwardPropogationSentenceData, ownerGroup, false);
					if(lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
					{
						cerr << "SANIpropagateCompactGenerateOperationsClass::updateActivatedNeuronWithMaxWordIndexCoverage error: (lastComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)" << endl;
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
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
				cout << "\n\t\tvariableComponentMaxDepth = " << variableComponentMaxDepth << endl;
				cout << "\t\tforwardPropogationSentenceData->variableFirstComponentMaxDepth = " << forwardPropogationSentenceData->variableFirstComponentMaxDepth << endl;
				cout << "\t\tmissingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;			
				//cout << "SANInodes.countParseTreeLeafSize(currentParseTreeGroupTemp) = " << SANInodes.countParseTreeLeafSize(currentParseTreeGroupTemp) << endl;
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_LAST_COMPONENTS_LAST_COMP_REQUIRE_MATCHING_DEPTH
				if(!missingOrVariableEndComponentFound || variableComponentMaxDepth == forwardPropogationSentenceData->variableFirstComponentMaxDepth)
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
					SANIComponentNeuralNetwork* startComponent = SANInodes.getFirstComponent(forwardPropogationSentenceData, ownerGroup, true);
					/*
					if(startComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
					{
						cerr << "SANIpropagateCompactGenerateOperationsClass::updateActivatedNeuronWithMaxWordIndexCoverage error: (startComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)" << endl;
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
				#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
				cout << "\n\t\tvariableComponentMaxDepth = " << variableComponentMaxDepth << endl;
				cout << "\t\tforwardPropogationSentenceData->variableFirstComponentMaxDepth = " << forwardPropogationSentenceData->variableFirstComponentMaxDepth << endl;
				cout << "\t\tmissingOrVariableStartComponentFound = " << missingOrVariableStartComponentFound << endl;			
				//cout << "SANInodes.countParseTreeLeafSize(currentParseTreeGroupTemp) = " << SANInodes.countParseTreeLeafSize(currentParseTreeGroupTemp) << endl;
				#endif
				#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS_FIRST_COMP_REQUIRE_MATCHING_DEPTH
				if(!missingOrVariableStartComponentFound || variableComponentMaxDepth == forwardPropogationSentenceData->variableFirstComponentMaxDepth)	//ORIGSPEC //intermediary: variableComponentMaxDepth < forwardPropogationSentenceData->variableFirstComponentMaxDepth
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
					//cout << "\t2 candidateCoveragePartial = " << candidateCoveragePartial << endl;

					//cout << "missingStartComponentFound = " << missingStartComponentFound << endl;

					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR
					cout << "++++++++++++++++++++++++++++++++++++++++++ forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage" << endl;
					cout << "\tactivatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = " << activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet << endl;		
					#endif
					//cout << "\tactivatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet = " << activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet << endl;		
					#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_UPDATE_ACTIVATED_NEURON_WITH_MAX_WORD_INDEX_COVERAGE
					cout << "\tcandidateCoveragePartial = " << candidateCoveragePartial << endl;
					cout << "\tforwardPropogationWordData->w = " << forwardPropogationWordData->w << endl;
					#endif
					//cout << "candidateCoveragePartial = " << candidateCoveragePartial << endl;

					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverage = currentParseTreeGroupTemp;
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial = candidateCoveragePartial;			
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent = activatedNeuronWithMaxWordIndexCoverageVariableStartComponentSet;
					#endif
					#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableEndComponent = missingOrVariableEndComponentFound;
					#endif
					/*
					#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_GENERATE_VARIABLE_FIRST_COMPONENTS
					forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageMissingStartComponent = activatedNeuronWithMaxWordIndexCoverageMissingStartComponentSet;	//NOTUSED
					#endif
					*/
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
		//cout << "SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_DISALLOW_IDENTICAL_INPUTS: (ownerGroup == forwardPropogationSentenceData->highLevelNeuronPriorTemp)" << endl;
		//exit(EXIT_ERROR);
	}
	#endif
	
	return result;
}



bool SANIpropagateCompactGenerateOperationsClass::verifyActivatedNeuronWithMaxWordIndexCoverage(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIGroupParseTree* activatedNeuronBaseline, SANIGroupParseTree* activatedNeuronCandidate, const bool testWordIndicesAllowed, const bool testMinNumComponents, const bool candidateCoveragePartial, const bool missingOrVariableStartComponentFound, const bool missingOrVariableEndComponentFound)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
	cout << "verifyActivatedNeuronWithMaxWordIndexCoverage START" << endl;
	#endif
	#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
	cout << "verifyActivatedNeuronWithMaxWordIndexCoverage START" << endl;
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
	for(int i=0; i<forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory.size(); i++)
	{
		if(activatedNeuronCandidate->groupRef == forwardPropogationSentenceData->listOfHighLevelNeuronsCompleteHistory[i])
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

		//if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)	//assumed true
		//{
		#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
		if(verifyNeverSplitGroupBetweenTwoIdenticalComponents(forwardPropogationSentenceData, activatedNeuronCandidate, candidateCoveragePartial))
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
				SANIpropagateOperationsParseTree.calculatePerformanceWeightOfParseTree(activatedNeuronCandidate, forwardPropogationSentenceData, &activatedNeuronCandidateMaxWeight);
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
			if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
			{
				if(activatedNeuronBaseline != NULL)
				{
					activatedNeuronBaselineCoverage = SANInodes.calculateCoverage(activatedNeuronBaseline);
					#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
					//activatedNeuronBaselineMaxWeight = activatedNeuronBaselineCoverage->parseTreeMaxWeight;
					SANIpropagateOperationsParseTree.calculatePerformanceWeightOfParseTree(activatedNeuronBaseline, forwardPropogationSentenceData, &activatedNeuronBaselineMaxWeight);
					#endif
				}
			}

			bool firstLastWordIndexTest = false;
			if(testWordIndicesAllowed)
			{	
				bool strictStartingCondition = true;
				firstLastWordIndexTest = verifyActivatedNeuronWithMaxWordIndexAllowed(forwardPropogationSentenceData, forwardPropogationSignalData, activatedNeuronCandidate, strictStartingCondition);
			}
			else
			{
				firstLastWordIndexTest = true;
			}
			
			#ifdef SANI_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS
			if(!testMinNumComponents || activatedNeuronCandidateMinNumComponentsTest)
			{
			#endif			
				if(firstLastWordIndexTest)
				{	
					bool passBaselineComparisonTests = true;
					if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage)
					{						
						passBaselineComparisonTests = false;
		
						#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
						cout << "(firstWordIndexTest && lastWordIndexTest)" << endl;
						cout << "activatedNeuronCandidateCoverage = " << activatedNeuronCandidateCoverage << endl;
						cout << "activatedNeuronBaselineCoverage = " << activatedNeuronBaselineCoverage << endl;		
						#endif
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
						cout << "(firstWordIndexTest && lastWordIndexTest)" << endl;
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
						if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial && !(candidateCoveragePartial) && !(missingOrVariableStartComponentFound) && !(missingOrVariableEndComponentFound))
						{
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;	//allow fully active activatedNeuronCandidate to override partially active activatedNeuronBaseline, assuming they have the same coverage	//CHECKTHIS
							}
						}
						#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_FIRST_COMPONENTS
						if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableStartComponent && !(missingOrVariableStartComponentFound) && !(missingOrVariableEndComponentFound))	//CHECKTHIS: !(candidateCoveragePartial)
						{
							if(activatedNeuronCandidateCoverage == activatedNeuronBaselineCoverage)
							{
								passBaselineCoverageTests = true;	//allow activatedNeuronCandidate without missingOrVariableStartComponentFound to override activatedNeuronBaseline with missingOrVariableStartComponentFound, assuming they have the same coverage	//CHECKTHIS
							}
						}
						#endif
						#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
						if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoverageVariableEndComponent && !(missingOrVariableStartComponentFound) && !(missingOrVariableEndComponentFound))	//CHECKTHIS: !(candidateCoveragePartial)
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
							#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
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
						#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
						cout << "verifyActivatedNeuronWithMaxWordIndexCoverage PASSED" << endl;
						#endif
						#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
						cout << "verifyActivatedNeuronWithMaxWordIndexCoverage PASSED" << endl;
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
	
bool SANIpropagateCompactGenerateOperationsClass::verifyActivatedNeuronWithMaxWordIndexAllowed(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIGroupParseTree* activatedNeuronCandidate, const bool strictStartingCondition)
{
	bool result = false;

	int activatedNeuronCandidateMinWordIndex = activatedNeuronCandidate->parseTreeMinWordIndex;
	int activatedNeuronCandidateMaxWordIndex = activatedNeuronCandidate->parseTreeMaxWordIndex;

	bool firstWordIndexTest = false;
	bool lastWordIndexTest = false;

	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		if(strictStartingCondition)
		{
			if(activatedNeuronCandidateMaxWordIndex == forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
			}
		}
		else
		{
			if(activatedNeuronCandidateMaxWordIndex <= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
			}		
		}
		if(activatedNeuronCandidateMinWordIndex >= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
		{
			lastWordIndexTest = true;
		}
	}
	else
	{
		#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDINDEX_VERIFICATION
		cout << "\tactivatedNeuronCandidateMinWordIndex = " << activatedNeuronCandidateMinWordIndex << endl;
		cout << "\tactivatedNeuronCandidateMaxWordIndex = " << activatedNeuronCandidateMaxWordIndex << endl;
		cout << "\tforwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = " << forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed << endl;
		cout << "\tforwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = " << forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed << endl;
		#endif
		if(strictStartingCondition)
		{
			if(activatedNeuronCandidateMinWordIndex == forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
			}
		}
		else
		{
			if(activatedNeuronCandidateMinWordIndex >= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed)
			{
				firstWordIndexTest = true;
			}		
		}
		if(activatedNeuronCandidateMaxWordIndex <= forwardPropogationSignalData->activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed)
		{
			lastWordIndexTest = true;
		}
	}

	if(firstWordIndexTest && lastWordIndexTest)
	{
		result = true;
	}	

	return result;
}
		
#ifdef SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
bool SANIpropagateCompactGenerateOperationsClass::verifyNeverSplitGroupBetweenTwoIdenticalComponents(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, const bool candidateCoveragePartial)
{
	bool result = false;
	#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
	cout << "verifyNeverSplitGroupBetweenTwoIdenticalComponents START" << endl;
	#endif
	
	if(verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
	{
		bool pass = true;
		#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
		cout << "verifyActivatedNeuronsAtLeastOneBasic" << endl;
		#endif
		
		//condition1: dont select a matched neuron activatedNeuronWithMaxWordIndexCoverage if it stops half way between a repeated section of POS type x in the word sequence
		if(neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
		{			
			//prevent activatedNeuronWithMaxWordIndexCoverage from being selected by verifyActivatedNeuronWithMaxWordIndexCoverage
			pass = false;
			#ifdef SANI_DEBUG_LIMIT_NUM_COMPONENTS
			cout << "neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence"<< endl;		
			#endif
			#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
			cout << "SANI_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS: condition1: activatedNeuronWithMaxWordIndexCoverage, activatedNeuronWithMaxWordIndexCoveragePartial = " << activatedNeuronWithMaxWordIndexCoverage->activatedNeuronWithMaxWordIndexCoveragePartial << endl;
			//exit(EXIT_ERROR);
			#endif
		}
			
		if(candidateCoveragePartial)
		{			
			//condition2: dont select a matched neuron activatedNeuronWithMaxWordIndexCoverage and split it if the final activated component groupRef (ie of the matched sequence equals) in activatedNeuronWithMaxWordIndexCoverage equals the next unactivated component groupRef in activatedNeuronWithMaxWordIndexCoverage
			int lastActivatedIndex = INT_DEFAULT_VALUE;
			int firstUnactivatedIndex = INT_DEFAULT_VALUE;
			bool partialActivationConfirmed = identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage, &lastActivatedIndex, &firstUnactivatedIndex);
			
			if(partialActivationConfirmed)	//OLD: partial activation test required in case where activatedNeuronWithMaxWordIndexCoverage is recorded by SANIpropagateCompactGenerateOperationsClass::propagateWordThroughNetworkGroupComponentWrapper but is subsequently fully activated
			{	
				if(lastActivatedIndex == firstUnactivatedIndex)
				{
					cout << "(lastActivatedIndex == firstUnactivatedIndex)" << endl;
					exit(EXIT_ERROR);
				}
				SANIComponentNeuralNetwork* lastActivatedComponent = (activatedNeuronWithMaxWordIndexCoverage->groupRef)->components[lastActivatedIndex];		//BAD: activatedNeuronWithMaxWordIndexCoverage->components[lastActivatedIndex]->componentRef;
				SANIComponentNeuralNetwork* firstNonActivatedComponent = (activatedNeuronWithMaxWordIndexCoverage->groupRef)->components[firstUnactivatedIndex];	//BAD: activatedNeuronWithMaxWordIndexCoverage->components[firstUnactivatedIndex]->componentRef;
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
					
					//prevent activatedNeuronWithMaxWordIndexCoverage from being selected by verifyActivatedNeuronWithMaxWordIndexCoverage
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

bool SANIpropagateCompactGenerateOperationsClass::neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
{
	bool result = false;
	
	//condition1: dont select a matched neuron activatedNeuronWithMaxWordIndexCoverage if it stops half way between a repeated section of POS type x in the word sequence
	int currentMaxWordIndexInSequence;
	int nextWordIndexInSequence;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		currentMaxWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex;
		nextWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex - 1;
	}
	else
	{
		currentMaxWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex;
		nextWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex + 1;
	}
	int currentMinWordIndexInSequence;
	int previousWordIndexInSequence;
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		currentMinWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex;
		previousWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex + 1;
	}
	else
	{
		currentMinWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex;
		previousWordIndexInSequence = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex - 1;
	}
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_NEVER_SPLIT_GROUP_BETWEEN_TWO_IDENTICAL_COMPONENTS
	cout << "\n\n activatedNeuronWithMaxWordIndexCoverage->components.size() = " << activatedNeuronWithMaxWordIndexCoverage->components.size() << endl;	
	cout << "currentMaxWordIndexInSequence = " << currentMaxWordIndexInSequence << endl;
	cout << "nextWordIndexInSequence = " << nextWordIndexInSequence << endl;
	cout << "currentMinWordIndexInSequence = " << currentMinWordIndexInSequence << endl;
	cout << "previousWordIndexInSequence = " << previousWordIndexInSequence << endl;
	#endif
	
	if(((currentMaxWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (currentMaxWordIndexInSequence >= 0)) && ((nextWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (nextWordIndexInSequence >= 0)))
	{//boundary test
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		vector<SANIGroupNeuralNetwork*>* currentMaxWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[currentMaxWordIndexInSequence];
		vector<SANIGroupNeuralNetwork*>* nextWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[nextWordIndexInSequence];			
		for(int i=0; i<currentMaxWordIndexInSequenceFirstLayerNeurons.size(); i++)
		{
			SANIGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*currentMinWordIndexInSequenceFirstLayerNeurons)[i];
			SANIGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*previousWordIndexInSequenceFirstLayerNeurons)[i];		
			cerr << "neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence hasnt yet been coded for SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
			exit(EXIT_ERROR);
		#else
			SANIGroupNeuralNetwork* currentMaxWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[currentMaxWordIndexInSequence];
			SANIGroupNeuralNetwork* nextWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[nextWordIndexInSequence];	
		#endif
			if(currentMaxWordIndexInSequenceFirstLayerNeuron == nextWordIndexInSequenceFirstLayerNeuron)
			{
				result = true;
			}
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		}
		#endif
	}
	if(((currentMinWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (currentMinWordIndexInSequence >= 0)) && ((previousWordIndexInSequence < forwardPropogationSentenceData->firstLayer->size()) && (previousWordIndexInSequence >= 0)))
	{//boundary test
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		vector<SANIGroupNeuralNetwork*>* currentMinWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[currentMinWordIndexInSequence];
		vector<SANIGroupNeuralNetwork*>* previousWordIndexInSequenceFirstLayerNeurons = (*forwardPropogationSentenceData->firstLayer)[previousWordIndexInSequence];
		for(int i=0; i<currentMaxWordIndexInSequenceFirstLayerNeurons.size(); i++)
		{
			SANIGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*currentMinWordIndexInSequenceFirstLayerNeurons)[i];
			SANIGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*previousWordIndexInSequenceFirstLayerNeurons)[i];			
			cerr << "neuronFirstOrLastActivatedComponentWordIndexEncapsulatesPartOfRepeatedPOSsectionOfSentenceWordSequence hasnt yet been coded for SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY" << endl;
			exit(EXIT_ERROR);		
		#else		
		SANIGroupNeuralNetwork* currentMinWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[currentMinWordIndexInSequence];
		SANIGroupNeuralNetwork* previousWordIndexInSequenceFirstLayerNeuron = (*forwardPropogationSentenceData->firstLayer)[previousWordIndexInSequence];	
		#endif
			if(currentMinWordIndexInSequenceFirstLayerNeuron == previousWordIndexInSequenceFirstLayerNeuron)
			{
				result = true;
			}
		#ifdef SANI_PROPAGATE_ALL_POS_VALUES_SIMULTANEOUSLY
		}
		#endif
	}
	
	return result;
}
#endif


bool SANIpropagateCompactGenerateOperationsClass::verifyActivatedNeuronsAtLeastOneBasic(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
{
	bool result = false;
	
	if(activatedNeuronWithMaxWordIndexCoverage != NULL)
	{
		result = true;
	}
	
	return result;
}

bool SANIpropagateCompactGenerateOperationsClass::verifyActivatedNeuronsAtLeastOne(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverage, const SANIGroupNeuralNetwork* newNeuronSequenceGroup1)
{
	bool result = false;
	
	if(verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage))
	{
		bool pass = true;
		if(forwardPropogationSentenceData->activatedNeuronWithMaxWordIndexCoveragePartial)
		{			
			#ifdef SANI_SEQUENCE_PREVENT_INTRASENTENCE_MATCHING
			if(newNeuronSequenceGroup1 == activatedNeuronWithMaxWordIndexCoverage->groupRef) 
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
//ordered (do not assume normal order): "first" and "last" definitions respect (forwardPropogationSentenceData->parseSentenceReverse)
//partialActivationConfirmed: tests whether there is at least 1 unactivated component succeeding the activated component (not for SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION case where missingStartComponentFound, but all remaining components are active)
bool SANIpropagateCompactGenerateOperationsClass::identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex)
{
	bool partialActivationConfirmed = false;
	
	int firstActivatedIndexUnordered;
	int lastActivatedIndexUnordered;
	if(identifyComponentIndexFirstAndLastActivatedIndexUnordered(forwardPropogationSentenceData, parseTreeGroupNeuron, &firstActivatedIndexUnordered, &lastActivatedIndexUnordered))
	{
		SANIGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;

		if(forwardPropogationSentenceData->parseSentenceReverse)
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

//unordered (assume normal order): "first" and "last" definitions do not respect (forwardPropogationSentenceData->parseSentenceReverse)
bool SANIpropagateCompactGenerateOperationsClass::identifyComponentIndexFirstAndLastActivatedIndexUnordered(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* firstActivatedIndex, int* lastActivatedIndex)
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

//preconditions: note no components->size() boundary check required as identifyComponentIndexFirstUnactivatedIndex can only be executed on partiallyActivatedNeuronWithMaxWordIndexCoverage (never fullyActivatedNeuronWithMaxWordIndexCoverage)
//partialActivationConfirmed: tests whether there is at least 1 unactivated component succeeding the activated component (not for SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION case where missingStartComponentFound, but all remaining components are active)
bool SANIpropagateCompactGenerateOperationsClass::identifyComponentIndexLastActivatedAndFirstUnactivatedIndex(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex, int* firstUnactivatedIndex)
{
	bool partialActivationConfirmed = false;
	
	int numberOfActivatedComponents = parseTreeGroupNeuron->components.size();
	SANIGroupNeuralNetwork* groupOrig = parseTreeGroupNeuron->groupRef;
	if(parseTreeGroupNeuron->components.size() < groupOrig->components.size())
	{
		partialActivationConfirmed = true;
		
		if(forwardPropogationSentenceData->parseSentenceReverse)
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

//unordered (assume normal order): "first" and "last" definitions do not respect (forwardPropogationSentenceData->parseSentenceReverse)
bool SANIpropagateCompactGenerateOperationsClass::identifyComponentIndexLastActivatedIndexUnordered(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* parseTreeGroupNeuron, int* lastActivatedIndex)
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
bool SANIpropagateCompactGenerateOperationsClass::identifyMissingOrVariableStartComponentFound(const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, const SANIComponentNeuralNetwork* currentComponent, const SANIComponentNeuralNetwork* previousActiveComponent, bool* missingOrVariableStartComponentFound)
{
	bool result = true;
	
	//note it doesn't matter if the start component is active or inactive. Even if it has been activated miscellaneously (ie without maintaining word index connectivity), it can be reset.
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableStartComponent)
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
				if(identifyMissingOrVariableStartComponentFound(forwardPropogationSentenceData, components, currentComponent))
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
bool SANIpropagateCompactGenerateOperationsClass::identifyMissingOrVariableStartComponentFound(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIComponentNeuralNetwork* currentComponent)
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
	if(calculateVariableComponentPassCriteria(forwardPropogationSentenceData, currentComponent, components, testFirstComponentNotString, testLastComponentNotString, testFirstComponentSubgroupsUnique, testLastComponentSubgroupsUnique, testNotDualLowerLevelConnections))
	{
		//cout << "missingOrVariableStartComponentFound = true" << endl;
		result = true;
	}
	
	return result;
}	
#endif		

#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_GENERATE_VARIABLE_LAST_COMPONENTS
bool SANIpropagateCompactGenerateOperationsClass::identifyMissingOrVariableEndComponentFound(const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, vector<SANIComponentNeuralNetwork*>* components, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int i, const SANIComponentNeuralNetwork* currentComponent, const SANIComponentNeuralNetwork* previousActiveComponent, bool* missingOrVariableEndComponentFound)
{
	bool result = true;
	
	if(forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverageSupportVariableEndComponent)
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
		if(forwardPropogationSentenceData->parseSentenceReverse)
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
				if(identifyMissingOrVariableEndComponentFound(forwardPropogationSentenceData, components, currentComponent))
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
bool SANIpropagateCompactGenerateOperationsClass::identifyMissingOrVariableEndComponentFound(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const vector<SANIComponentNeuralNetwork*>* components, const SANIComponentNeuralNetwork* currentComponent)
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
	if(calculateVariableComponentPassCriteria(forwardPropogationSentenceData, currentComponent, components, testFirstComponentNotString, testLastComponentNotString, testFirstComponentSubgroupsUnique, testLastComponentSubgroupsUnique, testNotDualLowerLevelConnections))
	{
		//cout << "missingOrVariableEndComponentFound = true" << endl;
		result = true;
	}
	
	return result;
}
#endif

#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_SHARED
bool SANIpropagateCompactGenerateOperationsClass::calculateVariableComponentPassCriteria(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIComponentNeuralNetwork* component, const vector<SANIComponentNeuralNetwork*>* components, const bool testFirstComponentNotString, const bool testLastComponentNotString, const bool testFirstComponentSubgroupsUnique, const bool testLastComponentSubgroupsUnique, const bool testNotDualLowerLevelConnections)
{
	bool passCriteria = true;

	SANIComponentNeuralNetwork* secondComponent = NULL;
	
	//cout << "components->size() = " << components->size() << endl;
	
	if(forwardPropogationSentenceData->parseSentenceReverse)
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
		if(SANIpropagateCompactOperations.findValidDualLowerLevelConnection(forwardPropogationSentenceData, components, component, false))		
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



int SANIpropagateCompactGenerateOperationsClass::calculateNextIndexInSequenceProspective(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIGroupParseTree* activatedNeuronWithMaxWordIndexCoverageProspective)
{
	int indexInSequence = 0;
	
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		indexInSequence = forwardPropogationSentenceData->sentenceContents->size()-1 - (activatedNeuronWithMaxWordIndexCoverageProspective->parseTreeMinWordIndex - 1);
	}
	else
	{
		indexInSequence = activatedNeuronWithMaxWordIndexCoverageProspective->parseTreeMaxWordIndex + 1;
	}
	
	return indexInSequence;
}


#ifdef SANI_SEQUENCE_GRAMMAR_COMPONENT_IDENTIFY_VARIABLE_COMPONENTS_VERIFY_THAT_VARIABLE_EDGE_COMPONENT_SOURCE_POS_IS_NOT_IDENTICAL
bool SANIpropagateCompactGenerateOperationsClass::variableEdgeComponentSourcePOSisIdenticalWrapper1(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* generatedNeuron, SANIGroupNeuralNetwork* candidateMatchGroup, bool identifyVariableFirstOrLastComponent)
{
	bool result = false;
	
	//find every existing variable edge (first/last) component pos
	
	SANIComponentNeuralNetwork* newVariableComponent = SANInodes.getFirstComponent(forwardPropogationSentenceData, generatedNeuron, identifyVariableFirstOrLastComponent);
	SANIGroupNeuralNetwork* neuronWithNewVariableComponent = (newVariableComponent->SANIbackGroupConnectionList)[0];	//generated neuron doesn't have variable components
	
	SANIComponentNeuralNetwork* edgeComponent = SANInodes.getFirstComponent(forwardPropogationSentenceData, candidateMatchGroup, identifyVariableFirstOrLastComponent);
	for(int l2=0; l2<edgeComponent->SANIbackGroupConnectionList.size(); l2++)
	{
		SANIGroupNeuralNetwork* edgeComponentSource = edgeComponent->SANIbackGroupConnectionList[l2];
		if(variableEdgeComponentSourcePOSisIdenticalWrapper2(forwardPropogationSentenceData, neuronWithNewVariableComponent, edgeComponentSource, identifyVariableFirstOrLastComponent))
		{
			result = true;
		}
	}
	
	return result;
}

bool SANIpropagateCompactGenerateOperationsClass::variableEdgeComponentSourcePOSisIdenticalWrapper2(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* neuronWithNewVariableComponent, SANIGroupNeuralNetwork* edgeNeuron, bool identifyVariableFirstOrLastComponent)
{
	bool result = false;
	
	//find every existing variable edge (first/last) component pos
	
	if(edgeNeuron->inputLayerNeuron)	//groupTypeIsString
	{
		if(variableEdgeComponentSourcePOSisIdentical(forwardPropogationSentenceData, edgeNeuron, neuronWithNewVariableComponent, identifyVariableFirstOrLastComponent))
		{
			result = true;
		}
	}
	
	if(edgeNeuron->components.size() > 0)
	{
		SANIComponentNeuralNetwork* edgeComponent = SANInodes.getFirstComponent(forwardPropogationSentenceData, edgeNeuron, !identifyVariableFirstOrLastComponent);

		for(int l2=0; l2<edgeComponent->SANIbackGroupConnectionList.size(); l2++)
		{
			SANIGroupNeuralNetwork* edgeComponentSource = edgeComponent->SANIbackGroupConnectionList[l2];
			if(variableEdgeComponentSourcePOSisIdenticalWrapper2(forwardPropogationSentenceData, neuronWithNewVariableComponent, edgeComponentSource, identifyVariableFirstOrLastComponent))
			{
				result = true;
			}
		}
	}
	
	return result;
}

bool SANIpropagateCompactGenerateOperationsClass::variableEdgeComponentSourcePOSisIdentical(const SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const SANIGroupNeuralNetwork* neuronToDetect, SANIGroupNeuralNetwork* edgeNeuron, bool identifyVariableFirstOrLastComponent)
{
	bool result = false;
		
	if(neuronToDetect == edgeNeuron)
	{
		result = true;	
	}
	
	if(edgeNeuron->components.size() > 0)
	{
		SANIComponentNeuralNetwork* edgeComponent = SANInodes.getFirstComponent(forwardPropogationSentenceData, edgeNeuron, !identifyVariableFirstOrLastComponent);

		for(int l2=0; l2<edgeComponent->SANIbackGroupConnectionList.size(); l2++)
		{
			SANIGroupNeuralNetwork* edgeComponentSource = edgeComponent->SANIbackGroupConnectionList[l2];
			if(variableEdgeComponentSourcePOSisIdentical(forwardPropogationSentenceData, neuronToDetect, edgeComponentSource, identifyVariableFirstOrLastComponent))
			{
				result = true;
			}
		}
	}
	
	return result;
}
#endif





		
#endif
