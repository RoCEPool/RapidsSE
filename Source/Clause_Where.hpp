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
  
    Clause_Where(TokenVec* pTknVec, i8& i_Tkn_);
   ~Clause_Where(void) noexcept(false);
  
    static bool bDescend(Descend* pDescend);
    static bool bParse(Descend* pDescend);

    Footer* aFooter(void);
  };

  Clause_Where* _pClause_Where(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
