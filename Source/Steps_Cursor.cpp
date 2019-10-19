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
  ENTRY("vCursorFetch"_Stz);
  
  i8 iPeek = 1;
  
  GenVar xCursorVar;
  
  m_pReturnStack->vPeekValue(iPeek,xCursorVar);
  
  m_pDataStack->vPushGenVar(xCursorVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vCursorStore(void)
{
  ENTRY("vCursorStore"_Stz);
  
  i8 iPoke = 1;
  
  GenVar xCursorVar;
  if ( ! m_pDataStack->bPop(xCursorVar) )
    THROW_DSTACK();
  
  m_pReturnStack->vPokeGenVar(iPoke,xCursorVar);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
