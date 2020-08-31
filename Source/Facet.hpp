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
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Facet(void);
   ~Facet(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  Facet* _pFacet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
