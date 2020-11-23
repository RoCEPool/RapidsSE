/*============================================================================*/
/* String_t.hpp -- fixed-length string (template)                             */
/*============================================================================*/
#pragma once

namespace Esses
{
  template <int SIZE>
  class String_t : public Object
  {
  private:
    i8   m_nSize;       // size of str
    char m_caStr[SIZE]; // str area
    
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
    bool bBreak(i8 iBreakAt, i8& iPrevBreak);
    byte* ayByte(void);
    void vCat(Stz* szArg, ...);
    void vCatSprint(Stz* szFormat, ...);
    char* azChar(void);
    void vClip(i8 i_Char);
    void vConcat(Stz* szArg);
    bool bContains(Stz* szArg);
    void vCopy(Stz* szArg);
    void vCopyTill(Stz* szArg, char cTill);
    bool bEqual(Stz* szArg);
    bool bIsNumeric(void);
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
    void vSprint(va_list vaArgs, Stz* szFormat);
    void vSprintCat(Stz* szFormat, ...);
    String* pString(void);
    Stz* szStz(void);
    i8   iToI8(void);
    void vToLower(void);

    Footer* aFooter(void);
  };
  
  void* _pStrVar(void* aAddr);
}

using namespace Correctness;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
String_t<SIZE>::String_t(void)
: Object(CN_STRING_T)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  m_nSize = SIZE;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caStr,SIZE,CN_STRING_T);
#else
  m_caStr[0] = '\0';
#endif

  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
String_t<SIZE>::String_t(Stz* szArg)
: Object(CN_STRING_T)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  m_nSize = SIZE;

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caStr,SIZE,CN_STRING_T);
#endif
  
  StrLib::vCopyLen(m_caStr,szArg->azChar(),SIZE-1);
  
  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
String_t<SIZE>::~String_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as String*                                      */
/*----------------------------------------------------------------------------*/
template <int SIZE>
String_t<SIZE>::operator String*(void)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  return (String*)this;
}

/*----------------------------------------------------------------------------*/
/* auto conversion operator : get address of m_castr area                     */
/*----------------------------------------------------------------------------*/
template <int SIZE>
String_t<SIZE>::operator Stz*(void)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  return (Stz*)m_caStr;
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::operator =(i8 iArg)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  StrLib::wToString(iArg,m_caStr);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::operator =(Stz* szArg)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  StrLib::vCopyLen(m_caStr,szArg->azChar(),SIZE-1);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append given char                                                          */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vAppend(const char cChar)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  StrLib::vConcat(m_caStr,cChar);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given string argument using given cursor                            */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vAppend(char** aazCursor, Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  StrLib::vAppendString(aazCursor,szArg->azChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* break at this length                                                       */
/*----------------------------------------------------------------------------*/
template <int SIZE>
bool String_t<SIZE>::bBreak(i8 iBreakAt, i8& iPrevBreak)
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
template <int SIZE>
byte* String_t<SIZE>::ayByte(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (byte*)m_caStr;
}

/*----------------------------------------------------------------------------*/
/* cat given variable sequence of stz* arguments                              */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vCat(Stz* szArg, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,szArg);
  
  StrLib::vConcat(azChar(),szArg->azChar());
  
  while (Stz* szVal = va_arg(xVarArgs,Stz*))
    StrLib::vConcat(azChar(),szVal->azChar());
  
  va_end(xVarArgs);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* concatenated sprint                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vCatSprint(Stz* szFormat, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  va_list vaArgs;
  
  char* aEnd = aAdd<char*>(azChar(),nLength());
  
  va_start(vaArgs,szFormat);
  vsprintf(aEnd,(char*)szFormat,vaArgs);
  va_end(vaArgs);
}

/*----------------------------------------------------------------------------*/
/* get char pointer to str area                                               */
/*----------------------------------------------------------------------------*/
template <int SIZE>
char* String_t<SIZE>::azChar(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return (char*)m_caStr;
}

/*----------------------------------------------------------------------------*/
/* clip                                                                       */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vClip(i8 i_Char)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  if (i_Char < m_nSize)
    m_caStr[i_Char] = '\0';
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* concat given stz*                                                          */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vConcat(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vConcat(m_caStr,szArg->azChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test if arg is a sub-string in this string                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
bool String_t<SIZE>::bContains(Stz* szArg)
{
  return StrLib::bContains(m_caStr,szArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* copy given stz* into this string                                           */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vCopy(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(m_caStr,szArg->azChar(),SIZE-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given stz* argument up till (but not including)                       */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vCopyTill(Stz* szArg, char cTill)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyTill(m_caStr,szArg->azChar(),cTill);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument? (same case)                                    */
/*----------------------------------------------------------------------------*/
template <int SIZE>
bool String_t<SIZE>::bEqual(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(m_caStr,(const char*)szArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is this string a number?                                                   */
/*----------------------------------------------------------------------------*/
template <int SIZE>
bool String_t<SIZE>::bIsNumeric(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bIsNumeric(m_caStr);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of this str                                                     */
/*----------------------------------------------------------------------------*/
template <int SIZE>
i8 String_t<SIZE>::nLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::nLength(azChar());
}

/*----------------------------------------------------------------------------*/
/* put max value into this str                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vMaxValue(void)
{
  StrLib::vCopy(m_caStr,"Z");
}

/*----------------------------------------------------------------------------*/
/* put mid value into this str                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vMidValue(void)
{
  StrLib::vCopy(m_caStr,"M");
}

/*----------------------------------------------------------------------------*/
/* put min value into this str                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vMinValue(void)
{
  StrLib::vCopy(m_caStr,"A");
}

/*----------------------------------------------------------------------------*/
/* null value                                                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vNullValue(void)
{
}

/*----------------------------------------------------------------------------*/
/* reset string                                                               */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vReset(void)
{
  m_caStr[0] = '\0';
}

/*----------------------------------------------------------------------------*/
/* is same as given argument?  (ignores case)                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
bool String_t<SIZE>::bSame(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bSame(m_caStr,(const char*)szArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get size of str in data area                                               */
/*----------------------------------------------------------------------------*/
template <int SIZE>
i8 String_t<SIZE>::nSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::nSize(azChar());
}

/*----------------------------------------------------------------------------*/
/* ensure max size of string                                                  */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vSizeMax(i8 nChars)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_caStr[nChars - 1] = '\0';
}

/*----------------------------------------------------------------------------*/
/* get space of data area                                                     */
/*----------------------------------------------------------------------------*/
template <int SIZE>
i8 String_t<SIZE>::nSpace(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return SIZE;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vSprint(Stz* szFormat, ...)
{
  va_list vaArgs;
  
  va_start(vaArgs,szFormat);
  StrLib::vSprint(azChar(),vaArgs,szFormat->azChar());
  va_end(vaArgs);
}

/*----------------------------------------------------------------------------*/
/* sprint                                                                     */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vSprint(va_list vaArgs, Stz* szFormat)
{
  StrLib::vSprint(azChar(),vaArgs,szFormat->azChar());
}

/*----------------------------------------------------------------------------*/
/* concatenated sprint                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vSprintCat(Stz* szFormat, ...)
{
  va_list vaArgs;
  
  va_start(vaArgs,szFormat);
  StrLib::vSprintCat(azChar(),vaArgs,szFormat->azChar());
  va_end(vaArgs);
}

/*----------------------------------------------------------------------------*/
/* get pString                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
String* String_t<SIZE>::pString(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (String*) this;
}

/*----------------------------------------------------------------------------*/
/* get m_caStr                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
Stz* String_t<SIZE>::szStz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*)m_caStr;
}

/*----------------------------------------------------------------------------*/
/* convert to i8                                                              */
/*----------------------------------------------------------------------------*/
template <int SIZE>
i8 String_t<SIZE>::iToI8(void)
{
  i8 i = StrLib::iToI8(m_caStr);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert to lower case                                                      */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vToLower(void)
{
  StrLib::vToLower(m_caStr);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <int SIZE>
Footer* String_t<SIZE>::aFooter(void)
{
  _precondition_(_MAGIC_,CN_STRING_T);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
