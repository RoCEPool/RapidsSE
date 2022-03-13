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
 
  i8 iError = m_p_TaskLogicUnit->m_eErrorCode_reg;
  
  m_pDataStack->vPushInt(iError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorCode[].set -- set task                                                */
/*----------------------------------------------------------------------------*/
void Step::vErrorCodeSet(void)
{
  ENTRY();
    
  i8 iError = m_pDataStack->iPopInt(__func__);

  m_p_TaskLogicUnit->m_eErrorCode_reg = (eEC)iError;

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
