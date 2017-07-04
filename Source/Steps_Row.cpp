/*============================================================================*/
/* Steps_Row.cpp - row step group                                             */
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

// index: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Row(void)
{
  vAddPrimitive("row.string"_Str,  &vRowString, false);
}

/*----------------------------------------------------------------------------*/
/* row.string -- get row as string                                            */
/*----------------------------------------------------------------------------*/
void Task::vRowString(MP_Task mpTask)
{
  ENTRY("vRowString"_Str);
  
  Object* pPtrRow;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRow) )
    THROW_INVALID_DSTACK("row.string"_Str);
  
  Entry* pRow = _pEntry(pPtrRow);
  ASSERT(CN_ENTRY == pRow->eClass());
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(pRow->aRow());
  s256 sRow = pStrVal->aStr();
  
  mpTask->m_pDataStack->vPushS256(sRow);
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
