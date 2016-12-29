/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"
#include "UnitTests.h"

extern Arguments gxArguments;

void vTest1(void);

void vKillPrev(void);

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, const char* argv[])
{
//  i8 i = 0;
//  Synch::uAtomicInc(&i);
//  printf("AtomicInc i = %lld\n",i);
//  Synch::uAtomicDec(&i);
//  printf("AtomicDec i = %lld\n",i);
//  
//  u8 u = 2;
//  bool b = Synch::bTestAndSet(&u,1,3);
//  printf("TestAndSet %s u = %lld\n",b ? "true" : "false",u);
  
  
//  Timer xTimer;
//  xTimer.vStart();
//  PosixThread::vSleepMicro(500);
//  xTimer.vUpdate();
//  
//  printf("Timert %lld\n",xTimer.iMicroseconds());
  
  const char *aRelDbg;
#ifdef _DEBUG
  aRelDbg = "debug";
#else
  aRelDbg = "release";
#endif
  
  //vTest1();
  
  StorageEngine::vCheckAllAssumptions();
  
  if ( ! gxArguments.bParse(argc,argv) )
    exit(EXIT_FAILURE);
  
  if (gxArguments.m_bWelcome)
    printf("RapidsSE v%s (%s %s -- %s) pid %lld\n",
           RAPIDSSE_VERSION_STRING,
           __DATE__,
           __TIME__,
           aRelDbg,
           Process::iGetPid()
           );

  Dir xDir;
  s256 sCurrent;
//  if (xDir.bGetCurrent(sCurrent))
//    printf("dir: %s\n",sCurrent.a());
  
  
//  vTrace("Task::vWord %p\n",Task::vWord);
  
//  Timer xTimer;
//  
//  xTimer.vStart();
//  
//  PosixThread::vSleepMilli(10);
//  
//  xTimer.vUpdate();
//  
//  vTrace("duration %lldns\n",xTimer.iNanoseconds());
  
//  if (gxArguments.m_bVerbose)
    gxArguments.vPrint();
  
  // change working directory to commands dir
  bool bResult = xDir.bSetCurrent(gxArguments.m_sCommands);
  // ignore error
//  if (xDir.bGetCurrent(sCurrent))
//    printf("dir: %s\n",sCurrent.a());
  
  
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
//        printf("dir: %s\n",sCurrent.a());
//    }
    SePrompt* pSePrompt = new SePrompt(gxArguments.m_iPartition);
    pSePrompt->vLoop();
  }

  // unmap from shm and optionally unlink it
  RapidsSE::vPoolClose();

  if (gxArguments.m_bVerbose)
    printf("RapidsSE exits\n");
    
  return EXIT_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* kill previous node instances                                               */
/*----------------------------------------------------------------------------*/
void vKillPrev(void)
{
  // only node0 does this
  if (0 != gxArguments.m_iNode)
    return;
  
  // an assumption is that regions will be /SE_<node>
  
  void* pRegion;
  s256 sRegionName;
  int iFD;
  bool b;
  
  for (i8 iNode = 0; iNode < gxArguments.m_nNodes; iNode++) {
  
    String::vSprint(sRegionName.a(),"/SE_%lld",iNode);
    
    if ( ! Region::bTest(sRegionName) )
      continue;
    
    if ( ! (pRegion = Region::pOpen(sRegionName,iFD)) )
      continue;
    
    AttachMap* pAttachMap =
      reinterpret_cast<AttachMap*>(pcAdd(pRegion,C_SIZE_POOL_BUFFER));

    if (Process::bTestPid(pAttachMap->m_iPid)) {
      
      b = Process::bKillPid(pAttachMap->m_iPid);
      if ( b )
        vTrace("vKillPrev: process %lld killed\n",pAttachMap->m_iPid);
      else
        vTrace("vKillPrev: process %lld not killed\n",pAttachMap->m_iPid);
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
  
  vTrace("Reading lineitem.tbl in chunks of %lld\n", iChunk);
  xFile.vPath("/Volumes/Data/Users/alkayamarti/tpch_SF10/lineitem.tbl");
  bool b = xFile.bOpen(C_OPEN_READ);
  ASSERT(b);
  
  xTimer.vStart();
  i8 iBytesRead = xFile.iReadChunk(buffer,iChunk);
  
  while (iBytesRead) {
    iBytesRead = xFile.iReadChunk(buffer,iChunk);
  }
  xTimer.vUpdate();
  vTrace("duration %lldms\n",xTimer.iMilliseconds());
  
  xFile.vClose();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
