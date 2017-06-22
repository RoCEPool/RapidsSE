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
    void vGenPaths(Table* pTable, Index* pIndex);
    void vGenPaths(Table* pTable);
    void vGenPaths(void);
    void vMatchPaths(void);
    void vParseWhere(void);
    void vScorePaths(void);
    void vShowPaths(void);
    void vShowPredicates(void);
  
    Index* pChosenIndex(void);
    AccessPath* pChosenPath(void);
  
    PredicateVec* pPredicateVec(void);
  
    Footer* aFooter(void);
  };
  
  Planner* _pPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
