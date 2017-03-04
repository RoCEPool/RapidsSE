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


void vStrTest(Str* pStr)
{
  pStr->vPrint();
}

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, const char* argv[])
{
  
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  vStrTest("This is an Str literal"_Str);
  
  char* a = *"This is an Str literal"_Str;
  
  // this compiles but doesnt work.  not even sure what it means
  Str xStr = "abc";
  xStr.vPrint();
  
  // this works.  _Str takes a cons char* and casts it to a Str*
  Str* pStr = "abc"_Str;
  pStr->vPrint();
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  printf(
         "sizeof(eCN) %lu; sizeof(bool) %lu; \n"
         "sizeof(Object) %lu; sizeof(StrVal) %lu; \n"
         "sizeof(s32) %lu; sizeof(s256) %lu; \n"
         "sizeof(Value) %lu; sizeof(IntVal) %lu; sizeof(ValWsp) %lu \n"
         "\n",
         sizeof(eCN),sizeof(bool),
         sizeof(Object),sizeof(StrVal),
         sizeof(s32),sizeof(s256),
         sizeof(Value),sizeof(IntVal),sizeof(ValWsp)
  );
  
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
  
  if (gxArguments.m_bWelcome && ! gxArguments.m_bStatus)
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
//    printf("dir: %s\n",sCurrent.aStr());
  
  
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

  // write the regionName into a se.region file
  // used by shell script to start se prompt
  if (gxArguments.m_bStart) {
    File xFile;
    xFile.vPath("./se.region");
    bool b = xFile.bOpen(C_OPEN_READ_WRITE_CREATE);
    ASSERT(b);
    xFile.vPrint("%s\n",gxArguments.m_sRegionName.aStr());
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
//        printf("dir: %s\n",sCurrent.aStr());
//    }
    SePrompt* pSePrompt = new SePrompt();
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
    
    String::vSprint(sRegionName.aStr(),"%s_%s",
      gxArguments.m_sRegionPrefix.aStr(),
      gxArguments.m_xNodes[iIndex].m_sNodeName.aStr());
    
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
