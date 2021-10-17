/*============================================================================*/
/* Steps_Stencil.cpp -- stencil step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Op.hpp"
#include "Stream.hpp"

#include "LogicUnit.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Stencil(void)
{
  vAddStep("Stencil[].delete"_Stz,       &Step::vStencilDelete);
  vAddStep("Stencil[].execute"_Stz,      &Step::vStencilExecute);
  vAddStep("Stencil[].new"_Stz,          &Step::vStencilNew);
  vAddStep("Stencil[].sketch"_Stz,       &Step::vStencilSketch);
  vAddStep("Stencil[].source@"_Stz,      &Step::vStencilSourceFetch);
}

/*----------------------------------------------------------------------------*/
/* Stencil[].delete -- delete stencil                                         */
/*----------------------------------------------------------------------------*/
void Step::vStencilDelete(void)
{
  ENTRY();
  
  //vTrace("vStencilDelete\n"_Stz);
  
  delete * m_p_TaskLogicUnit->m_xpStencil;

  m_p_TaskLogicUnit->m_xpStencil = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil[].execute -- execute the stencil                                   */
/*----------------------------------------------------------------------------*/
void Step::vStencilExecute(void)
{
  ENTRY();

  //vTrace("vStencilExecute\n"_Stz);

  i8 iTrace = m_pDataStack->iPopInt(__func__);

  bool (*afFun)(void*);
  afFun = (bool (*)(void*))m_pDataStack->iPopInt(__func__);

  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  afFun(pCrwBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil[].new -- new stencil                                               */
/*----------------------------------------------------------------------------*/
void Step::vStencilNew(void)
{
  ENTRY();
  
  //vTrace("vStencilNew\n"_Stz);
  
  m_p_TaskLogicUnit->m_xpStencil = new Stencil;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil[].sketch -- use stencil to sketch                                  */
/*----------------------------------------------------------------------------*/
void Step::vStencilSketch(void)
{
  ENTRY();
  
  Stencil* pStencil   = * m_p_TaskLogicUnit->m_xpStencil;

  // get the current stream
  Op* pOp = _pOp(* m_p_TaskLogicUnit->m_xpOp);
  Stream* pStream = _pStream(pOp->pStream());

  pStencil->vSketch(pStream);
  pStencil->vCombine();
  pStencil->vPrint();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil[].source@ -- get sketched source                                   */
/*----------------------------------------------------------------------------*/
void Step::vStencilSourceFetch(void)
{
  ENTRY();

  Stencil* pStencil = * m_p_TaskLogicUnit->m_xpStencil;
  
  Stz* szSource = pStencil->szOpSource();

  m_pDataStack->vPushStz(szSource);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
