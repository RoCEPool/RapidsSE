/*============================================================================*/
/* SubSysFn.hpp -- description of a subsystem                                 */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SubSysFn : public SelfFn
  {
  public:

  public:
    SubSysFn(Stz* szSubSysNm, fpSELFFN* fpSubSysFn, i8 iArg);
  
    void vDoSubSysFn(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
