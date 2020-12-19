/*============================================================================*/
/* Steps_Cursor.cpp - cursor steps group                                      */
/*============================================================================*/

#include "Steps.h"

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
  
  CellSingle xCursorVar;
  
  m_pReturnStack->vPeekVal(iPeek,xCursorVar);
  
  m_pDataStack->vPushVal(xCursorVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vCursorStore(void)
{
  ENTRY();
  
  i8 iPoke = 1;
  
  CellSingle xCursorVar;
  m_pDataStack->vPopVal(xCursorVar,__func__);
  
  m_pReturnStack->vPokeVal(iPoke,xCursorVar);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
