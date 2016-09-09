/*============================================================================*/
/* main.cpp - main entry                                                      */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

extern Arguments  gxArguments;

void vTest1(void);

/*----------------------------------------------------------------------------*/
/* main                                                                       */
/*----------------------------------------------------------------------------*/
int main(int argc, const char* argv[])
{  
  printf("RapidsSE v%s (%s %s)\n",RAPIDSSE_VERSION_STRING,__DATE__,__TIME__);
  
  RapidsSE::vCheckAssumptions();
    
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

  
  if ( ! gxArguments.bParse(argc,argv) )
    ASSERT(0);
  
  if (gxArguments.m_bVerbose)
    gxArguments.vPrint();
  
  // either create new shm region
  // or attach to an existing one
  RapidsSE::vPoolOpen();
  
  // execute the command line
  RapidsSE::vExecute();
  
  // optionally execute se> prompt
  if (gxArguments.m_bSePrompt) {
    SePrompt* pSePrompt = new SePrompt(gxArguments.m_iPartition);
    pSePrompt->vLoop();
  }

  // unmap from shm and optionally unlink it
  RapidsSE::vPoolClose();

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