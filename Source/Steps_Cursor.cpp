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
void Step::vCursorFetch(Forth* pForth)
{
  ENTRY("vCursorFetch"_Stz);
  
  i8 iPeek = 1;
  
  GenVar xCursorVar;
  
  pForth->m_pReturnStack->vPeekValue(iPeek,xCursorVar);
  
  pForth->m_pDataStack->vPushGenVar(xCursorVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vCursorStore(Forth* pForth)
{
  ENTRY("vCursorStore"_Stz);
  
  i8 iPoke = 1;
  
  GenVar xCursorVar;
  if ( ! pForth->m_pDataStack->bPop(xCursorVar) )
    THROW_DSTACK();
  
  pForth->m_pReturnStack->vPokeGenVar(iPoke,xCursorVar);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
