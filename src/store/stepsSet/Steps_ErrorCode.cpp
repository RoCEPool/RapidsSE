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
  vAddStep("ErrorCode[]"_Stz,      &Step::vErrorCodeGet);
  vAddStep("ErrorCode[].get"_Stz,  &Step::vErrorCodeGet);
  vAddStep("ErrorCode[].set"_Stz,  &Step::vErrorCodeSet);
}

/*----------------------------------------------------------------------------*/
/* ErrorCode[].get -- get task                                                */
/*----------------------------------------------------------------------------*/
void Step::vErrorCodeGet(void)
{
  ENTRY();
 
  eEC eError = (eEC)m_pLogicUnit->m_eErrorCode_reg;
  
  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorCode[].set -- set task                                                */
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
