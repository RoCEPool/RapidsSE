/*============================================================================*/
/* StrVal.cpp -- string value                                                 */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StrVal* Values::_pStrVal(void* pAddr)
{
  StrVal* psRefactor = reinterpret_cast<StrVal*>(pAddr);
  
  RASSERT( !psRefactor
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
StrVal::StrVal(eCN eClass)
: Value(eClass)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(StrVal* pThis)
: Value(CN_STRVAL)
{
  ASSERT(this == pThis);
  
  m_nDataBytes = 0;
  
  _checkInitialised_(CN_STRVAL);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(StrVal* pThis, i8 iChars)
: Value(CN_STRVAL)
{
  ASSERT(this == pThis);
  
  m_nDataBytes = iChars;
  
  Memory::vZero(pDataArea(),m_nDataBytes);
  
  _checkInitialised_(CN_STRVAL);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(eCN eClassNo, i8 iSize)
: Value(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_nDataBytes = iSize;
  *pDataArea() = '\0';
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StrVal::~StrVal(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator = (const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(pDataArea(),aArg,m_nDataBytes-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator = (char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(pDataArea(),aArg,m_nDataBytes-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVal::operator = (StrVal& rArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(rArg.a());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void StrVal::vStrValAssign(const char* sStr)
{
  //ASSERT(m_nDataBytes);
  
  String::vCopy(pDataArea(),sStr);
  m_nDataBytes = String::iSize(pDataArea());
}



/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* StrVal::pDataArea(void)
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
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_nDataBytes = String::iSize(pDataArea());
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
char* StrVal::a(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return pDataArea();
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
char* StrVal::z(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return String::zTerminator(pDataArea());
}




/*----------------------------------------------------------------------------*/
/* append given character                                                     */
/*----------------------------------------------------------------------------*/
void StrVal::vAppend(const char c)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  String::vConcat(pDataArea(),c);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append argument                                                            */
/*----------------------------------------------------------------------------*/
void StrVal::vAppend(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vConcat(pDataArea(),aArg);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy argument                                                              */
/*----------------------------------------------------------------------------*/
void StrVal::vCopy(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(pDataArea(),aArg,m_nDataBytes-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test if string ends with given suffix                                      */
/*----------------------------------------------------------------------------*/
bool StrVal::bEnds(const char* aSuffix)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEnds(pDataArea(),aSuffix);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void StrVal::vInit(i8 iSize)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_nDataBytes = iSize;
  *pDataArea() = '\0';
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is euqal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool StrVal::bIsEqual(StrVal& sArg, i8 iCase)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEqualTo(pDataArea(),sArg.pDataArea(),iCase);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of string in data area                                          */
/*----------------------------------------------------------------------------*/
i8 StrVal::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return String::iLength(pDataArea());
}


/*----------------------------------------------------------------------------*/
/* get iChars                                                                 */
/*----------------------------------------------------------------------------*/
i8 StrVal::iChars(void) const
{
  return m_nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* reset string value                                                         */
/*----------------------------------------------------------------------------*/
void StrVal::vReset(void)
{
  m_nDataBytes = 0;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes                                                          */
/*----------------------------------------------------------------------------*/
i8 StrVal::iSize(void) const
{
  //i8 iOverrun = m_nDataBytes > 8 ? m_nDataBytes - 8 : 0;
  
  i8 iSize = sizeof(StrVal) + m_nDataBytes;
  
  return iSize;
}

/*----------------------------------------------------------------------------*/
/* set size in bytes                                                          */
/*----------------------------------------------------------------------------*/
void StrVal::vSize(void)
{
  m_nDataBytes = String::iSize(pDataArea());
}

/*----------------------------------------------------------------------------*/
/* set size in bytes                                                          */
/*----------------------------------------------------------------------------*/
void StrVal::vSize(i8 iSize)
{
  m_nDataBytes = iSize;
}

/*----------------------------------------------------------------------------*/
/* get sStr                                                                   */
/*----------------------------------------------------------------------------*/
char* StrVal::sStr(void)
{
  return pDataArea();
}

/*----------------------------------------------------------------------------*/
/* convert to command                                                         */
/*----------------------------------------------------------------------------*/
void StrVal::vToCommand(char* aWorkspace)
{
  String::vCopy(aWorkspace,"\"");
  String::vCopy(aWorkspace,pDataArea());
  String::vCopy(aWorkspace,"\"");
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void StrVal::vToString(char* aWorkspace)
{
  String::vCopy(aWorkspace,pDataArea());
}

/*----------------------------------------------------------------------------*/
/* copy a string into workspace                                               */
/*----------------------------------------------------------------------------*/
void StrVal::vCopyString(char* aStr, i8 iLength)
{
  vClass(CN_STRVAL);
  
  String::vCopyLen(pDataArea(),aStr,iLength);
  
  m_nDataBytes = String::iSize(pDataArea());
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate number of bytes reqired to hold string of given length           */
/*----------------------------------------------------------------------------*/
i8 StrVal::nBytesRequired(i8 iLength)
{
  i8 nBytes = sizeof(StrVal) + iLength + 1;
  
  return nBytes;
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
