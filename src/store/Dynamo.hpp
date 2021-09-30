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
    Stz*          m_szPipelineStmt;
    TagSet*       m_p_JobTags;       // tag set from job
    PipelineSpec* m_p_PipelineSpec;  // the pipeline spec
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(Stz* szPipelineStmt, TagSet* p_JobTags);
   ~Dynamo(void) noexcept(false);

    PipelineSpec* pCompile(void);

    PipelineSpec* pPipelineSpec(void);

  private:
    Footer* aFooter(void);
  };

  Dynamo* _pDynamo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
