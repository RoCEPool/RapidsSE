/*============================================================================*/
/* String_t.hpp -- fixed-length string (template)                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  template <int AREA>
  class String_t : public Object
  {
  private:
    i8   m_nArea;       // size of char area
  
    char m_caCharArea[AREA]; // str area
    
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

using namespace BASE;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int AREA>
String_t<AREA>::String_t(void)
: Object(CN_STRING_T)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  m_nArea = AREA;
  
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
template <int AREA>
String_t<AREA>::String_t(Stz* szArg)
: Object(CN_STRING_T)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  m_nArea = AREA;

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCharArea,AREA,CN_STRING_T);
#endif
  
  StrLib::vCopyLen(m_caCharArea,szArg->azChar(),AREA-1);
  
  _checkInitialisedSize_(AREA);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <int AREA>
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
template <int AREA>
String_t<AREA>::operator String*(void)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  return (String*)this;
}

/*----------------------------------------------------------------------------*/
/* auto conversion operator : get address of m_castr area                     */
/*----------------------------------------------------------------------------*/
template <int AREA>
String_t<AREA>::operator Stz*(void)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  return (Stz*)m_caCharArea;
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::operator =(i8 iArg)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  StrLib::wToString(iArg,m_caCharArea);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::operator =(Stz* szArg)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  StrLib::vCopyLen(m_caCharArea,szArg->azChar(),AREA-1);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append given char                                                          */
/*----------------------------------------------------------------------------*/
template <int AREA>
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
template <int AREA>
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
template <int AREA>
bool String_t<AREA>::bBeginsWith(Stz* szArg)
{
  return StrLib::bBeginsWith(m_caCharArea,szArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* break at this length                                                       */
/*----------------------------------------------------------------------------*/
template <int AREA>
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
template <int AREA>
byte* String_t<AREA>::ayByte(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (byte*)m_caCharArea;
}

/*----------------------------------------------------------------------------*/
/* cat given variable sequence of stz* arguments                              */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vCat(Stz* szArg, ...)
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
template <int AREA>
void String_t<AREA>::vCatSprint(Stz* szFormat, ...)
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
template <int AREA>
char* String_t<AREA>::azChar(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return (char*)m_caCharArea;
}

/*----------------------------------------------------------------------------*/
/* clip                                                                       */
/*----------------------------------------------------------------------------*/
template <int AREA>
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
template <int AREA>
void String_t<AREA>::vConcat(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vConcat(m_caCharArea,szArg->azChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test if arg is a sub-string in this string                                 */
/*----------------------------------------------------------------------------*/
template <int AREA>
bool String_t<AREA>::bContains(Stz* szArg)
{
  return StrLib::bContains(m_caCharArea,szArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* copy given stz* into this string                                           */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vCopy(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(m_caCharArea,szArg->azChar(),AREA-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given stz* argument up till (but not including)                       */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vCopyTill(Stz* szArg, char cTill)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyTill(m_caCharArea,szArg->azChar(),cTill);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument? (same case)                                    */
/*----------------------------------------------------------------------------*/
template <int AREA>
bool String_t<AREA>::bEqual(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(m_caCharArea,(const char*)szArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is this string a number?                                                   */
/*----------------------------------------------------------------------------*/
template <int AREA>
bool String_t<AREA>::bIsNumeric(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bIsNumeric(m_caCharArea);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of this str                                                     */
/*----------------------------------------------------------------------------*/
template <int AREA>
i8 String_t<AREA>::nLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::nLength(azChar());
}

/*----------------------------------------------------------------------------*/
/* put max value into this str                                                */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vMaxValue(void)
{
  StrLib::vCopy(m_caCharArea,"Z");
}

/*----------------------------------------------------------------------------*/
/* put mid value into this str                                                */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vMidValue(void)
{
  StrLib::vCopy(m_caCharArea,"M");
}

/*----------------------------------------------------------------------------*/
/* put min value into this str                                                */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vMinValue(void)
{
  StrLib::vCopy(m_caCharArea,"A");
}

/*----------------------------------------------------------------------------*/
/* null value                                                                 */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vNullValue(void)
{
}

/*----------------------------------------------------------------------------*/
/* reset string                                                               */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vReset(void)
{
  m_caCharArea[0] = '\0';
}

/*----------------------------------------------------------------------------*/
/* is same as given argument?  (ignores case)                                 */
/*----------------------------------------------------------------------------*/
template <int AREA>
bool String_t<AREA>::bSame(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bSame(m_caCharArea,(const char*)szArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get size of str in data area                                               */
/*----------------------------------------------------------------------------*/
template <int AREA>
i8 String_t<AREA>::nSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::nSize(azChar());
}

/*----------------------------------------------------------------------------*/
/* ensure max size of string                                                  */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vSizeMax(i8 nChars)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_caCharArea[nChars - 1] = '\0';
}

/*----------------------------------------------------------------------------*/
/* get space of data area                                                     */
/*----------------------------------------------------------------------------*/
template <int AREA>
i8 String_t<AREA>::nSpace(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return AREA;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vSprint(Stz* szFormat, ...)
{
  va_list vaArgs;
  
  va_start(vaArgs,szFormat);
  StrLib::vSprint(azChar(),vaArgs,szFormat->azChar());
  va_end(vaArgs);
}

/*----------------------------------------------------------------------------*/
/* sprint                                                                     */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vSprint(va_list vaArgs, Stz* szFormat)
{
  StrLib::vSprint(azChar(),vaArgs,szFormat->azChar());
}

/*----------------------------------------------------------------------------*/
/* concatenated sprint                                                        */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vSprintCat(Stz* szFormat, ...)
{
  va_list vaArgs;
  
  va_start(vaArgs,szFormat);
  StrLib::vSprintCat(azChar(),vaArgs,szFormat->azChar());
  va_end(vaArgs);
}

/*----------------------------------------------------------------------------*/
/* get pString                                                                */
/*----------------------------------------------------------------------------*/
template <int AREA>
String* String_t<AREA>::pString(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (String*) this;
}

/*----------------------------------------------------------------------------*/
/* get m_caCharArea                                                                */
/*----------------------------------------------------------------------------*/
template <int AREA>
Stz* String_t<AREA>::szStz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*)m_caCharArea;
}

/*----------------------------------------------------------------------------*/
/* convert to i8                                                              */
/*----------------------------------------------------------------------------*/
template <int AREA>
i8 String_t<AREA>::iToI8(void)
{
  i8 i = StrLib::iToI8(m_caCharArea);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert to lower case                                                      */
/*----------------------------------------------------------------------------*/
template <int AREA>
void String_t<AREA>::vToLower(void)
{
  StrLib::vToLower(m_caCharArea);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <int AREA>
Footer* String_t<AREA>::aFooter(void)
{
  _precondition_(_MAGIC_,CN_STRING_T);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
