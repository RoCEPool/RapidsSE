/*============================================================================*/
/* Clause_OrderBy.hpp -- OrderBy Clause operand                               */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_OrderBy : public Clause
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_OrderBy(void);
   ~Clause_OrderBy(void) noexcept(false);
  
    static bool bParse(Descend* pDescend, Clause_SubQry* pSubQry);

    Footer* aFooter(void);
  };

  Clause_OrderBy* _pClause_OrderBy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
