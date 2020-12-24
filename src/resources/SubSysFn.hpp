/*============================================================================*/
/* SubSysFn.hpp -- description of a subsystem                                 */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SubSysFn : public SelfFn
  {
  public:
    Stz* m_szSubSysNm;
  
    fpSUBSYSFN* m_fpSubSysFn;  //

  public:
    SubSysFn(Stz* szSubSysNm, fpSUBSYSFN* fpSubSysFn, i8);
  
    void vDoSubSysFn(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
