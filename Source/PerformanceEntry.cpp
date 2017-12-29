/*============================================================================*/
/* PerformanceEntry.cpp -- Performance Entry                                  */
/*============================================================================*/

#include "Utilities.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PerformanceEntry* Utilities::_pPerformanceEntry(void* aAddr)
{
  PerformanceEntry* pPerformanceTrace = reinterpret_cast<PerformanceEntry*>(aAddr);
  
  RASSERT( !pPerformanceTrace || CN_PERFORMANCEENTRY == pPerformanceTrace->eClass() );
  
  return pPerformanceTrace;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PerformanceEntry::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_PERFORMANCEENTRY);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PerformanceEntry::operator delete(void* pAddress)
{
  PerformanceEntry* pPerformanceTrace = reinterpret_cast<PerformanceEntry*>(pAddress);
  
  Heap::vFree(pPerformanceTrace);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PerformanceEntry::PerformanceEntry(void)
: Entry(CN_PERFORMANCEENTRY,sizeof(PerformanceEntry),0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEENTRY,_FOOTER_);
    
  _checkInitialised_(CN_PERFORMANCEENTRY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PerformanceEntry::~PerformanceEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PerformanceEntry::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEENTRY);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
