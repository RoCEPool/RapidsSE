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
  
  GenVar xCursorWsp;
  
  mpTask->m_pReturnStack->vPeekValWrk(iPeek,xCursorWsp);
  
  mpTask->m_pDataStack->vPushValWrk(xCursorWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vCursorStore(MP_Task mpTask)
{
  ENTRY("vCursorStore"_Stz);
  
  i8 iPoke = 1;
  
  GenVar xCursorWsp;
  
  if ( ! mpTask->m_pDataStack->bPop(xCursorWsp) )
    THROW_INVALID_DSTACK("cursor!"_Stz);
  
  mpTask->m_pReturnStack->vPokeValWrk(iPoke,xCursorWsp);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
