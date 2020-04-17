/*============================================================================*/
/* Steps_Now.cpp - now step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Now(void)
{
  vAddStep("now.asOf"_Stz,         &Step::vNowAsOf);
  vAddStep("now.sinceEpoch"_Stz,   &Step::vNowSinceEpoch);
  vAddStep("now.timestampStr"_Stz, &Step::vNowTimestampStr);
}

/*----------------------------------------------------------------------------*/
/* now.asOf -- update to as of now (utc)                                      */
/*----------------------------------------------------------------------------*/
void Step::vNowAsOf(void)
{
  ENTRY();
  
  Now* pNow = _pNow(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pNow->vSetNowUTC();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* now.sinceEpoch -- get seconds since epoch from now                         */
/*----------------------------------------------------------------------------*/
void Step::vNowSinceEpoch(void)
{
  ENTRY();
  
  Now* pNow = _pNow(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iSinceEpoch = pNow->iSinceEpoch();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pDataStack->vPushInt(iSinceEpoch);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* now.timestampStr -- get timestamp as string from now                       */
/*----------------------------------------------------------------------------*/
void Step::vNowTimestampStr(void)
{
  ENTRY();
  
  Now* pNow = _pNow(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szTimestamp = pNow->sz();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pDataStack->vPushStz(szTimestamp);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
