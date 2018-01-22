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
void* PerformanceEntry::operator new(size_t iSize, PerformanceEntry* pPeformanceEntry)
{
  RASSERT(iSize);

  MemLib::vZero(pPeformanceEntry,sizeof(PerformanceEntry),CN_PERFORMANCEENTRY);
  return pPeformanceEntry;
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
: Item(CN_PERFORMANCEENTRY,sizeof(PerformanceEntry))
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEENTRY,_FOOTER_);
  
  m_iTicks = 0;
  m_sWord = ""_Stz;
  m_sNote = ""_Stz;
  
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
/* set performance entry                                                      */
/*----------------------------------------------------------------------------*/
void PerformanceEntry::vSet(i8 iTicks, Stz* aWord, Stz* aNote)
{
  m_iTicks = iTicks;
  m_sWord  = aWord;
  m_sNote  = aNote;
}

/*----------------------------------------------------------------------------*/
/* return tick count                                                          */
/*----------------------------------------------------------------------------*/
i8 PerformanceEntry::iTicks(void)
{
  return m_iTicks;
}

/*----------------------------------------------------------------------------*/
/* return word name                                                           */
/*----------------------------------------------------------------------------*/
Stz* PerformanceEntry::aWord(void)
{
  return (Stz*)m_sWord.aStr();
}

/*----------------------------------------------------------------------------*/
/* return entry note                                                          */
/*----------------------------------------------------------------------------*/
Stz* PerformanceEntry::aNote(void)
{
  return (Stz*)m_sNote.aStr();
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
