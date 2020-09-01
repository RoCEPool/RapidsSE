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

    Fact*    m_pPropertyKind;
    Property m_xProperty;

    FacetVec* m_pFacetVec; // ... which has facets
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Fact(i8 iFactNo, Fact* pPropertyKind, Stz* szFactNote=nullptr);
   ~Fact(void) noexcept(false);
  
    void vAddFacet(Facet* pFacet);

    i8   iFactNo(void);
    void vFactNo(i8 iFactNo);
    
    Stz* szFactNote(void);
    void vFactNote(Stz* szFactNote);
  
    Property& rxProperty(void);

    Fact* pPropertyKind(void);
    void vPropertyKind(Fact* pPropertyKind);
    
  private:
    Footer* aFooter(void);
  };

  Fact* _pFact(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
