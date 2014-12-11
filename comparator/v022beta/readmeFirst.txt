
Comparator Tool (version 0.22beta)
==================================

Release info:
-------------
This is a beta version release for the purpose of integration
into PLATO and the Planets testbed.


Description:
------------

This tool is part of the XCL. It compares the properties of two or more XCDLs of
information type *image* according to a predefined configuration file (PCR file). 
For exhaustive information please read the document 'PP5-D1' and related documents
which you can find in the folder comparator/doc. 

 
Distrubution/ Requirements / Installation:
------------------------------------------

This distribution contains a precompiled version for windows, compiled under
windows XP professional sp2, using MingW.
Important notice: If you use this version, make sure that you put 
the file "xerces-c_2_5_0.dll" into the directory where the precompiled file
is located!

If you want to build it from the source files you need to install Xercesc (a
XML parser which is used by the Comparator). You can find it following this link:

http://xerces.apache.org/xerces-c/

Please install it on your machine according to the installation guide provided 
there.

Please notice:
- This version of the Comparator is tested under xerces 2.5.0 and 2.8.0. Therefore we 
recommend to use one of these versions.
- To ease your effort for building the source, we added a shell script for compiling
the source. It should be easy to customize it for the requirements of your local system. 
- If you want to compile the source code under a Linux, Mac or any other OS distribution
you need to change the following lines in the Header file 'src/defines.h' :

change:   #define __OS_WIN     to     //#define __OS_WIN
change:   //#define __OSMAC    to     #define __OSMAC

In this case you should make sure, that the folder 'res' is a subfolder relative
to the folder where the executable file is located.



Despite of the version (precompiled or not) you use, unzip the distribution and store
the files in any directory on your local machine. Please note that you may not change
the names of the folders and their relative locations (esp. the folder named 'res').



Summary: SHORT INSTALLATION GUIDE
***********************************

(1)Please unzip to your local directory.
(2)Keep the directory res/ and its subdirectories as it is! The directories contain
the XSD schemas for validation of XCDL/ PCR.
(3) Comparator uses Apaches Xercesc. You either need to install this
    (We recommend version 2.5.0 OR 2.8.0) OR, for windows installation,
    you can use the .dll you find in directory "xercesc".
     If so put it in the directory where "comparator" runs!

(4)Test comparator by typing (e.g. windows):
comparator_v022beta.exe example\testfiles\XCDL\XCDL1.xcdl example\testfiles\XCDL\XCDL2.xcdl example\testfiles\xcdl\XCDL3.xcdl -c testfiles\pcr\PCRMulti.xml -o test\\




Usage:
------

Command line input: 

Comparator expects at least three parameters: 

[XCDL filename] [XCDL filename] [-c PCRFilename] 

where the first given XCDL filename stands for the source XCDL (the one to be compared against)
and all the following XCDL filenames for the target XCDL files. 

The third parameter is also mandatory. It additionally takes a flag (-c) where the name of a valid
PCR file must be quoted.

Output file: cpr.cpr; this file is written to the directory where executable is located, except
if option -o is used (see below).


Please note:
-All files are validated against their schemas. XCDLs and PCR are validated against according
schemas. These schemas have to be present in the directory "res". Do not change anything!

Schema names: XCDLCore.xsd, XCDLBasicTypes.xsd, preserve.xsd  image/XCDLImageProperties.xsd   
                   -->  for XCDL validation
	      pcr.xsd  
  		  -->  for PCR validation

 In case of invalid files, Comparator throws back an output cpr file that includes
an <error> tagset in <cpResponse> with the appropriate error.

- There must be exactly two XCDL filenames and one PCRFilename quoted. If not, Comparator also
reports an error as indicated above.

-Please note that the filenames of the compared XCDLs you find in the CPR are those you've 
transmitted while envoking Comparator from the command line NOT the one's quoted in the PCR.

- Comparator also throws an error message within the .cpr file (beneath <compSet> tag) if PCR and 
given input XCDLs do not match for any reason. So always make sure that your PCR includes
appropriate <compSet>s (matching with the number of your given input XCDL filenames).
E.g.:
if you want to compare a source XCDL with two target XCDLs, include for each comparison pair
(sourceXCDL1 - targetXCDL1, sourceXCDL - targetXCDL2) a <compSet> with your individual
parameters.

- Additional features:
Another parameter can be set optionally: 

-o someOutDirectory
 
Example:
-o test/cprs/      writes file to directory test/cprs/

Please note:
- always put a final slash at the end of your indicated directory; use masking and
backslashes for usage of windows version (e.g. myoutDir\\out\\).
- the directory you quote MUST already exist!
- this feature does not work if Comparator is used with less arguments than those indicated
above. In this case, the resulting cpr file is written to the dirctory where the executable
is located.


- Comparator also creates a logfile (log.txt) where unusual processing events are documented
(each new process is indicated witha timestamp). This was introduced for developers.
The file will not be deleted automatically (events are appended) !

- You can find an example of the output cpr and testfiles in the directory 'example'.

- In case of bugs, please report it to the appropriate gforge slot. Please give a detailed
description of the bug and attach the logfile plus the files you used while the bug
occured if possible.



Additional sources:
*******************
We strongly recommend to read these documents:

Detailed descriptions of PCR, CPR and comparison metrics can be found additionally here:
http://lehre.hki.uni-koeln.de/planets/documents/deliverables/PP5-D1-final.pdf

Descriptions of XCDL and XCL in general:
http://lehre.hki.uni-koeln.de/planets/documents/deliverables/Planets_PC2-D7_FinalXCDLSpec_Ext.pdf
http://lehre.hki.uni-koeln.de/planets/documents/deliverables/Planets_PC2-D8_FinalXCELSpecification_Ext.pdf




List of Errorcodes
------------------

Explanation: Errorcodes listed in tag <metric> of CPR descriptions.

ERROR_110 : Comparison of two sets of different sizes is not possible for metric hamming distance.
ERROR_111 : Comparison of two sets of different sizes is not possible for metric simpleMatchCoefficientN.
ERROR_112 : Comparison of two sets of different sizes is not possible for metric L2Distance.
ERROR_120 : No normData existing in source normData Tag.
ERROR_121 : No normData existing in target normData Tag.
ERROR_122 : No normData existing in source and target normData Tag.
ERROR_150 : Comparison of two sets of different sizes is not possible for metric RMSE.
ERROR_200 : Wrong syntax for metric simpleMatchCoefficientN in PCR file. Attribute 'param' must be set.
ERROR_201 : Metric intDiff:Out of size error. uint32's larger than 2.147.483.648 .
ERROR_202 : Metric intSum: Out of size error. uint32's larger than 2.147.483.648 .
ERROR_203 : Metric intRatio: Out of size error. uint32's larger than 2.147.483.648 .
ERROR_203 : Metric intProduct: Out of size error. uint32's larger than 2.147.483.648 .
ERROR_205 : Metric ratDiff: Out of size error. uint32's larger than 2.147.483.648 .
ERROR_206 : Metric ratSum: Out of size error. uint32's larger than 2.147.483.648 .
ERROR_207 : Metric ratRatio: Out of size error. uint32's larger than 2.147.483.648 .
ERROR_208 : Metric ratProduct: Out of size error. uint32's larger than 2.147.483.648 .
ERROR_210 : Metric percDev: Out of size error. uint32's larger than 2.147.483.648 .
ERROR_300 : Requested metric is not defined for property.
ERROR_301 : Requested metric is not defined for normData.