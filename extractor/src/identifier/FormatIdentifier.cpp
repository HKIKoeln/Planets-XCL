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
#include "FormatIdentifier.h"
#include "XCLException.h"

QTextStream cout(stdout, QIODevice::WriteOnly);

FormatIdentifier::FormatIdentifier():
        puid(0),
             currentFormat(0),
                           configFile("config.xml")
{
    XCLDOMBuilder dombuilder;
    configDOM=dombuilder.getDOM(configFile);
    extractionState=false;
}
/*
FormatIdentifier::FormatIdentifier(const QString& cf):
        puid(0),
             currentFormat(0),
                           configFile("config.xml")
{
    configFile=cf;
    XCLDOMBuilder dombuilder;
    configDOM=dombuilder.getDOM(configFile);
    extractionState=false;
}
*/
FormatIdentifier::~FormatIdentifier()
{}
/*
void FormatIdentifier::identify(const QString& filename, const QString& cf)
{
    currentFormat=filename;
    configFile = cf;
    identify(filename);
}
*/
void FormatIdentifier::identify(const QString& filename, const QString& xcelCurrentPath, const QString& output)
{
#ifdef PlanetsDebug
    std::cout<<"\nStarting Identifier \n";
#endif
    xcdlPath = output;
    extractionState=false;
    currentFormat=filename;		//Falls im config File command-Tag Eintrag hat, ändert sich filename
    								//File name ist dann die Ausgabe vom command-Befehl.
    
    //res/testdoc/buerozeiten.doc	-> 	buerozeiten
    QString currentFilename=filename;
    QStringList list = currentFilename.split("/");
    currentFilename=list.at(list.size()-1);
    list = currentFilename.split(".");
    currentFilename=list.at(0);
//     std::cout<<"\ncurrentFilename=:\n" <<   currentFilename.toStdString()<< "\n"; 
//     std::cout<<"filename=:\n" <<   filename.toStdString()<<"\n";
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    if((xcelCurrentPath=="" || xcelCurrentPath=="xcl/xcel/xcel_imageMagick.xml")&&(!filename.endsWith("tiffinfo.txt") && !filename.endsWith("identify-verbose.txt") ))
    {
    
        if(xcelCurrentPath=="xcl/xcel/xcel_imageMagick.xml")
        {
             // Das ImageMagick-Aufruf wird zusammengebaut
            std::string command = "identify -verbose  "+filename.toStdString()+" >identify_out.txt";
                //"identify -verbose  res/otherTestFiles/test_jpeg/image.jpg >identify_out.txt"
                
                 //Die Abfrage,ob Kommandoprozessor existiert.
            if (system(NULL))
            {
                 //Das ImageMagick-tool wird ausgeführt.
                int sytemReturnValue = system(command.c_str());
                if(sytemReturnValue!=0){
                    QString ex_message="\n\nException in FormatIdentifier.cpp\nThe process \"identify -verbose\" cannot be executed!\nTo extract the file \""+filename+"\", ensure that the file path is correct. If the input file name contains specific characters, these must be masked and the inputfile name must be set in quotation marks.\nTo execute the process \"identify -verbose\" the ImageMagick tool must be installed.\n\n";
                    throw XCLException(ex_message);
                }
                currentFormat="identify_out.txt";
            }
            
        }else{
            
            //puid: Pronom Unique (File)identifier
            //Fileformat wird identifiziert:
            puid = askDroid(filename); 		
    
            //Eine XCEL wird einem  Fileformat durch config file zugeordnet.
            QDomNodeList configPuidList = configDOM.elementsByTagName("identifier");
            //Schleife über identifiers-liste aus dem config file
            for(_UINT32  i=0; i<configPuidList .length();i++)
            {
                //Falls puid in der config vorkommt, wir die xcel zum puid ermittelt.
                if (configPuidList.item(i).toElement().text()==puid)
                {
                    QDomNode pn = configPuidList .item(i).parentNode() ;
                    pn = pn.parentNode() ;
                    xcelPath = pn.namedItem("xcel").toElement().text();

                    pn=pn.namedItem("process");
                    QString processForXCEL = pn.namedItem("command").toElement().text();

            
                    // Fall 1:	tritt bei JPEG auf. Siehe config.xml
                    if(processForXCEL!="" && (xcelPath=="xcl/xcel/xcel_imageMagick.xml"))
                    {
                
                        // Das ImageMagick-Aufruf wird zusammengebaut
                        std::string command = processForXCEL.toStdString()+"  "+filename.toStdString()+" >identify_out.txt";
                        //"identify -verbose  res/otherTestFiles/test_jpeg/image.jpg >identify_out.txt"
                
                        //Die Abfrage,ob Kommandoprozessor existiert.
                        if (system(NULL))
                        {
                            //Das ImageMagick-tool wird ausgeführt.
                            int sytemReturnValue = system(command.c_str());
                            if(sytemReturnValue!=0){
                                QString ex_message="\n\nException in FormatIdentifier.cpp\n Process \""+processForXCEL+"\" cannot be executed!\n";
                                throw XCLException(ex_message);
                            }
                            currentFormat="identify_out.txt";
                        }
                    }
            
                    // Fall 2: tritt bei doc auf. Siehe config.xml
                    if (processForXCEL != "" && xcelPath == "" )
                    {
                        std::string command;
                        if (processForXCEL == "other\\Microsoft_Conversion_Tool_0.7.5\\msct.exe" ) {
                            command = processForXCEL.toStdString() + " -i " + filename.toStdString() + " -t XCDL -o " + xcdlPath.toStdString();
                        } else {
                            command = "";
                        }
#ifdef PlanetsDebug
                        std::cout<<command<<"\n";
#endif
                        //Die Abfrage,ob Kommandoprozessor existiert.
                        if (system(NULL))
                        {
                            int sytemReturnValue = system(command.c_str());
                            if(sytemReturnValue<0){
                                QString ex_message="\n\nException in FormatIdentifier.cpp\n Process \""+processForXCEL+"\" cannot be executed!\n";
                                throw XCLException(ex_message);
                            }
                            extractionState = true;
                        }

                    }
                    if(xcelCurrentPath!="")
                        xcelPath=xcelCurrentPath;
                    break;
                }
     
      
            }

        
        }
    }
    
    if(xcelCurrentPath!="")
        xcelPath=xcelCurrentPath;   

#ifdef PlanetsDebug
    std::cout<<"\nIdentifier ready\n";
#endif
}


const QString FormatIdentifier::getCurrentFormat()
{
    return currentFormat;
}


const QString FormatIdentifier::getXCELPath()
{
    return xcelPath;
}


const QString FormatIdentifier::getXCDLPath()
{
    return xcdlPath;
}


BOOL FormatIdentifier::getExtractionState()
{
    return extractionState;
}


const QString FormatIdentifier::askDroid(const QString& filename)
{

        //Die Droid-Ausgabe  wird in die Datei DROID_out.xml geschrieben
    QString droidOutput="DROID_out.xml";
        // Das Droid-Kommandoaufruf wird zusammengebaut
    std::string command = "java -jar other/DROID-3.0/droid.jar -L"+filename.toStdString()+" -Sother/DROID-3.0/DROID_SignatureFile_V13.xml -O./DROID_out";
    if (system(NULL))
    {
            //Das Droid-tool wird ausgeführt.
//             time_t start = time(0);
        int i =remove("./DROID_out.xml");
        if(system(command.c_str())!=0)
        {
            std::cout<<"\nError: File \""<<filename.toStdString()<<"\"  could not be identified by DROID.\nTo extract the file, ensure that the file path is correct or explicitly specify the xcel by a second parameter. If the input file name contains specific characters, these must be masked and the inputfile  name must be set in quotation marks.\n\n"<<std::endl;
            exit(0);
        }
        
        
//             time_t end = time(0);
//             double  difftimeValue=difftime(end,start);
//             cout << "DROID Laufzeit "<<difftimeValue<<"sek."<< endl;
    }
    
    	//retrieve puid from DROID_out.xml.
    QFile file(droidOutput);
    QDomDocument doc(droidOutput);
    if (!file.exists()  && !file.open(QIODevice::ReadOnly))
    {
        std::cout<<"\nError: File \""<<filename.toStdString()<<"  could not be identified by DROID.\nTo extract the file, ensure that the file path is correct or explicitly specify the xcel by a second parameter. If the input file name contains specific characters, these must be masked and the inputfile  name must be set in quotation marks.\n\n"<<std::endl;
        exit(0);
    }   
    if (!doc.setContent(&file)) 
    {
        file.close();
        return "failed to create QDomDocument of  file" + droidOutput;
    }
    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNodeList nl = docElem.elementsByTagName("PUID");
    	 //hier wird angenommen, dass DROID_out.xml die Beschreibung von einem FileFormat hat, d.h. der Tag puid kommt 1x in der Datei vor.
    QDomElement  e = nl.item(0).toElement();
    puid=e.text();
    
    if (puid == "")
    {
        std::cout<<"File "<<filename.toStdString()<<"  could not be identified by DROID"<<std::endl;
        exit(0);
    }
   
    return puid;
}


