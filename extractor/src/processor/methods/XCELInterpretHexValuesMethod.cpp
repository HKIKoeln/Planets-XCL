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
#include "XCELInterpretHexValuesMethod.h"
#include "XCLCalculator.h"
#include "XCELProcessorInterface.h"
#include "XCLParsingSymbol.h"
#include <string>
#include <iostream>
using namespace std;



BOOL* XCELInterpretHexValuesMethod::getMode()
{
    BOOL mod[]= {FALSE,TRUE,FALSE,TRUE};
    return mod;
}



int XCELInterpretHexValuesMethod::expectedParameters()
{
    return 2;
}


BOOL XCELInterpretHexValuesMethod::preprocess(XCELReader* reader, XCLSyntaxItem& grammar, const XCLSyntaxProcess& preprocessing)
{
    return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)reader;
  (void)grammar;
  (void)preprocessing;
}


BOOL XCELInterpretHexValuesMethod::process(	XCELReader* reader,
        XCLSyntaxExpression* expr,
        QVector<XCLProcessParameter> param,
        QHash<QString,IndexedVector*> vectorHash)

{

    //std::cout<<"\nin XCELInterpretAsNumberMethod \n";
    FileParserState* state = reader->getState();
    bool check=FALSE;
    
  
    XCLProcessParameter p2=param.at(0);
    QString fontIdentifier = p2.getValue(state->index);

//     printFontObject( reader);
    
    
    XCLParsingExpression* fontObject =getFontObject(reader, fontIdentifier);
    
    
    //falls encoding existiert, wird encodingItem zurückgeliefert, sonst NULL
    XCLParsingExpression* encodingExpr = getChild(fontObject, "encoding");
    //Name von der vordefinierten CMap
    QString encoding ="";
    
    if(encodingExpr!=NULL)
    {
        encoding = encodingExpr->getInterpretedValue();
//         std::cout<<"\nencoding non Fx = "<<encoding.toStdString()<<"\n";
        if(encoding == "WinAnsiEncoding") encoding = "Windows-1252";
        else if(encoding == "MacRomanEncoding") encoding = "Apple Roman";
        else if(encoding == "StandardEncoding") encoding = "ISO 8859-1";
        else if(encoding == "PDFDocEncoding") encoding = "UTF-16";
        else encoding="";

    }
    
     //if(encoding=="")
    //falls cMap existiert, wird cMap vom Font 'fontIdentifier'(=F1, F2, F3)geliefert;
    //sonst ist der return-Wert NULL.
    XCLParsingExpression* fontCMapExpr = getChild(fontObject, "cMap");



   


   //if(encoding=="")
	//hohle CMap, falls die CMap nicht vorhanden ist, wird NULL zurückgeliefert
// 	XCLParsingExpression*  fontCMapExpr  = getCMap(reader, fontIdentifier);
    ushort cMapIndexLegth=0;
    if( fontCMapExpr!=NULL){
        cMapIndexLegth =getCMapIndexLength( fontCMapExpr);
//         std::cout<<"cMapLength  "<< cMapIndexLegth<<"\n";
    }else {
        cMapIndexLegth=2;
    }

    XCLParsingExpression* pExp= state->index.get(expr->getIdentifier())->last();;
    QString interpretedValue = pExp->getInterpretedValue();
//     std::cout<<"(hex) interpretedValue:  "<< interpretedValue.toStdString()<<"\n";
	
    QString*  encodedStr=new QString();    
    QString hex;
    
    for(int i=0;i<interpretedValue.size();i+=cMapIndexLegth)
    {
        hex = interpretedValue.mid(i,cMapIndexLegth);
        QString value="";
        if( fontCMapExpr!=NULL){
//             std::cout<<"\n====CMAP ist nicht NULL=====\n";
            value= getValue( fontCMapExpr, hex);			
            if(value!="")
                encodedStr->append(value);
            else encodedStr->append(" ");
			
        }else 
        {
//             std::cout<<"\n====CMAP ist NULL=====\n";
            hex="00"+hex;
            value=getEncoding(hex, encoding);
            if(value!="")
                encodedStr->append(value);
            else encodedStr->append(" ");

        }
    }
   
    pExp->setInterpretedValue(*encodedStr);
    interpretedValue = pExp->getInterpretedValue();
//     std::cout<<"(encoded)interpretedValue:  "<< interpretedValue.toStdString()<<"\n";


	/*#ifdef PlanetsDebug
    std::cout<<pExp->getInterpretedValue().toStdString();
#endif*/
	
   
    return check;
    // useless statements for less compiler-warnings, params are in use
    (void)vectorHash;
}


BOOL XCELInterpretHexValuesMethod::postprocess( XCLParsingItem& parseTree,
                                               QVector<XCLProcessParameter> param,
                                               QVector<XCLParsingExpression*> referenceData,
                                               FileParserState& state,
                                               QHash<QString,IndexedVector*> vectorHash)
{
  return FALSE;
  // useless statements for less compiler-warnings, params are in use
  (void)parseTree;
  (void)param;
  (void)referenceData;
  (void)state;
  (void)vectorHash;
}


//Diese Methode getCMap() liefert die aktuelle cMap zu einem Font-Objekt(=fontIdentifier)
// return value von getCMap() == return value von getChild(), siehe unten getChild(XCLParsingExpression* pExpr, QString identifier).
//
//XCLParsingExpression* getCMap() : 1) hohle alle cMap Expressions, 2) finde raus welche CMap zu dem aktuellen Fontobject gehört.
//fontIdentifier = Fontobject name, z.B F1, F2 oder F3
// Kein Aufruf in XCELInterpretHexValuesMethod().
XCLParsingExpression* XCELInterpretHexValuesMethod::getCMap(XCELReader* reader, QString fontIdentifier)
{
    
    FileParserState* state = reader->getState();
    
    //1) hohle alle cMap Expressions
    QList<XCLParsingExpression*>* cMapsList = state->index.get("cMap");
    
    XCLParsingExpression* cMapExpr = NULL;
    //Schleife über alle CMaps im ParseTree
    for(int i=0;i<cMapsList->size();i++)
    {
// 	std::cout<<"\n CMapNr=="<<cMapsList->at(i)->getIdentifier().toStdString()<<"\n";	
        cMapExpr = cMapsList->at(i);;
        QVector<XCLParsingExpression*> cMapChildren=((XCLParsingItem*)(cMapExpr))->getChildren();
	
        //2) firstChild gibt Informationen ueber FontObject Namen(=fontMarker=fontChange).
        XCLParsingExpression* firstChild = cMapChildren.at(0);		
        if(firstChild->getInterpretedValue() == fontIdentifier){
           return cMapExpr;
        }
    }
    
    return NULL;
}


ushort XCELInterpretHexValuesMethod::getCMapIndexLength(XCLParsingExpression* cMap){

    QString value="";
    QVector<XCLParsingExpression*> cMapChildren=((XCLParsingItem*)(cMap))->getChildren();
	//Schleife ueber CMap Entries:
	//k=1, da firstChild der Liste ist CMap-FontIdentifier.Dies wurde ausgelesen in getCMap().
	//Rest der Liste sind CMap Eintries mir key & value als Subknoten
    for(int k=1;k<cMapChildren.size();k++)
    {
        XCLParsingExpression* child = cMapChildren.at(k);
		
        if(child->getType()==XCLSyntaxElement::ITEM_TYPE && child->getIdentifier()=="cMapEntry")
        {
            QVector<XCLParsingExpression*> cMapEntryList=((XCLParsingItem*)(child))->getChildren();
            XCLParsingExpression* cMapEntry = cMapEntryList.at(0);
            
            //schleife ueber "cMapEntry"Knoten:
            //Schleife, da in "cMapEntry"Knoten auch andere Knoten wie extrc_ enthalten sein können.
            for(int k2=0;k2<cMapEntryList.size();k2++)
            {
                cMapEntry = cMapEntryList.at(k2);
                if(cMapEntry->getIdentifier()=="cMapKey"){
                    QString key_ascii = getInterpretationASCII(cMapEntry->getInterpretedValue());
 
                    ushort l= key_ascii.size();
//                     std::cout<<"\ncMap index length="<<key_ascii.toStdString()<<"="<<l<<"\n";
                    return l;
                }
            }
        }
    }


    return 0;
}

//Methode entschlüsselt eine hex Zahl anhand cMap
//cMap: aktuelle cMap
//key: Index von cMap
 QString XCELInterpretHexValuesMethod::getValue(XCLParsingExpression* cMap, QString key){
	
     QString value="";
     QVector<XCLParsingExpression*> cMapChildren=((XCLParsingItem*)(cMap))->getChildren();
	//Schleife ueber CMap Entries:
	//k=1, da firstChild der Liste ist CMap-FontIdentifier.Dies wurde ausgelesen in getCMap().
	//Rest der Liste sind CMap Eintries mir key & value als Subknoten
     for(int k=1;k<cMapChildren.size();k++)
     {
         XCLParsingExpression* child = cMapChildren.at(k);
		
         if(child->getType()==XCLSyntaxElement::ITEM_TYPE && child->getIdentifier()=="cMapEntry")
         {
             QVector<XCLParsingExpression*> cMapEntryList=((XCLParsingItem*)(child))->getChildren();
             XCLParsingExpression* cMapEntry = cMapEntryList.at(0);
             
            //schleife ueber "cMapEntry"-Knoten:
            //Schleife, da in "cMapEntry"Knoten auch andere Knoten wie 'extrc_' enthalten sein können.
             for(int k2=0;k2<cMapEntryList.size();k2++)
             {
                 cMapEntry = cMapEntryList.at(k2);
                 if(cMapEntry->getIdentifier()=="cMapKey"){
				
                     QString key_ascii = getInterpretationASCII(cMapEntry->getInterpretedValue());
                     
                     if(key_ascii==key){
                         k2++; 
                       
                         for(;k2<cMapEntryList.size();k2++)
                         {
                             cMapEntry = cMapEntryList.at(k2);
                             if(cMapEntry->getIdentifier()=="cMapValue")
                             {
                                 QString value_ascii = getInterpretationASCII(cMapEntry->getInterpretedValue());
                                 value = getUTF8(value_ascii);
//                                  std::cout<<"\ningetValue() key===== "<<key.toStdString()<<"=>ox"<<value_ascii.toStdString()<<"= "<<value.toStdString()<<std::endl;;
                                 return value;
                             }
                         }
                     }
                 }
             }
         }
     }
     return value;
	
 }
 
 
//kein Aufruf
 QString XCELInterpretHexValuesMethod::getInterpretationASCII(QString decStr){
	
     QString ascii="";
	
     QString dec="";
     bool check=FALSE;
     int a=0;
     decStr.remove(QChar(' '));
     for(int i=0;i<decStr.size();i+=2)
     {
         check=FALSE;
         dec = decStr.mid(i,2);
         a = dec.toInt( &check,10);
         if(check==TRUE)
             ascii.append((char) a);
         else {
             ascii.append("x");
         }
		//std::cout<<"Interpretation: "<<a<<" -> "<<(char)a<<"\n";
     }
	
     return ascii;
 }

 //kein Aufruf
 //Diese Methode wandelt eine hex-Zahl(=utf-16) ins utf-8 String.
 QString XCELInterpretHexValuesMethod::getUTF8(QString unicodeStr){
	
     QString strUTF8="";
     ushort hex;
     QString hexStr="";
     hexStr.append("0x");
     hexStr.append(unicodeStr);
     bool ok=false;
     hex = hexStr.toShort(&ok, 16);
     if(ok==true){
         QChar ch=QChar(hex);
         QString qs= ch;//utf-16

         QByteArray qba=qs.toUtf8 ();
         QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
         QString f=qba.data();
        //f.normalized(QString::NormalizationForm_C);
        // f.normalized(QString::NormalizationForm_D);
        //f.normalized(QString::NormalizationForm_KC);
         f.normalized(QString::NormalizationForm_KD);

	//std::cout<<"\nnormalizedStr="<<f.toStdString()<<std::endl;
         strUTF8.append(f);
     }	
     
//      std:: cout << "encoded String: "<<hexStr.toStdString()<<" -> "<<strUTF8.toStdString()<<"|"<< std::endl;
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-16"));
     return strUTF8;
 }

 
  //Diese Methode wandelt eine hex-Zahl(=utf-16) ins erwünschte 'encoding'.
 //unicodeStr ist eine hex-Zahl(=utf-16) 
 //encoding ist z.B. Windows-1252, Apple Roman, ISO 8859-1, UTF-16
 QString XCELInterpretHexValuesMethod::getEncoding(QString unicodeStr, QString encoding){
	
     QString strUTF8="";
     ushort hex;
     QString hexStr="";
     hexStr.append("0x");
     hexStr.append(unicodeStr);
     bool ok=false;
     hex = hexStr.toShort(&ok, 16);
     if(ok==true){
         QByteArray codecQba=encoding.toAscii();
         QTextCodec::setCodecForCStrings(QTextCodec::codecForName(codecQba));
         QChar ch=QChar(hex);


         QString qs= ch;

         QByteArray qba=qs.toUtf8 ();
         QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
         QString f=qba.data();
        //f.normalized(QString::NormalizationForm_C);
        // f.normalized(QString::NormalizationForm_D);//leerzeichen
        //f.normalized(QString::NormalizationForm_KC);//leerzeichen
         f.normalized(QString::NormalizationForm_KD);//leerzeichen

         strUTF8.append(f);
     }	
  //   std:: cout << "IN getEncoding \nencoded String: "<<hexStr.toStdString()<<" ->"<<strUTF8.toStdString()<<"|"<< std::endl;
     return strUTF8;
 }


 void XCELInterpretHexValuesMethod::printFontObject(XCELReader* reader){

     FileParserState* state = reader->getState();
     QList<XCLParsingExpression*>* fontObjectList = state->index.get("fontDefinitions");

     for(int i=0;i<fontObjectList->size();i++)
     {	

         XCLParsingExpression* fontObjectExpr = fontObjectList->at(i);
         QVector<XCLParsingExpression*> fontObjectChildren=((XCLParsingItem*)(fontObjectExpr))->getChildren();
         std::cout<<"\n***********fontDefinitions******* "<<fontObjectExpr->getIdentifier().toStdString()<<std::endl;
         for(int ii=0;ii<fontObjectChildren.size();ii++)
         {
             XCLParsingExpression* child = fontObjectChildren.at(ii);	
             std::cout<<"\nidentifier= "<<child->getIdentifier().toStdString()<<"\t interpretedValue=";
             if(child->getIdentifier()=="fontIdentifierSubst")
                 std::cout<<child->getInterpretedValue().toStdString();

             std::cout<<"\n";

         }
     }




 }
 
 //Diese Methode gibt zurück den aktuellen Fontobject.
 //fontIdentifier ist identifier vom aktuellen Fontoject, das gesucht wird..
 XCLParsingExpression* XCELInterpretHexValuesMethod::getFontObject(XCELReader* reader, QString fontIdentifier){

     FileParserState* state = reader->getState();
     QList<XCLParsingExpression*>* fontObjectList = state->index.get("fontDefinitions");
     XCLParsingExpression* fontObjectExpr = NULL;

     for(int i=0;i<fontObjectList->size();i++)
     {	
         fontObjectExpr = fontObjectList->at(i);
         QVector<XCLParsingExpression*> fontObjectChildren=((XCLParsingItem*)(fontObjectExpr))->getChildren();
         
         for(int ii=0;ii<fontObjectChildren.size();ii++)
         {
             XCLParsingExpression* child = fontObjectChildren.at(ii);	
             if(child->getIdentifier() == "fontIdentifierSubst")
             {
                 
                 if(child->getInterpretedValue()==fontIdentifier)
                 {
//                      std::cout<<"\nfontIdentifierSubst= "<<child->getInterpretedValue().toStdString()<<"?="<<fontIdentifier.toStdString()<<"\n";
                     return fontObjectExpr;
                 }
             }
         }
     }

     return NULL;
 }

 //Diese Methode gibt zurück einen Unterknoten des aktuellen Objektes
 //pExpr ist aktuelles Objekt(=XCLParsingExpression), in dem gesucht wird.
 //identifier ist id des Sub-Expressions.
 XCLParsingExpression* XCELInterpretHexValuesMethod::getChild(XCLParsingExpression* pExpr, QString identifier){

     XCLParsingExpression* child = NULL;	
	
     QVector<XCLParsingExpression*> pExprChildren=((XCLParsingItem*)(pExpr))->getChildren();
     for(int i=0;i<pExprChildren.size();i++)
     {
         child = pExprChildren.at(i);	

         if(child->getIdentifier()== identifier)
         {
			
             return child;
			
         }
     }


     return NULL;
 }






