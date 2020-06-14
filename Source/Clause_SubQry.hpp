/*============================================================================*/
/* Clause_SubQry.hpp -- Sub-Query Clause operand                              */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Clause_SubQry : public Clause
  {
  private:

    i8   m_iAtDepth;
    s32  m_sSubQryNbr;
  
    Clause_SubQry*  m_pParentSubQry;
 
  
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
  
    Clause_SubQry(void);
   ~Clause_SubQry(void) noexcept(false);
  
    static bool bParse(Descend* pDescend, Clause_SubQry* pSubQry);

    Clause_SubQry*  pParentSubQry(void);
    void            vParentSubQry(Clause_SubQry*);

    i8 iAtDepth(void);
    void vAtDepth(i8 iDepth);
  
    Stz* szSubQryNbr(void);
    
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
