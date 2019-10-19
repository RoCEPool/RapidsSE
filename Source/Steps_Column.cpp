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
void Step::vColumnIsNullableFetch(Forth* pForth)
{
  ENTRY("vColumnIsNullableFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrColumn) )
  THROW_DSTACK();
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sColumnIsNullable;
  sColumnIsNullable = pColumn->bIsNullable() ? "true"_Stz : "false"_Stz;
  
  pForth->m_pDataStack->vPushS32(sColumnIsNullable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.isPartitionKey@ -- fetch column is partitionKey                     */
/*----------------------------------------------------------------------------*/
void Step::vColumnIsPartitionKeyFetch(Forth* pForth)
{
  ENTRY("vColumnIsPartitionKeyFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrColumn) )
  THROW_DSTACK();
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sIsPartitionKey;
  sIsPartitionKey = pColumn->bIsPtnKey() ? "true"_Stz : "false"_Stz;
  
  pForth->m_pDataStack->vPushS32(sIsPartitionKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.length@ -- fetch column length                                      */
/*----------------------------------------------------------------------------*/
void Step::vColumnLengthFetch(Forth* pForth)
{
  ENTRY("vColumnLengthFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrColumn) )
  THROW_DSTACK();
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  pForth->m_pDataStack->vPushI8(pColumn->iColumnLength());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.name@ -- fetch column name                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnNameFetch(Forth* pForth)
{
  ENTRY("vColumnNameFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrColumn) )
  THROW_DSTACK();
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  pForth->m_pDataStack->vPushS32(pColumn->sColumnName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.scale@ -- fetch column scale                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnScaleFetch(Forth* pForth)
{
  ENTRY("vColumnScaleFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrColumn) )
  THROW_DSTACK();
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  pForth->m_pDataStack->vPushI8(pColumn->iColumnScale());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.type@ -- fetch column type                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnTypeFetch(Forth* pForth)
{
  ENTRY("vColumnTypeFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrColumn) )
  THROW_DSTACK();
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sColumnType;
  sColumnType = aCT[pColumn->eColumnType()];
  
  pForth->m_pDataStack->vPushS32(sColumnType);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
