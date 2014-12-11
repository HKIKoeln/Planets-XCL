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
#include <iostream>
#include <QLayout>
#include <QMenuBar>
#include <QLabel>
#include <QString>
#include <QAction>
#include <QTextEdit>
#include <QScrollArea>
#include <QFileDialog>
#include <QApplication>
#include <QPoint>
#include <QTabWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopWidget>
#include <QStatusBar>
#include <QTableWidget>
#include <QToolBar>
#include <QComboBox>
#include <QFontDatabase>
#include <QList>
#include <QTreeWidget>
#include <QTextBrowser>
#include "ExtractorGUI.h"
#include "FormatIdentifier.h"
#include "MyXMLSyntaxHighlighter.h"
#include "Defines.h"
#include "NamesLibProperty.h"

ExtractorGUI::ExtractorGUI(XCDLWriterFactory* fact, int w, int h, QWidget* parent, const char* name)
    :QMainWindow(parent)
{
  builder = new XCELTreeBuilder();
  factory = fact;
  writer = factory->create();

  statusBar=QMainWindow::statusBar();
  QFrame* frame= new QFrame(this);
  mainLayout = new QVBoxLayout();
  QGridLayout* bottomLayout=new QGridLayout();

  table = new QTableWidget(this);
  QLabel* xcelLabel =   new QLabel("   XCEL");
  textField = new QLineEdit();
  QPushButton* chooseXCELButton = new QPushButton("...");
  QPushButton* chooseBinariesButton = new QPushButton("...");
  QPushButton* button = new QPushButton("Find XCEL");
  QPushButton* startButton = new QPushButton("  G O  ");

  button->setMaximumWidth(100);

  startButton->setMinimumHeight(100);

  bottomLayout->addWidget(table,0,0,5,1);
  bottomLayout->addWidget(chooseBinariesButton,0,1);


  bottomLayout->addWidget(xcelLabel,1,2,1,1);
  bottomLayout->addWidget(textField,2,2,1,1);
  bottomLayout->addWidget(chooseXCELButton,2,3,1,1);
  //bottomLayout->addWidget(button,3,2,1,1); //ARR doesn´t work on Win32
  bottomLayout->addWidget(startButton,4,3,1,2);

  tabWidget = new QTabWidget();

  mainLayout->addWidget(tabWidget);
  mainLayout->addLayout(bottomLayout);


  frame->setLayout(mainLayout);

  setCentralWidget(frame);
  mySize=QSize(800,600);
  myResize();
  move(0,0);
  createActions();
  createMenus();
  createToolBars();
  update();

  connect(chooseXCELButton,SIGNAL(released()),this,SLOT(findXCEL()));
  connect(chooseBinariesButton,SIGNAL(released()),this,SLOT(findBinaries()));
  connect(startButton,SIGNAL(released()),this,SLOT(go()));
  connect(button,SIGNAL(released()),this,SLOT(identifySlot()));

  // Useless Block, Parameters not in use, for less compiler-warnings
  // Parameters: int w, int h, const char* name
  (void)name;
  (void)w;
  (void)h;
}


void ExtractorGUI::resizeEvent(QResizeEvent* rE)
{
  QMainWindow::resizeEvent(rE);
}


void ExtractorGUI::createActions()
{
  findXCELAct = new QAction(tr("&Choose XCEL..."),this);
  findXCELAct->setStatusTip(tr("Choose XCEL"));
  connect(findXCELAct, SIGNAL(activated()), this, SLOT(findXCEL()));

  findBinariesAct = new QAction(tr("&Open"),this);
  findXCELAct->setStatusTip(tr("Open Binaries"));
  connect(findBinariesAct, SIGNAL(activated()), this, SLOT(findBinaries()));

  preferencesAct = new QAction(tr("&Preferences..."),this);
  preferencesAct->setStatusTip(tr("Preferences"));
  connect(preferencesAct, SIGNAL(activated()), this, SLOT(setPreferences()));

  goAct = new QAction(tr("&RUN..."),this);
  preferencesAct->setStatusTip(tr("RUN"));
  connect(goAct, SIGNAL(activated()), this, SLOT(go()));

  identifyAct = new QAction(tr("&Identify Binary..."),this);
  identifyAct->setStatusTip(tr("Identify Binary"));
  connect(identifyAct, SIGNAL(activated()), this, SLOT(identifySlot()));

  closeAllTabsAct = new QAction(tr("&Close All Tabs"),this);
  closeAllTabsAct->setStatusTip(tr("Close All Tabs"));
  connect(closeAllTabsAct, SIGNAL(activated()), this, SLOT(closeAllTabs()));

  closeTabAct = new QAction(tr("&Close Tab"),this);
  closeTabAct->setStatusTip(tr("Close Tab"));
  connect(closeTabAct, SIGNAL(activated()), this, SLOT(closeTab()));

  showDebugTreeViewAct = new QAction(tr("&Open DebugTree View"),this);
  showDebugTreeViewAct->setStatusTip(tr("Debugging information"));
  connect(showDebugTreeViewAct, SIGNAL(activated()), this, SLOT(showDebugTreeView()));

  showParseTreeViewAct = new QAction(tr("&Open ParseTree View"),this);
  showParseTreeViewAct->setStatusTip(tr("Debugging information"));
  connect(showParseTreeViewAct, SIGNAL(activated()), this, SLOT(showParseTreeView()));

}


void ExtractorGUI::createMenus()
{
  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(findBinariesAct);

  QMenu* actionsMenu = menuBar()->addMenu(tr("&Actions"));
  actionsMenu->addAction(identifyAct); //ARR doesn´t work on Win32
  actionsMenu->addAction(goAct);
  actionsMenu->addAction(findXCELAct);

  QMenu* optionsMenu = menuBar()->addMenu(tr("&Options"));
  optionsMenu->addAction(preferencesAct);

  QMenu* windowsMenu = menuBar()->addMenu(tr("&Windows"));
  windowsMenu->addAction(closeAllTabsAct);
  windowsMenu->addAction(closeTabAct);
#ifdef PlanetsDebug
  QMenu* debugMenu = menuBar()->addMenu(tr("&Debug"));
  debugMenu->addAction(showDebugTreeViewAct);
  debugMenu->addAction(showParseTreeViewAct);
#endif
}


void ExtractorGUI::createToolBars()
{
  QToolBar* toolBar = addToolBar("Font");
  QComboBox* textSizeComboBox =new QComboBox();
  textSizeComboBox->setMinimumWidth(100);
  toolBar->addAction(findBinariesAct);
  toolBar->addSeparator();

  textSizeComboBox->insertItem(0,"10");
  textSizeComboBox->insertItem(1,"12");
  textSizeComboBox->insertItem(2,"14");
  textSizeComboBox->insertItem(3,"18");
  textSizeComboBox->insertItem(4,"20");
  textSize=12;
  toolBar->addWidget(new QLabel("Text size"));
  toolBar->addWidget(textSizeComboBox);
  connect(textSizeComboBox,SIGNAL(activated ( const QString & )),this,SLOT(setTextSize(const QString &)));

  QComboBox* textWeightComboBox =new QComboBox();
  textWeightComboBox->setMinimumWidth(100);

  textWeightComboBox->insertItem(0,"25");
  textWeightComboBox->insertItem(1,"50");
  textWeightComboBox->insertItem(2,"63");
  textWeightComboBox->insertItem(3,"75");
  textWeightComboBox->insertItem(4,"87");
  textWeight=50;
  toolBar->addWidget(new QLabel("Text weight"));
  toolBar->addWidget(textWeightComboBox);
  connect(textWeightComboBox,SIGNAL(activated ( const QString & )),this,SLOT(setTextWeight(const QString &)));
}


void ExtractorGUI::setPreferences()
{

  QVector<QString> ignoredProperties;
  QVector<NamesLibProperty*> definedProperties;

  boxes.clear();
  properties.clear();

  ignoredProperties=factory->getIgnoredProperties();
  definedProperties=factory->getDefinedProperties();


  QDialog* dialog = new QDialog(this);
  QScrollArea* sv = new QScrollArea(dialog);
  sv->setMinimumSize(200,600);
  dialog->setMinimumSize(210,600);
  dialog->setMaximumSize(210,600);

  QFrame* frame = new QFrame();
  QGridLayout* mmainLayout = new QGridLayout(frame);
  int dPSize=definedProperties.size();


  for(int i=0;i<dPSize;i++)
  {
    QCheckBox* checkBox = new QCheckBox(definedProperties.at(i)->getPropertyValue());
    checkBox->setCheckState(Qt::Checked);
    mmainLayout->addWidget(checkBox,i,0);
    boxes.append(checkBox);
    //connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(checkProperty(int)));
  }

  for(int i=0;i<ignoredProperties.size();i++)
  {
    QCheckBox* checkBox = new QCheckBox(ignoredProperties.at(i));
    checkBox->setCheckState(Qt::Unchecked);
    mmainLayout->addWidget(checkBox,i+dPSize,0);
    boxes.append(checkBox);
    //connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(checkProperty(int)));
  }
  properties.clear();
  for (int i = 0; i < definedProperties.size(); ++i) {
      QString s=(definedProperties.at(i))->getPropertyValue();
      properties+=s;
  }
//   properties+=definedProperties;
  properties+=ignoredProperties;

  frame->setLayout(mmainLayout);
  sv->setWidget(frame);

  connect(dialog, SIGNAL(finished(int)), this, SLOT(checkProperty(int)));

  dialog->show();
}


void ExtractorGUI::findXCEL(const QString& f)
{
  xcelFileName=f;
  textField->setText(xcelFileName);
  chooseXCEL=TRUE;
}


void ExtractorGUI::findXCEL()
{
  QString filename;
  filename=QFileDialog::getOpenFileName(this,QString(),"xcl/xcel/");
  if(filename.isNull())return;
  findXCEL(filename);
}


void ExtractorGUI::findBinaries()
{
  binaryFileNames.clear();
  QStringList bFileNames=QFileDialog::getOpenFileNames(this,QString(),"res");
  if(bFileNames.isEmpty())return;
  findBinaries(bFileNames);
}


void ExtractorGUI::findBinaries(const QStringList& filename)
{
  binaryFileNames.clear();
  QStringList f=filename;
  table->setRowCount(f.size());
  table->setColumnCount(1);
  table->setMinimumWidth(200);
  QFileInfo info;
  info.setFile(f.at(0));
  QString suffix = info.suffix();

  for (int i=0;i<f.size();i++)
  {
    info.setFile(f.at(i));
    if(suffix!=info.suffix())
    {
      QMessageBox::warning(this, tr("Heterogen material is not permitted"),
                           "This is alpha software "+info.fileName()+" will be excluded\n",
                           QMessageBox::Yes);
      continue;
    }
    binaryFileNames.append(f.at(i));
    QTableWidgetItem* tableItem=new QTableWidgetItem(info.fileName());

    table->setItem(i,0,tableItem);
    table->setMinimumWidth(200);
  }
  chooseXCEL=FALSE;
  textField->clear();
}


void ExtractorGUI::go()
{
  try
  {
    if(chooseXCEL==FALSE)
    {
      int ret = QMessageBox::warning(this, tr("Please choose an XCEL"),
                                     tr("Try to find an XCEL via DROID?\n"),
                                     QMessageBox::Yes | QMessageBox::No);
      if(ret == QMessageBox::Yes)
      {
        identify();

      }
      else
      {
        // ARR doesn´t work at Win32
        ret = QMessageBox::warning(this, tr("Please choose an XCEL"),
                                   tr("Please try to find an XCEL\n"),
                                   QMessageBox::Yes);
        if(ret == QMessageBox::Yes)
          findXCEL();
        else
          return;
      }

    }
  }
  catch ( XCLException exception)
  {
    return;
  }


  QString* filename;
  QString* xcdlFilename;
  try
  {
    QFileInfo info;

    for (int i=0; i<binaryFileNames.size();i++)
    {
      info.setFile(binaryFileNames[i]);
      filename = new QString(binaryFileNames[i]);
      xcdlFilename = new QString("output/"+(info.fileName()).remove(QChar('\\'))+".xcdl");


      parseTree=builder->build(xcelFileName,*filename);
#ifdef PlanetsDebug
      debugTree=builder->getDebugTree();
      debugTree.print();
#endif

#ifdef PlanetsDebug
      std::cout<<"Generate XCDL...\n";
#endif
      writer = factory->create();
      QDomDocument xcdlDoc = writer->write(parseTree,*xcdlFilename);
      delete writer;
      editor = new QTextBrowser(tabWidget);
      editor->setFontWeight(textWeight);
      editor->setFontPointSize(textSize);
      connect(editor,SIGNAL(anchorClicked(const QUrl&)),this,SLOT(goToNormData(const QUrl&)));

      QDomNodeList propSets = xcdlDoc.elementsByTagName("propertySet");
      for(int j = 0;j<propSets.size();j++)
      {

        QDomElement element = propSets.at(j).toElement().elementsByTagName("dataRef").at(0).toElement();

        QDomNodeList refls = element.elementsByTagName("ref");
        QVector<QDomElement> refs;

        for(int k=0;k<refls.size();k++)
        {
          refs.append(refls.at(k).toElement());
        }
        for(int k=0;k<refs.size();k++)
        {

          QDomElement anchorElement = xcdlDoc.createElement("a");

          QDomElement ref=refs.at(k).toElement();
          QString begin = ref.attributeNode("begin").value();
          QString end = ref.attributeNode("end").value();
          //	std::cout<<ref.nodeName().toStdString()<<" : "<<begin.toStdString()<<", "<<end.toStdString()<<", "<<refs.size()<<"\n";
          anchorElement.setAttribute("href","#"+begin+"_"+end);

          anchorElement.appendChild(ref);
          element.appendChild(anchorElement);
          element.removeChild(ref);
        }


      }

      actStr = xcdlDoc.toString(4);

      actStr.replace( "<","&lt;");
      actStr.replace( ">","&gt;");
      actStr.replace( "\n","<br/>");
      actStr.replace(" ","&#160;");

      QRegExp anchorReg("&lt;a&#160;href=(\"[^\"]*\")&#160;&gt;<br/>(&#160;)*");
      actStr.replace(anchorReg,"<a href=\\1>");
      actStr.replace(QRegExp("(&#160;)*&lt;/a&gt;"),"</a>");

      actStr.prepend("<font face=\"Courier\">");
      actStr.append("</font>");

      editor->setHtml(actStr);

      MyXMLSyntaxHighlighter* highlighter = new MyXMLSyntaxHighlighter( editor, textSize, textWeight);
      // Useless statement, less compiler-warnings for unused variables
      (void)highlighter;

      allEditors.append(editor);
      tabWidget->addTab(editor,*xcdlFilename);

      delete filename;
      delete xcdlFilename;

    }
  }
  catch(XCLException excp)
  {
    delete filename;
    delete xcdlFilename;
    // ### Beobachten!!! Hier standen vorher zwei Gleichheitszeichen, sprich Vergleich!!!
    // Der Compiler hat sich zurecht ueber ein unsinniges Statement beschwert
    chooseXCEL = FALSE;
#ifdef PlanetsDebug
    debugTree=builder->getDebugTree();
#endif
    QMessageBox::warning(this, tr("WARNING"),excp.getMessage(),QMessageBox::Yes);

  }
  catch(...){}

}


void ExtractorGUI::checkProperty(int box)
{
  QVector<QString> ignoredProperties;
  QVector<QString> definedProperties;

  for(int i=0;i<boxes.size();i++)
  {
    if(boxes.at(i)->checkState()==0)
      ignoredProperties+=(properties.at(i));
    else
      definedProperties+=(properties.at(i));

  }
  factory->setDefinedProperties(definedProperties);
  factory->setIgnoredProperties(ignoredProperties);
  boxes.clear();
  properties.clear();

  // Unused Parameters, less compiler-warnings
  (void)box;
}


void ExtractorGUI::identifySlot()
{
  if(binaryFileNames.isEmpty())return;
  FormatIdentifier fi;
  try
  {
    fi.identify(binaryFileNames.at(0),"","xcdlOutput.xcdl");
    findXCEL(fi.getXCELPath());
  }
  catch(XCLException exception)
  {
    QMessageBox::warning(this, tr("File Type is not supported"),
                         exception.getMessage(),
                         QMessageBox::Yes);
  }
}


void ExtractorGUI::identify()
{
  if(binaryFileNames.isEmpty())return;
  FormatIdentifier fi;
  try
  {
    fi.identify(binaryFileNames.at(0),"","xcdlOutput.xcdl");
    findXCEL(fi.getXCELPath());
  }
  catch(XCLException exception)
  {
    int ret = QMessageBox::warning(this, tr("File Type is not supported"),
                                   exception.getMessage(),
                                   QMessageBox::Yes);
    throw XCLException("Can not process\n");
    // Useless statement, less compiler-warnings, vars not in use
    (void)ret;
  }
}


void ExtractorGUI::closeAllTabs()
{
  while(tabWidget->count() != 0)
    tabWidget->removeTab(0);
  allEditors.clear();

}


void ExtractorGUI::closeTab()
{
  tabWidget->removeTab(tabWidget->currentIndex());
  allEditors.remove(tabWidget->currentIndex());
}


void ExtractorGUI::myResize()
{
  resize(mySize);
}


void ExtractorGUI::setTextSize(const QString& s)
{
  textSize=s.toInt( );
  for(int i=0;i<allEditors.size();i++)
  {
    allEditors.at(i)->selectAll();
    allEditors.at(i)->setFontPointSize(textSize);
  }
}


void ExtractorGUI::setTextWeight(const QString& s)
{
  textWeight=s.toInt( );
  for(int i=0;i<allEditors.size();i++)
  {
    allEditors.at(i)->selectAll();
    allEditors.at(i)->setFontWeight(textWeight);
  }
}


void ExtractorGUI::showDebugTreeView()
{
  XCLParsingItem tree = debugTree;
  QDialog* dialog = new QDialog(this);
  //QScrollArea* sv = new QScrollArea(dialog);
  // sv->setMinimumSize(width(),height());
  dialog->setMinimumSize(width(),height());
  dialog->setMaximumSize(width(),height());

  QFrame* frame = new QFrame(dialog);

  QHBoxLayout* mmainLayout = new QHBoxLayout(frame);

  QTreeWidget *treeWidget = new QTreeWidget();
  treeWidget->setMinimumSize(width()-20,height()-20);
  treeWidget->setColumnCount(3);
  QList<QTreeWidgetItem *> items;
  QTreeWidgetItem* parent=new QTreeWidgetItem((QTreeWidget*)0 , QStringList(tree.getName())<<tree.getIdentifier()<<tree.getInterpretedValue()) ;
  buildTreeWidget(&tree,parent);
  //  parent->setExpanded(TRUE);


  items.append(parent);
  treeWidget->insertTopLevelItems(0, items);
  treeWidget->setHeaderLabels(QStringList()<<"Name"<<"Identifier"<<"Value");
  QModelIndex index = treeWidget->model()->index(0, 0);
  treeWidget->expand(index);
  mmainLayout->addWidget(treeWidget);

  frame->setLayout(mmainLayout);
  // sv->setWidget(frame);

  // connect(dialog, SIGNAL(finished(int)), this, SLOT(checkProperty(int)));
  dialog->setWindowTitle("Debug Tree");
  dialog->show();
}


void ExtractorGUI::showParseTreeView()
{

  XCLParsingItem tree = parseTree;
  QDialog* dialog = new QDialog(this);
  //QScrollArea* sv = new QScrollArea(dialog);
  // sv->setMinimumSize(width(),height());
  dialog->setMinimumSize(width(),height());
  dialog->setMaximumSize(width(),height());

  QFrame* frame = new QFrame(dialog);

  QHBoxLayout* mmainLayout = new QHBoxLayout(frame);

  QTreeWidget *treeWidget = new QTreeWidget();
  treeWidget->setMinimumSize(width()-20,height()-20);
  treeWidget->setColumnCount(3);
  QList<QTreeWidgetItem *> items;
  QTreeWidgetItem* parent=new QTreeWidgetItem((QTreeWidget*)0 , QStringList(tree.getName())<<tree.getIdentifier()<<tree.getInterpretedValue()) ;
  buildTreeWidget(&tree,parent);
  // parent->setExpanded(TRUE);


  items.append(parent);
  treeWidget->insertTopLevelItems(0, items);
  treeWidget->setHeaderLabels(QStringList()<<"Name"<<"Identifier"<<"Value");
  QModelIndex index = treeWidget->model()->index(0, 0);
  treeWidget->expand(index);
  treeWidget->resizeColumnToContents(0);
  mmainLayout->addWidget(treeWidget);

  frame->setLayout(mmainLayout);
  // sv->setWidget(frame);

  // connect(dialog, SIGNAL(finished(int)), this, SLOT(checkProperty(int)));
  dialog->setWindowTitle("Parse Tree");
  dialog->show();
}


void ExtractorGUI::buildTreeWidget(XCLParsingItem* expr, QTreeWidgetItem* parent)
{
  try
  {
    QVector<XCLParsingExpression*> children=expr->getChildren( );
    QList<QTreeWidgetItem *> items;
    QTreeWidgetItem* me=new QTreeWidgetItem((QTreeWidget*)0 , QStringList(expr->getName())) ;
    for (int i = 0; i < children.size(); ++i)
    {
      if(children.at(i)->getType() == XCLParsingExpression::ITEM_TYPE)
        buildTreeWidget((XCLParsingItem*)children.at(i), me);
      me->addChild( new QTreeWidgetItem((QTreeWidget*)0 , QStringList(children.at(i)->getName())<<children.at(i)->getIdentifier()<<children.at(i)->getInterpretedValue()));
    }
    //  me->setExpanded(TRUE);
    parent->addChild(me);

  }
  catch(XCLException exception)
  {}}

void ExtractorGUI::goToNormData(const QUrl& u)
{
  QTextBrowser* editor = allEditors.at(tabWidget->currentIndex());
  editor->setHtml(actStr);
  editor->reload();
  QUrl url = u;

  QString begin = url.toString().mid(1,url.toString().indexOf("_")-1);
  QString end = url.toString().mid(url.toString().indexOf("_")+1);

  QString str = editor->toHtml();
  int normDataTagBegin = str.indexOf("&lt;normData");
  int normDataTagEnd = str.indexOf("&gt;",normDataTagBegin) +4;

  str.insert(normDataTagEnd+end.toInt()+1,"</i></b></font>");
  str.insert(normDataTagEnd+begin.toInt(),"<font size=\"14\" color=\"blue\"><b><i>");

  editor->setHtml(str);
  editor->reload();
}
