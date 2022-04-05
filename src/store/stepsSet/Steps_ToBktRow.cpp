/*============================================================================*/
/* Steps_ToLglPtnRow.cpp - Convert row buff to logical partitioned row  buff  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "RowBuffer.hpp"


#include "Node_I.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToLglPtnRow(void)
{
  vAddStep("toLglPtnRow[].completed"_Stz, &Step::vToLglPtnRowCompleted);
  vAddStep("toLglPtnRow[].convert"_Stz,   &Step::vToLglPtnRowConvert);
  vAddStep("toLglPtnRow[].create"_Stz,    &Step::vToLglPtnRowCreate);
  vAddStep("toLglPtnRow[].dangling"_Stz,  &Step::vToLglPtnRowDangling);
  vAddStep("toLglPtnRow[].delete"_Stz,    &Step::vToLglPtnRowDelete);
}

/*----------------------------------------------------------------------------*/
/* toLglPtnRow[].completed -- get any completed/full buffer                   */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnRowCompleted(void)
{
  ENTRY();
  
  ToLglPtnRow* pToBktRow = *m_pLogicUnit->m_xpToLglPtnRow;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RowBuffer* pRowBuffer = pToBktRow->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toLglPtnRow[].convert -- convert row buffer                                */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnRowConvert(void)
{
  ENTRY();

  ToLglPtnRow* pToBktRow = *m_pLogicUnit->m_xpToLglPtnRow;
    
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  //vTrace("Hey, I got table:%s\n"_Stz, pTable->szTableNm()->azChar());

  pToBktRow->vConvert(pTable, pRowBuffer);
  
  //delete pRowBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toLglPtnRow[].create -- create toBktRow                                    */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnRowCreate(void)
{
  ENTRY();
    
  ToLglPtnRow* pToBktRow = new ToLglPtnRow;
      
  m_pLogicUnit->m_xpToLglPtnRow = pToBktRow;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toLglPtnRow[].dangling -- get any dangling/last buffer                     */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnRowDangling(void)
{
  ENTRY();
  
  ToLglPtnRow* pToBktRow = *m_pLogicUnit->m_xpToLglPtnRow;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RowBuffer* pRowBuffer = pToBktRow->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toLglPtnRow[].delete -- delete toBktRow                                    */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnRowDelete(void)
{
  ENTRY();
  
  ToLglPtnRow* pToBktRow = *m_pLogicUnit->m_xpToLglPtnRow;
  m_pLogicUnit->m_xpToLglPtnRow = nullptr;
  
  delete pToBktRow;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
