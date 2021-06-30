/*============================================================================*/
/* Dynamo.hpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/
#pragma once

#include "RootNode.hpp"
#include "GraphNav.hpp"
#include "QueryNode.hpp"

using namespace Parser;

namespace STORE
{
  class Dynamo : public Object
  {
  
    friend class STORE::PipelineSpec;

  private:
    Job*          m_p_PipelineJob;  // job this dynamo is associated with
    
    TokenVec*     m_pTknVec;        // tokenised for of the pipeline statement
    i8            m_i_Tkn;          // current token
    Token*        m_pTkn;           // current token

    PipelineSpec* m_p_PipelineSpec_root; // the top pipeline spec
  
    StageSpec*    m_p_StageSpec;    // current stage spec being formed
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(Stz* szPipelineStmt, Job* p_PipelineJob);
   ~Dynamo(void) noexcept(false);

    static bool bToPipelineStmt(String* pAsPipelineStmt, TokenVec* pFromTokenVec);

    bool bCompile(void);
    PipelineSpec* pPipelineSpec(void);
    void vShow(void);

  private:

    void vTokenAccept(void);
    void vTokenBegin(i8 i_Tkn);

    Footer* aFooter(void);
  };

  Dynamo* _pDynamo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
