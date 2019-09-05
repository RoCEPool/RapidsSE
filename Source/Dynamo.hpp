/*============================================================================*/
/* Dynamo.hpp -- Dynamo (dynamic plan compiler)                               */
/*============================================================================*/
#pragma once

namespace Dynamics
{
  class Dynamo : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(void);
   ~Dynamo(void) noexcept(false);
  
    bool bCompile(TokenVec* pTokenVec);
  
    bool bCompileName(TokenVec* pTokenVec, i8& i_Tkn);
    bool bCompileStage(TokenVec* pTokenVec, i8& i_Tkn);
    bool bCompileTopology(TokenVec* pTokenVec, i8& i_Tkn);
    bool bCompilePipe(TokenVec* pTokenVec, i8& i_Tkn);
  
    bool bIsOperator(Token* pTkn);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
