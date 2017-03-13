/*============================================================================*/
/* Steps_Dot.cpp - dot step group                                             */
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
  vAddPrimitive("."_Str,               &vDot,              false);
  vAddPrimitive(".put"_Str,            &vDotPut,           false);
  vAddPrimitive(".\\n"_Str,            &vDotNewline,       false);
  vAddPrimitive(".verbose"_Str,     &vDotVerbose,       false);
}

/*----------------------------------------------------------------------------*/
/* . -- print top value on stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vDot(NNP_Task nnpTask)
{
  ENTRY("vDot"_Str);
  
  s48K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK("."_Str);
  
  ValLib::vToString(&xValWsp,sWorkspace);
  /*
  vTrace("[N%lld:%8s\\%lld %-11s] %s\n"_Str,
         
         gpGrid->iNode(),
         nnpTask->m_sName.aStr(),
         *nnpTask->m_zAcks,
         nnpTask->m_sComment.aStr(),
         
         sWorkspace.aStr()
         );
  */
  vTrace("[%8s\\%lld %-11s] %s\n"_Str,
         
         nnpTask->m_sName.aStr(),
         *nnpTask->m_zAcks,
         nnpTask->m_sComment.aStr(),
         
         sWorkspace.aStr()
         );

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .put -- print top value on stack (no new line)                             */
/*----------------------------------------------------------------------------*/
void Task::vDotPut(NNP_Task nnpTask)
{
  ENTRY("vDotPut"_Str);
  
  s48K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK(".put"_Str);
  
  ValLib::vToString(&xValWsp,sWorkspace);
  vTrace("%s"_Str,sWorkspace.aStr());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .\n -- print new line                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDotNewline(NNP_Task nnpTask)
{
  ENTRY("vDotNewline"_Str);
  
  vTrace("\n"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .verbose -- print top value on stack if arg.m_bVerbose                     */
/*----------------------------------------------------------------------------*/
void Task::vDotVerbose(NNP_Task nnpTask)
{
  ENTRY("vDotVerbose"_Str);
  
  s48K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK(".verbose"_Str);
  
  if (gxArguments.m_bVerbose) {
    ValLib::vToString(&xValWsp,sWorkspace);
    vTrace("[%p %s] %s\n"_Str,*nnpTask,nnpTask->m_sName.aStr(),sWorkspace.aStr());
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
