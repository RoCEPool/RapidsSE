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

  vAddStep("ErrorStr[]"_Stz,             &Step::vErrorStrGet);
  vAddStep("ErrorStr[].get"_Stz,         &Step::vErrorStrGet);
  vAddStep("ErrorStr[].set"_Stz,         &Step::vErrorStrSet);
}

/*----------------------------------------------------------------------------*/
/* ErrorCode[].get -- get error code (from logic unit)                        */
/*----------------------------------------------------------------------------*/
void Step::vErrorCodeGet(void)
{
  ENTRY();
 
  eEC eError = (eEC)m_pLogicUnit->m_eErrorCode_reg;
  
  m_oDataStack.vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorCode::decodeError -- decode error code                                */
/*----------------------------------------------------------------------------*/
void Step::vErrorCodeDecodeError(void)
{
  ENTRY();
 
  eEC eError = (eEC)m_oDataStack.iPopInt(__func__);

  Stz* szError = aEC[eError];
  
  m_oDataStack.vPushStz(szError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorCode[].set -- set error code in logic unit                            */
/*----------------------------------------------------------------------------*/
void Step::vErrorCodeSet(void)
{
  ENTRY();
    
  eEC eError = (eEC)m_oDataStack.iPopInt(__func__);

  m_pLogicUnit->m_eErrorCode_reg = eError;

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* ErrorStr[].get -- get supplementary error details (from logic unit)        */
/*----------------------------------------------------------------------------*/
void Step::vErrorStrGet(void)
{
  ENTRY();

  m_oDataStack.vPushStr(m_pLogicUnit->m_sErrorStr_reg);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* ErrorStr[].set -- set supplementary error details in logic unit            */
/*----------------------------------------------------------------------------*/
void Step::vErrorStrSet(void)
{
  ENTRY();

  s256 sErr;
  m_oDataStack.vPopStr(sErr, __func__);

  m_pLogicUnit->m_sErrorStr_reg.vCopy(sErr.szStz());

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
