/*============================================================================*/
/* Transform.hpp - Transform                                                  */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Transform : public Object
  {
  private:
 
    Kbase* m_pKbase; //

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Transform(Kbase* pKbase);
   ~Transform(void) noexcept(false);

    void  vAddAttribute(Fact* pToFact, Fact* pAttribute);
  
    Fact* pNewAttribute(Fact* pToFact,
                    Fact* pFacetKind, Fact* pFactKind, Stz* szFactNm=nullptr);
 
    Fact* pNewFact(Fact* pFactKind, Stz* szFactNm=nullptr);
  
    //Fact* pNewFactv(Fact* pKind, Stz* szFactNm, ...);

  private:
    Footer* aFooter(void);
  };

  Transform* _pTransform(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
