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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StrVal::StrVal(void)
: Value(CN_STRVAL)
{
  _precondition_(_MAGIC_);
  
  vFatal("fatal: StrVal class cannot be a local variable\n"_Stz);
}

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
/* copy given Stz                                                             */
/*----------------------------------------------------------------------------*/
void StrVal::vCopy(Stz* aStr)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopyLen(aChar(),aStr->aChar(),m_xWsp.nSize()-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
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
