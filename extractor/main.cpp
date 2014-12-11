/*
    * This module is part of the XCL software system
    *
    * Written by Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller, Elona Weiper 2006 - 2009
    *
    * Copyright (C) 2006 - 2009 Sebastian Beyl, Volker Heydegger, Jan Schnasse, Manfred Thaller, Elona Weiper
    *
    * This program is free software; you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation; either version 2 of the License, or
    * (at your option) any later version.
    *
    * This program is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    *
*/

#include <QApplication>
#include <QDesktopWidget>
#include "XCLException.h"
#include "XCLXMLValidator.h"
#include "XCELTreeBuilder.h"
#include "XCDLWriterFactory.h"
#include "FormatIdentifier.h"
#include "Printer.h"
#include "XCLInputNormalizer.h"

#include <time.h> // Used for performance operations

#include <QFile>
#include <QTextStream>

#if defined(XERCES_NEW_IOSTREAMS)
#include <fstream>
#else
#include <fstream.h>
#endif

#include "ExtractorGUI.h"

QString setOutputFile(QString str,int& pos)
{
	QChar ch = str.at(++pos);
	while(ch == ' ')
		ch = str.at(++pos);
	QString xcdlFilename;
	while(ch != ' ')
	{
		xcdlFilename.append(ch);
		ch=str.at(++pos);
	}
	return xcdlFilename;
	
}
int main(int argc,char** arg)
{
try
{
	QString usageText =
	  " Usage:\n Extractor [Options] <fileToExtract> [xcelFilename] [outputfile]\n\n If no XCEL description is defined, the tool tries to identify\n the file type via droid and searches locally for an appropriate XCEL.\n\n If no outputfile is defined, XCDL output will be available under xcdlOutput.xcdl.\n\n Option list:\n\t-G starts a Gui for testing purposes\n\t-o specifies an outputfile.\n\t\tMake sure that it does not conflict with third parameter!\n\t-n All normData-nodes are empty\n\n";

	BOOL bDisplayNormData = TRUE;
	BOOL displayRawData   = FALSE;
	BOOL showGui          = FALSE;
	QString xcdlFilename  ="xcdlOutput.xcdl";;

	QString nargv;
	for(int i=0;i<argc;i++)
	{
		nargv.append(arg[i]);
		nargv.append('#');
	}

	// std::cout<<nargv.toStdString()<<"\n";

	BOOL initState          = TRUE;
	BOOL readOptionState    = FALSE;
	BOOL readParameterState = TRUE;
	BOOL showParseTree      = FALSE;

	QVector<QString*> parameters;
	for(int i=0;i<nargv.size();i++)
	{
		QChar ch=nargv.at(i);
		if(initState)
		{
			initState = FALSE;
			if (ch=='#')
				initState = TRUE;
			else if (ch=='-')
				readOptionState = TRUE;
			else
			{
				parameters.append(new QString());
				readParameterState = TRUE;
			}
		}
		if(readParameterState)
			if(ch=='#')
			{
				  readParameterState = FALSE;
				  initState=TRUE;
			}
			else
				parameters.last()->append(ch);
		else
			if(readOptionState)
			{
				if(ch== '#')
				{
					readOptionState = FALSE;
					initState = TRUE;
				}
				else
				{
					if(ch == 'G')
						showGui=TRUE;
					else if(ch == 'n')
					{
						bDisplayNormData = FALSE;
						// std::cout << "Ignoriere Normdaten" << std::endl;
					}
					else if(ch == 'r')
						displayRawData = TRUE;
					else if(ch == 'p')
						showParseTree=TRUE;
					else if(ch == 'o')
					{
						xcdlFilename=setOutputFile(nargv,i);
						readOptionState=FALSE;
						initState=TRUE;
					}
				}
			}
	}
	if(parameters.size() > 4)
	{	
		Printer::print("WARNING: To many parameters\n");
		Printer::print(usageText);
		return 0;	
	}

	QString xcelFilename;
	QString filename;
  
	BOOL extractionState=false;


	FormatIdentifier fi;
	if(parameters.size() == 4)
	{
		filename = *(parameters.at(1));
		xcelFilename = *(parameters.at(2));
		xcdlFilename = *(parameters.at(3));
		fi.identify(filename, xcelFilename, xcdlFilename);
		xcelFilename=fi.getXCELPath();
		filename=fi.getCurrentFormat();
		extractionState=fi.getExtractionState();
		xcdlFilename=fi.getXCDLPath();
	}
	else if(parameters.size() == 3)
	{
		filename = *(parameters.at(1));
		xcelFilename = *(parameters.at(2));
    
		QStringList list = filename.split("/");
		QString currentXCDLFilename= (list.at(list.size()-1));
		xcdlFilename = "output/"+currentXCDLFilename+".xcdl";

		fi.identify(filename, xcelFilename, xcdlFilename);
		xcelFilename=fi.getXCELPath();
		filename=fi.getCurrentFormat();
		extractionState=fi.getExtractionState();
		xcdlFilename=fi.getXCDLPath();
	}
	else if(parameters.size() == 2)
	{
		//  xcdlFilename="xcdlOutput.xcdl";
		filename=*(parameters.at(1));
      
		QStringList list = filename.split("/");
		QString currentXCDLFilename= (list.at(list.size()-1));
		xcdlFilename = "output/"+currentXCDLFilename+".xcdl";

		fi.identify(filename, xcelFilename, xcdlFilename);
		xcelFilename=fi.getXCELPath();
		filename=fi.getCurrentFormat();
		xcdlFilename=fi.getXCDLPath();
		extractionState=fi.getExtractionState();
	}
	else
	{
#ifdef PlanetsDebug
//		xcdlFilename="xcdlOutput.xcdl";
		filename="res/testpng/ps1n0g08.png";
      
		QStringList list = filename.split("/");
		QString currentXCDLFilename= (list.at(list.size()-1));
		xcdlFilename = "output/"+currentXCDLFilename+".xcdl";

		fi.identify(filename,xcelFilename,xcdlFilename);
		xcdlFilename= fi.getXCDLPath();
		xcelFilename=fi.getXCELPath();
		filename=fi.getCurrentFormat();
		xcdlFilename=fi.getXCDLPath();
		extractionState=fi.getExtractionState();
#else
		Printer::print(usageText);
		return 0; // Das Programm wird an dieser Stelle beendet!
#endif
	}


	XCDLWriterFactory factory("config.xml");
	factory.setDisplayNormData(bDisplayNormData);		// Hier wird festgelegt, ob Normdaten angezeigt werden.

	/*
	QVector<QString> ignoredProperties;
	QVector<QString> definedProperties;
	ignoredProperties=factory.getIgnoredProperties();
	definedProperties=factory.getDefinedProperties();
	if(ignoreNormData)
	{
	  ignoredProperties.append("normData");
	  for(int i=0;i<definedProperties.size();i++)
	    if(definedProperties.at(i)=="normData")
	      definedProperties.remove(i);
	}
	if(displayRawData)
	  factory.setRawDataDisplay(TRUE);

	factory.setDefinedProperties(definedProperties);
	factory.setIgnoredProperties(ignoredProperties);
	*/

	Printer::print("------------------------------------------\n");
	Printer::print("File to extract:"+filename+"\n");
	Printer::print("XCEL: "+xcelFilename+"\n");
	Printer::print("XCDL: "+xcdlFilename+"\n");
	Printer::print("------------------------------------------\n");

	if(!showGui)
	{
		if(!filename.isEmpty() && !xcelFilename.isEmpty()&& extractionState==false)
		{
			XCELTreeBuilder builder;
			XCLParsingItem parseTree;
			try
			{
				XMLPlatformUtils::Initialize();
			}
			catch (const XMLException& toCatch)
			{
				char* message = XMLString::transcode(toCatch.getMessage());
				std::cout << "Error during initialization! :\n"    << message << "\n";
				XMLString::release(&message);
			}

			try
			{
				std::cout<<"Generate internal Representation...\n";
				parseTree=builder.build(xcelFilename,filename);
				std::cout<<"Generate XCDL...";
	
				XCDLWriter* writer = factory.create();
				writer->write(parseTree,xcdlFilename);
				std::cout<<"ready\n";
				if(showParseTree)
				{
					std::cout<<"SHOW ParseTree\n";
					QFile parseTreeFile("parseTree.log");
					parseTreeFile.open(QIODevice::WriteOnly);
					QTextStream stream(&parseTreeFile);
					parseTree.print(stream);  
					parseTreeFile.close(); 
					// parseTree.print();
				}
			}
			catch(XCLException excp)
			{
				excp.message();
				std::cout<<"Return 1\n";
				XMLPlatformUtils::Terminate();
				return 1;
			}
				XMLPlatformUtils::Terminate();
			}
		else
		{
			std::cout<<"WARNING: To few parameters\n";
			std::cout<<usageText.toStdString();
		}
		std::cout << "Clock Apprun: " << clock();
	return 0;
	}
	else // if showGui
	{
		try
		{
			QApplication app(argc, arg);
			QDesktopWidget* desk=app.desktop();
			QRect rect=desk->screenGeometry();

			ExtractorGUI main(&factory,rect.width(),rect.height());
			if(!filename.isEmpty() && !xcelFilename.isEmpty()&& extractionState==false)
			{
				main.findBinaries(QStringList(filename));
				main.findXCEL(xcelFilename);
				main.go();
			}
			app.setActiveWindow(&main);
			main.show();

			return app.exec();
		}
		catch(XCLException exception)
		{
			return 2;
		}
	}
}
catch(XCLException exception)
{
	exception.message();
	exit(22);
}
catch(...)
{
	exit(23);
}


}

