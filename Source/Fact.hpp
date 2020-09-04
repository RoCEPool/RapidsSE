/*============================================================================*/
/* Fact.hpp - Fact                                                            */
/*============================================================================*/
#pragma once

namespace Facts
{
  union Property {
    Fact*   m_pAssociation;
    i8      m_iInt;
    Object* m_pObject;
    void*   m_aOpaque;
    Text*   m_pText;
  };

  //----------------------------------------------------------------------------

  class Fact : public Object
  {
  private:
  
    i8    m_iFactNo; // unique number (synonym of this facts address)
    Fact* m_pParent; // parent (super-fact)
    s256  m_sNote;   // optional textual decription

    Property m_xProperty;   // some property
  
    FactVec* m_pAttributes; // a fact may have a set of sub-facts
  
    Fact* m_pNextElement;   // next element in fact set
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Fact(i8 iFactNo, Fact* pParent, Stz* szNote=nullptr);
   ~Fact(void) noexcept(false);
  
    void vAddAttribute(Fact* pAttribute);
    FactVec* pAttributes(void);
  
    void vAppend(Fact* pElement);
  
    i8   iFactNo(void);
    void vFactNo(i8 iFactNo);
    
    Fact* pAttributeThatIsSubClass(Fact* pOfFact);
    bool bHasSpecificAttribute(Fact* pFact);
    bool bIsSubClass(Fact* pOfFact);

    Fact* pNextElement(void);
    void vNextElement(Fact* pElement);
    
    Stz* szNote(void);
    void vNote(Stz* szNote);
  
    Fact* pParent(void);
    void vParent(Fact* pParent);
  
    Property& rxProperty(void);
    
  private:
    Footer* aFooter(void);
  };

  Fact* _pFact(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
