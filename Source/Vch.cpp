/*============================================================================*/
/* Vch.cpp -- varchar datum                                                   */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Vch::operator new(size_t)
{
  vFatal("fatal: Vch class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Vch::Vch(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Vch::~Vch(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this varchar less than given varchar?                                   */
/*----------------------------------------------------------------------------*/
bool Vch::operator < (Vch& xVch)
{                                                             //! to be reviewed
  
  // NEED TO CHECK FOR NULLS
  
  bool bLessThan;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) < 0)
    bLessThan = true;
  else
    bLessThan = false;
  
  ASSERT(0);
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is this varchar less than or equal to given varchar?                       */
/*----------------------------------------------------------------------------*/
bool Vch::operator <= (Vch& xVch)
{                                                             //! to be reviewed
  
  // NEED TO CHECK FOR NULLS
  
  bool bLessEqual;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) <= 0)
    bLessEqual = true;
  else
    bLessEqual = false;
  
  ASSERT(0);
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is this varchar equal to given varchar?                                    */
/*----------------------------------------------------------------------------*/
bool Vch::operator == (Vch& xVch)
{                                                             //! to be reviewed
  
  // NEED TO CHECK FOR NULLS
  
  bool bEqualTo;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) == 0)
    bEqualTo = true;
  else
    bEqualTo = false;
  
  ASSERT(0);
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is this varchar not equal to given varchar?                                */
/*----------------------------------------------------------------------------*/
bool Vch::operator != (Vch& xVch)
{                                                             //! to be reviewed
  
  // NEED TO CHECK FOR NULLS
  
  bool bNotEqual;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) != 0)
    bNotEqual = true;
  else
    bNotEqual = false;
  
  ASSERT(0);
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is this varchar greater than or equal to given varchar?                    */
/*----------------------------------------------------------------------------*/
bool Vch::operator >= (Vch& xVch)
{                                                             //! to be reviewed
  
  // NEED TO CHECK FOR NULLS
  
  bool bGreaterEqual;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) >= 0)
    bGreaterEqual = true;
  else
    bGreaterEqual = false;
  
  ASSERT(0);
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is this varchar greater than given varchar?                                */
/*----------------------------------------------------------------------------*/
bool Vch::operator > (Vch& xVch)
{                                                             //! to be reviewed
  
  // NEED TO CHECK FOR NULLS
  
  bool bGreaterThan;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) > 0)
    bGreaterThan = true;
  else
    bGreaterThan = false;
  
  ASSERT(0);
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Vch::vInitZero(void)
{
                                                                 //! to be coded
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Vch::bIsNull(void)
{  
  //vTrace("%lld\n"_Stz,*((i8*)this));
  
  if (C_NULL_VCH == *(i8*)this)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Vch::vSetNull(void)
{
  *(i8*)this = C_NULL_VCH;
  //vTrace("%lld\n"_Stz,*((i8*)this));
}

/*----------------------------------------------------------------------------*/
/*-- VLQ ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Extract and Sprint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy str datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Vch::nCopy(void* pDst, void* pSrc)
{
  if (((Vch*)pSrc)->bIsNull()) {
    ((Vch*)pDst)->vSetNull();
    return sizeof(i8);
  }
  
  i8 nBytes = StrLib::iSize((char*)pSrc);
  
  StrLib::vCopyLen((char*)pDst,(char*)pSrc, nBytes);     //! could return length
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* copy str datum                                                             */
/*----------------------------------------------------------------------------*/
void Vch::vCopy(void* pDst, void* pSrc)
{
  StrLib::vCopy((char*)pDst,(char*)pSrc);
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Vch::bExtract(char** aaSrc, Stz* aDst,
                   Column* pColumn, Formatters* pFormatters)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null
  
  Vch* aVch = (Vch*)aDst;
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(aaSrc,pColumn,pFormatters,bValidNull,false)) {
    if ( ! bValidNull )
      return false;
    aVch->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field processing
  
  i8 iColumnLength = pColumn->iColumnLength();
  if ( ! StrLib::bExtractVarchar(aaSrc,(char*)aVch,
                                 iColumnLength,
                                 pFormatters->m_cEnclosedBy_Leading,
                                 pFormatters->m_cEnclosedBy_Trailing,
                                 pFormatters->m_cEscapeChar,
                                 pFormatters->m_cFieldDelimiter,
                                 pFormatters->m_cRecordTerminator
                                 ) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  // skip trailing spaces
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Vch::bExtract2(char** aaSrc, Stz* aDst,
                    Column* pColumn, Formatters* pFormatters)
{
  return bExtract2(aaSrc, aDst, pColumn, pFormatters, false);
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Vch::bExtractDefault2(char** aaSrc, Stz* aDst,
                    Column* pColumn, Formatters* pFormatters)
{
  return bExtractDefault2(aaSrc, aDst, pColumn, pFormatters, false);
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Vch::bExtract2(char** aaSrc, Stz* aDst,
                    Column* pColumn, Formatters* pFormatters,
                    bool bAllowNonPrintable)
{
  Vch* aVch = (Vch*)aDst;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(aaSrc,pColumn,pFormatters,bValidNull,false)) {
    if ( ! bValidNull )
      return false;
    aVch->vSetNull();
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  bool bSawEnclosedLeading = DtmLib::bIsEnclosed(aaSrc,pFormatters);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field processing
  
  i8 iColumnLength = pColumn->iColumnLength();
  if ( ! StrLib::bExtractVarchar2(aaSrc,(char*)aVch,
                                  iColumnLength,
                                  bSawEnclosedLeading,
                                  pFormatters->m_cEnclosedBy_Leading,
                                  pFormatters->m_cEnclosedBy_Trailing,
                                  pFormatters->m_cEscapeChar,
                                  pFormatters->m_cFieldDelimiter,
                                  pFormatters->m_cRecordTerminator,
                                  bAllowNonPrintable
                                 ) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
    StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Vch::bExtractDefault2(char** aaSrc, Stz* aDst,
                    Column* pColumn, Formatters* pFormatters,
                    bool bAllowNonPrintable)
{
  Vch* aVch = (Vch*)aDst;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(aaSrc,pColumn,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aVch->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  //bool bSawEnclosedLeading = DtmLib::bIsEnclosed(aaSrc,pFormatters);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field processing
  
  i8 iColumnLength = pColumn->iColumnLength();
  if ( ! StrLib::bExtractVarcharDefault2(aaSrc,(char*)aVch,
                                  iColumnLength,
                                  pFormatters->m_cEscapeChar,
                                  pFormatters->m_cFieldDelimiter,
                                  bAllowNonPrintable
                                  ) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  //bool bSawEnclosedTrailing =
  //StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  //if (! bSawEnclosedTrailing)
  //  return false;
  
  // skip any blanks
  while (' ' == **aaSrc)
    (*aaSrc)++;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* ssize if datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Vch::iSize(char* aSrc)
{
  Vch* aVch = (Vch*)aSrc;
  //vTrace("%lld\n"_Stz,*((i8*)aVch));
  
  if (aVch->bIsNull())
    return sizeof(i8);
  else
    return StrLib::iSize(aSrc);
}

/*----------------------------------------------------------------------------*/
/* sprint vch datum                                                           */
/*----------------------------------------------------------------------------*/
/*
void Vch::vSprint(Stz* aDst, Stz* aSrc)
{
  // test for null
  Vch* aVch = (Vch*)aSrc;
  if (aVch->bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,'\0');
    return;
  }
  
  // get the formatters
  char cEnclosedBy_Leading  = gpAttachMap->m_xFormatters.m_cEnclosedBy_Leading;
  char cEnclosedBy_Trailing = gpAttachMap->m_xFormatters.m_cEnclosedBy_Trailing;
  char cEscapeChar = gpAttachMap->m_xFormatters.m_cEscapeChar;
  
  char* aDstChar = (char*)aDst;
  char* aSrcChar = (char*)aSrc;
  
  // put leading enclosing quote/char
  *aDstChar++ = cEnclosedBy_Leading;
  
  // copy from src to dst
  while (*aSrcChar) {
    
    if (cEnclosedBy_Leading == *aSrcChar) {
      // escape an enclosing quote/char
      *aDstChar++ = cEnclosedBy_Leading;
      *aDstChar++ = *aSrcChar++;
    }
    else {
      // copy an ordinary char
      *aDstChar++ = *aSrcChar++;
    }
  }
  
  // put trailing enclosing quote/char and \0 terminate
  *aDstChar++ = cEnclosedBy_Trailing;
  *aDstChar = '\0';
}
*/
void Vch::vSprint(Stz* aDst, Stz* aSrc)
{
  // test for null
  Vch* aVch = (Vch*)aSrc;
  if (aVch->bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,'\0');
    return;
  }
  
  char* aDstChar = (char*)aDst;
  char* aSrcChar = (char*)aSrc;
  
  // copy from src to dst
  while (*aSrcChar) {
    // copy an ordinary char
    *aDstChar++ = *aSrcChar++;
  }
  
  // put \0 
  *aDstChar = '\0';
}

void Vch::vSprintQuotes(Stz* aDst, Stz* aSrc)
{
  // test for null
  Vch* aVch = (Vch*)aSrc;
  if (aVch->bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,'\0');
    return;
  }
  
  char* aDstChar = (char*)aDst;
  char* aSrcChar = (char*)aSrc;
  
  // put leading enclosing quote/char
  *aDstChar++ = '\'';
  
  // copy from src to dst
  while (*aSrcChar) {
    
    if ('\'' == *aSrcChar) {
      // escape an enclosing quote/char
      *aDstChar++ = '\'';
      *aDstChar++ = *aSrcChar++;
    }
    else {
      // copy an ordinary char
      *aDstChar++ = *aSrcChar++;
    }
  }
  
  // put trailing enclosing quote/char and \0 terminate
  *aDstChar++ = '\'';
  *aDstChar = '\0';
}

/*----------------------------------------------------------------------------*/
/* sprint vch datum in escaped form                                           */
/*----------------------------------------------------------------------------*/
void Vch::vSprintEscaped(Stz* aDst, Stz* aSrc)
{
  // test for null
  Vch* aVch = (Vch*)aSrc;
  if (aVch->bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,'\0');
    return;
  }
  
  // get the formatters
  char cEnclosedBy_Leading  = '\''; //gpAttachMap->m_xFormatters.m_cEnclosedBy_Leading;
  char cEnclosedBy_Trailing = '\''; //gpAttachMap->m_xFormatters.m_cEnclosedBy_Trailing;
  char cEscapeChar = gpAttachMap->m_xFormatters.m_cEscapeChar;
  
  char* aDstChar = (char*)aDst;
  char* aSrcChar = (char*)aSrc;
  
  // put leading enclosing quote/char
  if (cEnclosedBy_Leading)
    *aDstChar++ = cEnclosedBy_Leading;
  
  // copy from src to dst
  while (*aSrcChar) {
    
    if (cEscapeChar == *aSrcChar) {
      // escape an escape character
      *aDstChar++ = cEscapeChar;
      *aDstChar++ = *aSrcChar++;
    }
    else
    if (cEnclosedBy_Leading == *aSrcChar) {
      // escape an enclosing quote/char
      *aDstChar++ = cEnclosedBy_Leading;
      *aDstChar++ = *aSrcChar++;
    }
    else
    if (StrLib::bEscapedSpecial(*aSrcChar)) {
      // convert special character back to escaped form
      *aDstChar++ = cEscapeChar;
      *aDstChar++ = StrLib::cEscapeInverse(*aSrcChar++);
    }
    else {
      // copy an ordinary char
      *aDstChar++ = *aSrcChar++;
    }
  }
  
  // put trailing enclosing quote/char and \0 terminate
  if (cEnclosedBy_Leading)
    *aDstChar++ = cEnclosedBy_Leading;
  *aDstChar = '\0';
}

/*----------------------------------------------------------------------------*/
/* sprint vch datum                                                           */
/*----------------------------------------------------------------------------*/
/*
void Vch::vSprintEscaped(Stz* aDst, Stz* aSrc)
{
  // test for null
  Vch* aVch = (Vch*)aSrc;
  if (aVch->bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,'\0');
    return;
  }
  
  // get the formatters
  char cEscapeChar = gpAttachMap->m_xFormatters.m_cEscapeChar;
  
  char* aDstChar = (char*)aDst;
  char* aSrcChar = (char*)aSrc;
  
  // copy from src to dst
  while (*aSrcChar) {
    
    if (cEscapeChar == *aSrcChar) {
      // escape an escape character
      *aDstChar++ = cEscapeChar;
      *aDstChar++ = *aSrcChar++;
    }
    else
  
      if (StrLib::bEscapedSpecial(*aSrcChar)) {
        // convert special character back to escaped form
        *aDstChar++ = cEscapeChar;
        *aDstChar++ = StrLib::cEscapeInverse(*aSrcChar++);
      }
      else {
        // copy an ordinary char
        *aDstChar++ = *aSrcChar++;
      }
  }
  
  // put \0 terminate
  *aDstChar = '\0';
}
*/
/*----------------------------------------------------------------------------*/
/* is the size valid ?                                                        */
/*----------------------------------------------------------------------------*/
bool Vch::bIsValidSize(i8 iSize)
{
  if (iSize > C_MAX_SIZE_VCH)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is varchar argA < aArgB?                                                   */
/*----------------------------------------------------------------------------*/
bool Vch::bLessThan(char* aArgA, char* aArgB)
{
  bool bLessThan;

  if (((Vch*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Vch*)aArgB)->bIsNull())
    bLessThan = false;
  
  else {
    
    if (StrLib::iCompare(aArgA, aArgB) < 0)
      bLessThan = true;
    else
      bLessThan = false;
  }
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is varchar argA <= aArgB?                                                  */
/*----------------------------------------------------------------------------*/
bool Vch::bLessEqual(char* aArgA, char* aArgB)
{
  bool bLessEqual;
  
  if (((Vch*)aArgA)->bIsNull() && ((Vch*)aArgB)->bIsNull())
    bLessEqual = true;
  
  else if (((Vch*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Vch*)aArgB)->bIsNull())
    bLessEqual = false;
  
  else {
    
    if (StrLib::iCompare(aArgA, aArgB) <= 0)
      bLessEqual = true;
    else
      bLessEqual = false;
  }
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is varchar argA == aArgB?                                                  */
/*----------------------------------------------------------------------------*/
bool Vch::bEqualTo(char* aArgA, char* aArgB)
{
  bool bEqualTo;

  if (StrLib::iCompare(aArgA, aArgB) == 0)
    bEqualTo = true;
  else
    bEqualTo = false;
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is varchar argA != aArgB?                                                  */
/*----------------------------------------------------------------------------*/
bool Vch::bNotEqual(char* aArgA, char* aArgB)
{
  bool bNotEqual;
  
  if (StrLib::iCompare(aArgA, aArgB) != 0)
    bNotEqual = true;
  else
    bNotEqual = false;
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is varchar argA >= aArgB?                                                  */
/*----------------------------------------------------------------------------*/
bool Vch::bGreaterEqual(char* aArgA, char* aArgB)
{
  bool bGreaterEqual;
  
  if (((Vch*)aArgA)->bIsNull() && ((Vch*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else if (((Vch*)aArgA)->bIsNull())
    bGreaterEqual = false;
  
  else if (((Vch*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else {
    if (StrLib::iCompare(aArgA, aArgB) >= 0)
      bGreaterEqual = true;
    else
      bGreaterEqual = false;
  }
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is Vch argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Vch::bGreaterThan(char* aArgA, char* aArgB)
{
  bool bGreaterThan;
  
  if (((Vch*)aArgA)->bIsNull())
    bGreaterThan = false;
  
  else if (((Vch*)aArgB)->bIsNull())
    bGreaterThan = true;
  
  else {
    if (StrLib::iCompare(aArgA, aArgB) > 0)
      bGreaterThan = true;
    else
      bGreaterThan = false;
  }
  
  return bGreaterThan;

}

/*----------------------------------------------------------------------------*/
/* cmp argA with argB (-ve: argA < argB, 0: argA == argB, +ve: argA > argB)   */
/*----------------------------------------------------------------------------*/
i8 Vch::iCmp(char* aArgA, char* aArgB)
{
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Vch*)aArgA)->bIsNull() && ((Vch*)aArgB)->bIsNull())
    return 0;
  
  if (((Vch*)aArgA)->bIsNull())
    return -1;
  
  if (((Vch*)aArgB)->bIsNull())
    return 1;
  
  i8 iCmp = StrLib::iCompare(aArgA, aArgB);
  
  return iCmp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
