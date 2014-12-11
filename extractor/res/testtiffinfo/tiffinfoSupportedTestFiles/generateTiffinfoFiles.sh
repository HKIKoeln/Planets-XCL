#! /bin/bash

# The Script generates tiffinfo-files for images which are in the following folder:
#  res/testtiff;
#
# The generated tiffinfo-files are stored in the folder 'test_tiffinfo_generated'.
#
#
#
#
#
#

# 'test_tiffinfo_generated'-directory is generated.
if find tiffinfoGenerated;then
 rm -r tiffinfoGenerated
 mkdir tiffinfoGenerated
else
 mkdir tiffinfoGenerated
fi


# generating of tiffinfo-Files for tiff
cd ../../tiffsuit;
for i in `ls *tif`;
do tiffinfo -D -c -d -r $i > ../testtiffinfo/tiffinfoSupportedTestFiles/tiffinfoGenerated/${i}tiffinfo.txt;
done;cd -
