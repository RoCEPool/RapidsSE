/*============================================================================*/
/* Format.cpp -- set of formatting characters and rules                       */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"

#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Arguments,Format,CN_FORMAT)
__aNewFunction(Format,CN_FORMAT)
__vDeleteFunction(Format,CN_FORMAT)
__aFooterFunction(Format)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Format::Format(void)
: Object(CN_FORMAT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT);

  m_bAllowNonPrintable   = false;

  //
  // these are the default values
  //
  
  m_eDateComponent[0]    = DC_YYYY;  // default is YYYY-MM-DD
  m_eDateComponent[1]    = DC_MM;
  m_eDateComponent[2]    = DC_DD;
  m_cDateSeparator       = '-';
  m_cEnclosedBy_Leading  = D_ENCLOSED_BY_LEADING;  //  '\0';
  m_cEnclosedBy_Trailing = D_ENCLOSED_BY_TRAILING; //  '\0';
  m_cEscapeChar          = D_ESCAPE_CHAR;          //  '\\';
  m_cFieldDelimiter      = D_FIELD_DELIMITER;      //  ',';
  m_eLineEnd             = LE_NL;
  m_cRecordTerminator    = D_RECORD_TERMINATOR;    //  '\0';
  
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
/* test for standard/normal csv file                                          */
/*----------------------------------------------------------------------------*/
bool Format::bIsStdCsv(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT);
  
  if (DC_YYYY != m_eDateComponent[0])
    return false;
  
  if (DC_MM != m_eDateComponent[1])
    return false;
  
  if (DC_DD != m_eDateComponent[2])
    return false;
  
  if ('-' != m_cDateSeparator)
    return false;
  
  if ('\0' != m_cEnclosedBy_Leading)
    return false;
  
  if ('\0' != m_cEnclosedBy_Trailing)
    return false;
  
  if ('\\' != m_cEscapeChar)
    return false;
  
  if (',' != m_cFieldDelimiter)
    return false;
  
  if (LE_NL != m_eLineEnd)
    return false;
  
  if ('\0' != m_cRecordTerminator)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* reset the formatters to given set of defaults                              */
/*----------------------------------------------------------------------------*/
void Format::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT);
  
  m_bAllowNonPrintable   = AM::pFmt()->m_bAllowNonPrintable;
  m_eDateComponent[0]    = AM::pFmt()->m_eDateComponent[0];
  m_eDateComponent[1]    = AM::pFmt()->m_eDateComponent[1];
  m_eDateComponent[2]    = AM::pFmt()->m_eDateComponent[2];
  m_cDateSeparator       = AM::pFmt()->m_cDateSeparator;
  m_cEnclosedBy_Leading  = AM::pFmt()->m_cEnclosedBy_Leading;
  m_cEnclosedBy_Trailing = AM::pFmt()->m_cEnclosedBy_Trailing;
  m_cEscapeChar          = AM::pFmt()->m_cEscapeChar;
  m_cFieldDelimiter      = AM::pFmt()->m_cFieldDelimiter;
  m_cRecordTerminator    = AM::pFmt()->m_cRecordTerminator;
  m_eLineEnd             = AM::pFmt()->m_eLineEnd;
  m_nMaxErrors           = AM::pFmt()->m_nMaxErrors;
}

/*----------------------------------------------------------------------------*/
/* reset the formatters to given set of defaults                              */
/*----------------------------------------------------------------------------*/
void Format::vSet(Format* pFmt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT);
  
  m_bAllowNonPrintable   = pFmt->m_bAllowNonPrintable;
  m_eDateComponent[0]    = pFmt->m_eDateComponent[0];
  m_eDateComponent[1]    = pFmt->m_eDateComponent[1];
  m_eDateComponent[2]    = pFmt->m_eDateComponent[2];
  m_cDateSeparator       = pFmt->m_cDateSeparator;
  m_cEnclosedBy_Leading  = pFmt->m_cEnclosedBy_Leading;
  m_cEnclosedBy_Trailing = pFmt->m_cEnclosedBy_Trailing;
  m_cEscapeChar          = pFmt->m_cEscapeChar;
  m_cFieldDelimiter      = pFmt->m_cFieldDelimiter;
  m_cRecordTerminator    = pFmt->m_cRecordTerminator;
  m_eLineEnd             = pFmt->m_eLineEnd;
  m_nMaxErrors           = pFmt->m_nMaxErrors;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Format::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT);
  
  vTrace("date component 0 %s\n"_Stz,
         aDC[m_eDateComponent[0]]);
  
  vTrace("date component 1 %s\n"_Stz,
         aDC[m_eDateComponent[1]]);
  
  vTrace("date component 2 %s\n"_Stz,
         aDC[m_eDateComponent[2]]);
  
  vTrace("date_separator %c\n"_Stz,
         m_cDateSeparator ? m_cDateSeparator : '0');
  
  vTrace("enclosed_by_Leading %c\n"_Stz,
         m_cEnclosedBy_Leading ? m_cEnclosedBy_Leading : '0');
  
  vTrace("enclosed_by_trailing %c\n"_Stz,
         m_cEnclosedBy_Trailing ? m_cEnclosedBy_Trailing : '0');
  
  vTrace("escape_char %c\n"_Stz,
         m_cEscapeChar ? m_cEscapeChar : '0');
  
  vTrace("delimiter %c\n"_Stz,
         m_cFieldDelimiter ? m_cFieldDelimiter : '0');
  
  vTrace("record_terminator %c\n"_Stz,
         m_cRecordTerminator ? m_cRecordTerminator : '0');
  
  vTrace("record_end %s\n"_Stz, aLE[m_eLineEnd]);
  
  vTrace("max_errors %lld\n"_Stz,
         m_nMaxErrors);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
        
/*----------------------------------------------------------------------------*/
/* extract the date separator                                                 */
/*----------------------------------------------------------------------------*/
bool Format::bExtractDateFormat(Stz* dateFormat)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORMAT);
  
  i8 i_Offset = 0;
  
  // leading YYYY
  if (*dateFormat->azChar() == 'Y') {
    
    // expect YYYY
    if ( (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
         (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
         (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
         (*(dateFormat + i_Offset++)->azChar() != 'Y') )
      return false;
    
    // extract format separator
    Stz* pSeparator= dateFormat + i_Offset++;
    m_cDateSeparator = *pSeparator->azChar();
    
    // is next character 'M' ?
    if (*(dateFormat + i_Offset)->azChar() == 'M') {
      
      // FORMAT should be YYYY-MM-DD
      // expect another M
      if ( (*(dateFormat + i_Offset++)->azChar() != 'M') ||
           (*(dateFormat + i_Offset++)->azChar() != 'M'))
        return false;
      
      // expect format separator
      if ( (*(dateFormat + i_Offset++)->azChar() != m_cDateSeparator) )
        return false;
      
      // expect DD
      if ( (*(dateFormat + i_Offset++)->azChar() != 'D') ||
           (*(dateFormat + i_Offset++)->azChar() != 'D') )
        return false;
      
      m_eDateComponent[0] = DC_YYYY;
      m_eDateComponent[1] = DC_MM;
      m_eDateComponent[2] = DC_DD;
      
      
    }
    else {
      // FORMAT should be YYYY-DD-MM
      // expect DD
      if ( (*(dateFormat + i_Offset++)->azChar() != 'D') ||
           (*(dateFormat + i_Offset++)->azChar() != 'D'))
        return false;
      
      // expect format separator
      if ( (*(dateFormat + i_Offset++)->azChar() != m_cDateSeparator) )
        return false;
      
      // expect MM
      if ( (*(dateFormat + i_Offset++)->azChar() != 'M') ||
           (*(dateFormat + i_Offset++)->azChar() != 'M') )
        return false;
      
      m_eDateComponent[0] = DC_YYYY;
      m_eDateComponent[1] = DC_DD;
      m_eDateComponent[2] = DC_MM;
      
    }
  }
  // leading MM
  else if (*dateFormat->azChar() == 'M') {
    
    // expect MM
    if ( (*(dateFormat + i_Offset++)->azChar() != 'M') ||
        (*(dateFormat + i_Offset++)->azChar() != 'M') )
      return false;
    
    // extract format separator
    Stz* pSeparator= dateFormat + i_Offset++;
    m_cDateSeparator = *pSeparator->azChar();
    
    // is next character 'Y' ?
    if (*(dateFormat + i_Offset)->azChar() == 'Y') {
      
      // FORMAT should be MM-YYYY-DD
      // expect YYY
      if ( (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
          (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
          (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
          (*(dateFormat + i_Offset++)->azChar() != 'Y') )
        return false;
      
      // expect format separator
      if ( (*(dateFormat + i_Offset++)->azChar() != m_cDateSeparator) )
        return false;
      
      // expect DD
      if ( (*(dateFormat + i_Offset++)->azChar() != 'D') ||
          (*(dateFormat + i_Offset++)->azChar() != 'D') )
        return false;
      
      m_eDateComponent[0] = DC_MM;
      m_eDateComponent[1] = DC_YYYY;
      m_eDateComponent[2] = DC_DD;
    }
    else {
      // FORMAT should be MM-DD-YYYY
      // expect DD
      if ( (*(dateFormat + i_Offset++)->azChar() != 'D') ||
          (*(dateFormat + i_Offset++)->azChar() != 'D'))
        return false;
      
      // expect format separator
      if ( (*(dateFormat + i_Offset++)->azChar() != m_cDateSeparator) )
        return false;
      
      // expect YYYY
      if ( (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
          (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
          (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
          (*(dateFormat + i_Offset++)->azChar() != 'Y') )
        return false;
      
      m_eDateComponent[0] = DC_MM;
      m_eDateComponent[1] = DC_DD;
      m_eDateComponent[2] = DC_YYYY;
      
    }
  }
  // leading DD
  else if (*dateFormat->azChar() == 'D') {
    
    // expect DD
    if ( (*(dateFormat + i_Offset++)->azChar() != 'D') ||
         (*(dateFormat + i_Offset++)->azChar() != 'D') )
      return false;
    
    // extract format separator
    Stz* pSeparator= dateFormat + i_Offset++;
    m_cDateSeparator = *pSeparator->azChar();
    
    // is next character 'M' ?
    if (*(dateFormat + i_Offset)->azChar() == 'M') {
      
      // FORMAT should be DD-MM-YYYY
      // expect another M
      if ( (*(dateFormat + i_Offset++)->azChar() != 'M') ||
           (*(dateFormat + i_Offset++)->azChar() != 'M') )
        return false;
      
      // expect format separator
      if ( (*(dateFormat + i_Offset++)->azChar() != m_cDateSeparator) )
        return false;
      
      // expect YYYY
      if ( (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
           (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
           (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
           (*(dateFormat + i_Offset++)->azChar() != 'Y') )
        return false;
      
      m_eDateComponent[0] = DC_DD;
      m_eDateComponent[1] = DC_MM;
      m_eDateComponent[2] = DC_YYYY;
      
    }
    else {
      
      // expect YYYY
      if ( (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
           (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
           (*(dateFormat + i_Offset++)->azChar() != 'Y') ||
           (*(dateFormat + i_Offset++)->azChar() != 'Y') )
        return false;
      
      // expect format separator
      if ( (*(dateFormat + i_Offset++)->azChar() != m_cDateSeparator) )
        return false;
      
      if ( (*(dateFormat + i_Offset++)->azChar() != 'M') ||
           (*(dateFormat + i_Offset++)->azChar() != 'M'))
        return false;
      
      m_eDateComponent[0] = DC_DD;
      m_eDateComponent[1] = DC_YYYY;
      m_eDateComponent[2] = DC_MM;
      
    }
  }
  else
    return false;
  
  return true;
}
        
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
