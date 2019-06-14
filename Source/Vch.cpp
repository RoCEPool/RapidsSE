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
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Vch::~Vch(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Vch::bIsNull(void)
{  
  //vTrace("%lld\n"_Stz,*((i8*)this));
  
  if (C_DATUM_NULL == *(i8*)this)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Vch::vSetNull(void)
{
  *(i8*)this = C_DATUM_NULL;
  //vTrace("%lld\n"_Stz,*((i8*)this));
}

/*----------------------------------------------------------------------------*/
/* sprint                                                                     */
/*----------------------------------------------------------------------------*/
void Vch::vSprint(Stz* aDst, Stz* aSrc, bool bQuoted)
{
  if (bIsNull()) {
    StrLib::vSprint(aDst,"null"_Stz);
    return;
  }

  if (bQuoted)
    vSprintQuotes(aDst,aSrc);
  else
    vSprintEscaped(aDst,aSrc);
}


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
  char cEnclosedBy_Leading  = gpAttachMap->m_oFormatters.m_cEnclosedBy_Leading;
  char cEnclosedBy_Trailing = gpAttachMap->m_oFormatters.m_cEnclosedBy_Trailing;
  char cEscapeChar = gpAttachMap->m_oFormatters.m_cEscapeChar;
  
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
  char cEnclosedBy_Leading  = '\''; //gpAttachMap->m_oFormatters.m_cEnclosedBy_Leading;
  char cEnclosedBy_Trailing = '\''; //gpAttachMap->m_oFormatters.m_cEnclosedBy_Trailing;
  char cEscapeChar = gpAttachMap->m_oFormatters.m_cEscapeChar;
  
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
  char cEscapeChar = gpAttachMap->m_oFormatters.m_cEscapeChar;
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
