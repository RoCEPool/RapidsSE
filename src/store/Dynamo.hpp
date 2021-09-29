/*============================================================================*/
/* Dynamo.hpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/
#pragma once

#include "RootNode.hpp"
#include "GraphNav.hpp"
#include "QueryNode.hpp"

namespace STORE
{
  class Dynamo : public Object
  {  
    friend class STORE::PipelineSpec;

  private:
    Job*          m_p_PipelineJob;   // job this dynamo is associated with
    PipelineSpec* m_p_PipelineSpec;  // the pipeline spec
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(Job* p_PipelineJob);
   ~Dynamo(void) noexcept(false);

    PipelineSpec* pCompile(Stz* szPipelineStmt);

    PipelineSpec* pPipelineSpec(void);

  private:
    Footer* aFooter(void);
  };

  Dynamo* _pDynamo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
