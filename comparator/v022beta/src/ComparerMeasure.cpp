#include <vector>
#include <math.h>
#include <stdlib.h>
#include "defines.h"
#include "PropertyTagset.h"
#include "ComparatorError.h"
#include "ComparerMeasure.h"

ComparerMeasure :: ComparerMeasure(ComparatorError *pCompError)
{
pCE= pCompError;
}

ComparerMeasure :: ~ComparerMeasure() {cleanUp();}

void ComparerMeasure :: cleanUp() { pCE= NULL;}

int ComparerMeasure :: doHammingDistance(std::vector <int> *setA, std::vector <int> *setB)
{
std::vector <int> :: iterator iSetA;
std::vector <int> :: iterator iSetB;
int hamDist=0;

// first check if sets are of equal size
if(setA->size()!=setB->size())
  {
  //std::cout<<setA->size()<<" "<<setB->size()<<std::endl;system("PAUSE");
  pCE->registrateError("Error: doHammingDistance(): Not possible. Sets have different sizes but must be equal for this metric.");
  pCE->errorcode="ERRORCODE_110";
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

double ComparerMeasure :: doSimpleMatchCoefficientN(std::vector <int> *setA, std::vector <int> *setB, int ngram)
{
std::vector <int> :: iterator iSetA;
std::vector <int> :: iterator iSetB;
int tempCount, count, size;
double simpleMatch=0.0, iteration;


// first check if sets are of equal size
if(setA->size()!=setB->size())
  {
  pCE->registrateError("Error: simpleMatchCoefficientN(): Not possible. Sets have different sizes but must be equal for this metric.");
  pCE->errorcode="ERRORCODE_111";
  return -1;
  }

size= setA->size();

// TODO: Chech if ngram is set properly!!  (Size must be devideable by ngram)

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

// Euclid distance:
double ComparerMeasure :: doEuclidDistance(std::vector <int> *setA, std::vector <int> *setB)
{
std::vector <int> :: iterator iSetA;
std::vector <int> :: iterator iSetB;
double dist= 0;
double x=0.0, part1=0.0, part2=0.0, sum=0, count= 1.0;

//std::cout<<setA->size()<<" "<<setB->size()<<std::endl;system("PAUSE");

// first check if sets are of equal size
if(setA->size()!=setB->size())
  {
  //std::cout<<setA->size()<<" "<<setB->size()<<std::endl;
  pCE->registrateError("Error: doEuclidDistance(): Not possible. Sets have different sizes but must be equal for this metric.");
  pCE->errorcode="ERRORCODE_112";
  //system("PAUSE");
  return -1;
  }

for(x=1.0, iSetA=setA->begin(), iSetB=setB->begin(); iSetA!=setA->end(); ++iSetA, ++iSetB, x++)
   {
   dist= (*iSetA) - (*iSetB); //std::cout<<dist<<std::endl;
   dist= pow(dist, 2);        //std::cout<<dist<<std::endl;
   sum= sum + dist;            //std::cout<<"SUM:"<<sum<<std::endl;
   if(sum>2000000000)
     {//std::cout<<x<<" "<<sum<<std::endl;
     part1= sum / 1000;
     part2= part2 + part1;  //std::cout<<part2<<std::endl;
     part1= 0.0;
     sum= 0.0;
     count++;
     }
   }
if(count==1)
  {
  //std::cout<<x<<std::endl;
  part2= sum;
  }
else
  {
  part2= part2 + (sum / 1000);
  part2= part2 * 1000;
  }

//std::cout<<part2<<std::endl;

part2= sqrt(part2);

#ifdef DEBUG_COMPMEASURE
std::cout<<part2<<" =L2Distance"<<std::endl;
#endif


return part2;
}


double ComparerMeasure :: doRMSE(std::vector <int> *setA, std::vector <int> *setB)
{
std::vector <int> :: iterator iSetA;
std::vector <int> :: iterator iSetB;
double dist= 0;
double x=0.0, part1=0.0, part2=0.0, sum=0, count= 1.0;

//std::cout<<setA->size()<<" "<<setB->size()<<std::endl;system("PAUSE");

// first check if sets are of equal size
if(setA->size()!=setB->size())
  {
  //std::cout<<setA->size()<<" "<<setB->size()<<std::endl;
  pCE->registrateError("Error: doRMSE(): Not possible. Sets have different sizes but must be equal for this metric.");
  pCE->errorcode="ERRORCODE_150";
  //system("PAUSE");
  return -1;
  }

for(x=1.0, iSetA=setA->begin(), iSetB=setB->begin(); iSetA!=setA->end(); ++iSetA, ++iSetB, x++)
   {
   dist= (*iSetA) - (*iSetB);
   dist= pow(dist, 2);
   sum= sum + dist;
   if(sum>2000000000)
     {//std::cout<<x<<" "<<sum<<std::endl;
     part1= sum / 1000;
     part2= part2 + part1;  //std::cout<<part2<<std::endl;
     part1= 0.0;
     sum= 0.0;
     count++;
     }
   }
if(count==1)
  {
  //std::cout<<x<<std::endl;
  part2= sum / (x-1);
  }
else
  {
  part2= part2 + (sum / 1000);
  part2= part2 / (x-1);
  part2= part2 * 1000;
  }

//std::cout<<part2<<std::endl;

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
        pCE->errorcode="ERRORCODE_120";
        }
      else if(m==0)
        {
        pCE->registrateError("Error: doLevDist(): No normData existing in target <normData> Tags.");
        pCE->errorcode="ERRORCODE_121";
        }
      else if(n==0 && m==0)
        {
        pCE->registrateError("Error: doLevDist(): No normData existing in source and target <normData> Tags.");
        pCE->errorcode="ERRORCODE_122";
        }
      return -1;
      }

	// creating a matrix of m+1 rows and n+1 columns
    int **matrix = new int*[m+1];
	for(unsigned int c=0; c<(m+1); ++c)
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

std::string ComparerMeasure :: doIntSum(ValueSet *pValSet1, ValueSet *pValSet2)
{
unsigned long a, b, c;
char s[25];
std::string result;

a= strtoul(pValSet1->labValVal.c_str(), NULL, 0);
b= strtoul(pValSet2->labValVal.c_str(), NULL, 0);

c= a + b;

// convert to string again:
sprintf(s, "%u", c);

result= std::string(s);

#ifdef DEBUG_COMPMEASURE
std::cout<<result<<"=IntSum"<<std::endl;
#endif

return result;
}


std::string ComparerMeasure :: doIntDiff(ValueSet *pValSet1, ValueSet *pValSet2)
{
long a, b, c;
char s[25];
std::string result;

a= strtol(pValSet1->labValVal.c_str(), NULL, 0);
b= strtol(pValSet2->labValVal.c_str(), NULL, 0);

c= a - b;

// convert to string again:
sprintf(s, "%d", c);

result= std::string(s);

#ifdef DEBUG_COMPMEASURE
std::cout<<result<<"=IntDiff"<<std::endl;
#endif

return result;
}


std::string ComparerMeasure :: doRatSum(ValueSet *pValSet1, ValueSet *pValSet2)
{
double a, b, c;
char s[25];
std::string result;

a= atof(pValSet1->labValVal.c_str());
b= atof(pValSet2->labValVal.c_str());

c= a + b;

// convert to string again:
sprintf(s, "%f", c);

result= std::string(s);

#ifdef DEBUG_COMPMEASURE
std::cout<<result<<"=RatSum"<<std::endl;
#endif

return result;
}

std::string ComparerMeasure :: doRatDiff(ValueSet *pValSet1, ValueSet *pValSet2)
{
double a, b, c;
char s[25];
std::string result;

a= atof(pValSet1->labValVal.c_str());
b= atof(pValSet2->labValVal.c_str());

c= a - b;

// convert to string again:
sprintf(s, "%f", c);

result= std::string(s);

#ifdef DEBUG_COMPMEASURE
std::cout<<result<<"=RatDiff"<<std::endl;
#endif

return result;
}

std::string ComparerMeasure :: doIntRatio(ValueSet *pValSet1, ValueSet *pValSet2)
{
double a, b, c;
char s[25];
std::string result;

a= atof(pValSet1->labValVal.c_str());
b= atof(pValSet2->labValVal.c_str());

c= a / b;

// convert to string again:
sprintf (s, "%f", c);

result= std::string(s);

#ifdef DEBUG_COMPMEASURE
std::cout<<result<<"=IntRatio"<<std::endl;
#endif

return result;
}

std::string ComparerMeasure :: doRatRatio(ValueSet *pValSet1, ValueSet *pValSet2)
{
double a, b, c;
char s[25];
std::string result;

a= atof(pValSet1->labValVal.c_str());
b= atof(pValSet2->labValVal.c_str());

c= a / b;

// convert to string again:
sprintf(s, "%f", c);

result= std::string(s);

#ifdef DEBUG_COMPMEASURE
std::cout<<result<<"=RatRatio"<<std::endl;
#endif

return result;
}


std::string ComparerMeasure :: doIntProduct(ValueSet *pValSet1, ValueSet *pValSet2)
{
long a, b, c;
char s[25];
std::string result;

a= atol(pValSet1->labValVal.c_str());
b= atol(pValSet2->labValVal.c_str());

c= a * b;

// convert to string again:
sprintf (s, "%d", c);

result= std::string(s);

#ifdef DEBUG_COMPMEASURE
std::cout<<result<<"=IntProduct"<<std::endl;
#endif

return result;
}

std::string ComparerMeasure :: doRatProduct(ValueSet *pValSet1, ValueSet *pValSet2)
{
double a, b, c;
char s[25];
std::string result;

a= atof(pValSet1->labValVal.c_str());
b= atof(pValSet2->labValVal.c_str());

c= a * b;

// convert to string again:
sprintf(s, "%f", c);

result= std::string(s);

#ifdef DEBUG_COMPMEASURE
std::cout<<result<<"=RatProduct"<<std::endl;
#endif

return result;
}

std::string ComparerMeasure :: doPercDev(ValueSet *pValSet1, ValueSet *pValSet2)
{
double a, b, c, d;
char s[25];
std::string result;

a= atof(pValSet1->labValVal.c_str());
b= atof(pValSet2->labValVal.c_str());

c= b - a;
d= c / a * 100;

// convert to string again:
sprintf (s, "%f", d);

result= std::string(s);

#ifdef DEBUG_COMPMEASURE
std::cout<<result<<"=PercDev"<<std::endl;
#endif

return result;
}
