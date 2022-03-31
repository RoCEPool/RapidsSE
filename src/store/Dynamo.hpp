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
    friend class STORE::VertexSpec;

  private:
    TagSet*    m_p_JobTags;       // tag set from job
    
    Stz*       m_szPipelineStmt;
    VertexSpec*  m_p_PipeSpec;      // the pipeline spec
    
    bool       m_bWasOptimised;   // stmt was optimised
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(TagSet* p_JobTags);
   ~Dynamo(void) noexcept(false);

    static void vRepair(Object* pObject);
    static VertexSpec* pToPipeSpec(Stz* szPipelineStmt, TagSet* p_JobTags);

    
    VertexSpec* pCompile(Stz* szPipelineStmt);

    void vOptimise(void);

    VertexSpec* pPipeSpec(void);

    bool bWasOptimised(void);
    bool bWillOptimise(void);

  private:
    Footer* aFooter(void);
  };

  Dynamo* _pDynamo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
