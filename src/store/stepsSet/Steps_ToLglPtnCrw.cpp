/*============================================================================*/
/* Steps_ToLglPtnCrw.cpp - Convert lglptn nst row buff to lglptn crw buff     */
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
void Step::vAddSteps_ToLglPtnCrw(void)
{
  vAddStep("ToLglPtnCrw[].completed"_Stz, &Step::vToLglPtnCrwCompleted);
  vAddStep("ToLglPtnCrw[].convert"_Stz,   &Step::vToLglPtnCrwConvert);
  vAddStep("ToLglPtnCrw[].create"_Stz,    &Step::vToLglPtnCrwCreate);
  vAddStep("ToLglPtnCrw[].dangling"_Stz,  &Step::vToLglPtnCrwDangling);
  vAddStep("ToLglPtnCrw[].delete"_Stz,    &Step::vToLglPtnCrwDelete);
}

/*----------------------------------------------------------------------------*/
/* ToLglPtnCrw[].completed -- get any completed/full buffer                   */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnCrwCompleted(void)
{
  ENTRY();
  
  ToLglPtnCrw* pToBktCrw = *m_pLogicUnit->m_xpToLglPtnCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  CrwBuffer* pCrwBuffer = pToBktCrw->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToLglPtnCrw[].convert -- convert row buffer                                */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnCrwConvert(void)
{
  ENTRY();

  ToLglPtnCrw* pToBktCrw = *m_pLogicUnit->m_xpToLglPtnCrw;
    
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  pToBktCrw->vConvert(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToLglPtnCrw[].create -- create toHshCrw                                    */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnCrwCreate(void)
{
  ENTRY();
    
  ToLglPtnCrw* pToBktCrw = new ToLglPtnCrw(* m_pLogicUnit->m_xpStreamDefLft);
      
  m_pLogicUnit->m_xpToLglPtnCrw = pToBktCrw;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToLglPtnCrw[].dangling -- get any dangling/last buffer                     */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnCrwDangling(void)
{
  ENTRY();
  
  ToLglPtnCrw* pToBktCrw = *m_pLogicUnit->m_xpToLglPtnCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  CrwBuffer* pCrwBuffer = pToBktCrw->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToLglPtnCrw[].delete -- delete toHshCrw                                    */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnCrwDelete(void)
{
  ENTRY();
  
  ToLglPtnCrw* pToBktCrw = *m_pLogicUnit->m_xpToLglPtnCrw;
  m_pLogicUnit->m_xpToLglPtnCrw = nullptr;
  
  delete pToBktCrw;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
