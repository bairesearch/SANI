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
 * File Name: SANIpropagateHeavyUnoptimised.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1o5c 25-February-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Heavy Unoptimised - ~O(n^2)
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_HEAVY_UNOPTIMISED
#define HEADER_SANI_PROPAGATE_HEAVY_UNOPTIMISED

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
#include "ANNdisplay.hpp"



#ifdef SANI_HEAVY
#ifdef SANI_HEAVY_UNOPTIMISED

class SANIpropagateHeavyUnoptimisedClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorPOStaggerClass LRPpreprocessorPOStagger;
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: SANInodesClass SANInodes;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	private: SANInodesComponentClass SANInodesComponentClassObject;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	private: SANIpropagateOperationsParseTreeClass SANIpropagateOperationsParseTree;
	private: SANIformationClass SANIformation;
	#ifdef SANI_PARSE
	private: GIAposRelTranslatorParserOperationsClass GIAposRelTranslatorParserOperations;
	#endif
	private: ANNdisplayClass ANNdisplay;

	public: bool executePosRelTranslatorNeuralNetwork(const SANItranslatorVariablesClass* translatorVariables, vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
		private: bool propagateWordThroughNetworkIntro(const SANItranslatorVariablesClass* translatorVariables, int w, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup, const bool expectToSeeConnectionWithPreviousWordTrace, const bool passThrough);
			private: bool propagateWordThroughNetworkGroupInit(const SANItranslatorVariablesClass* translatorVariables, int w, int wordPOStype, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup, const bool passThrough);
				private: bool propagateWordThroughNetworkGroupInit(const SANItranslatorVariablesClass* translatorVariables, int w, SANIGroupNeuralNetwork* inputLayerGroup, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup, const bool passThrough);
					private: bool propagateWordThroughNetworkGroupIntro(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordFirstParseTreeGroup, const bool passThrough);
						private: bool propagateWordThroughNetworkGroup(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool passThrough);
							private: bool propagateWordThroughNetworkGroupSelect(const SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, const bool passThrough);
								private: bool propagateWordThroughNetworkGroupComponentWrapper(const SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, const SANIposRelTranslatorDebug* debug);	
									private: bool propagateWordThroughNetworkGroupComponent(const SANItranslatorVariablesClass* translatorVariables, const SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* currentComponent, const SANIGroupNeuralNetwork* ownerGroup, SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough, const SANIposRelTranslatorDebug* debug);
										#ifdef SANI_SOLIDIFY_NET_BACKPROP
										#ifdef SANI_SOLIDIFY_NET_BACKPROP_AFTER_ACTIVATING_INDIVIDUAL_COMPONENTS
										private: bool solidifyNeuralNetBackpropGroupLastComponent(SANIGroupParseTree* currentParseTreeGroup, const int level);
										#endif
										#endif
										//private: bool findComponentRef(SANIGroupParseTree* currentParseTreeGroup, SANIComponentNeuralNetwork* componentRef, SANIComponentNeuralNetwork** componentFound);
										private: bool propagateWordThroughNetworkGroupComponentPassNextWord(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* ownerGroupOrig, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const bool lastComponentFuzzyAndCompatible, const bool passThrough);


	private: bool printBackpropParseTree(SANIGroupParseTree* group, const int level);
	private: bool groupActivationCompleted(const SANIGroupNeuralNetwork* group);

	#ifdef SANI_SUPPORT_TRAILING_OPTIONAL_COMPONENTS_AND_SUBCOMPONENTS_DYNAMIC_RESET_INCLUDE_PARTIALLY_OR_FULLY_ACTIVATED_GROUPS
	private: bool groupPartiallyOrFullyActivated(const SANIGroupNeuralNetwork* group);
	#endif

	/*
	#ifdef SANI_SAVE_MEMORY_GROUPS
	private: SANIGroupNeuralNetwork* copyGroup(SANIGroupNeuralNetwork* group);
	#endif
	*/
};

#endif
#endif

#endif
