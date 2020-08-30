/*============================================================================*/
/* Concept.hpp - Concept                                                      */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Concept : public Object
  {
  private:
    FacetVec m_pFacetVec; // ... which has facets
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Concept(void);
   ~Concept(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Concept* _pConcept(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
