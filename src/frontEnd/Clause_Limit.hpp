/*============================================================================*/
/* Clause_Limit.hpp -- Limit Clause operand                                   */
/*============================================================================*/
#pragma once

namespace FrontEnd
{
  class Clause_Limit : public Clause
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_Limit(void);
   ~Clause_Limit(void) noexcept(false);
  
    static bool bParse(Descend* pDescend, Stmt_SubQry* pSubQry);

    Footer* aFooter(void);
  };

  Clause_Limit* _pClause_Limit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
