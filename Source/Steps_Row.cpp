/*============================================================================*/
/* Steps_Row.cpp - row step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// index: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Row(void)
{
  vAddStep("row.asString"_Stz,         &Task::vRowString);
  vAddStep("row.asEscapedString"_Stz,  &Task::vRowEscapedString);
}

/*----------------------------------------------------------------------------*/
/* return row as a string                                                     */
/*----------------------------------------------------------------------------*/
void Task::vRowString(MP_Task mpTask)
{
  ENTRY("vRowString"_Stz);
  
  ASSERT(0);
  
  /*
  Object* pPtrRow;
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRow) )
    THROW_INVALID_DSTACK("row.asString"_Stz);
  
  Entry* pRow = _pEntry(pPtrRow);
  ASSERT(CN_ENTRY == pRow->eClass());
         
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
     THROW_INVALID_DSTACK("row.asString"_Stz);
 
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s48K sRow;
  Row* aRow = pRow->aRow();
  aRow->vAsBinaryString(sRow,pTable->pColumnVec());
  
  mpTask->m_pDataStack->vPushS48K(sRow);
  */
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rows.asEscapedString return row as an escaped string                       */
/*----------------------------------------------------------------------------*/
void Task::vRowEscapedString(MP_Task mpTask)
{
  ENTRY("vRowEscapedString"_Stz);
  
  Object* pPtrRow;
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRow) )
    THROW_INVALID_DSTACK("rows.asEscapedString"_Stz);
  
  Entry* pRow = _pEntry(pPtrRow);
  ASSERT(CN_ENTRY == pRow->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("rows.asEscapedString"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s48K sRow;
  Row* aRow = pRow->aRow();
  aRow->vAsEscapedString(sRow,pTable->pColumnVec());

  mpTask->m_pDataStack->vPushS48K(sRow);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
