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
  vAddStep("[EvMgr].curEventNm@"_Stz,       &Step::vEventCurEventNmFetch);
  vAddStep("[EvMgr].decodeEventNm"_Stz,     &Step::vEventDecodeEventNm);
  vAddStep("[EvMgr].decodeHandlerNm"_Stz,   &Step::vEventDecodeHandlerNm);
  vAddStep("[EvMgr].eventCount@"_Stz,       &Step::vEventEventCountFetch);
}

/*----------------------------------------------------------------------------*/
/* [EvMgr].curEventNm@ -- get name of current event                           */
/*----------------------------------------------------------------------------*/
void Step::vEventCurEventNmFetch(void)
{
  ENTRY();
    
  eZK eCurEvent = m_pOp->pEvMgr()->m_eCurEvent;
    
  m_pDataStack->vPushStz(aZK[eCurEvent]);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [EvMgr].eventCount@ -- get count of given event so far                     */
/*----------------------------------------------------------------------------*/
void Step::vEventEventCountFetch(void)
{
  ENTRY();
  
  eZK eEvent = (eZK)m_pDataStack->iPopInt(__func__);
  
  i8 nCount = m_pOp->nEventCount(eEvent);

  m_pDataStack->vPushInt(nCount);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [EvMgr].decodeEventNm -- deccode eZK into aZK                              */
/*----------------------------------------------------------------------------*/
void Step::vEventDecodeEventNm(void)
{
  ENTRY();
  
  i8 iZK = m_pDataStack->iPopInt(__func__);

  Stz* szEventNm = aZK[iZK];
  
  m_pDataStack->vPushStz(szEventNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* event::handlerName -- deccode eZK into handler name                        */
/*----------------------------------------------------------------------------*/
void Step::vEventDecodeHandlerNm(void)
{
  ENTRY();
  
  i8 iZK = m_pDataStack->iPopInt(__func__);
  Stz* szEventNm = aZK[iZK];

  s32 sHandlerNm;
  
  sHandlerNm.vSprint("%s_handler"_Stz,szEventNm);
  
  m_pDataStack->vPushStr(sHandlerNm);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
