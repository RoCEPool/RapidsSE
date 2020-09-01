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
    s32 m_sFactNm;

    //Concept* m_pConcept; // about a concept
    FacetVec* m_pFacetVec; // ... which has facets
  
    Fact*    m_pFactKind;
    Property m_xProperty;


    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Fact(i8 iFactNo, Fact* pKind);
   ~Fact(void) noexcept(false);
  
    Stz* szFactNm(void);
    void vFactNm(Stz* szFactNm);

    i8   iFactNo(void);
    void vFactNo(i8 iFactNo);
  
    Fact* pFactKind(void);
    void vFactKind(Fact* pFactKind);
    
    void vAddFacet(Facet* pFacet);

  private:
    Footer* aFooter(void);
  };

  Fact* _pFact(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
