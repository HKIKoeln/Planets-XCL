PLANETS XCL
=======
This Software is one of the final products of the longterm preservation-project PLANETS (http://www.planets-project.eu/), that ran from 2006 to 2010 at instiutions in several european countries, most of them libraries, archives or universities, funded by the european commission as FP6 programme.
Within this context the Universtiy of Cologne (UzK) produced the XCL Softwaresuite (http://planetarium.hki.uni-koeln.de/planets_cms/imprint).

File-format obsolscence and other challenges
=========
File format obsolescence is understood to be one of the pressing challenges in the field of digital preservation. One – and currently the most frequently followed – solution to this problem is migration, during which a file in an old file format in danger of becoming obsolete is converted into a new file format which is considered to be more trustworthy for the future. It is quite difficult, however, to find out whether the byte stream in the new format really represents all the information contained in the old format. One of the goals of the preservation characterisation sub-project is to help develop a way to monitor whether a file format migration from format A to format B has fulfilled this condition. This needs to be done in an automated procedure, as a librarian or archivist will not have time to manually control the success of a migration process which may concern hundreds of thousands of files. Crucial in this context is to monitor whether a conversion from file format A to file format B is successful not only in the sense that a program did not crash but also that all (or most) characteristics within a file have stayed the same, i. e. that an image has not only the same width in pixels before and after a conversion from format A to format B, but also that the pixel data themselves have successfully been converted. The two test images below show a file before and after a migration where all the "characteristics" agree with each other, the conversion of the representation of the alpha channel in one format to that of the other was erroneous, however.

Every file format specification uses a different vocabulary for the properties of each file and stores these properties in its own structures in the byte stream. The Planets team of the Universität zu Köln (UzK) is developing ways to describe these file formats in a way to make the comparison of the information contained within files in different formats possible. This is done with two formal languages, called the Extensible Characterisation Definition Language (XCDL) and the Extensible Characterisation Extraction Language (XCEL), which describe formats and the information contained within individual files. 

The XCDL: the description of file-content and applied property-value pairs
====
An XCDL is designed to provide an abstract description of a file. The underlying technology for XCDL is XML and XML schema. Digital objects are characterised through certain attributes which are called properties within XCDL. A property always has a specific value. Each XCDL document therefore describes digital objects through the specification of its properties' values. As an XCDL description can conceivably be generated for any kind of file format, it allows the comparison between the information contained within a digital object before and after migration from format A to format B. 

The XCEL: an abstract file format description
====
The objective of the XCEL is to describe structures and meanings of file formats in a machine-readable way. In other words: the XCEL can be called an abstract file format specification.

The underlying technology for XCEL is XML and XML Schema. The idea is to provide an extensible set of XML Schemas defining an XML based language which enables a file format expert to transform any humanly readable format specification into an XCEL description, which can be processed by general purpose software. The XCEL file will then be parsed by the Extractor software from UzK to enable the analysis of single objects of this format.
An XCEL file contains 3 types of elements: items, symbols, and processings. Item elements are container objects that are used to structure a document into semantic meaningful parts which the file format requires to appear at certain positions.
Symbols work as leaves in the XCEL tree and contain pieces of data that are found in the files at given positions.
Processings are used to change the behaviour of the parser (usually the Extractor software) or the tree itself during runtime. This is, for example, useful in the context of compression algorithms.
With the aid of an XCEL, the Extractor software is able to parse single files of a certain format and produce an XCDL file for each of these files. The XCDL file then functions as an abstract representation of the information contained in these digital objects.

The XCL-Ontology
====
Not only do the structures between different file formats differ widely, there is also a wide variety between the notation of properties in file formats. To be able to automatically detect similarities and differences between the various formats used in digital libraries and archives, an ontology has been developed at UzK.
The XCL-Ontology is written in the OWL Lite dialect and edited in RDF and can thus be queried by machine to provide a general vocabulary which is applicable to all XCDLs of any file format provenance, and helps the developers of the XCL-Vocabulary in finding matching property-names for this vocabulary. Comparing apples and oranges is commonly seen as a bad idea: The XCL-Ontology prevents this by relating all properties encountered to a set of basic information types, of which "image" and "text" are so far fully supported.

The Cologne Information Model (CIM)
====
While "image" and "text" can be easily distinguished, the precise relationship between, e.g., vector graphics and bitmaps and whether they can be meaningfully compared is much harder to decide. Similarly it is occasionally quite difficult to decide whether a "significant characteristic" is actually formally specified within a file or only derived by the human observer from the rendering of its content. To provide a context for the solution of such problems, UzK is finishing the first draft of an information model extending Langefors' infological model.
