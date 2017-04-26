/*============================================================================*/
/* Steps_Enter.cpp - enter step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

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
  vAddPrimitive("enterObject"_Str,     &vEnterObject,      false);
  vAddPrimitive("enterRoutine"_Str,    &vEnterRoutine,     false);
}

/*----------------------------------------------------------------------------*/
/* enterObject -- enter object routine   (this is the does step for a object) */
/*----------------------------------------------------------------------------*/
void Task::vEnterObject(NNP_Task nnpTask)
{
  ENTRY("vEnterObject"_Str);
  
  nnpTask->m_pReturnStack->vPush(nnpTask->IP.m_pCell);
  nnpTask->IP.m_pCell = *(Cell**)nnpTask->m_pEntryCell;
  
  //  Word* pWord = pAdd<Word*>(nnpTask->m_pEntryCell,-(sizeof(Word)-sizeof(Cell*)));

  Word* pWord = pAdd<Word*>(nnpTask->m_pEntryCell,
                            -( sizeof(Word)
                               - sizeof(Cell*)
                               - sizeof(WspFxd<C_SIZE_MEDIUM_OBJECT - C_1KB>)
                               - sizeof(Footer)
                             )
                            );

  ASSERT(WT_OBJECT == pWord->eWordType());
  
  Object* pObject = (Object*)pWord->pFirstCell();
  nnpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enterRoutine -- enter word routine    (this is the does step for a : word) */
/*----------------------------------------------------------------------------*/
void Task::vEnterRoutine(NNP_Task nnpTask)
{
  ENTRY("vEnterRoutine"_Str);
  
  nnpTask->m_pReturnStack->vPush(nnpTask->IP.m_pCell);
  nnpTask->IP.m_pCell = *(Cell**)nnpTask->m_pEntryCell;
 
   if ( (nnpTask->m_iTracing & C_TRACE_ROUTINES) )
    nnpTask->vTracePoint(">"_Str);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
