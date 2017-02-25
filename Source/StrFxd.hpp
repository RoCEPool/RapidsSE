/*============================================================================*/
/* StrFxd.hpp -- fixed-length string template                                 */
/*============================================================================*/
#pragma once

#include "Values.h"

namespace Values
{
  template <int SIZE>
  class StrFxd : public StrVal
  {
  private:
    char m_mDataArea[SIZE]; // string data area
    
    // must be last
    Footer m_xFooter;
    
  public:    
    StrFxd(void);
    StrFxd(const char* aArg);
    StrFxd(StrFxd* pThis);
   ~StrFxd(void);
    
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (StrVal& xArg);
  
    Footer* pFooter(void);
  };
  
  void* _pStrFxd(void* pAddr);
}

using namespace Values;


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrFxd<SIZE>::StrFxd(void)
: StrVal(CN_STRFXD,SIZE)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
  
  Memory::vZero(m_mDataArea,SIZE);
  
  _checkInitialisedSize_(SIZE);
  
  _postcondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrFxd<SIZE>::StrFxd(const char* aArg)
: StrVal(CN_STRFXD,SIZE)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
  
  Memory::vZero(m_mDataArea,SIZE);
  
  StrVal::vCopy(aArg);
  
  _checkInitialisedSize_(SIZE);
  
  _postcondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrFxd<SIZE>::StrFxd(StrFxd* pThis)
: StrVal(CN_STRFXD,SIZE)
{
  _precondition_(_MAGIC_,CN_STRFXD);
  
  _postcondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrFxd<SIZE>::~StrFxd(void)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrFxd<SIZE>::operator = (const char* aArg)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
  
  StrVal::vCopy(aArg);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrFxd<SIZE>::operator = (char* aArg)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
  
  StrVal::vCopy(aArg);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrFxd<SIZE>::operator = (StrVal& rArg)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
  
  vCopy(rArg.a());
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <int SIZE>
Footer* StrFxd<SIZE>::pFooter(void)
{
  _precondition_(_MAGIC_,CN_STRFXD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
