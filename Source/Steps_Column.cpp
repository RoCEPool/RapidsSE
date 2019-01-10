/*============================================================================*/
/* Steps_Column.cpp - column step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vColumnIsNullableFetch(MP_Task mpTask)
{
  ENTRY("vColumnIsNullableFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrColumn) )
  THROW_INVALID_DSTACK("column.isNullable"_Stz);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sColumnIsNullable;
  sColumnIsNullable = pColumn->bIsNullable() ? "true"_Stz : "false"_Stz;
  
  mpTask->m_pDataStack->vPushS32(sColumnIsNullable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.isPartitionKey@ -- fetch column is partitionKey                     */
/*----------------------------------------------------------------------------*/
void Step::vColumnIsPartitionKeyFetch(MP_Task mpTask)
{
  ENTRY("vColumnIsPartitionKeyFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrColumn) )
  THROW_INVALID_DSTACK("column.isPartitionKey"_Stz);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sIsPartitionKey;
  sIsPartitionKey = pColumn->bIsPartitionKey() ? "true"_Stz : "false"_Stz;
  
  mpTask->m_pDataStack->vPushS32(sIsPartitionKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.length@ -- fetch column length                                      */
/*----------------------------------------------------------------------------*/
void Step::vColumnLengthFetch(MP_Task mpTask)
{
  ENTRY("vColumnLengthFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrColumn) )
  THROW_INVALID_DSTACK("column.length"_Stz);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  mpTask->m_pDataStack->vPushI8(pColumn->iColumnLength());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.name@ -- fetch column name                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnNameFetch(MP_Task mpTask)
{
  ENTRY("vColumnNameFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrColumn) )
  THROW_INVALID_DSTACK("column.name"_Stz);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  mpTask->m_pDataStack->vPushS32(pColumn->sColumnName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.scale@ -- fetch column scale                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnScaleFetch(MP_Task mpTask)
{
  ENTRY("vColumnScaleFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrColumn) )
  THROW_INVALID_DSTACK("column.scale"_Stz);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  mpTask->m_pDataStack->vPushI8(pColumn->iColumnScale());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.type@ -- fetch column type                                          */
/*----------------------------------------------------------------------------*/
void Step::vColumnTypeFetch(MP_Task mpTask)
{
  ENTRY("vColumnTypeFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrColumn) )
  THROW_INVALID_DSTACK("column.type"_Stz);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  s32 sColumnType;
  sColumnType = aCT[pColumn->eColumnType()];
  
  mpTask->m_pDataStack->vPushS32(sColumnType);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
