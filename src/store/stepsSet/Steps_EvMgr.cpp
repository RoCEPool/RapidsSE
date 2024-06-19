/*============================================================================*/
/* Steps_EvMgr.cpp - Event Manager steps                                      */
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
void Step::vAddSteps_EvMgr(void)
{
  vAddStep("[EvMgr].curEventNm@"_Stz,       &Step::vEvMgrCurEventNmFetch);
  vAddStep("[EvMgr].decodeEventNm"_Stz,     &Step::vEvMgrDecodeEventNm);
  vAddStep("[EvMgr].decodeHandlerNm"_Stz,   &Step::vEvMgrDecodeHandlerNm);
  vAddStep("[EvMgr].eventCount@"_Stz,       &Step::vEvMgrEventCountFetch);
}

/*----------------------------------------------------------------------------*/
/* [EvMgr].curEventNm@ -- get name of current event                           */
/*----------------------------------------------------------------------------*/
void Step::vEvMgrCurEventNmFetch(void)
{
  ENTRY();

  eEV eCurEvent = m_pOp->pEvMgr()->m_eCurEvent;
    
  m_oDataStack.vPushStz(aEV[eCurEvent]);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [EvMgr].eventCount@ -- get count of given event so far                     */
/*----------------------------------------------------------------------------*/
void Step::vEvMgrEventCountFetch(void)
{
  ENTRY();

  eEV eEvent = (eEV)m_oDataStack.iPopInt(__func__);
  
  i8 nCount = m_pOp->nEventCount(eEvent);

  m_oDataStack.vPushInt(nCount);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [EvMgr].decodeEventNm -- deccode eZK into aEV                              */
/*----------------------------------------------------------------------------*/
void Step::vEvMgrDecodeEventNm(void)
{
  ENTRY();
  
  i8 iZK = m_oDataStack.iPopInt(__func__);

  Stz* szEventNm = aEV[iZK];
  
  m_oDataStack.vPushStz(szEventNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* event::handlerName -- deccode eZK into handler name                        */
/*----------------------------------------------------------------------------*/
void Step::vEvMgrDecodeHandlerNm(void)
{
  ENTRY();
  
  i8 iZK = m_oDataStack.iPopInt(__func__);
  Stz* szEventNm = aEV[iZK];

  s32 sHandlerNm;
  
  sHandlerNm.vSprint("%s_handler"_Stz,szEventNm);
  
  m_oDataStack.vPushStr(sHandlerNm);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
