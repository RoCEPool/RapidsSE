/*============================================================================*/
/* TraceMultiLock.cpp -- Trace Multi-Line Lock                                */
/*============================================================================*/

#include "BASE.h"

#include "TraceMultiLock.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,TraceMultiLock,CN_TRACEMULTILOCK)
//__aNewFunction(TraceMultiLock,CN_TRACEMULTILOCK)
//__vDeleteFunction(TraceMultiLock,CN_TRACEMULTILOCK)
__aFooterFunction(TraceMultiLock);

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TraceMultiLock::TraceMultiLock(void)
: MasterLock(CN_TRACEMULTILOCK)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRACEMULTILOCK,_FOOTER_);
   
  _checkInitialised_(CN_TRACEMULTILOCK);
  
  //_postcondition_(_ADDRESS_,_MAGIC_,CN_TRACEMULTILOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TraceMultiLock::~TraceMultiLock(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRACEMULTILOCK,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
