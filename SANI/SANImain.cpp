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
 * File Name: SANImain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Sequentially Activated Neuronal Input neural network
 * Project Version: 1p4a 17-March-2021
 * Requirements: requires text parsed by BAI Language Reduction Preprocessor (LRP)
 * Description: Main
 * /
 *******************************************************************************/


#include "SANImain.hpp"

#ifndef LINUX
	#include <windows.h>
#endif

#ifdef SANI_NEURAL_NETWORK
#include "ANNdraw.hpp"
#endif


	

#ifdef COMPILE_SANI
static char errmessage[] = "Usage:  SANI.exe [options]\n\n\twhere options are any of the following\n"
"\n\t-itxt [string]       : plain text .txt input filename to be parsed by the SANI generator (def: inputText.txt)"
#ifdef SANI_SUPPORT_QUERIES
"\n\t-itxtq [string]      : query plain text .txt input filename to be parsed by the SANI generator (def: inputTextQuery.txt)"
#endif
#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
"\n\t-ixml [string]       : sequential network definition .xml input filename (def: sequentialNet.xml)"
"\n\t-oxml [string]       : sequential network definition .xml output filename (def: sequentialNet.xml)"
#ifdef SANI_SUPPORT_QUERIES
"\n\t-ixmlq [string]      : query sequential network definition .xml input filename (def: sequentialNetQuery.xml)"
"\n\t-oxmlq [string]      : query sequential network definition .xml output filename (def: sequentialNetQuery.xml)"	
#endif
#endif
"\n\t-oall [string]       : semantic network display xml/.svg/.ldr/.ppm default generic output filename (def: semanticNet)"
#ifdef SANI_NEURAL_NETWORK
"\n\t-oannxml [string]    : neural network definition .xml output filename (def: neuralNet.xml)"
"\n\t-oannldr [string]    : neural network display .ldr output filename (def: neuralNet.ldr)"
"\n\t-oannsvg [string]    : neural network display .svg output filename (def: neuralNet.svg)"
"\n\t-oannppm [string]    : neural network display .ppm opengl output filename (def: neuralNet.ppm)"
"\n\t-oannppm2 [string]   : neural network display .ppm raytraced output filename (def: neuralNetRaytraced.ppm)"
"\n\t-oannall [string]    : neural network display .xml/.svg/.ldr/.ppm default generic output filename (def: neuralNet)"
#endif
#ifdef LRP_PREPROCESSOR
"\n\t-lrp                               : language reduction preprocessor"
"\n\t-olrptxt [string]                  : plain text .txt output filename with LRP language reduction preprocessor applied (def: inputTextWithLRP.txt)"
//"\n\t-olrptxtq [string]                 : query plain text .txt output filename with LRP language reduction preprocessor applied (def: inputTextWithLRPQuery.txt)"
"\n\t-lrpfolder [string]                : folder of LRP data files (list of multiword verbs, multiword prepositions etc) (def: same as exe)"
#endif
//"\n\t-show                : display output in opengl"
"\n\t-width [int]         : raster graphics width in pixels (def: 640)"
"\n\t-height [int]        : raster graphics height in pixels (def: 480)"
"\n\t-inputfolder [string]              : input directory name for input files (def: same as exe)"
"\n\t-outputfolder [string]             : output directory name for temporary and output files (def: same as exe)"
"\n"
"\n\n\t-version         : print version"
"\n\n\tThis program performs SANI (Sequentially Activated Neuronal Input neural network) operations - creates sequentially activated neural network based upon text file; outputs sequential network to SANI sequential network file (.xml), outputs neural network to ANN neural network file (.xml); prints neural network to raster image (.ppm), 3D vector graphics (.ldr), or 2D vector graphics (.svg).\n\n";

int main(const int argc, const char** argv)
{
	bool result = true;

	srand( (unsigned)time(NULL) );	//seeds randomness

	#ifdef SANI_PRINT_EXECUTION_TIME
	SHAREDvarsClass().printTime("SANI execution time: ", " (start)");
	#endif

	string currentFolder = SHAREDvarsClass().getCurrentDirectory();
	string inputFolderLocal = currentFolder;
	string outputFolderLocal = currentFolder;

	bool useInputTextPlainTXTFile = false;
	string inputTextPlainTXTfileName = string(SANI_TEXT_BASE_FILE_NAME) + SANI_TEXT_FILE_EXTENSION;			//"inputText.txt";
	#ifdef SANI_SUPPORT_QUERIES
	bool useInputQueryPlainTXTFile = false;
	string inputQueryPlainTXTFileName = string(SANI_TEXT_BASE_FILE_NAME) + SANI_QUERY_FILE_PREPEND + SANI_TEXT_FILE_EXTENSION;				//"inputTextQuery.txt";
	#endif
	#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
	bool useInputTextXMLFile = false;
	string inputTextXMLFileName = string(SANI_SEQUENTIAL_NET_BASE_FILE_NAME) + SANI_SEQUENTIAL_NET_XML_FILE_EXTENSION;
	bool useOutputTextXMLFile = false;
	string outputTextXMLFileName = string(SANI_SEQUENTIAL_NET_BASE_FILE_NAME) + SANI_SEQUENTIAL_NET_XML_FILE_EXTENSION;
	#ifdef SANI_SUPPORT_QUERIES
	bool useInputQueryXMLFile = false;
	string inputQueryXMLFileName = string(SANI_SEQUENTIAL_NET_BASE_QUERY_FILE_NAME) + SANI_SEQUENTIAL_NET_XML_FILE_EXTENSION;
	bool useOutputQueryXMLFile = false;
	string outputQueryXMLFileName = string(SANI_SEQUENTIAL_NET_BASE_QUERY_FILE_NAME) + SANI_SEQUENTIAL_NET_XML_FILE_EXTENSION;	
	#endif
	#endif
	bool useOutputTextAllFile = false;
	string outputTextAllFileName = SANI_SEQUENTIAL_NET_BASE_FILE_NAME;
	
	#ifdef SANI_NEURAL_NETWORK
	bool ANNuseInputXMLFile = false;
	string ANNinputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputXMLFile = false;
	string ANNoutputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool ANNuseOutputLDRFile = false;
	string ANNoutputLDRFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
	bool ANNuseOutputSVGFile = false;
	string ANNoutputSVGFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
	bool ANNuseOutputPPMFile = false;
	string ANNoutputPPMFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
	bool ANNuseOutputPPMFileRaytraced = false;
	string ANNoutputPPMFileNameRaytraced = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
	string ANNoutputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;
	bool ANNuseOutputAllFile = false;
	string ANNoutputAllFileName = NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME;
	bool ANNdrawOutput = false;
	bool ANNuseSprites = true;
	#endif

	int rasterImageWidth = 640;
	int rasterImageHeight = 480;
	
	bool useInputQuery = false;

	#ifdef LRP_PREPROCESSOR
	bool useLRP = false;
	bool useOutputLRPTextPlainTXTFile = false;
	string outputLRPTextPlainTXTFileName = string(SANI_TEXT_BASE_FILE_NAME) + SANI_TEXT_FILE_WITH_LRP_PREPEND + SANI_TEXT_FILE_EXTENSION;
	#ifdef SANI_SUPPORT_QUERIES
	bool useOutputQueryLRPTextPlainTXTFile = false;
	string outputQueryLRPTextPlainTXTFileName = string(SANI_TEXT_BASE_FILE_NAME) + SANI_TEXT_FILE_WITH_LRP_PREPEND + SANI_QUERY_FILE_PREPEND + SANI_TEXT_FILE_EXTENSION;
	#endif
	string lrpDataFolderName = "";
	#endif


	//basic execution flow outline; if no dataset or xml inputText file is specified, just form network - do not train network

	if(!SHAREDvarsClass().argumentExists(argc, argv, "-itxt"))
	{
		cerr << "error: SANI requires a plain text inputText file (.txt) to be defined" << endl;
		printf(errmessage);
		exit(EXIT_ERROR);
	}
	
	if(SHAREDvarsClass().argumentExists(argc, argv, "-itxt"))
	{
		inputTextPlainTXTfileName = SHAREDvarsClass().getStringArgument(argc, argv, "-itxt");
		useInputTextPlainTXTFile = true;
	}
	#ifdef SANI_SUPPORT_QUERIES
	if(SHAREDvarsClass().argumentExists(argc, argv, "-itxtq"))
	{
		inputQueryPlainTXTFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-itxtq");
		useInputQueryPlainTXTFile = true;
		useInputQuery = true;
	}
	#endif
	#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
	if(SHAREDvarsClass().argumentExists(argc, argv, "-ixml"))
	{
		inputTextXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ixml");
		//train = true;
		useInputTextXMLFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oxml"))
	{
		outputTextXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oxml");
		useOutputTextXMLFile = true;
	}
	#ifdef SANI_SUPPORT_QUERIES
	if(SHAREDvarsClass().argumentExists(argc, argv, "-ixmlq"))
	{
		inputQueryXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ixmlq");
		useInputQueryXMLFile = true;
		useInputQuery = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oxmlq"))
	{
		outputQueryXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oxmlq");
		useOutputQueryXMLFile = true;
	}	
	#endif
	#endif
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oall"))
	{
		outputTextAllFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oall");
		useOutputTextAllFile = true;
	}
	
				
	#ifdef SANI_NEURAL_NETWORK
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannxml"))
	{
		ANNoutputXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannxml");
		ANNuseOutputXMLFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannldr"))
	{
		ANNoutputLDRFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannldr");
		ANNuseOutputLDRFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannsvg"))
	{
		ANNoutputSVGFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannsvg");
		ANNuseOutputSVGFile = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannppm"))
	{
		ANNoutputPPMFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannppm");
		ANNuseOutputPPMFile = true;
		ANNuseOutputLDRFile = true;	//required for OpenGL image generation
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannppm2"))
	{
		ANNoutputPPMFileNameRaytraced = SHAREDvarsClass().getStringArgument(argc, argv, "-oannppm2");
		ANNuseOutputPPMFileRaytraced = true;
		ANNuseOutputLDRFile = true;	//required for raytrace image generation
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-oannall"))
	{
		ANNoutputAllFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oannall");
		ANNuseOutputAllFile = true;
	}
	#ifndef ANN_DISPLAY_DISABLE_SPRITES
	if(SHAREDvarsClass().argumentExists(argc, argv, "-annsprites"))
	{
		ANNuseSprites = true;
	}
	#endif
	#endif

	#ifdef LRP_PREPROCESSOR
	if(SHAREDvarsClass().argumentExists(argc, argv, "-lrp"))
	{
		useLRP = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-olrptxt"))
	{
		outputLRPTextPlainTXTFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-olrptxt");
		useOutputLRPTextPlainTXTFile = true;
	}
	#ifdef SANI_SUPPORT_QUERIES
	if(SHAREDvarsClass().argumentExists(argc, argv, "-olrptxtq"))
	{
		outputQueryLRPTextPlainTXTFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-olrptxtq");
		useOutputQueryLRPTextPlainTXTFile = true;
	}
	#endif
	if(SHAREDvarsClass().argumentExists(argc, argv, "-lrpfolder"))
	{
		lrpDataFolderName = SHAREDvarsClass().getStringArgument(argc, argv, "-lrpfolder");
	}
	else
	{
		lrpDataFolderName = currentFolder;
	}
	#endif
	
	/*
	if(SHAREDvarsClass().argumentExists(argc, argv, "-show"))
	{
		displayInOpenGLAndOutputScreenshot = true;
	}
	*/
	if(SHAREDvarsClass().argumentExists(argc, argv, "-width"))
	{
		rasterImageWidth = SHAREDvarsClass().getFloatArgument(argc, argv, "-width");
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-height"))
	{
		rasterImageHeight = SHAREDvarsClass().getFloatArgument(argc, argv, "-height");
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-inputfolder"))
	{
		inputFolderLocal = SHAREDvarsClass().getStringArgument(argc, argv, "-inputfolder");
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-outputfolder"))
	{
		outputFolderLocal = SHAREDvarsClass().getStringArgument(argc, argv, "-outputfolder");
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-version"))
	{
		cout << "SANI.exe - Project Version: 1p4a 17-March-2021" << endl;
		exit(EXIT_OK);
	}


	SANItranslatorVariablesClass* translatorVariables = new SANItranslatorVariablesClass();
	translatorVariables->isQuery = false;
	#ifdef SANI_NEURAL_NETWORK
	translatorVariables->ANNtranslatorVariables = new ANNtranslatorVariablesClass();
	translatorVariables->ANNtranslatorVariables->firstInputNeuronInNetwork = new ANNneuron();
	#endif	
	
	
	#ifdef SANI_SUPPORT_QUERIES
	SANItranslatorVariablesClass* translatorVariablesQuery = new SANItranslatorVariablesClass();
	translatorVariablesQuery->isQuery = true;
	#ifdef SANI_NEURAL_NETWORK
	translatorVariablesQuery->ANNtranslatorVariables = new ANNtranslatorVariablesClass();
	translatorVariablesQuery->ANNtranslatorVariables->firstInputNeuronInNetwork = new ANNneuron();
	#endif
	#endif
	
	SANImainClass().executeSANI(

		translatorVariables,
		#ifdef SANI_SUPPORT_QUERIES
		translatorVariablesQuery,
		#endif

		inputFolderLocal,
		outputFolderLocal,

		useInputTextPlainTXTFile,
		inputTextPlainTXTfileName,
		#ifdef SANI_SUPPORT_QUERIES
		useInputQueryPlainTXTFile,
		inputQueryPlainTXTFileName,
		#endif
		#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
		useInputTextXMLFile,
		inputTextXMLFileName,
		useOutputTextXMLFile,
		outputTextXMLFileName,
		#ifdef SANI_SUPPORT_QUERIES
		useInputQueryXMLFile,
		inputQueryXMLFileName,
		useOutputQueryXMLFile,
		outputQueryXMLFileName,
		#endif
		#endif
		useOutputTextAllFile,
		outputTextAllFileName,
	
		#ifdef SANI_NEURAL_NETWORK
		ANNuseInputXMLFile,
		ANNinputXMLFileName,	
		ANNuseOutputXMLFile,
		ANNoutputXMLFileName,
		ANNuseOutputLDRFile,
		ANNoutputLDRFileName,
		ANNuseOutputSVGFile,
		ANNoutputSVGFileName,
		ANNuseOutputPPMFile,
		ANNoutputPPMFileName,
		ANNuseOutputPPMFileRaytraced,
		ANNoutputPPMFileNameRaytraced,			
		ANNuseOutputAllFile,
		ANNoutputAllFileName,
		ANNuseSprites,
		#endif

		rasterImageWidth,
		rasterImageHeight,

		#ifdef SANI_SUPPORT_QUERIES
		useInputQuery,
		#endif
		
		#ifdef LRP_PREPROCESSOR
		useLRP,
		useOutputLRPTextPlainTXTFile,
		outputLRPTextPlainTXTFileName,
		#ifdef SANI_SUPPORT_QUERIES
		useOutputQueryLRPTextPlainTXTFile,
		outputQueryLRPTextPlainTXTFileName,
		#endif
		lrpDataFolderName
		#endif
	);

	#ifdef SANI_PRINT_EXECUTION_TIME
	SHAREDvarsClass().printTime("SANI execution time: ", " (finish)");
	#endif
}

bool SANImainClass::executeSANI(

	SANItranslatorVariablesClass* translatorVariables,
	#ifdef SANI_SUPPORT_QUERIES
	SANItranslatorVariablesClass* translatorVariablesQuery,
	#endif
		
	string inputFolderLocal, 
	string outputFolderLocal,

	bool useInputTextPlainTXTFile,
	string inputTextPlainTXTfileName,
	#ifdef SANI_SUPPORT_QUERIES
	bool useInputQueryPlainTXTFile,
	string inputQueryPlainTXTFileName,	
	#endif
	#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
	bool useInputTextXMLFile,
	string inputTextXMLFileName,	
	bool useOutputTextXMLFile,
	string outputTextXMLFileName,
	#ifdef SANI_SUPPORT_QUERIES
	bool useInputQueryXMLFile,
	string inputQueryXMLFileName,
	bool useOutputQueryXMLFile,
	string outputQueryXMLFileName,	
	#endif
	#endif
	bool useOutputTextAllFile,
	string outputTextAllFileName,
	
	#ifdef SANI_NEURAL_NETWORK
	bool ANNuseInputXMLFile,
	string ANNinputXMLFileName,
	bool ANNuseOutputXMLFile,
	string ANNoutputXMLFileName,
	bool ANNuseOutputLDRFile,
	string ANNoutputLDRFileName,
	bool ANNuseOutputSVGFile,
	string ANNoutputSVGFileName,
	bool ANNuseOutputPPMFile,
	string ANNoutputPPMFileName,
	bool ANNuseOutputPPMFileRaytraced,
	string ANNoutputPPMFileNameRaytraced,
	bool ANNuseOutputAllFile,
	string ANNoutputAllFileName,
	bool ANNuseSprites,
	#endif

	int rasterImageWidth,
	int rasterImageHeight,
		
	#ifdef SANI_SUPPORT_QUERIES
	bool useInputQuery,
	#endif
	
	#ifdef LRP_PREPROCESSOR
	bool useLRP,
	bool useOutputLRPTextPlainTXTFile,
	string outputLRPTextPlainTXTFileName,
	#ifdef SANI_SUPPORT_QUERIES
	bool useOutputQueryLRPTextPlainTXTFile,
	string outputQueryLRPTextPlainTXTFileName,
	#endif
	string lrpDataFolderName
	#endif
	)
{
#ifdef USE_CS_WORKAROUND
}
bool SANImainClass::executeSANI2()
{
#endif
	bool result = true;

	bool displayInOpenGL = false;
	
	#ifdef SANI_NEURAL_NETWORK
	bool ANNdisplayInOpenGL = false;
	bool ANNdrawOutput = false;
	if(ANNuseOutputPPMFile)
	{
		ANNdisplayInOpenGL = true;
		displayInOpenGL = true;
	}
	if(ANNuseOutputLDRFile)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputSVGFile)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputPPMFile)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputPPMFileRaytraced)
	{
		ANNdrawOutput = true;
	}
	if(ANNuseOutputAllFile)
	{
		ANNdrawOutput = true;
	}	
	if(ANNdrawOutput)
	{
		if(ANNuseOutputAllFile)
		{
			if(!ANNuseOutputLDRFile)
			{
				ANNuseOutputLDRFile = true;
				ANNoutputLDRFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
			}

			if(!ANNuseOutputXMLFile)
			{
				ANNuseOutputXMLFile = true;
				ANNoutputXMLFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
			}

			if(!ANNuseOutputSVGFile)
			{
				ANNuseOutputSVGFile = true;	//SVG output is not always required when printing/drawing neural network
				ANNoutputSVGFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
			}
			if(!ANNuseOutputPPMFile)
			{
				ANNuseOutputPPMFile = true;
				ANNoutputPPMFileName = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
			}
			/* disable raytrace output by default
			if(!ANNuseOutputPPMFileRaytraced)
			{
				ANNuseOutputPPMFileRaytraced = true;
				ANNoutputPPMFileNameRaytraced = ANNoutputAllFileName + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
			}
			*/
		}
	}
	#endif
	
	#ifdef LRP_PREPROCESSOR
	lrpDataFolderName = lrpDataFolderName + CHAR_FOLDER_DELIMITER;	
	#endif
	
	inputFolder = inputFolderLocal;
	outputFolder = outputFolderLocal;
	SHAREDvarsClass().setCurrentDirectory(inputFolder);
	
	/*
	if(!XMLrulesClass.parseSANIrulesXMLfile())	//required for sprites only
	{
		cerr << "error: SANIrules.xml file not detected" << endl;
		cout << "currentDirectory = " << SHAREDvarsClass().getCurrentDirectory() << endl;
		exit(EXIT_ERROR);
	}
	LDsprite.fillInLDspriteExternVariables();
	*/
	
	#ifdef SANI_NEURAL_NETWORK
	if(!XMLrulesClassClass().parseANNrulesXMLfile())
	{
		result = false;
	}
	ANNdraw.fillInANNSpriteExternVariables();
	#endif

	#ifdef LRP_PREPROCESSOR
	if(!LRPpreprocessorWordIdentification.initialiseLRP(lrpDataFolderName, useLRP))
	{
		result = false;
	}
	#endif
	

	#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
	if(!useOutputTextXMLFile)
	{
		if(useOutputTextAllFile)
		{
			useOutputTextXMLFile = true;
			outputTextXMLFileName = outputTextAllFileName + ".xml";
		}
	}
	#endif

	/*
	if(displayInOpenGL)
	{
		LDopengl.initiateOpenGL(rasterImageWidth, rasterImageHeight, 0, 0, false);
	}
	*/

	#ifdef SANI_SUPPORT_QUERIES
	if(useInputQuery)
	{
		#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
		if(!useOutputQueryXMLFile)
		{
			if(useOutputTextAllFile)
			{
				useOutputQueryXMLFile = true;
				outputQueryXMLFileName = outputTextAllFileName + "Query.xml";
			}
		}
		#endif
	}
	#endif
	
	if(useLRP)
	{
		if(!useOutputLRPTextPlainTXTFile)
		{
			useOutputLRPTextPlainTXTFile = true;
			outputLRPTextPlainTXTFileName = outputTextAllFileName + "afterLRP.txt";
		}
		#ifdef SANI_SUPPORT_QUERIES
		if(useInputQuery)
		{
			if(!useOutputQueryLRPTextPlainTXTFile)
			{
				useOutputQueryLRPTextPlainTXTFile = true;
				outputQueryLRPTextPlainTXTFileName = outputTextAllFileName + "afterLRPQuery.txt";
			}
		}
		#endif
	}


	
	#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
	if(!(!useInputTextPlainTXTFile && useInputTextXMLFile))
	#else
	if(useInputTextPlainTXTFile)
	#endif
	{
		string inputTextNLPfeatureXMLfileNameNOTUSED = "";
		if(!LRPpreprocessor.preprocessTextForGIAwrapper(useLRP, &inputTextPlainTXTfileName, outputLRPTextPlainTXTFileName, false, &(translatorVariables->LRPpreprocessorTranslatorVariables), &useInputTextPlainTXTFile, inputTextNLPfeatureXMLfileNameNOTUSED))
		{
			result = false;
		}
	}


	if(useInputTextPlainTXTFile)
	{
		if(!SANIposRelTranslator.parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(translatorVariables))
		{
			result = false;
		}
	}

	#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
	if(useInputTextXMLFile)
	{
		if(useInputTextPlainTXTFile)
		{
			cerr << "error: useInputTextXMLFile && useInputTextPlainTXTFile" << endl;
			exit(EXIT_ERROR);
		}
		if(useInputTextNLPrelationXMLFile)
		{
			cerr << "error: useInputTextXMLFile && useInputTextNLPrelationXMLFile" << endl;
			exit(EXIT_ERROR);
		}
		else
		{
			if(!SANIxml.readSequentialNetXMLfile(translatorVariables, inputTextXMLFileName))
			{
				result = false;
			}

		}
	}
	/*
	//code moved below:
	else
	{
		if(useOutputTextXMLFile)
		{
			if(!SANIxml.writeSequentialNetXMLFile(translatorVariables, outputTextXMLFileName))
			{
				result = false;
			}
		}
	}
	*/
	#endif	

	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	if(useLRP)
	{
		LRPpreprocessorWordReduction.deinitialiseActiveLRPpreprocessorMultiwordReductionTagTextCorrespondenceInfo(false);	//required for local variable access
	}
	#endif

	SHAREDvars.setCurrentDirectory(inputFolder);

	#ifdef SANI_SUPPORT_QUERIES
	if(useInputQuery)
	{
		if(!(!useInputQueryPlainTXTFile && useInputQueryXMLFile))
		{
			string inputQueryNLPfeatureXMLFileNameNOTUSED = "";
			if(!LRPpreprocessor.preprocessTextForGIAwrapper(useLRP, &inputQueryPlainTXTFileName, outputQueryLRPTextPlainTXTFileName, true, &(translatorVariablesQuery->LRPpreprocessorTranslatorVariables), &useInputQueryPlainTXTFile, inputQueryNLPfeatureXMLFileNameNOTUSED))
			{
				result = false;
			}
		}

		if(useInputQueryPlainTXTFile)
		{
			if(!SANIposRelTranslator.parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(translatorVariablesQuery))
			{
				result = false;
			}
		}
		
		#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
		if(useInputQueryXMLFile)
		{
			if(useInputQueryPlainTXTFile)
			{
				cerr << "error: useInputQueryXMLFile && useInputQueryPlainTXTFile" << endl;
				exit(EXIT_ERROR);
			}
			if(useInputQueryNLPrelationXMLFile)
			{
				cerr << "error: useInputQueryXMLFile && useInputQueryNLPrelationXMLFile" << endl;
				exit(EXIT_ERROR);
			}
			else
			{
				if(!SANIxml.readSequentialNetXMLfile(translatorVariablesQuery, inputQueryXMLFileName))
				{
					result = false;
				}
			}
		}
		else
		{
			if(useOutputQueryXMLFile)
			{
				if(!SANIxml.writeSequentialNetXMLFile(translatorVariablesQuery, outputQueryXMLFileName)
				{
					result = false;
				}
			}		
		}
		#endif

		//TODO: perform comparison of query text with non-query text
	
		#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
		if(useLRP)
		{
			LRPpreprocessorWordReduction.deinitialiseActiveLRPpreprocessorMultiwordReductionTagTextCorrespondenceInfo(true);	//required for local variable access
		}
		#endif
			
	}
	#endif

	SHAREDvars.setCurrentDirectory(outputFolder);

	#ifdef SANI_NEURAL_NETWORK
	
	#ifdef SANI_ANN
	SANIneuralNetworkOperations.generateNeuralNetFromSANInet(translatorVariables->ANNtranslatorVariables);	//generate SANI neural network
	#endif
	
	if(ANNdrawOutput)
	{
		string ANNoutputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;
		ANNdisplay.outputNeuralNetworkToVectorGraphicsAndRaytrace(translatorVariables->ANNtranslatorVariables->firstInputNeuronInNetwork, ANNuseSprites, ANNuseOutputPPMFileRaytraced, ANNdisplayInOpenGL, ANNuseOutputLDRFile, ANNuseOutputSVGFile, ANNuseOutputPPMFile, ANNoutputLDRFileName, ANNoutputSVGFileName, ANNoutputPPMFileName, ANNoutputPPMFileNameRaytraced, ANNoutputTALFileName, rasterImageWidth, rasterImageHeight);
	}
	if(ANNuseOutputXMLFile)
	{
		if(!SANIneuralNetworkOperations.writeNeuralNetXMLfile(ANNoutputXMLFileName, translatorVariables->ANNtranslatorVariables->firstInputNeuronInNetwork))
		{
			result = false;
		}
	}		
	#endif	
	
	#ifdef SANI_SUPPORT_SEQUENTIAL_NET_XML_IO
	if(useOutputTextXMLFile)
	{
		if(!SANIxml.writeSequentialNetXMLFile(translatorVariables, outputTextXMLFileName))
		{
			result = false;
		}
	}
	#endif
	
	return result;
}

#endif


