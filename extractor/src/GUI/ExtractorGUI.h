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
#ifndef __ExtractorGUI_H
#define __ExtractorGUI_H
#include <QThread>
#include <QMainWindow>
#include <QUrl>
#include "XCELTreeBuilder.h"
#include "XCDLWriterFactory.h"
#include "XCLXMLValidator.h"


class QScrollView;
class QLabel;
class QToolBar;
class QAction;
class ObjectDisplay;
class QStatusBar;
class QLineEdit;
class QTableWidget;
class QCheckBox;
class QTabWidget;
class QVBoxLayout;
class QTextEdit;
class QTreeWidgetItem;
class QTextBrowser;
class ExtractorGUI : public QMainWindow
{
  Q_OBJECT
public:
  ExtractorGUI(XCDLWriterFactory* fact,int w=800, int h=600,QWidget* parent=0, const char* name=0);
  ~ExtractorGUI()
  {
    		XMLPlatformUtils::Terminate();
  }
private:

  QString xcelFileName;
  QStringList binaryFileNames;

  QString targetDirectory;

  XCELTreeBuilder* builder;
  XCLParsingItem parseTree;
XCLParsingItem debugTree;
  XCDLWriterFactory* factory;
  XCDLWriter* writer;

QVector<QCheckBox*> boxes;
QVector<QString> properties;

BOOL chooseXCEL;
 QTabWidget* tabWidget;
QVector<QTextBrowser*>allEditors;
QTextBrowser* editor; 
QString actStr;
private:
  void createActions();
  void createMenus();
  void createToolBars();

  void resizeEvent(QResizeEvent* rE);
  void createStatusBar();

  QAction* gotToNormData;
  QAction* findXCELAct;
  QAction* preferencesAct;
  QAction* findBinariesAct;
  QAction* goAct;
  QAction* identifyAct;
  QAction* closeAllTabsAct;
  QAction* closeTabAct;
  QAction* showParseTreeViewAct;
  QAction* showDebugTreeViewAct;
  QStatusBar* statusBar;


  QSize mySize;
  QLineEdit* textField;
  QTableWidget* table;
  QVBoxLayout* mainLayout;
  int textSize;
  int textWeight;
 void identify();
 void buildTreeWidget(XCLParsingItem* expr, QTreeWidgetItem* parent);
private slots:
  void findXCEL();
  
  void findBinaries();
  
  
 
  void identifySlot();
  void setPreferences();
  void myResize();
  void checkProperty(int box);
  void closeAllTabs();
  void closeTab();
  void setTextSize(const QString &);
  void setTextWeight(const QString &);
  void showDebugTreeView();
  void showParseTreeView();
  void goToNormData(const QUrl& u);

public slots:
	void findBinaries(const QStringList& filename);
	void findXCEL(const QString& filename);
	void go();
	
};


#endif
