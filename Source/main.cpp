/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"
#include "UnitTests.h"

extern Arguments gxArguments;
extern Grid*     gpGrid;
extern bool      gbVerbose;
extern bool      gbTracepoint;
extern bool      gbProgress;
extern bool      gbMetadata;

void vTest1(void);
void vTest2(void);
void vTest3(void);
void vTest4(void);
void vTest5(void);

void vKillPrev(void);


/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, const char* argv[])
{
  //vTest3();
  //vTest4();
  //vTest5();
  
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
  assert(sizeof(Object) == 16);
  
  vInfo("sizeof(Object_Emb) %lu; \n"_Stz, sizeof(Object_Emb));
  assert(sizeof(Object_Emb) == 2);

  Stz* aRelDbg;
#ifdef _DEBUG
  aRelDbg = "debug"_Stz;
#else
  aRelDbg = "release"_Stz;
#endif
  
  vSetSignalHandlers();
  
  //vTest1();
  //vTest2();
  
  StorageEngine::vCheckAllAssumptions();
  
  if ( ! gxArguments.bParse(argc,(Stz**)argv) )
    exit(EXIT_FAILURE);
  
  if (gxArguments.m_bWelcome && ! gxArguments.m_bStatus)
    vInfo("RapidsSE v%s (%s %s -- %s) pid %lld\n"_Stz,
           RAPIDSSE_VERSION_STRING,
           __DATE__,
           __TIME__,
           aRelDbg,
           Process::iGetPid()
           );

  // set globals from gxArguments
  gbVerbose    = gxArguments.m_bVerbose;
  gbTracepoint = gxArguments.m_bTracepoint;
  gbProgress   = gxArguments.m_bProgress;
  gbMetadata   = gxArguments.m_bMetadata;
  
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
  
//  if (gxArguments.m_bVerbose)
    if ( gxArguments.m_bWelcome)
      gxArguments.vPrint();

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
  
  // change working directory to commands dir
  bool bResult = xDir.bChangeCurrent(gxArguments.m_sCommands);
  
  // kill previous
  vKillPrev();
  
  // either create new shm region
  // or attach to an existing one
  RapidsSE::vPoolOpen();
  
  //vSkipListTest();
  if (xDir.bGetCurrent(sCurrent) && gxArguments.m_bWelcome)
    vInfo("\nCURRENT WORKING DIRECTORY: %s\n\n"_Stz,sCurrent.aStr());
  
  // execute the command line
  RapidsSE::vExecute();
  
  // unit tests ?
  if (gxArguments.m_bUnitTests) {
    RunTests* pRunTests = new RunTests();
    pRunTests->vRunTests();
  }

  /*
  if (gxArguments.m_bStart) {
    KeyTest::vTestB();
    KeyTest::vTestA();
    KeyTest::vTest();
  }
  */
  
  // optionally execute se> prompt
  if (gxArguments.m_bPrompt || gxArguments.m_bScript) {
    
    // if only prompt then cwd to Commands
    // (with --start sen changes cwd)
//    if (!gxArguments.m_bStart) {
//      // change working directory to commands dir
//      bool bResult = xDir.bSetCurrent(gxArguments.m_sCommands);
//      // ignore error
    
 //     if (xDir.bGetCurrent(sCurrent))
 //       vInfo("dir: %s\n"_Stz,sCurrent.aStr());
//    }
    SePrompt* pSePrompt = new SePrompt();
    pSePrompt->vLoop();
    delete pSePrompt;
  }

  // unmap from shm and optionally unlink it
  RapidsSE::vPoolClose();

  if (gxArguments.m_bVerbose)
    vInfo("RapidsSE exits\n"_Stz);
    
  return EXIT_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* kill previous node instances                                               */
/*----------------------------------------------------------------------------*/
void vKillPrev(void)
{
  // only node0 does this when starting
  //if ( ! (0 == gpGrid->iThisNode() && gxArguments.m_bStart) )
  //  return;
  
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
  
  i8 nBytes = IntLib::iIntToVlq(0,xVlq);
  i8 i = IntLib::iVlqToInt(xVlq);
  assert(0 == i);
  
  nBytes = IntLib::iIntToVlq(30,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  assert(30 == i);
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
