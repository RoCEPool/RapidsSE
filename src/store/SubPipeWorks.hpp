/*============================================================================*/
/* SubPipeWorks.hpp -- List of Sub-Pipeline Specifications                    */
/*============================================================================*/
#pragma once

#include "PipeWork.hpp"

namespace STORE
{
  class SubPipeWorks : public ListNL_t<PipeWork>
  {
  private:
   
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    SubPipeWorks(void);
   ~SubPipeWorks(void) noexcept(false);
  
  private:
  
    Footer* aFooter(void);

  };

SubPipeWorks* _pSubPipeWorks(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
