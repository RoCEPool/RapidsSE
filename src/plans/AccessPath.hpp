/*============================================================================*/
/* AccessPath.hpp - Access Path                                               */
/*============================================================================*/
#pragma once

#include "eAT.h"

namespace Plans
{
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
    ~AccessPath(void) noexcept(false);
  
    Table* pTable(void);
    void vTable(Table* pTable);
    
    TablePtn* pTablePtn(void);
    void vPartition(TablePtn* pTablePtn);
    
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
