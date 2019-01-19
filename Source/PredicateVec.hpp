/*============================================================================*/
/* PredicateVec.hpp -- vector of predicates                                   */
/*============================================================================*/
#pragma once

namespace Plans
{
  class PredicateVec : public VectorNL_t<CN_PREDICATEVEC,Predicate,CN_PREDICATE,16,u8>
  {
  private:
    i8   m_nMatchedPredicates; // number of matched columns
    bool m_bMatchedPredicate[C_MAX_INDEX_COLUMNS]; // which columns are matched so far
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PredicateVec(void);
    ~PredicateVec(void) noexcept(false);
  
    void vSetMatched(i8 i_Pred);
  
    bool bFullyMatched(void);
  
    static PredicateVec* _pPredicateVec(void* aAddr);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
