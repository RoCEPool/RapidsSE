/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"
#include "UnitTests.h"

extern Arguments  gxArguments;

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
   
//  s256 sTest;
//  sTest =
//    "abcdefghijklmnopqrstuvwxuz"
//    "abcdefghijklmnopqrstuvwxuz"
//    "abcdefghijklmnopqrstuvwxuz"
//  ;
//  s32 sTest2;
//  sTest2 = sTest;
  
  RapidsSE::vCheckAssumptions();
  
  if ( ! gxArguments.bParse(argc,argv) )
    exit(EXIT_FAILURE);
  
  if (gxArguments.m_bWelcome)
    printf("RapidsSE v%s (%s %s -- %s)\n",
           RAPIDSSE_VERSION_STRING,
           __DATE__,
           __TIME__,
           aRelDbg
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
    Tests* pTests = new Tests();
    pTests->vRunTests();
  }
  
  // optionally execute se> prompt
  if (gxArguments.m_bPrompt) {
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
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/