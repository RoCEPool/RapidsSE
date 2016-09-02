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
  printf("RapidsSE %s (%s %s)\n",RAPIDSSE_VERSION_STRING,__DATE__,__TIME__);
  
  RapidsSE::vCheckAssumptions();
  
//  Charred* pCharred = new(true) Charred;
//
//  s32 sPrompt("char> ");
//  pCharred->vPrompt(sPrompt);
//  pCharred->vUseStdin();
//  
//  vTrace(">>>");
//  while (char c = pCharred->cGet())
//    vTrace("%c",c);
//  vTrace("<<<\n");
  
//  s256 sString("\n");
//  s256 sString("abc \n");
//  s256 sString("a/* block comment */def//linecomment\n");
//  s256 sString("quick brown() fox jump over lazy fox\n");
//  s256 sString("quick brown() fox /* didn't */jump over lazy fox\n");
//  pCharred->vUseString(sString);

//  s256 sPath("/tmp/_tpch");
//  bool b = pCharred->bUseFile(sPath);
//  assert(b);
//  
//  vTrace(">>>");
//  while (char c = pCharred->cGet())
//    vTrace("%c",c);
//  vTrace("<<<\n");
//  
//  sleep(100000);
  
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
    exit(EXIT_FAILURE);
  
  gxArguments.vPrint();
  
  // either create new shm region
  // or attach to an existing one
  RapidsSE::vPoolOpen();
  
  // execute the command line
  RapidsSE::vExecute();
  
  // optionally execute se> prompt
  if (gxArguments.m_bSePrompt) {
    SePrompt* pSePrompt = new SePrompt(gxArguments.m_iPartition);
    pSePrompt->vPrompt();
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
  using namespace StorageEngine;
  
//  Task* xTask[1024];
//  
//  Heap* pHeap = new Heap(CN_MEDIUMHEAP,C_SIZE_MEDIUM_OBJECT);
//  
//  for (i8 i = 0; i < 1024; i++)
//    xTask[i] = (Task*)pHeap->pAllocObject();
//  
//  for (i8 i = 0; i < 1024; i++)
//    pHeap->vFreeObject(xTask[i]);
//  
//  i8 a = 1;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/