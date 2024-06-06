/*============================================================================*/
/* Steps_Cursor.cpp - cursor steps group                                      */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Cursor(void)
{
  vAddStep("cursor@"_Stz,         &Step::vCursorFetch);
  vAddStep("cursor!"_Stz,         &Step::vCursorStore);
}

/*----------------------------------------------------------------------------*/
/* cursor@ -- fetch cursor from return stack onto data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vCursorFetch(void)
{
  ENTRY();
  
  i8 iPeek = 1;
  
  SenCell xCursorVar;
  
  m_oReturnStack.vPeekTypeValueArea(iPeek, xCursorVar);
  
  m_oDataStack.vPushSenCell(xCursorVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vCursorStore(void)
{
  ENTRY();
  
  i8 iPoke = 1;
  
  SenCell xCursorVar;
  m_oDataStack.vPopSenCell(xCursorVar, __func__);
  
  m_oReturnStack.vPokeItem(iPoke, xCursorVar);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
