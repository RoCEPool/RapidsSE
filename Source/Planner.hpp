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
  
    Table*    m_pTable;       // table being accessed
    TokenVec* m_pWhereTknVec; // tokenised where
    PathVec*  m_pPathVec;     // vector of possible paths
    i8        m_i_Chosen;     // path chosen

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
    void vGenPaths(Index* pIndex, eAT eAccess);
    bool bIsExactOp(eRO eRelOp);
    bool bIsLowerOp(eRO eRelOp);
    bool bIsMatchingOp(eAT eAccess, eRO eRelOp);
    bool bIsUpperOp(eRO eRelOp);
    void vMatchPaths(void);
    void vMatchPredicates(PredicateVec* mpAccPath, eAT eAccess);
    void vParseWhere(s48K& sWhereClause);
    void vScorePaths(void);
    void vShowPaths(void);
    void   vTable(Table* pTable);
    Table* pTable(void);
  
    Footer* aFooter(void);
  };
  
  Planner* _pPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
