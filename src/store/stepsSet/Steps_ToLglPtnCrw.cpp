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
void Step::vAddSteps_ToBktCrw(void)
{
  vAddStep("ToLglPtnCrw[].completed"_Stz, &Step::vToLglPtnCrwRegCompleted);
  vAddStep("ToLglPtnCrw[].convert"_Stz,   &Step::vToLglPtnCrwRegConvert);
  vAddStep("ToLglPtnCrw[].create"_Stz,    &Step::vToLglPtnCrwRegCreate);
  vAddStep("ToLglPtnCrw[].dangling"_Stz,  &Step::vToLglPtnCrwRegDangling);
  vAddStep("ToLglPtnCrw[].delete"_Stz,    &Step::vToLglPtnCrwRegDelete);
}

/*----------------------------------------------------------------------------*/
/* ToLglPtnCrw[].completed -- get any completed/full buffer                   */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnCrwRegCompleted(void)
{
  ENTRY();
  
  ToBktCrw* pToBktCrw = *m_pLogicUnit->m_xpToBktCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  CrwBuffer* pCrwBuffer = pToBktCrw->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToLglPtnCrw[].convert -- convert row buffer                                */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnCrwRegConvert(void)
{
  ENTRY();

  ToBktCrw* pToBktCrw = *m_pLogicUnit->m_xpToBktCrw;
    
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  pToBktCrw->vConvert(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToLglPtnCrw[].create -- create toHshCrw                                    */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnCrwRegCreate(void)
{
  ENTRY();
    
  ToBktCrw* pToBktCrw = new ToBktCrw(* m_pLogicUnit->m_xpStreamDefLft);
      
  m_pLogicUnit->m_xpToBktCrw = pToBktCrw;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToLglPtnCrw[].dangling -- get any dangling/last buffer                     */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnCrwRegDangling(void)
{
  ENTRY();
  
  ToBktCrw* pToBktCrw = *m_pLogicUnit->m_xpToBktCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  CrwBuffer* pCrwBuffer = pToBktCrw->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ToLglPtnCrw[].delete -- delete toHshCrw                                    */
/*----------------------------------------------------------------------------*/
void Step::vToLglPtnCrwRegDelete(void)
{
  ENTRY();
  
  ToBktCrw* pToBktCrw = *m_pLogicUnit->m_xpToBktCrw;
  m_pLogicUnit->m_xpToBktCrw = nullptr;
  
  delete pToBktCrw;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
