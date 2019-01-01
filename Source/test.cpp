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
  char buffer[iChunk];
  Timer oTimer;
  
  vTrace("Reading lineitem.tbl in chunks of %lld\n"_Stz, iChunk);
  bool b = oFile.bOpen("/Volumes/Data/Users/alkayamarti/tpch_SF10/lineitem.tbl"_Stz,C_OPEN_READ);
  ASSERT(b);
  
  oTimer.vStart();
  i8 iBytesRead = oFile.iReadBytes(buffer,iChunk);
  
  while (iBytesRead) {
    iBytesRead = oFile.iReadBytes(buffer,iChunk);
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
  PosixThread::vSleepMilli(3500);
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
  
  i8 nBytes = IntLib::iIntToVlq(C_NULL_INT,xVlq);
  i8 i = IntLib::iVlqToInt(xVlq);
  assert(C_NULL_INT == i);
  
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
  char cBitAry[128];
  
  assert(! BitLib::bAnyOn(0));
  assert(BitLib::bAnyOn(1));
  
  assert(1 == BitLib::uBitMask(0));
  assert(2 == BitLib::uBitMask(1));
  
  BitLib::vDump(0,cBitAry);
  vTrace("BitMap: %s\n"_Stz,cBitAry);
  
  BitLib::vDump(1,cBitAry);
  vTrace("BitMap: %s\n"_Stz,cBitAry);
  
  u8 uBitMap = 0xFEDCBA;
  BitLib::vDump(uBitMap,cBitAry);
  vTrace("BitMap: %s\n"_Stz,cBitAry);
  
  i8 iBit = -1;
  while (BitLib::bEachBit(uBitMap,iBit))
    vTrace("BitOn: %lld\n"_Stz,iBit);

  vTrace("Highest Bit: %lld\n"_Stz,BitLib::iHighestBit(uBitMap));
  
  vTrace("Bit 6 is %s\n"_Stz,BitLib::bIsOn(uBitMap,6) ? "on" : "off");
  vTrace("Bit 23 is %s\n"_Stz,BitLib::bIsOn(uBitMap,23) ? "on" : "off");
  
  
  uBitMap = BitLib::uSetOn(uBitMap,6);
  vTrace("Bit 6 is now %s\n"_Stz,BitLib::bIsOn(uBitMap,6) ? "on" : "off");
  
  uBitMap = BitLib::uShuffleRight(uBitMap,2);
  BitLib::vDump(uBitMap,cBitAry);
  vTrace("BitMap: %s\n"_Stz,cBitAry);
  
  uBitMap = BitLib::uShuffleLeft(uBitMap,8);
  BitLib::vDump(uBitMap,cBitAry);
  vTrace("BitMap: %s\n"_Stz,cBitAry);
  
  uBitMap = BitLib::uShuffleRight(uBitMap,8);
  u8 uCut = BitLib::uCutRight(uBitMap,3);
  BitLib::vDump(uBitMap,cBitAry);
  
  vTrace("BitMap: %s\n"_Stz,cBitAry);
  BitLib::vDump(uCut,cBitAry);
  vTrace("Cut: %s\n"_Stz,cBitAry);
  
 
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
//         PosixThread::aStackBase(),
//         PosixThread::iStackSize(),
//         PosixThread::aStackPointer());
}

/*----------------------------------------------------------------------------*/
/* test7                                                                      */
/*----------------------------------------------------------------------------*/
void vTest7(void)
{
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
//    i8 iElem = Random::iRandom(nElem-1);
//    //vTrace("iElem %lld; iPmc_GetLlcMisses %llx\n"_Stz,iElem,iPmc_GetLlcMisses());
//    iSum += xI8[iElem];
//  }
//  vTrace("Random     iPmc_GetLlcMisses %lld; iSum %lld\n"_Stz,iPmc_GetLlcMisses(),iSum);
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
