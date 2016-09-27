/*============================================================================*/
/* s.cpp -- an s-string                                                       */
/*============================================================================*/

#include "Magic.h"
#include "String.hpp"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;

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
: Header(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_iSize = iSize;
  *aWorkspace() = '\0';
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
  
  String::vCopyLen(aWorkspace(),aArg,m_iSize-1);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void s::operator = (char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);

  String::vCopyLen(aWorkspace(),aArg,m_iSize-1);

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
  
  String::vConcat(aWorkspace(),c);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append argument                                                            */
/*----------------------------------------------------------------------------*/
void s::vAppend(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vConcat(aWorkspace(),aArg);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get address of buffer                                                      */
/*----------------------------------------------------------------------------*/
char* s::aWorkspace(void)
{
  //_precondition_(_MAGIC_,_FOOTER_);
  
  char* p = pcAdd(this,sizeof(s));

  return p;
}

/*----------------------------------------------------------------------------*/
/* copy argument                                                              */
/*----------------------------------------------------------------------------*/
void s::vCopy(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(aWorkspace(),aArg,m_iSize-1);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test if string ends with given suffix                                      */
/*----------------------------------------------------------------------------*/
bool s::bEnds(const char* aSuffix)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEnds(aWorkspace(),aSuffix);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void s::vInit(i8 iSize)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_iSize = iSize;
  *aWorkspace() = '\0';  

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is euqal to given argument?                                                */
/*----------------------------------------------------------------------------*/
bool s::bIsEqual(s& sArg, i8 iCase) 
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEqualTo(aWorkspace(),sArg.aWorkspace(),iCase);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of buffer                                                       */
/*----------------------------------------------------------------------------*/
i8 s::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return String::iLength(aWorkspace());
}

/*----------------------------------------------------------------------------*/
/* get size of buffer                                                         */
/*----------------------------------------------------------------------------*/
i8 s::iSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_iSize;
}

/*----------------------------------------------------------------------------*/
/* set size of buffer                                                         */
/*----------------------------------------------------------------------------*/
void s::vSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_iSize = String::iSize(aWorkspace());
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
char* s::a(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return aWorkspace();
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
char* s::z(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return String::zTerminator(aWorkspace());
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* s::pFooter(void)
{
  _precondition_(_MAGIC_);
  
  Footer* pFooter = static_cast<Footer*>(pvAdd(this,sizeof(s)+m_iSize));
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/