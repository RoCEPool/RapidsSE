/*============================================================================*/
/* String_t.hpp -- fixed-length string variable (template)                    */
/*============================================================================*/
#pragma once

namespace Esses
{
  template <int BYTES>
  class String_t : public Object
  {
  private:
    char m_caStr[BYTES]; // str area
    
    // must be last
    Footer m_xFooter;
    
  public:    
    String_t(void);
    String_t(Stz* aArg);
   ~String_t(void) noexcept(false);
  
    // auto conversion
    operator Stz*(void);

    void operator =(i8);
    void operator =(Stz* aArg);
  
    void vAppend(const char cChar);
    void vAppend(char** aazCursor, Stz* aStr);
    bool bBreak(i8 iBreakAt, i8& iPrevBreak);
    byte* ayByte(void);
    void vCat(Stz* aStr, ...);
    void vCatSprint(Stz* aFormat, ...);
    char* aChar(void);
    void vConcat(Stz* aStr);
    bool bContains(Stz* aArg);
    void vCopy(Stz* aStr);
    void vCopyTill(Stz* aSrc, char cTill);
    bool bEqual(Stz* aArg);
    bool bIsNumeric(void);
    i8   iLength(void);
    void vMaxValue(void);
    void vMidValue(void);
    void vMinValue(void);
    void vNullValue(void);
    void vReset(void);
    bool bSame(Stz* aArg);
    i8   nSize(void);
    void vSizeMax(i8 nBytes);
    i8   nSpace(void);
    void vSprint(Stz* aFormat, ...);
    void vSprintCat(Stz* aFormat, ...);
    Stz* aStz(void);
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
template <int BYTES>
String_t<BYTES>::String_t(void)
: Object(CN_STRING_T)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caStr,BYTES,CN_STRING_T);
#else
  m_caStr[0] = '\0';
#endif

  _checkInitialisedSize_(BYTES);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int BYTES>
String_t<BYTES>::String_t(Stz* aArg)
: Object(CN_STRING_T)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caStr,BYTES,CN_STRING_T);
#endif
  
  StrLib::vCopyLen(m_caStr,aArg->azChar(),BYTES-1);
  
  _checkInitialisedSize_(BYTES);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <int BYTES>
String_t<BYTES>::~String_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of Stz area                                                    */
/*----------------------------------------------------------------------------*/
template <int BYTES>
String_t<BYTES>::operator Stz*(void)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  return (Stz*)m_caStr;
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::operator =(i8 iArg)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  StrLib::wToString(iArg,m_caStr);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::operator =(Stz* aArg)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  StrLib::vCopyLen(m_caStr,aArg->azChar(),BYTES-1);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append given char                                                          */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vAppend(const char cChar)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  StrLib::vConcat(m_caStr,cChar);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given string using given cursor                                     */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vAppend(char** aazCursor, Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  StrLib::vAppendString(aazCursor,aStr->azChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* break at this length                                                       */
/*----------------------------------------------------------------------------*/
template <int BYTES>
bool String_t<BYTES>::bBreak(i8 iBreakAt, i8& iPrevBreak)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 iLen = iLength();
  
  i8 iBreak = iLen / iBreakAt;
  if (iBreak == iPrevBreak)
    return false;
  
  iPrevBreak = iBreak;
  return true;
}

/*----------------------------------------------------------------------------*/
/* get m_caStr                                                                */
/*----------------------------------------------------------------------------*/
template <int BYTES>
byte* String_t<BYTES>::ayByte(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (byte*)m_caStr;
}

/*----------------------------------------------------------------------------*/
/* cat given variable sequence of Str's                                       */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vCat(Stz* aStr, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,aStr);
  
  StrLib::vConcat(aChar(),aStr->azChar());
  
  while (Stz* aArg = va_arg(xVarArgs,Stz*))
    StrLib::vConcat(aChar(),aArg->azChar());
  
  va_end(xVarArgs);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* vCatSprint: formatted concatenated sprint                                  */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vCatSprint(Stz* aFormat, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  va_list args;
  
  char* aEnd = aAdd<char*>(aChar(),iLength());
  
  va_start(args,aFormat);
  vsprintf(aEnd,(char*)aFormat,args);
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get m_caStr                                                                */
/*----------------------------------------------------------------------------*/
template <int BYTES>
char* String_t<BYTES>::aChar(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return (char*)m_caStr;
}

/*----------------------------------------------------------------------------*/
/* concat given Stz                                                           */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vConcat(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vConcat(m_caStr,aStr->azChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* bContains: test if arg is in substr                                        */
/*----------------------------------------------------------------------------*/
template <int BYTES>
bool String_t<BYTES>::bContains(Stz* aArg)
{
  return StrLib::bContains(m_caStr,aArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* copy given Stz                                                             */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vCopy(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(m_caStr,aStr->azChar(),BYTES-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given Stz, then concat variable sequence of Str's                     */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vCopyTill(Stz* aSrc, char cTill)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyTill(m_caStr,aSrc->azChar(),cTill);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument? (same case)                                    */
/*----------------------------------------------------------------------------*/
template <int BYTES>
bool String_t<BYTES>::bEqual(Stz* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(m_caStr,(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument?                                                */
/*----------------------------------------------------------------------------*/
template <int BYTES>
bool String_t<BYTES>::bIsNumeric(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bIsNumeric(m_caStr);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of str in data area                                             */
/*----------------------------------------------------------------------------*/
template <int BYTES>
i8 String_t<BYTES>::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::iLength(aChar());
}

/*----------------------------------------------------------------------------*/
/* max value                                                                  */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vMaxValue(void)
{
  StrLib::vCopy(m_caStr,"Z");
}

/*----------------------------------------------------------------------------*/
/* mid value                                                                  */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vMidValue(void)
{
  StrLib::vCopy(m_caStr,"M");
}

/*----------------------------------------------------------------------------*/
/* min value                                                                  */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vMinValue(void)
{
  StrLib::vCopy(m_caStr,"A");
}

/*----------------------------------------------------------------------------*/
/* null value                                                                 */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vNullValue(void)
{
}

/*----------------------------------------------------------------------------*/
/* reset string                                                               */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vReset(void)
{
  m_caStr[0] = '\0';
}

/*----------------------------------------------------------------------------*/
/* is same as given argument?  (ignores case)                                 */
/*----------------------------------------------------------------------------*/
template <int BYTES>
bool String_t<BYTES>::bSame(Stz* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bSame(m_caStr,(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get size of str in data area                                               */
/*----------------------------------------------------------------------------*/
template <int BYTES>
i8 String_t<BYTES>::nSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::iSize(aChar());
}

/*----------------------------------------------------------------------------*/
/* ensure max size of string                                                  */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vSizeMax(i8 nChars)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_caStr[nChars - 1] = '\0';
}

/*----------------------------------------------------------------------------*/
/* get space of data area                                                     */
/*----------------------------------------------------------------------------*/
template <int BYTES>
i8 String_t<BYTES>::nSpace(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return BYTES;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vSprint(Stz* aFormat, ...)
{
  va_list args;
  
  va_start(args,aFormat);
  StrLib::vSprint(aChar(),args,aFormat->azChar());
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* formatted sprint concat                                                    */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vSprintCat(Stz* aFormat, ...)
{
  va_list args;
  
  va_start(args,aFormat);
  StrLib::vSprintCat(aChar(),args,aFormat->azChar());
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get m_caStr                                                                */
/*----------------------------------------------------------------------------*/
template <int BYTES>
Stz* String_t<BYTES>::aStz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*)m_caStr;
}

/*----------------------------------------------------------------------------*/
/* convert to i8                                                              */
/*----------------------------------------------------------------------------*/
template <int BYTES>
i8 String_t<BYTES>::iToI8(void)
{
  i8 i = StrLib::iToI8(m_caStr);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert to lower case                                                      */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void String_t<BYTES>::vToLower(void)
{
  StrLib::vToLower(m_caStr);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <int BYTES>
Footer* String_t<BYTES>::aFooter(void)
{
  _precondition_(_MAGIC_,CN_STRING_T);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
