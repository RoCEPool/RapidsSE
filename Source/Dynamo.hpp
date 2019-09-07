/*============================================================================*/
/* Dynamo.hpp -- Dynamo (dynamic plan compiler)                               */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class Dynamo : public Object
  {
  private:
  
    TokenVec*    m_p_TknVec;
  
    DynamicSpec* m_pDynamicSpec;
    StageSpec*   m_pStageSpec;
    OpSpec*      m_pOpSpec;
  
    i8           m_i_Tkn;
    Token*       m_pTkn;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(TokenVec* pTknVec);
   ~Dynamo(void) noexcept(false);
  
    bool bCompile(void);
  
    bool bCompileName(void);
    bool bCompileBegin(void);
    bool bCompileStage(void);
    bool bCompileTopology(void);
    bool bCompilePipe(void);
    bool bCompileOp();
    bool bCompileEnd(void);

    void vAcceptToken(void);
    void vBeginToken(i8 i_Tkn);
    DynamicSpec* pDynamicSpec(void);
    bool bIsOperator(Token* pTkn);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
