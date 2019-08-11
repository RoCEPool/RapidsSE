/*============================================================================*/
/* Stmt_Select.hpp -- Select Statement operand                                */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Stmt_Select : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Stmt_Select(void);
   ~Stmt_Select(void) noexcept(false);

    Footer* aFooter(void);
  };

  Stmt_Select* _pStmt_Select(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
