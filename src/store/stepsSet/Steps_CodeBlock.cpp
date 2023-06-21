/*============================================================================*/
/* Steps_CodeBlock.cpp - code block step group                                */
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
void Step::vAddSteps_CodeBlock(void)
{

  vAddStep("CodeBlock[]"_Stz,        &Step::vCodeBlockGet);
  vAddStep("CodeBlock[].get"_Stz,    &Step::vCodeBlockGet);
  vAddStep("CodeBlock[].set"_Stz,    &Step::vCodeBlockSet);
}

/*----------------------------------------------------------------------------*/
/* CodeBlock[].get -- get CodeBlock[]                                         */
/*----------------------------------------------------------------------------*/
void Step::vCodeBlockGet(void)
{
  ENTRY();
    
//  RASSERT(m_pLogicUnit->m_xpText.bIsNotNull());
//
//  m_pDataStack->vPushStz(m_pLogicUnit->m_xpText->szGet());


  m_pDataStack->vPushPtr(nullptr);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CodeBlock[].set -- set CodeBlock[]                                         */
/*----------------------------------------------------------------------------*/
void Step::vCodeBlockSet(void)
{
  ENTRY();
    
  if (m_pLogicUnit->m_xpText.bIsNull())
    m_pLogicUnit->m_xpText = new Text(C_32KB);
    
  m_pDataStack->vPopStz(m_pLogicUnit->m_xpText->szGet(), __func__);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
