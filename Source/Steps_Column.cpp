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
void Step::vColumnIsNullableFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vColumnIsNullableFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrColumn) )
  vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sColumnIsNullable;
  sColumnIsNullable = pColumn->bIsNullable() ? "true"_Stz : "false"_Stz;
  
  mpTask->m_pForth->m_pDataStack->vPushS32(sColumnIsNullable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.isPartitionKey@ -- fetch column is partitionKey                     */
/*----------------------------------------------------------------------------*/
void Step::vColumnIsPartitionKeyFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vColumnIsPartitionKeyFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrColumn) )
  vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sIsPartitionKey;
  sIsPartitionKey = pColumn->bIsPtnKey() ? "true"_Stz : "false"_Stz;
  
  mpTask->m_pForth->m_pDataStack->vPushS32(sIsPartitionKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.length@ -- fetch column length                                      */
/*----------------------------------------------------------------------------*/
void Step::vColumnLengthFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vColumnLengthFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrColumn) )
  vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  mpTask->m_pForth->m_pDataStack->vPushI8(pColumn->iColumnLength());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.name@ -- fetch column name                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnNameFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vColumnNameFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrColumn) )
  vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  mpTask->m_pForth->m_pDataStack->vPushS32(pColumn->sColumnName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.scale@ -- fetch column scale                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnScaleFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vColumnScaleFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrColumn) )
  vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  mpTask->m_pForth->m_pDataStack->vPushI8(pColumn->iColumnScale());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.type@ -- fetch column type                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnTypeFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vColumnTypeFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrColumn) )
  vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sColumnType;
  sColumnType = aCT[pColumn->eColumnType()];
  
  mpTask->m_pForth->m_pDataStack->vPushS32(sColumnType);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
