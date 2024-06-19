/*============================================================================*/
/* Steps_PayloadObj.cpp - PayloadObj step group                               */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "PayloadObj.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PayloadObj(void)
{

  vAddStep("payloadObj::delete"_Stz,       &Step::vPayloadObjDelete);
  vAddStep("payloadObj::size@"_Stz,        &Step::vPayloadObjSize);
  vAddStep("payloadObj::msgType@"_Stz,     &Step::vPayloadObjMsgType);
  vAddStep("payloadObj::creationTime@"_Stz,&Step::vPayloadObjCreationTime);
  vAddStep("payloadObj::doNotProcess@"_Stz,&Step::vPayloadObjDoNotProcess);

}

/*----------------------------------------------------------------------------*/
/* payloadObj::delete : delete the payloadObj on the stack                    */
/*----------------------------------------------------------------------------*/
void Step::vPayloadObjDelete(void)
{
  ENTRY();

  WLP::PayloadObj* pPayloadObj = WLP::_pPayloadObj(m_oDataStack.pPopPtr(__func__));
  ASSERT(CN_PAYLOADOBJ == pPayloadObj->eGetClassNo());

  delete pPayloadObj;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* payloadObj::size@ : push the size of the payloadObj on the stack           */
/*----------------------------------------------------------------------------*/
void Step::vPayloadObjSize(void)
{
  ENTRY();

  WLP::PayloadObj* pPayloadObj = WLP::_pPayloadObj(m_oDataStack.pPopPtr(__func__));
  ASSERT(CN_PAYLOADOBJ == pPayloadObj->eGetClassNo());

  i8 iSize = pPayloadObj->iPayloadSize();
  m_oDataStack.vPushInt(iSize);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* payloadObj::msgType@ : push a string description of the message type on stk*/
/*----------------------------------------------------------------------------*/
void Step::vPayloadObjMsgType(void)
{
  ENTRY();

  WLP::PayloadObj* pPayloadObj = WLP::_pPayloadObj(m_oDataStack.pPopPtr(__func__));
  ASSERT(CN_PAYLOADOBJ == pPayloadObj->eGetClassNo());

  s256 sBuf;
  pPayloadObj->vMsgType(sBuf);
  m_oDataStack.vPushStr(sBuf);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* payloadObj::creationTime@ : push the buffer creation time on the stk       */
/*----------------------------------------------------------------------------*/
void Step::vPayloadObjCreationTime(void)
{
  ENTRY();

  WLP::PayloadObj* pPayloadObj = WLP::_pPayloadObj(m_oDataStack.pPopPtr(__func__));
  ASSERT(CN_PAYLOADOBJ == pPayloadObj->eGetClassNo());

  u8 uCreationTime = pPayloadObj->uCreationTime();
  m_oDataStack.vPushInt((i8) uCreationTime);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* payloadObj::doNotProcess@ : get whether the adaptor should process this msg*/
/*----------------------------------------------------------------------------*/
void Step::vPayloadObjDoNotProcess(void)
{
  ENTRY();

  WLP::PayloadObj* pPayloadObj = WLP::_pPayloadObj(m_oDataStack.pPopPtr(__func__));
  ASSERT(CN_PAYLOADOBJ == pPayloadObj->eGetClassNo());

  bool bDoNotProcess = pPayloadObj->bDoNotProcess();
  m_oDataStack.vPushBol(bDoNotProcess);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
