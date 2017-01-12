/*============================================================================*/
/* Task_Dot.cpp - task dot group                                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// dot: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Dot(void)
{
  vAddPrimitive(".",               &vDot,              false);
  vAddPrimitive(".put",            &vDotPut,           false);
  vAddPrimitive(".\\n",            &vDotNewline,       false);
  vAddPrimitive(".verbose",     &vDotVerbose,       false);
}

/*----------------------------------------------------------------------------*/
/* . -- print top value on stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vDot(NNP_Task nnpTask)
{
  ENTRY("vDot");
  
  s48K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK(".");
  
  xValWsp.vToString(sWorkspace.a());
  vTrace("[N%lld:%8s\\%lld %-11s] %s\n",
         
         gxArguments.m_iNode,
         nnpTask->m_sName.a(),
         *nnpTask->m_zAcks,
         nnpTask->m_sComment.a(),
         
         sWorkspace.a()
         );
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .put -- print top value on stack (no new line)                             */
/*----------------------------------------------------------------------------*/
void Task::vDotPut(NNP_Task nnpTask)
{
  ENTRY("vDotPut");
  
  s48K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK(".put");
  
  xValWsp.vToString(sWorkspace.a());
  vTrace("%s",sWorkspace.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .\n -- print new line                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDotNewline(NNP_Task nnpTask)
{
  ENTRY("vDotNewline");
  
  vTrace("\n");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .verbose -- print top value on stack if arg.m_bVerbose                     */
/*----------------------------------------------------------------------------*/
void Task::vDotVerbose(NNP_Task nnpTask)
{
  ENTRY("vDotVerbose");
  
  s48K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK(".verbose");
  
  if (gxArguments.m_bVerbose) {
    xValWsp.vToString(sWorkspace.a());
    vTrace("[%p %s] %s\n",*nnpTask,nnpTask->m_sName.a(),sWorkspace.a());
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
