/*============================================================================*/
/* String.cpp -- String (maps over String_t<>)                                */
/*============================================================================*/

#include "Base.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* String::aFooter(void)
{
  m_xHeader.vAssertHeader();

  i8 nSizeHeaderBody = sizeof(m_xHeader)
                         + sizeof(m_nArea)
                         + sizeof(m__reserved__)
                         + m_nArea;
  
  Footer* aFooter = aAdd<Footer*>(this,nSizeHeaderBody);
  
  return aFooter;
}


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* String::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: String class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
String::String(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
String::~String(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy arg into this string                                                  */
/*----------------------------------------------------------------------------*/
void String::vCatSprint(Stz* szFormat, ...)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();
  
  va_list vaArgs;
  
  char* aEnd = aAdd<char*>(m_caCharArea,nLength());
  
  va_start(vaArgs,szFormat);
  vsprintf(aEnd,(char*)szFormat,vaArgs);
  va_end(vaArgs);
}

/*----------------------------------------------------------------------------*/
/* concat argument                                                            */
/*----------------------------------------------------------------------------*/
void String::vConcat(Stz* szArg)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();
  
  StrLib::vConcat(m_caCharArea,szArg->azChar());
}


/*----------------------------------------------------------------------------*/
/* copy arg into this string                                                  */
/*----------------------------------------------------------------------------*/
void String::vCopy(Stz* szArg)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  StrLib::vCopyLen(m_caCharArea,szArg->azChar(),m_nArea - 1);

  aFooter()->vAssertFooter();
}

/*----------------------------------------------------------------------------*/
/* get size of string                                                         */
/*----------------------------------------------------------------------------*/
i8 String::nLength(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  i8 nLen = StrLib::nLength(m_caCharArea);
  
  return nLen;
}

/*----------------------------------------------------------------------------*/
/* get size of string                                                         */
/*----------------------------------------------------------------------------*/
i8 String::nArea(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  return m_nArea;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
void String::vSprint(Stz* szFormat, ...)
{
  va_list args;
  va_start(args,szFormat);
  
  vsnprintf(m_caCharArea, m_nArea, szFormat->azChar(),args);
  
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get size of string                                                         */
/*----------------------------------------------------------------------------*/
i8 String::nStackUsage(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();
  
  i8 nBytes = sizeof(_Header) + sizeof(i8) + StrLib::nSize(m_caCharArea);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get char*                                                                  */
/*----------------------------------------------------------------------------*/
char* String::azStr(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  return (char*) m_caCharArea;
}

/*----------------------------------------------------------------------------*/
/* get Stz*                                                                   */
/*----------------------------------------------------------------------------*/
Stz* String::szStz(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  return (Stz*) m_caCharArea;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
