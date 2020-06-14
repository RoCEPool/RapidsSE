/*============================================================================*/
/* Stmt_Select.hpp -- Select Statement operand                                */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Stmt_Select : public Stmt
  {
  private:
  
    TokenVec* m_pTokenVec;
  
    Stmt_SubQry* m_pRootSubQry;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Stmt_Select(void);
   ~Stmt_Select(void) noexcept(false);
  
    bool bParse(TokenVec* pTokenVec);
  
    TokenVec* pTokenVec(void);
  
    Stmt_SubQry*  pRootSubQry(void);
    void            vRootSubQry(Stmt_SubQry*);


    Footer* aFooter(void);
  };

  Stmt_Select* _pStmt_Select(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
