/*============================================================================*/
/* Planner.hpp - Planner                                                      */
/*============================================================================*/
#pragma once

namespace Plans
{
  class Planner : public Object
  {
  private:
    static const i8 C_MAX_CANDIDATES = 128;
  
    Table*    m_pTable;         // table being accessed
    s48K      m_sWhereClause;   // the where clause
    TokenVec* m_pWhereTokenVec; // tokenised where
    PathVec*  m_pPathVec;       // vector of possible paths
    i8        m_i_Chosen;       // path chosen

    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Planner(void);
    ~Planner(void);
  
    void vChoosePath(void);
    AccessPath* pChosenPath(void);
    void vGenPaths(void);
    void vGenPaths(Table* pTable);
    void vGenExactPaths(Table* pTable, Index* pIndex);
    void vGenLowerPaths(Table* pTable, Index* pIndex);
    void vGenUpperPaths(Table* pTable, Index* pIndex);
    void vGenPaths(Table* pTable, Index* pIndex, eAT eAccess);
    bool bIsExactOp(eRO eRelOp);
    bool bIsLowerOp(eRO eRelOp);
    bool bIsMatchingOp(eAT eAccess, eRO eRelOp);
    bool bIsUpperOp(eRO eRelOp);
    void vMatchPaths(void);
    void vMatchPredicates(PredicateVec* mpAccPath, eAT eAccess);
    void vParseWhere(void);
    void vScorePaths(void);
    void vShowPaths(void);
    void   vTable(Table* pTable);
    Table* pTable(void);
    void  vWhere(s48K& sWhere);
    s48K& sWhere(void);
  
    Footer* aFooter(void);
  };
  
  Planner* _pPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
