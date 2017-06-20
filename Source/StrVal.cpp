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
StrVal* Values::_pStrVal(void* aAddr)
{
  StrVal* psRefactor = reinterpret_cast<StrVal*>(aAddr);
  
  RASSERT( ! psRefactor
          || CN_STRVAL == psRefactor->eClass()
          || CN_STRVAR == psRefactor->eClass()
          );  
  
  return psRefactor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StrVal::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: StrVal class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StrVal::operator new(size_t iSize, StrVal* pThis)
{
  RASSERT(iSize);
  
  return pThis;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(eCN eClassNo)
: Value(eClassNo)
{
  _precondition_(_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(eCN eClassNo, i8 nBytes)
: Value(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_xWsp.vUsed(nBytes);
  
  m_xWsp.vZero();
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(StrVal* pThis)
: Value(CN_STRVAL)
{
  _precondition_(_MAGIC_,CN_STRVAL);
  
  ASSERT(this == pThis);
  
  _checkInitialised_(CN_STRVAL);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(StrVal* pThis, i8 nBytes)
: Value(CN_STRVAL)
{
  _precondition_(_MAGIC_,CN_STRVAL);
  
  ASSERT(this == pThis);
  
  m_xWsp.vUsed(nBytes);
  
  m_xWsp.vZero();
  
  _checkInitialised_(CN_STRVAL);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StrVal::~StrVal(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get address of Str area                                                    */
/*----------------------------------------------------------------------------*/
StrVal::operator Str*(void)
{
  _precondition_(_MAGIC_);
  
  // return address to char byte area
  return reinterpret_cast<Str*>(pAdd<char*>(this,sizeof(StrVal)));
}


/*----------------------------------------------------------------------------*/
/* get reference to Str area                                                  */
/*----------------------------------------------------------------------------*/
StrVal::operator Str&(void)
{
  _precondition_(_MAGIC_);
  
  // return reference to char byte area
  return reinterpret_cast<Str&>(*(pAdd<char*>(this,sizeof(StrVal))));
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
  StrLib::wToString(iArg,s());
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
void StrVal::operator =(StrVal* pArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(pArg->aStr());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator =(Str* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(aStr);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator =(ValWrk& rArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy((Str*)rArg.aWspArea());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given char                                                          */
/*----------------------------------------------------------------------------*/
void StrVal::vAppend(const char cChar)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  StrLib::vConcat(s(),cChar);
  
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
  
  i8 nBytes = StrLib::iSize(s());
  
  m_xWsp.vUsed(nBytes);
}

/*----------------------------------------------------------------------------*/
/* assign given str                                                           */
/*----------------------------------------------------------------------------*/
void StrVal::vAssign(Str* aStr)
{
  //ASSERT(m_nAreaSize);
  
  StrLib::vCopy(s(),aStr->s());
  
  vAreaSize();
}

/*----------------------------------------------------------------------------*/
/* assign str of given length                                                 */
/*----------------------------------------------------------------------------*/
void StrVal::vAssign(Str* aStr, i8 iLength)
{
  //ASSERT(m_nAreaSize);
  
  StrLib::vCopyLen(s(),aStr->s(),iLength);
  
  s()[iLength] = '\0';
  
  m_xWsp.vUsed(iLength);
}

/*----------------------------------------------------------------------------*/
/* cat given variable sequence of Str's                                       */
/*----------------------------------------------------------------------------*/
void StrVal::vCat(Str* aStr, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,aStr);

  StrLib::vConcat(s(),aStr->s());
  
  while (Str* aArg = va_arg(xVarArgs,Str*))
    StrLib::vConcat(s(),aArg->s());
  
  va_end(xVarArgs);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* vCatSprint: formatted concatenated sprint                                  */
/*----------------------------------------------------------------------------*/
void StrVal::vCatSprint(Str* aFormat, ...)
{
  va_list args;
  
  char* aEnd = pAdd<char*>(s(),iLength());
  
  va_start(args,aFormat);
  vsprintf(aEnd,(char*)aFormat,args);
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* concat given Str                                                           */
/*----------------------------------------------------------------------------*/
void StrVal::vConcat(Str* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vConcat(s(),aStr->s());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* bContains: test if arg is in substr                                        */
/*----------------------------------------------------------------------------*/
bool StrVal::bContains(Str* aArg)
{
  return StrLib::bContains(s(),aArg->s());
}

/*----------------------------------------------------------------------------*/
/* copy given Str, then concat variable sequence of Str's                     */
/*----------------------------------------------------------------------------*/
void StrVal::vCopyTill(Str* aSrc, char cTill)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyTill(s(),aSrc->s(),cTill);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given Str                                                             */
/*----------------------------------------------------------------------------*/
void StrVal::vCopy(Str* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(s(),aStr->s(),m_xWsp.nSize()-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given Str, then concat variable sequence of Str's                     */
/*----------------------------------------------------------------------------*/
void StrVal::vCopyCat(Str* aStr, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,aStr);
  
  StrLib::vCopyLen(s(),aStr->s(),m_xWsp.nSize()-1);

  while (Str* aArg = va_arg(xVarArgs,Str*))
    StrLib::vConcat(s(),aArg->s());
  
  va_end(xVarArgs);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test if string ends with given suffix                                      */
/*----------------------------------------------------------------------------*/
bool StrVal::bEnds(Str* aSuffix)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEnds(s(),aSuffix->s());
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool StrVal::bIsEqual(StrVal& sArg, i8 iCase)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(s(),sArg.s(),iCase);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument? (same case)                                    */
/*----------------------------------------------------------------------------*/
bool StrVal::bEqual(Str* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(s(),(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of str in data area                                             */
/*----------------------------------------------------------------------------*/
i8 StrVal::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::iLength(s());
}

/*----------------------------------------------------------------------------*/
/* get char*                                                                  */
/*----------------------------------------------------------------------------*/
char* StrVal::s(void)
{
  char* aArea = m_xWsp.aArea();
  
  return aArea;
}

/*----------------------------------------------------------------------------*/
/* is same as given argument?  (ignores case)                                 */
/*----------------------------------------------------------------------------*/
bool StrVal::bSame(Str* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bSame(s(),(const char*)aArg);
  
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
void StrVal::vSprint(Str* aFormat, ...)
{
  va_list args;
  va_start(args,aFormat);
  
  vsprintf(s(),aFormat->s(),args);
  
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* get adddress of Str area                                                   */
/*----------------------------------------------------------------------------*/
Str* StrVal::aStr(void)
{
  char* aAddr = pAdd<char*>(this,sizeof(StrVal));
  
  return reinterpret_cast<Str*>(aAddr);
}

/*----------------------------------------------------------------------------*/
/* convert to i8                                                              */
/*----------------------------------------------------------------------------*/
i8 StrVal::iToI8(void)
{
  i8 i = StrLib::iToI8(s());
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* convert to lower case                                                      */
/*----------------------------------------------------------------------------*/
void StrVal::vToLower(void)
{
  StrLib::vToLower(s());
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void StrVal::vToString(Str* aStr)
{
  StrLib::vCopy(aStr->s(),s());
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
