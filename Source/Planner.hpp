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
    static const i8 C_MAX_PHRASES = 128;
  
    Table*    m_pTable;        // table being accessed
    PathVec*  m_pPathVec;      // vector of possible paths
    TokenVec* m_pWhereTknVec;  // tokenised where
  
    TokenVec* m_pPhraseTknVec[C_MAX_PHRASES]; // array of phrases
    i8        m_i_Chosen[C_MAX_PHRASES];      // path chosen
    i8        m_nPhrases;

    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Planner(void);
    ~Planner(void);
  
    void vChoosePath(i8 i_Phrase);
    AccessPath* pChosenPath(i8 i_Phrase);
    void vGenPaths(void);
    void vGenPaths(Index* pIndex, eAT eAccess);
    bool bIsExactOp(eRO eRelOp);
    bool bIsLowerOp(eRO eRelOp);
    bool bIsMatchingOp(eAT eAccess, eRO eRelOp);
    bool bIsUpperOp(eRO eRelOp);
    void vMatchPaths(void);
    void vMatchPredicates(PredicateVec* mpAccPath, eAT eAccess);
    void vParseWhere(s48K& sWhereClause);
    i8   nPhrases(void);
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
