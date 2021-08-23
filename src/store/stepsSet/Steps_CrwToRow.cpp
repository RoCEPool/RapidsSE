/*============================================================================*/
/* Steps_CrwToRow.cpp - crw to row convertor                                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "RowBuffer.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_CrwToRow(void)
{
  vAddStep("crwToRow[].completed"_Stz,   &Step::vCrwToRowCompleted);
  vAddStep("crwToRow[].convert"_Stz,     &Step::vCrwToRowConvert);
  vAddStep("crwToRow[].create"_Stz,      &Step::vCrwToRowCreate);
  vAddStep("crwToRow[].dangling"_Stz,    &Step::vCrwToRowDangling);
  vAddStep("crwToRow[].delete"_Stz,      &Step::vCrwToRowDelete);
}

/*----------------------------------------------------------------------------*/
/* toRow[].completed -- full row buffer                                       */
/*----------------------------------------------------------------------------*/
void Step::vCrwToRowCompleted(void)
{
  ENTRY();
  
  CrwToRow* pCrwToRow = *m_p_TaskLogicUnit->m_xpCrwToRow;

  RowBuffer* pRowBuffer = pCrwToRow->pFullRowBuffer();
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].convert -- convert csv buffer                                      */
/*----------------------------------------------------------------------------*/
void Step::vCrwToRowConvert(void)
{
  ENTRY();

  CrwToRow* pCrwToRow = *m_p_TaskLogicUnit->m_xpCrwToRow;

  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_p_TaskLogicUnit->m_xpCrwBuffer);
  
  bool bResult = pCrwToRow->bConvertCrwToRow(pCrwBuffer);
  UNUSED(bResult);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].create -- create toRow                                             */
/*----------------------------------------------------------------------------*/
void Step::vCrwToRowCreate(void)
{
  ENTRY();
    
  StreamDef* pStreamDef = * m_p_TaskLogicUnit->m_xpStreamDefLft;

  CrwToRow* pCrwToRow = new CrwToRow(pStreamDef);
      
  m_p_TaskLogicUnit->m_xpCrwToRow = pCrwToRow;
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].dangling -- fetch row buffer                                       */
/*----------------------------------------------------------------------------*/
void Step::vCrwToRowDangling(void)
{
  ENTRY();
  
  if (m_p_TaskLogicUnit->m_xpCrwToRow.bIsNull()) {
    m_pDataStack->vPushPtr(nullptr);
  }
  else {
    CrwToRow* pCrwToRow = *m_p_TaskLogicUnit->m_xpCrwToRow;
    RowBuffer* pRowBuffer = pCrwToRow->pDanglingRowBuffer();
    m_pDataStack->vPushPtr(pRowBuffer);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRow[].delete -- delete toRow                                             */
/*----------------------------------------------------------------------------*/
void Step::vCrwToRowDelete(void)
{
  ENTRY();
  
  CrwToRow* pCrwToRow = *m_p_TaskLogicUnit->m_xpCrwToRow;
  m_p_TaskLogicUnit->m_xpCrwToRow = nullptr;
  
  delete pCrwToRow;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
