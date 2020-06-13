/*============================================================================*/
/* Clause_GroupBy.hpp -- GroupBy Clause operand                               */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_GroupBy : public Clause
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_GroupBy(void);
   ~Clause_GroupBy(void) noexcept(false);
  
    static bool bDescend(Descend* pDescend, Clause_SubQry* pSubQry);
    static bool bParse(Descend* pDescend);

    Footer* aFooter(void);
  };

  Clause_GroupBy* _pClause_GroupBy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
