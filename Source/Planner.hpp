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
    CandidatePaths* m_pCandidatePaths;
    Index*          m_pIndex[C_MAX_CANDIDATES];
    i8              m_iScoreVec[C_MAX_CANDIDATES];
    bool            m_bIsCandidate[C_MAX_CANDIDATES];
    i8              m_nCandidates;
    s48K            m_sWhere;
  
    PredicateVec*   m_pPredicateVec;
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Planner(void);
    ~Planner(void);
  
    void vGenPaths(Table* pTable, Index* pIndex);
    void vGenPaths(Table* pTable);
    void vGenPaths(void);

    void vScorePaths(void);
  
    void   vTable(Table* pTable);
    Table* pTable(void);
  
    void  vWhere(s48K& sWhere);
    s48K& sWhere(void);
 
    void vMatchPaths(void);
    void vParseWhere(void);
    void vShowPaths(void);
    void vShowPredicates(void);
  
    void vTest(void);

  
    Footer* pFooter(void);
  };
  
  Planner* _pPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
