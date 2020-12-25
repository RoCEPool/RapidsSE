/*============================================================================*/
/* Steps_Cursor.cpp - cursor steps group                                      */
/*============================================================================*/

#include "Expressions.h"

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
  
  m_pReturnStack->vPeekItem(iPeek,xCursorVar);
  
  m_pDataStack->vPushItem(xCursorVar);
  
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
  m_pDataStack->vPopItem(xCursorVar,__func__);
  
  m_pReturnStack->vPokeItem(iPoke,xCursorVar);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
