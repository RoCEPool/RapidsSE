/*============================================================================*/
/* StrTpl.cpp -- fixed-length string template                                 */
/*============================================================================*/

#include "Values.h"
#include "Memory.hpp"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrTpl<SIZE>::StrTpl(void)
: StrVal(CN_S32,SIZE)
{
  _precondition_(_MAGIC_,CN_S32,_FOOTER_);
  
  Memory::vZero(m_mDataArea,SIZE);
  
  _checkInitialisedSize_(SIZE);
  
  _postcondition_(_MAGIC_,CN_S32,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrTpl<SIZE>::StrTpl(StrVal& rArg)
: StrVal(CN_S32,SIZE)
{
  _precondition_(_MAGIC_,CN_S32,_FOOTER_);
  
  Memory::vZero(m_mDataArea,SIZE);
  StrVal::vCopy(rArg.a());
  
  _checkInitialisedSize_(SIZE);
  
  _postcondition_(_MAGIC_,CN_S32,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrTpl<SIZE>::StrTpl(const char* aArg)
: StrVal(CN_S32,SIZE)
{
  _precondition_(_MAGIC_,CN_S32,_FOOTER_);

  Memory::vZero(m_mDataArea,SIZE);
  StrVal::vCopy(aArg);
  
  _checkInitialisedSize_(SIZE);
  
  _postcondition_(_MAGIC_,CN_S32,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrTpl<SIZE>::~StrTpl(void)
{
  _precondition_(_MAGIC_,CN_S32,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrTpl<SIZE>::operator = (const char* aArg)
{
  _precondition_(_MAGIC_,CN_S32,_FOOTER_);
  
  StrVal::vCopy(aArg);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrTpl<SIZE>::operator = (char* aArg)
{
  _precondition_(_MAGIC_,CN_S32,_FOOTER_);
  
  StrVal::vCopy(aArg);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrTpl<SIZE>::operator = (StrVal& rArg)
{
  _precondition_(_MAGIC_,CN_S32,_FOOTER_);
  
  vCopy(rArg.a());
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrTpl<SIZE>::vInit()
{
  _precondition_(_MAGIC_,CN_S32,_FOOTER_);
  
  StrVal::vInit(SIZE);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <int SIZE>
Footer* StrTpl<SIZE>::pFooter(void)
{
  _precondition_(_MAGIC_,CN_S32);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
