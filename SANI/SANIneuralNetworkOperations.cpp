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
 * File Name: SANIneuralNetworkOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p5a 19-March-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Neural Network Operations
 * /
 *******************************************************************************/


#include "SANIneuralNetworkOperations.hpp"


		

#ifdef SANI_NEURAL_NETWORK


bool SANIneuralNetworkOperationsClass::readNeuralNetXMLfile(const string xmlFileName, ANNneuron* firstInputNeuronInNetwork)
{
	bool result = false;

	int64_t numberOfInputNeuronsLoaded;
	int64_t numberOfOutputNeuronsLoaded;
	int64_t numberOfLayersLoaded;

	ANNneuron* firstOutputNeuronInNetwork = ANNxmlConversion.readNetXMLfileAndRecordFormationVariables(xmlFileName, firstInputNeuronInNetwork, &numberOfInputNeuronsLoaded, &numberOfOutputNeuronsLoaded, &numberOfLayersLoaded);

	return result;
}

bool SANIneuralNetworkOperationsClass::writeNeuralNetXMLfile(string xmlFileName, ANNneuron* firstInputNeuronInNetwork)
{
	bool result = true;

	if(!ANNxmlConversion.writeNetXMLfile(xmlFileName, firstInputNeuronInNetwork))
	{
		result = false;
	}
		
	return result;
}




#ifdef SANI_ANN

#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
static bool alreadyCoordinatedInputNeurons;
static ANNneuron* firstOutputNeuronInNetworkPreRecord;
static int64_t idBaseRecord;
void SANIneuralNetworkOperationsClass::initialiseIncrementalGeneration()
{
	alreadyCoordinatedInputNeurons = false;
	firstOutputNeuronInNetworkPreRecord = NULL;
	idBaseRecord = 0;
}
/*
bool SANIneuralNetworkOperationsClass::generateNeuralNetFromSANInetReset(ANNtranslatorVariablesClass* translatorVariables)
{
	vector<SANIGroupType*>* SANIGroupTypes = SANInodes.getSANIGroupTypesGlobal();
	determinePositonsOfNeuronsReset(SANIGroupTypes);
}
*/
#endif

bool SANIneuralNetworkOperationsClass::generateNeuralNetFromSANInet(ANNtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	vector<SANIGroupType*>* SANIGroupTypes = SANInodes.getSANIGroupTypesGlobal();
	
	SANIGroupNeuralNetwork* firstInputGroupInNetwork = SANIformation.getFirstInputGroupInNetwork();
	ANNneuron* firstInputNeuronInNetwork = firstInputGroupInNetwork->neuronReference;
	
	int64_t id = 0;
	ANNneuron* firstOutputNeuronInNetworkPre = NULL;
	ANNneuron* firstOutputNeuronInNetworkPost = NULL;
	
	#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
	if(!alreadyCoordinatedInputNeurons)
	{
		determinePositonsOfInputNeurons(&id, &firstOutputNeuronInNetworkPre);	
		alreadyCoordinatedInputNeurons = true;
		firstOutputNeuronInNetworkPreRecord = firstOutputNeuronInNetworkPre;
		idBaseRecord = id;
	}
	else
	{
		firstOutputNeuronInNetworkPre = firstOutputNeuronInNetworkPreRecord;
		id = idBaseRecord;
		
		firstOutputNeuronInNetworkPre->hasFrontLayer = false;
		firstOutputNeuronInNetworkPre->firstNeuronInFrontLayer = NULL;
		
		determinePositonsOfNeuronsReset(SANIGroupTypes);
	}
	#else
	determinePositonsOfInputNeurons(&id, &firstOutputNeuronInNetworkPre);
	#endif
	
	determinePositonsOfNeurons(SANIGroupTypes, &id, firstOutputNeuronInNetworkPre, &firstOutputNeuronInNetworkPost);
	
	//cout << "determinePositonsOfNeurons done" << endl;
	
	#ifndef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
	delete (translatorVariables->firstInputNeuronInNetwork);	//delete the ANNneuron object created by GIAmain.cpp
	#endif
	
	translatorVariables->firstInputNeuronInNetwork = firstInputNeuronInNetwork;
	translatorVariables->firstOutputNeuronInNetwork = firstOutputNeuronInNetworkPost;

	//cout << "SANIneuralNetworkOperationsClass::generateNeuralNetFromSANInet done" << endl;
	
	return result;	
}

bool SANIneuralNetworkOperationsClass::determinePositonsOfInputNeurons(int64_t* idBase, ANNneuron** firstOutputNeuronInNetworkPre)
{
	bool result = true;	

	int64_t id = *idBase;
	
	int x1flatAbsolute = 0;
	int x2flatAbsolute = 0;
	int x3flatAbsolute = 0;
	int x4flatAbsolute = 0;
		
	vector<SANIGroupType*> SANIGroupTypesArtificial;
	SANIGroupType SANIGroupTypeArtificialSectionWordPOStype;
	SANIGroupType SANIGroupTypeArtificialSectionExplicitWordMap;
	SANIGroupType SANIGroupTypeArtificialSectionTokensLayerMap;
	SANIGroupTypeArtificialSectionWordPOStype.groupTypeXindex = 0;
	SANIGroupTypeArtificialSectionExplicitWordMap.groupTypeXindex = 1;
	SANIGroupTypeArtificialSectionTokensLayerMap.groupTypeXindex = 2;
	SANIGroupTypesArtificial.push_back(&SANIGroupTypeArtificialSectionWordPOStype);
	SANIGroupTypesArtificial.push_back(&SANIGroupTypeArtificialSectionExplicitWordMap);
	SANIGroupTypesArtificial.push_back(&SANIGroupTypeArtificialSectionTokensLayerMap);
	
	SANIGroupType* groupType = &SANIGroupTypeArtificialSectionWordPOStype;
	ANNneuron* currentGroupNeuronInLayer1 = NULL;
	ANNneuron* previousGroupNeuronInLayer1 = NULL;
	ANNneuron* currentGroupNeuronInLayer2 = NULL;
	ANNneuron* previousGroupNeuronInLayer2 = NULL;
	ANNneuron* currentGroupNeuronInLayer3 = NULL;
	ANNneuron* previousGroupNeuronInLayer3 = NULL;
	ANNneuron* currentGroupNeuronInLayer4 = NULL;
	ANNneuron* previousGroupNeuronInLayer4 = NULL;
	
	ANNneuron* firstNeuronInLayer1 = NULL;
	ANNneuron* firstNeuronInLayer2 = NULL;
	ANNneuron* firstNeuronInLayer3 = NULL;
	ANNneuron* firstNeuronInLayer4 = NULL;
	
	int x = 0;
	int x2flat = 0;
	int maxX = 0;
	#ifdef SANI_SEQUENCE_GRAMMAR_INPUT_WORDS
	SANIGroupNeuralNetwork* firstGroupInInputLayerSectionWordPOStype = SANIformation.getFirstGroupInInputLayerSectionWordOrig();	
	#else
	SANIGroupNeuralNetwork* firstGroupInInputLayerSectionWordPOStype = SANIformation.getFirstGroupInInputLayerSectionWordPOStype();
	#endif
	SANIGroupNeuralNetwork* currentGroupInInputLayerSectionWordPOStype = firstGroupInInputLayerSectionWordPOStype;
	while(currentGroupInInputLayerSectionWordPOStype->next != NULL)
	{		
		SANIGroupNeuralNetwork* group = currentGroupInInputLayerSectionWordPOStype;
		currentGroupNeuronInLayer1 = group->neuronReference;
		if(x1flatAbsolute != 0)
		{
			previousGroupNeuronInLayer1->nextNeuron = currentGroupNeuronInLayer1;
		}
		else
		{
			firstNeuronInLayer1 = currentGroupNeuronInLayer1;
		}
		
		group->neuronDisplayPositionSet = true;
		group->neuronDisplayPositionX = x;
		group->neuronDisplayPositionY = 0;
		groupType->neuronDisplayPositionX = SANI_ANN_GROUPTYPE_ARTIFICIAL_INPUT_POSITION_X_WORDPOSTYPE;
		groupType->neuronDisplayPositionY = SANI_ANN_GROUPTYPE_ARTIFICIAL_INPUT_POSITION_Y;
		#ifdef SANI_ANN_CALCULATE_BOUNDING_BOXES
		int groupTypeXposAbsolute = maxX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X + groupType->groupTypeXindex*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
		#else
		int groupTypeXposAbsolute = groupType->neuronDisplayPositionX*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
		#endif
		group->neuronReference->xPosRel = groupTypeXposAbsolute + group->neuronDisplayPositionX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X;
		group->neuronReference->yPosRel = groupType->neuronDisplayPositionY*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_Y + group->neuronDisplayPositionY*SANI_ANN_GROUP_ARTIFICIAL_SPACING_Y;
		group->neuronReference->id = id;
		id++;
		
		#ifdef SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
		int x2 = 0;
		for(int i2=0; i2<group->SANIfrontGroupConnectionList.size(); i2++)
		{			
			SANIGroupNeuralNetwork* higherLevelGroup = (group->SANIfrontGroupConnectionList)[i2];
			SANIGroupNeuralNetwork* group = higherLevelGroup;
			currentGroupNeuronInLayer2 = group->neuronReference;
			if(x2flatAbsolute != 0)
			{
				previousGroupNeuronInLayer2->nextNeuron = currentGroupNeuronInLayer2;
			}
			else
			{
				firstNeuronInLayer2 = currentGroupNeuronInLayer2;
			}
		
			group->neuronDisplayPositionSet = true;
			group->neuronDisplayPositionX = x2flat;
			group->neuronDisplayPositionY = SANI_ANN_GROUP_ARTIFICIAL_INPUT_POSITION_Y_WORDPOSTYPE_NOUN_VERB_VARIANTS;
			#ifdef SANI_ANN_CALCULATE_BOUNDING_BOXES
			int groupTypeXposAbsolute = maxX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X + groupType->groupTypeXindex*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
			#else
			int groupTypeXposAbsolute = groupType->neuronDisplayPositionX*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
			#endif
			group->neuronReference->xPosRel = groupTypeXposAbsolute + group->neuronDisplayPositionX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X;
			group->neuronReference->yPosRel = groupType->neuronDisplayPositionY*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_Y + group->neuronDisplayPositionY*SANI_ANN_GROUP_ARTIFICIAL_SPACING_Y;
			group->neuronReference->id = id;
			id++;
		
			previousGroupNeuronInLayer2 = currentGroupNeuronInLayer2;
			x2++;
			x2flat++;
			x2flatAbsolute++;
		}
		#endif
 
		previousGroupNeuronInLayer1 = currentGroupNeuronInLayer1;
			
		x++;
		x1flatAbsolute++;
		
		currentGroupInInputLayerSectionWordPOStype = currentGroupInInputLayerSectionWordPOStype->next;
	}
	maxX = x1flatAbsolute;
	
	#ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	groupType = &SANIGroupTypeArtificialSectionExplicitWordMap;
	x = 0;
	unordered_map<string,SANIGroupNeuralNetwork*>* inputLayerSectionExplicitWordMap = SANIformation.getInputLayerSectionExplicitWordMap();
	for(unordered_map<string,SANIGroupNeuralNetwork*>::iterator iter1 = inputLayerSectionExplicitWordMap->begin(); iter1 != inputLayerSectionExplicitWordMap->end(); iter1++)
	{
		SANIGroupNeuralNetwork* currentGroupInExplicitWordMap = iter1->second;
		SANIGroupNeuralNetwork* group = currentGroupInExplicitWordMap;
		currentGroupNeuronInLayer1 = group->neuronReference;
		if(currentGroupNeuronInLayer1->nextNeuron != NULL)
		{
			//cout << "currentGroupNeuronInLayer1->id = " << currentGroupNeuronInLayer1->id << endl;
		}
		if(x1flatAbsolute != 0)
		{
			previousGroupNeuronInLayer1->nextNeuron = currentGroupNeuronInLayer1;
		}
		
		group->neuronDisplayPositionSet = true;
		group->neuronDisplayPositionX = x;
		group->neuronDisplayPositionY = 0;
		groupType->neuronDisplayPositionX = SANI_ANN_GROUPTYPE_ARTIFICIAL_INPUT_POSITION_X_EXPLICITWORD;
		groupType->neuronDisplayPositionY = SANI_ANN_GROUPTYPE_ARTIFICIAL_INPUT_POSITION_Y;
		#ifdef SANI_ANN_CALCULATE_BOUNDING_BOXES
		int groupTypeXposAbsolute = maxX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X + groupType->groupTypeXindex*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
		#else
		int groupTypeXposAbsolute = groupType->neuronDisplayPositionX*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
		#endif
		group->neuronReference->xPosRel = groupTypeXposAbsolute + group->neuronDisplayPositionX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X;
		group->neuronReference->yPosRel = groupType->neuronDisplayPositionY*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_Y + group->neuronDisplayPositionY*SANI_ANN_GROUP_ARTIFICIAL_SPACING_Y;
		group->neuronReference->id = id;
		id++;
			
		previousGroupNeuronInLayer1 = currentGroupNeuronInLayer1;
		x++;
		x1flatAbsolute++;
	}
	maxX = x1flatAbsolute;
	
	groupType = &SANIGroupTypeArtificialSectionTokensLayerMap;	
	int x1 = 0;
	int x1flat = 0;
	x2flat = 0;
	int x3flat = 0;
	int x4flat = 0;
	unordered_map<string,SANIGroupNeuralNetwork*>* inputLayerSectionTokensLayerMap = SANIformation.getInputLayerSectionTokensLayerMap();
	for(unordered_map<string,SANIGroupNeuralNetwork*>::iterator iter1 = inputLayerSectionTokensLayerMap->begin(); iter1 != inputLayerSectionTokensLayerMap->end(); iter1++)
	{
		SANIGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayer = iter1->second;
		SANIGroupNeuralNetwork* group = currentGroupInInputLayerSectionTokensLayer;
		currentGroupNeuronInLayer4 = group->neuronReference;
		if(x4flatAbsolute != 0)
		{
			previousGroupNeuronInLayer4->nextNeuron = currentGroupNeuronInLayer4;
		}
		else
		{
			firstNeuronInLayer4 = currentGroupNeuronInLayer4;
		}
		
		group->neuronDisplayPositionSet = true;
		group->neuronDisplayPositionX = x1flat;
		group->neuronDisplayPositionY = SANI_ANN_GROUP_ARTIFICIAL_INPUT_POSITION_Y_EXPLICITTOKENSLAYER_LAYER;
		groupType->neuronDisplayPositionX = SANI_ANN_GROUPTYPE_ARTIFICIAL_INPUT_POSITION_X_EXPLICITTOKENSLAYER;
		groupType->neuronDisplayPositionY = SANI_ANN_GROUPTYPE_ARTIFICIAL_INPUT_POSITION_Y;
		#ifdef SANI_ANN_CALCULATE_BOUNDING_BOXES
		int groupTypeXposAbsolute = maxX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X + groupType->groupTypeXindex*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
		#else
		int groupTypeXposAbsolute = groupType->neuronDisplayPositionX*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
		#endif
		group->neuronReference->xPosRel = groupTypeXposAbsolute + group->neuronDisplayPositionX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X;
		group->neuronReference->yPosRel = groupType->neuronDisplayPositionY*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_Y + group->neuronDisplayPositionY*SANI_ANN_GROUP_ARTIFICIAL_SPACING_Y;
		group->neuronReference->id = id;
		id++;
		
		int x2 = 0;
		for(int i=0; i<currentGroupInInputLayerSectionTokensLayer->SANIbackGroupConnectionList.size(); i++)
		{
			SANIGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClass = (currentGroupInInputLayerSectionTokensLayer->SANIbackGroupConnectionList)[i];
			SANIGroupNeuralNetwork* group = currentGroupInInputLayerSectionTokensLayerClass;
			currentGroupNeuronInLayer3 = group->neuronReference;
			if(x3flatAbsolute != 0)
			{
				previousGroupNeuronInLayer3->nextNeuron = currentGroupNeuronInLayer3;
			}
			else
			{
				firstNeuronInLayer3 = currentGroupNeuronInLayer3;
			}
			
			group->neuronDisplayPositionSet = true;
			group->neuronDisplayPositionX = x2flat;
			group->neuronDisplayPositionY = SANI_ANN_GROUP_ARTIFICIAL_INPUT_POSITION_Y_EXPLICITTOKENSLAYER_LAYER_CLASS;
			#ifdef SANI_ANN_CALCULATE_BOUNDING_BOXES
			int groupTypeXposAbsolute = maxX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X + groupType->groupTypeXindex*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
			#else
			int groupTypeXposAbsolute = groupType->neuronDisplayPositionX*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
			#endif
			group->neuronReference->xPosRel = groupTypeXposAbsolute + group->neuronDisplayPositionX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X;
			group->neuronReference->yPosRel = groupType->neuronDisplayPositionY*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_Y + group->neuronDisplayPositionY*SANI_ANN_GROUP_ARTIFICIAL_SPACING_Y;
			group->neuronReference->id = id;
			id++;
		
			int x3 = 0;
			for(int i=0; i<currentGroupInInputLayerSectionTokensLayerClass->SANIbackGroupConnectionList.size(); i++)
			{
				SANIGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassType = (currentGroupInInputLayerSectionTokensLayerClass->SANIbackGroupConnectionList)[i];
				SANIGroupNeuralNetwork* group = currentGroupInInputLayerSectionTokensLayerClassType;
				currentGroupNeuronInLayer2 = group->neuronReference;
				if(x2flatAbsolute != 0)
				{
					previousGroupNeuronInLayer2->nextNeuron = currentGroupNeuronInLayer2;
				}
				#ifndef SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
				else
				{
					firstNeuronInLayer2 = currentGroupNeuronInLayer2;
				}
				#endif
				
				group->neuronDisplayPositionSet = true;
				group->neuronDisplayPositionX = x3flat;
				group->neuronDisplayPositionY = SANI_ANN_GROUP_ARTIFICIAL_INPUT_POSITION_Y_EXPLICITTOKENSLAYER_LAYER_CLASS_TYPE;
				#ifdef SANI_ANN_CALCULATE_BOUNDING_BOXES
				int groupTypeXposAbsolute = maxX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X + groupType->groupTypeXindex*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
				#else
				int groupTypeXposAbsolute = groupType->neuronDisplayPositionX*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
				#endif
				group->neuronReference->xPosRel = groupTypeXposAbsolute + group->neuronDisplayPositionX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X;
				group->neuronReference->yPosRel = groupType->neuronDisplayPositionY*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_Y + group->neuronDisplayPositionY*SANI_ANN_GROUP_ARTIFICIAL_SPACING_Y;
				group->neuronReference->id = id;
				id++;
		
				int x4 = 0;
				for(int i=0; i<currentGroupInInputLayerSectionTokensLayerClassType->SANIbackGroupConnectionList.size(); i++)
				{
					SANIGroupNeuralNetwork* currentGroupInInputLayerSectionTokensLayerClassTypeInstance = (currentGroupInInputLayerSectionTokensLayerClassType->SANIbackGroupConnectionList)[i];
					SANIGroupNeuralNetwork* group = currentGroupInInputLayerSectionTokensLayerClassTypeInstance;
					currentGroupNeuronInLayer1 = group->neuronReference;
					if(x1flatAbsolute != 0)
					{
						previousGroupNeuronInLayer1->nextNeuron = currentGroupNeuronInLayer1;
					}
					
					group->neuronDisplayPositionSet = true;
					group->neuronDisplayPositionX = x4flat;
					group->neuronDisplayPositionY = SANI_ANN_GROUP_ARTIFICIAL_INPUT_POSITION_Y_EXPLICITTOKENSLAYER_LAYER_CLASS_TYPE_INSTANCE;
					#ifdef SANI_ANN_CALCULATE_BOUNDING_BOXES
					int groupTypeXposAbsolute = maxX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X + groupType->groupTypeXindex*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
					#else
					int groupTypeXposAbsolute = groupType->neuronDisplayPositionX*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_X;
					#endif
					group->neuronReference->xPosRel = groupTypeXposAbsolute + group->neuronDisplayPositionX*SANI_ANN_GROUP_ARTIFICIAL_SPACING_X;
					group->neuronReference->yPosRel = groupType->neuronDisplayPositionY*SANI_ANN_GROUPTYPE_ARTIFICIAL_SPACING_Y + group->neuronDisplayPositionY*SANI_ANN_GROUP_ARTIFICIAL_SPACING_Y;
					group->neuronReference->id = id;
					id++;
		
					previousGroupNeuronInLayer1 = currentGroupNeuronInLayer1;
					x4++;
					x4flat++;
					x1flatAbsolute++;
				}
				
				previousGroupNeuronInLayer2 = currentGroupNeuronInLayer2;
				x3++;
				x3flat++;
				x2flatAbsolute++;
			}
			
			previousGroupNeuronInLayer3 = currentGroupNeuronInLayer3;
			x2++;
			x2flat++;
			x3flatAbsolute++;
		}
		
		previousGroupNeuronInLayer4 = currentGroupNeuronInLayer4;
		x1++;
		x1flat++;
		x4flatAbsolute++;
	}	
	#endif
	
	#ifdef SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
	firstNeuronInLayer1->hasFrontLayer = true;
	firstNeuronInLayer1->firstNeuronInFrontLayer = firstNeuronInLayer2;
	#endif
	#ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	firstNeuronInLayer2->hasFrontLayer = true;
	firstNeuronInLayer2->firstNeuronInFrontLayer = firstNeuronInLayer3;
	firstNeuronInLayer3->hasFrontLayer = true;
	firstNeuronInLayer3->firstNeuronInFrontLayer = firstNeuronInLayer4;
	#endif
	
	currentGroupNeuronInLayer1->nextNeuron = new ANNneuron();	//create a null neuron at end of layer
	#ifdef SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
	currentGroupNeuronInLayer2->nextNeuron = new ANNneuron();	//create a null neuron at end of layer
	#endif
	#ifndef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	currentGroupNeuronInLayer3->nextNeuron = new ANNneuron();	//create a null neuron at end of layer
	currentGroupNeuronInLayer4->nextNeuron = new ANNneuron();	//create a null neuron at end of layer
	#endif
	
	#ifdef SANI_SIMPLE_WORD_POS_TYPE_INPUT_ONLY
	#ifdef SANI_CREATE_NEURONS_FOR_NOUN_VERB_VARIANTS
	*firstOutputNeuronInNetworkPre = firstNeuronInLayer2;
	#else
	*firstOutputNeuronInNetworkPre = firstNeuronInLayer1;
	#endif	
	#else
	*firstOutputNeuronInNetworkPre = firstNeuronInLayer4;
	#endif
		
	*idBase = id;

	return result;
}


	
bool SANIneuralNetworkOperationsClass::determinePositonsOfNeurons(vector<SANIGroupType*>* SANIGroupTypes, int64_t* idBase, ANNneuron* firstOutputNeuronInNetworkPre, ANNneuron** firstOutputNeuronInNetworkPost)
{
	bool result = true;
	
	int64_t id = *idBase;
	
	bool foundAtLeastOneNeuron = false;
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		for(int i1=0; i1<groupType->groups.size(); i1++)
		{
			SANIGroupNeuralNetwork* groupBase = (groupType->groups)[i1];
			foundAtLeastOneNeuron = true;
		}
	}			

	if(foundAtLeastOneNeuron)
	{
		for(int i=0; i<SANIGroupTypes->size(); i++)
		{
			SANIGroupType* groupType = SANIGroupTypes->at(i);
			vector<SANIGroupNeuralNetwork*> groupsOrdered;

			bool stillDisplayOrdering = true;
			int y = 0;
			#ifdef SANI_ANN_CENTRE_NEURONS
			int groupMaxY = 0;
			int groupMaxXacrossAllY = 0;
			#endif
			while(stillDisplayOrdering)
			{
				int x = 0;
				bool neuronDisplayPositionNotSetFound = false;
				for(int i1=0; i1<groupType->groups.size(); i1++)
				{
					SANIGroupNeuralNetwork* groupBase = (groupType->groups)[i1];
					if(!(groupBase->neuronDisplayPositionSet))
					{
						bool foundLowerLevelNeuronToBase = false;
						for(int i2=0; i2<groupType->groups.size(); i2++)
						{
							SANIGroupNeuralNetwork* group = (groupType->groups)[i2];
							if(!(group->neuronDisplayPositionSet) || (group->neuronDisplayPositionY == y))	//OLD: if(!(group->neuronDisplayPositionSet))
							{
								for(int i3=0; i3<group->SANIfrontComponentConnectionList.size(); i3++)
								{
									SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i3];
			 						SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	
									if(group->groupTypeName == groupBase->groupTypeName)		//added GIA3h3aTEMP33
									{
										if(group != ownerGroup)	//added GIA3h3aTEMP33 //OR: (group != groupBase)	//required for cases where a group directly references itself eg subReferenceSetsObject:act; <component componentType="group" groupTypeName="subReferenceSetsObject" optional="true" />
										{
											if(ownerGroup == groupBase)
											{
												foundLowerLevelNeuronToBase = true;
											}
										}
									}
								}
							}
						}
						if(!foundLowerLevelNeuronToBase)
						{
							foundAtLeastOneNeuron = true;

							neuronDisplayPositionNotSetFound = true;
							groupBase->neuronDisplayPositionSet = true;
							groupBase->neuronDisplayPositionX = x;
							groupBase->neuronDisplayPositionY = y;

							//cout << "groupBase->groupName = " << groupBase->groupName << endl;
							//cout << "groupType->neuronDisplayPositionX/Y = " << groupBase->neuronDisplayPositionX << "/" << groupBase->neuronDisplayPositionY << endl;
							#ifdef SANI_ANN_CENTRE_NEURONS
							if(x > groupMaxXacrossAllY)
							{
								groupMaxXacrossAllY = x;
							}
							#endif
							x++;
						}
					}
				}
				if(!neuronDisplayPositionNotSetFound)
				{
					stillDisplayOrdering = false;
				}
				#ifdef SANI_ANN_CENTRE_NEURONS
				groupMaxY = y;
				#endif
				y++;
			}

			#ifdef SANI_ANN_CENTRE_NEURONS
			#ifdef SANI_ANN_CENTRE_NEURONS_X_AXIS
			for(int y=0; y<groupMaxY; y++)
			{
				int groupMaxX = 0;
				for(int i1=0; i1<groupType->groups.size(); i1++)
				{
					SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
					if(group->neuronDisplayPositionY == y)
					{
						if(group->neuronDisplayPositionX > groupMaxX)
						{
							groupMaxX = group->neuronDisplayPositionX;
						}
					}
				}			
				for(int i1=0; i1<groupType->groups.size(); i1++)
				{
					SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
					if(group->neuronDisplayPositionY == y)
					{
						group->neuronDisplayPositionXcentred = group->neuronDisplayPositionX + (groupMaxXacrossAllY-groupMaxX)/2;
						group->neuronDisplayPositionYcentred = group->neuronDisplayPositionY;
					}
				}
			}
			#else
			for(int y=0; y<groupMaxY; y++)
			{
				for(int i1=0; i1<groupType->groups.size(); i1++)
				{
					SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
					group->neuronDisplayPositionXcentred = group->neuronDisplayPositionX;
				}
			}
			#endif
			#ifdef SANI_ANN_SEGREGATE_TOP_LAYER_NEURONS
			for(int i1=0; i1<groupType->groups.size(); i1++)
			{
				SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
				if(group->topLevelSentenceNeuron)
				{
					group->neuronDisplayPositionYcentred = group->neuronDisplayPositionY + SANI_ANN_SEGREGATE_TOP_LAYER_NEURONS_Y_SEPARATION;

				}
			}
			#endif
			/*
			#ifdef SANI_ANN_SEGREGATE_TOP_LAYER_NEURONS_OLD
			for(int i1=0; i1<groupType->groups.size(); i1++)
			{
				SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
				if(group->neuronDisplayPositionY == groupMaxY-1)
				{
					group->neuronDisplayPositionYcentred = group->neuronDisplayPositionY + SANI_ANN_SEGREGATE_TOP_LAYER_NEURONS_Y_SEPARATION;
				}
			}
			#endif
			*/
			#endif

		}
	
	
		bool stillDisplayOrdering = true;
		int y = 0;
		int groupTypeMaxY = 0;
		#ifdef SANI_ANN_SUPPORT_RECURSION
		int numberOfGroupTypesFound = 0;
		int totalNumberGroupTypes = SANIGroupTypes->size();
		bool activateLowestLevelUndisplayedGroupTypeAsInferredByXMLdisplayOrderFound = false;
		#endif
		while(stillDisplayOrdering)
		{
			int x = 0;
			bool neuronDisplayPositionNotSetFound = false;
			for(int i=0; i<SANIGroupTypes->size(); i++)
			{
				SANIGroupType* groupTypeBase = SANIGroupTypes->at(i);
				if(!(groupTypeBase->neuronDisplayPositionSet))
				{
					bool foundLowerLevelNeuronToBase = false;
					if(activateLowestLevelUndisplayedGroupTypeAsInferredByXMLdisplayOrderFound)
					{
						activateLowestLevelUndisplayedGroupTypeAsInferredByXMLdisplayOrderFound = false;
					}
					else
					{
						for(int i1=0; i1<SANIGroupTypes->size(); i1++)
						{
							SANIGroupType* groupType = SANIGroupTypes->at(i1);
							if(groupType != groupTypeBase)
							{
								if(!(groupType->neuronDisplayPositionSet) || (groupType->neuronDisplayPositionY == y))	//OLD:	if(!(groupType->neuronDisplayPositionSet))
								{	
									for(int i2=0; i2<groupType->groups.size(); i2++)
									{
										SANIGroupNeuralNetwork* group = (groupType->groups)[i2];
										for(int i3=0; i3<group->SANIfrontComponentConnectionList.size(); i3++)
										{
											SANIComponentNeuralNetwork* currentComponent = (group->SANIfrontComponentConnectionList)[i3];
			 								SANIGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;
											if(ownerGroup->groupTypeName == groupTypeBase->groupTypeName)
											{
												foundLowerLevelNeuronToBase = true;
											}
										}
									}
								}
							}
						}
					}
					if(!foundLowerLevelNeuronToBase)
					{ 
						neuronDisplayPositionNotSetFound = true;
						groupTypeBase->neuronDisplayPositionSet = true;
						groupTypeBase->neuronDisplayPositionX = x;
						groupTypeBase->neuronDisplayPositionY = y;
						groupTypeMaxY = y;
						numberOfGroupTypesFound++;
						x++;
					}
				}
			}
			if(!neuronDisplayPositionNotSetFound)
			{
				#ifdef SANI_ANN_SUPPORT_RECURSION
				if(numberOfGroupTypesFound < totalNumberGroupTypes)
				{
					activateLowestLevelUndisplayedGroupTypeAsInferredByXMLdisplayOrderFound = true;
					y--;
				}
				else
				{
					stillDisplayOrdering = false;
				}
				#else
				stillDisplayOrdering = false;
				#endif
			}
			y++;
		}

		//cout << "\n\n\n\n\n\n" << endl;

		//create neuron layer connections;
		ANNneuron* firstGroupNeuronInLayerX = NULL;
		ANNneuron* previousGroupNeuronInLayerX = NULL;
		ANNneuron* firstGroupNeuronInGroupTypeLastLayer = NULL;

		#ifdef SANI_ANN_CALCULATE_BOUNDING_BOXES
		int groupTypeLayerYsum = SANI_ANN_GROUPTYPE_INPUT_POSITION_Y;
		for(int groupTypeY=0; groupTypeY<=groupTypeMaxY; groupTypeY++)
		{
			int groupTypeLayerMaxY = 0;
			int groupTypeMaxX = 0;
			for(int i=0; i<SANIGroupTypes->size(); i++)
			{
				SANIGroupType* groupType = SANIGroupTypes->at(i);
				if(groupType->neuronDisplayPositionY == groupTypeY)
				{
					for(int i1=0; i1<groupType->groups.size(); i1++)
					{
						SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
						ANNneuron* currentGroupNeuronInLayerX = group->neuronReference;
						if(group->neuronDisplayPositionX == 0)
						{	
							if(group->neuronDisplayPositionY > groupTypeLayerMaxY)
							{
								groupTypeLayerMaxY = group->neuronDisplayPositionY;
							}	
						}
					}
					if(groupType->neuronDisplayPositionX > groupTypeMaxX)
					{
						groupTypeMaxX = groupType->neuronDisplayPositionX;
					}
				}
			}

			for(int groupY=0; groupY<=groupTypeLayerMaxY; groupY++)
			{
				int groupTypeLayerXsum = SANI_ANN_GROUPTYPE_INPUT_POSITION_X;
				for(int groupTypeX=0; groupTypeX<=groupTypeMaxX; groupTypeX++)
				{
					int groupTypeLayerMaxX = 0;
					for(int i=0; i<SANIGroupTypes->size(); i++)
					{
						SANIGroupType* groupType = SANIGroupTypes->at(i);
						if((groupType->neuronDisplayPositionY == groupTypeY) && (groupType->neuronDisplayPositionX == groupTypeX))
						{
							for(int i1=0; i1<groupType->groups.size(); i1++)
							{
								SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
								if(group->neuronDisplayPositionY == groupY)
								{	
									if(group->neuronDisplayPositionX > groupTypeLayerMaxX)
									{
										groupTypeLayerMaxX = group->neuronDisplayPositionX;
									}	
								}
							}
						}
					}
					for(int groupX=0; groupX<=groupTypeLayerMaxX; groupX++)
					{
						for(int i=0; i<SANIGroupTypes->size(); i++)
						{
							SANIGroupType* groupType = SANIGroupTypes->at(i);
							if((groupType->neuronDisplayPositionY == groupTypeY) && (groupType->neuronDisplayPositionX == groupTypeX))
							{
								for(int i1=0; i1<groupType->groups.size(); i1++)
								{
									SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
									if((group->neuronDisplayPositionY == groupY) && (group->neuronDisplayPositionX == groupX))
									{
										SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
										#ifdef SANI_ANN_CALCULATE_BOUNDING_BOXES
										int groupTypeXposAbsolute = groupTypeLayerXsum;
										int groupTypeYposAbsolute = groupTypeLayerYsum;
										#else
										int groupTypeXposAbsolute = SANI_ANN_GROUPTYPE_INPUT_POSITION_X + groupType->neuronDisplayPositionX*SANI_ANN_GROUPTYPE_SPACING_X;
										int groupTypeYposAbsolute = SANI_ANN_GROUPTYPE_INPUT_POSITION_Y + groupType->neuronDisplayPositionY*SANI_ANN_GROUPTYPE_SPACING_Y;
										#endif

										#ifdef SANI_ANN_CENTRE_NEURONS
										group->neuronReference->xPosRel = groupTypeXposAbsolute + group->neuronDisplayPositionXcentred*SANI_ANN_GROUP_SPACING_X;
										group->neuronReference->yPosRel = groupTypeYposAbsolute + group->neuronDisplayPositionYcentred*SANI_ANN_GROUP_SPACING_Y;
										#else
										group->neuronReference->xPosRel = groupTypeXposAbsolute + group->neuronDisplayPositionX*SANI_ANN_GROUP_SPACING_X;
										group->neuronReference->yPosRel = groupTypeYposAbsolute + group->neuronDisplayPositionY*SANI_ANN_GROUP_SPACING_Y;
										#endif
										group->neuronReference->id = id;

										/*
										cout << "group->neuronReference->id = " << group->neuronReference->id << endl;
										cout << "group->neuronReference->SANIneuronName = " << group->neuronReference->SANIneuronName << endl;
										cout << "groupType->neuronDisplayPositionX/Y, group->neuronDisplayPositionX/Y = " << groupType->neuronDisplayPositionX << "/" << groupType->neuronDisplayPositionY << ", " << group->neuronDisplayPositionX << "/" << group->neuronDisplayPositionY << endl;
										*/

										#ifdef SANI_ANN_CALCULATE_BOUNDING_BOXES2
										bool firstGroupNeuronInLayerXFound = false;
										ANNneuron* currentGroupNeuronInLayerX = group->neuronReference;
										//if(!(group->neuronConnectivitySet))
										//{
										//group->neuronConnectivitySet = true;
										if((groupType->neuronDisplayPositionX == 0) && (group->neuronDisplayPositionX == 0))
										{
											//cout << "((groupType->neuronDisplayPositionX == 0) && (group->neuronDisplayPositionX == 0))" << endl;

											firstGroupNeuronInLayerXFound = true;
											firstGroupNeuronInLayerX = currentGroupNeuronInLayerX;
											*firstOutputNeuronInNetworkPost = firstGroupNeuronInLayerX;

											if((groupType->neuronDisplayPositionY == 0) && (group->neuronDisplayPositionY == 0))
											{
												firstOutputNeuronInNetworkPre->hasFrontLayer = true;
												firstOutputNeuronInNetworkPre->firstNeuronInFrontLayer = firstGroupNeuronInLayerX;	
											}
											else
											{
												firstGroupNeuronInGroupTypeLastLayer->hasFrontLayer = true;
												firstGroupNeuronInGroupTypeLastLayer->firstNeuronInFrontLayer = firstGroupNeuronInLayerX;	
											}

											firstGroupNeuronInGroupTypeLastLayer = firstGroupNeuronInLayerX;
										}
										else
										{
											previousGroupNeuronInLayerX->nextNeuron = currentGroupNeuronInLayerX;
										}
										previousGroupNeuronInLayerX = currentGroupNeuronInLayerX;
										//}
										#endif

										id++;
									}
								}
							}
						}
					}
					groupTypeLayerXsum = groupTypeLayerXsum + groupTypeLayerMaxX*SANI_ANN_GROUP_SPACING_X + SANI_ANN_GROUPTYPE_SPACING_X;
				}
				#ifdef SANI_ANN_CALCULATE_BOUNDING_BOXES2
				previousGroupNeuronInLayerX->nextNeuron = new ANNneuron();	//create a null neuron at end of layer	
				#endif		
			}
			groupTypeLayerYsum = groupTypeLayerYsum + groupTypeLayerMaxY*SANI_ANN_GROUP_SPACING_Y + SANI_ANN_GROUPTYPE_SPACING_Y;
		}
		#else
		cerr << "SANIneuralNetworkOperationsClass::determinePositonsOfNeurons{} warning: !SANI_ANN_CALCULATE_BOUNDING_BOXES has not been coded" << endl;
		for(int i=0; i<SANIGroupTypes->size(); i++)
		{
			SANIGroupType* groupType = SANIGroupTypes->at(i);

			for(int i1=0; i1<groupType->groups.size(); i1++)
			{
				SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
				group->neuronReference->xPosRel = SANI_ANN_GROUPTYPE_INPUT_POSITION_X + groupType->neuronDisplayPositionX*SANI_ANN_GROUPTYPE_SPACING_X + group->neuronDisplayPositionX*SANI_ANN_GROUP_SPACING_X;
				group->neuronReference->yPosRel = SANI_ANN_GROUPTYPE_INPUT_POSITION_Y + groupType->neuronDisplayPositionY*SANI_ANN_GROUPTYPE_SPACING_Y + group->neuronDisplayPositionY*SANI_ANN_GROUP_SPACING_Y;
				group->neuronReference->id = id;
				id++;
			}
		}
		#endif


		#ifndef SANI_ANN_CALCULATE_BOUNDING_BOXES2
		for(int groupTypeY=0; groupTypeY<=groupTypeMaxY; groupTypeY++)
		{
			int groupTypeLayerMaxY = 0;
			for(int i=0; i<SANIGroupTypes->size(); i++)
			{
				SANIGroupType* groupType = SANIGroupTypes->at(i);
				if(groupType->neuronDisplayPositionY == groupTypeY)
				{
					for(int i1=0; i1<groupType->groups.size(); i1++)
					{
						SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
						ANNneuron* currentGroupNeuronInLayerX = group->neuronReference;
						if(group->neuronDisplayPositionX == 0)	//CHECKTHIS
						{	
							if(group->neuronDisplayPositionY > groupTypeLayerMaxY)
							{
								groupTypeLayerMaxY = group->neuronDisplayPositionY;
								//firstGroupNeuronInGroupTypeLastLayer = group;
							}	
						}
					}
				}
			}

			previousGroupNeuronInLayerX = NULL;

			for(int groupY=0; groupY<=groupTypeLayerMaxY; groupY++)
			{	
				for(int i=0; i<SANIGroupTypes->size(); i++)
				{
					SANIGroupType* groupType = SANIGroupTypes->at(i);
					if(groupType->neuronDisplayPositionY == groupTypeY)
					{
						for(int i1=0; i1<groupType->groups.size(); i1++)
						{
							SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
							if(group->neuronDisplayPositionY == groupY)
							{
								bool firstGroupNeuronInLayerXFound = false;
								ANNneuron* currentGroupNeuronInLayerX = group->neuronReference;
								if(!(group->neuronConnectivitySet))
								{
									group->neuronConnectivitySet = true;

									/*
									cout << "group->neuronReference->id = " << group->neuronReference->id << endl;
									cout << "group->neuronReference->SANIneuronName = " << group->neuronReference->SANIneuronName << endl;
									cout << "groupType->neuronDisplayPositionX/Y, group->neuronDisplayPositionX/Y = " << groupType->neuronDisplayPositionX << "/" << groupType->neuronDisplayPositionY << ", " << group->neuronDisplayPositionX << "/" << group->neuronDisplayPositionY << endl;
									*/
									if((groupType->neuronDisplayPositionX == 0) && (group->neuronDisplayPositionX == 0))
									{
										//cout << "((groupType->neuronDisplayPositionX == 0) && (group->neuronDisplayPositionX == 0))" << endl;

										firstGroupNeuronInLayerXFound = true;
										firstGroupNeuronInLayerX = currentGroupNeuronInLayerX;
										*firstOutputNeuronInNetworkPost = firstGroupNeuronInLayerX;

										if((groupType->neuronDisplayPositionY == 0) && (group->neuronDisplayPositionY == 0))
										{
											firstOutputNeuronInNetworkPre->hasFrontLayer = true;
											firstOutputNeuronInNetworkPre->firstNeuronInFrontLayer = firstGroupNeuronInLayerX;	
										}
										else
										{
											firstGroupNeuronInGroupTypeLastLayer->hasFrontLayer = true;
											firstGroupNeuronInGroupTypeLastLayer->firstNeuronInFrontLayer = firstGroupNeuronInLayerX;	
										}

										firstGroupNeuronInGroupTypeLastLayer = firstGroupNeuronInLayerX;
									}
									else
									{
										previousGroupNeuronInLayerX->nextNeuron = currentGroupNeuronInLayerX;
									}

									previousGroupNeuronInLayerX = currentGroupNeuronInLayerX;
								}
							}
						}
					}
				}

				previousGroupNeuronInLayerX->nextNeuron = new ANNneuron();	//create a null neuron at end of layer
			}
		}
		#endif

		*idBase = id;
	}
	
	return result;
}

#ifdef SANI_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_CONTINUOUSLY_OUTPUT_NETWORK
bool SANIneuralNetworkOperationsClass::determinePositonsOfNeuronsReset(vector<SANIGroupType*>* SANIGroupTypes)
{
	bool result = true;
	
	for(int i=0; i<SANIGroupTypes->size(); i++)
	{
		SANIGroupType* groupType = SANIGroupTypes->at(i);
		
		groupType->neuronDisplayPositionSet = false;
		groupType->neuronDisplayPositionX = INT_DEFAULT_VALUE;
		groupType->neuronDisplayPositionY = INT_DEFAULT_VALUE;
		groupType->groupTypeXindex = INT_DEFAULT_VALUE;
		groupType->groupMaxY = 0;
			
		for(int i1=0; i1<groupType->groups.size(); i1++)
		{
			SANIGroupNeuralNetwork* group = (groupType->groups)[i1];
			
			group->neuronDisplayPositionSet = false;
			group->neuronDisplayPositionX = INT_DEFAULT_VALUE;
			group->neuronDisplayPositionY = INT_DEFAULT_VALUE;
			group->neuronConnectivitySet = false;
			#ifdef SANI_ANN_CENTRE_NEURONS
			group->neuronDisplayPositionXcentred = INT_DEFAULT_VALUE;
			group->neuronDisplayPositionYcentred = INT_DEFAULT_VALUE;	
			#endif
			
			group->neuronReference->hasFrontLayer = false;
			group->neuronReference->firstNeuronInFrontLayer = NULL;
		}
	}	
	
	return result;
}
#endif


#endif

#endif




