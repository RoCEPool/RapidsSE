/*============================================================================*/
/* PerformanceTrace.cpp -- Performance Trace                                  */
/*============================================================================*/

#include "Utilities.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PerformanceTrace* Utilities::_pPerformanceTrace(void* aAddr)
{
  PerformanceTrace* pPerformanceTrace = reinterpret_cast<PerformanceTrace*>(aAddr);
  
  RASSERT( !pPerformanceTrace || CN_PERFORMANCETRACE == pPerformanceTrace->eClass() );
  
  return pPerformanceTrace;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PerformanceTrace::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_PERFORMANCETRACE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PerformanceTrace::operator delete(void* pAddress)
{
  PerformanceTrace* pPerformanceTrace = reinterpret_cast<PerformanceTrace*>(pAddress);
  
  Heap::vFree(pPerformanceTrace);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PerformanceTrace::PerformanceTrace(void)
: Object(CN_PERFORMANCETRACE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCETRACE,_FOOTER_);
  
  //m_xTimer
  
  m_pFirstBuffer = nullptr;
  m_pLastBuffer  = nullptr;
  
  _checkInitialised_(CN_PERFORMANCETRACE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCETRACE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PerformanceTrace::~PerformanceTrace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCETRACE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PerformanceTrace::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCETRACE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
