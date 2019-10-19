/*============================================================================*/
/* Steps_Error.cpp - error step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Error(void)
{
  vAddStep("error.msg"_Stz,       &Step::vErrorMsg);
}

/*----------------------------------------------------------------------------*/
/* error.msg -- get error message given the error code                        */
/*----------------------------------------------------------------------------*/
void Step::vErrorMsg(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vErrorMsg"_Stz);
  
  
  i8 iErrorCode;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iErrorCode) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  if (iErrorCode < EC_NONE || iErrorCode > EC_ENUMS)
    vThrow(__func__,__FILE__,__LINE__,"error.msg"_Stz);

  s256 sErrorMsg;
  sErrorMsg = aEC[iErrorCode];
  
  mpTask->m_pForth->m_pDataStack->vPushS256(sErrorMsg);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
