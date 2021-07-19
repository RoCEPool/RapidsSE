/*============================================================================*/
/* SPmedium.cpp - Sprint buffers (medium m_caResult)                             */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SPmedium::SPmedium(void)
: StackObject(CN_SPMEDIUM)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caFormat,sizeof(m_caFormat),CN_SPMEDIUM);
  MemLib::vZero(m_caResult,sizeof(m_caResult),CN_SPMEDIUM);
#else
  m_caFormat[0] = '\0';
  m_caResult[0] = '\0';
#endif
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SPmedium::~SPmedium(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* concat given substring to result                                           */
/*----------------------------------------------------------------------------*/
void SPmedium::vCatf(Stz* szFormat, ...)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);

  va_list vaArgs;
  
  va_start(vaArgs,szFormat);

  StrLib::vSprintCat(m_caResult,vaArgs,szFormat->azChar());
  
  va_end(vaArgs);
}

/*----------------------------------------------------------------------------*/
/* concat given substring to result                                           */
/*----------------------------------------------------------------------------*/
void SPmedium::vCatf(i8 iDepth, Stz* szFormat, ...)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);

  if (iDepth)
    vCats(Stz::szTab(iDepth));
  
  va_list vaArgs;
  
  va_start(vaArgs,szFormat);

  StrLib::vSprintCat(m_caResult,vaArgs,szFormat->azChar());
  
  va_end(vaArgs);
}

/*----------------------------------------------------------------------------*/
/* concat given substring to result                                           */
/*----------------------------------------------------------------------------*/
void SPmedium::vCats(Stz* szSubstring)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);

  StrLib::vConcat(m_caResult,szSubstring->azChar());
}

/*----------------------------------------------------------------------------*/
/* concat given substring to result                                           */
/*----------------------------------------------------------------------------*/
void SPmedium::vCats(i8 iDepth, Stz* szSubstring)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);

  if (iDepth)
    vCats(Stz::szTab(iDepth));

  StrLib::vConcat(m_caResult,szSubstring->azChar());
}

/*----------------------------------------------------------------------------*/
/* concat given substring to result                                           */
/*----------------------------------------------------------------------------*/
void SPmedium::vInit(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  m_caResult[0] = '\0';
}

/*----------------------------------------------------------------------------*/
/* get Stz* of m_caResult                                                     */
/*----------------------------------------------------------------------------*/
Stz* SPmedium::szResult(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* put Stz* into m_caResult                                                   */
/*----------------------------------------------------------------------------*/
void SPmedium::vResult(Stz* szResult)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);

  StrLib::vCopy(m_caResult,szResult->azChar());
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SPmedium)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
