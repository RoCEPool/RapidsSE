/*============================================================================*/
/* Facet.hpp - Facet                                                          */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Facet : public Object
  {
  private:

    Fact* m_pAttributeKind;
    Fact* m_pAttribute;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Facet(Fact* pAttributeKind, Fact* pAttribute);
   ~Facet(void) noexcept(false);

    Fact* pAttribute(void);
    Fact* pAttributeKind(void);

  private:
    Footer* aFooter(void);
  };

  Facet* _pFacet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
