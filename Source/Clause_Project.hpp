/*============================================================================*/
/* Clause_Project.hpp -- Project Clause operand                               */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_Project : public Clause
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_Project(void);
   ~Clause_Project(void) noexcept(false);

    static bool bParse(Descend* pDescend, Stmt_SubQry* pSubQry);
    
    Footer* aFooter(void);
  };

  Clause_Project* _pClause_Project(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
