


#include "SamplingData.hpp"


typedef unsigned long long u64;
void vInsertIntData(SamplingData &SampData)
{

  // Read in from a file
  /*
  fstream infile;
  int array[20];
  int i=0;
  char cNum[10] ;
  infile.open ("sample.txt", ifstream::in);
  if (infile.is_open())
  {
    while (infile.good())
    {
      infile.getline(cNum, 256, ',');
      arInt[i]= atoi(cNum) ;
      i++ ;
    }
    infile.close();
  }
  else
  {
    cout << "Error opening file";
  }
   */
  long nMachines = 0;
  u64 ulVal = 0;
  for (int i=0; i< MAX_ARRAY; i++){

    ulVal = (rand() % MAX_ARRAY);

    //printf("lVal=%ld, ", lVal);

    SampData.vInput(ulVal, nMachines);
  }

  SampData.vFinalize();
}




void vInsertStrData(SamplingData &SampData)
{

  // Read in from a file
  /*
  fstream infile;
  int array[20];
  int i=0;
  char cNum[10] ;
  infile.open ("sample.txt", ifstream::in);
  if (infile.is_open())
  {
    while (infile.good())
    {
      infile.getline(cNum, 256, ',');
      arInt[i]= atoi(cNum) ;
      i++ ;
    }
    infile.close();
  }
  else
  {
    cout << "Error opening file";
  }
   */
  long nMachines = 0;
  u64 ulVal = 0;

  for (int i=0; i< MAX_ARRAY; i++){
    string strVal;
    ulVal = (rand() % 2);

    if (ulVal==0)
      strVal.append("pending");
    else
      strVal.append("shipped");

    //printf("lVal=%llu, ", ulVal);

    SampData.vInput(strVal);
  }
  SampData.vFinalize();
}

void vInsertDteData(SamplingData &SampData)
{

  // Read in from a file
  /*
  fstream infile;
  int array[20];
  int i=0;
  char cNum[10] ;
  infile.open ("sample.txt", ifstream::in);
  if (infile.is_open())
  {
    while (infile.good())
    {
      infile.getline(cNum, 256, ',');
      arInt[i]= atoi(cNum) ;
      i++ ;
    }
    infile.close();
  }
  else
  {
    cout << "Error opening file";
  }
   */
  long nMachines = 0;
  u64 ulVal = 0;



  SampData.vInput("1996-01-02");
  SampData.vInput("1993-10-14");
  SampData.vInput("1996-01-10");
  SampData.vInput("1997-02-10");
  SampData.vInput("1996-01-12");
  SampData.vInput("1998-01-10");
  SampData.vInput("1996-01-20");
  SampData.vInput("1992-01-10");
  SampData.vInput("1993-10-14");
  SampData.vInput("1996-01-10");
  SampData.vInput("1991-02-10");
  SampData.vInput("1996-01-10");
  SampData.vInput("1995-03-10");
  SampData.vInput("1996-04-11");
  SampData.vInput("1996-04-12");
  SampData.vInput("1996-04-10");
  SampData.vInput("1994-01-10");
  SampData.vInput("1994-05-10");
  SampData.vInput("1996-09-10");
  SampData.vInput("1993-07-10");
  SampData.vInput("1993-01-10");
  SampData.vInput("1991-11-10");
  SampData.vInput("1991-12-10");
  SampData.vInput("1990-07-10");

  SampData.vFinalize();
}

void vInsertDblData(SamplingData &SampData)
{

  SampData.vInput(2.00000002);
  SampData.vInput(2.00000002);
  SampData.vInput(2.0000003);
  SampData.vInput(5.01);
  SampData.vInput(6.05);
  SampData.vInput(1.9999999);
  SampData.vInput(2.000000001);
  SampData.vInput(6.09);
  SampData.vInput(7.1);
  SampData.vInput(8.25);
  SampData.vInput(9.25);
  SampData.vInput(7.709);
  SampData.vInput(8.04);
  SampData.vInput(7.5);
  SampData.vInput(7.455577);
  SampData.vInput(8.43611);
  SampData.vInput(8.44);
  SampData.vInput(102.01);
  SampData.vInput(102.02);
  SampData.vInput(102.03);
  SampData.vInput(102.03);
  SampData.vInput(102.03);
  SampData.vInput(102.03);
  SampData.vInput(102.03);
  SampData.vInput(102.03);

  SampData.vFinalizeD();

}

int main() {

  srand(time(NULL));

  SamplingData SampIntData;

  vInsertIntData(SampIntData);

  u64 lVal =  9027;
  int iCnt = SampIntData.lDiscrete(lVal);

  printf( "%lld appears %d times.\n", lVal, iCnt);

  SampIntData.vPrint();
  printf("==========================================\n");
  ///////////////////////////////////

  SamplingData SampStrData;
  vInsertStrData(SampStrData);

  iCnt = SampStrData.lDiscrete("shipped");
  printf( "\n\'%s\' appears %d times.\n", "shipped", iCnt);

  iCnt = SampStrData.lDiscrete("pending");
  printf( "\n\'%s\' appears %d times.\n", "pending", iCnt);
  SampStrData.vPrint();
  printf("==========================================\n");
  ///////////////////////////////////

  SamplingData SampDteData;
  vInsertDteData(SampDteData);

  iCnt = SampDteData.lDiscrete("1996-01-10");
  printf( "\n\'%s\' appears %d times.\n", "1996-01-10", iCnt);

  iCnt = SampDteData.lDiscrete("1996-02-10");
  printf( "\n\'%s\' appears %d times.\n", "1996-02-10", iCnt);
  SampDteData.vPrint();
  printf("==========================================\n");


  SamplingData SampDblData;
  vInsertDblData(SampDblData);

  iCnt = SampDblData.lDiscrete(102.030000);
  printf( "\n%.6lf appears %d times.\n", 102.030000, iCnt);

  iCnt = SampDblData.lDiscrete(2.00000002);
  printf( "\n%.8lf appears %d times.\n", 2.00000002, iCnt);

  SampDblData.vPrintD();
  printf("==========================================\n");




  return 0;
}