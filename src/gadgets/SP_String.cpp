/*============================================================================*/
/* SP_String.cpp - String sprint helper                          (on c stack) */
/*============================================================================*/

#include "Gadgets.h"

#include "eOC.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_String::SP_String(void)
: SP(CN_SPSTRING)
{
  _precondition_(_MAGIC_,CN_SPSTRING,_FOOTER_);
  
  _checkInitialised_(CN_SPSTRING);
  
  _postcondition_(_MAGIC_,CN_SPSTRING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_String::~SP_String(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPSTRING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint a inspect of string and return Stz* to result                       */
/*----------------------------------------------------------------------------*/
Stz* SP_String::szChar(char cChr)
{
  _precondition_(_MAGIC_,CN_SPSTRING,_FOOTER_);
  
  char* azResult = m_caResult;

  if (!cChr) {
    *azResult++ = '\\';
    *azResult++ = '0';
  }
  else if (' ' == cChr || isgraph(cChr)) {
    *azResult++ = cChr;
  }
  else if ('\f' == cChr) {
    *azResult++ = '\\';
    *azResult++ = 'f';
  }
  else if ('\n' == cChr) {
    *azResult++ = '\\';
    *azResult++ = 'n';
  }
  else if ('\r' == cChr) {
    *azResult++ = '\\';
    *azResult++ = 'r';
  }
  else if ('\t' == cChr) {
    *azResult++ = '\\';
    *azResult++ = 't';
  }
  else {
    *azResult++ = '\\';
    *azResult++ = '?';
  }
    
  *azResult = '\0';
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint a inspect of string and return Stz* to result                       */
/*----------------------------------------------------------------------------*/
Stz* SP_String::szInspect(char* azString, i8 nAfter, bool bQuitNL)
{
  _precondition_(_MAGIC_,CN_SPSTRING,_FOOTER_);
  
  char cChr = 0;
  char* azResult = m_caResult;
  
  for (i8 nChars = 0; nChars < nAfter; nChars++) {
    
    cChr = *((char*)azString + nChars);
    
    if (!cChr)
      break;
    
    if (' ' == cChr || isgraph(cChr)) {
      *azResult++ = cChr;
      continue;
    }
    
    if ('\f' == cChr) {
      *azResult++ = '\\';
      *azResult++ = 'f';
      continue;
    }
    
    if ('\n' == cChr) {
      *azResult++ = '\\';
      *azResult++ = 'n';
      if (bQuitNL)
        break;
      continue;
    }
    
    if ('\r' == cChr) {
      *azResult++ = '\\';
      *azResult++ = 'r';
      continue;
    }

    if ('\t' == cChr) {
      *azResult++ = '\\';
      *azResult++ = 't';
      continue;
    }

    *azResult++ = '\\';
    *azResult++ = '?';
  }
  
  if (cChr) {
    *azResult++ = '.';
    *azResult++ = '.';
    *azResult++ = '.';
  }
  
  *azResult = '\0';
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint String and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_String::szRight(Stz* szString, i8 iLength)
{
  _precondition_(_MAGIC_,CN_SPSTRING,_FOOTER_);
  
  StrLib::vSprint(m_caFormat,"%%%llds"_Stz,iLength);

  StrLib::vSprint(m_caResult,(Stz*)m_caFormat,szString);
  
  if (StrLib::bClip(m_caResult,iLength))
    StrLib::vReplaceTail(m_caResult,(char*)"..."_Stz);

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint a sample of string and return Stz* to result                        */
/*----------------------------------------------------------------------------*/
Stz* SP_String::szSample(Stz* szString, Stz* szCursor, i8 iAfter, i8 iBefore)
{
  _precondition_(_MAGIC_,CN_SPSTRING,_FOOTER_);
  
  char caSubStrBefore[1024];
  char caEscapedBefore[1024];

  if (-1 == iAfter) {
    iAfter = 256;
    iBefore = 256;
  }
  
  Stz* szStart = aSubtract<Stz*>(szCursor,iBefore);
  
  if (szStart < szString)
    szStart = szString;
  else
    Resources::vDebugPoint();
  
  i8 nBytes = nDifference(szStart,szCursor);
  
  StrLib::vCopyLen(caSubStrBefore,szStart->azChar(),nBytes);
  StrLib::vEscape(caEscapedBefore,caSubStrBefore);
  
  StrLib::vCopy(m_caResult,caEscapedBefore);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  char caSubStrAfter[1024];
  char caEscapedAfter[1024];

  StrLib::vCopyLen(caSubStrAfter,szCursor->azChar(),iAfter);
  StrLib::vEscape(caEscapedAfter,caSubStrAfter);
  
  StrLib::vConcat(m_caResult,aOC[OC_CURSOR]->azChar());
  StrLib::vConcat(m_caResult,caEscapedAfter);

  if (StrLib::nSize(szCursor->azChar()) < iAfter)
    StrLib::vConcat(m_caResult,"\\0");
  else
    StrLib::vConcat(m_caResult,"...");

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_String);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
