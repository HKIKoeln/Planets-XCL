#include <vector>
#include <math.h>
#include <stdlib.h>
#include "defines.h"
#include "DataTagset.h"
#include "ComparatorError.h"
#include "ComparerMeasure.h"
#include "ComparerMeasureValueSet.h"
#include "ComparerMeasureDataRef.h"
#include "XCDLIndexes.h"


ComparerMeasure :: ComparerMeasure(ComparatorError *pCompError)
{
pCE= pCompError;
}

ComparerMeasure :: ~ComparerMeasure() {cleanUp();}

void ComparerMeasure :: cleanUp() { pCE= NULL;}

// hamming distance for int arrays
int ComparerMeasure :: doHammingDistance(std::vector <int> *setA, std::vector <int> *setB)
{
std::vector <int> :: iterator iSetA;
std::vector <int> :: iterator iSetB;
int hamDist=0;

// first check if sets are of equal size
if(setA->size()!=setB->size())
  {
  pCE->registrateError("Error: doHammingDistance(): Not possible. Sets have different sizes but must be equal for this metric.");
  pCE->errorcode="code_B_50: Size of src and tar sets is not equal.";
  return -1;
  }

for(iSetA=setA->begin(), iSetB=setB->begin(); iSetA!=setA->end(); ++iSetA, ++iSetB)
   {
   if(*iSetA!=*iSetB) hamDist++;
   }

#ifdef DEBUG_COMPMEASURE
std::cout<<hamDist<<" =hammingDistance"<<std::endl;
#endif

return hamDist;
}

// hamming distance for character arrays
int ComparerMeasure :: doHammingDistance(NormData *pND1, NormData *pND2)
{
// first check if sets are of equal size
if(pND1->normDataLength!=pND2->normDataLength)
  {
  //std::cout<<setA->size()<<" "<<setB->size()<<std::endl;system("PAUSE");
  pCE->registrateError("Error: doHammingDistance(): Not possible. Sets have different sizes but must be equal for this metric.");
  pCE->errorcode="code_B_50: Size of src and tar sets is not equal.";
  return -1;
  }

int hamDist=0;
for(int i=0; i<pND1->normDataLength;++i)
   {
   if(pND1->normDataAsIs[i] != pND2->normDataAsIs[i]) hamDist++;
   }

#ifdef DEBUG_COMPMEASURE
std::cout<<hamDist<<" =hammingDistance"<<std::endl;
#endif

return hamDist;
}

// hamming distance for string arrays
int ComparerMeasure :: doHammingDistance(std::vector <std::string> *pArray1, std::vector <std::string> *pArray2)
{
// first check if sets are of equal size
int size= pArray1->size();
if(size!=pArray2->size())
  {
  pCE->registrateError("Error: doHammingDistance(): Not possible. Sets have different sizes but must be equal for this metric.");
  pCE->errorcode="code_B_50: Size of src and tar sets is not equal.";
  return -1;
  }

int hamDist= 0;

for(int i=0; i<size; ++i)
   {
   if((*pArray1)[i] != (*pArray2)[i]) ++hamDist;
   }

#ifdef DEBUG_COMPMEASURE
std::cout<<hamDist<<" =hammingDistance"<<std::endl;
system("PAUSE");
#endif

return hamDist;
}



// simpleMatchCofficient for characters
double ComparerMeasure :: doSimpleMatchCoefficient(NormData *pND1, NormData *pND2)
{
int size, mismatch=0;
double simpleMatch=0.0;


// first check if sets are of equal size
if(pND1->normDataLength != pND2->normDataLength)
  {
  pCE->registrateError("Error: simpleMatchCoefficientN(): Not possible. Sets have different sizes but must be equal for this metric.");
  pCE->errorcode="code_B_50: Size of src and tar sets is not equal.";
  return -1;
  }

size= pND1->normDataLength;
// TODO NEXT VERSION: Chech if ngram is set properly!!  (Size must be devideable by ngram)

for(int x=0; x<size; ++x)
   {
   if(pND1->normDataAsIs[x] != pND2->normDataAsIs[x])
     {
     ++mismatch;
     }
   }

simpleMatch= (((double)(size)) - ((double)(mismatch))) / ((double) (size));

#ifdef DEBUG_COMPMEASURE
std::cout<<simpleMatch<<" =simpleMatchCoefficient"<<std::endl;
#endif


return simpleMatch;
}


// TODO NEXT VERSION: Find better prerforming solution?
double ComparerMeasure :: doSimpleMatchCoefficientN(std::vector <int> *setA, std::vector <int> *setB, int ngram)
{
int tempCount, count, size;
double simpleMatch=0.0, iteration;


// first check if sets are of equal size
if(setA->size()!=setB->size())
  {
  pCE->registrateError("Error: simpleMatchCoefficientN(): Not possible. Sets have different sizes but must be equal for this metric.");
  pCE->errorcode="code_B_50: Size of src and tar sets is not equal.";
  return -1;
  }

size= setA->size();

// TODO NEXT VERSION : Chech if ngram is set properly!!  (Size must be devideable by ngram)

for(count=0, iteration=0.0; count<size;)
   {
   tempCount=ngram;
   for(;tempCount!=0;tempCount--)
      {
      if(((*setA)[count])!=((*setB)[count]))
        {
        simpleMatch++;
        count= count + tempCount;
        break;
        }
       count++;
      }
   iteration++;
   }

simpleMatch= simpleMatch/iteration;

#ifdef DEBUG_COMPMEASURE
std::cout<<simpleMatch<<" =simpleMatchCoefficient"<<ngram<<std::endl;
#endif

return simpleMatch;
}

// TODO NEXT VERSION: Find better prerforming solution?
double ComparerMeasure :: doEuclidDistance(std::vector <int> *setA, std::vector <int> *setB)
{
std::vector <int> :: iterator iSetA;
std::vector <int> :: iterator iSetB;
double dist= 0;
double x=0.0, part1=0.0, part2=0.0, sum=0.0, count= 1.0;

// first check if sets are of equal size
if(setA->size()!=setB->size())
  {
  pCE->registrateError("Error: doEuclidDistance(): Not possible. Sets have different sizes but must be equal for this metric.");
  pCE->errorcode="code_B_50: Size of src and tar sets is not equal.";
  return -1;
  }

for(x=1.0, iSetA=setA->begin(), iSetB=setB->begin(); iSetA!=setA->end(); ++iSetA, ++iSetB, x++)
   {
   dist= (*iSetA) - (*iSetB);
   dist= pow(dist, 2);
   sum= sum + dist;
   if(sum>2000000000)
     {
     part1= sum / 1000;
     part2= part2 + part1;
     part1= 0.0;
     sum= 0.0;
     count++;
     }
   }
if(count==1)
  {
  part2= sum;
  }
else
  {
  part2= part2 + (sum / 1000);
  part2= part2 * 1000;
  }

part2= sqrt(part2);

#ifdef DEBUG_COMPMEASURE
std::cout<<part2<<" =L2Distance"<<std::endl;
#endif

return part2;
}

// TODO NEXT VERSION: Find better performing solution?
double ComparerMeasure :: doRMSE(std::vector <int> *setA, std::vector <int> *setB)
{
std::vector <int> :: iterator iSetA;
std::vector <int> :: iterator iSetB;
double dist= 0;
double x=0.0, part1=0.0, part2=0.0, sum=0, count= 1.0;

// first check if sets are of equal size
if(setA->size()!=setB->size())
  {
  pCE->registrateError("Error: doRMSE(): Not possible. Sets have different sizes but must be equal for this metric.");
  pCE->errorcode="code_B_50: Size of src and tar sets is not equal.";
  return -1;
  }

for(x=1.0, iSetA=setA->begin(), iSetB=setB->begin(); iSetA!=setA->end(); ++iSetA, ++iSetB, x++)
   {
   dist= (*iSetA) - (*iSetB);
   dist= pow(dist, 2);
   sum= sum + dist;
   if(sum>2000000000)
     {
     part1= sum / 1000;
     part2= part2 + part1;
     part1= 0.0;
     sum= 0.0;
     count++;
     }
   }
if(count==1)
  {
  part2= sum / (x-1);
  }
else
  {
  part2= part2 + (sum / 1000);
  part2= part2 / (x-1);
  part2= part2 * 1000;
  }

part2= sqrt(part2);

#ifdef DEBUG_COMPMEASURE
std::cout<<part2<<" =RMSE"<<std::endl;system("PAUSE")
#endif

return part2;
}

// Levensthein Distance for two strings of arbitrary length
int ComparerMeasure :: doLevenstheinDistance(std::string x, std::string y)
{
#define min(a, b) (((a) < (b)) ? (a) : (b))
unsigned int i, n, m;
unsigned int above_cell, left_cell, diagonal_cell, cost; // complementary variables for computing the Levenshtein distance

    // check if there is normDataStr at all:
    n= x.length();
    m= y.length();
    if(n == 0 || m==0)
      {
      if(n==0)
        {
        pCE->registrateError("Error: doLevDist(): No normData existing in source <normData> Tags.");
        pCE->errorcode="code_B_51: No normData existing in source (content of: normData)";
        }
      else if(m==0)
        {
        pCE->registrateError("Error: doLevDist(): No normData existing in target <normData> Tags.");
        pCE->errorcode="code_B_52: No normData existing in target (content of: normData)";
        }
      else if(n==0 && m==0)
        {
        pCE->registrateError("Error: doLevDist(): No normData existing in source and target <normData> Tags.");
        pCE->errorcode="code_B_53: No normData existing in source and target(content of: normData)";
        }
      return -1;
      }

	// creating a matrix of m+1 rows and n+1 columns
    int **matrix = new int*[m+1];
	for(int c=0; c<(m+1); ++c)
       {
	   matrix[c]= new int[n+1];
       }

	// initialising the first row of the matrix
	for(i=0; i<=n; ++i)  {matrix[0][i] = i;}
	// initialising the first column of the matrix
	for(i=0; i<=m; ++i) {matrix[i][0] = i;}

	// starting the main process for computing
	// the distance between the two strings "x" and "y"
	for(i= 1; i <=m; ++i)
       {
		for(unsigned int j=1; j<=n; ++j)
           {
			// if the current two characters of both strings are the same
			// then, the corresponding cost value will be zero,otherwise it will be 1
			if (x[i-1] == y[j-1]) {cost = 0;}
			else {cost = 1;}

			// current cell of the matrix: matrix[i][j]
			// finds the above cell to the current cell
			above_cell= matrix[i-1][j];

			// finds the left cell to the current cell
			left_cell= matrix[i][j-1];

			// finds the diagonally above cell to the current cell
			diagonal_cell= matrix[i-1][j-1];

			// computes the current value of the levensthein distance and places
			// the result into the current matrix cell
			matrix[i][j]= min(min(above_cell+1, left_cell+1), diagonal_cell+cost);
		   }
	   }

	// placing the final result into a variable
	int result= matrix[m][n];
	// freeing memory that has been used for the matrix variable
	delete [] matrix;

	return result;
}


long ComparerMeasure :: doIntDiff(ValueSetIndex *pValSet1, ValueSetIndex *pValSet2)
{
long a= strtol(pValSet1->labValVal.c_str(), NULL, 0);
long b= strtol(pValSet2->labValVal.c_str(), NULL, 0);

#ifdef DEBUG_COMPMEASURE
std::cout<<(a - b)<<"=IntDiff"<<std::endl;
#endif

return (a - b);
}


double ComparerMeasure :: doRatDiff(ValueSetIndex *pValSet1, ValueSetIndex *pValSet2)
{
try
  {
  double a= atof(pValSet1->labValVal.c_str());
  double b= atof(pValSet2->labValVal.c_str());

  #ifdef DEBUG_COMPMEASURE
  std::cout<<(a - b)<<"=RatDiff"<<std::endl;
  #endif

  return (a - b);
  }
catch(...)
  {
  pCE->error= -1;
  return  0.0;
  }
}

long ComparerMeasure :: doIntSum(ValueSetIndex *pValSet1, ValueSetIndex *pValSet2)
{
long a= strtol(pValSet1->labValVal.c_str(), NULL, 0);
long b= strtol(pValSet2->labValVal.c_str(), NULL, 0);

#ifdef DEBUG_COMPMEASURE
std::cout<<(a + b)<<"=IntSum"<<std::endl;
#endif

return (a + b);
}


double ComparerMeasure :: doRatSum(ValueSetIndex *pValSet1, ValueSetIndex *pValSet2)
{
try
  {
  double a= atof(pValSet1->labValVal.c_str());
  double b= atof(pValSet2->labValVal.c_str());

  #ifdef DEBUG_COMPMEASURE
  std::cout<<(a + b)<<"=RatSum"<<std::endl;
  #endif

  return (a + b);
  }
catch(...)
  {
  pCE->error= -1;
  return  0.0;
  }
}

double ComparerMeasure :: doIntRatio(ValueSetIndex *pValSet1, ValueSetIndex *pValSet2)
{
try
  {
  double a= atof(pValSet1->labValVal.c_str());
  double b= atof(pValSet2->labValVal.c_str());

  #ifdef DEBUG_COMPMEASURE
  std::cout<<(a / b)<<"=IntRatio"<<std::endl;
  #endif

  return (a / b);
  }
catch(...)
  {
  pCE->error= -1;
  return  0.0;
  }
}

double ComparerMeasure :: doRatRatio(ValueSetIndex *pValSet1, ValueSetIndex *pValSet2)
{
double a= atof(pValSet1->labValVal.c_str());
double b= atof(pValSet2->labValVal.c_str());

if(b==0.0)
  {
  pCE->error= -1;
  return 0.0;
  }

#ifdef DEBUG_COMPMEASURE
std::cout<<(a / b)<<"=RatRatio"<<std::endl;
#endif

return (a / b);
}

long ComparerMeasure :: doIntProduct(ValueSetIndex *pValSet1, ValueSetIndex *pValSet2)
{
try
  {
  long a= atol(pValSet1->labValVal.c_str());
  long b= atol(pValSet2->labValVal.c_str());
  #ifdef DEBUG_COMPMEASURE
  std::cout<<a*b<<"=IntProduct"<<std::endl;
  #endif

  return a * b;
  }
catch(...)
  {
  pCE->error= -1;
  return  0;
  }
}

double ComparerMeasure :: doRatProduct(ValueSetIndex *pValSet1, ValueSetIndex *pValSet2)
{
try
  {
  double a= atof(pValSet1->labValVal.c_str());
  double b= atof(pValSet2->labValVal.c_str());

  #ifdef DEBUG_COMPMEASURE
  std::cout<<(a * b)<<"=RatProduct"<<std::endl;
  #endif

  return (a * b);
  }
catch(...)
  {
  pCE->error= -1;
  return  0.0;
  }
}

double ComparerMeasure :: doPercDeviation(ValueSetIndex *pValSet1, ValueSetIndex *pValSet2)
{
try
  {
  double a= atof(pValSet1->labValVal.c_str());
  double b= atof(pValSet2->labValVal.c_str());

  #ifdef DEBUG_COMPMEASURE
  std::cout<<(((b - a) / a) * 100)<<"=PercDeviation"<<std::endl;
  #endif

  return (((b - a) / a) * 100);
  }
catch(...)
  {
  pCE->error= -1;
  return  0.0;
  }
}

