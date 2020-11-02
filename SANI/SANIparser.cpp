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
 * File Name: SANIparser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1n7b 01-November-2020
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Parser
 * /
 *******************************************************************************/


#include "SANIparser.hpp"
#include "SHAREDvars.hpp"



//TODO: check this code

#ifdef USE_GIA
#ifdef SANI_PARSER

#ifdef SANI_PARSE_SIMULTANEOUS
bool SANIparserClass::generateSemanticRelationsFromTxtRelationsWrapperNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, SANIGroupParseTree* firstParseTreeGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, int layer)
{
	bool result = true;
	
	#ifdef SANI_PARSE_BASIC
	forwardPropogationSignalData->parserForwardPropogationSignalData = GIAposRelTranslatorParserForwardPropogationSignalData();	//reinitialise parserForwardPropogationSignalData (clear all variables)
	#endif
	
	GIAposRelTranslatorParserForwardPropogationSignalData parserForwardPropogationSignalData;
	if(!generateSemanticRelationsFromTxtRelationsNeuralNetwork(translatorVariables, firstParseTreeGroup, &parserForwardPropogationSignalData, layer))
	{
		result = false;
	}

	#ifndef SANI_PARSE_BASIC
	forwardPropogationSignalData->parserForwardPropogationSignalData = parserForwardPropogationSignalData;	//store for next generateSemanticRelationsFromTxtRelationsWrapper execution
	#endif
		
	return result;
}

bool SANIparserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, SANIGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorParserForwardPropogationSignalData* parserForwardPropogationSignalData, int layer)
{
	bool result = true;
	
	int numberOfComponentsInGroup = currentParseTreeGroup->components.size();
	vector<GIAposRelTranslatorParserForwardPropogationSignalData> parseTreeComponentSignalDataArray(numberOfComponentsInGroup);
	
	/*
	//code used if SANIGroupParseTree* currentParseTreeGroup replaced with SANIGroupNeuralNetwork* currentParseTreeGroup
	if(currentParseTreeGroup->neuronActive)
	{
	*/		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			SANIComponentParseTree* ownerComponent = (currentParseTreeGroup->components).at(i);
			/*
			//code used if SANIGroupParseTree* currentParseTreeGroup replaced with SANIGroupNeuralNetwork* currentParseTreeGroup
			if(ownerComponent->neuronComponentConnectionActive)
			{
			*/
				SANIComponentParseTree* parseTreeComponent = ownerComponent;
				
				/*
				//code used if SANIGroupParseTree* currentParseTreeGroup replaced with SANIGroupNeuralNetwork* currentParseTreeGroup
				if(SANInodesComponentClassObject.componentHasSubcomponents(ownerComponent))
				{
					for(int i1=0; i1<ownerComponent->subComponents.size(); i1++)
					{
						SANIComponent* subComponent = (ownerComponent->subComponents)[i1];
						if(subComponent->neuronComponentConnectionActive)
						{
							parseTreeComponent = subComponent;	//CHECKTHIS: what if the OR component has multiple activated subcomponents?
						}
					}
				}
				*/

				GIAposRelTranslatorParserForwardPropogationSignalData* parseTreeComponentSignalData = &(parseTreeComponentSignalDataArray[i]);

				#ifdef SANI_DEBUG_RULES_PRINT_PARSE_PROCESS2
				SANIrules.printComponent(parseTreeComponent, layer);
				#endif

				if(parseTreeComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
				{
					#ifdef SANI_PARSE_SIMULTANEOUS_BIO
					*parseTreeComponentSignalData = *(parseTreeComponent->parserForwardPropogationSignalData);
					#else
					if(parseTreeComponent->parseTreeGroupRef != NULL)
					{
						#ifdef SANI_PARSE_BASIC
						if(!generateSemanticRelationsFromTxtRelationsNeuralNetwork(translatorVariables, parseTreeComponent->parseTreeGroupRef, parseTreeComponentSignalData, layer+1))
						{
							result = false;
						}
						#else
						*parseTreeComponentSignalData = (parseTreeComponent->parseTreeGroupRef->groupOrig->semanticRelationReturnEntityForwardPropogationSignalData).parserForwardPropogationSignalData;
						/*
						//note can't check parseTreeComponent->parseTreeGroupRef->neuronActive as it may have been reset by SANIClass::propagateWordThroughNetworkGroup; resetGroupActivation
							//note it will not reset semanticRelationReturnEntityForwardPropogationSignalData, but deactivate the group+components
						if(!parseTreeComponent->parseTreeGroupRef->neuronActive)
						{
							cerr << "SANIparserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork{} error; (!parseTreeComponent->parseTreeGroupRef->neuronActive)" << endl;
							exit(EXIT_ERROR);
						}
						*/						
						#endif
					}
					else
					{
						cout << "SANIparserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork error: (parseTreeComponent->parseTreeGroupRef == NULL)" << endl;
						exit(EXIT_ERROR);
					}
					#endif
					
					if(parseTreeComponentSignalData->semanticRelationReturnEntity == NULL)
					{
						cerr << "SANIparserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork{} error; (parseTreeComponentSignalData->semanticRelationReturnEntity == NULL)" << endl;
						exit(EXIT_ERROR);
					}
				}
				else if(parseTreeComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
				{
					LRPpreprocessorPlainTextWord* parseTreeComponentWord = parseTreeComponent->candidateStringMatch;
					parseTreeComponentSignalData->semanticRelationReturnEntity = parseTreeComponentWord->translatorEntity;
					//cout << "parseTreeComponentWord->tagName = " << parseTreeComponentWord->tagName << endl;
					//cout << "parseTreeComponentWord->translatorEntity->entityName = " << parseTreeComponentWord->translatorEntity->entityName << endl;
				}
				else
				{
					cerr << "GIAposRelTranslatorParserOperationsClass::generateRulesGroupTreeComponent{} error: parseTreeComponent->componentType illdefined" << endl;
					exit(EXIT_ERROR);
				}
			/*
			//code used if SANIGroupParseTree* currentParseTreeGroup replaced with SANIGroupNeuralNetwork* currentParseTreeGroup
			}
			else
			{
				bool optional = false;
				if(!(ownerComponent->optional))
				{
					cerr << "SANIparserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork error: !(ownerComponent->optional)" << endl;
					exit(EXIT_ERROR);
				}
			}
			*/
		}

		if(!GIAposRelTranslatorParserOperations.generateSemanticRelationsFromTxtRelationsPart2(translatorVariables->GIAtranslatorVariables, currentParseTreeGroup, &parseTreeComponentSignalDataArray, parserForwardPropogationSignalData, layer))
		{
			result = false;
		}

	/*
	}
	else
	{
		#ifndef SANI_PARSE_BASIC
		cerr << "SANIparserClass::generateSemanticRelationsFromTxtRelationsNeuralNetwork error: !(currentParseTreeGroup->neuronActive)" << endl;
		exit(EXIT_ERROR);
		#endif
	}
	*/
	
	return result;
}
#endif
#endif


#endif
