/*============================================================================*/
/* SubPipeSpecs.hpp -- List of Sub-Pipeline Specifications                    */
/*============================================================================*/
#pragma once

#include "PipeSpec.hpp"

namespace STORE
{
  class SubPipeSpecs : public ListNL_t<PipeSpec>
  {
  private:
   
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    SubPipeSpecs(void);
   ~SubPipeSpecs(void) noexcept(false);
  
  private:
  
    Footer* aFooter(void);

  };

SubPipeSpecs* _pSubPipeSpecs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
