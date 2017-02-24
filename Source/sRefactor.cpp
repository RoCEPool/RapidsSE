/*============================================================================*/
/* sRefactor.cpp -- temporary class for refactoring s                         */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StrVal* Values::_pStrVal(void* pAddr)
{
  StrVal* pStrVal = reinterpret_cast<StrVal*>(pAddr);
  
  RASSERT( !pStrVal || CN_STRVAL == pStrVal->eClass() );
  
  return pStrVal;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
sRefactor* Values::_psRefactor(void* pAddr)
{
  sRefactor* psRefactor = reinterpret_cast<sRefactor*>(pAddr);
  
  RASSERT( !psRefactor
          || CN_STRVAL == psRefactor->eClass()
          || CN_S32    == psRefactor->eClass()
          || CN_S256   == psRefactor->eClass()
          || CN_S48K   == psRefactor->eClass()
          || CN_S1M    == psRefactor->eClass()
          );
  
  return psRefactor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* sRefactor::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: sRefactor class cannot be new'd\n");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* sRefactor::operator new(size_t iSize, sRefactor* pStrVal)
{
  RASSERT(iSize);
  
  return pStrVal;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
sRefactor::sRefactor(void)
: Value(CN_STRVAL)
{
  m_nDataBytes = 0;
  
  _checkInitialised_(CN_STRVAL);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
sRefactor::sRefactor(eCN eClass)
: Value(eClass)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
sRefactor::sRefactor(eCN eClassNo, i8 iSize)
: Value(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_nDataBytes = iSize;
  *pDataArea() = '\0';
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
sRefactor::~sRefactor(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void sRefactor::operator = (const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(pDataArea(),aArg,m_nDataBytes-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void sRefactor::operator = (char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(pDataArea(),aArg,m_nDataBytes-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void sRefactor::operator = (sRefactor& rArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(rArg.a());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}




/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* sRefactor::pDataArea(void)
{
  //_precondition_(_MAGIC_,_FOOTER_);
  
  char* p = pAdd<char*>(this,sizeof(sRefactor));
  
  return p;
}

/*----------------------------------------------------------------------------*/
/* get size of data area                                                      */
/*----------------------------------------------------------------------------*/
i8 sRefactor::nDataBytes(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* set limit of data size (based on the string size)                          */
/*----------------------------------------------------------------------------*/
void sRefactor::vDataBytes(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_nDataBytes = String::iSize(pDataArea());
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
char* sRefactor::a(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return pDataArea();
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
char* sRefactor::z(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return String::zTerminator(pDataArea());
}




/*----------------------------------------------------------------------------*/
/* append given character                                                     */
/*----------------------------------------------------------------------------*/
void sRefactor::vAppend(const char c)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  String::vConcat(pDataArea(),c);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append argument                                                            */
/*----------------------------------------------------------------------------*/
void sRefactor::vAppend(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vConcat(pDataArea(),aArg);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy argument                                                              */
/*----------------------------------------------------------------------------*/
void sRefactor::vCopy(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(pDataArea(),aArg,m_nDataBytes-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test if string ends with given suffix                                      */
/*----------------------------------------------------------------------------*/
bool sRefactor::bEnds(const char* aSuffix)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEnds(pDataArea(),aSuffix);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void sRefactor::vInit(i8 iSize)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_nDataBytes = iSize;
  *pDataArea() = '\0';
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is euqal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool sRefactor::bIsEqual(sRefactor& sArg, i8 iCase)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEqualTo(pDataArea(),sArg.pDataArea(),iCase);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of string in data area                                          */
/*----------------------------------------------------------------------------*/
i8 sRefactor::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return String::iLength(pDataArea());
}


/*----------------------------------------------------------------------------*/
/* get iChars                                                                 */
/*----------------------------------------------------------------------------*/
i8 sRefactor::iChars(void) const
{
  return m_nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* reset string value                                                         */
/*----------------------------------------------------------------------------*/
void sRefactor::vReset(void)
{
  m_nDataBytes = 0;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes                                                          */
/*----------------------------------------------------------------------------*/
i8 sRefactor::iSize(void) const
{
  //i8 iOverrun = m_nDataBytes > 8 ? m_nDataBytes - 8 : 0;
  
  i8 iSize = sizeof(StrVal) + m_nDataBytes;
  
  return iSize;
}

/*----------------------------------------------------------------------------*/
/* set size in bytes                                                          */
/*----------------------------------------------------------------------------*/
void sRefactor::vSize(void)
{
  m_nDataBytes = String::iSize(pDataArea());
}

/*----------------------------------------------------------------------------*/
/* set size in bytes                                                          */
/*----------------------------------------------------------------------------*/
void sRefactor::vSize(i8 iSize)
{
  m_nDataBytes = iSize;
}

/*----------------------------------------------------------------------------*/
/* get sStr                                                                   */
/*----------------------------------------------------------------------------*/
char* sRefactor::sStr(void)
{
  return pDataArea();
}

/*----------------------------------------------------------------------------*/
/* convert to command                                                         */
/*----------------------------------------------------------------------------*/
void sRefactor::vToCommand(char* aWorkspace)
{
  String::vCopy(aWorkspace,"\"");
  String::vCopy(aWorkspace,pDataArea());
  String::vCopy(aWorkspace,"\"");
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void sRefactor::vToString(char* aWorkspace)
{
  String::vCopy(aWorkspace,pDataArea());
}

/*----------------------------------------------------------------------------*/
/* copy a string into workspace                                               */
/*----------------------------------------------------------------------------*/
void sRefactor::vCopyString(char* aStr, i8 iLength)
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
i8 sRefactor::nBytesRequired(i8 iLength)
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
Footer* sRefactor::pFooter(void)
{
  _precondition_(_MAGIC_);
  
  Footer* pFooter = pAdd<Footer*>(this,sizeof(sRefactor)+m_nDataBytes);
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
