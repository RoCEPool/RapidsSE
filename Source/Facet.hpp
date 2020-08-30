/*============================================================================*/
/* Facet.hpp - Facet                                                          */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Facet : public Object
  {
  private:

    PropertyVec m_pPropertyVec; // ... which have properties
  
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
