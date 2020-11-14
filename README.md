# XML-Converter-C-

Implementation based on previous project but in this second version, we use C for extracting objects from an XML file.

### Implementation

In 'main.cpp', we have the test generation (void random_xml(int depth, string tab);) before handle extracting and converting back object to an XML file, See '// Test Gen' to '//End of Test Gen'.

We can change MAX_LENGTH, MIN_LENGTH, MAX_DEPTH or MAX_ATTRIBUTES to generate XML file with different lengths (Do not set them too big, the xml file can be really large).

There are 10000 iterations in main(), when we continuously write information to 'file.xml', parse it and then write those infomation (or objects) to 'output.xml'. See 'xml.h' for definitions and 'xml.c' for implementations

### 'xml.h' and 'xml.c'
  
  - exe(): read and write to files. See 'data' directory
  - extract(): save objects, return root element
  - convert(): convert objects back to a xml file

### 'charUntility.h'

  This file contains function for handling 'char*' (we usually use 'string' instead)

### Data

  - dictionary.txt (Test Gen resource)
  - file.xml (We get info. from this file)
  - output.xml (... and write it back to output.xml)

Project 1 | 20201 | HUST
