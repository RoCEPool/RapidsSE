/*============================================================================*/
/* StrVal2.cpp -- null-terminated string value                                */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StrVal2* Values::_aStrVal2(void* aAddr)
{
  StrVal2* aStrVal2 = reinterpret_cast<StrVal2*>(aAddr);
  
  RASSERT( ! aStrVal2
          || CN_STRVAL == aStrVal2->eClass()
          || CN_STRVAR == aStrVal2->eClass()
          );  
  
  return aStrVal2;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StrVal2::operator new(size_t)
{
  vFatal("fatal: StrVal2 class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal2::StrVal2(void)
: Value(CN_STRVAL)
{
  _precondition_(_MAGIC_);
  
  vFatal("fatal: StrVal2 class cannot be a local variable\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get address of Stz area                                                    */
/*----------------------------------------------------------------------------*/
StrVal2::operator Stz*(void)
{
  _precondition_(_MAGIC_);
  
  // return address to char byte area
  return reinterpret_cast<Stz*>(pAdd<char*>(this,sizeof(StrVal2)));
}


/*----------------------------------------------------------------------------*/
/* get reference to Stz area                                                  */
/*----------------------------------------------------------------------------*/
StrVal2::operator Stz&(void)
{
  _precondition_(_MAGIC_);
  
  // return reference to char byte area
  return reinterpret_cast<Stz&>(*(pAdd<char*>(this,sizeof(StrVal2))));
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate number of bytes required to hold str of given length             */
/*----------------------------------------------------------------------------*/
i8 StrVal2::nBytesRequired(i8 iStrLength)
{
  i8 nBytes = sizeof(StrVal2) + iStrLength + 1;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* = operator                                                                 */
/*----------------------------------------------------------------------------*/
void StrVal2::operator =(i8 iArg)
{
  StrLib::wToString(iArg,aChar());
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal2::operator =(StrVal2& rArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(rArg);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal2::operator =(StrVal2* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(aArg->aStz());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal2::operator =(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(aStr);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get size of data area                                                      */
/*----------------------------------------------------------------------------*/
i8 StrVal2::nAreaSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set data area size (based on the string size)                              */
/*----------------------------------------------------------------------------*/
void StrVal2::vAreaSize(void)
{
  //_precondition_(_MAGIC_,_FOOTER_);
  
  i8 nBytes = StrLib::iSize(aChar());
  
  m_xWsp.vUsed(nBytes);
}

/*----------------------------------------------------------------------------*/
/* assign given str                                                           */
/*----------------------------------------------------------------------------*/
void StrVal2::vAssign(Stz* aStr)
{
  //ASSERT(m_nAreaSize);
  
  StrLib::vCopy(aChar(),aStr->aChar());
  
  vAreaSize();
}

/*----------------------------------------------------------------------------*/
/* cat given variable sequence of Str's                                       */
/*----------------------------------------------------------------------------*/
void StrVal2::vCat(Stz* aStr, ...)
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
void StrVal2::vCatSprint(Stz* aFormat, ...)
{
  va_list args;
  
  char* aEnd = pAdd<char*>(aChar(),iLength());
  
  va_start(args,aFormat);
  vsprintf(aEnd,(char*)aFormat,args);
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* concat given Stz                                                           */
/*----------------------------------------------------------------------------*/
void StrVal2::vConcat(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vConcat(aChar(),aStr->aChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* bContains: test if arg is in substr                                        */
/*----------------------------------------------------------------------------*/
bool StrVal2::bContains(Stz* aArg)
{
  return StrLib::bContains(aChar(),aArg->aChar());
}

/*----------------------------------------------------------------------------*/
/* copy given Stz, then concat variable sequence of Str's                     */
/*----------------------------------------------------------------------------*/
void StrVal2::vCopyTill(Stz* aSrc, char cTill)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyTill(aChar(),aSrc->aChar(),cTill);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given Stz                                                             */
/*----------------------------------------------------------------------------*/
void StrVal2::vCopy(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(aChar(),aStr->aChar(),m_xWsp.nSize()-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given Stz, then concat variable sequence of Str's                     */
/*----------------------------------------------------------------------------*/
void StrVal2::vCopyCat(Stz* aStr, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,aStr);
  
  StrLib::vCopyLen(aChar(),aStr->aChar(),m_xWsp.nSize()-1);

  while (Stz* aArg = va_arg(xVarArgs,Stz*))
    StrLib::vConcat(aChar(),aArg->aChar());
  
  va_end(xVarArgs);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test if string ends with given suffix                                      */
/*----------------------------------------------------------------------------*/
bool StrVal2::bEnds(Stz* aSuffix)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEnds(aChar(),aSuffix->aChar());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool StrVal2::bIsEqual(StrVal2& sArg, i8 iCase)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(aChar(),sArg.aChar(),iCase);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument? (same case)                                    */
/*----------------------------------------------------------------------------*/
bool StrVal2::bEqual(Stz* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(aChar(),(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of str in data area                                             */
/*----------------------------------------------------------------------------*/
i8 StrVal2::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::iLength(aChar());
}

/*----------------------------------------------------------------------------*/
/* get char*                                                                  */
/*----------------------------------------------------------------------------*/
char* StrVal2::aChar(void)
{
  char* aArea = m_xWsp.aArea();
  
  return aArea;
}

/*----------------------------------------------------------------------------*/
/* is same as given argument?  (ignores case)                                 */
/*----------------------------------------------------------------------------*/
bool StrVal2::bSame(Stz* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bSame(aChar(),(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get size of this StrVal2 in bytes                                           */
/*----------------------------------------------------------------------------*/
i8 StrVal2::nSize(void)
{
  i8 nBytes = sizeof(StrVal2) + m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
void StrVal2::vSprint(Stz* aFormat, ...)
{
  va_list args;
  va_start(args,aFormat);
  
  vsprintf(aChar(),aFormat->aChar(),args);
  
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get adddress of Stz area                                                   */
/*----------------------------------------------------------------------------*/
Stz* StrVal2::aStz(void)
{
  char* aAddr = pAdd<char*>(this,sizeof(StrVal2));
  
  return reinterpret_cast<Stz*>(aAddr);
}

/*----------------------------------------------------------------------------*/
/* convert to i8                                                              */
/*----------------------------------------------------------------------------*/
i8 StrVal2::iToI8(void)
{
  i8 i = StrLib::iToI8(aChar());
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert to lower case                                                      */
/*----------------------------------------------------------------------------*/
void StrVal2::vToLower(void)
{
  StrLib::vToLower(aChar());
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void StrVal2::vToString(Stz* aStz)
{
  StrLib::vCopy(aStz->aChar(),aChar());
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool StrVal2::bIsNumeric(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bIsNumeric(aChar());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* StrVal2::aFooter(void)
{
  _precondition_(_MAGIC_);
  
  Footer* aFooter = pAdd<Footer*>(this,sizeof(StrVal2) + m_xWsp.nUsed());
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
