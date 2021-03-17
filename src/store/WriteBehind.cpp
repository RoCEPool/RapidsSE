/*============================================================================*/
/* WriteBehind.cpp -- Working WriteBehind Buffer                              */
/*============================================================================*/

#include "BASE.h"

#include "NamedBuffer.hpp"

#include "WriteBehind.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,WriteBehind,CN_WRITEBEHIND)
__aNewFunction(WriteBehind,CN_WRITEBEHIND)
__vDeleteFunction(WriteBehind,CN_WRITEBEHIND)
__aFooterFunction(WriteBehind)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
WriteBehind::WriteBehind(void)
: Object(CN_WRITEBEHIND)
, m_okLock(this,C_DONT_LOG)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  m_pStreamDef_events = AM::pStreamDefFind(C_TDEF_EVENTS);
  
  m_pNamedBuffer_events = nullptr;
    
  _checkInitialised_(CN_WRITEBEHIND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
WriteBehind::~WriteBehind(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* deposit var-arg list of column values into an __events row                 */
/*----------------------------------------------------------------------------*/
void WriteBehind::vDepositEvent(i8 iVargsBegin, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);

  ASSERT(C_VARGS_BEGIN == iVargsBegin);
  
  //m_okLock.vGrabWrite();
  
  Row* pRow = pAllotEventRow();

  va_list xVarArgs;
  va_start(xVarArgs,iVargsBegin);

  pRow->vDeposit(xVarArgs);
  
  va_end(xVarArgs);
  
  //m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a event row                                                          */
/*----------------------------------------------------------------------------*/
Row* WriteBehind::pAllotEventRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WRITEBEHIND,_FOOTER_);
  
  if (! m_pNamedBuffer_events) {
    m_pNamedBuffer_events = new NamedBuffer(m_pStreamDef_events,C_DONT_LOG);
  }
  
  Row* pRow = m_pNamedBuffer_events->pAllotRow();
  
  if (! pRow) {
    
    
    // temp until the demon is in place
    //delete m_pNamedBuffer_events;
    //NEVER_REACH;
    
    AM::DEMON::vPipeTo("__eventDemon"_Stz,m_pNamedBuffer_events);
    
    m_pNamedBuffer_events = nullptr;
    
    m_pNamedBuffer_events = new NamedBuffer(m_pStreamDef_events,C_DONT_LOG);
    pRow = m_pNamedBuffer_events->pAllotRow();
    RASSERT(pRow);
  }
  
  return pRow;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
