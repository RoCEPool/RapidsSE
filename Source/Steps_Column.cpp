/*============================================================================*/
/* Steps_Column.cpp - column step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_Column(void)
{
  vAddStep("column.isNullable@"_Stz,     &vColumnIsNullableFetch,   false);
  vAddStep("column.isPartitionKey@"_Stz, &vColumnIsPartitionKeyFetch, false);
  vAddStep("column.length@"_Stz,         &vColumnLengthFetch,       false);
  vAddStep("column.name@"_Stz,           &vColumnNameFetch,         false);
  vAddStep("column.scale@"_Stz,          &vColumnScaleFetch,        false);
  vAddStep("column.type@"_Stz,           &vColumnTypeFetch,         false);
}

/*----------------------------------------------------------------------------*/
/* column.isNullable@ -- fetch column is nullable                             */
/*----------------------------------------------------------------------------*/
void Task::vColumnIsNullableFetch(MP_Task mpTask)
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
void Task::vColumnIsPartitionKeyFetch(MP_Task mpTask)
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
void Task::vColumnLengthFetch(MP_Task mpTask)
{
  ENTRY("vColumnLengthFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrColumn) )
  THROW_INVALID_DSTACK("column.length"_Stz);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  mpTask->m_pDataStack->vPushInt(pColumn->iColumnLength());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.name@ -- fetch column name                                          */
/*----------------------------------------------------------------------------*/
void Task::vColumnNameFetch(MP_Task mpTask)
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
void Task::vColumnScaleFetch(MP_Task mpTask)
{
  ENTRY("vColumnScaleFetch"_Stz);
  
  Object* pPtrColumn;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrColumn) )
  THROW_INVALID_DSTACK("column.scale"_Stz);
  
  Column* pColumn = _pColumn(pPtrColumn);
  ASSERT(CN_COLUMN == pColumn->eClass());
  
  mpTask->m_pDataStack->vPushInt(pColumn->iColumnScale());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* column.type@ -- fetch column type                                          */
/*----------------------------------------------------------------------------*/
void Task::vColumnTypeFetch(MP_Task mpTask)
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
