/*============================================================================*/
/* Steps_ToBktRow.cpp - Convert row buffers to partitioned row slab buffers   */
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
void Step::vAddSteps_ToBktRow(void)
{
  vAddStep("toLglPtnRow[].completed"_Stz, &Step::vToBktRowRegCompleted);
  vAddStep("toLglPtnRow[].convert"_Stz,   &Step::vToBktRowRegConvert);
  vAddStep("toLglPtnRow[].create"_Stz,    &Step::vToBktRowRegCreate);
  vAddStep("toLglPtnRow[].dangling"_Stz,  &Step::vToBktRowRegDangling);
  vAddStep("toLglPtnRow[].delete"_Stz,    &Step::vToBktRowRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toLglPtnRow[].completed -- get any completed/full buffer                      */
/*----------------------------------------------------------------------------*/
void Step::vToBktRowRegCompleted(void)
{
  ENTRY();
  
  ToBktRow* pToBktRow = *m_pLogicUnit->m_xpToBktRow;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RowBuffer* pRowBuffer = pToBktRow->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toLglPtnRow[].convert -- convert row buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToBktRowRegConvert(void)
{
  ENTRY();

  ToBktRow* pToBktRow = *m_pLogicUnit->m_xpToBktRow;
    
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  //vTrace("Hey, I got table:%s\n"_Stz, pTable->szTableNm()->azChar());

  pToBktRow->vConvert(pTable, pRowBuffer);
  
  //delete pRowBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toLglPtnRow[].create -- create toBktRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToBktRowRegCreate(void)
{
  ENTRY();
    
  ToBktRow* pToBktRow = new ToBktRow;
      
  m_pLogicUnit->m_xpToBktRow = pToBktRow;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toLglPtnRow[].dangling -- get any dangling/last buffer                        */
/*----------------------------------------------------------------------------*/
void Step::vToBktRowRegDangling(void)
{
  ENTRY();
  
  ToBktRow* pToBktRow = *m_pLogicUnit->m_xpToBktRow;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RowBuffer* pRowBuffer = pToBktRow->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toLglPtnRow[].delete -- delete toBktRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToBktRowRegDelete(void)
{
  ENTRY();
  
  ToBktRow* pToBktRow = *m_pLogicUnit->m_xpToBktRow;
  m_pLogicUnit->m_xpToBktRow = nullptr;
  
  delete pToBktRow;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
