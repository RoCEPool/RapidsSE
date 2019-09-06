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

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(TokenVec* pTknVec);
   ~Dynamo(void) noexcept(false);
  
    bool bCompile(void);
  
    bool bCompileName(i8& i_Tkn);
    bool bCompileStage(i8& i_Tkn);
    bool bCompileTopology(i8& i_Tkn);
    bool bCompilePipe(i8& i_Tkn);
    bool bCompileOp(i8& i_Tkn);

    bool bIsOperator(Token* pTkn);

    Token* pGetToken(i8 i_Tkn);
    void   vAcceptToken(i8& i_Tkn);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
