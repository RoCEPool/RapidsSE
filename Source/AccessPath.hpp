/*============================================================================*/
/* AccessPath.hpp - Access Path                                               */
/*============================================================================*/
#pragma once

namespace Interface
{
  class SEitf;
}

namespace Plans
{
  class AccessPath : public Object
  {
  private:
    Table*     m_pTable;      // table
    Partition* m_pPartition;  // partition

    bool       m_bIsCandidate; // is this path a candidate
    i8         m_iPathRating;  // its score

    Index*     m_pIndex;       // index
    eFT        m_eAccess;      // access technique
  
    PredicateVec*  m_pLowerPredicates; // predicates of lower/exact bound
    PredicateVec*  m_pUpperPredicates; // predicates of upper bound
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    AccessPath(void);
    ~AccessPath(void);
  
    Table* pTable(void);
    void vTable(Table* pTable);
    
    Partition* pPartition(void);
    void vPartition(Partition* pPartition);
    
    bool bIsCandidate(void);
    void vIsCandidate(bool bIsCandidate);
  
    i8 iPathRating(void);
    void vPathRating(i8 iPathRating);
  
    Index* pIndex(void);
    void vIndex(Index* pIndex);
    
    eFT eAccess(void);
    void vAccess(eFT eAccess);
  
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
