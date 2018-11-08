/*============================================================================*/
/* Planner.hpp - Planner                                                      */
/*============================================================================*/
#pragma once

namespace Plans
{
  class Planner : public Object
  {
  private:
    static const i8 C_MAX_PHRASES = 128;
  
    Table*    m_pTable; // table being accessed

    TokenVec* m_pWhereTknVec; // tokenised where clause
    TokenVec* m_pPhraseTknVec[C_MAX_PHRASES]; // array of phrases
    i8        m_nPhrases; // number of phrases
  
    PathVec*  m_pPathVec[C_MAX_PHRASES]; // array of possible path vectors
    i8        m_i_Chosen[C_MAX_PHRASES]; // path chosen

    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Planner(void);
    ~Planner(void) noexcept(false);
  
    void vChoosePath(i8 i_Phrase);
    AccessPath* pChosenPath(i8 i_Phrase);
    void vGenPaths(void);
    void vGenPaths(i8 i_Phrase, Index* pIndex, eAT eAccess);
    eRO  eGetFirstLowerPredicateRO(i8 iPhrase);
    eRO  eGetFirstUpperPredicateRO(i8 iPhrase);
    
    bool bIsExactOp(eRO eRelOp);
    bool bIsLowerOp(eRO eRelOp);
    bool bIsMatchingOp(eAT eAccess, eRO eRelOp);
    bool bIsRelationalOp(Token* pToken);
    bool bIsUpperOp(eRO eRelOp);
    void vMatchPaths(void);
    void vMatchPredicates(i8 i_Phrase, PredicateVec* mpAccPath, eAT eAccess);
    void vParsePhrases(void);
    void vParseWhere(s48K& sWhereClause);
    i8   nPhrases(void);
    TokenVec* pPhraseTknVec(i8 i_Phrase);
    void vScorePaths(void);
    void vShowPaths(void);
    void vShowPhrases(void);
    void   vTable(Table* pTable);
    Table* pTable(void);
  
    Footer* aFooter(void);
  };
  
  Planner* _pPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
