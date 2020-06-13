/*============================================================================*/
/* Clause_Select.hpp -- Select Clause operand                                 */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_Select : public Clause
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_Select(void);
   ~Clause_Select(void) noexcept(false);

    static bool bParse(Descend* pDescend, Clause_SubQry* pSubQry);
  
    Footer* aFooter(void);
  };

  Clause_Select* _pClause_Select(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
