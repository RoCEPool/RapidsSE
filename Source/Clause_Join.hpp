/*============================================================================*/
/* Clause_Join.hpp -- Join Clause operand                                     */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_Join : public Clause
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_Join(TokenVec* pTknVec, i8& i_Tkn_);
   ~Clause_Join(void) noexcept(false);
  
    static bool bDescend(Descend* pDescend, Clause_SubQry* pSubQry);
    static bool bParse(Descend* pDescend);

    Footer* aFooter(void);
  };

  Clause_Join* _pClause_Join(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
