/*============================================================================*/
/* s.cpp -- an s-string                                                       */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/* operator new: class cannot be new'd                                        */
/*----------------------------------------------------------------------------*/
void* s::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: s class cannot be new'd");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
s::s(eCN eClassNo, i8 iSize)
: sRefactor(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_nDataBytes = iSize;
  *pDataArea() = '\0';
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
s::~s(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void s::operator = (const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(pDataArea(),aArg,m_nDataBytes-1);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void s::operator = (char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);

  String::vCopyLen(pDataArea(),aArg,m_nDataBytes-1);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void s::operator = (s& rArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(rArg.a());

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given character                                                     */
/*----------------------------------------------------------------------------*/
void s::vAppend(const char c)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  String::vConcat(pDataArea(),c);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append argument                                                            */
/*----------------------------------------------------------------------------*/
void s::vAppend(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vConcat(pDataArea(),aArg);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy argument                                                              */
/*----------------------------------------------------------------------------*/
void s::vCopy(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(pDataArea(),aArg,m_nDataBytes-1);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get size of data area                                                      */
/*----------------------------------------------------------------------------*/
i8 s::nDataBytes(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* set limit of data size (based on the string size)                          */
/*----------------------------------------------------------------------------*/
void s::vDataBytes(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_nDataBytes = String::iSize(pDataArea());
}

/*----------------------------------------------------------------------------*/
/* test if string ends with given suffix                                      */
/*----------------------------------------------------------------------------*/
bool s::bEnds(const char* aSuffix)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEnds(pDataArea(),aSuffix);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void s::vInit(i8 iSize)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_nDataBytes = iSize;
  *pDataArea() = '\0';  

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is euqal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool s::bIsEqual(s& sArg, i8 iCase) 
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEqualTo(pDataArea(),sArg.pDataArea(),iCase);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of string in data area                                          */
/*----------------------------------------------------------------------------*/
i8 s::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return String::iLength(pDataArea());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
