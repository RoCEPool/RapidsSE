/*============================================================================*/
/* Format.cpp -- set of formatting characters and rules                       */
/*============================================================================*/

#include "BASE.h"

#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Format,CN_FORMAT)
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

  m_bIsSetDateComponent       = false;
  m_bIsSetDateSeparator       = false;
  m_bIsSetEnclosedBy_Leading  = false;
  m_bIsSetEnclosedBy_Trailing = false;
  m_bIsSetEscapeChar          = false;
  m_bIsSetFieldDelimiter      = false;
  m_bIsSetLineEnd             = false;
  m_bIsSetRecordTerminator    = false;
  
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

  m_bIsSetDateComponent       = false;
  m_bIsSetDateSeparator       = false;
  m_bIsSetEnclosedBy_Leading  = false;
  m_bIsSetEnclosedBy_Trailing = false;
  m_bIsSetEscapeChar          = false;
  m_bIsSetFieldDelimiter      = false;
  m_bIsSetLineEnd             = false;
  m_bIsSetRecordTerminator    = false;
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

  m_bIsSetDateComponent       = true;
  m_bIsSetDateSeparator       = true;
  m_bIsSetEnclosedBy_Leading  = true;
  m_bIsSetEnclosedBy_Trailing = true;
  m_bIsSetEscapeChar          = true;
  m_bIsSetFieldDelimiter      = true;
  m_bIsSetLineEnd             = true;
  m_bIsSetRecordTerminator    = true;
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

  m_bIsSetDateComponent = true;
  m_bIsSetDateSeparator = true;
  return true;
}

/*----------------------------------------------------------------------------*/
/* Set the enclosedby leading character                                       */
/*----------------------------------------------------------------------------*/
void Format::vEnclosedBy_Leading(char c)
{
  m_cEnclosedBy_Leading = c;
  m_bIsSetEnclosedBy_Leading = true;
}

/*----------------------------------------------------------------------------*/
/* Set the enclosedby trailing character                                      */
/*----------------------------------------------------------------------------*/
void Format::vEnclosedBy_Trailing(char c)
{
  m_cEnclosedBy_Trailing = c;
  m_bIsSetEnclosedBy_Trailing = true;
}

/*----------------------------------------------------------------------------*/
/* Set the escape character                                                   */
/*----------------------------------------------------------------------------*/
void Format::vEscapeChar(char c)
{
  m_cEscapeChar = c;
  m_bIsSetEscapeChar = true;
}

/*----------------------------------------------------------------------------*/
/* Set the field delimiter character                                          */
/*----------------------------------------------------------------------------*/
void Format::vFieldDelimiter(char c)
{
  m_cFieldDelimiter = c;
  m_bIsSetFieldDelimiter = true;
}

/*----------------------------------------------------------------------------*/
/* Set the line ending character                                              */
/*----------------------------------------------------------------------------*/
void Format::vLineEnd(eLE eEnding)
{
  m_eLineEnd = eEnding;
  m_bIsSetLineEnd = true;
}

/*----------------------------------------------------------------------------*/
/* Set the record terminator character                                        */
/*----------------------------------------------------------------------------*/
void Format::vRecordTerminator(char c)
{
  m_cRecordTerminator = c;
  m_bIsSetRecordTerminator = true;
}

/*----------------------------------------------------------------------------*/
/* Set whether non printable characters are allowed                           */
/*----------------------------------------------------------------------------*/
void Format::vAllowNonPrintable(bool bAllow)
{
  m_bAllowNonPrintable = bAllow;
}

/*----------------------------------------------------------------------------*/
/* Set the max allowed errors                                                 */
/*----------------------------------------------------------------------------*/
void Format::vMaxErrors(i8 iVal)
{
  m_nMaxErrors = iVal;
}

/*----------------------------------------------------------------------------*/
/* Get a pointer to the array of date components                              */
/*----------------------------------------------------------------------------*/
eDC* Format::pDateComponents()
{
  return m_eDateComponent;
}
/*----------------------------------------------------------------------------*/
/* Get the date component                                                     */
/*----------------------------------------------------------------------------*/
eDC Format::eDateComponent(BASE::i8 iIdx)
{
  ASSERT(iIdx >= 0);
  ASSERT(iIdx < 3);
  return m_eDateComponent[iIdx];
}

/*----------------------------------------------------------------------------*/
/* Get the date separator character                                           */
/*----------------------------------------------------------------------------*/
char Format::cDateSeparator()
{
  return m_cDateSeparator;
}

/*----------------------------------------------------------------------------*/
/* Get the enclosedby leading character                                       */
/*----------------------------------------------------------------------------*/
char Format::cEnclosedBy_Leading()
{
  return m_cEnclosedBy_Leading;
}

/*----------------------------------------------------------------------------*/
/* Get the enclosedby trailing character                                      */
/*----------------------------------------------------------------------------*/
char Format::cEnclosedBy_Trailing()
{
  return m_cEnclosedBy_Trailing;
}

/*----------------------------------------------------------------------------*/
/* Get the escape character                                                   */
/*----------------------------------------------------------------------------*/
char Format::cEscapeChar()
{
  return m_cEscapeChar;
}

/*----------------------------------------------------------------------------*/
/* Get the field delimiter character                                          */
/*----------------------------------------------------------------------------*/
char Format::cFieldDelimiter()
{
  return m_cFieldDelimiter;
}

/*----------------------------------------------------------------------------*/
/* Get the line ending character                                              */
/*----------------------------------------------------------------------------*/
eLE  Format::eLineEnd()
{
  return m_eLineEnd;
}

/*----------------------------------------------------------------------------*/
/* Get the record terminator character                                        */
/*----------------------------------------------------------------------------*/
char Format::cRecordTerminator()
{
  return m_cRecordTerminator;
}

/*----------------------------------------------------------------------------*/
/* Get whether non printable characters are allowed                            */
/*----------------------------------------------------------------------------*/
bool Format::bAllowNonPrintable()
{
  return m_bAllowNonPrintable;
}

/*----------------------------------------------------------------------------*/
/* Get the max allowed errors                                                 */
/*----------------------------------------------------------------------------*/
i8 Format::nMaxErrors()
{
  return m_nMaxErrors;
}


/*----------------------------------------------------------------------------*/
/* Has the date separator character been set?                                 */
/*----------------------------------------------------------------------------*/
bool Format::bIsSetDateSeparator()
{
  return m_bIsSetDateSeparator;
}

/*----------------------------------------------------------------------------*/
/* Has the enclosedby leading character been set?                             */
/*----------------------------------------------------------------------------*/
bool Format::bIsSetEnclosedBy_Leading()
{
  return m_bIsSetEnclosedBy_Leading;
}

/*----------------------------------------------------------------------------*/
/* Has the enclosedby trailing character been set?                            */
/*----------------------------------------------------------------------------*/
bool Format::bIsSetEnclosedBy_Trailing()
{
  return m_bIsSetEnclosedBy_Trailing;
}

/*----------------------------------------------------------------------------*/
/* Has the escape character been set?                                         */
/*----------------------------------------------------------------------------*/
bool Format::bIsSetEscapeChar()
{
  return m_bIsSetEscapeChar;
}

/*----------------------------------------------------------------------------*/
/* Has the field delimiter character been set?                                */
/*----------------------------------------------------------------------------*/
bool Format::bIsSetFieldDelimiter()
{
  return m_bIsSetFieldDelimiter;
}

/*----------------------------------------------------------------------------*/
/* Has the line ending character been set?                                    */
/*----------------------------------------------------------------------------*/
bool Format::bIsSetLineEnd()
{
  return m_bIsSetLineEnd;
}

/*----------------------------------------------------------------------------*/
/* Has the record terminator character been set?                              */
/*----------------------------------------------------------------------------*/
bool Format::bIsSetRecordTerminator()
{
  return m_bIsSetRecordTerminator;
}


        
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
