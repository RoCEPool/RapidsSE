/*============================================================================*/
/* Optimus.hpp -- Optimus (pipeline plan optimizer)                           */
/*============================================================================*/
#pragma once

#include "RootNode.hpp"
#include "GraphNav.hpp"
#include "QueryNode.hpp"

namespace STORE
{
  class Optimus : public Object
  {  
    friend class STORE::PipeSpec;

  private:
    Job*          m_p_PipelineJob;       // job this dynamo is associated with
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Optimus(Job* p_PipelineJob);
   ~Optimus(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Optimus* _pOptimus(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
