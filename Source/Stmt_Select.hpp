/*============================================================================*/
/* Stmt_Select.hpp -- Select Statement operand                                */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Stmt_Select : public Operand
  {
  private:
  
    TokenVec* m_pTokenVec;
  
    Clause_SubQry* m_pSubQry;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Stmt_Select(void);
   ~Stmt_Select(void) noexcept(false);
  
    bool bParse(TokenVec* pTokenVec);
  
    TokenVec* pTokenVec(void);
  
    Clause_SubQry*  pSubQry(void);
    void            vSubQry(Clause_SubQry*);


    Footer* aFooter(void);
  };

  Stmt_Select* _pStmt_Select(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
