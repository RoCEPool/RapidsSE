/*============================================================================*/
/* Format.cpp -- set of formatting characters                             */
/*============================================================================*/

#include "Chunks.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Format* Chunks::_pFormatters(void* aAddr)
{
  Format* pFormatters = reinterpret_cast<Format*>(aAddr);
  
  RASSERT_TYPE(pFormatters,CN_FORMAT);
  
  return pFormatters;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Format::operator new(size_t)
{
  void* pFormatters = HeapMem::pAllocate(CN_FORMAT);
  
  return pFormatters;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Format::operator delete(void* pFormattersAddress)
{
  Format* pFormatters = _pFormatters(pFormattersAddress);
  
  HeapMem::vFree(pFormatters);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Format::Format(void)
: Object(CN_FORMAT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT);

  m_bDefault             = true;
  m_bAllowNonPrintable   = false;

  //
  // these are the default values
  //
  
  m_cEnclosedBy_Leading  = D_ENCLOSED_BY_LEADING;  //  '\0';
  m_cEnclosedBy_Trailing = D_ENCLOSED_BY_TRAILING; //  '\0';
  //m_cEnclosedBy_Leading  = '{';
  //m_cEnclosedBy_Trailing = '}';
  m_cEscapeChar          = D_ESCAPE_CHAR;          //  '\\';
  m_cFieldDelimiter      = D_FIELD_DELIMITER;      //  ',';
  m_cRecordTerminator    = D_RECORD_TERMINATOR;    //  '\0';
  m_eRecordEnd           = RE_NL;
  
  m_eDateComponent[0]    = DC_YYYY;  // default is YYYY-MM-DD
  m_eDateComponent[1]    = DC_MM;
  m_eDateComponent[2]    = DC_DD;
  m_cDateSeparator       = '-';
  
  m_nMaxErrors           = 0;
  
  _checkInitialised_(CN_FORMAT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FORMAT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Format::~Format(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* reset the formatters to given set of defaults                              */
/*----------------------------------------------------------------------------*/
void Format::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT);

  m_bDefault = true;
  
  m_bAllowNonPrintable   = AM::pFormatters()->m_bAllowNonPrintable;
  m_cEnclosedBy_Leading  = AM::pFormatters()->m_cEnclosedBy_Leading;
  m_cEnclosedBy_Trailing = AM::pFormatters()->m_cEnclosedBy_Trailing;
  
  m_cEscapeChar       = AM::pFormatters()->m_cEscapeChar;
  m_cFieldDelimiter   = AM::pFormatters()->m_cFieldDelimiter;
  m_cRecordTerminator = AM::pFormatters()->m_cRecordTerminator;
  m_eRecordEnd        = AM::pFormatters()->m_eRecordEnd;
  
  m_eDateComponent[0] = AM::pFormatters()->m_eDateComponent[0];
  m_eDateComponent[1] = AM::pFormatters()->m_eDateComponent[1];
  m_eDateComponent[2] = AM::pFormatters()->m_eDateComponent[2];
  
  m_cDateSeparator    = AM::pFormatters()->m_cDateSeparator;
  m_nMaxErrors        = AM::pFormatters()->m_nMaxErrors;
}

/*----------------------------------------------------------------------------*/
/* reset the formatters to given set of defaults                              */
/*----------------------------------------------------------------------------*/
void Format::vSet(Format* pFormatters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT);
  
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
  m_nMaxErrors        = pFormatters->m_nMaxErrors;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Format::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT);
  
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
  
  vTrace("max_errors %lld\n"_Stz, m_nMaxErrors);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Format::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
