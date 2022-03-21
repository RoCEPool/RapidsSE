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
void Step::vAddSteps_ToPtnCrw(void)
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
  
  ToPtnCrw* pToPtnCrw = *m_pLogicUnit->m_xpToPtnCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  CrwBuffer* pCrwBuffer = pToPtnCrw->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnCrw[].convert -- convert row buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToPtnCrwRegConvert(void)
{
  ENTRY();

  ToPtnCrw* pToPtnCrw = *m_pLogicUnit->m_xpToPtnCrw;
    
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  pToPtnCrw->vConvert(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnCrw[].create -- create toHshCrw                                       */
/*----------------------------------------------------------------------------*/
void Step::vToPtnCrwRegCreate(void)
{
  ENTRY();
    
  ToPtnCrw* pToPtnCrw = new ToPtnCrw(* m_pLogicUnit->m_xpStreamDefLft);
      
  m_pLogicUnit->m_xpToPtnCrw = pToPtnCrw;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnCrw[].dangling -- get any dangling/last buffer                        */
/*----------------------------------------------------------------------------*/
void Step::vToPtnCrwRegDangling(void)
{
  ENTRY();
  
  ToPtnCrw* pToPtnCrw = *m_pLogicUnit->m_xpToPtnCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  CrwBuffer* pCrwBuffer = pToPtnCrw->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnCrw[].delete -- delete toHshCrw                                       */
/*----------------------------------------------------------------------------*/
void Step::vToPtnCrwRegDelete(void)
{
  ENTRY();
  
  ToPtnCrw* pToPtnCrw = *m_pLogicUnit->m_xpToPtnCrw;
  m_pLogicUnit->m_xpToPtnCrw = nullptr;
  
  delete pToPtnCrw;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
