**********************************
FPM tool, version 1.1 (28/11/2008)
**********************************

Description:
------------

This tool is part of the XCL tools, particularly in conjunction with the Comparator tool.
The acronym FPM stands for "Format - Properties - Metrics". According to that, the basic
purpose of the tool is to deliver for a given format all properties and metrics
which are supported for comparison using the Comparator tool.

An example of the FPM tools' output can be found in the 'example' folder.
A 'custom' user of the FPM tool uses the output of the tool to configure a so called
PCR description that is used by the Comparator. For complete information on this issue
see the documents in the 'docs' folder, especially 'PP5-D1'.


Usage:
------

To work properly, the tool needs a so called PUIDlist. 'PUID' stands for PRONOM Persistent
Unique Identifier. This is a unique identifier for a file format according to the PRONOM
system of the National Archives of the UK. For more information on this see

http://www.nationalarchives.gov.uk/pronom

. For command line usage type:

fpmTool [PUIDlist] [-o someoutdir]

e.g. (windows)

fpmTool.exe fmt/10:fmt/13: -o out\\

Calling this would create an output file called 'fpm.fpm' in the 'out' subfolder,
including a description of properties with associated metrics for the indicated
formats 'fmt/10' (this is tiff, latest version) and 'fmt/13' (png, latest version).
The indication of an output directory is optional. If it is not indicated, the output
will directly be written to the directory where the FPM tool binary is located.


Please notice:
- This version DOES NOT support automated creation of a directory that does not
exist, so the output directory must already exist!
- The syntax of the PUIDlist and output directory is case-sensitive. Always use valid
PUIDs and finish each PUID in the list with a colon! If this is not done in the right
way, FPM tool creates a command line message and/or a message within the output 'fpm.fpm'.
For windows, please use the 'masked' form for indication of the output directory
(using double backslashes: \\).
- FPM tool additionally reports a 'logfile.txt' file with information on
process events. This was introduced mainly for the developers of the tool and
should be ignored, unless it seems you detected a bug. In this case, please report
the bug to the developers using the appropriate functionality in the Planets gforge
repository and attach the log file together with a description of the bug.




Distribution/ Requirements/ Installation:
-----------------------------------------

The FPM Tool is currently available as source code (C/C++) and as pre-compiled binary for
windows (compiled under windows XP using MingW) and Mac OS (compiled on 10.5 Intel).

If you build FPM tool from the source code files you need to install c++ BOOST library
package for FPM tool uses the boost regex lib for validation of the command line input.
Boost can be downloaded from

http://www.boost.org/

.The current version is 1.37.0.

To ease your effort, an exemplary script for building FPM tool on Linux and Mac OS machines
can be found in the 'scripts' folder. Slight modifications should do the job!

If you use the windows or Mac OS executable, there is nothing to do than just to put
the unzipped package in a directory of your choice. The only thing that should be
noticed is to not change the relative location and the names of the folders included.
=======




Additional version:
-------------------

FPM tool is also available as a webservice. Please visit


http://gforge.planets-project.eu/gf/project/pserv/


for further information.

Please notice: In the nature of things, the -o option is not available in this case.




List of supported PUIDs
-----------------------

For a detailed description of the PUIDs see

http://www.nationalarchives.gov.uk/pronom



gif:
fmt/3
fmt/4

tif:
fmt/7
fmt/8
fmt/9
fmt/10
fmt/152
fmt/153
fmt/154
fmt/155
fmt/156
x-fmt/387
x-fmt/388
x-fmt/399


png:
fmt/11
fmt/12
fmt/13


jpg:
fmt/41
fmt/42
fmt/43
fmt/44
fmt/112
fmt/149
x-fmt/390
x-fmt/391
x-fmt/398


bmp:
fmt/114
fmt/115
fmt/116
fmt/117
fmt/118
fmt/119
x-fmt/25
x-fmt/270

jp2:
x-fmt/392

pbm:
x-fmt/164

pcd:
x-fmt/165

pcx:
fmt/86
fmt/87
fmt/88
fmt/89
fmt/90

pict:
x-fmt/80

ppm:
x-fmt/178

psd:
x-fmt/92

svg:
fmt/91
fmt/92
x-fmt/109

tga:
x-fmt/367

xbm:
x-fmt/299
x-fmt/207

xpm:
x-fmt/208

--------------------------------------------------------------------------
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

For further information on the copyright, please see sourcecode files.
--------------------------------------------------------------------------