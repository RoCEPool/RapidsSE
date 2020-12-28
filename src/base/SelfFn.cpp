/*============================================================================*/
/* SelfFn.cpp -- System Self Function                                         */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SelfFn::SelfFn(Stz* szSelfNm, fpSELFFN* fpSelfFn, Stz* szDescription)
{
  _nonLocalStatic_ // memory is automatically zero'd
  
  m_szSelfNm = szSelfNm;
  m_fpSelfFn = fpSelfFn;
  m_szDescription = szDescription;
  
  m_nSeeAlso = 0;
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SelfFn::vAddSeeAlso(va_list xVarArgs)
{
  _nonProper_

  for (;;) {
    Stz* szSeeAlso = va_arg(xVarArgs,Stz*);

    if (!szSeeAlso)
      break;
    
    m_szaSeeAlso[m_nSeeAlso ++] = szSeeAlso;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
