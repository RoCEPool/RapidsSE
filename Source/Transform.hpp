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

    void  vAddFacet(Fact* pParent, Fact* pKind, Fact* pKid);
  
    Fact* pNewFacet(Fact* pParent, Fact* pKind, Stz* szFactNm = nullptr);
 
    Fact* pNewFact(Fact* pKind = nullptr);
    Fact* pNewFact(Stz* szFactNm, Fact* pKind = nullptr);
  
    Fact* pNewFactv(Fact* pKind, ...);
    Fact* pNewFactv(Stz* szFactNm, Fact* pKind, ...);

  private:
    Footer* aFooter(void);
  };

  Transform* _pTransform(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
