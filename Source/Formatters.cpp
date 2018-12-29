/*============================================================================*/
/* Formatters.hpp -- set of formatting characters                             */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Formatters::operator new(size_t)
{
  Formatters* pFormatters = (Formatters*)gpBufferPool->pAllocBuffer();
  
  return pPoolCheck<void*>(pFormatters);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Formatters::operator delete(void* pFormattersAddress)
{
  Formatters* pFormatters = static_cast<Formatters*>(pFormattersAddress);
  
  gpBufferPool->vFree((Buffer*)pFormatters);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Formatters::Formatters(void)
: Object(CN_FORMATTERS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS);

  m_cEnclosedBy_Leading  = '\0';
  m_cEnclosedBy_Trailing = m_cEnclosedBy_Leading;
  //m_cEnclosedBy_Leading  = '{';
  //m_cEnclosedBy_Trailing = '}';
  
  m_cEscapeChar = '\\';
  m_cFieldDelimiter = ',';
  m_cRecordTerminator = '\0';
  m_eRecordEnd = RE_NL;
  
  m_eDateComponent[0] = DC_YYYY;  // default is YYYY=MM-DD
  m_eDateComponent[1] = DC_MM;
  m_eDateComponent[2] = DC_DD;
  m_cDateSeparator = '-';
  
  _checkInitialised_(CN_FORMATTERS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Formatters::~Formatters(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* reset the formatters to given set of defaults                              */
/*----------------------------------------------------------------------------*/
void Formatters::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS);
  
  vReset(&gpAttachMap->m_xFormatters);
}

/*----------------------------------------------------------------------------*/
/* reset the formatters to given set of defaults                              */
/*----------------------------------------------------------------------------*/
void Formatters::vReset(Formatters* pDefaults)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS);
  
  m_cEnclosedBy_Leading  = pDefaults->m_cEnclosedBy_Leading;
  m_cEnclosedBy_Trailing = pDefaults->m_cEnclosedBy_Trailing;
  
  m_cEscapeChar       = pDefaults->m_cEscapeChar;
  m_cFieldDelimiter   = pDefaults->m_cFieldDelimiter;
  m_cRecordTerminator = pDefaults->m_cRecordTerminator;
  m_eRecordEnd       = pDefaults->m_eRecordEnd;
  
  m_eDateComponent[0] = pDefaults->m_eDateComponent[0];
  m_eDateComponent[1] = pDefaults->m_eDateComponent[1];
  m_eDateComponent[2] = pDefaults->m_eDateComponent[2];
  
  m_cDateSeparator    = pDefaults->m_cDateSeparator;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Formatters::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS);
  
  vTrace("enclosed_by_Leading %c\n"_Stz, m_cEnclosedBy_Leading ? m_cEnclosedBy_Leading : '0');
  vTrace("enclosed_by_trailing %c\n"_Stz, m_cEnclosedBy_Trailing ? m_cEnclosedBy_Trailing : '0');
  
  vTrace("escape_char %c\n"_Stz, m_cEscapeChar ? m_cEscapeChar : '0');
  vTrace("delimiter %c\n"_Stz, m_cFieldDelimiter ? m_cFieldDelimiter : '0');
  vTrace("record_terminator %c\n"_Stz, m_cRecordTerminator ? m_cRecordTerminator : '0');
  vTrace("record_end %s\n"_Stz, aRE[m_eRecordEnd]);
  
  vTrace("date component 0 %s\n"_Stz, aDC[m_eDateComponent[0]]);
  vTrace("date component 1 %s\n"_Stz, aDC[m_eDateComponent[1]]);
  vTrace("date component 2 %s\n"_Stz, aDC[m_eDateComponent[2]]);
  
  vTrace("date_separator %c\n"_Stz, m_cDateSeparator ? m_cDateSeparator : '0');
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Formatters::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
