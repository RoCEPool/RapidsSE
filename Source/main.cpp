/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"
#include "UnitTests.h"

extern Arguments gxArguments;
extern Grid*     gpGrid;

void vTest1(void);

void vKillPrev(void);


/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, const char* argv[])
{
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  i8 iLevels[6];
  for (u8 iLevel = 0; iLevel < 6; iLevel++)
    iLevels[iLevel] = 0;
  
  for (u8 i = 0; i < 10000; i++) {
    //printf("%lld\n",Random::uLogLevel(6, 2));
    u8 iLevel = Random::uLogLevel(6, 2);
    iLevels[iLevel]++;
  }
  for (i8 iLevel = 0; iLevel < 6; iLevel++)
    printf("%lld: %lld\n",iLevel,iLevels[iLevel]);
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  
  
  
  vInfo(
         "sizeof(eCN) %lu; sizeof(bool) %lu; \n"_Str
         "sizeof(Object) %lu; sizeof(StrVal) %lu; \n"_Str
         "sizeof(s32) %lu; sizeof(s256) %lu; \n"_Str
         "sizeof(Value) %lu; sizeof(IntVal) %lu; sizeof(ValWsp) %lu \n"_Str
         "\n"_Str,
         sizeof(eCN),sizeof(bool),
         sizeof(Object),sizeof(StrVal),
         sizeof(s32),sizeof(s256),
         sizeof(Value),sizeof(IntVal),sizeof(ValWsp)
  );
  
  Str* aRelDbg;
#ifdef _DEBUG
  aRelDbg = "debug"_Str;
#else
  aRelDbg = "release"_Str;
#endif
  
  //vTest1();
  
  StorageEngine::vCheckAllAssumptions();
  
  if ( ! gxArguments.bParse(argc,(Str**)argv) )
    exit(EXIT_FAILURE);
  
  if (gxArguments.m_bWelcome && ! gxArguments.m_bStatus)
    vInfo("RapidsSE v%s (%s %s -- %s) pid %lld\n"_Str,
           RAPIDSSE_VERSION_STRING,
           __DATE__,
           __TIME__,
           aRelDbg,
           Process::iGetPid()
           );

  Dir xDir;
  s256 sCurrent;
//  if (xDir.bGetCurrent(sCurrent))
//    vInfo("dir: %s\n",sCurrent.s());
  
  
//  vTrace("Task::vWord %p\n"_Str,Task::vWord);
  
//  Timer xTimer;
//  
//  xTimer.vStart();
//  
//  PosixThread::vSleepMilli(10);
//  
//  xTimer.vUpdate();
//  
//  vTrace("duration %lldns\n"_Str,xTimer.iNanoseconds());
  
//  if (gxArguments.m_bVerbose)
    gxArguments.vPrint();

  // write the regionName into a se.region file
  // used by shell script to start se prompt
  if (gxArguments.m_bStart) {
    File xFile;
    xFile.vPath("./se.region"_Str);
    bool b = xFile.bOpen(C_OPEN_READ_WRITE_CREATE);
    ASSERT(b);
    xFile.vPrint("%s\n"_Str,gxArguments.m_sRegionName.s());
    xFile.vClose();
  }
  
  // change working directory to commands dir
  bool bResult = xDir.bChangeCurrent(gxArguments.m_sCommands);
  
  // kill previous
  vKillPrev();
  
  // either create new shm region
  // or attach to an existing one
  RapidsSE::vPoolOpen();
  
  // execute the command line
  RapidsSE::vExecute();
  
  // unit tests ?
  if (gxArguments.m_bUnitTests) {
    RunTests* pRunTests = new RunTests();
    pRunTests->vRunTests();
  }
  
  // optionally execute se> prompt
  if (gxArguments.m_bPrompt) {
    
    // if only prompt then cwd to Commands
    // (with --start sen changes cwd)
//    if (!gxArguments.m_bStart) {
//      // change working directory to commands dir
//      bool bResult = xDir.bSetCurrent(gxArguments.m_sCommands);
//      // ignore error
//      if (xDir.bGetCurrent(sCurrent))
//        vInfo("dir: %s\n",sCurrent.);
//    }
    SePrompt* pSePrompt = new SePrompt();
    pSePrompt->vLoop();
    delete pSePrompt;
  }

  // unmap from shm and optionally unlink it
  RapidsSE::vPoolClose();

  if (gxArguments.m_bVerbose)
    vInfo("RapidsSE exits\n"_Str);
    
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
  
  for (i8 iIndex=0; iIndex < gxArguments.m_nNodes; iIndex++) {
    
    sRegionName.vSprint("%s_%s"_Str,
                        gxArguments.m_sRegionPrefix.s(),
                        gxArguments.m_xNodes[iIndex].m_sNodeName.s());
    
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
        vTrace("vKillPrev: process %lld killed\n"_Str,pAttachMap->m_iPid);
      else
        vTrace("vKillPrev: process %lld not killed\n"_Str,pAttachMap->m_iPid);
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
  
  vTrace("Reading lineitem.tbl in chunks of %lld\n"_Str, iChunk);
  xFile.vPath("/Volumes/Data/Users/alkayamarti/tpch_SF10/lineitem.tbl"_Str);
  bool b = xFile.bOpen(C_OPEN_READ);
  ASSERT(b);
  
  xTimer.vStart();
  i8 iBytesRead = xFile.iReadChunk(buffer,iChunk);
  
  while (iBytesRead) {
    iBytesRead = xFile.iReadChunk(buffer,iChunk);
  }
  xTimer.vUpdate();
  vTrace("duration %lldms\n"_Str,xTimer.iMilliseconds());
  
  xFile.vClose();
}

//  i8 i = 0;
//  Synch::uAtomicInc(&i);
//  vInfo("AtomicInc i = %lld\n"_Str,i);
//  Synch::uAtomicDec(&i);
//  vInfo("AtomicDec i = %lld\n"_Str,i);
//
//  u8 u = 2;
//  bool b = Synch::bTestAndSet(&u,1,3);
//  vInfo("TestAndSet %s u = %lld\n"_Str,b ? "true"_Str : "false"_Str,u);


//  Timer xTimer;
//  xTimer.vStart();
//  PosixThread::vSleepMicro(500);
//  xTimer.vUpdate();
//
//  vInfo("Timert %lld\n"_Str,xTimer.iMicroseconds());


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
