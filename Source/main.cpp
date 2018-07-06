/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"
#include "ShmUtility.hpp"

extern AttachMap* gpAttachMap;
extern Arguments  gxArguments;

extern bool gbSHM;

void vTest1(void);
void vTest2(void);
void vTest3(void);
void vTest4(void);
void vTest5(void);
void vTest6(void);
void vTest7(void);
void vTest8(void);
void vTest9(void);

void vChangeDirectory(void);
void vKillPrev(void);
void vPrintSizes(void);
void vPrintWelcome(void);
void vRunPrompt(void);
void vSetGlobals(void);
void vWriteRegionName(void);

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, const char* argv[])
{
  if ( ! gxArguments.bParse(argc,(Stz**)argv) )
    exit(EXIT_FAILURE);
  
  if (gbSHM) {
    ShmUtility::vRun(argc,(Stz**)argv);
    exit(EXIT_SUCCESS);
  }
  
  vPrintWelcome();
  
  if (gxArguments.m_bWelcome && gxArguments.m_bStart)
    gxArguments.vPrintStartupInfo();
  
  //OpSys::vGetInfo();
 // OpSys::vTraceInfo();
  
  if (gxArguments.m_bVerbose)
    vPrintSizes();
  
  vSetSignalHandlers();
/*
  vTrace("sizeof(short) %lld\n"_Stz,(i8)sizeof(short));
  vTrace("sizeof(int) %lld\n"_Stz,(i8)sizeof(int));
  vTrace("sizeof(long) %lld\n"_Stz,(i8)sizeof(long));
  vTrace("sizeof(long long) %lld\n"_Stz,(i8)sizeof(long long));
  
  vTrace("sizeof(i1) %lld\n"_Stz,(i8)sizeof(i1));
  vTrace("sizeof(i2) %lld\n"_Stz,(i8)sizeof(i2));
  vTrace("sizeof(i3) %lld\n"_Stz,(i8)sizeof(i3));
  vTrace("sizeof(i4) %lld\n"_Stz,(i8)sizeof(i4));
  vTrace("sizeof(i8) %lld\n"_Stz,(i8)sizeof(i8));
  
  vTrace("sizeof(Object) %lld\n"_Stz,(i8)sizeof(Object));
  vTrace("sizeof(Object2) %lld\n"_Stz,(i8)sizeof(Object2));
  vTrace("sizeof(eCN) %lld\n"_Stz,(i8)sizeof(eCN));
  
  vTrace("sizeof(Item) %lld\n"_Stz,(i8)sizeof(Item));
  vTrace("sizeof(Entry) %lld\n"_Stz,(i8)sizeof(Entry));
  
  vTrace("alignof(i3) %lld\n"_Stz,(i8)alignof(i3));
  vTrace("alignof(Entry) %lld\n"_Stz,(i8)alignof(Entry));
  */
  
  //vTest1();
  //vTest2();
  //vTest3();
  //vTest4();
  //vTest5();
  //vTest6();
  //vTest7();
  //vTest8();
  //vTest9();

  StorageEngine::vCheckAllAssumptions();
  
  vWriteRegionName();
  vChangeDirectory();
  vKillPrev();
  
  RapidsSE::vPoolOpen();
  
  if (gxArguments.m_bStart)
     vSetGlobals();
  
  RapidsSE::vCheckPort();
  
  if (gxArguments.m_bPrompt && !gxArguments.m_bStart)
    gpAttachMap->vPrintStartupInfo();
  
  RapidsSE::vExecuteCmdLine();

  //gpAttachMap->vPrintAttachMap();
  
  //vSkipListTest();
  
//  ResultBuffer* pRB = new ResultBuffer;
//  delete pRB;
  
  vRunPrompt();
  
  RapidsSE::vPoolClose();

  if (gxArguments.m_bVerbose)
    vInfo("RapidsSE exits\n"_Stz);
    
  return EXIT_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* change current directory                                                   */
/*----------------------------------------------------------------------------*/
void vChangeDirectory(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Change Working Directory\n*\n"_Stz);
  
  // change working directory to commands dir
  Dir xDir;
  s256 sCurrent;
  bool bResult = xDir.bChangeCurrent(gxArguments.m_sCommands);
  if (xDir.bGetCurrent(sCurrent) && gxArguments.m_bVerbose)
    vInfo("CURRENT WORKING DIRECTORY: %s\n"_Stz,sCurrent.aStr());
}

/*----------------------------------------------------------------------------*/
/* kill previous node instances                                               */
/*----------------------------------------------------------------------------*/
void vKillPrev(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Kill Previous Occurences\n*\n"_Stz);
  
  if ( ! gxArguments.m_bKillPrev)
    return;
  
  // an assumption is that regions will be /SE_<node>
  
  void* pRegion;
  s256 sRegionName;
  int iFD;
  bool b;
  
  for (i8 i_Index=0; i_Index < gxArguments.m_nNodes; i_Index++) {
    
    sRegionName.vSprint("%s_%s"_Stz,
                        gxArguments.m_sRegionPrefix.aChar(),
                        gxArguments.m_xNodes[i_Index].m_sNodeName.aChar());
    
    if ( ! Region::bTest(sRegionName) )
      continue;
    
    if ( ! (pRegion = Region::pOpen(sRegionName,iFD)) )
      continue;
    
    AttachMap* pAttachMap = pAdd<AttachMap*>(pRegion,C_SIZE_POOL_BUFFER);
    
    if (pAttachMap->m_iPid &&
        pAttachMap->m_iPid != Process::iGetPid() &&
        Process::bTestPid(pAttachMap->m_iPid)) {
      
      b = Process::bKillPid(pAttachMap->m_iPid);
      if ( b )
        vTrace("vKillPrev: process %lld killed\n"_Stz,pAttachMap->m_iPid);
      else
        vTrace("vKillPrev: process %lld not killed\n"_Stz,pAttachMap->m_iPid);
    }
    
    Region::vClose(pRegion,iFD);
    
    Region::vUnlink(sRegionName);
  }
}

/*----------------------------------------------------------------------------*/
/* print size of things                                                       */
/*----------------------------------------------------------------------------*/
void vPrintSizes(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Sizes\n*\n"_Stz);
  
  /*
   vInfo(
   "sizeof(eCN) %lu; sizeof(bool) %lu; \n"_Stz
   "sizeof(Object) %lu; sizeof(StrVal) %lu; \n"_Stz
   "sizeof(s32) %lu; sizeof(s256) %lu; \n"_Stz
   "sizeof(Value) %lu; sizeof(IntVal) %lu; sizeof(ValWrk) %lu \n"_Stz
   "\n"_Stz,
   sizeof(eCN),sizeof(bool),
   sizeof(Object),sizeof(StrVal),
   sizeof(s32),sizeof(s256),
   sizeof(Value),sizeof(IntVal),sizeof(ValWrk)
   );
   */
  
  vInfo("sizeof(Object) %lu; \n"_Stz, sizeof(Object));
  assert(sizeof(Object) == 8);
  
}

/*----------------------------------------------------------------------------*/
/* print welcome                                                              */
/*----------------------------------------------------------------------------*/
void vPrintWelcome(void)
{
  Stz* aRelDbg;
#ifdef _DEBUG
  aRelDbg = "debug"_Stz;
#else
  aRelDbg = "release"_Stz;
#endif
  
  if (gxArguments.m_bWelcome && ! gxArguments.m_bStatus)
    vInfo("RapidsSE v%s (%s %s -- %s) pid %lld\n"_Stz,
          RAPIDSSE_VERSION_STRING,
          __DATE__,
          __TIME__,
          aRelDbg,
          Process::iGetPid()
          );
}

/*----------------------------------------------------------------------------*/
/* run se prompt                                                              */
/*----------------------------------------------------------------------------*/
void vRunPrompt(void)
{
  // optionally execute se> prompt
  if (gxArguments.m_bPrompt || gxArguments.m_bScript) {

    if (gxArguments.m_bVerbose)
      vInfo("\n*\n* Run Prompt\n*\n"_Stz);
    
    // if only prompt then cwd to Commands
    // (with --start sen changes cwd)
    //    if (!gxArguments.m_bStart) {
    //      // change working directory to commands dir
    //      bool bResult = xDir.bSetCurrent(gxArguments.m_sCommands);
    //      // ignore error
    
    //     if (xDir.bGetCurrent(sCurrent))
    //       vInfo("dir: %s\n"_Stz,sCurrent.aStr());
    //    }
    
    // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    // start prompt R35
    
    i8 iTraceLevel = 0;
    eEC eError = RapidsSE_Prompt(iTraceLevel);
    ASSERT(EC_NONE == eError);
    vTrace("RapidsSE_Prompt() called\n"_Stz);

    // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

    SePrompt* pSePrompt = new SePrompt();
    pSePrompt->vLoop();
    delete pSePrompt;
    
    return;
  }
  
  if (gxArguments.m_bPromptR35) {
    vTrace("\n\n se R35> \n"_Stz);
    exit(0);
  }
}

/*----------------------------------------------------------------------------*/
/* set globals                                                                */
/*----------------------------------------------------------------------------*/
void vSetGlobals(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Set Globals From Arguments\n*\n"_Stz);
  
  // set globals from gxArguments
  gpAttachMap->m_bVerbose      = gxArguments.m_bVerbose;
  gpAttachMap->m_bTracepoint   = gxArguments.m_bTracepoint;
  gpAttachMap->m_bProgress     = gxArguments.m_bProgress;
  gpAttachMap->m_bMetadata     = gxArguments.m_bMetadata;
  gpAttachMap->m_nIndexEntries = gxArguments.m_nEntries;
  gpAttachMap->m_nSplitRatio   = gxArguments.m_nSplitRatio;
  gpAttachMap->m_nMaxQueue     = gxArguments.m_nMaxQueue;
}

/*----------------------------------------------------------------------------*/
/* write region name                                                          */
/*----------------------------------------------------------------------------*/
void vWriteRegionName(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Write Region Name\n*\n"_Stz);
  
  // write the regionName into a se.region file
  // used by shell script to start se prompt
  if (gxArguments.m_bStart) {
    File xFile;
    xFile.vPath("./se.region"_Stz);
    bool b = xFile.bOpen(C_OPEN_READ_WRITE_CREATE);
    ASSERT(b);
    xFile.vPrint("%s\n"_Stz,gxArguments.m_sRegionName.aChar());
    xFile.vClose();
  }
  
  if (gxArguments.m_bVerbose)
    vInfo("REGION NAME: %s\n"_Stz,gxArguments.m_sRegionName.aChar());
}

/*----------------------------------------------------------------------------*/
/* -- Tests ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test1                                                                      */
/*----------------------------------------------------------------------------*/
void vTest1(void)
{
  File xFile;
  i8 iChunk = C_32KB;
  char buffer[iChunk];
  Timer xTimer;
  
  vTrace("Reading lineitem.tbl in chunks of %lld\n"_Stz, iChunk);
  xFile.vPath("/Volumes/Data/Users/alkayamarti/tpch_SF10/lineitem.tbl"_Stz);
  bool b = xFile.bOpen(C_OPEN_READ);
  ASSERT(b);
  
  xTimer.vStart();
  i8 iBytesRead = xFile.iReadChunk(buffer,iChunk);
  
  while (iBytesRead) {
    iBytesRead = xFile.iReadChunk(buffer,iChunk);
  }
  xTimer.vUpdate();
  vTrace("duration %lldms\n"_Stz,xTimer.iMilliseconds());
  
  xFile.vClose();
}

//  i8 i = 0;
//  Synch::uAtomicInc(&i);
//  vInfo("AtomicInc i = %lld\n"_Stz,i);
//  Synch::uAtomicDec(&i);
//  vInfo("AtomicDec i = %lld\n"_Stz,i);
//
//  u8 u = 2;
//  bool b = Synch::bTestAndSet(&u,1,3);
//  vInfo("TestAndSet %s u = %lld\n"_Stz,b ? "true"_Stz : "false"_Stz,u);


//  Timer xTimer;
//  xTimer.vStart();
//  PosixThread::vSleepMicro(500);
//  xTimer.vUpdate();
//
//  vInfo("Timert %lld\n"_Stz,xTimer.iMicroseconds());

/*----------------------------------------------------------------------------*/
/* test2                                                                      */
/*----------------------------------------------------------------------------*/
void vTest2(void)
{
  vTrace("-------------------------vTest2--------------------------\n"_Stz);

  Ivl xIvl1;
  Ivl xIvl2;
  
  xIvl1.vSet(0,0);
  xIvl2.vSet(1,0);
  assert(xIvl1 < xIvl2);
  assert( ! (xIvl2 < xIvl1) );

  xIvl1.vSet(1,0);
  xIvl2.vSet(1,1);
  assert(xIvl1 < xIvl2);
  assert( ! (xIvl2 < xIvl1) );
  
  
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
  i8* aArray = (i8*)malloc(8 * nINTS);
  
  for (i8 i_Int = 0; i_Int < nINTS; i_Int++)
    aArray[i_Int] = i_Int;
  
  Timer xTimer;
  xTimer.vStart();
  
  i8 iSum = 0;
  for (i8 i_Times = 0; i_Times < 1000; i_Times++)
    for (i8 i_Int = 0; i_Int < nINTS; i_Int++)
      iSum += aArray[i_Int];

  xTimer.vUpdate();
  vTrace("duration %lldms %lld\n"_Stz,xTimer.iMilliseconds(),iSum);
}

/*----------------------------------------------------------------------------*/
/* test6                                                                      */
/*----------------------------------------------------------------------------*/
void vTest6(void)
{
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  /*
   i8 iLevels[gxArguments.m_iLevels];
   for (i8 i_Level = 0; i_Level < gxArguments.m_iLevels; i_Level++)
   iLevels[i_Level] = 0;
   
   for (i8 i = 0; i < 5; i++) {
   u8 iLevel = Random::iLogLevel(gxArguments.m_iLevels, gxArguments.m_iFrequency);
   iLevels[iLevel]++;
   }
   for (i8 u_Level = 0; u_Level < gxArguments.m_uLevels; u_Level++)
   printf("%llu: %llu\n",u_Level,iLevels[uLevel]);
   */
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

/*----------------------------------------------------------------------------*/
/* test7                                                                      */
/*----------------------------------------------------------------------------*/
void vTest7(void)
{
  Dir xDir;
  s256 sCurrent;
  //  if (xDir.bGetCurrent(sCurrent))
  //    vInfo("dir: %s\n",sCurrent.aChar());
  
  
  //  vTrace("Task::vWord %p\n"_Stz,Task::vWord);
  
  //  Timer xTimer;
  //
  //  xTimer.vStart();
  //
  //  PosixThread::vSleepMilli(10);
  //
  //  xTimer.vUpdate();
  //
  //  vTrace("duration %lldns\n"_Stz,xTimer.iNanoseconds());
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
void vTest9(void)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
