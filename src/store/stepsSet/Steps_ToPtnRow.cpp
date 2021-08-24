/*============================================================================*/
/* Steps_ToPtnRow.cpp - Convert row buffers to partitioned row slab buffers   */
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
void Step::vAddSteps_ToPtnRow(void)
{
  vAddStep("toPtnRow[].completed"_Stz, &Step::vToPtnRowRegCompleted);
  vAddStep("toPtnRow[].convert"_Stz,   &Step::vToPtnRowRegConvert);
  vAddStep("toPtnRow[].create"_Stz,    &Step::vToPtnRowRegCreate);
  vAddStep("toPtnRow[].dangling"_Stz,  &Step::vToPtnRowRegDangling);
  vAddStep("toPtnRow[].delete"_Stz,    &Step::vToPtnRowRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toPtnRow[].completed -- get any completed/full buffer                      */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRowRegCompleted(void)
{
  ENTRY();
  
  ToPtnRow* pToPtnRow = *m_p_TaskLogicUnit->m_xpToPtnRow;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RowBuffer* pRowBuffer = pToPtnRow->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnRow[].convert -- convert row buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRowRegConvert(void)
{
  ENTRY();

  ToPtnRow* pToPtnRow = *m_p_TaskLogicUnit->m_xpToPtnRow;
    
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  pToPtnRow->vConvert(pRowBuffer);
  
  //delete pRowBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnRow[].create -- create toPtnRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRowRegCreate(void)
{
  ENTRY();
    
  ToPtnRow* pToPtnRow = new ToPtnRow;
      
  m_p_TaskLogicUnit->m_xpToPtnRow = pToPtnRow;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnRow[].dangling -- get any dangling/last buffer                        */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRowRegDangling(void)
{
  ENTRY();
  
  ToPtnRow* pToPtnRow = *m_p_TaskLogicUnit->m_xpToPtnRow;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RowBuffer* pRowBuffer = pToPtnRow->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnRow[].delete -- delete toPtnRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRowRegDelete(void)
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
