/*============================================================================*/
/* Fact.hpp - Fact                                                            */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Fact : public Object
  {
  private:

    //Concept* m_pConcept; // about a concept
    FacetVec* m_pFacetVec; // ... which has facets

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Fact(void);
   ~Fact(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Fact* _pFact(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
