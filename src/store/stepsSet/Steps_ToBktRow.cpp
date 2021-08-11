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

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToBktRow(void)
{
  vAddStep("toBktRow[].completed"_Stz, &Step::vToBktRowRegCompleted);
  vAddStep("toBktRow[].convert"_Stz,   &Step::vToBktRowRegConvert);
  vAddStep("toBktRow[].create"_Stz,    &Step::vToBktRowRegCreate);
  vAddStep("toBktRow[].dangling"_Stz,  &Step::vToBktRowRegDangling);
  vAddStep("toBktRow[].delete"_Stz,    &Step::vToBktRowRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toBktRow[].completed -- get any completed/full buffer                      */
/*----------------------------------------------------------------------------*/
void Step::vToBktRowRegCompleted(void)
{
  ENTRY();
  
  ToPtnRow* pToPtnRow = *m_p_TaskLogicUnit->m_xpToPtnRow;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RowBuffer* pRowBuffer = pToPtnRow->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toBktRow[].convert -- convert row buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToBktRowRegConvert(void)
{
  ENTRY();

  ToPtnRow* pToPtnRow = *m_p_TaskLogicUnit->m_xpToPtnRow;
    
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  pToPtnRow->vConvert(pRowBuffer);
  
  delete pRowBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toBktRow[].create -- create toPtnRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToBktRowRegCreate(void)
{
  ENTRY();
    
  ToPtnRow* pToPtnRow = new ToPtnRow;
      
  m_p_TaskLogicUnit->m_xpToPtnRow = pToPtnRow;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toBktRow[].dangling -- get any dangling/last buffer                        */
/*----------------------------------------------------------------------------*/
void Step::vToBktRowRegDangling(void)
{
  ENTRY();
  
  ToPtnRow* pToPtnRow = *m_p_TaskLogicUnit->m_xpToPtnRow;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RowBuffer* pRowBuffer = pToPtnRow->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toBktRow[].delete -- delete toPtnRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToBktRowRegDelete(void)
{
  ENTRY();
  
  ToPtnRow* pToPtnRow = *m_p_TaskLogicUnit->m_xpToPtnRow;
  m_p_TaskLogicUnit->m_xpToPtnRow = nullptr;
  
  delete pToPtnRow;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
