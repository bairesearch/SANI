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
 * File Name: SANIpropagateOperationsParseTree.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1q2b 19-September-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Operations Parse Tree - generic functions
 * /
 *******************************************************************************/


#include "SANIpropagateOperationsParseTree.hpp"


#ifdef SANI_FORWARD

#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT
bool SANIpropagateOperationsParseTreeClass::getMinMaxSequenceIndexInParseTree(const SANIGroupParseTree* currentParseTreeGroup, const bool findMaxOrMinSequenceIndex, int* sequenceIndexMaxOrMin, int level)
{
	bool result = false;
	
	#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
	if(findMaxOrMinSequenceIndex)
	{
		SANInodes.printParseTreeDebugIndentation(level);
		cout << "SANIpropagateOperationsParseTreeClass::getMinMaxSequenceIndexInParseTree: currentParseTreeGroup: " <<  currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;
	}
	#endif
	
	#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
	if(currentParseTreeGroup->components.size() > 0)
	{
		if(findMaxOrMinSequenceIndex)
		{
			*sequenceIndexMaxOrMin = currentParseTreeGroup->parseTreeMaxSequenceIndex;
			//cout << "currentParseTreeGroup->parseTreeMaxSequenceIndex = " << currentParseTreeGroup->parseTreeMaxSequenceIndex << endl;
			result = true;
		}
		else
		{
			*sequenceIndexMaxOrMin = currentParseTreeGroup->parseTreeMinSequenceIndex;
			//cout << "currentParseTreeGroup->parseTreeMinSequenceIndex = " << currentParseTreeGroup->parseTreeMinSequenceIndex << endl;
			result = true;
		}
	}
	#else
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
		#ifdef SANI_DEBUG_PROPAGATE_EXTRA6
		if(!findMaxOrMinSequenceIndex)
		{
			SANInodes.printParseTreeDebugIndentation(level);
			cout << "currentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex = " << currentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex  << endl;
		}
		#endif
		
		if(findMaxOrMinSequenceIndex)
		{
			if(currentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex > *sequenceIndexMaxOrMin)
			{
				*sequenceIndexMaxOrMin = currentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
				result = true;
			}
		}
		else
		{
			if(currentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex < *sequenceIndexMaxOrMin)
			{
				*sequenceIndexMaxOrMin = currentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
				result = true;
			}
		}
						
		if(currentComponent->parseTreeGroupRef != NULL)
		{					
			if(getMinMaxSequenceIndexInParseTree(currentComponent->parseTreeGroupRef, findMaxOrMinSequenceIndex, sequenceIndexMaxOrMin, level+1))
			{
				result = true;
			}
		}
	}
	#endif
	
	return result;	
}
#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY
bool SANIpropagateOperationsParseTreeClass::componentWordConnectivityTestsCompareToMemory(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int sequenceIndexProspectiveFirst, SANIGroupNeuralNetwork* ownerGroup, SANIComponent* lastActiveComponent)
{
	bool result = false;
	
	if(lastActiveComponent->neuronComponentConnectionActiveSequenceElementRecord == NULL)
	{
		cerr << "lastActiveComponent->componentIndex = " << lastActiveComponent->componentIndex << endl;
		cerr << "lastActiveComponent->componentType = " << lastActiveComponent->componentType << endl;
		cerr << "ownerGroup->groupIndex = " << ownerGroup->groupIndex << endl;
		cerr << "SANIpropagateOperationsParseTreeClass::componentWordConnectivityTestsCompareToMemory error: lastActiveComponent->neuronComponentConnectionActiveSequenceElementRecord == NULL" << endl;
		exit(EXIT_ERROR);
	}
	
	if(lastActiveComponent->neuronComponentConnectionActiveSequenceElementRecordMemory.size() != ownerGroup->parseTreeGroupMemory.size())
	{
		cerr << "lastActiveComponent->neuronComponentConnectionActiveSequenceElementRecordMemory.size() = " << lastActiveComponent->neuronComponentConnectionActiveSequenceElementRecordMemory.size() << endl;
		cerr << "ownerGroup->parseTreeGroupMemory.size() = " << ownerGroup->parseTreeGroupMemory.size() << endl;
		cerr << "SANIpropagateOperationsParseTreeClass::componentWordConnectivityTestsCompareToMemory error: (lastActiveComponent->neuronComponentConnectionActiveSequenceElementRecordMemory.size() != ownerGroup->parseTreeGroupMemory.size())" << endl;
		exit(EXIT_ERROR);
	}
	
	int ownerGroupParseTreeGroupMemorySize = ownerGroup->parseTreeGroupMemory.size();	//OR: lastActiveComponent->neuronComponentConnectionActiveSequenceElementRecordMemory.size()
	SANIForwardPropogationSequenceElementData* lastActiveComponentNeuronComponentConnectionActiveSequenceElementRecordOrig = lastActiveComponent->neuronComponentConnectionActiveSequenceElementRecord;
	
	for(int m=0; m<ownerGroupParseTreeGroupMemorySize; m++)
	{
		//cout << "m = " << m << endl;
		
		int sequenceIndexMin;
		int sequenceIndexMax;
		SANIForwardPropogationSequenceElementData* neuronComponentConnectionActiveSequenceElementOption = lastActiveComponent->neuronComponentConnectionActiveSequenceElementRecordMemory[m];
		
		if(forwardPropogationSequenceData->parseSentenceReverse)
		{
			sequenceIndexMin = neuronComponentConnectionActiveSequenceElementOption->sequenceIndex;
			sequenceIndexMax = sequenceIndexProspectiveFirst;
		}
		else
		{
			sequenceIndexMin = sequenceIndexProspectiveFirst;
			sequenceIndexMax = neuronComponentConnectionActiveSequenceElementOption->sequenceIndex;
		}
		
		/*
		cout << "sequenceIndexProspectiveFirst = " << sequenceIndexProspectiveFirst << endl;
		cout << "neuronComponentConnectionActiveSequenceElementOption->translatorSentenceWordIndex = " << neuronComponentConnectionActiveSequenceElementOption->translatorSentenceWordIndex << endl;
		cout << "sequenceIndexMin = " << sequenceIndexMin << endl;
		cout << "sequenceIndexMax = " << sequenceIndexMax << endl;
		*/
		
		if(sequenceIndexMin-sequenceIndexMax == 1)
		{
			bool overWrite = false;
			if(!result || neuronComponentConnectionActiveSequenceElementOption == lastActiveComponentNeuronComponentConnectionActiveSequenceElementRecordOrig)
			{
				overWrite = true;	//favour existing currentParseTreeGroupTemp
			}
			if(overWrite)
			{
				//cout << "SANIpropagateOperationsParseTreeClass::componentWordConnectivityTestsCompareToMemory; overWrite" << endl;
				
				lastActiveComponent->neuronComponentConnectionActiveSequenceElementRecord = neuronComponentConnectionActiveSequenceElementOption;	//GIA3k13a
				ownerGroup->currentParseTreeGroupTemp = ownerGroup->parseTreeGroupMemory[m];	//GIA3k13a

				result = true;
			}
		}	
	}
	
	return result;
}
#endif
#endif



#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
bool SANIpropagateOperationsParseTreeClass::calculatePerformanceWeightOfParseTree(SANIGroupParseTree* currentParseTreeGroup, int* maxWeight)
{
	bool result;
	
	#ifdef SANI_PARSE_PERFORMANCE_WEIGHT_BIO		
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
	*maxWeight = currentParseTreeGroup->parseTreeTotalWeight;
	#else
	*maxWeight = currentParseTreeGroup->parseTreeMaxWeight;
	#endif
	#else
	*maxWeight = 0;
	traceBackpropParseTreeCalculateWeight(currentParseTreeGroup, 1, maxWeight);
	resetNeuronBackprop(currentParseTreeGroup, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
	#endif
	
	return result;
}

bool SANIpropagateOperationsParseTreeClass::setPerformanceWeightOfParseTree(SANIGroupParseTree* currentParseTreeGroup, int maxWeight)
{
	bool result;
	
	#ifdef SANI_PARSE_PERFORMANCE_WEIGHT_BIO		
	#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
	currentParseTreeGroup->parseTreeTotalWeight = maxWeight;
	#else
	currentParseTreeGroup->parseTreeMaxWeight = maxWeight;
	#endif
	#else
	cerr << "SANIpropagateOperationsParseTreeClass::setPerformanceWeightOfParseTree requires SANI_PARSE_PERFORMANCE_WEIGHT_BIO" << endl;
	exit(EXIT_ERROR);
	#endif
	
	return result;
}
#endif


#ifdef SANI_PARSE

#ifdef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE

#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
bool SANIpropagateOperationsParseTreeClass::updatePerformance(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer)
{
	bool result = true;
	
	bool topLevelGroup = SANInodesGroupClassObject.isTopLevelGroupType(currentParseTreeGroup->groupTypeName, currentParseTreeGroup->groupTypeReferenceSetType, forwardPropogationSequenceData->isQuery, forwardPropogationSequenceData->parseIsolatedSubreferenceSets);
	if(topLevelGroup)
	{
		result = updateAndVerifyPerformanceGroupSentence(currentParseTreeGroup, forwardPropogationSequenceData, layer);
	}
	
	return result;
}
#endif

//doesn't perform topLevelGroup check;
bool SANIpropagateOperationsParseTreeClass::updateAndVerifyPerformanceGroupSentence(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer)
{
	bool updatePerformance = true;
	bool verifyPerformance = true;
	return updateAndOrVerifyPerformanceGroup(currentParseTreeGroup, forwardPropogationSequenceData, layer, updatePerformance, verifyPerformance, false, NULL);
}
bool SANIpropagateOperationsParseTreeClass::updatePerformanceGroupSentence(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer)
{
	bool updatePerformance = true;
	bool verifyPerformance = false;
	return updateAndOrVerifyPerformanceGroup(currentParseTreeGroup, forwardPropogationSequenceData, layer, updatePerformance, verifyPerformance, false, NULL);
}
bool SANIpropagateOperationsParseTreeClass::verifyPerformanceGroupSentence(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer)
{
	bool updatePerformance = false;
	bool verifyPerformance = true;
	return updateAndOrVerifyPerformanceGroup(currentParseTreeGroup, forwardPropogationSequenceData, layer, updatePerformance, verifyPerformance, false, NULL);
}
bool SANIpropagateOperationsParseTreeClass::updateAndOrVerifyPerformanceGroup(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer, const bool updatePerformance, const bool verifyPerformance, const bool partialSentenceSequence, const SANIForwardPropogationSignalData* forwardPropogationSignalData)
{
	bool result = true;
			
	int performance = 0;
	
	#ifdef SANI_PARSE_PERFORMANCE_BIO
	
	performance = currentParseTreeGroup->parseTreeMaxSequenceIndex - currentParseTreeGroup->parseTreeMinSequenceIndex + 1;
	
	#else
	
	#ifdef SANI_DEBUG_SEQUENCE_GRAMMAR_WORDCONNECTIVITY_VERIFICATION
	cout << "SANI_SEQUENCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFICATION: SANIpropagateOperationsParseTreeClass::updateOrVerifyPerformanceGroup, traceBackpropParseTreeSequenceIndexCoverage:" << endl;
	#endif
	
	#ifdef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
	int performanceTemp = 0;
	if(!traceBackpropParseTreePerformance(currentParseTreeGroup, 1, &performanceTemp))
	{
		//cout << "SANIpropagateOperationsParseTreeClass::updateOrVerifyPerformanceGroup:traceBackpropParseTreeSequenceIndexCoverage fail #1" << endl;
		result = false;
	}
	resetNeuronBackprop(currentParseTreeGroup, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
	performance = performanceTemp;
	#else
	if(!traceBackpropParseTreeSequenceIndexCoverage(currentParseTreeGroup, 1, forwardPropogationSequenceData->sentenceContents))	
	{
		//cout << "SANIpropagateOperationsParseTreeClass::updateOrVerifyPerformanceGroup:traceBackpropParseTreeSequenceIndexCoverage fail #1" << endl;
		result = false;
	}
	resetNeuronBackprop(currentParseTreeGroup, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED);
	performance = 0;
	for(int i=0; i<forwardPropogationSequenceData->sentenceContents->size(); i++)
	{
		LRPpreprocessorPlainTextWord* currentWord = (forwardPropogationSequenceData->sentenceContents)->at(i);
		if(currentWord->alreadyFoundMatch)
		{
			performance = performance + 1;
			currentWord->alreadyFoundMatch = false;
		}
	}	
	#endif
	
	#endif


	#ifdef SANI_PARSE	//SANI_PARSE_PERFORMANCE
	if(updatePerformance)
	{
		forwardPropogationSequenceData->performance = performance;
	}
	#endif
	
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC_STRICT
	if(verifyPerformance)
	{
		if(result)
		{
			bool sequenceElementConnectivityTest = false;
			
			#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			if(partialSentenceSequence)
			{
				int parseTreeFirstIndex;
				int parseTreeLastIndex;
				if(forwardPropogationSequenceData->parseSentenceReverse)
				{
					parseTreeFirstIndex = currentParseTreeGroup->parseTreeMaxSequenceIndex;
					parseTreeLastIndex = currentParseTreeGroup->parseTreeMinSequenceIndex;
				}
				else
				{
					parseTreeFirstIndex = currentParseTreeGroup->parseTreeMinSequenceIndex;
					parseTreeLastIndex = currentParseTreeGroup->parseTreeMaxSequenceIndex;
				}

				if(forwardPropogationSignalData->firstIndexInSequence == parseTreeFirstIndex)
				{
					if(forwardPropogationSignalData->currentIndexInSequence == parseTreeLastIndex)
					{	
						if(performance = (forwardPropogationSignalData->currentIndexInSequence - forwardPropogationSignalData->firstIndexInSequence + 1))
						{
							sequenceElementConnectivityTest = true;
						}
					}
				}	
			}
			else
			{	
			#endif			
				if(performance == forwardPropogationSequenceData->sentenceContents->size())
				{
					sequenceElementConnectivityTest = true;
				}
			#ifdef SANI_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
			}
			#endif
			
			if(!sequenceElementConnectivityTest)
			{
				#ifndef SANI_SEQUENCE_SEQUENCEELEMENT_CONNECTIVITY_VERIFICATION_DO_NOT_STOP
				cout << "SANIpropagateOperationsParseTreeClass::updateOrVerifyPerformanceGroup:sequenceElementConnectivityTest fail #2" << endl;
				#endif
				result = false;
			}
		}
	}
	#endif
			
	return result;
}

#endif

bool SANIpropagateOperationsParseTreeClass::traceBackpropParseTreeSetTraced(SANIGroupParseTree* currentParseTreeGroup, int level)
{
	bool result = true;
	
	#ifndef SANI_DONT_SET_NEURON_TRACED
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
	#endif
		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		

			if(SANInodes.hasComponentTypeString(currentComponent))
			{
			}
			#ifndef SANI_HEAVY_OPTIMISED	//CHECKTHIS
			else
			{
			#endif					
				if(currentComponent->parseTreeGroupRef != NULL)
				{													
					if(!traceBackpropParseTreeSetTraced(currentComponent->parseTreeGroupRef, level+1))
					{
						result = false;
					}
				}
				else
				{
					//cout << "(currentComponent->parseTreeGroupRef == NULL)" << endl;
				}
			#ifndef SANI_HEAVY_OPTIMISED
			}
			#endif
		}
	#ifndef SANI_DONT_SET_NEURON_TRACED	
	}
	#endif
	
	return result;
}

#ifdef SANI_PARSE_PERFORMANCE_RECORD_PERFORMANCE_METHOD_OLD_INCREMENT_FOR_EVERY_GROUP_REF_RECURSE
bool SANIpropagateOperationsParseTreeClass::traceBackpropParseTreePerformance(SANIGroupParseTree* currentParseTreeGroup, int level, int* performance)
{
	bool result = true;
	
	#ifndef SANI_DONT_SET_NEURON_TRACED
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
	#endif

		*performance = *performance + 1;
		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
			if(SANInodes.hasComponentTypeString(currentComponent))
			{
			}
			#ifndef SANI_HEAVY_OPTIMISED	//CHECKTHIS
			else
			{
			#endif					
				if(currentComponent->parseTreeGroupRef != NULL)
				{													
					if(!traceBackpropParseTreePerformance(currentComponent->parseTreeGroupRef, level+1, performance))
					{
						result = false;
					}
				}
				else
				{
					//cout << "(currentComponent->parseTreeGroupRef == NULL)" << endl;
				}
			#ifndef SANI_HEAVY_OPTIMISED
			}
			#endif
		}
	#ifndef SANI_DONT_SET_NEURON_TRACED	
	}
	#endif
	
	return result;
}
#else
bool SANIpropagateOperationsParseTreeClass::traceBackpropParseTreeSequenceIndexCoverage(SANIGroupParseTree* currentParseTreeGroup, int level, vector<LRPpreprocessorPlainTextWord*>* sentenceContents)
{
	bool result = true;
	
	#ifndef SANI_DONT_SET_NEURON_TRACED
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
	#endif
		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
			if(SANInodes.hasComponentTypeString(currentComponent))
			{
				if(LRPpreprocessorWordClassObject.isWordInWordlist(sentenceContents, currentComponent->candidateStringMatch))
				{	
					#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC_STRICT_MUTUALLY_EXCLUSIVE
					if(currentComponent->candidateStringMatch->alreadyFoundMatch)
					{
						result = false;
						//cout << "duplicate instance of sequenceElement detected in parsetree - fail to parse sentence" << endl;
						//duplicate instance of sequenceElement detected in parsetree - fail to parse sentence
					}
					else
					{
					#endif		
							currentComponent->candidateStringMatch->alreadyFoundMatch = true;
							//nb this method won't work if subReferenceSets are syntactically identical (and neural net groups are therefore reused); eg the red dog eats a blue apple.
								//"the" and "a" will use identical neural groups and so will only count to +1 performance total
					#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_POSTHOC_STRICT_MUTUALLY_EXCLUSIVE
					}
					#endif
				}
			}
			#ifndef SANI_HEAVY_OPTIMISED	//CHECKTHIS
			else
			{
			#endif					
				if(currentComponent->parseTreeGroupRef != NULL)
				{													
					if(!traceBackpropParseTreeSequenceIndexCoverage(currentComponent->parseTreeGroupRef, level+1, sentenceContents))
					{
						result = false;
					}
				}
				else
				{
					//cout << "(currentComponent->parseTreeGroupRef == NULL)" << endl;
				}
			#ifndef SANI_HEAVY_OPTIMISED
			}
			#endif
		}
	#ifndef SANI_DONT_SET_NEURON_TRACED	
	}
	#endif
	
	return result;
}
#endif

#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT
bool SANIpropagateOperationsParseTreeClass::traceBackpropParseTreeCalculateWeight(SANIGroupParseTree* currentParseTreeGroup, int level, int* maxWeight)
{
	bool result = true;
	
	#ifndef SANI_DONT_SET_NEURON_TRACED
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
	#endif
	
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_SUM
		*maxWeight = *maxWeight + currentParseTreeGroup->groupWeight;
		#else
		if(currentParseTreeGroup->groupWeight > *maxWeight)
		{
			*maxWeight = currentParseTreeGroup->groupWeight;
		}
		#endif
		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
		
			if(SANInodes.hasComponentTypeString(currentComponent))
			{
			}
			#ifndef SANI_HEAVY_OPTIMISED	//CHECKTHIS
			else
			{
			#endif					
				if(currentComponent->parseTreeGroupRef != NULL)
				{												
					if(!traceBackpropParseTreeCalculateWeight(currentComponent->parseTreeGroupRef, level+1, maxWeight))
					{
						result = false;
					}
				}
				else
				{
					//cout << "(currentComponent->parseTreeGroupRef == NULL)" << endl;
				}
			#ifndef SANI_HEAVY_OPTIMISED
			}
			#endif
		}
	#ifndef SANI_DONT_SET_NEURON_TRACED	
	}
	#endif
	
	return result;
}

#endif

#ifdef SANI_DEBUG_PARSE_TREE_PRINT_SUPPORT_RECURSION
bool SANIpropagateOperationsParseTreeClass::traceBackpropParseTreePrint(SANIGroupParseTree* currentParseTreeGroup, int level)
{
	bool result = true;
	
	#ifndef SANI_DONT_SET_NEURON_TRACED
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
	#endif

		//#ifdef SANI_DEBUG_PROPAGATE
		SANInodes.printParseTreeDebugIndentation(level);
		#ifdef SANI_SEQUENCE_GRAMMAR
		cout << "traceBackpropParseTreePrint: currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << ", currentParseTreeGroup->groupRef->groupIndex = " << currentParseTreeGroup->groupRef->groupIndex << endl;
		#else
		cout << "traceBackpropParseTreePrint: currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << ", currentParseTreeGroup->groupTypeName = " << currentParseTreeGroup->groupTypeName << endl;		//<< ", parse sequenceElement (providing semanticRelationReturnEntity) = ?"
		#endif
		//#endif
		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i];
			
			//#ifdef SANI_DEBUG_PROPAGATE_EXTRA
			SANInodes.printComponent(currentComponent, level);	//TEMP for DEBUG
			//#endif
		
			if(SANInodes.hasComponentTypeString(currentComponent))
			{
				SANInodes.printComponent(currentComponent, level+1);	//+1 added @GIA3j5aTEMP66
			}
			#ifndef SANI_HEAVY_OPTIMISED	//CHECKTHIS
			else
			{
			#endif					
				if(currentComponent->parseTreeGroupRef != NULL)
				{												
					//#ifdef SANI_DEBUG_PROPAGATE_EXTRA
					SANInodes.printParseTreeDebugIndentation(level);
					cout << "(currentComponent->parseTreeGroupRef != NULL): currentComponent->parseTreeGroupRef->groupName = " << currentComponent->parseTreeGroupRef->groupName << endl;
					//#endif	
																
					if(!traceBackpropParseTreePrint(currentComponent->parseTreeGroupRef, level+1))
					{
						result = false;
					}
				}
				else
				{
					//cout << "(currentComponent->parseTreeGroupRef == NULL)" << endl;
				}
			#ifndef SANI_HEAVY_OPTIMISED
			}
			#endif
		}
	#ifndef SANI_DONT_SET_NEURON_TRACED	
	}
	#endif
	
	return result;
}
#endif



#endif






#ifdef SANI_PARSE
#ifdef SANI_HEAVY
#ifdef SANI_HEAVY_OPTIMISED
bool SANIpropagateOperationsParseTreeClass::resetGroupParseTreeGroupRef(SANIGroupActivationMemory* activationMemoryGroup, SANIGroupParseTree* currentParseTreeGroup, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;	
	
	#ifdef SANI_EFFICIENCY_STORE_POINT_ARRAY_IN_BASE_GROUP			
	for(int i=1; i<activationMemoryGroup->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray.size(); i++)	//i=1; never remove the first activationPathSequenceElementFirstActivationMemoryGroupActivationPoint (as this will correspond to activationPathSequenceElementFirstActivationMemoryGroup)
	{
		SANIGroupActivationMemory* activationPathSequenceElementFirstActivationMemoryGroupActivationPointTemp = activationMemoryGroup->activationPathSequenceElementFirstActivationMemoryGroupActivationPointArray[i];
		SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroupActivationPointTemp = currentParseTreeGroup->activationPathSequenceElementFirstParseTreeGroupActivationPointArray[i];
		
		if(!(activationPathSequenceElementFirstActivationMemoryGroupActivationPointTemp->firstLevelActivationPointAdded))
		{
			SANInodes.deleteGroup(activationPathSequenceElementFirstActivationMemoryGroupActivationPointTemp);	
			if(!(currentParseTreeGroup->neuronTraced))	//parseTreeGroups in activationPointArray >i=0 can still be added to final parseTree
			{
				SANInodes.deleteGroup(activationPathSequenceElementFirstParseTreeGroupActivationPointTemp);
			}
		}
	}
	#endif

	if(deleteExistingParseTreeGroupRef)
	{
		if(!(currentParseTreeGroup->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
		{
			SANInodes.deleteGroup(currentParseTreeGroup);
		}
	}
		
	SANInodes.deleteGroup(activationMemoryGroup);
	
	return result;
}
#else
bool SANIpropagateOperationsParseTreeClass::resetGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		for(int i=0; i<group->currentParseTreeGroupArray.size(); i++)
		{
			SANIGroupParseTree* tempParseTreeGroup = (group->currentParseTreeGroupArray)[i];
			if(!(tempParseTreeGroup->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
			{
				delete tempParseTreeGroup;
			}
		}
	}
	group->currentParseTreeGroupArray.clear();

	SANIGroupParseTree* tempParseTreeGroup = SANInodes.convertNeuralNetworkGroupToParseTreeGroupNew(group);	//new SANIGroupParseTree(*convertNeuralNetworkGroupToParseTreeGroup(group));
	/*
	NO, as dont wish to copy subcomponents into tempParseTreeGroup;
	SANIGroupParseTree* tempParseTreeGroup = copyGroup(group);
	deleteComponents(&(tempParseTreeGroup->components));
	*/
	tempParseTreeGroup->components.clear();
	group->currentParseTreeGroupArray.push_back(tempParseTreeGroup);		
	
	#ifdef SANI_SAVE_MEMORY_GROUPS
	for(int i=0; i<group->activationMemoryGroupArray.size(); i++)
	{
		SANIGroupActivationMemory* tempMemoryGroup = (group->activationMemoryGroupArray)[i];
		delete tempMemoryGroup;
	}
	group->activationMemoryGroupArray.clear();

	SANIGroupActivationMemory* tempMemoryGroup = SANInodes.convertNeuralNetworkGroupToMemoryActivationGroupNew(group); //copyGroup(convertNeuralNetworkGroupToMemoryActivationGroup(group));
	group->activationMemoryGroupArray.push_back(tempMemoryGroup);
	#endif
	
	return result;
}
#endif
#else
bool SANIpropagateOperationsParseTreeClass::resetGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, const bool deleteExistingParseTreeGroupRef)
{	
	bool result = true;
	
	if(deleteExistingParseTreeGroupRef)
	{
		if(group->currentParseTreeGroupTemp != NULL)
		{
			#ifndef SANI_DONT_SET_NEURON_TRACED
			if(!(group->currentParseTreeGroupTemp->neuronTraced))	//fixed GIA3g11aTEMP14
			//if(!(group->neuronTraced))	//added GIA3g6aTEMP32 - only delete parseTreeGroup if !neuronTraced
			{
			#endif
				//delete group->currentParseTreeGroupTemp;	//OLD: don't delete components
				SANInodes.deleteGroup(group->currentParseTreeGroupTemp);	//added GIA3g11aTEMP27
			#ifndef SANI_DONT_SET_NEURON_TRACED
			}
			#endif
		}
	}
	group->currentParseTreeGroupTemp = SANInodes.convertNeuralNetworkGroupToParseTreeGroupNew(group); 	//new SANIGroupParseTree(*convertNeuralNetworkGroupToParseTreeGroup(group));
	group->currentParseTreeGroupTemp->components.clear();	//added 3g1h
	
	#ifdef SANI_ENFORCE_SEQUENCEELEMENT_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT_MEMORY	
	//clear neuronComponentConnectionActiveSequenceElementRecordMemory
	if(deleteExistingParseTreeGroupRef)
	{
		group->parseTreeGroupMemory.clear();
		group->parseTreeGroupMemory.push_back(group->currentParseTreeGroupTemp);
		for(int i=0; i<group->components.size(); i++)
		{
			SANIComponent* currentComponent = (group->components)[i];
			currentComponent->neuronComponentConnectionActiveSequenceElementRecordMemory.clear();
		}
	}
	#endif
	
	return result;
}
bool SANIpropagateOperationsParseTreeClass::restoreGroupParseTreeGroupRef(SANIGroupNeuralNetwork* group, SANIGroupParseTree* parseTreeGroupOrig)
{
	bool result = true;
	
	SANInodes.deleteGroup(group->currentParseTreeGroupTemp);	//CHECKTHIS - delete newly created currentParseTreeGroupTemp 
	group->currentParseTreeGroupTemp = parseTreeGroupOrig;
	
	return result;
}


#endif
#endif




#ifdef SANI_PARSE

bool SANIpropagateOperationsParseTreeClass::resetNeuronBackprop(SANIGroupParseTree* currentParseTreeGroup, const int groupBoolIndexType)
{
	bool result = true;
	
	bool* boolPointer = NULL;

	
	if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_NEURON_TRACED)
	{
		boolPointer = &(currentParseTreeGroup->neuronTraced);
	}
	/*
	else if(groupBoolIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_BACKPROP_SOLIDIFY)
	{
		boolPointer = &(currentParseTreeGroup->neuronSolidified);
	}
	*/
				
	if(*boolPointer)
	{
		*boolPointer = false;
		
		for(int i1=0; i1<currentParseTreeGroup->components.size(); i1++)
		{
			SANIComponentParseTree* currentComponent = (currentParseTreeGroup->components)[i1];
			if(currentComponent->parseTreeGroupRef != NULL)
			{
				resetNeuronBackprop(currentComponent->parseTreeGroupRef, groupBoolIndexType);
			}
		}
	}
	
	return result;
}

#endif


#ifdef SANI_FREE_MEMORY
bool SANIpropagateOperationsParseTreeClass::initialiseParseTreeGroupList(vector<SANIGroupType*>* SANIGroupTypes, vector<SANIGroupParseTree*>* parseTreeGroupList)
{	
	bool result = true;
	
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[i2];
			parseTreeGroupList->push_back(group->currentParseTreeGroupTemp);
			//cout << "initialiseParseTreeGroupList" << endl;
		}
	}
	
	return result;
}

bool SANIpropagateOperationsParseTreeClass::deinitialiseParseTreeGroupList(vector<SANIGroupType*>* SANIGroupTypes, vector<SANIGroupParseTree*>* parseTreeGroupList)
{	
	bool result = true;
	
	for(int i2=0; i2<parseTreeGroupList->size(); i2++)
	{
		SANIGroupParseTree* parseTreeGroupTemp = (*parseTreeGroupList)[i2];
		SANInodes.deleteGroup(parseTreeGroupTemp);
	}
	parseTreeGroupList->clear();
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		for(int i2=0; i2<groupType->groups.size(); i2++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[i2];
			group->currentParseTreeGroupTemp = NULL;
		}
	}
	
	return result;
}
#endif


#ifdef SANI_BIO_DO_NOT_RELY_ON_PARSE_TREE_MEMORY
void SANIpropagateOperationsParseTreeClass::updateParseTreeMaxMinSequenceIndexOfParent(SANIGroupParseTree* parentGroup, SANIComponentParseTree* parentComponent, SANIGroupParseTree* childGroup)
{
	#ifdef SANI_PARSE_GENERATE_PARSE_TREE
	if(!SANInodes.parseTreeNodeInputLayer(parentComponent->parseTreeGroupRef))
	#else
	if(!SANInodes.parseTreeNodeInputLayer(childGroup))
	#endif
	{	
		//has children
		#ifdef SANI_PARSE_GENERATE_PARSE_TREE
		childGroup = parentComponent->parseTreeGroupRef;	//not required
		#endif
		if(childGroup->parseTreeMaxSequenceIndex > parentGroup->parseTreeMaxSequenceIndex)
		{
			parentGroup->parseTreeMaxSequenceIndex = childGroup->parseTreeMaxSequenceIndex;
		}
		if(childGroup->parseTreeMinSequenceIndex < parentGroup->parseTreeMinSequenceIndex)
		{
			parentGroup->parseTreeMinSequenceIndex = childGroup->parseTreeMinSequenceIndex;
		}
		
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(childGroup->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = childGroup->parseTreeMaxWeight;
		}
		if(childGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = childGroup->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + childGroup->parseTreeTotalWeight;
		#endif
	}
	else
	{
		if(parentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex > parentGroup->parseTreeMaxSequenceIndex)
		{
			parentGroup->parseTreeMaxSequenceIndex = parentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
		}
		if(parentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex < parentGroup->parseTreeMinSequenceIndex)
		{
			parentGroup->parseTreeMinSequenceIndex = parentComponent->neuronComponentConnectionActiveSequenceElementRecord->sequenceIndex;
		}
	
		/*//FUTURE; take into account component weights;
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(parentComponent->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = parentComponent->parseTreeMaxWeight;
		}
		if(parentGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = parentComponent->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + parentComponent->parseTreeTotalWeight;	
		#endif	
		*/
	}
}
#ifdef SANI_SEQUENCE_GRAMMAR
void SANIpropagateOperationsParseTreeClass::updateParseTreeMaxMinSequenceIndexOfParentBasic(SANIGroupParseTree* parentGroup, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIGroupParseTree* childGroup)
{
	if(!SANInodes.parseTreeNodeInputLayer(childGroup))
	{	
		//has children
		if(childGroup->parseTreeMaxSequenceIndex > parentGroup->parseTreeMaxSequenceIndex)
		{
			parentGroup->parseTreeMaxSequenceIndex = childGroup->parseTreeMaxSequenceIndex;
		}
		if(childGroup->parseTreeMinSequenceIndex < parentGroup->parseTreeMinSequenceIndex)
		{
			parentGroup->parseTreeMinSequenceIndex = childGroup->parseTreeMinSequenceIndex;
		}
		
		/*
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(childGroup->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = childGroup->parseTreeMaxWeight;
		}
		if(childGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = childGroup->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + childGroup->parseTreeTotalWeight;
		#endif
		*/
	}
	else
	{
		if(forwardPropogationSequenceElementData->sequenceIndex > parentGroup->parseTreeMaxSequenceIndex)
		{
			parentGroup->parseTreeMaxSequenceIndex = forwardPropogationSequenceElementData->sequenceIndex;
		}
		if(forwardPropogationSequenceElementData->sequenceIndex < parentGroup->parseTreeMinSequenceIndex)
		{
			parentGroup->parseTreeMinSequenceIndex = forwardPropogationSequenceElementData->sequenceIndex;
		}
	
		/*//FUTURE; take into account component weights;
		#ifdef SANI_TAKE_LAST_SUCCESSFUL_PARSE_LIMIT_ITERATIONS_PREFERENCE_WEIGHT_DYNAMIC_BIO
		if(parentComponent->parseTreeMaxWeight > parentGroup->parseTreeMaxWeight)
		{
			parentGroup->parseTreeMaxWeight = parentComponent->parseTreeMaxWeight;
		}
		if(parentGroup->parseTreeMinWeight < parentGroup->parseTreeMinWeight)
		{
			parentGroup->parseTreeMinWeight = parentComponent->parseTreeMinWeight;
		}
		parentGroup->parseTreeTotalWeight = parentGroup->parseTreeTotalWeight + parentComponent->parseTreeTotalWeight;	
		#endif	
		*/
	}
}
#endif
#endif


#endif

