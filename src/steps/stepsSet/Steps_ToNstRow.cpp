/*============================================================================*/
/* Steps_ToNstRow.cpp - Convert row to normalised string row buffers          */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToNstRow(void)
{
  vAddStep("toNstRow[].convert"_Stz,     &Step::vToNstRowRegConvert);
  vAddStep("toNstRow[].create"_Stz,      &Step::vToNstRowRegCreate);
  vAddStep("toNstRow[].delete"_Stz,      &Step::vToNstRowRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toNstRow[].convert -- convert row buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToNstRowRegConvert(void)
{
  ENTRY();

  ToNstRow* pToNstRow = *m_pLogicUnit->m_xpToNstRow;
  
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  pToNstRow->vConvert(pRowBuffer);
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toNstRow[].create -- create toNstRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToNstRowRegCreate(void)
{
  ENTRY();
    
  ToNstRow* pToNstRow = new ToNstRow;
      
  m_pLogicUnit->m_xpToNstRow = pToNstRow;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toNstRow[].delete -- delete toNstRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToNstRowRegDelete(void)
{
  ENTRY();
  
  ToNstRow* pToNstRow = *m_pLogicUnit->m_xpToNstRow;
  
  m_pLogicUnit->m_xpToNstRow = nullptr;
  
  delete pToNstRow;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
