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
  
    Clause_GroupBy(TokenVec* pTknVec, i8& i_Tkn_);
   ~Clause_GroupBy(void) noexcept(false);
  
    static bool bParse(Descend* pDescend);

    Footer* aFooter(void);
  };

  Clause_GroupBy* _pClause_GroupBy(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
