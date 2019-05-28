/*============================================================================*/
/* String_t.hpp -- fixed-length string variable (template)                    */
/*============================================================================*/
#pragma once

namespace Correctness
{
  template <int SPACE>
  class String_t : public Object
  {
  private:
    char m_caArea[SPACE]; // Stz byte area
    
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
template <int SPACE>
String_t<SPACE>::String_t(void)
: Object(CN_STRING_T)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caArea,SPACE,CN_STRING_T);
#else
  m_caArea[0] = '\0';
#endif

  _checkInitialisedSize_(SPACE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SPACE>
String_t<SPACE>::String_t(Stz* aArg)
: Object(CN_STRING_T)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caArea,SPACE,CN_STRING_T);
#endif
  
  StrLib::vCopyLen(m_caArea,aArg->aChar(),SPACE-1);
  
  _checkInitialisedSize_(SPACE);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <int SPACE>
String_t<SPACE>::~String_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of Stz area                                                    */
/*----------------------------------------------------------------------------*/
template <int SPACE>
String_t<SPACE>::operator Stz*(void)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  return (Stz*)m_caArea;
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::operator =(i8 iArg)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  StrLib::wToString(iArg,m_caArea);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::operator =(Stz* aArg)
{
  _precondition_(_MAGIC_,CN_STRING_T,_FOOTER_);
  
  StrLib::vCopyLen(m_caArea,aArg->aChar(),SPACE-1);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append given char                                                          */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vAppend(const char cChar)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  StrLib::vConcat(m_caArea,cChar);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given string using given cursor                                     */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vAppend(char** aazCursor, Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  StrLib::vAppendString(aazCursor,aStr->aChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* break at this length                                                       */
/*----------------------------------------------------------------------------*/
template <int SPACE>
bool String_t<SPACE>::bBreak(i8 iBreakAt, i8& iPrevBreak)
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
/* get m_caArea                                                                */
/*----------------------------------------------------------------------------*/
template <int SPACE>
byte* String_t<SPACE>::ayByte(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (byte*)m_caArea;
}

/*----------------------------------------------------------------------------*/
/* cat given variable sequence of Str's                                       */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vCat(Stz* aStr, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,aStr);
  
  StrLib::vConcat(aChar(),aStr->aChar());
  
  while (Stz* aArg = va_arg(xVarArgs,Stz*))
    StrLib::vConcat(aChar(),aArg->aChar());
  
  va_end(xVarArgs);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* vCatSprint: formatted concatenated sprint                                  */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vCatSprint(Stz* aFormat, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  va_list args;
  
  char* aEnd = aAdd<char*>(aChar(),iLength());
  
  va_start(args,aFormat);
  vsprintf(aEnd,(char*)aFormat,args);
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get m_caArea                                                                */
/*----------------------------------------------------------------------------*/
template <int SPACE>
char* String_t<SPACE>::aChar(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return (char*)m_caArea;
}

/*----------------------------------------------------------------------------*/
/* concat given Stz                                                           */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vConcat(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vConcat(m_caArea,aStr->aChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* bContains: test if arg is in substr                                        */
/*----------------------------------------------------------------------------*/
template <int SPACE>
bool String_t<SPACE>::bContains(Stz* aArg)
{
  return StrLib::bContains(m_caArea,aArg->aChar());
}

/*----------------------------------------------------------------------------*/
/* copy given Stz                                                             */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vCopy(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(m_caArea,aStr->aChar(),SPACE-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given Stz, then concat variable sequence of Str's                     */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vCopyTill(Stz* aSrc, char cTill)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyTill(m_caArea,aSrc->aChar(),cTill);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument? (same case)                                    */
/*----------------------------------------------------------------------------*/
template <int SPACE>
bool String_t<SPACE>::bEqual(Stz* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(m_caArea,(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument?                                                */
/*----------------------------------------------------------------------------*/
template <int SPACE>
bool String_t<SPACE>::bIsNumeric(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bIsNumeric(m_caArea);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of str in data area                                             */
/*----------------------------------------------------------------------------*/
template <int SPACE>
i8 String_t<SPACE>::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::iLength(aChar());
}

/*----------------------------------------------------------------------------*/
/* max value                                                                  */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vMaxValue(void)
{
  StrLib::vCopy(m_caArea,"Z");
}

/*----------------------------------------------------------------------------*/
/* mid value                                                                  */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vMidValue(void)
{
  StrLib::vCopy(m_caArea,"M");
}

/*----------------------------------------------------------------------------*/
/* min value                                                                  */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vMinValue(void)
{
  StrLib::vCopy(m_caArea,"A");
}

/*----------------------------------------------------------------------------*/
/* null value                                                                 */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vNullValue(void)
{
}

/*----------------------------------------------------------------------------*/
/* reset string                                                               */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vReset(void)
{
  MemLib::vZero(m_caArea,SPACE,CN_STRING_T);                      //! TEMP-TOGO
  
  m_caArea[0] = '\0';
}

/*----------------------------------------------------------------------------*/
/* is same as given argument?  (ignores case)                                 */
/*----------------------------------------------------------------------------*/
template <int SPACE>
bool String_t<SPACE>::bSame(Stz* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bSame(m_caArea,(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get size of str in data area                                               */
/*----------------------------------------------------------------------------*/
template <int SPACE>
i8 String_t<SPACE>::nSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::iSize(aChar());
}

/*----------------------------------------------------------------------------*/
/* get space of data area                                                     */
/*----------------------------------------------------------------------------*/
template <int SPACE>
i8 String_t<SPACE>::nSpace(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return SPACE;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vSprint(Stz* aFormat, ...)
{
  va_list args;
  
  va_start(args,aFormat);
  StrLib::vSprint(aChar(),args,aFormat->aChar());
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* formatted sprint concat                                                    */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vSprintCat(Stz* aFormat, ...)
{
  va_list args;
  
  va_start(args,aFormat);
  StrLib::vSprintCat(aChar(),args,aFormat->aChar());
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get m_caArea                                                                */
/*----------------------------------------------------------------------------*/
template <int SPACE>
Stz* String_t<SPACE>::aStz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*)m_caArea;
}

/*----------------------------------------------------------------------------*/
/* convert to i8                                                              */
/*----------------------------------------------------------------------------*/
template <int SPACE>
i8 String_t<SPACE>::iToI8(void)
{
  i8 i = StrLib::iToI8(m_caArea);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert to lower case                                                      */
/*----------------------------------------------------------------------------*/
template <int SPACE>
void String_t<SPACE>::vToLower(void)
{
  StrLib::vToLower(m_caArea);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <int SPACE>
Footer* String_t<SPACE>::aFooter(void)
{
  _precondition_(_MAGIC_,CN_STRING_T);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
