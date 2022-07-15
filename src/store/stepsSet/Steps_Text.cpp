/*============================================================================*/
/* Steps_Text.cpp - text step group                                           */
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
void Step::vAddSteps_Text(void)
{
  vAddStep("text::delete"_Stz,  &Step::vTextDelete);
  vAddStep("text::new"_Stz,     &Step::vTextNew);
  
  vAddStep("text[].get"_Stz,    &Step::vTextGet);
  vAddStep("text[].set"_Stz,    &Step::vTextSet);
}

/*----------------------------------------------------------------------------*/
/* text::delete -- delete an instance of text                                 */
/*----------------------------------------------------------------------------*/
void Step::vTextDelete(void)
{
  ENTRY();
  
  Text* pText = _pText(m_pDataStack->pPopPtr(__func__));

  delete pText;
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* text::new -- create a new instance of text                                 */
/*----------------------------------------------------------------------------*/
void Step::vTextNew(void)
{
  ENTRY();

  Text* pText = new Text(C_SIZE_LARGE_TEXT);
  
  m_pDataStack->vPushPtr(pText);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* text[].get -- get text[]                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTextGet(void)
{
  ENTRY();
    
  RASSERT(m_pLogicUnit->m_xpText.bIsNotNull());
  
  m_pDataStack->vPushStz(m_pLogicUnit->m_xpText->szGet());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* text[].set -- set text[]                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTextSet(void)
{
  ENTRY();
    
  if (m_pLogicUnit->m_xpText.bIsNull())
    m_pLogicUnit->m_xpText = new Text(C_SIZE_MEDIUM_TEXT);
    
  m_pDataStack->vPopStz(m_pLogicUnit->m_xpText->szGet(), __func__);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
