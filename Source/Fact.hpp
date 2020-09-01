/*============================================================================*/
/* Fact.hpp - Fact                                                            */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Fact : public Object
  {
  private:
  
    i8  m_iFactNo;
    s32 m_sFactNote;

    Fact*    m_pPropKind;
    Property m_xProperty;

    FacetVec* m_pFacetVec; // ... which has facets
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Fact(i8 iFactNo, Fact* pPropKind, Stz* szFactNote=nullptr);
   ~Fact(void) noexcept(false);
  
    Stz* szFactNote(void);
    void vFactNote(Stz* szFactNote);

    i8   iFactNo(void);
    void vFactNo(i8 iFactNo);
  
    Fact* pPropKind(void);
    void vPropKind(Fact* pPropKind);
    
    void vAddFacet(Facet* pFacet);

  private:
    Footer* aFooter(void);
  };

  Fact* _pFact(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
