/*============================================================================*/
/* Stmt_SubQry.hpp -- Statement Sub-Query                                     */
/*============================================================================*/
#pragma once

namespace CompilerStream
{
  class Stmt_SubQry : public Stmt
  {
  private:

    i8   m_iSubSeqDgt; // last digit(s) of the subQryNbr ..0 ..1 ..2
    s32  m_sSubQryNbr; // the entire sub query number
  
    i8   m_iAtDepth;   // what depth from root
    i8   m_iNxtKidDgt; // next digit(s) of a kid sub qry ..0 ..1 ..2

    Stmt_SubQry*    m_pDadSubQry;
 
    Clause_Project* m_pProject;
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
  
    Stmt_SubQry(void);
   ~Stmt_SubQry(void) noexcept(false);
  
    static bool bParse(Descend* pDescend, Stmt_SubQry* pSubQry);

    Stmt_SubQry*  pDadSubQry(void);
    void          vDadSubQry(Stmt_SubQry*);

    i8 iAtDepth(void);
    void vAtDepth(i8 iDepth);

    i8 iNextKidDgt(void);

    Stz* szSubQryNbr(void);
  
    i8 iSubSeqDgt(void);
    void vSubSeqDgt(i8 iSubSeqDgt);

  
    Clause_Project* pProject(void);
    void            vProject(Clause_Project*);

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

  Stmt_SubQry* _pStmt_SubQry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
