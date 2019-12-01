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
  ENTRY(); // ENTRY("vSlotsColumnCount"_Stz);
  
  Object* pPtrSlots;
  m_pDataStack->vPopPtr(pPtrSlots,__func__);
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  m_pDataStack->vPushInt(pSlots->pPtnColVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.columnName -- get column name                                        */
/*----------------------------------------------------------------------------*/
void Step::vSlotsColumnName(void)
{
  ENTRY(); // ENTRY("vSlotsColumnName"_Stz);
  
  Object* pPtrSlots;
  m_pDataStack->vPopPtr(pPtrSlots,__func__);
  Slots* pSlots = _pSlots(pPtrSlots);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  i8 iIndex = m_pDataStack->iPopInt(__func__);
  
  ASSERT(iIndex < pSlots->pPtnColVec()->nItems());
  Column* pColumn = pSlots->pPtnColVec()->pGet(iIndex);
  
  s32 sColNm(pColumn->szColNm());
  
  m_pDataStack->vPushStr(sColNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* slots.createColumnVec -- create slots column vector from subdic         */
/*----------------------------------------------------------------------------*/
void Step::vSlotsCreateColumnVec(void)
{
  ENTRY(); // ENTRY("vSlotsCreateColumnVec"_Stz);
  
  Object* pPtrSlots;
  m_pDataStack->vPopPtr(pPtrSlots,__func__);
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
  ENTRY(); // ENTRY("vSlotsSubDic"_Stz);
  
  Object* pSlotsPtr;
  m_pDataStack->vPopPtr(pSlotsPtr,__func__);
  
  Slots* pSlots = _pSlots(pSlotsPtr);
  ASSERT(CN_SLOTS == pSlots->eClass());
  
  Dic* pDic = pSlots->pSubDic();
  
  m_pDataStack->vPushPtr(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
