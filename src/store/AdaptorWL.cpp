/*============================================================================*/
/* AdaptorWL.cpp -- Wire Line Adaptor                                         */
/*============================================================================*/

#include "BASE.h"

#include "AdaptorWL.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, AdaptorWL, CN_ADAPTORWL)
__aNewFunction(AdaptorWL, CN_ADAPTORWL)
__vDeleteFunction(AdaptorWL, CN_ADAPTORWL)
__aFooterFunction(AdaptorWL)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AdaptorWL::AdaptorWL(void)
: Object(CN_ADAPTORWL)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ADAPTORWL, _FOOTER_);

  m_pPayloadSendList = new PayloadList();
  m_bContinueProcessingMsg = false;

  _checkInitialised_(CN_ADAPTORWL);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_ADAPTORWL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AdaptorWL::~AdaptorWL(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ADAPTORWL, _FOOTER_);
  m_pPayloadSendList->vDeleteAll();
  delete m_pPayloadSendList;
}

/*----------------------------------------------------------------------------*/
/* repair the aggregate time                                                  */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vRepair(Object* pObject)
{
  _static_

  AdaptorWL* pAdaptorWL = _pAdaptorWL(pObject);

  delete pAdaptorWL;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Decode the payload into a wireline message                                 */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vDecodePayload(WLP::PayloadObj* pPayload)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);
  // TODO

}

/*----------------------------------------------------------------------------*/
/* Process or continue processing the current decoded wireline message        */
/*----------------------------------------------------------------------------*/
void AdaptorWL::vProcessWLMsg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);
  // TODO
}

/*----------------------------------------------------------------------------*/
/* Return true if vProcessWLMsg() needs to be called again                    */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bKeepProcessingMsg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);
  return m_bContinueProcessingMsg;
}

/*----------------------------------------------------------------------------*/
/* Return true if there is at least 1 PayloadObj to be sent                   */
/*----------------------------------------------------------------------------*/
bool AdaptorWL::bHasPayloadToSend(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);

  return (m_pPayloadSendList->nItems() > 0);
}

/*----------------------------------------------------------------------------*/
/* Return the next PayloadObj to be sent                                      */
/*----------------------------------------------------------------------------*/
WLP::PayloadObj* AdaptorWL::pNextPayloadToSend(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ADAPTORWL,_FOOTER_);
  return m_pPayloadSendList->pPop();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
