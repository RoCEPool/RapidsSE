/*============================================================================*/
/* SelfFn.hpp --  System-Level Self-Describing Function                       */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SelfFn
  {
  public:
    Stz* m_szSelfNm;
    fpSELFFN* m_fpSelfFn;  //
  
    Stz* m_szDescription;

    i8 m_nSeeAlso;
    Stz* m_szaeeAlso[16];


  public:
    SelfFn(Stz* szSelfNm, fpSELFFN* fpSelfFn, Stz* szDescription, i8 iArg);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
