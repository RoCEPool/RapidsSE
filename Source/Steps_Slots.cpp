/*============================================================================*/
/* Steps_Slots.cpp - partitions slots step group                              */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Slots(void)
{
  vAddStep("slots.columnCount"_Stz,     &Step::vSlotsColumnCount);
  vAddStep("slots.columnName"_Stz,      &Step::vSlotsColumnName);
  vAddStep("slots.createColumnVec"_Stz, &Step::vSlotsCreateColumnVec);
  vAddStep("slots.subDic"_Stz,          &Step::vSlotsSubDic);
}

/*----------------------------------------------------------------------------*/
/* slots.columnCount -- get column count                                      */
/*----------------------------------------------------------------------------*/
void Step::vSlotsColumnCount(MP_Task mpTask)
{
  ENTRY("vSlotsColumnCount"_Stz);
  
  Object* pPtrSlots;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSlots) )
    THROW_INVALID_DSTACK("slots.columnCount"_Stz);
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  mpTask->m_pDataStack->vPushI8(pSlots->pPtnColVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.columnName -- get column name                                        */
/*----------------------------------------------------------------------------*/
void Step::vSlotsColumnName(MP_Task mpTask)
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
/* slots.createColumnVec -- create slots column vector from subdic         */
/*----------------------------------------------------------------------------*/
void Step::vSlotsCreateColumnVec(MP_Task mpTask)
{
  ENTRY("vSlotsCreateColumnVec"_Stz);
  
  Object* pPtrSlots;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSlots) )
    THROW_INVALID_DSTACK("slots.createColumnVec"_Stz);
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  pSlots->vCreateColumnVec();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.subDic -- get slots sub dictionary                                   */
/*----------------------------------------------------------------------------*/
void Step::vSlotsSubDic(MP_Task mpTask)
{
  ENTRY("vSlotsSubDic"_Stz);
  
  Object* pSlotsPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSlotsPtr) )
    THROW_INVALID_DSTACK("slots.subDic"_Stz);
  
  Slots* pSlots = _pSlots(pSlotsPtr);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  Dic* pDic = pSlots->pSubDic();
  
  mpTask->m_pDataStack->vPushPointer(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
