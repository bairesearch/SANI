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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1m5b 01-September-2020
 * Requirements: 
 * Description: SANI (Sequentially Activated Neuronal Input neural network) Light Optimised - ~O(n)
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_LIGHT_OPTIMISED
#define HEADER_SANI_PROPAGATE_LIGHT_OPTIMISED

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#include "SANInodes.hpp"
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANIpropagateOperations.hpp"
#include "SANIformation.hpp"
#ifdef SANI_PARSE
#include "GIAposRelTranslatorParserOperations.hpp"
#endif
#ifdef SANI_PARSE_SIMULTANEOUS
#include "SANIparser.hpp"
#endif
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorWordClass.hpp"
#include "GIApreprocessorPOStagger.hpp"
#include "ANNdisplay.hpp"



#ifdef SANI_LIGHT_OPTIMISED


class SANIpropagateLightOptimisedClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: SANIrulesClass SANIrules;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: SANIformationClass SANIformation;
	#ifdef SANI_PARSE
	private: GIAposRelTranslatorParserOperationsClass GIAposRelTranslatorParserOperations;
	#endif
	#ifdef SANI_PARSE_SIMULTANEOUS
	private: SANIparserClass SANIparser;
	#endif
	private: ANNdisplayClass ANNdisplay;

	public: bool executePosRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
		private: bool propagateWordThroughNetworkIntro(GIAtranslatorVariablesClass* translatorVariables, const int w, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup);
			private: bool propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup);
				private: bool propagateWordThroughNetworkGroupInit(GIAtranslatorVariablesClass* translatorVariables, const int w, SANIGroupNeuralNetwork* inputLayerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup);
					private: bool propagateWordThroughNetworkGroupIntro(GIAtranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup);
						private: bool propagateWordThroughNetworkGroup(GIAtranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
							private: bool propagateWordThroughNetworkGroupSelect(GIAtranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool preprocessSearchForNonResetActivations);
								private: bool propagateWordThroughNetworkGroupComponentWrapper(GIAtranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, const bool measureSequentialActivationOnly, bool* sequentialActivationFound, bool* existingActivationFoundStartComponent, bool* resetUnallowed, GIAposRelTranslatorDebug* debug);	
									private: bool propagateWordThroughNetworkGroupComponent(GIAtranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const int groupFrontComponentConnectionListIndex, bool existingActivationFoundStartComponent, GIAposRelTranslatorDebug* debug);
										private: bool propagateWordThroughNetworkGroupComponentPassNextWord(GIAtranslatorVariablesClass* translatorVariables, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner);
										#ifdef SANI_SOLIDIFY_NET_BACKPROP
										#ifdef SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
										private: bool solidifyNeuralNetBackpropGroupLastComponent(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupParseTree* currentParseTreeGroup, int level);
										#endif
										#endif
										
										#ifdef SANI_PREVIOUS_WORD_POS_TYPE_CHECKS
										private: bool componentTests2(SANIGroupNeuralNetwork* group, SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted);
											private: bool findPreviousWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int previousWordPOStype);
											private: bool findPreceedingWordInSentence(vector<GIApreprocessorPlainTextWord*>* sentenceContents, int currentWordIndex, int existsPreceedingWordPOStype);
										#endif
										
										#ifdef SANI_LIGHT_OPTIMISED_PREPROCESS_DONT_OVERWRITE_REFERENCESETS_THAT_CAPTURES_EOS
										private: bool resetAllowed(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool firstActiveComponentInGroup, SANIComponentNeuralNetwork* ownerComponent);
											private: bool existingActivatedComponentCapturesLastWordInSentence(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool firstActiveComponentInGroup);
												private: bool existingActivatedComponentCapturesLastWordInSentence(SANIGroupParseTree* prospectiveNewlyActiveComponentInParseTreeParseTreeGroupRef, SANIGroupParseTree* lastActiveComponentInParseTreeParseTreeGroupRef, SANIComponentParseTree* lastActiveComponentInParseTree, SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
										#endif

										#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_CONTAINS_EXISTING_WORDS_IN_PARSETREE
										private: bool verifyNewActivationParseTreeContainsExistingActivationWordsInParseTree(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationWordData* forwardPropogationWordData, const bool existingActivationFoundStartComponent);
											private: bool getWordIndicesInParseTree(SANIGroupParseTree* currentParseTreeGroup, vector<int>* wordIndexList, int level);
										#endif
	
										#ifdef SANI_LIGHT_OPTIMISED_ALLOW_MULTIPLE_ACTIVATIONS_VERIFY_MAX_NUMBER_OF_WORDS_IN_PARSETREE
										private: bool existingActivationVerifyMaxNumberWordsInParseTree(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, const bool existingActivationFoundStartComponent);
											private: int countNumberWordsInParseTree(SANIGroupParseTree* currentParseTreeGroup, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
										#endif
										
										#ifdef SANI_VERIFY_OWNER_GROUP_DOESNT_CONTAIN_NEWLY_ACTIVATED_COMPONENT_IN_ITS_PARSE_TREE
										private: bool verifyOwnerParseTreeGroupDoesntContainNewActivationComponentParseTree(SANIGroupNeuralNetwork* ownerGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, int layer);
											private: bool parseTreeContainsGroup(SANIGroupParseTree* currentParseTreeGroup, SANIGroupParseTree* parseTreeGroupToFind, int level);
										#endif

										private: void updateParseTreeMaxMinWordIndexOfParent(SANIGroupParseTree* parentGroup, SANIComponentParseTree* parentComponent, SANIGroupParseTree* childGroup);
										
	private: bool printBackpropParseTree(SANIGroupParseTree* group, int level);
	private: bool groupActivationCompleted(SANIGroupNeuralNetwork* group);
	
	#ifdef SANI_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
	public: bool demarkateOptimumPathwayBackprop(SANIGroupParseTree* currentParseTreeGroup);
	private: bool isWordInGroupOptimumPathwayWordList(SANIForwardPropogationSentenceData* forwardPropogationSentenceData, SANIComponentNeuralNetwork* currentComponent, SANIForwardPropogationWordData* forwardPropogationWordData);
	#endif



};

#endif

#endif
