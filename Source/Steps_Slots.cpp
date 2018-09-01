/*============================================================================*/
/* Steps_Slots.cpp - partitions slots step group                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Slots(void)
{
  vAddPrimitive("slots.columnCount"_Stz,     &vSlotsColumnCount,     false);
  vAddPrimitive("slots.columnName"_Stz,      &vSlotsColumnName,      false);
  vAddPrimitive("slots.createColumnVecR35"_Stz, &vSlotsCreateColumnVecR35, false);
  vAddPrimitive("slots.subDic"_Stz,          &vSlotsSubDic,          false);
}

/*----------------------------------------------------------------------------*/
/* slots.columnCount -- get column count                                      */
/*----------------------------------------------------------------------------*/
void Task::vSlotsColumnCount(MP_Task mpTask)
{
  ENTRY("vSlotsColumnCount"_Stz);
  
  Object* pPtrSlots;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSlots) )
    THROW_INVALID_DSTACK("slots.columnCount"_Stz);
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
  ENTRY("vSlotsColumnName"_Stz);
  
  Object* pPtrSlots;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSlots) )
    THROW_INVALID_DSTACK("slots.columnName"_Stz);
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  i8 iIndex;
  if ( ! mpTask->m_pDataStack->bPop(iIndex) )
    THROW_INVALID_DSTACK("slots.columnName"_Stz);
  
  ASSERT(iIndex < pSlots->pPtnColVec()->nItems());
  Column* pColumn = pSlots->pPtnColVec()->pGet(iIndex);
  
  mpTask->m_pDataStack->vPushS32(pColumn->sColumnName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.createColumnVecR35 -- create slots column vector from subdic         */
/*----------------------------------------------------------------------------*/
void Task::vSlotsCreateColumnVecR35(MP_Task mpTask)
{
  ENTRY("vSlotsCreateColumnVecR35"_Stz);
  
  Object* pPtrSlots;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSlots) )
    THROW_INVALID_DSTACK("slots.createColumnVecR35"_Stz);
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  pSlots->vCreateColumnVec();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.subDic -- get slots sub dictionary                                   */
/*----------------------------------------------------------------------------*/
void Task::vSlotsSubDic(MP_Task mpTask)
{
  ENTRY("vSlotsSubDic"_Stz);
  
  Object* pSlotsPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSlotsPtr) )
    THROW_INVALID_DSTACK("slots.subDic"_Stz);
  
  Slots* pSlots = _pSlots(pSlotsPtr);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  Dic* pDic = pSlots->pSubDic();
  
  mpTask->m_pDataStack->vPushObject(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
