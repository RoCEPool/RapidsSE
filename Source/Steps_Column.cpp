/*============================================================================*/
/* Steps_Column.cpp - column step group                                       */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Column(void)
{
  vAddStep("column.isNullable@"_Stz,     &Step::vColumnIsNullableFetch);
  vAddStep("column.isPartitionKey@"_Stz, &Step::vColumnIsPartitionKeyFetch);
  vAddStep("column.length@"_Stz,         &Step::vColumnLengthFetch);
  vAddStep("column.name@"_Stz,           &Step::vColumnNameFetch);
  vAddStep("column.scale@"_Stz,          &Step::vColumnScaleFetch);
  vAddStep("column.type@"_Stz,           &Step::vColumnTypeFetch);
}

/*----------------------------------------------------------------------------*/
/* column.isNullable@ -- fetch column is nullable                             */
/*----------------------------------------------------------------------------*/
void Step::vColumnIsNullableFetch(void)
{
  ENTRY();
  
  Column* pColumn = _pColumn(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sColumnIsNullable;
  sColumnIsNullable = pColumn->bIsNullable() ? "true"_Stz : "false"_Stz;
  
  m_pDataStack->vPushStr(sColumnIsNullable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.isPartitionKey@ -- fetch column is partitionKey                     */
/*----------------------------------------------------------------------------*/
void Step::vColumnIsPartitionKeyFetch(void)
{
  ENTRY();
  
  Column* pColumn = _pColumn(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sIsPartitionKey;
  sIsPartitionKey = pColumn->bIsPtnKey() ? "true"_Stz : "false"_Stz;
  
  m_pDataStack->vPushStr(sIsPartitionKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.length@ -- fetch column length                                      */
/*----------------------------------------------------------------------------*/
void Step::vColumnLengthFetch(void)
{
  ENTRY();
  
  Column* pColumn = _pColumn(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  m_pDataStack->vPushInt(pColumn->iColumnLength());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.name@ -- fetch column name                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnNameFetch(void)
{
  ENTRY();
  
  Column* pColumn = _pColumn(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sColNm(pColumn->szColNm());
  
  m_pDataStack->vPushStr(sColNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.scale@ -- fetch column scale                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnScaleFetch(void)
{
  ENTRY();
  
  Column* pColumn = _pColumn(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  m_pDataStack->vPushInt(pColumn->iColumnScale());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.type@ -- fetch column type                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnTypeFetch(void)
{
  ENTRY();
  
  Column* pColumn = _pColumn(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  m_pDataStack->vPushInt(pColumn->eColumnType());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
