/*============================================================================*/
/* Steps_Enter.cpp - enter step group                                         */
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

// enter: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Enter(void)
{
  vAddPrimitive("enterObject",     &vEnterObject,      false);
  vAddPrimitive("enterRoutine",    &vEnterRoutine,     false);
}

/*----------------------------------------------------------------------------*/
/* enterObject -- enter object routine   (this is the does step for a object) */
/*----------------------------------------------------------------------------*/
void Task::vEnterObject(NNP_Task nnpTask)
{
  ENTRY("vEnterObject");
  
  nnpTask->m_pReturnStack->vPush(nnpTask->IP.m_pCell);
  nnpTask->IP.m_pCell = *(Cell**)nnpTask->m_pEntryCell;
  
  Word* pWord = reinterpret_cast<Word*>(
                  pcAdd(nnpTask->m_pEntryCell, -(sizeof(Word)-sizeof(Cell*)) ));
  
  ASSERT(WT_OBJECT == pWord->eWordType());
  
  Object* pObject = (Object*)pWord->pFirstCell();
  nnpTask->m_pDataStack->vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enterRoutine -- enter word routine    (this is the does step for a : word) */
/*----------------------------------------------------------------------------*/
void Task::vEnterRoutine(NNP_Task nnpTask)
{
  ENTRY("vEnterRoutine");
  
  nnpTask->m_pReturnStack->vPush(nnpTask->IP.m_pCell);
  nnpTask->IP.m_pCell = *(Cell**)nnpTask->m_pEntryCell;
 
   if ( (nnpTask->m_iTracing & C_TRACE_ROUTINES) )
    nnpTask->vTracePoint(">");
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
