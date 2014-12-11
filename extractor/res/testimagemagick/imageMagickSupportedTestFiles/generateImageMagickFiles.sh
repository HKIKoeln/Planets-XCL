#! /bin/bash

# The Script generates imageMagick files for images which are in the following folder:
#  res/testbmp;
#  res/testtiff;
#  res/testpng;
#  res/testimagemagick/imageMagickSupportedTestFiles/test_gif;
#  res/testimagemagick/imageMagickSupportedTestFiles/test_svg;
#  res/testimagemagick/imageMagickSupportedTestFiles/test_jpeg;
#  res/testimagemagick/imageMagickSupportedTestFiles/test_jp2; 
#
# The generated imageMagick-files are stored in the folder 'imageMagickGenerated'. For every file format a folder is generated .
#
#
#
#
#
#


# 'imageMagickGenerated'-directory is generated.
if find imageMagickGenerated;then
 rm -r imageMagickGenerated
 mkdir imageMagickGenerated
else
 mkdir imageMagickGenerated
fi



# generating of directory for bmp file format &
# generating of imageMagick-Files for bmp
mkdir imageMagickGenerated/imageMagick_bmp;
cd ../../testbmp;
for i in `ls *BMP`;
do identify -verbose $i > ../testimagemagick/imageMagickSupportedTestFiles/imageMagickGenerated/imageMagick_bmp/${i}identify-verbose.txt;
done;cd -


mkdir imageMagickGenerated/imageMagick_tiff;
cd ../../tiffsuit;
for i in `ls *tif`;
do identify -verbose $i > ../testimagemagick/imageMagickSupportedTestFiles/imageMagickGenerated/imageMagick_tiff/${i}identify-verbose.txt;
done;cd -

mkdir imageMagickGenerated/imageMagick_png;
cd ../../testpng;
for i in `ls *png`;
do identify -verbose $i > ../testimagemagick/imageMagickSupportedTestFiles/imageMagickGenerated/imageMagick_png/${i}identify-verbose.txt;
done;cd -


###

mkdir imageMagickGenerated/imageMagick_gif;
cd test_gif;
for i in `ls *gif`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_gif/${i}identify-verbose.txt;
done;cd -

mkdir imageMagickGenerated/imageMagick_svg;
cd test_svg;
for i in `ls *svg`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_svg/${i}identify-verbose.txt;
done;cd -

mkdir imageMagickGenerated/imageMagick_jpeg;
cd test_jpeg;
for i in `ls *jpg`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_jpeg/${i}identify-verbose.txt;
done;cd -

mkdir imageMagickGenerated/imageMagick_jp2a;
cd testjp2a;
for i in `ls *jp2`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_jp2a/${i}identify-verbose.txt;
done;cd -


#jp2
mkdir imageMagickGenerated/imageMagick_jp2b;
cd testjp2b;
for i in `ls *jp2`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_jp2b/${i}identify-verbose.txt;
done;cd -

#pict
mkdir imageMagickGenerated/imageMagick_pict;
cd testpict;
for i in `ls *ct`;# for *.pict and *.pct
do identify -verbose $i > ../imageMagickGenerated/imageMagick_pict/${i}identify-verbose.txt;
done;cd -

#pbm
mkdir imageMagickGenerated/imageMagick_pbm;
cd testpbm;
for i in `ls *pbm`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_pbm/${i}identify-verbose.txt;
done;cd -

#ppm
mkdir imageMagickGenerated/imageMagick_ppm;
cd testppm;
for i in `ls *ppm`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_ppm/${i}identify-verbose.txt;
done;cd -

#pcd
mkdir imageMagickGenerated/imageMagick_pcd;
cd testpcd;
for i in `ls *pcd`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_pcd/${i}identify-verbose.txt;
done;cd -

#pcx
mkdir imageMagickGenerated/imageMagick_pcx;
cd testpcx;
for i in `ls *pcx`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_pcx/${i}identify-verbose.txt;
done;cd -

#psd
mkdir imageMagickGenerated/imageMagick_psd;
cd testpsd;
for i in `ls *psd`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_psd/${i}identify-verbose.txt;
done;cd -

#tga
mkdir imageMagickGenerated/imageMagick_tga;
cd testtga;
for i in `ls *tga`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_tga/${i}identity-identify-verbose.txt;
done;cd -

#xbm
mkdir imageMagickGenerated/imageMagick_xbm;
cd testxbm;
for i in `ls *xbm`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_xbm/${i}identify-verbose.txt;
done;cd -

#xpm
mkdir imageMagickGenerated/imageMagick_xpm;
cd testxpm;
for i in `ls *xpm`;
do identify -verbose $i > ../imageMagickGenerated/imageMagick_xpm/${i}identify-verbose.txt;
done;cd -






