/*============================================================================*/
/* Clause_SubQry.hpp -- Sub-Query Clause operand                              */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_SubQry : public Clause
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_SubQry(TokenVec* pTknVec, i8& i_Tkn_);
   ~Clause_SubQry(void) noexcept(false);
  
    static bool bParse(Descend* pDescend);

    Footer* aFooter(void);
  };

  Clause_SubQry* _pClause_SubQry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
