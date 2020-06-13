/*============================================================================*/
/* Clause_Where.hpp -- Where Clause operand                                   */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_Where : public Clause
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_Where(void);
   ~Clause_Where(void) noexcept(false);
  
    static bool bParse(Descend* pDescend, Clause_SubQry* pSubQry);

    Footer* aFooter(void);
  };

  Clause_Where* _pClause_Where(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
