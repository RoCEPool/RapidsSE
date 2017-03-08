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
          || CN_VALWSP == psRefactor->eClass()
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
/* calculate number of bytes required to hold string of given length          */
/*----------------------------------------------------------------------------*/
i8 StrVal::nAreaRequired(i8 iLength)
{
  i8 nBytes = sizeof(StrVal) + iLength + 1;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator =(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(aArea(),aArg,m_xWsp.nUsed()-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator =(char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(aArea(),aArg,m_xWsp.nUsed()-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator =(StrVal& rArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(rArg.aStr());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given char                                                          */
/*----------------------------------------------------------------------------*/
void StrVal::vAppend(const char cChar)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  StrLib::vConcat(aArea(),cChar);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given str                                                           */
/*----------------------------------------------------------------------------*/
void StrVal::vAppend(const char* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vConcat(aArea(),aStr);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* StrVal::aArea(void)
{
  //_precondition_(_MAGIC_,_FOOTER_);
  
  char* aAddr = m_xWsp.sArea();
  
  return aAddr;
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
  
  i8 nBytes = StrLib::iSize(aArea());
  
  m_xWsp.vUsed(nBytes);
}

/*----------------------------------------------------------------------------*/
/* assign given str                                                           */
/*----------------------------------------------------------------------------*/
void StrVal::vAssign(Str* aStr)
{
  //ASSERT(m_nAreaSize);
  
  StrLib::vCopy(aArea(),aStr->a());
  
  vAreaSize();
}

/*----------------------------------------------------------------------------*/
/* assign str of given length                                                 */
/*----------------------------------------------------------------------------*/
void StrVal::vAssign(Str* aStr, i8 iLength)
{
  //ASSERT(m_nAreaSize);
  
  StrLib::vCopyLen(aArea(),aStr->a(),iLength);
  
  aArea()[iLength] = '\0';
  
  m_xWsp.vUsed(iLength);
}

/*----------------------------------------------------------------------------*/
/* copy given str                                                             */
/*----------------------------------------------------------------------------*/
void StrVal::vCopy(const char* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(aArea(),aStr,m_xWsp.nUsed()-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy str of given length                                                   */
/*----------------------------------------------------------------------------*/
void StrVal::vCopy(const char* aStr, i8 iLength)
{
  vClass(CN_STRVAL);
  
  StrLib::vCopyLen(aArea(),aStr,iLength);
  
  vAreaSize();
}

/*----------------------------------------------------------------------------*/
/* copy given Str                                                             */
/*----------------------------------------------------------------------------*/
void StrVal::vCopy(Str* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(aArea(),aStr->a(),m_xWsp.nSize()-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy cat given Strs                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::vCopyCat(Str* aStr, ...)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,aStr);
  
  StrLib::vCopyLen(aArea(),aStr->a(),m_xWsp.nSize()-1);

  while (Str* aArg = va_arg(xVarArgs,Str*))
    StrLib::vConcat(aArea(),aArg->a());
  
  va_end(xVarArgs);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test if string ends with given suffix                                      */
/*----------------------------------------------------------------------------*/
bool StrVal::bEnds(const char* aSuffix)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEnds(aArea(),aSuffix);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool StrVal::bIsEqual(StrVal& sArg, i8 iCase)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(aArea(),sArg.aArea(),iCase);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument? (same case)                                    */
/*----------------------------------------------------------------------------*/
bool StrVal::bEqual(Str* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bEqualTo(aArea(),(const char*)aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of str in data area                                             */
/*----------------------------------------------------------------------------*/
i8 StrVal::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return StrLib::iLength(aArea());
}

/*----------------------------------------------------------------------------*/
/* is same as given argument?  (ignores case)                                 */
/*----------------------------------------------------------------------------*/
bool StrVal::bSame(Str* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = StrLib::bSame(aArea(),(const char*)aArg);
  
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
/* get aStr                                                                   */
/*----------------------------------------------------------------------------*/
char* StrVal::aStr(void)
{
  return aArea();
}

/*----------------------------------------------------------------------------*/
/* convert to command                                                         */
/*----------------------------------------------------------------------------*/
void StrVal::vToCommand(char* aStr)
{
  StrLib::vCopy(aStr,"\"");
  StrLib::vCopy(aStr,aArea());
  StrLib::vCopy(aStr,"\"");
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void StrVal::vToString(char* aStr)
{
  StrLib::vCopy(aStr,aArea());
}

/*----------------------------------------------------------------------------*/
/* get adddress of Str area                                                   */
/*----------------------------------------------------------------------------*/
Str* StrVal::aStrArea(void)
{
  char* aAddr = pAdd<char*>(this,sizeof(StrVal));
  
  return reinterpret_cast<Str*>(aAddr);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* StrVal::pFooter(void)
{
  _precondition_(_MAGIC_);
  
  Footer* pFooter = pAdd<Footer*>(this,sizeof(StrVal) + m_xWsp.nUsed());
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
