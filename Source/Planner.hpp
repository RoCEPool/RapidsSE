/*============================================================================*/
/* Planner.hpp - Planner                                                      */
/*============================================================================*/
#pragma once

namespace Plans
{  
  class Planner : public Object
  {
  private:
    Table*          m_pTable;
    CandidatePaths* m_pCandidatePaths;
    Index*          m_pIndex[C_INDEXES_PER_TABLE];
    i8              m_iScoreVec[C_INDEXES_PER_TABLE];
  
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
  
 
    void vShowPaths(void);
  
    void vTest(void);

  
    Footer* pFooter(void);
  };
  
  Planner* _pPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
