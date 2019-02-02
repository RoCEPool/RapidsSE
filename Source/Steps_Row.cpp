/*============================================================================*/
/* Steps_Row.cpp - row step group                                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Row(void)
{
  vAddStep("row.asEscapedString"_Stz,  &Step::vRowEscapedString);
}

/*----------------------------------------------------------------------------*/
/* rows.asEscapedString return row as an escaped string                       */
/*----------------------------------------------------------------------------*/
void Step::vRowEscapedString(MP_Task mpTask)
{
  ENTRY("vRowEscapedString"_Stz);
  
  Object* pPtrRow;
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRow) )
    THROW_INVALID_DSTACK("rows.asEscapedString"_Stz);
  
  Item* aEntry = (Item*)_aItem(pPtrRow);
  ASSERT(CN_ENTRY == aEntry->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("rows.asEscapedString"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s48K sRow;
  RowData* aEmbeddedRow = _aRowData(aEntry->aDataArea());
  aEmbeddedRow->vAsEscapedString(sRow,pTable->pColumnVec());

  mpTask->m_pDataStack->vPushS48K(sRow);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
