/*============================================================================*/
/* Steps_ErrorCode.cpp - ErrorCode[] step group                               */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

//#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ErrorCode(void)
{
  vAddStep("ErrorCode::decodeError"_Stz,  &Step::vErrorCodeDecodeError);

  vAddStep("ErrorCode[]"_Stz,             &Step::vErrorCodeGet);
  vAddStep("ErrorCode[].get"_Stz,         &Step::vErrorCodeGet);
  vAddStep("ErrorCode[].set"_Stz,         &Step::vErrorCodeSet);
}

/*----------------------------------------------------------------------------*/
/* ErrorCode[].get -- get error code (from logic unit)                        */
/*----------------------------------------------------------------------------*/
void Step::vErrorCodeGet(void)
{
  ENTRY();
 
  eEC eError = (eEC)m_pLogicUnit->m_eErrorCode_reg;
  
  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorCode::decodeError -- decode error code                                */
/*----------------------------------------------------------------------------*/
void Step::vErrorCodeDecodeError(void)
{
  ENTRY();
 
  eEC eError = (eEC)m_pDataStack->iPopInt(__func__);

  Stz* szError = aEC[eError];
  
  m_pDataStack->vPushStz(szError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorCode[].set -- set error code in logic unit                            */
/*----------------------------------------------------------------------------*/
void Step::vErrorCodeSet(void)
{
  ENTRY();
    
  eEC eError = (eEC)m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_eErrorCode_reg = eError;

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
