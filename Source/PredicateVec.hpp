/*============================================================================*/
/* PredicateVec.hpp -- vector of predicates                                   */
/*============================================================================*/
#pragma once

namespace Parsing
{
  class PredicateVec : public Vector<CN_PREDICATEVEC,Predicate,CN_PREDICATE,16,u8>
  {
  private:
    
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PredicateVec(void);
    ~PredicateVec(void);
  
    static PredicateVec* _pPredicateVec(void* aAddr);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
