/*============================================================================*/
/* Test.cpp - general testing                                                 */
/*============================================================================*/
  
#include "StorageEngine.h"
#include "Test.hpp"

/*----------------------------------------------------------------------------*/
/* test1                                                                      */
/*----------------------------------------------------------------------------*/
void vTest1(void)
{
  File oFile(nullptr);
  i8 iChunk = C_32KB;
  char caChunk[iChunk];
  Timer oTimer;
  
  vTrace("Reading lineitem.tbl in chunks of %lld\n"_Stz, iChunk);
  bool b = oFile.bOpenRead("/Volumes/Rows/Users/alkayamarti/tpch_SF10/lineitem.csv"_Stz);
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
  vSkipListTest();
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
      
      pHashMap->vInsert((byte*)sKey.azChar(),sKey.nLength(),iKey,false);
      iKey++;
    }
    pHashMap->vAnalysis(false);
  }
  
//  HashCursor xCursor(pHashMap);
//
//  while (xCursor.bIterate()) {
//    StrLib::vCopyLen(sKey.azChar(),(char*)xCursor.m_yaKey,xCursor.m_nBytes);
//    vTrace("[%lld] %s/%lld %lld\n"_Stz,
//           xCursor.m_ia_HashEntry[xCursor.m_i_Stack],
//           sKey.azChar(),
//           xCursor.m_nBytes,
//           xCursor.m_iValue);
//  }
  
//  sKey.vSprint("%02lld"_Stz,4206);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),4206,true);
//
//  sKey.vSprint("%02lld"_Stz,4249);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),4249,true);
//
//  sKey.vSprint("%02lld"_Stz,25396);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),25396,true);
//
//  sKey.vSprint("%02lld"_Stz,28848);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),28848,true);
//
//  sKey.vSprint("%02lld"_Stz,40263);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),40263,true);
//
//  sKey.vSprint("%02lld"_Stz,48122);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),48122,true);
  
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
//  RefBuffer* pRefBuffer = new RefBuffer(0);
//  
//  while (pRefBuffer->aAllotRefSet(2))
//    {;}
//  
//  vDebugPoint();
//  
//  delete pRefBuffer;
}

/*----------------------------------------------------------------------------*/
/* test7                                                                      */
/*----------------------------------------------------------------------------*/
void vTest7(void)
{
  vTrace("Arrange Test\n"_Stz);
  
  Arrange* pArrange = new Arrange;
  
  Dic* pRootDic = AM::pRootDic();
  
  Word* pWord = nullptr;
  
  while ( (pWord = pRootDic->pEachWord(pWord)) )
    pArrange->vInsert(pWord->szWordNm(),pWord);
  
  pArrange->vSequence();
  
  void* vpCursor;

  //pArrange->vBeginIter(&vpCursor);
  
  for (pArrange->bFirst(&vpCursor);
       vpCursor;
       pArrange->bNext(&vpCursor))
    vTrace("  [%03lld] %s\n"_Stz,
           pArrange->iSeqNo(&vpCursor),
           pArrange->szObjNm(&vpCursor));
  
  //pArrange->vEndIter(&vpCursor);

  delete pArrange;
  
  vTrace("End Arrange Test\n"_Stz);
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
  
  i8 i1 = RndLib::iInteger(-999999,999999);
  i8 i2 = RndLib::iInteger(-999999,999999);
  
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

  xCacheLine.vSet(0,'a',sizeof(CacheLine));
  
  // set bytes to be moved + 2 to 'b'
  xCacheLine.vSet(5, 'b', 12);
  
  // dump the cacheline
  MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
  
  // cut 10 bytes from offset 6
  MemLib::vCut(&xCacheLine, sizeof(CacheLine), 6, 10,CN_NONE);
  
  // dump cacheline after cut
  MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
  
  xCacheLine.vSet(0,'a',sizeof(CacheLine));

  // set bytes to be moved + 2 to 'b'
  xCacheLine.vSet(4, 'b', sizeof(CacheLine) - 6);
  
  // cut last but 6 bytes
  MemLib::vCut(&xCacheLine, sizeof(CacheLine), 6, sizeof(CacheLine) - 6,CN_NONE);
  
  // dump cacheline after cut
  MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
  
  xCacheLine.vSet(0,'a',sizeof(CacheLine));
  
  // set bytes to be moved + 2 to 'b'
  xCacheLine.vSet(0, 'b', 8);
  
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
    
    for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
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
    for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
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
    
    for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
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
  i8 A[C_ARRAY_ELEMENTS];
  
  vTrace("\nSorting array\n"_Stz);
    
  for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
    A[i] = RndLib::iInteger(0,1000);
    vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
  }

  //--------------------------------------------------------------------------
  // EXPERIMENT 1:
  vTrace("SORTING USING ARRAY INDEXES\n"_Stz);
   vQuickSort(A, 0, C_ARRAY_ELEMENTS - 1);

  vTrace("\nSorted array:\n"_Stz);

  for (i8 i = 0; i < C_ARRAY_ELEMENTS; i++) {
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
    
    for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
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
    
    for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
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
  i8 A[C_ARRAY_ELEMENTS];
  
  vTrace("\nSorting array\n"_Stz);
  
  for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
    A[i] = RndLib::iInteger(0,1000);
    vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
  }
  
  //--------------------------------------------------------------------------
  // EXPERIMENT 2: SORTING USING ARRAY ELEMENT POINTERS
  vQuickSort(A, &A[0], &A[C_ARRAY_ELEMENTS - 1]);
  
  vTrace("\nSorted array:\n"_Stz);
  
  for (i8 i = 0; i < C_ARRAY_ELEMENTS; i++) {
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
    
//    for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
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
    
    for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
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
  i8 A[C_ARRAY_ELEMENTS];
  
  vTrace("\nSorting array\n"_Stz);
  
  for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
    A[i] = RndLib::iInteger(0,1000);
    vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
  }
  
  //--------------------------------------------------------------------------
  // EXPERIMENT 3: SORTING USING CURSORS
  
  // create a Low cursor pointing to the first element in the array
  Cursor xLow(&A[0]);

  // create a High cursor pointing to the first element in the array
  Cursor xHigh(&A[C_ARRAY_ELEMENTS-1]);

  // call quick sort (recursive call)
  vQuickSort(A, xLow, xHigh);

  vTrace("\nSorted array:\n"_Stz);
  
  for (i8 i = 0; i < C_ARRAY_ELEMENTS; i++) {
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
void vPartitionBuffer(ArrayBuffer* pArrayBuffer, BufCursor& xLowCursor, BufCursor& xHighCursor, BufCursor& xPivotCursor)
{
  
  // point a cursor at the pivot point
  BufCursor xPartitionCursor(xLowCursor, xHighCursor);
  
  vTrace("iPivot %lld Low %lld High %lld\n"_Stz,
         *xPivotCursor,
         *xLowCursor.m_aElement,
         *xHighCursor.m_aElement);
  
  // define the start and end positions
  BufCursor xLeft(xLowCursor);
  BufCursor xRight(xHighCursor);
  
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
    
    for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
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
void vQuickSortBuffer(ArrayBuffer* pArrayBuffer, BufCursor& xLowCursor, BufCursor& xHighCursor)
{
  
  // create a Partition cursor intially with a nullptr element
  BufCursor xPartitionCursor(nullptr, C_UNKNOWN);
  
  if (xLowCursor < xHighCursor) {
    vPartitionBuffer(pArrayBuffer, xLowCursor, xHighCursor, xPartitionCursor);
    
    vTrace("PIVOT %lld\n"_Stz, *xPartitionCursor.m_aElement);
    
    for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
      vTrace("A[%lld] = %lld\n"_Stz, i, pArrayBuffer->A[i]);
    }
    
    // quick sort from low to partition
    vQuickSortBuffer(pArrayBuffer, xLowCursor, xPartitionCursor);
    
    // quick sort from partition+1 to high
    BufCursor xPartitionNext(xPartitionCursor.m_pArrayBuffer, xPartitionCursor.m_aElement + 1);
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
  i8 A[C_ARRAY_ELEMENTS];
  
  //for (i8 iLoop = 0; iLoop < 10; iLoop++ ) {
  
  vTrace("\nSorting array\n"_Stz);
  
  for (i8 i = 0; i < C_ARRAY_ELEMENTS; i ++) {
    A[i] = RndLib::iInteger(0,1000);
    vTrace("A[%lld] = %lld\n"_Stz, i, A[i]);
  }
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // EXPERIMENT 4: SORTING USING CURSORS INTO BUFFER OF ARRAY
  vTrace("SORTING USING CUSRORS INTO BUFFER OF ARRAY\n"_Stz);
  
  ArrayBuffer xBuffer;
  
  // create a Low cursor pointing to the first element in the array
  BufCursor xLow(&xBuffer, (i8)0);
  
  // create a High cursor pointing to the first element in the array
  BufCursor xHigh(&xBuffer, C_ARRAY_ELEMENTS-1);
  
  // call quick sort (recursive call)
  vQuickSortBuffer(&xBuffer, xLow, xHigh);
  
  vTrace("\nSorted array:\n"_Stz);
  
  for (i8 i = 0; i < C_ARRAY_ELEMENTS; i++) {
    vTrace("A[%lld] = %lld\n"_Stz, i, xBuffer.A[i]);
    if (i)
      RASSERT(xBuffer.A[i] >= xBuffer.A[i-1]);
  }
  
}

//****************************************************************************//
/* QUICKSORT using cursors to a buffer of int elements                        */
//****************************************************************************//

/*----------------------------------------------------------------------------*/
/* Partition                                                                  */
/*----------------------------------------------------------------------------*/
/*
BufferCursor* pPartitionBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor,
                      BufferCursor* pHighCursor)
{
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // point a cusror at the pivot point
  BufferCursor* pPivotCursor = new BufferCursor(pLowCursor);
  
  vTrace("iPivot %lld Low %lld High %lld\n"_Stz,
         *pPivotCursor->m_aElement,
         *pLowCursor->m_aElement,
         *pHighCursor->m_aElement);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // define the start and end positions
  BufferCursor* pLeft = new BufferCursor(pLowCursor);
  BufferCursor* pRight = new BufferCursor(pHighCursor);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the pivot element value
  i8 iPivotValue = *pPivotCursor->m_aElement;
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move start and end toward each other, until they detect an inversion:
  // a pair of elements, one greater than or equal to the pivotValue,
  // one lesser or equal, that are in the wrong order relative to each other.
  
  while (true) {
    
    vTrace("\nLeft %lld Right %lld\n"_Stz, *pLeft->m_aElement, *pRight->m_aElement);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - -
    // move start cursor forward till we go beyond the pivot value
    while (pLeft->bIsLessThan(iPivotValue)) {
      pLeft->vMoveRight();
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move end cursor backward till pivot value is beyond it
    while (pRight->bIsGreaterThan(iPivotValue)) {
      pRight->vMoveLeft();
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // crossed
    if (pLeft->bIsAfterOrSame(pRight)) {
      // create a Partition cursor intially with a nullptr element
      BufferCursor* pPartitionCursor = new BufferCursor(pRight);
      vTrace("Returning %lld\n"_Stz, *pRight->m_aElement);
      return pPartitionCursor;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - -
    // swap the inverted eIright and left) lements
    vTrace("Swapping %lld %lld\n"_Stz, *pLeft->m_aElement, *pRight->m_aElement);
    pLeft->vSwap(pRight);
    //pIntBuffer->vDump();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move start cursor forward
    pLeft->vMoveRight();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move end cursor backward
    pRight->vMoveLeft();
  }
}
*/
/*----------------------------------------------------------------------------*/
/* QuickSort                                                                  */
/*----------------------------------------------------------------------------*/
/*
void vQuickSortBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor, BufferCursor* pHighCursor)
{
  
  if (pLowCursor->bIsBefore(pHighCursor)) {
    BufferCursor* pPartitionCursor = pPartitionBuffer(pIntBuffer, pLowCursor, pHighCursor);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // quick sort from low to partition
    vQuickSortBuffer(pIntBuffer, pLowCursor, pPartitionCursor);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // quick sort from partition+1 to high
    pPartitionCursor->vMoveRight();
    BufferCursor* pPartitionNext = new BufferCursor(pPartitionCursor);
    vQuickSortBuffer(pIntBuffer, pPartitionNext, pHighCursor);
  }
  
}
*/
/*----------------------------------------------------------------------------*/
/* test16                                                                     */
/*----------------------------------------------------------------------------*/
void vTest16(void)
{
/*
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // EXPERIMENT 5: SORTING BUFFER OF INTEGERS USING A BUFFER CURSOR
  vTrace("\n\n**************************************************"_Stz);
  vTrace("\nQUICKSORT: LOMUTO PARTITION SCHEME\n"_Stz);
  vTrace("SORTING USING CUSRORS INTO BUFFER OF INTEGER ARRAY\n"_Stz);
  vTrace("**************************************************"_Stz);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create an IntBuffer
  IntBuffer* pIntBuffer = new IntBuffer();
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Add random integer values to the buffer till the buffer is full
  while (true) {
    i8 iRandom = Random::iInteger(0,1000);
    if (pIntBuffer->bHasSpace())
      pIntBuffer->vAppendValue(iRandom);
    else
      break;
  }
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the buffer
  vTrace("\n\nSorting buffer:\n"_Stz);
  pIntBuffer->vDump();
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a Low cursor pointing to the first element in the array
  BufferCursor* pLowCursor = new BufferCursor(pIntBuffer, 0, pIntBuffer->aGetElement(0));
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a High cursor pointing to the first element in the array
  BufferCursor* pHighCursor = new BufferCursor(pIntBuffer, 0, pIntBuffer->aGetElement(pIntBuffer->nCount() - 1));
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // call quick sort (recursive call)
  vQuickSortBuffer(pIntBuffer, pLowCursor, pHighCursor);

  vTrace("\nSorted buffer:\n"_Stz);
  pIntBuffer->vDump();
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify that the data is sorted
  RASSERT(pIntBuffer->bVerifyOrder());
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete the bufferCursors
  delete pLowCursor;
  delete pHighCursor;
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete the intBuffer
  delete pIntBuffer;
 */
}

/*----------------------------------------------------------------------------*/
/* test17                                                                     */
/*----------------------------------------------------------------------------*/
void vTest17(void)
{
 /*
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // EXPERIMENT 5: SORTING BUFFER OF INTEGERS USING A BUFFER CURSOR
  vTrace("\n\n**************************************************"_Stz);
  vTrace("\nQUICKSORT: LOMUTO PARTITION SCHEME\n"_Stz);
  vTrace("SORTING USING CUSRORS INTO BUFFER OF INTEGER ARRAY\n"_Stz);
  vTrace("**************************************************"_Stz);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create an IntBuffer
  IntBuffer* pRootBuffer = new IntBuffer();
  IntBuffer* pIntBuffer = pRootBuffer;
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Add random integer values to the buffer till the buffer is full
  i8 iTotalValues = ELEMENTS * 6;
  
  while (iTotalValues--) {
    i8 iRandom = Random::iInteger(0,1000);
    if (pIntBuffer->bHasSpace())
      pIntBuffer->vAppendValue(iRandom);
    else {
      // create a new buffer
      IntBuffer* pNextBuffer = new IntBuffer();
      pIntBuffer->vNextBuffer(pNextBuffer);
      pNextBuffer->vPrevBuffer(pIntBuffer);
      pIntBuffer = pNextBuffer;
      pIntBuffer->vAppendValue(iRandom);
    }
  }

  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the buffer
  vTrace("\n\nSorting buffer:\n"_Stz);
  pIntBuffer = pRootBuffer;
  while (pIntBuffer) {
    pIntBuffer->vDump();
    pIntBuffer = pIntBuffer->pNextBuffer();
  }
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a Low cursor pointing to the first element in the array
  BufferCursor* pLowCursor = new BufferCursor(pRootBuffer, 0, pRootBuffer->aGetElement(0));
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a High cursor pointing to the first element in the array
  pIntBuffer = pRootBuffer;
  IntBuffer* pLastBuffer = nullptr;
  i8 iBufferNo = C_UNKNOWN;
  while (pIntBuffer) {
    pLastBuffer = pIntBuffer;
    iBufferNo++;
    pIntBuffer = pIntBuffer->pNextBuffer();
  }
    
  BufferCursor* pHighCursor = new BufferCursor(pLastBuffer, iBufferNo, pLastBuffer->aGetElement(pLastBuffer->nCount() - 1));
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // call quick sort (recursive call)
  vQuickSortBuffer(pRootBuffer, pLowCursor, pHighCursor);
  
  vTrace("\nSorted buffer:\n"_Stz);
  pIntBuffer = pRootBuffer;
  
  while (pIntBuffer) {
    pIntBuffer->vDump();
    pIntBuffer = pIntBuffer->pNextBuffer();
  }

  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify that the data is sorted
  RASSERT(pRootBuffer->bVerifyOrder());
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete the bufferCursors
  delete pLowCursor;
  delete pHighCursor;
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete the intBuffer
  delete pIntBuffer;
 */
}

/*----------------------------------------------------------------------------*/
/* test18                                                                     */
/*----------------------------------------------------------------------------*/
void vTest18(void)
{
  char a[C_1K];
  char b[C_1K];
  char c[C_1K];
  Timer oTimer;
  
  for (i8 iIndex = 0; iIndex < C_1K-1; iIndex++)
    a[iIndex] = 'a';
  a[C_1K-1] = '\0';
  
  i8 n = 0;
  
  oTimer.vStart();
  for (i8 iIndex = 0; iIndex < C_1K-1; iIndex++) {
    MemLib::vCopy(&c[0], &a[0], C_1K, CN_NONE);
    n++;
  }
  oTimer.vUpdate();
  vTrace("%s %lld\n"_Stz, c, n);
  vTrace("duration %lldns\n"_Stz,oTimer.iNanoseconds());
  
  n = 0;
  oTimer.vStart();
  for (i8 iIndex = 0; iIndex < C_1K-1; iIndex++) {
    i8 nL = StrLib::nCopy(&b[0],&a[0]);
    n++;
    //RASSERT(nL == C_1K-1);
  }
  oTimer.vUpdate();
  vTrace("%s %lld\n"_Stz, b, n);
  vTrace("duration %lldns\n"_Stz,oTimer.iNanoseconds());
}

/*----------------------------------------------------------------------------*/
/* test19 : testing wspConverge_t                                             */
/*----------------------------------------------------------------------------*/
void vTest19(void)
{
  Buffer xBuffer;
  s256 s;
  
  // allot heads of size upto 100 bytes and tails of size upto 30 bytes
  while (true) {
    // allocate from head
    i8 iHeadSize = RndLib::iInteger(1,100);
    i8 iTailSize = RndLib::iInteger(1,30);
    
    if (xBuffer.m_xWspConverge.nFree() < iHeadSize + iTailSize)
      break;
    
    byte* pbHead = xBuffer.m_xWspConverge.ayAllotHead(iHeadSize);
    ASSERT(pbHead);
    
    byte* pbTail = xBuffer.m_xWspConverge.ayAllotTail(iTailSize);
    ASSERT(pbTail);
    
    RndLib::vString(s,iTailSize);
    StrLib::vCopy((char*)pbTail, s.azChar());
    
    vTrace("Head:%p; Tail:%p; iHeadSize: %lld; iTailSize: %lld; Tail: %s\n"_Stz, pbHead, pbTail, iHeadSize, iTailSize, pbTail);
  }
}

/*----------------------------------------------------------------------------*/
/* test20 : row slab test                                                     */
/*----------------------------------------------------------------------------*/
void vTest20(void)
{
  vTrace("This is RsbBuffer Test; sizeof(RsbBuffer) = %lld(B)\n"_Stz,
         sizeof(RsbBuffer));

  RsbBuffer* pRsbBuffer = new RsbBuffer;

  delete pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* test21 : determine the number of int entries which fit into an ent buffer  */
/*----------------------------------------------------------------------------*/
i8 nTest21_intEnts(void)
{
  Ref xRef;
  EntWrk xEntWrk;
  
  i8 nEnts = 0;

  EntBuffer* pEntBufferCalc = new EntBuffer;

  for (;;) {
    
    i8 nOffsets = AM::nRndSkipLevel();
    
    //vTrace("%lld "_Stz,iInt);
    xEntWrk.vAppendInt(nEnts);
    xEntWrk.vOffsets(nOffsets);
    
    i8 nBytesNeeded = xEntWrk.nSize_EntKeyNeo();
    if (! pEntBufferCalc->bHasSpace(nBytesNeeded))
      break;
    
    Ent* aEnt = pEntBufferCalc->aInsertEntAbs(xEntWrk.aEnt());
    nEnts++;
    
    xEntWrk.vReset();

  }
  
  delete pEntBufferCalc;
  
  return nEnts;
}

/*----------------------------------------------------------------------------*/
/* test21 : create an int set for key samples                                 */
/*----------------------------------------------------------------------------*/
IntSet* pTest21_keySamples(i8 nSamples)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first calculate how many int entries fit into an EntBuffer
  
  i8 nEntsWhichFit = C_UNKNOWN == nSamples ? nTest21_intEnts() : nSamples;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create that many keys

  IntSet* pKeySamples = new IntSet(nEntsWhichFit);
  vTrace("KeySamples of %lld ints\n"_Stz,pKeySamples->nInts());
  
  return pKeySamples;
}

/*----------------------------------------------------------------------------*/
/* test21 : print test results                                                */
/*----------------------------------------------------------------------------*/
void vTest21_printResults(Stz* szTestFn, IntSet* pKeySamples,
                         AggTimer* pAggTimer, i8 nTotalEnts)
{
  SP_Bytes spBytes;
  SP_Timer spTimer1;
  SP_Timer spTimer2;

  i8 nElapsedNsec = pAggTimer->iNanoseconds();
  d8 dRatePerSec  = pAggTimer->dRatePerSec(nTotalEnts);

  i8 nBytesPerDay = (i8)(dRatePerSec * C_SECS_PER_DAY * 128 * 48);
  
  vTrace("\n"_Stz);
  vTrace("%s of %lld %s entries took %s "
         ": %s/ent : %6.2f ent/s "
         ": %4.2fs/1M "
         ": %4.2fs for SF100 LineItem over 48ptn "
         ": %s per day"
         "\n"_Stz,
         
         szTestFn,
         nTotalEnts,
         pKeySamples->szDesc(),
         spTimer1.sz(nElapsedNsec),
         
         spTimer2.sz(nElapsedNsec / nTotalEnts),
         dRatePerSec,
         
         (d8)C_1M / dRatePerSec,
         
         (d8)(C_TPCH_LINEITEM * 100) / dRatePerSec / 48,
         
         spBytes.sz(nBytesPerDay)
         
         );
}

/*----------------------------------------------------------------------------*/
/* test21 : delete entries from given EntBuffer using KeySamples              */
/*----------------------------------------------------------------------------*/
void vTest21_delete(EntBuffer* pEntBuffer,IntSet* pKeySamples, i8& nTotalDel_)
{
  EntWrk xEntWrk;

  i8 nInts = pKeySamples->nInts();
  
  for (i8 i_Int = 0; i_Int < nInts; i_Int++) {
          
    i8 iInt = pKeySamples->iGet(i_Int);
        
    // key
    xEntWrk.vAppendInt(iInt);
        
    Ent* aPriorEnt = pEntBuffer->aPositionPriorEntry(xEntWrk.aKey());
    RASSERT(aPriorEnt);
    
    Ent* aDeleteEnt = pEntBuffer->aNextEntry(aPriorEnt,0);
    
    if (aDeleteEnt) {
      bool b = Key::bEqualTo(aDeleteEnt->aKey(),xEntWrk.aKey());
      
      if (b) {
        pEntBuffer->vDeleteEntry(&aDeleteEnt);
        nTotalDel_++;
      }
    }

    xEntWrk.vReset();
  }

}

/*----------------------------------------------------------------------------*/
/* test21 : insert entries into EntBuffer using KeySamples                    */
/*----------------------------------------------------------------------------*/
void vTest21_insert(EntBuffer* pEntBuffer,IntSet* pKeySamples, i8& nTotalIns_)
{
  Ref xRef;
  EntWrk xEntWrk;

  i8 nInts = pKeySamples->nInts();
  
  for (i8 i_Int = 0; i_Int < nInts; i_Int++) {
          
    i8 iInt = pKeySamples->iGet(i_Int);
    
    // ref
    xRef.vBuffer(0);
    xRef.vItem(iInt);
    xEntWrk.vRef(xRef);
    
    // key
    xEntWrk.vAppendInt(iInt);
    
    // neo
    i8 nOffsets = AM::nRndSkipLevel();
    xEntWrk.vOffsets(nOffsets);
    
    i8 nBytesNeeded = xEntWrk.nSize_EntKeyNeo();
    if (! pEntBuffer->bHasSpace(nBytesNeeded))
      break;
    
    Ent* aEnt = pEntBuffer->aInsertEntAbs(xEntWrk.aEnt());
    nTotalIns_++;
    
    xEntWrk.vReset();
  }

}

/*----------------------------------------------------------------------------*/
/* test21 : search entries in EntBuffer using KeySamples                      */
/*----------------------------------------------------------------------------*/
void vTest21_search(EntBuffer* pEntBuffer,IntSet* pKeySamples, i8& nTotalDel_)
{
  EntWrk xEntWrk;

  i8 nInts = pKeySamples->nInts();
  
  for (i8 i_Int = 0; i_Int < nInts; i_Int++) {
          
    i8 iInt = pKeySamples->iGet(i_Int);
        
    // key
    xEntWrk.vAppendInt(iInt);
        
    Ent* aPriorEnt = pEntBuffer->aPositionPriorEntry(xEntWrk.aKey());
    RASSERT(aPriorEnt);
    
    Ent* aSearchedEnt = pEntBuffer->aNextEntry(aPriorEnt,0);
    
    if (aSearchedEnt) {
      bool b = Key::bEqualTo(aSearchedEnt->aKey(),xEntWrk.aKey());
    }
    //RASSERT(aSearchedEnt);

    nTotalDel_++;
    
    xEntWrk.vReset();
  }

}

/*----------------------------------------------------------------------------*/
/* test21 : run delete test                                                   */
/*----------------------------------------------------------------------------*/
void vTest21_testDelete(IntSet* pKeySamples, i8 nBuffers, bool bDump)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a number of EntBuffers using the key samples
  
  i8 nTotalDel = 0;
  
  Timer    xTimer;
  AggTimer xAggTimer;

  
  for (i8 i_Buf = 0; i_Buf < nBuffers; i_Buf++) {
    EntBuffer* pEntBuffer = new EntBuffer;
    vTest21_insert(pEntBuffer,pKeySamples,nTotalDel);
    
    xTimer.vStart();
    
    vTest21_delete(pEntBuffer,pKeySamples,nTotalDel);
    if (bDump) {
      bDump = false;
      pEntBuffer->vDump();
    }

    xTimer.vUpdate();
    xAggTimer.vAdd(&xTimer);

    delete pEntBuffer;
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // print timing results
  
  vTest21_printResults("vDeleteEntry()"_Stz,pKeySamples,&xAggTimer,nTotalDel);
}

/*----------------------------------------------------------------------------*/
/* test21 : run insert test                                                   */
/*----------------------------------------------------------------------------*/
void vTest21_testInsert(IntSet* pKeySamples, i8 nBuffers, bool bDump)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a number of EntBuffers using the key samples
  
  i8 nTotalIns = 0;
  
  Timer    xTimer;
  AggTimer xAggTimer;

  xTimer.vStart();
  
  for (i8 i_Buf = 0; i_Buf < nBuffers; i_Buf++) {
    EntBuffer* pEntBuffer = new EntBuffer;
    vTest21_insert(pEntBuffer,pKeySamples,nTotalIns);
    if (bDump) {
      bDump = false;
      pEntBuffer->vDump();
    }
    delete pEntBuffer;
  }

  xTimer.vUpdate();

  xAggTimer.vAdd(&xTimer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // print timing results
  
  vTest21_printResults("aInsertEntAbs()"_Stz,pKeySamples,&xAggTimer,nTotalIns);
}

/*----------------------------------------------------------------------------*/
/* test21 : run search test                                                   */
/*----------------------------------------------------------------------------*/
void vTest21_testSearch(IntSet* pKeySamples, i8 nBuffers, bool bDump)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a number of EntBuffers using the key samples
  
  i8 nTotalIns = 0;
  i8 nTotalSel = 0;

  Timer    xTimer;
  AggTimer xAggTimer;

  
  for (i8 i_Buf = 0; i_Buf < nBuffers; i_Buf++) {
    EntBuffer* pEntBuffer = new EntBuffer;
    vTest21_insert(pEntBuffer,pKeySamples,nTotalIns);
    
    xTimer.vStart();
    
    vTest21_search(pEntBuffer,pKeySamples,nTotalSel);

    xTimer.vUpdate();
    xAggTimer.vAdd(&xTimer);

    delete pEntBuffer;
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // print timing results
  
  vTest21_printResults("aPositionPriorEntry()"_Stz,pKeySamples,&xAggTimer,nTotalSel);
}

/*----------------------------------------------------------------------------*/
/* test21 : EntBuffer Test -- tune the insert of ent into skiplist of 1 buffer*/
/*----------------------------------------------------------------------------*/
void vTest21(void)
{
  i8 nSamples = C_UNKNOWN;
  bool bDump  = false;
  
  bool bInsertTest = true;
  bool bSearchTest = true;
  bool bDeleteTest = false;

  vTrace("Test21 -- This is EntBuffer Test MODE: %s %s\n"_Stz,
         AM::szBuild(),
         AM::bDeveloper() ? "DEVELOPER"_Stz : ""_Stz
         );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create key sample int set

  IntSet* pKeySamples = pTest21_keySamples(nSamples);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // run our tests
  
  pKeySamples->vAscending();
  if (bInsertTest) vTest21_testInsert(pKeySamples,10,bDump);
  if (bSearchTest) vTest21_testSearch(pKeySamples,10,bDump);
  if (bDeleteTest) vTest21_testDelete(pKeySamples,10,bDump);

  pKeySamples->vDescending();
  if (bInsertTest) vTest21_testInsert(pKeySamples,10,bDump);
  if (bSearchTest) vTest21_testSearch(pKeySamples,10,bDump);
  if (bDeleteTest) vTest21_testDelete(pKeySamples,10,bDump);

  pKeySamples->vRandomise();
  if (bInsertTest) vTest21_testInsert(pKeySamples,10,bDump);
  if (bSearchTest) vTest21_testSearch(pKeySamples,10,bDump);
  if (bDeleteTest) vTest21_testDelete(pKeySamples,10,bDump);
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
  IntSet* pIntSet = new IntSet(64);

  pIntSet->vAscending();          pIntSet->vPrint(false);
  pIntSet->vDescending();         pIntSet->vPrint(false);
  pIntSet->vSort();               pIntSet->vPrint(false);
  pIntSet->vSortDescend();        pIntSet->vPrint(false);

  pIntSet->vRandoms(0,16);        pIntSet->vPrint(false);
  pIntSet->vSort();               pIntSet->vPrint(false);
  pIntSet->vSortDescend();        pIntSet->vPrint(false);

  pIntSet->vRandomsUnique(0,64);  pIntSet->vPrint(false);
  pIntSet->vSort();               pIntSet->vPrint(false);
  pIntSet->vSortDescend();        pIntSet->vPrint(false);

  pIntSet->vRandomsUnique(0,128); pIntSet->vPrint(false);
  pIntSet->vSort();               pIntSet->vPrint(false);
  pIntSet->vSortDescend();        pIntSet->vPrint(false);

  pIntSet->vRandoms(0,128);       pIntSet->vPrint(false);
  pIntSet->vSort();               pIntSet->vPrint(false);
  pIntSet->vSortDescend();        pIntSet->vPrint(false);
}

/*----------------------------------------------------------------------------*/
/* test26                                                                     */
/*----------------------------------------------------------------------------*/
void vTest26(void)
{
  bool bDumpTable = false;
  i8 nSamples = C_32KB;

  i8 i_NextRnd = C_UNKNOWN;
  
  static const i8 C_FRQ = 8;
  static const i8 C_LVL = 8;
  static const i8 C_CNT = 8;

  i8 iaCounts[C_LVL][C_FRQ][C_CNT];
  
  for (i8 i_Lvl = 0; i_Lvl < C_LVL; i_Lvl++)
    for (i8 i_Frq = 0; i_Frq < C_FRQ; i_Frq++)
      for (i8 i_Cnt = 0; i_Cnt < C_CNT; i_Cnt++)
        iaCounts[i_Lvl][i_Frq][i_Cnt] = 0;


  if (bDumpTable) {
    vTrace("L     3           4           5\n"_Stz);
    vTrace("F 1 2 3 4 5   1 2 3 4 5   1 2 3 4 5\n"_Stz);
    vTrace("  - - - - -   - - - - -   - - - - -\n"_Stz);
  }
  
  for (i8 i_Sample = 0; i_Sample < nSamples; i_Sample++) {
    if (bDumpTable)
      vTrace("  "_Stz);

    for (i8 i_Lvl = 3; i_Lvl < C_LVL; i_Lvl++) {

      if (bDumpTable)
        if (i_Lvl > 3)
          vTrace(": "_Stz);

      for (i8 i_Frq = 2; i_Frq < C_FRQ; i_Frq++) {

        //i8 iLvl = RndLib::iLogLevel(nLevels,iFreq,i_NextRnd);
        i8 i_Cnt = RndLib::iSkipLevel(i_Lvl,i_Frq,i_NextRnd) ;
        if (bDumpTable)
          vTrace("%lld "_Stz,i_Cnt);

        iaCounts[i_Lvl][i_Frq][i_Cnt]++;
        
      } // freq
      
    } // level
    if (bDumpTable)
      vTrace("\n"_Stz);
    

  } // sample
  
  i8 iXchk = 0;
  vTrace("\n"_Stz);
  for (i8 i_Lvl = 3; i_Lvl < C_LVL; i_Lvl++) {
    vTrace("%lld Levels\n"_Stz,i_Lvl);

    for (i8 i_Frq = 2; i_Frq < C_FRQ; i_Frq++) {
      vTrace("Freq %lld: "_Stz,i_Frq);

      iXchk = 0;
      for (i8 i_Cnt = 1; i_Cnt < C_CNT; i_Cnt++) {
        if (i_Cnt == 1)
          vTrace("%lld "_Stz,iaCounts[i_Lvl][i_Frq][i_Cnt]);
        else {
          i8 iRatio = iaCounts[i_Lvl][i_Frq][i_Cnt]
                        ? iaCounts[i_Lvl][i_Frq][i_Cnt - 1] /
                          iaCounts[i_Lvl][i_Frq][i_Cnt]
                        : 0;
                      
          vTrace("/%lld=%lld "_Stz,iRatio,iaCounts[i_Lvl][i_Frq][i_Cnt]);
        }
        iXchk += iaCounts[i_Lvl][i_Frq][i_Cnt];
      }
      vTrace(" (iXchk=%lld)\n"_Stz,iXchk);
    }
    vTrace("\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* test27 : SP_bits test (along with using IntLib::iBytePower() striding      */
/*----------------------------------------------------------------------------*/
void vTest27(void)
{
  u8 i;
  i8 iStart;
  i8 iStride;
  
  i8 iLoop;

  i8 iTimes;

  SP_Bits spBits;
  
  iTimes = 16;
    
  for (i8 iPower = 0; iPower < (i8)C_BYTES_PER_WORD; iPower += 1) {
    
    iStart  = IntLib::iBytePower(iPower);
    iStride = iStart ? iStart : 1;
    
    i = iStart;

    for (iLoop = 0; iLoop < iTimes; iLoop++, i += iStride)
      vTrace("[%02lld] %20llu %s %016llx Power=%lld Start=%llx Stride=%llx\n"_Stz,
             iLoop,i,spBits.sz(i),i,iPower,iStart,iStride);
    vTrace("...\n"_Stz);
    
    iStride = iStride * 16;
    i = iStride;
    for (iLoop = 0; iLoop < iTimes - 1; iLoop++, i += iStride)
      vTrace("[%02lld] %20llu %s %016llx Power=%lld Start=%llx Stride=%llx\n"_Stz,
             iLoop,i,spBits.sz(i),i,iPower,iStart,iStride);
    vTrace("\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* test28 -- visualise random levels                                          */
/*----------------------------------------------------------------------------*/
void vTest28(void)
{
  i8 i_NextRnd = C_UNKNOWN;
  
  i8 iTimes = C_2KB;
  i8 iLevel;
  
  for (i8 i = 0; i < iTimes; i++) {
    iLevel = RndLib::iLogLevel(C_DEF_LVL_MAXLEVELS,C_DEF_LVL_FREQUENCY,i_NextRnd);
    //vTrace("[%02lld] %lld "_Stz,i,iLevel);
    if (i && ! (i % 64))
      vTrace("\n"_Stz);
    if (3 == iLevel)
      vTrace("$ "_Stz);
    else
    if (1 == iLevel)
      vTrace("  "_Stz);
    else
      vTrace(". "_Stz);

  }
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* test29                                                                     */
/*----------------------------------------------------------------------------*/
void vTest29(void)
{
  i8 iTimes = 32;
  
  i8 i_NextRnd = C_UNKNOWN;

  for (i8 i = 0; i < iTimes; i++) {
    i8 iLevel = RndLib::iSkipLevel(3,3,i_NextRnd);
    vTrace("Level = %lld\n"_Stz,iLevel);
  }

}

/*----------------------------------------------------------------------------*/
/* test30 : observe generation of random bit patterns                         */
/*----------------------------------------------------------------------------*/
void vTest30(void)
{
 i8 iTimes = 32;
 u8 uPattern;
 
  SP_Bits spBits;

 for (i8 i = 0; i < iTimes; i++) {
   uPattern = RndLib::uInteger();
   vTrace("[%02lld] %lls\n"_Stz,i,spBits.sz(uPattern));
 }
}

/*----------------------------------------------------------------------------*/
/* test31                                                                     */
/*----------------------------------------------------------------------------*/
void vTest31(void)
{
}

/*----------------------------------------------------------------------------*/
/* test32                                                                     */
/*----------------------------------------------------------------------------*/
void vTest32(void)
{
}

/*----------------------------------------------------------------------------*/
/* test33                                                                     */
/*----------------------------------------------------------------------------*/
void vTest33(void)
{
}

/*----------------------------------------------------------------------------*/
/* test34                                                                     */
/*----------------------------------------------------------------------------*/
void vTest34(void)
{
}

/*----------------------------------------------------------------------------*/
/* test35                                                                     */
/*----------------------------------------------------------------------------*/
void vTest35(void)
{
}

/*----------------------------------------------------------------------------*/
/* test36                                                                     */
/*----------------------------------------------------------------------------*/
void vTest36(void)
{
}

/*----------------------------------------------------------------------------*/
/* test37                                                                     */
/*----------------------------------------------------------------------------*/
void vTest37(void)
{
}

/*----------------------------------------------------------------------------*/
/* test38                                                                     */
/*----------------------------------------------------------------------------*/
void vTest38(void)
{
}

/*----------------------------------------------------------------------------*/
/* test39                                                                     */
/*----------------------------------------------------------------------------*/
void vTest39(void)
{
}

/*----------------------------------------------------------------------------*/
/* test40                                                                     */
/*----------------------------------------------------------------------------*/
void vTest40(void)
{
}

/*----------------------------------------------------------------------------*/
/* test41                                                                     */
/*----------------------------------------------------------------------------*/
void vTest41(void)
{
}

/*----------------------------------------------------------------------------*/
/* test42                                                                     */
/*----------------------------------------------------------------------------*/
void vTest42(void)
{
}

/*----------------------------------------------------------------------------*/
/* test43                                                                     */
/*----------------------------------------------------------------------------*/
void vTest43(void)
{
}

/*----------------------------------------------------------------------------*/
/* test44                                                                     */
/*----------------------------------------------------------------------------*/
void vTest44(void)
{
}

/*----------------------------------------------------------------------------*/
/* test45                                                                     */
/*----------------------------------------------------------------------------*/
void vTest45(void)
{
}

/*----------------------------------------------------------------------------*/
/* test46                                                                     */
/*----------------------------------------------------------------------------*/
void vTest46(void)
{
}

/*----------------------------------------------------------------------------*/
/* test47                                                                     */
/*----------------------------------------------------------------------------*/
void vTest47(void)
{
}

/*----------------------------------------------------------------------------*/
/* test48                                                                     */
/*----------------------------------------------------------------------------*/
void vTest48(void)
{
}

/*----------------------------------------------------------------------------*/
/* test49                                                                     */
/*----------------------------------------------------------------------------*/
void vTest49(void)
{
}

/*----------------------------------------------------------------------------*/
/* test50                                                                     */
/*----------------------------------------------------------------------------*/
void vTest50(void)
{
}

/*----------------------------------------------------------------------------*/
/* test51                                                                     */
/*----------------------------------------------------------------------------*/
void vTest51(void)
{
}

/*----------------------------------------------------------------------------*/
/* test52                                                                     */
/*----------------------------------------------------------------------------*/
void vTest52(void)
{
}

/*----------------------------------------------------------------------------*/
/* test53                                                                     */
/*----------------------------------------------------------------------------*/
void vTest53(void)
{
}

/*----------------------------------------------------------------------------*/
/* test54                                                                     */
/*----------------------------------------------------------------------------*/
void vTest54(void)
{
}

/*----------------------------------------------------------------------------*/
/* test55                                                                     */
/*----------------------------------------------------------------------------*/
void vTest55(void)
{
}

/*----------------------------------------------------------------------------*/
/* test56                                                                     */
/*----------------------------------------------------------------------------*/
void vTest56(void)
{
}

/*----------------------------------------------------------------------------*/
/* test57                                                                     */
/*----------------------------------------------------------------------------*/
void vTest57(void)
{
}

/*----------------------------------------------------------------------------*/
/* test58                                                                     */
/*----------------------------------------------------------------------------*/
void vTest58(void)
{
}

/*----------------------------------------------------------------------------*/
/* test59                                                                     */
/*----------------------------------------------------------------------------*/
void vTest59(void)
{
}

/*----------------------------------------------------------------------------*/
/* test60                                                                     */
/*----------------------------------------------------------------------------*/
void vTest60(void)
{
}

/*----------------------------------------------------------------------------*/
/* test61                                                                     */
/*----------------------------------------------------------------------------*/
void vTest61(void)
{
}

/*----------------------------------------------------------------------------*/
/* test62                                                                     */
/*----------------------------------------------------------------------------*/
void vTest62(void)
{
}

/*----------------------------------------------------------------------------*/
/* test63                                                                     */
/*----------------------------------------------------------------------------*/
void vTest63(void)
{
}

/*----------------------------------------------------------------------------*/
/* test64                                                                     */
/*----------------------------------------------------------------------------*/
void vTest64(void)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
