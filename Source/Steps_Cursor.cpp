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
  ENTRY(); // ENTRY("vCursorFetch"_Stz);
  
  i8 iPeek = 1;
  
  GenVar xCursorVar;
  
  m_pReturnStack->vPeekVar(iPeek,xCursorVar);
  
  m_pDataStack->vPushVal(xCursorVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vCursorStore(void)
{
  ENTRY(); // ENTRY("vCursorStore"_Stz);
  
  i8 iPoke = 1;
  
  GenVar xCursorVar;
  if ( ! m_pDataStack->bPopVal(xCursorVar) )
    THROW_DSTACK();
  
  m_pReturnStack->vPokeVar(iPoke,xCursorVar);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
