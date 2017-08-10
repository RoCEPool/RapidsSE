/*============================================================================*/
/* Steps_Logic.cpp - logic step group                                         */
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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// logic: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Logic(void)
{
  vAddPrimitive("and"_Stz,             &vLogicAnd,         false);
  vAddPrimitive("not"_Stz,             &vLogicNot,         false);
  vAddPrimitive("or"_Stz,              &vLogicOr,          false);
}

/*----------------------------------------------------------------------------*/
/* and -- logical and of top two values                                       */
/*----------------------------------------------------------------------------*/
void Task::vLogicAnd(MP_Task mpTask)
{
  ENTRY("vLogicAnd"_Stz);
  
  i8 iArgB;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iArgB) )
    THROW_INVALID_DSTACK("and"_Stz);
  
  i8 iArgA;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iArgA) )
    THROW_INVALID_DSTACK("and"_Stz);
  
  i8 iArgR = (iArgA && iArgB) ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(iArgR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* not -- not of top value                                                    */
/*----------------------------------------------------------------------------*/
void Task::vLogicNot(MP_Task mpTask)
{
  ENTRY("vLogicNot"_Stz);
  
  i8 iValA;
  if ( ! mpTask->m_pDataStack->bPopAsI8_Inline(iValA) )
    THROW_INVALID_DSTACK("not"_Stz);
  
  i8 iNotA = iValA ? 0 : 1;
  
  mpTask->m_pDataStack->vPushInt(iNotA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* or -- logical or of top two values                                         */
/*----------------------------------------------------------------------------*/
void Task::vLogicOr(MP_Task mpTask)
{
  ENTRY("vLogicOr"_Stz);
  
  i8 iArgB;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iArgB) )
    THROW_INVALID_DSTACK("or"_Stz);
  
  i8 iArgA;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iArgA) )
    THROW_INVALID_DSTACK("or"_Stz);
  
  i8 iArgR = (iArgA || iArgB) ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(iArgR);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
