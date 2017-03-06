/*============================================================================*/
/* StrVbl.cpp -- variable-length string value                                 */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StrVbl* Values::_pStrVbl(void* aAddr)
{
  StrVbl* psRefactor = reinterpret_cast<StrVbl*>(aAddr);
  
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
void* StrVbl::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: StrVbl class cannot be new'd\n");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StrVbl::operator new(size_t iSize, StrVbl* pThis)
{
  RASSERT(iSize);
  
  return pThis;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVbl::StrVbl(eCN eClassNo)
: Value(eClassNo)
{
  _precondition_(_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVbl::StrVbl(eCN eClassNo, i8 nBytes)
: Value(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_xWsp.vUsed(nBytes);
  
  m_xWsp.vZero();
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVbl::StrVbl(StrVbl* pThis)
: Value(CN_STRVAL)
{
  _precondition_(_MAGIC_,CN_STRVAL);
  
  ASSERT(this == pThis);
  
  _checkInitialised_(CN_STRVAL);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVbl::StrVbl(StrVbl* pThis, i8 nBytes)
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
StrVbl::~StrVbl(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get address of Str area                                                    */
/*----------------------------------------------------------------------------*/
StrVbl::operator Str*(void)
{
  _precondition_(_MAGIC_);
  
  // return address to char byte area
  return reinterpret_cast<Str*>(pAdd<char*>(this,sizeof(StrVbl)));
}


/*----------------------------------------------------------------------------*/
/* get reference to Str area                                                  */
/*----------------------------------------------------------------------------*/
StrVbl::operator Str&(void)
{
  _precondition_(_MAGIC_);
  
  // return reference to char byte area
  return reinterpret_cast<Str&>(*(pAdd<char*>(this,sizeof(StrVbl))));
}



/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate number of bytes required to hold string of given length          */
/*----------------------------------------------------------------------------*/
i8 StrVbl::nAreaRequired(i8 iLength)
{
  i8 nBytes = sizeof(StrVbl) + iLength + 1;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVbl::operator =(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(aArea(),aArg,m_xWsp.nUsed()-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVbl::operator =(char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(aArea(),aArg,m_xWsp.nUsed()-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void StrVbl::operator =(StrVbl& rArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(rArg.aStr());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given char                                                          */
/*----------------------------------------------------------------------------*/
void StrVbl::vAppend(const char cChar)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  String::vConcat(aArea(),cChar);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given str                                                           */
/*----------------------------------------------------------------------------*/
void StrVbl::vAppend(const char* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vConcat(aArea(),aStr);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* StrVbl::aArea(void)
{
  //_precondition_(_MAGIC_,_FOOTER_);
  
  char* aAddr = m_xWsp.aArea();
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* get size of data area                                                      */
/*----------------------------------------------------------------------------*/
i8 StrVbl::nAreaSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set data area size (based on the string size)                              */
/*----------------------------------------------------------------------------*/
void StrVbl::vAreaSize(void)
{
  //_precondition_(_MAGIC_,_FOOTER_);
  
  i8 nBytes = String::iSize(aArea());
  
  m_xWsp.vUsed(nBytes);
}

/*----------------------------------------------------------------------------*/
/* assign given str                                                           */
/*----------------------------------------------------------------------------*/
void StrVbl::vAssign(const char* aStr)
{
  //ASSERT(m_nAreaSize);
  
  String::vCopy(aArea(),aStr);
  
  vAreaSize();
}

/*----------------------------------------------------------------------------*/
/* assign str of given length                                                 */
/*----------------------------------------------------------------------------*/
void StrVbl::vAssign(const char* aStr, i8 iLength)
{
  //ASSERT(m_nAreaSize);
  
  String::vCopyLen(aArea(),aStr,iLength);
  
  aArea()[iLength] = '\0';
  
  m_xWsp.vUsed(iLength);
}

/*----------------------------------------------------------------------------*/
/* copy given str                                                             */
/*----------------------------------------------------------------------------*/
void StrVbl::vCopy(const char* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(aArea(),aStr,m_xWsp.nUsed()-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy str of given length                                                   */
/*----------------------------------------------------------------------------*/
void StrVbl::vCopy(const char* aStr, i8 iLength)
{
  vClass(CN_STRVAL);
  
  String::vCopyLen(aArea(),aStr,iLength);
  
  vAreaSize();
}

/*----------------------------------------------------------------------------*/
/* test if string ends with given suffix                                      */
/*----------------------------------------------------------------------------*/
bool StrVbl::bEnds(const char* aSuffix)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEnds(aArea(),aSuffix);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool StrVbl::bIsEqual(StrVbl& sArg, i8 iCase)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEqualTo(aArea(),sArg.aArea(),iCase);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of str in data area                                             */
/*----------------------------------------------------------------------------*/
i8 StrVbl::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return String::iLength(aArea());
}

/*----------------------------------------------------------------------------*/
/* get size of this StrVbl in bytes                                           */
/*----------------------------------------------------------------------------*/
i8 StrVbl::nSize(void)
{
  i8 nBytes = sizeof(StrVbl) + m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get aStr                                                                   */
/*----------------------------------------------------------------------------*/
char* StrVbl::aStr(void)
{
  return aArea();
}

/*----------------------------------------------------------------------------*/
/* convert to command                                                         */
/*----------------------------------------------------------------------------*/
void StrVbl::vToCommand(char* aStr)
{
  String::vCopy(aStr,"\"");
  String::vCopy(aStr,aArea());
  String::vCopy(aStr,"\"");
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void StrVbl::vToString(char* aStr)
{
  String::vCopy(aStr,aArea());
}

/*----------------------------------------------------------------------------*/
/* get adddress of Str area                                                   */
/*----------------------------------------------------------------------------*/
Str* StrVbl::aStrArea(void)
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
Footer* StrVbl::pFooter(void)
{
  _precondition_(_MAGIC_);
  
  Footer* pFooter = pAdd<Footer*>(this,sizeof(StrVbl) + m_xWsp.nUsed());
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
