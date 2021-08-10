/*============================================================================*/
/* Steps_ToPtnCrw.cpp - Convert ptn nst row buffers to hashed crw buffers     */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "RowBuffer.hpp"

#include "EntWrk.hpp"

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
void Step::vAddSteps_ToHshCrw(void)
{
  vAddStep("toPtnCrw[].completed"_Stz, &Step::vToPtnCrwRegCompleted);
  vAddStep("toPtnCrw[].convert"_Stz,   &Step::vToPtnCrwRegConvert);
  vAddStep("toPtnCrw[].create"_Stz,    &Step::vToPtnCrwRegCreate);
  vAddStep("toPtnCrw[].dangling"_Stz,  &Step::vToPtnCrwRegDangling);
  vAddStep("toPtnCrw[].delete"_Stz,    &Step::vToPtnCrwRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toPtnCrw[].completed -- get any completed/full buffer                      */
/*----------------------------------------------------------------------------*/
void Step::vToPtnCrwRegCompleted(void)
{
  ENTRY();
  
  ToPtnCrw* pToHshCrw = *m_p_TaskLogicUnit->m_xpToHshCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  CrwBuffer* pCrwBuffer = pToHshCrw->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnCrw[].convert -- convert row buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToPtnCrwRegConvert(void)
{
  ENTRY();

  ToPtnCrw* pToHshCrw = *m_p_TaskLogicUnit->m_xpToHshCrw;
    
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  pToHshCrw->vConvert(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnCrw[].create -- create toHshCrw                                       */
/*----------------------------------------------------------------------------*/
void Step::vToPtnCrwRegCreate(void)
{
  ENTRY();
    
  ToPtnCrw* pToHshCrw = new ToPtnCrw(* m_p_TaskLogicUnit->m_xpStreamDefLft);
      
  m_p_TaskLogicUnit->m_xpToHshCrw = pToHshCrw;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnCrw[].dangling -- get any dangling/last buffer                        */
/*----------------------------------------------------------------------------*/
void Step::vToPtnCrwRegDangling(void)
{
  ENTRY();
  
  ToPtnCrw* pToHshCrw = *m_p_TaskLogicUnit->m_xpToHshCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  CrwBuffer* pCrwBuffer = pToHshCrw->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnCrw[].delete -- delete toHshCrw                                       */
/*----------------------------------------------------------------------------*/
void Step::vToPtnCrwRegDelete(void)
{
  ENTRY();
  
  ToPtnCrw* pToHshCrw = *m_p_TaskLogicUnit->m_xpToHshCrw;
  m_p_TaskLogicUnit->m_xpToHshCrw = nullptr;
  
  delete pToHshCrw;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
