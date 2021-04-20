/*============================================================================*/
/* Steps_ActivityLog.cpp -- Activity Log step group                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Log(void)
{
  vAddStep("log::markEntry"_Stz,  &Step::vLogMarkEntry);
  vAddStep("log::markExit"_Stz,   &Step::vLogMarkExit);

  vAddStep("Log::markSeed"_Stz,   &Step::vLogMarkSeed);
  vAddStep("Log::markWord"_Stz,   &Step::vLogMarkWord);
}

/*----------------------------------------------------------------------------*/
/* log::markEntry -- write an log entry for word entry                        */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkEntry(void)
{
  ENTRY();
  
  s256 sWordNm;
  
  m_pDataStack->vPopStr(sWordNm,__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::markExit -- write an log entry for word exit                          */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkExit(void)
{
  ENTRY();
  
  s256 sWordNm;
  
  m_pDataStack->vPopStr(sWordNm,__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::markSeed -- write a seed mark into the action sequence                */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkSeed(void)
{
  ENTRY();
  
  i8 iSeed = m_pDataStack->iPopInt(__func__);
  UNUSED(iSeed);

  s32 sWordNm;
  m_pDataStack->vPopStr(sWordNm,__func__);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* log::markWord -- write a word mark into the action sequence                */
/*----------------------------------------------------------------------------*/
void Step::vLogMarkWord(void)
{
  ENTRY();
  
  i8 iAN = m_pDataStack->iPopInt(__func__);
  UNUSED(iAN);

  s32 sWordNm;
  m_pDataStack->vPopStr(sWordNm,__func__);
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
