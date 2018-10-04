/*============================================================================*/
/* AccessPath.hpp - Access Path                                               */
/*============================================================================*/
#pragma once

namespace Interface
{
  class Session;
}

namespace Plans
{
  enum eAT // access technique
  {
  /* 0 */ AT_NONE=0,
  /* 1 */ AT_EXACT,
  /* 2 */ AT_LOWER,
  /* 3 */ AT_UPPER,
  /* 4 */ AT_RANGE,
  /* 5 */ AT_SCAN,
  /* 6 */ AT_ENUMS
  };
  
  static Stz* aAT[]
  {
  /* 0 */ "none"_Stz,
  /* 1 */ "exact"_Stz,
  /* 2 */ "lower"_Stz,
  /* 3 */ "upper"_Stz,
  /* 4 */ "range"_Stz,
  /* 5 */ "scan"_Stz,
  /* 6 */ "enums"_Stz
  };
  
  class AccessPath : public Object
  {
  
  private:
    Table*     m_pTable;      // table
    TablePtn*  m_pPartition;  // partition

    bool       m_bIsCandidate; // is this path a candidate
    i8         m_iPathScore;   // its score

    Index*     m_pIndex;       // index
    eAT        m_eAccess;      // access technique
  
    PredicateVec*  m_pLowerPredicates; // predicates of lower/exact bound
    PredicateVec*  m_pUpperPredicates; // predicates of upper bound
  
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    AccessPath(eAT eAccess);
    ~AccessPath(void);
  
    Table* pTable(void);
    void vTable(Table* pTable);
    
    TablePtn* pPartition(void);
    void vPartition(TablePtn* pPartition);
    
    bool bIsCandidate(void);
    void vIsCandidate(bool bIsCandidate);
  
    i8 iPathScore(void);
    void vPathScore(i8 iPathRating);
  
    Index* pIndex(void);
    void vIndex(Index* pIndex);
    
    eAT eAccess(void);
    void vAccess(eAT eAccess);
  
    PredicateVec* pLowerPredicates(void);
    void vLowerPredicates(PredicateVec* pLowerPredicates);

    PredicateVec* pUpperPredicates(void);
    void vUpperPredicates(PredicateVec* pUpperPredicates);
  
    void vShow(void);
  
    Footer* aFooter(void);
  };
  
  AccessPath* _pAccessPath(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
