/*============================================================================*/
/* Steps_Slots.cpp - partitions slots step group                              */
/*============================================================================*/

#include "Steps.h"

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
void Step::vSlotsColumnCount(void)
{
  ENTRY("vSlotsColumnCount"_Stz);
  
  Object* pPtrSlots;
  if ( ! m_pDataStack->bPop(pPtrSlots) )
    THROW_DSTACK();
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  m_pDataStack->vPushI8(pSlots->pPtnColVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.columnName -- get column name                                        */
/*----------------------------------------------------------------------------*/
void Step::vSlotsColumnName(void)
{
  ENTRY("vSlotsColumnName"_Stz);
  
  Object* pPtrSlots;
  if ( ! m_pDataStack->bPop(pPtrSlots) )
    THROW_DSTACK();
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  i8 iIndex;
  if ( ! m_pDataStack->bPop(iIndex) )
    THROW_DSTACK();
  
  ASSERT(iIndex < pSlots->pPtnColVec()->nItems());
  Column* pColumn = pSlots->pPtnColVec()->pGet(iIndex);
  
  m_pDataStack->vPushS32(pColumn->sColumnName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.createColumnVec -- create slots column vector from subdic         */
/*----------------------------------------------------------------------------*/
void Step::vSlotsCreateColumnVec(void)
{
  ENTRY("vSlotsCreateColumnVec"_Stz);
  
  Object* pPtrSlots;
  if ( ! m_pDataStack->bPop(pPtrSlots) )
    THROW_DSTACK();
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  pSlots->vCreateColumnVec();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.subDic -- get slots sub dictionary                                   */
/*----------------------------------------------------------------------------*/
void Step::vSlotsSubDic(void)
{
  ENTRY("vSlotsSubDic"_Stz);
  
  Object* pSlotsPtr;
  if ( ! m_pDataStack->bPop(pSlotsPtr) )
    THROW_DSTACK();
  
  Slots* pSlots = _pSlots(pSlotsPtr);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  Dic* pDic = pSlots->pSubDic();
  
  m_pDataStack->vPushPointer(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
