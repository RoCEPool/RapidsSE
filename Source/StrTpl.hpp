/*============================================================================*/
/* StrTpl.hpp -- fixed-length string template                                 */
/*============================================================================*/
#pragma once

#include "Values.h"

namespace Values
{
  template <int SIZE>
  class StrTpl : public StrVal
  {
  private:
    char m_mDataArea[SIZE]; // string data area
    
    // must be last
    Footer m_xFooter;
    
  public:    
    StrTpl(void);
    StrTpl(StrVal& xArg);
    StrTpl(const char* aArg);
    StrTpl(const char* aArg, i8 nChars);
   ~StrTpl(void);
    
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (StrVal& xArg);
    
    void vInit(void);
    
    Footer* pFooter(void);
  };
}

using namespace Values;


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
