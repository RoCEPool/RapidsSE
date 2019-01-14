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

  m_bDefault = true;
  m_bAllowNonPrintable = false;
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

  m_bDefault = true;
  
  m_bAllowNonPrintable   = gpAttachMap->m_oFormatters.m_bAllowNonPrintable;
  m_cEnclosedBy_Leading  = gpAttachMap->m_oFormatters.m_cEnclosedBy_Leading;
  m_cEnclosedBy_Trailing = gpAttachMap->m_oFormatters.m_cEnclosedBy_Trailing;
  
  m_cEscapeChar       = gpAttachMap->m_oFormatters.m_cEscapeChar;
  m_cFieldDelimiter   = gpAttachMap->m_oFormatters.m_cFieldDelimiter;
  m_cRecordTerminator = gpAttachMap->m_oFormatters.m_cRecordTerminator;
  m_eRecordEnd        = gpAttachMap->m_oFormatters.m_eRecordEnd;
  
  m_eDateComponent[0] = gpAttachMap->m_oFormatters.m_eDateComponent[0];
  m_eDateComponent[1] = gpAttachMap->m_oFormatters.m_eDateComponent[1];
  m_eDateComponent[2] = gpAttachMap->m_oFormatters.m_eDateComponent[2];
  
  m_cDateSeparator    = gpAttachMap->m_oFormatters.m_cDateSeparator;
}

/*----------------------------------------------------------------------------*/
/* reset the formatters to given set of defaults                              */
/*----------------------------------------------------------------------------*/
void Formatters::vSet(Formatters* pFormatters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMATTERS);
  
  m_bDefault = false;
  
  m_bAllowNonPrintable   = pFormatters->m_bAllowNonPrintable;
  m_cEnclosedBy_Leading  = pFormatters->m_cEnclosedBy_Leading;
  m_cEnclosedBy_Trailing = pFormatters->m_cEnclosedBy_Trailing;
  
  m_cEscapeChar       = pFormatters->m_cEscapeChar;
  m_cFieldDelimiter   = pFormatters->m_cFieldDelimiter;
  m_cRecordTerminator = pFormatters->m_cRecordTerminator;
  m_eRecordEnd        = pFormatters->m_eRecordEnd;
  
  m_eDateComponent[0] = pFormatters->m_eDateComponent[0];
  m_eDateComponent[1] = pFormatters->m_eDateComponent[1];
  m_eDateComponent[2] = pFormatters->m_eDateComponent[2];
  
  m_cDateSeparator    = pFormatters->m_cDateSeparator;
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
