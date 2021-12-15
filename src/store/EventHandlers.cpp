/*============================================================================*/
/* EventHandlers.cpp - Event Handlers                                         */
/*============================================================================*/

#include "STORE.h"
#include "EventHandlers.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,EventHandlers,CN_EVENTHANDLERS)
__aNewFunction(EventHandlers,CN_EVENTHANDLERS)
__vDeleteFunction(EventHandlers,CN_EVENTHANDLERS)
__aFooterFunction(EventHandlers)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EventHandlers::EventHandlers(void)
: Object(CN_EVENTHANDLERS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EVENTHANDLERS,_FOOTER_);
  
  for (i8 i_Event =0 ; i_Event < ZK_ENUMS; i_Event++)
    m_paEventHandler[i_Event] = nullptr;
  
  _checkInitialised_(CN_EVENTHANDLERS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_EVENTHANDLERS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EventHandlers::~EventHandlers(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EVENTHANDLERS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a handler                                                              */
/*----------------------------------------------------------------------------*/
Word* EventHandlers::pGet(eZK eEvent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EVENTHANDLERS,_FOOTER_);
  
  return m_paEventHandler[eEvent];
}

/*----------------------------------------------------------------------------*/
/* initialse the set of event handlers                                        */
/*----------------------------------------------------------------------------*/
void EventHandlers::vInitialise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EVENTHANDLERS,_FOOTER_);
  
  Dic* pDicRoot = AM::DIC::pRoot();
  
  s32 sHandlerNm;
  
  for (i8 i_Event = 1; i_Event < ZK_ENUMS; i_Event++) {
    
    sHandlerNm = aZK[i_Event];
    sHandlerNm.vConcat("_handler"_Stz);
        
    Word* pHandler = pDicRoot->pFindWord(sHandlerNm);
    if (! pHandler)
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "Unable to find operand handler for: %s"_Stz,
             sHandlerNm.szStz());
    
    m_paEventHandler[i_Event] = pHandler;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
