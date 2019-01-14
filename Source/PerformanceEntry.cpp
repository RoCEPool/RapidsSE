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
PerformanceEntry* Measurement::_pPerformanceEntry(void* aAddr)
{
  PerformanceEntry* aPerformanceTrace = reinterpret_cast<PerformanceEntry*>(aAddr);
  
  RASSERT_TYPE(aPerformanceTrace,CN_PERFORMANCEENTRY);
  
  return aPerformanceTrace;
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
PerformanceEntry::~PerformanceEntry(void) noexcept(false)
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
  return (Stz*)m_sWord.aStz();
}

/*----------------------------------------------------------------------------*/
/* return entry note                                                          */
/*----------------------------------------------------------------------------*/
Stz* PerformanceEntry::aNote(void)
{
  return (Stz*)m_sNote.aStz();
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
