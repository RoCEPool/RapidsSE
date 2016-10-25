/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"
#include "UnitTests.h"

extern Arguments gxArguments;

void vTest1(void);

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, const char* argv[])
{
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
  
  if (gxArguments.m_bVerbose)
    gxArguments.vPrint();
  
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
    if (!gxArguments.m_bStart) {
      
      Dir xDir;
      s256 sCommandDir;
      
#ifdef __APPLE__
      sCommandDir = "../../../../Commands";
#else
      sCommandDir = "../../Commands";"
#endif
      
      // change working directory to Commands
      bool bResult = xDir.bSetCurrent(sCommandDir);
      // ignore error
    }
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
