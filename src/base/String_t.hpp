/*============================================================================*/
/* String_t.hpp -- fixed-length string (template)                             */
/*============================================================================*/
#pragma once

#include <cstdarg>

namespace BASE
{
  template <i8 AREA>
  class String_t : public Object
  {
  private:
    i8   m_nArea;            // byte length of char area/array
    i8   m__reserved__;      // reserved for size
    i8   m__reserved2__;     // reserved
    char m_caCharArea[AREA]; // area to hold chars

    // must be last
    Footer m_xFooter;
    
  public:    
    String_t(void);
    String_t(Stz* szArg);
   ~String_t(void) noexcept(false);
  
    // auto conversion
    operator Stz*(void);
    operator String*(void);

    void operator =(i8);
    void operator =(Stz* szArg);
  
    void vAppend(const char cChar);
    void vAppend(char** aazCursor, Stz* szArg);
    bool bBeginsWith(Stz* szArg);
    bool bBreak(i8 iBreakAt, i8& iPrevBreak);
    byte* ayByte(void);
    void vCat(Stz* szArg, ...);
    void vCatSprint(Stz* szFormat, ...);
    char* azChar(void);
    void vClip(i8 i_Char);
    void vConcat(Stz* szArg);
    bool bContains(Stz* szArg);
    void vCopy(Stz* szArg);
    void vCopy(Stz* szArg, i8 iLen);
    void vCopyTill(Stz* szArg, char cTill);
    bool bEqual(Stz* szArg);
    bool bIsEmpty(void);
    bool bIsInteger(void);
    i8   nLength(void);
    void vMaxValue(void);
    void vMidValue(void);
    void vMinValue(void);
    void vNullValue(void);
    void vReset(void);
    bool bSame(Stz* szArg);
    i8   nSize(void);
    void vSizeMax(i8 nSIZE);
    i8   nSpace(void);
    void vSprint(Stz* szFormat, ...);
    void vSprintCat(Stz* szFormat, ...);
    void vSprintVA(Stz* szFormat, va_list vaArgs);
    String* pString(void);
    Stz* szStz(void);
    i8   iToI8(void);
    void vToLower(void);
    void vToUpper(void);

    Footer* aFooter(void);
  };
  
  void* _pStrVar(void* aAddr);
}

using namespace BASE;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
String_t<AREA>::String_t(void)
: Object(CN_STRING_T)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  m_nArea = AREA;
  m__reserved__ = C_UNKNOWN;
  m__reserved2__ = C_UNKNOWN;
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCharArea,AREA,CN_STRING_T);
#else
  m_caCharArea[0] = '\0';
#endif

  _checkInitialisedSize_(AREA);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
String_t<AREA>::String_t(Stz* szArg)
: Object(CN_STRING_T)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  m_nArea = AREA;
  m__reserved__ = C_UNKNOWN;
  m__reserved2__ = C_UNKNOWN;

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCharArea,AREA,CN_STRING_T);
#endif
  
  StrLib::vCopyLen(m_caCharArea,szArg->azChar(),AREA-1);
  
  _checkInitialisedSize_(AREA);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
String_t<AREA>::~String_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as String*                                      */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
String_t<AREA>::operator String*(void)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  return (String*)this;
}

/*----------------------------------------------------------------------------*/
/* auto conversion operator : get address of m_castr area                     */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
String_t<AREA>::operator Stz*(void)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  return (Stz*)m_caCharArea;
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::operator =(i8 iArg)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  StrLib::wToString(iArg,m_caCharArea);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::operator =(Stz* szArg)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  StrLib::vCopyLen(m_caCharArea,szArg->azChar(),AREA-1);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append given char                                                          */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vAppend(const char cChar)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  StrLib::vConcat(m_caCharArea,cChar);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given string argument using given cursor                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vAppend(char** aazCursor, Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  StrLib::vAppendString(aazCursor,szArg->azChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test if arg is a leading sub-string of this string                         */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
bool String_t<AREA>::bBeginsWith(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return StrLib::bBeginsWith(m_caCharArea,szArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* break at this length                                                       */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
bool String_t<AREA>::bBreak(i8 iBreakAt, i8& iPrevBreak)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 iLen = nLength();
  
  i8 iBreak = iLen / iBreakAt;
  if (iBreak == iPrevBreak)
    return false;
  
  iPrevBreak = iBreak;
  return true;
}

/*----------------------------------------------------------------------------*/
/* get byte pointer to str area                                               */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
byte* String_t<AREA>::ayByte(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (byte*)m_caCharArea;
}

/*----------------------------------------------------------------------------*/
/* cat given variable sequence of stz* arguments                              */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vCat(Stz* szArg, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vConcat(m_caCharArea,szArg->azChar());
  
  
  va_list xVarArgs;
  
  va_start(xVarArgs,szArg);
  
  
  while (Stz* szVal = va_arg(xVarArgs,Stz*)) {
    StrLib::vConcat(m_caCharArea,szVal->azChar());
  }
  
  va_end(xVarArgs);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* concatenated sprint                                                        */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vCatSprint(Stz* szFormat, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  va_list vaArgs;
  
  char* aEnd = aAdd<char*>(azChar(),nLength());
  
  va_start(vaArgs,szFormat);
  vsprintf(aEnd,(char*)szFormat,vaArgs);
  va_end(vaArgs);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get char pointer to str area                                               */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
char* String_t<AREA>::azChar(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return (char*)m_caCharArea;
}

/*----------------------------------------------------------------------------*/
/* clip                                                                       */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vClip(i8 i_Char)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  if (i_Char < m_nArea)
    m_caCharArea[i_Char] = '\0';
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* concat given stz*                                                          */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vConcat(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vConcat(m_caCharArea,szArg->azChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test if arg is a sub-string in this string                                 */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
bool String_t<AREA>::bContains(Stz* szArg)
{
  return StrLib::bContains(m_caCharArea,szArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* copy given stz* into this string                                           */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vCopy(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(m_caCharArea,szArg->azChar(),AREA-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given stz* up to a given length into this string                      */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vCopy(Stz* szArg, i8 iLen)
{
  _precondition_(_MAGIC_,_FOOTER_);

  i8 nBytes = (iLen > AREA-1 ? AREA-1 : iLen);

  StrLib::vCopyLen(m_caCharArea,szArg->azChar(),nBytes);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given stz* argument up till (but not including)                       */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vCopyTill(Stz* szArg, char cTill)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyTill(m_caCharArea,szArg->azChar(),cTill);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument? (same case)                                    */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
bool String_t<AREA>::bEqual(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(m_caCharArea,(const char*)szArg);
  
  return b;
}


/*----------------------------------------------------------------------------*/
/* is this an empty string (i.e. "". Not to be confused with a null ptr)      */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
bool String_t<AREA>::bIsEmpty(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_caCharArea[0] == 0;
}

/*----------------------------------------------------------------------------*/
/* is this string an integer?                                                 */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
bool String_t<AREA>::bIsInteger(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bIsInteger(m_caCharArea);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of this str                                                     */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 String_t<AREA>::nLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::nLength(azChar());
}

/*----------------------------------------------------------------------------*/
/* put max value into this str                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vMaxValue(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  StrLib::vCopy(m_caCharArea,"Z");

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* put mid value into this str                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vMidValue(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  StrLib::vCopy(m_caCharArea,"M");

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* put min value into this str                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vMinValue(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  StrLib::vCopy(m_caCharArea,"A");

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* null value                                                                 */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vNullValue(void)
{
}

/*----------------------------------------------------------------------------*/
/* reset string                                                               */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vReset(void)
{
  m_caCharArea[0] = '\0';
}

/*----------------------------------------------------------------------------*/
/* is same as given argument?  (ignores case)                                 */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
bool String_t<AREA>::bSame(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bSame(m_caCharArea,(const char*)szArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get size of str in data area                                               */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 String_t<AREA>::nSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::nSize(azChar());
}

/*----------------------------------------------------------------------------*/
/* ensure max size of string                                                  */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vSizeMax(i8 nChars)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_caCharArea[nChars - 1] = '\0';

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get space of data area                                                     */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 String_t<AREA>::nSpace(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return AREA;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vSprint(Stz* szFormat, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);

  va_list vaArgs;
  
  va_start(vaArgs,szFormat);
  StrLib::vSprint(azChar(),vaArgs,szFormat->azChar());
  va_end(vaArgs);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* concatenated sprint                                                        */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vSprintCat(Stz* szFormat, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);

  va_list vaArgs;
  
  va_start(vaArgs,szFormat);
  StrLib::vSprintCat(azChar(),vaArgs,szFormat->azChar());
  va_end(vaArgs);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* var-args sprint                                                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vSprintVA(Stz* szFormat, va_list vaArgs)
{
  _precondition_(_MAGIC_,_FOOTER_);

  StrLib::vSprint(azChar(),vaArgs,szFormat->azChar());

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get pString                                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
String* String_t<AREA>::pString(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (String*) this;
}

/*----------------------------------------------------------------------------*/
/* get m_caCharArea                                                           */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Stz* String_t<AREA>::szStz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*)m_caCharArea;
}

/*----------------------------------------------------------------------------*/
/* convert to i8                                                              */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 String_t<AREA>::iToI8(void)
{
  i8 i = StrLib::iToI8(m_caCharArea);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert to lower case                                                      */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vToLower(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  StrLib::vToLower(m_caCharArea);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* convert to upper case                                                      */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void String_t<AREA>::vToUpper(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  StrLib::vToUpper(m_caCharArea);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Footer* String_t<AREA>::aFooter(void)
{
  _precondition_(_MAGIC_,CN_STRING_T);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
