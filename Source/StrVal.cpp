/*============================================================================*/
/* StrVal.cpp -- string value                                                 */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StrVal* Values::_pStrVal(void* pAddr)
{
  StrVal* psRefactor = reinterpret_cast<StrVal*>(pAddr);
  
  RASSERT( ! psRefactor
          || CN_VALWSP == psRefactor->eClass()
          || CN_STRVAL == psRefactor->eClass()
          || CN_STRFXD == psRefactor->eClass()
          );
  
  return psRefactor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StrVal::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: StrVal class cannot be new'd\n");
  
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
StrVal::StrVal(StrVal* pThis)
: Value(CN_STRVAL)
{
  _precondition_(_MAGIC_,CN_STRVAL);
  
  ASSERT(this == pThis);
  
  m_nDataBytes = 0;
  
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
  
  m_nDataBytes = nBytes;
  
  Memory::vZero(aDataArea(),m_nDataBytes);
  
  _checkInitialised_(CN_STRVAL);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(eCN eClassNo, i8 nBytes)
: Value(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_nDataBytes = nBytes;
  
  Memory::vZero(aDataArea(),m_nDataBytes);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StrVal::~StrVal(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate number of bytes required to hold string of given length          */
/*----------------------------------------------------------------------------*/
i8 StrVal::nSizeRequired(i8 iLength)
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
  
  String::vCopyLen(aDataArea(),aArg,m_nDataBytes-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator =(char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(aDataArea(),aArg,m_nDataBytes-1);
  
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
  
  String::vConcat(aDataArea(),cChar);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given str                                                           */
/*----------------------------------------------------------------------------*/
void StrVal::vAppend(const char* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vConcat(aDataArea(),aStr);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assign given str                                                           */
/*----------------------------------------------------------------------------*/
void StrVal::vAssign(const char* aStr)
{
  //ASSERT(m_nDataBytes);
  
  String::vCopy(aDataArea(),aStr);
  m_nDataBytes = String::iSize(aDataArea());
}

/*----------------------------------------------------------------------------*/
/* assign str of given lengt                                                  */
/*----------------------------------------------------------------------------*/
void StrVal::vAssign(const char* aStr, i8 iLength)
{
  //ASSERT(m_nDataBytes);
  
  String::vCopyLen(aDataArea(),aStr,iLength);
  
  aDataArea()[iLength] = '\0';
  
  m_nDataBytes = iLength;
}

/*----------------------------------------------------------------------------*/
/* copy given str                                                             */
/*----------------------------------------------------------------------------*/
void StrVal::vCopy(const char* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(aDataArea(),aStr,m_nDataBytes-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy str of given length                                                   */
/*----------------------------------------------------------------------------*/
void StrVal::vCopy(const char* aStr, i8 iLength)
{
  vClass(CN_STRVAL);
  
  String::vCopyLen(aDataArea(),aStr,iLength);
  
  m_nDataBytes = String::iSize(aDataArea());
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* StrVal::aDataArea(void)
{
  //_precondition_(_MAGIC_,_FOOTER_);
  
  char* p = pAdd<char*>(this,sizeof(StrVal));
  
  return p;
}

/*----------------------------------------------------------------------------*/
/* get size of data area                                                      */
/*----------------------------------------------------------------------------*/
i8 StrVal::nDataBytes(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* set limit of data size (based on the string size)                          */
/*----------------------------------------------------------------------------*/
void StrVal::vDataBytes(void)
{
  //_precondition_(_MAGIC_,_FOOTER_);
  
  m_nDataBytes = String::iSize(aDataArea());
}

/*----------------------------------------------------------------------------*/
/* test if string ends with given suffix                                      */
/*----------------------------------------------------------------------------*/
bool StrVal::bEnds(const char* aSuffix)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEnds(aDataArea(),aSuffix);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool StrVal::bIsEqual(StrVal& sArg, i8 iCase)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEqualTo(aDataArea(),sArg.aDataArea(),iCase);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of str in data area                                             */
/*----------------------------------------------------------------------------*/
i8 StrVal::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return String::iLength(aDataArea());
}

/*----------------------------------------------------------------------------*/
/* get size of this StrVal in bytes                                           */
/*----------------------------------------------------------------------------*/
i8 StrVal::nSize(void) const
{
  i8 nBytes = sizeof(StrVal) + m_nDataBytes;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get aStr                                                                   */
/*----------------------------------------------------------------------------*/
char* StrVal::aStr(void)
{
  return aDataArea();
}

/*----------------------------------------------------------------------------*/
/* convert to command                                                         */
/*----------------------------------------------------------------------------*/
void StrVal::vToCommand(char* aStr)
{
  String::vCopy(aStr,"\"");
  String::vCopy(aStr,aDataArea());
  String::vCopy(aStr,"\"");
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void StrVal::vToString(char* aStr)
{
  String::vCopy(aStr,aDataArea());
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
  
  Footer* pFooter = pAdd<Footer*>(this,sizeof(StrVal)+m_nDataBytes);
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
