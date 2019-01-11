/*============================================================================*/
/* Steps_Cursor.cpp - cursor steps group                                      */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vCursorFetch(MP_Task mpTask)
{
  ENTRY("vCursorFetch"_Stz);
  
  i8 iPeek = 1;
  
  GenVar xCursorVar;
  
  mpTask->m_pReturnStack->vPeekValue(iPeek,xCursorVar);
  
  mpTask->m_pDataStack->vPushGenVar(xCursorVar);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vCursorStore(MP_Task mpTask)
{
  ENTRY("vCursorStore"_Stz);
  
  i8 iPoke = 1;
  
  GenVar xCursorVar;
  if ( ! mpTask->m_pDataStack->bPop(xCursorVar) )
    THROW_INVALID_DSTACK("cursor!"_Stz);
  
  mpTask->m_pReturnStack->vPokeGenVar(iPoke,xCursorVar);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
