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
 * File Name: SANIpropagateHeavyOptimised.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p7b 24-March-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Propagate Heavy Optimised - ~O(nlogn)
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_PROPAGATE_HEAVY_OPTIMISED
#define HEADER_SANI_PROPAGATE_HEAVY_OPTIMISED

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
#ifdef SANI_HEAVY_OPTIMISED

class SANIpropagateHeavyOptimisedClass
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

	public: bool executePosRelTranslatorNeuralNetwork(const SANItranslatorVariablesClass* translatorVariables, constEffective vector<SANIGroupType*>* SANIGroupTypes, vector<LRPpreprocessorPlainTextWord*>* sentenceContents, SANIGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance);
		private: bool propagateWordThroughNetworkWordGroupIntro(const SANItranslatorVariablesClass* translatorVariables, int w, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
			private: bool propagateWordThroughNetworkWordGroupInit(const SANItranslatorVariablesClass* translatorVariables, int w, int wordPOStype, const SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);
				private: bool propagateWordThroughNetworkWordGroupInit(const SANItranslatorVariablesClass* translatorVariables, int w, SANIGroupNeuralNetwork* inputLayerGroup, const SANIForwardPropogationSignalData* forwardPropogationSignalData, const SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const int layer);
		private: bool propagateWordThroughNetworkGroupIntro(const SANItranslatorVariablesClass* translatorVariables, SANIForwardPropogationSentenceData* forwardPropogationSentenceData);
			private: bool connectToPreviousActivationGroup(const SANItranslatorVariablesClass* translatorVariables, SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroup, SANIGroupActivationMemory* activationPathWordFirstActivationMemoryGroupNext, const SANIForwardPropogationActivationPointData* forwardPropogationActivationPointData, const SANIForwardPropogationActivationPointData* forwardPropogationActivationPointDataNext, SANIForwardPropogationSignalData* forwardPropogationSignalDataNext, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationWordData* forwardPropogationWordDataNext, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer);		
			private: bool propagateWordThroughNetworkGroup(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* group, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, constEffective SANIGroupParseTree* activationPathWordCurrentParseTreeGroup);
				private: bool propagateWordThroughNetworkGroupSelect(const SANItranslatorVariablesClass* translatorVariables, SANIGroupNeuralNetwork* ownerGroup, constEffective SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* currentComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, constEffective SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner);
					private: bool propagateWordThroughNetworkGroupComponentWrapper(const SANItranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, int layer, constEffective SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const SANIposRelTranslatorDebug* debug);	
						private: bool propagateWordThroughNetworkGroupComponent(const SANItranslatorVariablesClass* translatorVariables, SANIComponentNeuralNetwork* currentComponent, SANIGroupNeuralNetwork* ownerGroup, const SANIGroupNeuralNetwork* ownerGroupOrig, SANIComponentNeuralNetwork* ownerComponent, SANIForwardPropogationSignalData* forwardPropogationSignalData, SANIForwardPropogationWordData* forwardPropogationWordData, SANIForwardPropogationSentenceData* forwardPropogationSentenceData, const bool activationSequenceCompleted, int layer, const bool repeatDetected, constEffective SANIGroupParseTree* activationPathWordCurrentParseTreeGroup, SANIGroupParseTree* activationPathWordCurrentParseTreeGroupOwner, const SANIposRelTranslatorDebug* debug);
			
	private: bool printBackpropParseTree(SANIGroupParseTree* group, const int level);

	private: void generateMemoryActiveGroup(SANIGroupNeuralNetwork* activationMemoryGroup, constEffective SANIGroupNeuralNetwork* groupOrig, SANIGroupNeuralNetwork* currentParseTreeGroupTemplate, SANIGroupActivationMemory** activationMemoryGroupNew, SANIGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents);
	private: void generateMemoryActiveGroup(SANIGroupNeuralNetwork* activationMemoryGroup, constEffective SANIGroupNeuralNetwork* groupOrig, SANIGroupParseTree* currentParseTreeGroup, SANIGroupActivationMemory** activationMemoryGroupNew, SANIGroupParseTree** currentParseTreeGroupNew, const bool copyParseTreeGroupComponents);

	private: void printComponents(const vector<SANIComponentNeuralNetwork*>* components);

};

#endif
#endif

#endif
