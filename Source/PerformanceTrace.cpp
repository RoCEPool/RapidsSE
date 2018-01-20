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
  PerformanceTrace* pPerformanceTrace =
    reinterpret_cast<PerformanceTrace*>(pAddress);
  
  Heap::vFree(pPerformanceTrace);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PerformanceTrace::PerformanceTrace(void)
: Object(CN_PERFORMANCETRACE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCETRACE,_FOOTER_);
  
  m_xTimer.vInit();
  m_xTimer.vStart();
  
  m_pFirstBuffer   = nullptr;
  m_pCurrentBuffer = nullptr;
  
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
/* add performance entry                                                      */
/*----------------------------------------------------------------------------*/
void PerformanceTrace::vAdd(Stz* aWord, Stz* aNote)
{
  PerformanceEntry* pPerformanceEntry = newPerformanceEntry();

  m_xTimer.vUpdate();
  i8 iTickDelta = m_xTimer.iTicks();
  pPerformanceEntry->vSet(iTickDelta,aWord,aNote);
  
//  vTrace("Performance Add: %lld %s %s\n"_Stz,
//         m_xTimer.iMicroseconds(),
//         pPerformanceEntry->aWord(),
//         pPerformanceEntry->aNote()
//         );
}

/*----------------------------------------------------------------------------*/
/* print performance entries                                                  */
/*----------------------------------------------------------------------------*/
//void PerformanceTrace::vPrint(void)
//{
//  Timer xTimerElapsed;
//  Timer xTimerDelta;
//  
//  i8 iTicksStart  = m_xTimer.iStart();
//  i8 iTicksUpdate = iTicksStart;
//  
//  for (EntryBuffer* pEntryBuffer = m_pFirstBuffer;
//       pEntryBuffer;
//       pEntryBuffer = pEntryBuffer->pNextBuffer()) {
//
//    for (PerformanceEntry* pEntry =
//           static_cast<PerformanceEntry*>(pEntryBuffer->pHead());
//         pEntry;
//         pEntry =
//           static_cast<PerformanceEntry*>(pEntryBuffer->pNextEntry(pEntry))) {
//      
//      i8 iEntryTicks = pEntry->iTicks();
//      iTicksUpdate += iEntryTicks;
//      xTimerElapsed.vSet(iTicksStart,iTicksUpdate);
//      xTimerDelta.vSet(0,iEntryTicks);
//
//      vTrace("Performance Entry: %lldus %lldus %s %s\n"_Stz,
//             xTimerElapsed.iMicroseconds(),
//             xTimerDelta.iMicroseconds(),
//             pEntry->aWord(),
//             pEntry->aNote()
//             );
//    }
//    
//  }
//}

void PerformanceTrace::vPrint(void)
{
  Timer xTimerDelta;
  Timer xTimerElapsed;
  PerformanceEntry* pPrevEntry  = nullptr;
  PerformanceEntry* pFirstEntry = nullptr;
  
  i8 iEntryNumber = 0;
  
  for (PerformanceBuffer* pPerfBuffer = m_pFirstBuffer;
       pPerfBuffer;
       pPerfBuffer = pPerfBuffer->pNextBuffer()) {
    
    for (PerformanceEntry* pEntry =
         static_cast<PerformanceEntry*>(pPerfBuffer->pFirstEntry());
         pEntry;
         pEntry =
         static_cast<PerformanceEntry*>(pPerfBuffer->pNextEntry(pEntry))) {
  
      i8 iEntryTicks = pEntry->iTicks();
      xTimerElapsed.vSet(0,iEntryTicks);
      
      if (pPrevEntry) {
        i8 iPrevEntryTicks = pPrevEntry->iTicks();
        xTimerDelta.vSet(iPrevEntryTicks,iEntryTicks);
      }
      else {
        xTimerDelta.vSet(0,iEntryTicks);
      }
      
      vTrace("Performance Entry[%lld]: %lldus %lldus %s %s\n"_Stz,
             iEntryNumber++,
             xTimerElapsed.iMicroseconds(),
             xTimerDelta.iMicroseconds(),
             pEntry->aWord(),
             pEntry->aNote()
             );
      
      pPrevEntry = pEntry;
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* reset performance tracing                                                  */
/*----------------------------------------------------------------------------*/
void PerformanceTrace::vReset(void)
{
  PerformanceBuffer* pEntryBuffer = m_pFirstBuffer;

  while (pEntryBuffer) {
    PerformanceBuffer* pNextEntryBuffer = pEntryBuffer->pNextBuffer();
    delete pEntryBuffer;
    pEntryBuffer = pNextEntryBuffer;
  }
  
  m_pFirstBuffer = nullptr;
  m_pCurrentBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* create a new performance entry                                             */
/*----------------------------------------------------------------------------*/
PerformanceEntry* PerformanceTrace::newPerformanceEntry(void)
{
  PerformanceEntry* pPerformanceEntry = nullptr;
  
  if ( ! m_pCurrentBuffer ) {
    m_pFirstBuffer = new PerformanceBuffer;
    m_pCurrentBuffer = m_pFirstBuffer;
    m_xTimer.vInit();
    m_xTimer.vStart();
  }
  
  pPerformanceEntry = m_pCurrentBuffer->pAllotEntry();
  
  if (pPerformanceEntry)
    return pPerformanceEntry;
  
  PerformanceBuffer* pNewBuffer = new PerformanceBuffer;
  
  m_pCurrentBuffer->vNextBuffer(pNewBuffer);
  pNewBuffer->vPrevBuffer(m_pCurrentBuffer);
  m_pCurrentBuffer = pNewBuffer;
  
  pPerformanceEntry = m_pCurrentBuffer->pAllotEntry();
  
  return pPerformanceEntry;
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
