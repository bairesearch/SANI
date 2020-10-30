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
 * File Name: SANIformation.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1n5d 29-October-2020
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Formation
 * /
 *******************************************************************************/


#ifndef HEADER_SANI_FORMATION
#define HEADER_SANI_FORMATION

#include "SHAREDglobalDefs.hpp"
#ifdef USE_GIA
#include "GIAglobalDefs.hpp"
#else
#include "SANIglobalDefs.hpp"
#endif
#include "SHAREDvars.hpp"
#include "XMLparserClass.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
#include "SANIrules.hpp"
#endif
#include "SANInodesGroupClass.hpp"
#include "SANInodesComponentClass.hpp"
#include "SANInodes.hpp"
#include "LRPpreprocessorSentenceClass.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "ANNdisplay.hpp"

#ifdef SANI_FORWARD

class SANIformationClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: XMLparserClassClass XMLparserClass;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: SANInodesGroupClass SANInodesGroupClassObject;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	private: SANIrulesClass SANIrules;
	#endif
	private: SANInodesClass SANInodes;
	private: ANNdisplayClass ANNdisplay;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	public: SANIGroupNeuralNetwork* getFirstGroupInInputLayerSectionWordOrig();
	#else
	#ifdef SANI_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	public: vector<string>* getExplicitWordList();
	#endif
	public: unordered_map<string, SANIGroupNeuralNetwork*>* getInputLayerSectionExplicitWordMap();
	public: unordered_map<string, SANIGroupNeuralNetwork*>* getInputLayerSectionTokensLayerMap();
	public: SANIGroupNeuralNetwork* getFirstGroupInInputLayerSectionWordPOStype();
	#endif
	#ifdef SANI_NEURAL_NETWORK
	public: SANIGroupNeuralNetwork* getFirstInputGroupInNetwork();
	#endif

	public: bool createSANI(vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes);
		private: bool createInputNeuronLayer(vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes);
			#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
			private: bool createInputNeuronLayerSectionWordOrig(SANIGroupNeuralNetwork** currentGroupInInputLayerSectionWordOrig, int* numberOfGroupsInSection);
			public: bool addInputNeuronLayerSectionWordOrig(LRPpreprocessorPlainTextWord* currentWord, SANIGroupNeuralNetwork** currentGroupInInputLayerSection);
			public: bool findInputNeuronLayerSectionWordOrig(LRPpreprocessorPlainTextWord* currentWord, SANIGroupNeuralNetwork** currentGroupInInputLayerSection);
			#else
			private: bool createInputNeuronLayerSectionWordPOStype(SANIGroupNeuralNetwork** currentGroupInInputLayerSectionWordPOStype, int* numberOfGroupsInSection);
			#ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY	
				#ifdef SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
				private: bool createInputNeuronLayerSectionNounVariantType(SANIGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, SANIGroupNeuralNetwork** currentGroupInInputLayerSectionWordNounVariantType, int* numberOfGroupsInSection);
				private: bool createInputNeuronLayerSectionVerbVariantType(SANIGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype, SANIGroupNeuralNetwork** currentGroupInInputLayerSectionWordVerbVariantType, int* numberOfGroupsInSection);
				#endif
			private: bool createInputNeuronLayerSectionExplicitWord(SANIGroupNeuralNetwork** currentGroupInInputLayerSection, int* numberOfGroupsInSection, vector<SANIGroupType*>* SANIGroupTypes);	
				private: bool createInputNeuronLayerSectionExplicitWord(vector<SANIComponentNeuralNetwork*>* components, bool subcomponents, vector<string>* explicitWordList);	
			private: bool createInputNeuronLayerSectionTokensLayer(vector<XMLparserTag*>* SANIrulesTokenLayers, SANIGroupNeuralNetwork** currentGroupInInputLayerSectionTokensLayer, int* numberOfGroupsInSectionTokensLayer);
			#endif
			#endif
	#ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
		private: bool createNeuronLayerIntro(vector<XMLparserTag*>* SANIrulesTokenLayers, vector<SANIGroupType*>* SANIGroupTypes);
			private: bool createNeuronLayerGroupType(vector<XMLparserTag*>* SANIrulesTokenLayers, SANIComponentNeuralNetwork* higherLevelComponent, SANIGroupType* groupType);
				private: bool createNeuronLayerGroup(vector<XMLparserTag*>* SANIrulesTokenLayers, SANIGroupNeuralNetwork* group);
					private: bool createNeuronLayerComponents(vector<XMLparserTag*>* SANIrulesTokenLayers, SANIGroupNeuralNetwork* group, vector<SANIComponentNeuralNetwork*>* components, bool subcomponents, SANIComponentNeuralNetwork* higherLevelComponent);
		
		
	private: bool findTokensLayerClassType(string layerName, string layerClassName, string layerClassTypeName, SANIGroupNeuralNetwork** groupFound);		
		private: bool findTokensLayerClass(string layerName, string layerClassName, SANIGroupNeuralNetwork** layerClassGroupFound);
	#endif
	private: bool findWordInList(const string word, vector<string>* explicitWordList);
	public: bool findWordInGroupMap(const string word, unordered_map<string, SANIGroupNeuralNetwork*>* wordMap, SANIGroupNeuralNetwork** groupFound);
	public: SANIGroupNeuralNetwork* getInputGroupLayerSection(SANIGroupNeuralNetwork* firstGroupInInputLayerSection, int groupIndexInSection);
	private: void addGroupToLayer(SANIGroupNeuralNetwork** currentGroupInLayer, int* numberOfGroupsInSection);
	#ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	private: bool createGroupANNconnectionIO(SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelGroup);
	private: bool createGroupANNconnectionIObasic(SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelGroup);
	#endif
	public: bool createGroupANNconnection(SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* higherLevelComponent);
			#ifdef SANI_ANN
			public: ANNneuronConnection* createANNconnection(SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* higherLevelComponent);
				public: ANNneuronConnection* createANNconnection(SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* higherLevelComponent, const double connectionStrength);
					private: ANNneuronConnection* createANNconnection(SANIGroupNeuralNetwork* group, SANIGroupNeuralNetwork* higherLevelGroup);
			#ifdef SANI_SEQUENCE_GRAMMAR
			public: void deleteANNconnections(SANIGroupNeuralNetwork* group, SANIComponentNeuralNetwork* component);
			#endif
			#ifdef SANI_ANN_DELAY_ANN_CONNECTIVITY_TILL_END
			public: bool createANNconnectivity(vector<SANIGroupType*>* SANIGroupTypes);
			#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
			public: bool createANNconnectivityReset(vector<SANIGroupType*>* SANIGroupTypes);
			#endif
			#endif
			#endif
};

#endif

#endif
