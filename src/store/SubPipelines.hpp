/*============================================================================*/
/* SubPipelines.hpp -- List of Sub-Pipeline Specifications                    */
/*============================================================================*/
#pragma once

#include "PipeSpec.hpp"

namespace STORE
{
  class SubPipelines : public ListNL_t<PipeSpec>
  {
  private:
   
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    SubPipelines(void);
   ~SubPipelines(void) noexcept(false);
  
  private:
  
    Footer* aFooter(void);

  };

SubPipelines* _pSubPipelines(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
