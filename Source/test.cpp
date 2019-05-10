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
  vTrace("-------------------------vTest2--------------------------\n"_Stz);

  Since oStart;
  Since oEnd;
  
  oStart.vNow();
  Pthread::vSleepMilli(3500,CN_NONE);
  oEnd.vNow();
  
  //Ivl wDiff = oEnd.wSubtract(oStart);
  //Ivl wDiff = oStart.wSubtract(oEnd);
  //Ivl wDiff = oStart.wSubtract(oStart);
  Ivl wDiff = oEnd.wSubtract(oEnd);

  vTrace("Diff: %llds%lldus\n"_Stz,wDiff.iSecond(),wDiff.iMilli());
  
  vTrace("-----------------------End vTest2------------------------\n"_Stz);
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

  HashMap* pHashMap = new HashMap(false,0,8);
  
  //s32 sKey("ALPHA"_Stz);
  s32 sKey;
  
  for (i8 nDupes = 0; nDupes < 1; nDupes++) {
    for (i8 i_Times = 0; i_Times < 100000; i_Times++) {
      sKey.vSprint("%02lld"_Stz,i_Times);

//      if ( 4206 == i_Times ||
//           4249 == i_Times ||
//          25396 == i_Times ||
//          28848 == i_Times ||
//          40263 == i_Times ||
//          48122 == i_Times)
//        pHashMap->vInsert((byte*)sKey.aChar(),sKey.iLength(),i_Times,true);
//      else
        pHashMap->vInsert((byte*)sKey.aChar(),sKey.iLength(),i_Times,false);
    }
  }
  
//  HashCursor xCursor(pHashMap);
  
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
  
  pHashMap->vAnalysis(false);
  
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
//  printf("Stack base %llx size %lld pointer %llx\n",
//         Pthread::aStackBase(),
//         Pthread::iStackSize(),
//         Pthread::aStackPointer());
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
  RowNub::vCut(&xCacheLine, sizeof(CacheLine), 6, 10);
  
  // dump cacheline after cut
  MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
  
  xCacheLine.vSetAll('a');

  // set bytes to be moved + 2 to 'b'
  xCacheLine.vSet(4, sizeof(CacheLine) - 6, 'b');
  
  // cut last but 6 bytes
  RowNub::vCut(&xCacheLine, sizeof(CacheLine), 6, sizeof(CacheLine) - 6);
  
  // dump cacheline after cut
  MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
  
  xCacheLine.vSetAll('a');
  
  // set bytes to be moved + 2 to 'b'
  xCacheLine.vSet(0, 8, 'b');
  
  // cut frst 6 bytes
  RowNub::vCut(&xCacheLine, sizeof(CacheLine), 0, 6);
  
  // dump cacheline after cut
  MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
