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

  public:
    SelfFn(Stz* szSelfNm, fpSELFFN* fpSelfFn, i8);
  
    void vDoSelfFn(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
