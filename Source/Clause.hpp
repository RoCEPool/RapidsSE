/*============================================================================*/
/* Clause.hpp -- Clause                                                       */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause : public Operand
  {
  protected:
    s32 m_sName;
    TokenVec* m_pTokenVec;

  public:
    void operator delete(void*);
  
    Clause(eCN eClass, TokenVec* pTknVec, i8& i_Tkn_);
   ~Clause(void) noexcept(false);
  
    static bool bDescend(Descend* pDescend);
  
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
