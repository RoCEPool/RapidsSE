/*============================================================================*/
/* Clause.hpp -- Clause                                                       */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_Select;
  class Clause_From;
  class Clause_Where;
  class Clause_GroupBy;
  class Clause_OrderBy;
  class Clause_Limit;
  class Descend;

  class Clause : public Operand
  {
  protected:
    s32 m_sName;
    TokenVec* m_pTokenVec;

  public:
    void operator delete(void*);
  
    Clause(eCN eClass);
   ~Clause(void) noexcept(false);

    static void vCollect(Descend* pDescend);

    TokenVec* pTokenVec(void);
  
  };

  Clause* _pClause(void* aAddr);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static Stz* szaClauseWords[]
{
  "from"_Stz,
  "group"_Stz,
  "join"_Stz,
  "limit"_Stz,
  "order"_Stz,
  "select"_Stz,
  "where"_Stz,

  nullptr
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
