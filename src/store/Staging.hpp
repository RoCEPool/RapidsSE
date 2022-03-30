/*============================================================================*/
/* StageSet.hpp -- List of Stages                                             */
/*============================================================================*/
#pragma once

#include "Stage.hpp"

namespace STORE { class PipeSpec; }
namespace STORE
{
  class Staging : public ListNL_t<Stage>
  {
  private:
   
    PipeWork* m_pPipeWork;
    PipeSpec* m_pPipeSpec; // specification of this pipework

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Staging(PipeWork* pPipeWork, PipeSpec* pPipeSpec);
   ~Staging(void) noexcept(false);
  
    PipeWork* pPipeWork(void);

  private:
  
    Footer* aFooter(void);

  };

Staging* _pStaging(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
