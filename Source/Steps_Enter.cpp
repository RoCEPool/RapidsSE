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
void Task::vEnterObject(MP_Task mpTask)
{
  ENTRY("vEnterObject"_Str);
  
  mpTask->m_pReturnStack->vPushCell(mpTask->IP.m_pCell);
  mpTask->IP.m_pCell = *(Cell**)mpTask->m_pEntryCell;
  
  //  Word* pWord = pAdd<Word*>(mpTask->m_pEntryCell,-(sizeof(Word)-sizeof(Cell*)));

  Word* pWord = pAdd<Word*>(mpTask->m_pEntryCell,
                            -( sizeof(Word)
                               - sizeof(Cell*)
                               - sizeof(WspFxd<C_SIZE_MEDIUM_OBJECT - C_1KB>)
                               - sizeof(Footer)
                             )
                            );

  ASSERT(WT_OBJECT == pWord->eWordType());
  
  Object* pObject = (Object*)pWord->pFirstCell();
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enterRoutine -- enter word routine    (this is the does step for a : word) */
/*----------------------------------------------------------------------------*/
void Task::vEnterRoutine(MP_Task mpTask)
{
  ENTRY("vEnterRoutine"_Str);
  
  mpTask->m_pReturnStack->vPushCell(mpTask->IP.m_pCell);
  mpTask->IP.m_pCell = *(Cell**)mpTask->m_pEntryCell;
 
   if ( (mpTask->m_iTracing & C_TRACE_ROUTINES) )
    mpTask->vTracePoint(">"_Str);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
