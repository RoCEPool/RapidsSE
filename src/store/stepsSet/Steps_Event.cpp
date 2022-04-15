/*============================================================================*/
/* Steps_Event.cpp - event group                                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Op.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Event(void)
{
  vAddStep("Event[].count@"_Stz,       &Step::vEventCountFetch);

  vAddStep("event::handlerName@"_Stz,  &Step::vEventHandlerName);
  vAddStep("event::name@"_Stz,         &Step::vEventName);
}

/*----------------------------------------------------------------------------*/
/* Event[].count@ -- get count of given event so far                          */
/*----------------------------------------------------------------------------*/
void Step::vEventCountFetch(void)
{
  ENTRY();
  
  eZK eEvent = (eZK)m_pDataStack->iPopInt(__func__);
  
  i8 nCount = m_pOp->nEventCount(eEvent);

  m_pDataStack->vPushInt(nCount);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* event::handlerName -- deccode eZK into handler name                        */
/*----------------------------------------------------------------------------*/
void Step::vEventHandlerName(void)
{
  ENTRY();
  
  i8 iZK = m_pDataStack->iPopInt(__func__);
  Stz* szEventNm = aZK[iZK];

  s32 sHandlerNm;
  
  sHandlerNm.vSprint("%s_handler"_Stz,szEventNm);
  
  m_pDataStack->vPushStr(sHandlerNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* event::name -- deccode eZK into aZK                                        */
/*----------------------------------------------------------------------------*/
void Step::vEventName(void)
{
  ENTRY();
  
  i8 iZK = m_pDataStack->iPopInt(__func__);

  Stz* szEventNm = aZK[iZK];
  
  m_pDataStack->vPushStz(szEventNm);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
