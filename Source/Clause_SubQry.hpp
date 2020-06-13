/*============================================================================*/
/* Clause_SubQry.hpp -- Sub-Query Clause operand                              */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_SubQry : public Clause
  {
  private:
  
    Clause_Select*  m_pSelect;
    Clause_From*    m_pFrom;
    Clause_Where*   m_pWhere;
    Clause_GroupBy* m_pGroupBy;
    Clause_OrderBy* m_pOrderBy;
    Clause_Limit*   m_pLimit;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Clause_SubQry(TokenVec* pTknVec, i8& i_Tkn_);
   ~Clause_SubQry(void) noexcept(false);
  
    static bool bDescend(Descend* pDescend, Clause_SubQry* pSubQry);
    static bool bParse(Descend* pDescend);

    Clause_Select*  pSelect(void);
    void            vSelect(Clause_Select*);

    Clause_From*    pFrom(void);;
    void            vFrom(Clause_From*);

    Clause_Where*   pWhere(void);;
    void            vWhere(Clause_Where*);

    Clause_GroupBy* pGroupBy(void);;
    void            vGroupBy(Clause_GroupBy*);

    Clause_OrderBy* pOrderBy(void);;
    void            vOrderBy(Clause_OrderBy*);

    Clause_Limit*   pLimit(void);;
    void            vOrderBy(Clause_Limit*);

    Footer* aFooter(void);
  };

  Clause_SubQry* _pClause_SubQry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
