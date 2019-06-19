/*============================================================================*/
/* test.cpp - general testing                                                 */
/*============================================================================*/
  
#include "StorageEngine.h"
#include "Test.hpp"

/*----------------------------------------------------------------------------*/
/* test1                                                                      */
/*----------------------------------------------------------------------------*/
void vTest1(void)
{
  File oFile;
  i8 iChunk = C_32KB;
  char caChunk[iChunk];
  Timer oTimer;
  
  vTrace("Reading lineitem.tbl in chunks of %lld\n"_Stz, iChunk);
  bool b = oFile.bOpenRead("/Volumes/Data/Users/alkayamarti/tpch_SF10/lineitem.csv"_Stz);
  ASSERT(b);
  
  oTimer.vStart();
  i8 nBytesRead = oFile.nReadBytes(caChunk,iChunk);
  
  while (nBytesRead) {
    nBytesRead = oFile.nReadBytes(caChunk,iChunk);
  }
  oTimer.vUpdate();
  vTrace("duration %lldms\n"_Stz,oTimer.iMilliseconds());
  
  oFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* test2                                                                      */
/*----------------------------------------------------------------------------*/
void vTest2(void)
{
}

/*----------------------------------------------------------------------------*/
/* test3                                                                      */
/*----------------------------------------------------------------------------*/
void vTest3(void)
{
  byte xVlq[16];
  
  i8 nBytes = IntLib::iIntToVlq(C_DATUM_NULL,xVlq);
  i8 i = IntLib::iVlqToInt(xVlq);
  assert(C_DATUM_NULL == i);
  
  nBytes = IntLib::iIntToVlq(0,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  assert(0 == i);
  
  nBytes = IntLib::iIntToVlq(30,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  assert(30 == i);
  
  nBytes = IntLib::iIntToVlq(-30,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  assert(-30 == i);
  
  nBytes = IntLib::iIntToVlq(31,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  assert(31 == i);
  
  nBytes = IntLib::iIntToVlq(32,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  assert(32 == i);
  
  nBytes = IntLib::iIntToVlq(127,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  assert(127 == i);
  
  nBytes = IntLib::iIntToVlq(128,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  assert(128 == i);
  
  nBytes = IntLib::iIntToVlq(-130,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  assert(-130 == i);
  
  nBytes = IntLib::iIntToVlq(-130,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  assert(-130 == i);
  
  nBytes = IntLib::iIntToVlq(4096,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  assert(4096 == i);
  
}

/*----------------------------------------------------------------------------*/
/* test4                                                                      */
/*----------------------------------------------------------------------------*/
void vTest4(void)
{
  vTrace("vTest4\n"_Stz);

  HashMap* pHashMap = new HashMap(false,13999,8);
  
  //s32 sKey("ALPHA"_Stz);
  s32 sKey;
  
  i8 iKey = 0;
  for (i8 nRepeats = 0; nRepeats < 10; nRepeats++) {
    for (i8 nTimes = 0; nTimes < 1000000; nTimes++) {
      sKey.vSprint("%02lld"_Stz,iKey);
      
      pHashMap->vInsert((byte*)sKey.aChar(),sKey.iLength(),iKey,false);
      iKey++;
    }
    pHashMap->vAnalysis(false);
  }
  
//  HashCursor xCursor(pHashMap);
//
//  while (xCursor.bIterate()) {
//    StrLib::vCopyLen(sKey.aChar(),(char*)xCursor.m_yaKey,xCursor.m_nBytes);
//    vTrace("[%lld] %s/%lld %lld\n"_Stz,
//           xCursor.m_ia_HashEntry[xCursor.m_i_Stack],
//           sKey.aChar(),
//           xCursor.m_nBytes,
//           xCursor.m_iValue);
//  }
  
//  sKey.vSprint("%02lld"_Stz,4206);
//  pHashMap->vInsert((byte*)sKey.aChar(),sKey.iLength(),4206,true);
//
//  sKey.vSprint("%02lld"_Stz,4249);
//  pHashMap->vInsert((byte*)sKey.aChar(),sKey.iLength(),4249,true);
//
//  sKey.vSprint("%02lld"_Stz,25396);
//  pHashMap->vInsert((byte*)sKey.aChar(),sKey.iLength(),25396,true);
//
//  sKey.vSprint("%02lld"_Stz,28848);
//  pHashMap->vInsert((byte*)sKey.aChar(),sKey.iLength(),28848,true);
//
//  sKey.vSprint("%02lld"_Stz,40263);
//  pHashMap->vInsert((byte*)sKey.aChar(),sKey.iLength(),40263,true);
//
//  sKey.vSprint("%02lld"_Stz,48122);
//  pHashMap->vInsert((byte*)sKey.aChar(),sKey.iLength(),48122,true);
  
  //pHashMap->vAnalysis(false);
  
  delete pHashMap;
}

/*----------------------------------------------------------------------------*/
/* test5                                                                      */
/*----------------------------------------------------------------------------*/
void vTest5(void)
{
#define nINTS (C_1MB * 8)
  i8* aArray = (i8*)MemLib::aMallocBlock(8 * nINTS);
  
  for (i8 i_Int = 0; i_Int < nINTS; i_Int++)
    aArray[i_Int] = i_Int;
  
  Timer oTimer;
  oTimer.vStart();
  
  i8 iSum = 0;
  for (i8 i_Times = 0; i_Times < 1000; i_Times++)
    for (i8 i_Int = 0; i_Int < nINTS; i_Int++)
      iSum += aArray[i_Int];

  oTimer.vUpdate();
  vTrace("duration %lldms %lld\n"_Stz,oTimer.iMilliseconds(),iSum);
}

/*----------------------------------------------------------------------------*/
/* test6                                                                      */
/*----------------------------------------------------------------------------*/
void vTest6(void)
{
  RefBuffer* pRefBuffer = new RefBuffer;
  
  while (pRefBuffer->aAllotRefSet(2))
    {;}
  
  vDebugPoint();
  
  delete pRefBuffer;
}

/*----------------------------------------------------------------------------*/
/* test7                                                                      */
/*----------------------------------------------------------------------------*/
void vTest7(void)
{
  DicPath* pDicPath = new DicPath;

  DicStack* pDicStack = new DicStack;

  s32 sTableNm("j10"_Stz);
  
  s256 sDicPath("tenants.boray.catalogs.rapidsse.schemas.public.tables"_Stz);
  
  pDicPath->vSetPath(sDicPath);
  pDicStack->vSetStack(pDicPath);
  
  Table* pTable = Table::pFindTable(pDicStack,sTableNm);
  
  delete pDicStack;
  
  delete pDicPath;
}

/*----------------------------------------------------------------------------*/
/* test8                                                                      */
/*----------------------------------------------------------------------------*/
void vTest8(void)
{
  i3 i3B_1;
  i3 i3B_2;
  
  i3B_1 = 1;
  vTrace("i3B_1 %lld\n"_Stz,(i8)i3B_1);
  
  i3B_2 = i3B_1;
  vTrace("i3B_2 %lld\n"_Stz,(i8)i3B_2);
  
  i3B_2 = i3B_1 + 1;
  vTrace("i3B_2 %lld\n"_Stz,(i8)i3B_2);
  
  i3B_2 = i3B_1 + i3B_1 + 1;
  vTrace("i3B_2 %lld\n"_Stz,(i8)i3B_2);
  
  i3B_1 = 1;
  i3B_2 = 2;
  if (i3B_2 > i3B_1)
    vTrace("i3B_2 %lld > i3B_1 %lld\n"_Stz,(i8)i3B_2,(i8)i3B_1);
  
  i3B_1 = 0;
  vTrace("i3B_1 %lld\n"_Stz,(i8)i3B_1++);
  vTrace("i3B_1 %lld\n"_Stz,(i8)i3B_1);
  
  i3B_1 = 0;
  vTrace("i3B_1 %lld\n"_Stz,(i8)++i3B_1);
  vTrace("i3B_1 %lld\n"_Stz,(i8)i3B_1);
  
  i3B_1 = 8388608;
  vTrace("i3B_1 %lld\n"_Stz,(i8)i3B_1);
  
  i3B_1 = 16777216 - 1;
  vTrace("i3B_1 %lld\n"_Stz,(i8)i3B_1);
  
  return;
}

/*----------------------------------------------------------------------------*/
/* test9                                                                      */
/*----------------------------------------------------------------------------*/
//#define nElem (1024*1024*16)
//i8 xI8[nElem];
//
//void vTest9(void)
//{
//  i8 iSum = 0;
//
//  vTrace("iPmc_Info %llx\n"_Stz,iPmc_Info());
//
//  vTrace("Starting   iPmc_GetLlcMisses %lld\n"_Stz,iPmc_GetLlcMisses());
//  for (i8 i = 0; i < nElem; i++)
//    xI8[i] = i;
//  //vTrace("Sequential iPmc_GetLlcMisses %lld; iSum %lld\n"_Stz,iPmc_GetLlcMisses(),iSum);
//  for (i8 i = 0; i < nElem; i++) {
//    i8 iElem = Random::iInteger(nElem-1);
//    //vTrace("iElem %lld; iPmc_GetLlcMisses %llx\n"_Stz,iElem,iPmc_GetLlcMisses());
//    iSum += xI8[iElem];
//  }
//  vTrace("Random     iPmc_GetLlcMisses %lld; iSum %lld\n"_Stz,iPmc_GetLlcMisses(),iSum);
//}

/*----------------------------------------------------------------------------*/
/* test10                                                                      */
/*----------------------------------------------------------------------------*/
void vTest10(void)
{
  
  i8 i1 = Random::iInteger(-999999,999999);
  i8 i2 = Random::iInteger(-999999,999999);
  
  for (i8 i = 0; i < 10000000; i ++) {
    //i8 i1 = Random::iInteger(-999999,999999);
    //i8 i2 = Random::iInteger(-999999,999999);
  
    i8 iCmp = MemLib::iCompareWords(&i1, &i2, 8);
    //i8 iCmp2 = MemLib::iCompare2(&i1, &i2, 8);
  }
  
  
//    if (iCmp > 0)
//      vTrace("%lld > %lld\n"_Stz, i1, i2);
//    else if (iCmp < 0)
//      vTrace("%lld > %lld\n"_Stz, i2, i1);
//    else
//      vTrace("%lld == %lld\n"_Stz, i1, i2);
  
}

/*----------------------------------------------------------------------------*/
/* test11                                                                     */
/*----------------------------------------------------------------------------*/
void vTest11(void)
{
  
  CacheLine xCacheLine;

  xCacheLine.vSetAll('a');
  
  // set bytes to be moved + 2 to 'b'
  xCacheLine.vSet(5, 12, 'b');
  
  // dump the cacheline
  MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
  
  // cut 10 bytes from offset 6
  MemLib::vCut(&xCacheLine, sizeof(CacheLine), 6, 10,CN_NONE);
  
  // dump cacheline after cut
  MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
  
  xCacheLine.vSetAll('a');

  // set bytes to be moved + 2 to 'b'
  xCacheLine.vSet(4, sizeof(CacheLine) - 6, 'b');
  
  // cut last but 6 bytes
  MemLib::vCut(&xCacheLine, sizeof(CacheLine), 6, sizeof(CacheLine) - 6,CN_NONE);
  
  // dump cacheline after cut
  MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
  
  xCacheLine.vSetAll('a');
  
  // set bytes to be moved + 2 to 'b'
  xCacheLine.vSet(0, 8, 'b');
  
  // cut frst 6 bytes
  MemLib::vCut(&xCacheLine, sizeof(CacheLine), 0, 6, CN_NONE);
  
  // dump cacheline after cut
  MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
  
}

//****************************************************************************//
/* QUICKSORT using cursor with pointers to array elements                     */
//****************************************************************************//

/*
 algorithm quicksort(A, lo, hi) is
 if lo < hi then
 p := partition(A, lo, hi)
 quicksort(A, lo, p)
 quicksort(A, p + 1, hi)
 
 algorithm partition(A, lo, hi) is
 pivot := A[lo + (hi - lo) / 2]
 i := lo - 1
 j := hi + 1
 loop forever
 do
 i := i + 1
 while A[i] < pivot
 
 do
 j := j - 1
 while A[j] > pivot
 
 if i >= j then
 return j
 
 swap A[i] with A[j]
 */

/*----------------------------------------------------------------------------*/
/* Partition                                                                  */
/*----------------------------------------------------------------------------*/
void vPartition_MarkI(i8* A, Cursor& xLowCursor, Cursor& xHighCursor, Cursor& xPartitionCursor)
{
  
  // point a cusror at the pivot point
  Cursor xPivotCursor(xLowCursor, xHighCursor);
  
  vTrace("iPivot %lld Low %lld High %lld\n"_Stz,
         *xPivotCursor,
         *xLowCursor.m_aElement,
         *xHighCursor.m_aElement);
  
  // define the start and end positions
  Cursor xStart(xLowCursor);
  Cursor xEnd(xHighCursor);
  
  // step back with the start position
  xStart.vPrevious();
  
  // step ahead with the end position
  xEnd.vNext();
  
  // get the pivot element value
  i8 iPivot = *xPivotCursor;
  
  // Move start and end toward each other, until they detect an inversion:
  // a pair of elements, one greater than or equal to the pivot,
  // one lesser or equal, that are in the wrong order relative to each other.
  
  while (true) {
    
    // move start cursor forward till we go beyond the pivot value
    do {
      xStart.vNext();
    } while (xStart < iPivot);

    // move end cursor backward till pivot value is beyond it
    do {
      xEnd.vPrevious();
    } while (xEnd > iPivot);

    // crossed
    if ( xStart >= xEnd) {
      xPartitionCursor.vSet(xEnd);
      vTrace("Returning %lld\n"_Stz, *xEnd);
      return;
    }
    
    // swap the inverted elements
    vTrace("Swapping %lld %lld\n"_Stz, *xStart, *xEnd);
    // swap A[i] and A[j]
    xStart.vSwap(xEnd);
    
    for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
      vTrace("A[%lld] = %lld %x\n"_Stz, i, A[i], &A[i]);
    }
    
  }
}

//****************************************************************************//
/* QUICKSORT using indexes to array elements                                  */
//****************************************************************************//
/*----------------------------------------------------------------------------*/
/* Partition                                                                  */
/*----------------------------------------------------------------------------*/
i8 iPartition(i8* A, i8 iLow, i8 iHigh)
{
  i8 iPivot = A[iLow + (iHigh - iLow) / 2];
  
  i8 i = iLow - 1;
  i8 j = iHigh + 1;
  
  while (true) {
    do {
      i++;
    } while (A[i] < iPivot);

    do {
      j--;
    } while (A[j] > iPivot);
    
    if ( i >= j) {
      vTrace("Returning j %lld\n"_Stz, j);
      return j;
    }
    
    // swap A[i} and A[j]
    i8 iTemp = A[j];
    A[j] = A[i];
    A[i] = iTemp;
    
    vTrace("Swapping \n"_Stz);
    for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
      vTrace("A[%lld] = %lld %x\n"_Stz, i, A[i], &A[i]);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* QuickSort                                                                  */
/*----------------------------------------------------------------------------*/
void vQuickSort(i8* A, i8 iLow, i8 iHigh) {
  
  if (iLow < iHigh) {
    i8 iPivot = iPartition(A, iLow, iHigh);
    vTrace("PIVOT %lld\n"_Stz, A[iPivot]);
    
    for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
      vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
    }
    vQuickSort(A, iLow, iPivot);
    vQuickSort(A, iPivot + 1, iHigh);
  }
  
}

/*----------------------------------------------------------------------------*/
/* test12 : SORING INT ARRAY USING INDEXES INTO THE ARRAY                     */
/*----------------------------------------------------------------------------*/
void vTest12(void)
{
  vTrace("Quicksort: Hoare partition scheme (INDEXES)\n"_Stz);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create an array of integers
  i8 A[ARRAY_ELEMENTS];
  
  vTrace("\nSorting array\n"_Stz);
    
  for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
    A[i] = Random::iInteger(0,1000);
    vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
  }

  //--------------------------------------------------------------------------
  // EXPERIMENT 1:
  vTrace("SORTING USING ARRAY INDEXES\n"_Stz);
   vQuickSort(A, 0, ARRAY_ELEMENTS - 1);

  vTrace("\nSorted array:\n"_Stz);

  for (i8 i = 0; i < ARRAY_ELEMENTS; i++) {
    vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
    if (i)
      RASSERT(A[i] >= A[i-1]);
  }
}

//****************************************************************************//
/* QUICKSORT using pointers to array elements                                 */
//****************************************************************************//
/*----------------------------------------------------------------------------*/
/* Partition                                                                  */
/*----------------------------------------------------------------------------*/
i8* aPartition(i8* A, i8* aLow, i8* aHigh)
{
  
  i8* aPivot = aLow + (aHigh - aLow) / 2;
  i8 iPivot = *aPivot;
  vTrace("aPivot %lld Low %lld High %lld\n"_Stz, *aPivot, *aLow, *aHigh);
  
  i8* i = aLow - 1;
  i8* j = aHigh + 1;
  
  while (true) {
    do {
      i++;
    } while (*i < iPivot);
    
    do {
      j--;
    } while (*j > iPivot);
    
    if ( i >= j) {
      vTrace("Returning j 0x%x\n"_Stz, j);
      return j;
    }
    
    // swap A[i} and A[j]
    vTrace("Swapping %lld %lld\n"_Stz, *i, *j);
    i8 iTemp = *j;
    *j = *i;
    *i = iTemp;
    
    for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
      vTrace("A[%lld] = %lld %x\n"_Stz, i, A[i], &A[i]);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* QuickSort                                                                  */
/*----------------------------------------------------------------------------*/
void vQuickSort(i8* A, i8* aLow, i8* aHigh) {
  
  if (aLow < aHigh) {
    i8* aPivot = aPartition(A, aLow, aHigh);
    
    vTrace("PIVOT %lld\n"_Stz, *aPivot);
    
    for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
      vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
    }
    
    vQuickSort(A, aLow, aPivot);
    vQuickSort(A, aPivot+1, aHigh);
  }
  
}

/*----------------------------------------------------------------------------*/
/* test13 --  SORING INT ARRAY USING POINTERS TO ELEMENTS IN THE ARRAY        */
/*----------------------------------------------------------------------------*/
void vTest13(void)
{
  vTrace("Quicksort: Hoare partition scheme\n"_Stz);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create an array of integers
  i8 A[ARRAY_ELEMENTS];
  
  vTrace("\nSorting array\n"_Stz);
  
  for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
    A[i] = Random::iInteger(0,1000);
    vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
  }
  
  //--------------------------------------------------------------------------
  // EXPERIMENT 2: SORTING USING ARRAY ELEMENT POINTERS
  vQuickSort(A, &A[0], &A[ARRAY_ELEMENTS - 1]);
  
  vTrace("\nSorted array:\n"_Stz);
  
  for (i8 i = 0; i < ARRAY_ELEMENTS; i++) {
    vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
    if (i)
      RASSERT(A[i] >= A[i-1]);
  }

}

//****************************************************************************//
/* QUICKSORT using cursors to array elements                                  */
//****************************************************************************//
/*----------------------------------------------------------------------------*/
/* Partition                                                                  */
/*----------------------------------------------------------------------------*/
void vPartition(i8* A, Cursor& xLowCursor, Cursor& xHighCursor, Cursor& xPartitionCursor)
{
  
  // point a cusror at the pivot point
  Cursor xPivotCursor(xLowCursor, xHighCursor);
  
  vTrace("iPivot %lld Low %lld High %lld\n"_Stz,
         *xPivotCursor,
         *xLowCursor.m_aElement,
         *xHighCursor.m_aElement);
  
  // define the start and end positions
  Cursor xLeft(xLowCursor);
  Cursor xRight(xHighCursor);
  
  // get the pivot element value
  i8 iPivotValue = *xPivotCursor;
  
  // Move start and end toward each other, until they detect an inversion:
  // a pair of elements, one greater than or equal to the pivotValue,
  // one lesser or equal, that are in the wrong order relative to each other.
  
  while (true) {
    
    vTrace("A: Start %lld End %lld\n"_Stz, *xLeft, *xRight);
    
    // move start cursor forward till we go beyond the pivot value
    while (xLeft < iPivotValue) {
      xLeft.vNext();
    }
    
    // move end cursor backward till pivot value is beyond it
    while (xRight > iPivotValue) {
      xRight.vPrevious();
    }
    
    // crossed
    if ( xLeft >= xRight) {
      xPartitionCursor.vSet(xRight);
      vTrace("Returning %lld\n"_Stz, *xRight);
      return;
    }
    
    // swap the inverted elements
    vTrace("Swapping %lld %lld\n"_Stz, *xLeft, *xRight);
    // swap A[i] and A[j]
    xLeft.vSwap(xRight);
    
//    for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
//      vTrace("A[%lld] = %lld %x\n"_Stz, i, A[i], &A[i]);
//    }
    
    // move start cursor forward
    xLeft.vNext();
    
    // move end cursor backward
    xRight.vPrevious();
  }
}

/*----------------------------------------------------------------------------*/
/* QuickSort                                                                  */
/*----------------------------------------------------------------------------*/
void vQuickSort(i8* A, Cursor& xLowCursor, Cursor& xHighCursor)
{
  
  // create a Partition cursor intially with a nullptr element
  Cursor xPartitionCursor(nullptr);
  
  if (xLowCursor < xHighCursor) {
    vPartition(A, xLowCursor, xHighCursor, xPartitionCursor);
    
    vTrace("PIVOT %lld\n"_Stz, *xPartitionCursor.m_aElement);
    
    for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
      vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
    }
    
    // quick sort from low to partition
    vQuickSort(A, xLowCursor, xPartitionCursor);
    
    // quick sort from partition+1 to high
    Cursor xPartitionNext(xPartitionCursor.m_aElement + 1);
    vQuickSort(A, xPartitionNext, xHighCursor);
  }
  
}

/*----------------------------------------------------------------------------*/
/* test14 -- SORTING AN ARRAY OF INTEGERS USING CURSORS                       */
/*----------------------------------------------------------------------------*/
void vTest14(void)
{
  vTrace("Quicksort: Hoare partition scheme\n"_Stz);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create an array of integers
  i8 A[ARRAY_ELEMENTS];
  
  vTrace("\nSorting array\n"_Stz);
  
  for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
    A[i] = Random::iInteger(0,1000);
    vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
  }
  
  //--------------------------------------------------------------------------
  // EXPERIMENT 3: SORTING USING CURSORS
  
  // create a Low cursor pointing to the first element in the array
  Cursor xLow(&A[0]);

  // create a High cursor pointing to the first element in the array
  Cursor xHigh(&A[ARRAY_ELEMENTS-1]);

  // call quick sort (recursive call)
  vQuickSort(A, xLow, xHigh);

  vTrace("\nSorted array:\n"_Stz);
  
  for (i8 i = 0; i < ARRAY_ELEMENTS; i++) {
    vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
    if (i)
      RASSERT(A[i] >= A[i-1]);
  }

}

//****************************************************************************//
/* QUICKSORT using cursors to a buffer of array elements                      */
//****************************************************************************//
/*----------------------------------------------------------------------------*/
/* Partition                                                                  */
/*----------------------------------------------------------------------------*/
void vPartitionBuffer(ArrayBuffer* pArrayBuffer, BufferCursor& xLowCursor,
                      BufferCursor& xHighCursor, BufferCursor& xPartitionCursor)
{
  
  // point a cusror at the pivot point
  BufferCursor xPivotCursor(xLowCursor, xHighCursor);
  
  vTrace("iPivot %lld Low %lld High %lld\n"_Stz,
         *xPivotCursor,
         *xLowCursor.m_aElement,
         *xHighCursor.m_aElement);
  
  // define the start and end positions
  BufferCursor xLeft(xLowCursor);
  BufferCursor xRight(xHighCursor);
  
  // get the pivot element value
  i8 iPivotValue = *xPivotCursor;
  
  // Move start and end toward each other, until they detect an inversion:
  // a pair of elements, one greater than or equal to the pivotValue,
  // one lesser or equal, that are in the wrong order relative to each other.
  
  while (true) {
    
    vTrace("A: Start %lld End %lld\n"_Stz, *xLeft, *xRight);
    
    // move start cursor forward till we go beyond the pivot value
    while (xLeft < iPivotValue) {
      xLeft.vNext();
    }
    
    // move end cursor backward till pivot value is beyond it
    while (xRight > iPivotValue) {
      xRight.vPrevious();
    }
    
    // crossed
    if ( xLeft >= xRight) {
      xPartitionCursor.vSet(xRight);
      vTrace("Returning %lld\n"_Stz, *xRight);
      return;
    }
    
    // swap the inverted elements
    vTrace("Swapping %lld %lld\n"_Stz, *xLeft, *xRight);
    // swap A[i] and A[j]
    xLeft.vSwap(xRight);
    
    for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
      vTrace("A[%lld] = %lld\n"_Stz, i, pArrayBuffer->A[i]);
    }
    
    // move start cursor forward
    xLeft.vNext();
    
    // move end cursor backward
    xRight.vPrevious();
  }
}

/*----------------------------------------------------------------------------*/
/* QuickSort                                                                  */
/*----------------------------------------------------------------------------*/
void vQuickSortBuffer(ArrayBuffer* pArrayBuffer, BufferCursor& xLowCursor, BufferCursor& xHighCursor)
{
  
  // create a Partition cursor intially with a nullptr element
  BufferCursor xPartitionCursor(nullptr, C_UNKNOWN);
  
  if (xLowCursor < xHighCursor) {
    vPartitionBuffer(pArrayBuffer, xLowCursor, xHighCursor, xPartitionCursor);
    
    vTrace("PIVOT %lld\n"_Stz, *xPartitionCursor.m_aElement);
    
    for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
      vTrace("A[%lld] = %lld\n"_Stz, i, pArrayBuffer->A[i]);
    }
    
    // quick sort from low to partition
    vQuickSortBuffer(pArrayBuffer, xLowCursor, xPartitionCursor);
    
    // quick sort from partition+1 to high
    BufferCursor xPartitionNext(xPartitionCursor.m_pArrayBuffer, xPartitionCursor.m_aElement + 1);
    vQuickSortBuffer(pArrayBuffer, xPartitionNext, xHighCursor);
  }
  
}

/*----------------------------------------------------------------------------*/
/* test15                                                                     */
/*----------------------------------------------------------------------------*/
void vTest15(void)
{
  vTrace("Quicksort: Hoare partition scheme\n"_Stz);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create an array of integers
  i8 A[ARRAY_ELEMENTS];
  
  //for (i8 iLoop = 0; iLoop < 10; iLoop++ ) {
  
  vTrace("\nSorting array\n"_Stz);
  
  for (i8 i = 0; i < ARRAY_ELEMENTS; i ++) {
    A[i] = Random::iInteger(0,1000);
    vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
  }
  
  //--------------------------------------------------------------------------
  // EXPERIMENT 4: SORTING USING CURSORS INTO BUFFER OF ARRAY
  vTrace("SORTING USING CUSRORS INTO BUFFER OF ARRAY\n"_Stz);
  
  ArrayBuffer xBuffer;
  
  // create a Low cursor pointing to the first element in the array
  BufferCursor xLow(&xBuffer, (i8)0);
  
  // create a High cursor pointing to the first element in the array
  BufferCursor xHigh(&xBuffer, ARRAY_ELEMENTS-1);
  
  // call quick sort (recursive call)
  vQuickSortBuffer(&xBuffer, xLow, xHigh);
  
  vTrace("\nSorted array:\n"_Stz);
  
  for (i8 i = 0; i < ARRAY_ELEMENTS; i++) {
    vTrace("A[%lld] = %lld\n"_Stz, i, xBuffer.A[i]);
    if (i)
      RASSERT(xBuffer.A[i] >= xBuffer.A[i-1]);
  }
  
}

/*----------------------------------------------------------------------------*/
/* test16                                                                     */
/*----------------------------------------------------------------------------*/
void vTest16(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* test17                                                                     */
/*----------------------------------------------------------------------------*/
void vTest17(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* test18                                                                     */
/*----------------------------------------------------------------------------*/
void vTest18(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* test19                                                                     */
/*----------------------------------------------------------------------------*/
void vTest19(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* test20                                                                     */
/*----------------------------------------------------------------------------*/
void vTest20(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* test21                                                                     */
/*----------------------------------------------------------------------------*/
void vTest21(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* test22                                                                     */
/*----------------------------------------------------------------------------*/
void vTest22(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* test23                                                                     */
/*----------------------------------------------------------------------------*/
void vTest23(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* test24                                                                     */
/*----------------------------------------------------------------------------*/
void vTest24(void)
{
  
}

/*----------------------------------------------------------------------------*/
/* test25                                                                     */
/*----------------------------------------------------------------------------*/
void vTest25(void)
{
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
