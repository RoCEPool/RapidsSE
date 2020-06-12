/*============================================================================*/
/* Clause_Limit.hpp -- Limit Clause operand                                   */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_Limit : public Clause
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_Limit(TokenVec* pTknVec, i8& i_Tkn_);
   ~Clause_Limit(void) noexcept(false);
  
    static bool bParse(Descend* pDescend);

    Footer* aFooter(void);
  };

  Clause_Limit* _pClause_Limit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
