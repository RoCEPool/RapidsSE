/*============================================================================*/
/* StrVal.cpp -- null-terminated string value                                 */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StrVal* Values::_aStrVal(void* aAddr)
{
  StrVal* aStrVal = reinterpret_cast<StrVal*>(aAddr);
  
  RASSERT( ! aStrVal
          || CN_STRVAL == aStrVal->eClass()
          || CN_STRVAR == aStrVal->eClass()
          );  
  
  return aStrVal;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StrVal::operator new(size_t)
{
  vFatal("fatal: StrVal class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
//void* StrVal::operator new(size_t, StrVal* aThis)
//{
//  return aThis;
//}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(void)
: Value(CN_STRVAL)
{
  _precondition_(_MAGIC_);
  
  vFatal("fatal: StrVal class cannot be a local variable\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
//StrVal::StrVal(eCN eClassNo)
//: Value(eClassNo)
//{
//  _precondition_(_MAGIC_);
//}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
//StrVal::StrVal(eCN eClassNo, i8 nBytes)
//: Value(eClassNo)
//{
//  _precondition_(_MAGIC_,eClassNo);
//
//  m_xWsp.vUsed(nBytes);
//
//#ifdef __DEBUG_CHECKS
//  m_xWsp.vZero(eClassNo);
//#endif
//
//  *m_xWsp.aArea() = '\0';
//}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
//StrVal::StrVal(StrVal* aThis)
//: Value(CN_STRVAL)
//{
//  _precondition_(_MAGIC_,CN_STRVAL);
//
//  ASSERT(this == aThis);
//
//  _checkInitialised_(CN_STRVAL);
//}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
//StrVal::StrVal(StrVal* aThis, i8 nBytes)
//: Value(CN_STRVAL)
//{
//  _precondition_(_MAGIC_,CN_STRVAL);
//
//  ASSERT(this == aThis);
//
//  m_xWsp.vUsed(nBytes);
//
//#ifdef __DEBUG_CHECKS
//  m_xWsp.vZero(CN_STRVAL);
//#endif
//
//  *m_xWsp.aArea() = '\0';
//
//  _checkInitialised_(CN_STRVAL);
//}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
//StrVal::~StrVal(void) noexcept(false)
//{
//  _precondition_(_MAGIC_,_FOOTER_);
//}

/*----------------------------------------------------------------------------*/
/* get address of Stz area                                                    */
/*----------------------------------------------------------------------------*/
StrVal::operator Stz*(void)
{
  _precondition_(_MAGIC_);
  
  // return address to char byte area
  return reinterpret_cast<Stz*>(pAdd<char*>(this,sizeof(StrVal)));
}


/*----------------------------------------------------------------------------*/
/* get reference to Stz area                                                  */
/*----------------------------------------------------------------------------*/
StrVal::operator Stz&(void)
{
  _precondition_(_MAGIC_);
  
  // return reference to char byte area
  return reinterpret_cast<Stz&>(*(pAdd<char*>(this,sizeof(StrVal))));
}



/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate number of bytes required to hold str of given length             */
/*----------------------------------------------------------------------------*/
i8 StrVal::nBytesRequired(i8 iStrLength)
{
  i8 nBytes = sizeof(StrVal) + iStrLength + 1;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* = operator                                                                 */
/*----------------------------------------------------------------------------*/
void StrVal::operator =(i8 iArg)
{
  StrLib::wToString(iArg,aChar());
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator =(StrVal& rArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(rArg);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator =(StrVal* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(aArg->aStz());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator =(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(aStr);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
//void StrVal::operator =(GenVar& rArg)
//{
//  _precondition_(_MAGIC_,_FOOTER_);
//  
//  vCopy((Stz*)rArg.aWspArea());
//  
//  _postcondition_(_MAGIC_,_FOOTER_);
//}

/*----------------------------------------------------------------------------*/
/* append given char                                                          */
/*----------------------------------------------------------------------------*/
void StrVal::vAppend(const char cChar)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  StrLib::vConcat(aChar(),cChar);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get size of data area                                                      */
/*----------------------------------------------------------------------------*/
i8 StrVal::nAreaSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set data area size (based on the string size)                              */
/*----------------------------------------------------------------------------*/
void StrVal::vAreaSize(void)
{
  //_precondition_(_MAGIC_,_FOOTER_);
  
  i8 nBytes = StrLib::iSize(aChar());
  
  m_xWsp.vUsed(nBytes);
}

/*----------------------------------------------------------------------------*/
/* assign given str                                                           */
/*----------------------------------------------------------------------------*/
void StrVal::vAssign(Stz* aStr)
{
  //ASSERT(m_nAreaSize);
  
  StrLib::vCopy(aChar(),aStr->aChar());
  
  vAreaSize();
}

/*----------------------------------------------------------------------------*/
/* assign str of given length                                                 */
/*----------------------------------------------------------------------------*/
void StrVal::vAssign(Stz* aStr, i8 iLength)
{
  //ASSERT(m_nAreaSize);
  
  StrLib::vCopyLen(aChar(),aStr->aChar(),iLength);
  
  aChar()[iLength] = '\0';
  
  m_xWsp.vUsed(iLength);
}

/*----------------------------------------------------------------------------*/
/* cat given variable sequence of Str's                                       */
/*----------------------------------------------------------------------------*/
void StrVal::vCat(Stz* aStr, ...)
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
void StrVal::vCatSprint(Stz* aFormat, ...)
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
void StrVal::vConcat(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vConcat(aChar(),aStr->aChar());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* bContains: test if arg is in substr                                        */
/*----------------------------------------------------------------------------*/
bool StrVal::bContains(Stz* aArg)
{
  return StrLib::bContains(aChar(),aArg->aChar());
}

/*----------------------------------------------------------------------------*/
/* copy given Stz, then concat variable sequence of Str's                     */
/*----------------------------------------------------------------------------*/
void StrVal::vCopyTill(Stz* aSrc, char cTill)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyTill(aChar(),aSrc->aChar(),cTill);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given Stz                                                             */
/*----------------------------------------------------------------------------*/
void StrVal::vCopy(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(aChar(),aStr->aChar(),m_xWsp.nSize()-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given Stz, then concat variable sequence of Str's                     */
/*----------------------------------------------------------------------------*/
void StrVal::vCopyCat(Stz* aStr, ...)
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
bool StrVal::bEnds(Stz* aSuffix)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEnds(aChar(),aSuffix->aChar());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool StrVal::bIsEqual(StrVal& sArg, i8 iCase)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(aChar(),sArg.aChar(),iCase);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument? (same case)                                    */
/*----------------------------------------------------------------------------*/
bool StrVal::bEqual(Stz* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(aChar(),(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of str in data area                                             */
/*----------------------------------------------------------------------------*/
i8 StrVal::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::iLength(aChar());
}

/*----------------------------------------------------------------------------*/
/* get char*                                                                  */
/*----------------------------------------------------------------------------*/
char* StrVal::aChar(void)
{
  char* aArea = m_xWsp.aArea();
  
  return aArea;
}

/*----------------------------------------------------------------------------*/
/* is same as given argument?  (ignores case)                                 */
/*----------------------------------------------------------------------------*/
bool StrVal::bSame(Stz* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bSame(aChar(),(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get size of this StrVal in bytes                                           */
/*----------------------------------------------------------------------------*/
i8 StrVal::nSize(void)
{
  i8 nBytes = sizeof(StrVal) + m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
void StrVal::vSprint(Stz* aFormat, ...)
{
  va_list args;
  va_start(args,aFormat);
  
  vsprintf(aChar(),aFormat->aChar(),args);
  
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get adddress of Stz area                                                   */
/*----------------------------------------------------------------------------*/
Stz* StrVal::aStz(void)
{
  char* aAddr = pAdd<char*>(this,sizeof(StrVal));
  
  return reinterpret_cast<Stz*>(aAddr);
}

/*----------------------------------------------------------------------------*/
/* convert to i8                                                              */
/*----------------------------------------------------------------------------*/
i8 StrVal::iToI8(void)
{
  i8 i = StrLib::iToI8(aChar());
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert to lower case                                                      */
/*----------------------------------------------------------------------------*/
void StrVal::vToLower(void)
{
  StrLib::vToLower(aChar());
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void StrVal::vToString(Stz* aStz)
{
  StrLib::vCopy(aStz->aChar(),aChar());
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool StrVal::bIsNumeric(void)
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
Footer* StrVal::aFooter(void)
{
  _precondition_(_MAGIC_);
  
  Footer* aFooter = pAdd<Footer*>(this,sizeof(StrVal) + m_xWsp.nUsed());
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
