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
void Step::vErrorMsg(void)
{
  ENTRY("vErrorMsg"_Stz);
  
  
  i8 iErrorCode;
  if ( ! m_pDataStack->bPopI8(iErrorCode) )
    THROW_DSTACK();
  
  if (iErrorCode < EC_NONE || iErrorCode > EC_ENUMS)
    vThrow(FK_FAILED,__func__,__FILE__,__LINE__,"error.msg"_Stz);

  s256 sErrorMsg;
  sErrorMsg = aEC[iErrorCode];
  
  m_pDataStack->vPushS256(sErrorMsg);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
