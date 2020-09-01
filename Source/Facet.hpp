/*============================================================================*/
/* Facet.hpp - Facet                                                          */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Facet : public Object
  {
  private:

    FeatureVec* m_pFeatureVec; // ... which have features
  
  
    Fact* m_pFacetKind;
    Fact* m_pFact;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Facet(Fact* pKind, Fact* pFact);
   ~Facet(void) noexcept(false);

    Fact* pFacetKind(void);
    Fact* pFact(void);

  private:
    Footer* aFooter(void);
  };

  Facet* _pFacet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
