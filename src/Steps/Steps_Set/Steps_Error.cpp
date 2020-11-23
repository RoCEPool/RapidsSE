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
  ENTRY();
  
  
  i8 iErrorCode = m_pDataStack->iPopInt(__func__);
  
  if (iErrorCode < EC_NONE || iErrorCode > EC_ENUMS)
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,"error.msg"_Stz);

  s256 sErrorMsg;
  sErrorMsg = aEC[iErrorCode];
  
  m_pDataStack->vPushStr(sErrorMsg);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
