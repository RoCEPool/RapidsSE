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
void Task::vAddPrimitives_Row(void)
{
  vAddPrimitive("row.asString"_Stz,  &vRowString, false);
  vAddPrimitive("row.asEscapedString"_Stz,  &vRowString, false);
}

/*----------------------------------------------------------------------------*/
/* return row as a string                                                     */
/*----------------------------------------------------------------------------*/
void Task::vRowString(MP_Task mpTask)
{
  ENTRY("vRowString"_Stz);
  
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
  RowData::vRowAsString(sRow, (Stz*)pRow->aRow(), pTable->pColumnVec());
  
  mpTask->m_pDataStack->vPushS48K(sRow);
  
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
  //RowData::vRowAsEscapedString(sRow, (Stz*)pRow->aRow(), pTable->pColumnVec());
  
  mpTask->m_pDataStack->vPushS48K(sRow);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
