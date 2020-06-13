/*============================================================================*/
/* Clause_From.hpp -- From Clause operand                                     */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_From : public Clause
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_From(void);
   ~Clause_From(void) noexcept(false);

    static bool bDescend(Descend* pDescend, Clause_SubQry* pSubQry);
    static bool bParse(Descend* pDescend);
    
    Footer* aFooter(void);
  };

  Clause_From* _pClause_From(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
