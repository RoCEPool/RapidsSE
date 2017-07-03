/*============================================================================*/
/* Steps_Slots.cpp - partitions slots step group                              */
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

// table: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Slots(void)
{
  vAddPrimitive("slots.columnCount"_Str,     &vSlotsColumnCount,     false);
  vAddPrimitive("slots.columnName"_Str,      &vSlotsColumnName,      false);
  vAddPrimitive("slots.createColumnVec"_Str, &vSlotsCreateColumnVec, false);
  vAddPrimitive("slots.subDic"_Str,          &vSlotsSubDic,          false);
}

/*----------------------------------------------------------------------------*/
/* slots.columnCount -- get column count                                      */
/*----------------------------------------------------------------------------*/
void Task::vSlotsColumnCount(MP_Task mpTask)
{
  ENTRY("vSlotsColumnCount"_Str);
  
  Object* pPtrSlots;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSlots) )
    THROW_INVALID_DSTACK("slots.columnCount"_Str);
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSlots->pPtnColVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.columnName -- get column name                                        */
/*----------------------------------------------------------------------------*/
void Task::vSlotsColumnName(MP_Task mpTask)
{
  ENTRY("vSlotsColumnName"_Str);
  
  Object* pPtrSlots;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSlots) )
    THROW_INVALID_DSTACK("slots.columnName"_Str);
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  i8 iIndex;
  if ( ! mpTask->m_pDataStack->bPop(iIndex) )
    THROW_INVALID_DSTACK("slots.columnName"_Str);
  
  ASSERT(iIndex < pSlots->pPtnColVec()->nItems());
  Column* pColumn = pSlots->pPtnColVec()->pGet(iIndex);
  
  mpTask->m_pDataStack->vPushS32(pColumn->sColumnName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.createColumnVec -- create slots column vector from subdic            */
/*----------------------------------------------------------------------------*/
void Task::vSlotsCreateColumnVec(MP_Task mpTask)
{
  ENTRY("vSlotsCreateColumnVec"_Str);
  
  Object* pPtrSlots;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSlots) )
    THROW_INVALID_DSTACK("slots.createColumnVec"_Str);
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  s32 sColumnNm;
  if ( ! mpTask->m_pDataStack->bPop(sColumnNm) )
    THROW_INVALID_DSTACK("slots.createColumnVec"_Str);
  
  s48K sColumnDef;
  if ( ! mpTask->m_pDataStack->bPop(sColumnDef) )
    THROW_INVALID_DSTACK("slots.createColumnVec"_Str);
  
  pSlots->vCreateColumnVec(sColumnNm, sColumnDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.subDic -- get slots sub dictionary                                   */
/*----------------------------------------------------------------------------*/
void Task::vSlotsSubDic(MP_Task mpTask)
{
  ENTRY("vSlotsSubDic"_Str);
  
  Object* pSlotsPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSlotsPtr) )
    THROW_INVALID_DSTACK("slots.subDic"_Str);
  
  Slots* pSlots = _pSlots(pSlotsPtr);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  Dic* pDic = pSlots->pSubDic();
  
  mpTask->m_pDataStack->vPushObject(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
