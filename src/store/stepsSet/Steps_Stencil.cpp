/*============================================================================*/
/* Steps_Stencil.cpp -- stencil step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Stencil(void)
{
  vAddStep("Stencil[].delete"_Stz,       &Step::vStencilDelete);
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
  
  Stencil* pStencil = * m_p_TaskLogicUnit->m_xpStencil;
  
  pStencil->vSketch();
  
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
