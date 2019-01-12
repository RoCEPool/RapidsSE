/*============================================================================*/
/* String_t.hpp -- fixed-length string variable (template)                    */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  template <int SIZE>
  class String_t : public Object
  {
  private:
    char m_cArea[SIZE]; // Stz byte area
    
    // must be last
    Footer m_xFooter;
    
  public:    
    String_t(void);
    String_t(Stz* aArg);
   ~String_t(void) noexcept(false);
  
    operator Stz*(void);

    void operator =(i8);
    void operator =(Stz* aArg);
  
    void vAppend(const char cChar);
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
    bool bSame(Stz* aArg);
    void vSprint(Stz* aFormat, ...);
    Stz* aStz(void);
    i8   iToI8(void);
    void vToLower(void);

    Footer* aFooter(void);
  };
  
  void* _pStrVar(void* aAddr);
}

using namespace CorrectnessAlgebra;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
String_t<SIZE>::String_t(void)
: Object(CN_SnnVAR)
{
  _precondition_(_MAGIC_,CN_SnnVAR,_FOOTER_);
  
#ifdef __DEBUG_CHECKS
  MemLib::vZero(m_cArea,SIZE,CN_SnnVAR);
#else
  m_cArea[0] = '\0';
#endif

  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
String_t<SIZE>::String_t(Stz* aArg)
: Object(CN_SnnVAR)
{
  _precondition_(_MAGIC_,CN_SnnVAR,_FOOTER_);
  
  StrLib::vCopyLen(m_cArea,aArg->aChar(),SIZE-1);
  
  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
String_t<SIZE>::~String_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SnnVAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of Stz area                                                    */
/*----------------------------------------------------------------------------*/
template <int SIZE>
String_t<SIZE>::operator Stz*(void)
{
  _precondition_(_MAGIC_,CN_SnnVAR,_FOOTER_);
  
  return (Stz*)m_cArea;
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::operator =(i8 iArg)
{
  _precondition_(_MAGIC_,CN_SnnVAR,_FOOTER_);
  
  StrLib::wToString(iArg,m_cArea);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::operator =(Stz* aArg)
{
  _precondition_(_MAGIC_,CN_SnnVAR,_FOOTER_);
  
  StrLib::vCopyLen(m_cArea,aArg->aChar(),SIZE-1);
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
  
  StrLib::vConcat(m_cArea,cChar);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* cat given variable sequence of Str's                                       */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vCat(Stz* aStr, ...)
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
template <int SIZE>
void String_t<SIZE>::vCatSprint(Stz* aFormat, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  va_list args;
  
  char* aEnd = pAdd<char*>(aChar(),iLength());
  
  va_start(args,aFormat);
  vsprintf(aEnd,(char*)aFormat,args);
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get m_cArea                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
char* String_t<SIZE>::aChar(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return (char*)m_cArea;
}

/*----------------------------------------------------------------------------*/
/* concat given Stz                                                           */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vConcat(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vConcat(m_cArea,aStr->aChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* bContains: test if arg is in substr                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
bool String_t<SIZE>::bContains(Stz* aArg)
{
  return StrLib::bContains(m_cArea,aArg->aChar());
}

/*----------------------------------------------------------------------------*/
/* copy given Stz                                                             */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vCopy(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(m_cArea,aStr->aChar(),SIZE-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given Stz, then concat variable sequence of Str's                     */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vCopyTill(Stz* aSrc, char cTill)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyTill(m_cArea,aSrc->aChar(),cTill);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument? (same case)                                    */
/*----------------------------------------------------------------------------*/
template <int SIZE>
bool String_t<SIZE>::bEqual(Stz* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(m_cArea,(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument?                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
bool String_t<SIZE>::bIsNumeric(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bIsNumeric(m_cArea);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of str in data area                                             */
/*----------------------------------------------------------------------------*/
template <int SIZE>
i8 String_t<SIZE>::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::iLength(aChar());
}

/*----------------------------------------------------------------------------*/
/* max value                                                                  */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vMaxValue(void)
{
  StrLib::vCopy(m_cArea,"Z");
}

/*----------------------------------------------------------------------------*/
/* mid value                                                                  */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vMidValue(void)
{
  StrLib::vCopy(m_cArea,"M");
}

/*----------------------------------------------------------------------------*/
/* min value                                                                  */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vMinValue(void)
{
  StrLib::vCopy(m_cArea,"A");
}

/*----------------------------------------------------------------------------*/
/* null value                                                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vNullValue(void)
{
}

/*----------------------------------------------------------------------------*/
/* is same as given argument?  (ignores case)                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
bool String_t<SIZE>::bSame(Stz* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bSame(m_cArea,(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vSprint(Stz* aFormat, ...)
{
  va_list args;
  va_start(args,aFormat);
  
  vsprintf(aChar(),aFormat->aChar(),args);
  
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get m_cArea                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
Stz* String_t<SIZE>::aStz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*)m_cArea;
}

/*----------------------------------------------------------------------------*/
/* convert to i8                                                              */
/*----------------------------------------------------------------------------*/
template <int SIZE>
i8 String_t<SIZE>::iToI8(void)
{
  i8 i = StrLib::iToI8(m_cArea);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert to lower case                                                      */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void String_t<SIZE>::vToLower(void)
{
  StrLib::vToLower(m_cArea);
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
  _precondition_(_MAGIC_,CN_SnnVAR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
