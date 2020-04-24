/*============================================================================*/
/* SP_String.cpp - String sprint helper                          (on c stack) */
/*============================================================================*/

#include "Gadgets.h"

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
Stz* SP_String::szSample(Stz* szString, Stz* szCursor, i8 iBefore, i8 iAfter)
{
  _precondition_(_MAGIC_,CN_SPSTRING,_FOOTER_);
  
  char caTemp[1024];
  
  StrLib::vCopyLen(caTemp,szCursor->azChar(),iAfter);

  StrLib::vCopy(m_caResult,aOC[OC_CURSOR]->azChar());

  StrLib::vConcat(m_caResult,caTemp);

  if (StrLib::iSize(szCursor->azChar()) < iAfter)
    StrLib::vConcat(m_caResult,"<END>");
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
