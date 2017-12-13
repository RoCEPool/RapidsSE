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
void* Vch::operator new(size_t iSize)
{
  RASSERT(iSize);
  
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
Vch::~Vch(void)
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
/* extract an varchar from substr                                             */
/*----------------------------------------------------------------------------*/
//bool Vch::bExtract(char** aaSrc, Vch* aVch, Column* pColumn, Formatters* pFormatters)
//{
//  char* aChar = (char*)aVch;
//  
//  // is there data ?
//  if ( ! DtmLib::bIsThereFieldData(**aaSrc,pFormatters) ) {
//    // empty string
//    *aChar++ = '\0';
//    return true;
//  }
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // test for null               THIS IS DIFFERENT TO OTHERS.  DISCUSS WITH ALKA
//  
//  if (StrLib::iCompare(*aaSrc, "NULL", 4, C_NO_CASE) == 0) {
//    
//    char* aPtr = (*aaSrc) + 4;
//    
//    if (!*aPtr
//        || C_FS == *aPtr
//        || C_NL == *aPtr
//        || C_CR == *aPtr) {
//      
//      if ( !pColumn->bIsNullable() )
//        return false;
//      
//      aVch->vSetNull();
//      *aaSrc = aPtr;
//      return true;
//    }
//  }
////  bool bValidNull;
////  if (DtmLib::bTestForNull(aaSrc,pColumn,pFormatters,bValidNull)) {
////    if ( ! bValidNull )
////      return false;
////    aVch->vSetNull();
////    return true;
////  }
//  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // collect the csv
//  
//  i8 iLength = 0;
//  i8 iColumnLength = pColumn->iColumnLength();
//  bool bInQuotes = false;
//  
//  if ('"' == **aaSrc ||
//      '\'' == **aaSrc) {
//    bInQuotes = true;
//    (*aaSrc)++;
//  }
//  
//  if (bInQuotes) {
//    while ('"' != **aaSrc
//           && '\'' != **aaSrc
//           && C_NL != **aaSrc
//           && C_CR != **aaSrc) {
//      
//      if (iLength++ < iColumnLength)
//        *aChar++ = *(*aaSrc)++;
//      else
//        (*aaSrc)++;
//    }
//    
//    // did we see an end quote ?
//    if ('"' != **aaSrc &&
//        '\'' != **aaSrc)
//      return false;
//    
//    (*aaSrc)++;
//  }
//  else {
//    while (**aaSrc
//            && C_FS != **aaSrc
//            && C_NL != **aaSrc
//            && C_CR != **aaSrc) {
//      
//      if (iLength++ < iColumnLength)
//        *aChar++ = *(*aaSrc)++;
//      else
//        (*aaSrc)++;
//    }
//  }
//  
//  *aChar++ = '\0';
//  
//  // skip trailing spaces
//  StrLib::vSkipSpaces(aaSrc);
//    
//  return true;
//}

bool Vch::bExtract(char** aaSrc, Vch* aVch, Column* pColumn, Formatters* pFormatters)
{
  // is there data ?
  if ( ! DtmLib::bIsThereFieldData(**aaSrc,pFormatters) ) {
    // empty string
    *(char*)aVch = '\0';
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null
  
  char* aStart = *aaSrc;
  bool bValidNull;
  if (DtmLib::bTestForNull(aaSrc,pColumn,pFormatters,bValidNull,false)) {
    if ( ! bValidNull )
      return false;
    aVch->vSetNull();
    return true;
  }
  *(aaSrc) = aStart;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect the csv
  
//  i8 iLength = 0;
//  i8 iColumnLength = pColumn->iColumnLength();
//  bool bInQuotes = false;
//  
//  char* aChar = (char*)aVch;
//  
//  if ('"' == **aaSrc ||
//      '\'' == **aaSrc) {
//    bInQuotes = true;
//    (*aaSrc)++;
//  }
//  
//  if (bInQuotes) {
//    while ('"' != **aaSrc
//           && '\'' != **aaSrc
//           && C_NL != **aaSrc
//           && C_CR != **aaSrc) {
//      
//      if (iLength++ < iColumnLength)
//        *aChar++ = *(*aaSrc)++;
//      else
//        (*aaSrc)++;
//    }
//    
//    // did we see an end quote ?
//    if ('"' != **aaSrc &&
//        '\'' != **aaSrc)
//      return false;
//    
//    (*aaSrc)++;
//  }
//  else {
//    while (**aaSrc
//           && C_FS != **aaSrc
//           && C_NL != **aaSrc
//           && C_CR != **aaSrc) {
//      
//      if (iLength++ < iColumnLength)
//        *aChar++ = *(*aaSrc)++;
//      else
//        (*aaSrc)++;
//    }
//  }
//  
//  *aChar++ = '\0';

  i8 iColumnLength = pColumn->iColumnLength();
  if ( ! StrLib::bExtractVarchar(aaSrc,(char*)aVch,iColumnLength,
                                 pFormatters->m_cEnclosedBy,
                                 pFormatters->m_cEscapeChar,
                                 pFormatters->m_cFieldSeparator,
                                 pFormatters->m_cRecordTerminator
                                 ) )
    return false;
  
//    if (1 == pColumn->iColumnNo())
//     vTrace("vch: ]%s[\n"_Stz,aVch);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  // skip trailing spaces
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a FS, NL or CR
  if ( ! DtmLib::bValidFieldEnd(aaSrc,pFormatters) )
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
//void Vch::vSprint(Stz* aDst, Stz* aSrc)
//{
//  Vch* aVch = (Vch*)aSrc;
//  
//  if (aVch->bIsNull())
//    StrLib::vSprint(aDst,"null%c"_Stz,
//                    '\0');
//  else
//    StrLib::vSprint(aDst,"\'%s\'%c"_Stz,
//                  aSrc->aChar(),
//                  '\0');
//  
//}

void Vch::vSprint(Stz* aDst, Stz* aSrc)
{
  // test for null
  Vch* aVch = (Vch*)aSrc;
  if (aVch->bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,'\0');
    return;
  }
  
  // get the formatters
  char cEnclosedBy = gpAttachMap->m_xFormatters.m_cEnclosedBy;
  char cEscapeChar = gpAttachMap->m_xFormatters.m_cEscapeChar;
  
  char* aDstChar = (char*)aDst;
  char* aSrcChar = (char*)aSrc;
  
  // put leading enclosing quote/char
  *aDstChar++ = cEnclosedBy;
  
  // copy from src to dst
  while (*aSrcChar) {
    
//    if (cEscapeChar == *aSrcChar) {
//      // escape an escape character
//      *aDstChar++ = cEscapeChar;
//      *aDstChar++ = *aSrcChar++;
//    }
//    else
    if (cEnclosedBy == *aSrcChar) {
      // escape an enclosing quote/char
      *aDstChar++ = cEnclosedBy;
      *aDstChar++ = *aSrcChar++;
    }
//    else
//    if (StrLib::bEscapedSpecial(*aSrcChar)) {
//      // convert special character back to escaped form
//      *aDstChar++ = cEscapeChar;
//      *aDstChar++ = StrLib::cEscapeInverse(*aSrcChar++);
//    }
    else {
      // copy an ordinary char
      *aDstChar++ = *aSrcChar++;
    }
  }
  
  // put trailing enclosing quote/char and \0 terminate
  *aDstChar++ = cEnclosedBy;
  *aDstChar = '\0';
}

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
bool Vch::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessThan;

  if (((Vch*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Vch*)aArgB)->bIsNull())
    bLessThan = false;
  
  else {
    
    ASSERT(StrLib::iSize(aArgA) == nBytesA);
    ASSERT(StrLib::iSize(aArgB) == nBytesB);
    
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
bool Vch::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessEqual;
  
  if (((Vch*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Vch*)aArgB)->bIsNull())
    bLessEqual = false;
  
  else {
    
    ASSERT(StrLib::iSize(aArgA) == nBytesA);
    ASSERT(StrLib::iSize(aArgB) == nBytesB);
    
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
bool Vch::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEqualTo;
  
  if (((Vch*)aArgA)->bIsNull())
    bEqualTo = false;
  
  else if (((Vch*)aArgB)->bIsNull())
    bEqualTo = false;
  
  else {
    
    ASSERT(StrLib::iSize(aArgA) == nBytesA);
    ASSERT(StrLib::iSize(aArgB) == nBytesB);
    
    if (StrLib::iCompare(aArgA, aArgB) == 0)
      bEqualTo = true;
    else
      bEqualTo = false;
  }
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is varchar argA != aArgB?                                                  */
/*----------------------------------------------------------------------------*/
bool Vch::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual;
  
  if (((Vch*)aArgA)->bIsNull())
    bNotEqual = true;
  
  else if (((Vch*)aArgB)->bIsNull())
    bNotEqual = true;
  
  else {

    ASSERT(StrLib::iSize(aArgA) == nBytesA);
    ASSERT(StrLib::iSize(aArgB) == nBytesB);
    
    if (StrLib::iCompare(aArgA, aArgB) != 0)
      bNotEqual = true;
    else
      bNotEqual = false;
  }
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is varchar argA >= aArgB?                                                  */
/*----------------------------------------------------------------------------*/
bool Vch::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual;
  
  if (((Vch*)aArgA)->bIsNull())
    bGreaterEqual = false;
  
  else if (((Vch*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else {
    ASSERT(StrLib::iSize(aArgA) == nBytesA);
    ASSERT(StrLib::iSize(aArgB) == nBytesB);
    
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
bool Vch::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterThan;
  
  if (((Vch*)aArgA)->bIsNull())
    bGreaterThan = false;
  
  else if (((Vch*)aArgB)->bIsNull())
    bGreaterThan = true;
  
  else {
    ASSERT(StrLib::iSize(aArgA) == nBytesA);
    ASSERT(StrLib::iSize(aArgB) == nBytesB);
    
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
i8 Vch::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  
  if (((Vch*)aArgA)->bIsNull())
    return -1;
  
  if (((Vch*)aArgB)->bIsNull())
    return 1;
    
  ASSERT(StrLib::iSize(aArgA) == nBytesA);
  ASSERT(StrLib::iSize(aArgB) == nBytesB);
  
  i8 iCmp = StrLib::iCompare(aArgA, aArgB);
  
  return iCmp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
