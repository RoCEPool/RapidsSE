/*============================================================================*/
/* SelfFn.hpp --  System-Level Self-Describing Function                       */
/*============================================================================*/
#pragma once

namespace BASE { class SelfFn; }

typedef void (fpSELFFN)(SelfFn* aSelfFn, TokenVec* pTokenVec);

namespace BASE
{
  class SelfFn
  {
  public:
    Stz* m_szSelfNm;
    fpSELFFN* m_fpSelfFn;  //
  
    Stz* m_szDescription;

    i8 m_nSeeAlso;
    Stz* m_szaSeeAlso[16];


  public:
    SelfFn(Stz* szSelfNm, fpSELFFN* fpSelfFn, Stz* szDescription);
  
    void vAddSeeAlso(va_list xVarArgs);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
