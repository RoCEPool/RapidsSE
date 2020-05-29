/*============================================================================*/
/* Planner.hpp - Planner                                                      */
/*============================================================================*/
#pragma once

namespace Plans
{
  enum eBK // bound kind
  {
  /* 0 */ BK_EXACT,
  /* 1 */ BK_LOWER,
  /* 1 */ BK_UPPER,
  };
  
  static Str* aBK[]
  {
  /* 0 */ "exact"_Str,
  /* 1 */ "lower"_Str,
  /* 2 */ "upper"_Str,
  };
  
  class Planner : public Object
  {
  private:
    static const i8 C_MAX_CANDIDATES = 128;
  
    Table*          m_pTable;
  
    PathVec*        m_pPathVec;
  
    Index*          m_pIndex[C_MAX_CANDIDATES];
    i8              m_iScoreVec[C_MAX_CANDIDATES];
    bool            m_bIsCandidate[C_MAX_CANDIDATES];
    i8              m_nCandidates;
    s48K            m_sWhere;
    i8              m_i_Chosen;

    PredicateVec*   m_pPredicateVec;
  
    TokenVec*       m_pWhereTokenVec;
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Planner(void);
    ~Planner(void);
  
    void   vTable(Table* pTable);
    Table* pTable(void);
    
    void  vWhere(s48K& sWhere);
    s48K& sWhere(void);
    
    void vChoosePath(void);
    void vGenPaths(void);
    void vGenPaths(Table* pTable);
    void vGenExactPaths(Table* pTable, Index* pIndex);
    void vGenLowerPaths(Table* pTable, Index* pIndex);
    void vGenUpperPaths(Table* pTable, Index* pIndex);
    void vGenRangePaths(Table* pTable, Index* pIndex);
    void vMatchPaths(void);
    void vMatchExactPaths(void);
    void vMatchLowerPaths(void);
    void vMatchUpperPaths(void);
    void vMatchRangePaths(void);
    void vMatchGivenPaths(eBK eBound);
    void vParseWhere(void);
    void vScorePaths(void);
    void vShowPaths(void);
    void vShowPredicates(void);
  
    Index* pChosenIndex(void);
    AccessPath* pChosenPath(void);
  
    PredicateVec* pPredicateVec(void);
  
    bool bIsExactOp(eRO eRelOp);
    bool bIsLowerOp(eRO eRelOp);
    bool bIsUpperOp(eRO eRelOp);
  
    Footer* aFooter(void);
  };
  
  Planner* _pPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
