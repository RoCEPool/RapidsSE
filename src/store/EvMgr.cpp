/*============================================================================*/
/* EvMgr.cpp - Event Manager                                                  */
/*============================================================================*/

#include "STORE.h"

#include "EvMgr.hpp"
#include "EventHandlers.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,EvMgr,CN_EVMGR)
__aNewFunction(EvMgr,CN_EVMGR)
__vDeleteFunction(EvMgr,CN_EVMGR)
__aFooterFunction(EvMgr)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EvMgr::EvMgr(void)
: Object(CN_EVMGR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EVMGR,_FOOTER_);
  
  m_pEventHandlers
    = _pEventHandlers(AM::HANDLERS::pEventHandlers()->pMemoryCopy());

  m_eCurEvent = ZK_NONE;

  for (i8 i_Event = 0; i_Event < ZK_ENUMS; i_Event++)
    m_naEventCount[i_Event] = 0;
  
  m_pOperandWatch = new OperandWatch;
  
  _checkInitialised_(CN_EVMGR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_EVMGR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EvMgr::~EvMgr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EVMGR,_FOOTER_);
  
  delete m_pEventHandlers;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get left input queue                                                       */
/*----------------------------------------------------------------------------*/
//OperandQueue* EvMgr::pQueueLeft(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_EVMGR,_FOOTER_);
//
//  return m_pQueueLeft;
//}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
