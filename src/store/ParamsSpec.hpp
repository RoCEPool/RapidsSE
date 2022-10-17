/*============================================================================*/
/* ParamsSpec.hpp -- Parameters (list) Specification                          */
/*============================================================================*/
#pragma once

#include "ParamSpec.hpp"

namespace STORE
{
  class ParamsSpec : public ListNL_t<ParamSpec>
                   , public ListItem_t<ParamsSpec>
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ParamsSpec(void);
   ~ParamsSpec(void) noexcept(false);
   
    Footer* aFooter(void);
  };

  ParamsSpec* _pParamsSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
