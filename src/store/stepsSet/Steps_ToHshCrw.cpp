/*============================================================================*/
/* Steps_ToHshCrw.cpp - Convert ptn nst row buffers to hashed crw buffers   */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToHshCrw(void)
{
  vAddStep("toHshCrw[].completed"_Stz, &Step::vToHshCrwRegCompleted);
  vAddStep("toHshCrw[].convert"_Stz,   &Step::vToHshCrwRegConvert);
  vAddStep("toHshCrw[].create"_Stz,    &Step::vToHshCrwRegCreate);
  vAddStep("toHshCrw[].dangling"_Stz,  &Step::vToHshCrwRegDangling);
  vAddStep("toHshCrw[].delete"_Stz,    &Step::vToHshCrwRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toHshCrw[].completed -- get any completed/full buffer                      */
/*----------------------------------------------------------------------------*/
void Step::vToHshCrwRegCompleted(void)
{
  ENTRY();
  
  ToHshCrw* pToHshCrw = *( m_pJob->pLogicUnit_job() )->m_xpToHshCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  CrwBuffer* pCrwBuffer = pToHshCrw->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toHshCrw[].convert -- convert row buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToHshCrwRegConvert(void)
{
  ENTRY();

  ToHshCrw* pToHshCrw = *( m_pJob->pLogicUnit_job() )->m_xpToHshCrw;
    
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  pToHshCrw->vConvert(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toHshCrw[].create -- create toHshCrw                                       */
/*----------------------------------------------------------------------------*/
void Step::vToHshCrwRegCreate(void)
{
  ENTRY();
    
  ToHshCrw* pToHshCrw = new ToHshCrw(* ( m_pJob->pLogicUnit_job() )->m_xpStreamDef);
      
  ( m_pJob->pLogicUnit_job() )->m_xpToHshCrw = pToHshCrw;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toHshCrw[].dangling -- get any dangling/last buffer                        */
/*----------------------------------------------------------------------------*/
void Step::vToHshCrwRegDangling(void)
{
  ENTRY();
  
  ToHshCrw* pToHshCrw = *( m_pJob->pLogicUnit_job() )->m_xpToHshCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  CrwBuffer* pCrwBuffer = pToHshCrw->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toHshCrw[].delete -- delete toHshCrw                                       */
/*----------------------------------------------------------------------------*/
void Step::vToHshCrwRegDelete(void)
{
  ENTRY();
  
  ToHshCrw* pToHshCrw = *( m_pJob->pLogicUnit_job() )->m_xpToHshCrw;
  ( m_pJob->pLogicUnit_job() )->m_xpToHshCrw = nullptr;
  
  delete pToHshCrw;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
