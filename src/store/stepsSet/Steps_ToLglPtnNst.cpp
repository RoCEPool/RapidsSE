/*============================================================================*/
/* Steps_ToLglPtnNst.cpp - Convert row buffer to have normalised strings      */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "RowSingle.hpp"
#include "RowBuffer.hpp"

#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToLglPtnNst(void)
{
  vAddStep("toNstRow[].convert"_Stz,     &Step::vToLglPtnNstConvert);
  vAddStep("toNstRow[].create"_Stz,      &Step::vToLglPtnNstCreate);
  vAddStep("toNstRow[].delete"_Stz,      &Step::vToLglPtnNstDelete);
}

/*----------------------------------------------------------------------------*/
/* toNstRow[].convert -- convert row buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnNstConvert(void)
{
  ENTRY();

  ToLglPtnNst* pToNstRow = *m_pLogicUnit->m_xpToLglPtnNst;
  
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  pToNstRow->vConvert(pRowBuffer);
  
  m_pDataStack->vPushPtr(pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toNstRow[].create -- create toNstRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnNstCreate(void)
{
  ENTRY();
    
  ToLglPtnNst* pToNstRow = new ToLglPtnNst;
      
  m_pLogicUnit->m_xpToLglPtnNst = pToNstRow;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toNstRow[].delete -- delete toNstRow                                       */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnNstDelete(void)
{
  ENTRY();
  
  ToLglPtnNst* pToNstRow = *m_pLogicUnit->m_xpToLglPtnNst;
  
  m_pLogicUnit->m_xpToLglPtnNst = nullptr;
  
  delete pToNstRow;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
