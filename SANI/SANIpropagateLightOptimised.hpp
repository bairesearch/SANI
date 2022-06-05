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
 * File Name: SANIpropagateLightOptimised.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1r1a 05-June-2022
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Light Optimised - ~O(n)
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_LIGHT_OPTIMISED
#define HEADER_SANI_PROPAGATE_LIGHT_OPTIMISED

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
#include "SANIformation.hpp"
#include "SANIposRelTranslatorVariables.hpp"
#ifdef USE_GIA
#ifdef SANI_PARSE
#include "GIAposRelTranslatorParserOperations.hpp"
#endif
#endif
#ifdef SANI_PARSE_SIMULTANEOUS
#include "SANIparser.hpp"
#endif
#include "ANNdisplay.hpp"



#ifdef SANI_LIGHT_OPTIMISED


class SANIpropagateLightOptimisedClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorPOStaggerClass LRPpreprocessorPOStagger;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: SANIrulesClass SANIrules;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: SANIpropagateOperationsParseTreeClass SANIpropagateOperationsParseTree;
	private: SANIformationClass SANIformation;
	#ifdef SANI_PARSE
	private: GIAposRelTranslatorParserOperationsClass GIAposRelTranslatorParserOperations;
	#endif
	#ifdef SANI_PARSE_SIMULTANEOUS
	private: SANIparserClass SANIparser;
	#endif
	private: ANNdisplayClass ANNdisplay;

	public: bool executePosRelTranslatorNeuralNetwork(SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, constEffective vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
		private: bool propagateWordThroughNetworkIntro(SANItranslatorVariablesClass* translatorVariables, int w, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup);
			private: bool propagateWordThroughNetworkGroupInit(SANItranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup);
				private: bool propagateWordThroughNetworkGroupInit(SANItranslatorVariablesClass* translatorVariables, const int w, SANIGroupNeuralNetwork* inputLayerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup);
					private: bool propagateWordThroughNetworkGroupIntro(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementFirstParseTreeGroup);
						private: bool propagateWordThroughNetworkGroup(SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup);
							private: bool propagateWordThroughNetworkGroupSelect(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const bool preprocessSearchForNonResetActivations);
								private: bool propagateWordThroughNetworkGroupComponentWrapper(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool measureSequentialActivationOnly, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent, bool* resetUnallowed, const SANIposRelTranslatorDebug* debug);	
									private: bool propagateWordThroughNetworkGroupComponent(SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* ownerComponent, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool activationSequenceCompleted, int layer, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool existingActivationFoundStartComponent, const SANIposRelTranslatorDebug* debug);
										private: bool propagateWordThroughNetworkGroupComponentPassNextWord(SANItranslatorVariablesClass* translatorVariables, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, int layer, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner);
										#ifdef SANI_SOLIDIFY_NET_BACKPROP
										#ifdef SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
										private: bool solidifyNeuralNetBackpropGroupLastComponent(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupParseTree* currentParseTreeGroup, int level);
										#endif
										#endif
										
										#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
										private: bool componentTests2(const SANIGroupNeuralNetwork* group, const SANIGroupParseTree* currentParseTreeGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool activationSequenceCompleted);
											private: bool findPreviousWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const int currentSequenceIndex, const int previousWordPOStype);
											private: bool findPreceedingWordInSentence(vector<LRPpreprocessorPlainTextWord*>* sentenceContents, const int currentSequenceIndex, const int existsPreceedingWordPOStype);
										#endif
										
										#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
										private: bool resetAllowed(constEffective SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const bool existingActivationFoundStartComponent, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool firstActiveComponentInGroup, const SANIComponentNeuralNetwork* ownerComponent);
											private: bool existingActivatedComponentCapturesLastWordInSentence(constEffective SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const bool existingActivationFoundStartComponent, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const bool firstActiveComponentInGroup);
												private: bool existingActivatedComponentCapturesLastWordInSentence(const SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, const SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, const SANIComponentParseTree* lastActiveComponentInParseTree, const SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const bool existingActivationFoundStartComponent, const SANIForwardPropogationSequenceData* forwardPropogationSequenceData);
										#endif

										#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
										private: bool verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData, const bool existingActivationFoundStartComponent);
											private: bool getWordIndicesInParseTree(const SANIGroupParseTree* currentParseTreeGroup, vector<int>* sequenceIndexList, const int level);
										#endif
	
										#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_MAX_NUMBER_OF_WORDS_IN_PARSETREE
										private: bool existingActivationVerifyMaxNumberWordsInParseTree(const SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer, const bool existingActivationFoundStartComponent);
											private: int countNumberWordsInParseTree(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSequenceData* forwardPropogationSequenceData, const int layer);
										#endif
										
										#ifdef SANI_VERIFY_OWNER_GROUP_DOESNT_CONTAIN_NEWLY_ACTIVATED_COMPONENT_IN_ITS_PARSE_TREE
										private: bool verifyOwnerParseTreeGroupDoesntContainNewActivationComponentParseTree(const SANIGroupNeuralNetwork* ownerGroup, const SANIGroupParseTree* activationPathSequenceElementCurrentParseTreeGroup, const int layer);
											private: bool parseTreeContainsGroup(const SANIGroupParseTree* currentParseTreeGroup, const SANIGroupParseTree* parseTreeGroupToFind, const int level);
										#endif
										
	private: bool printBackpropParseTree(SANIGroupParseTree* group, const int level);
	private: bool groupActivationCompleted(const SANIGroupNeuralNetwork* group);
	
	#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	public: bool demarkateOptimumPathwayBackprop(const SANIGroupParseTree* currentParseTreeGroup);
	private: bool isWordInGroupOptimumPathwayWordList(const SANIForwardPropogationSequenceData* forwardPropogationSequenceData, SANIComponentNeuralNetwork* currentComponent, SANIForwardPropogationSequenceElementData* forwardPropogationSequenceElementData);
	#endif



};

#endif

#endif
