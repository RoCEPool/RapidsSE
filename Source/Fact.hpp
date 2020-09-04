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
  
    i8       m_iFactNo;      // unique number (synonym of this facts address)
    s256     m_sNote;        // optional textual decription
    Property m_xProperty;    // some property
    
    Fact*    m_pSuper;       // super-class fact

    Fact*    m_pAttrs;       // attributes of this fact
    Fact*    m_pAttributeOf; // attribute of which fact
    Fact*    m_pNextAttribute; // next attribute of fact

    Fact*    m_pElements;    // elements of this fact set
    Fact*    m_pElementOf;   // element of which fact set
    Fact*    m_pNextElement; // next element in fact set
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Fact(i8 iFactNo, Fact* pSuper, Stz* szNote=nullptr);
   ~Fact(void) noexcept(false);
  
    void  vAttributeAdd(Fact* pAttribute);
    Fact* pAttributeOf(void);
    void  vAttributeOf(Fact* pAttributeOf);
    Fact* pAttributes(void);
    Fact* pAttributeThatIsSubClass(Fact* pOfFact);
    Fact* pAttrs(void);
    void  vAttrs(Fact* pAttrs);

    void  vElementAdd(Fact* pElement);
    Fact* pElementOf(void);
    void  vElementOf(Fact* pElementOf);
    Fact* pElements(void);
    void  vElements(Fact* pElems);

    i8    iFactNo(void);
    void  vFactNo(i8 iFactNo);
    
    bool  bHasSpecificAttribute(Fact* pFact);
  
    bool  bIsSubClass(Fact* pOfFact);

    Fact* pNextAttribute(void);
    void  vNextAttribute(Fact* pAttribute);
    Fact* pNextElement(void);
    void  vNextElement(Fact* pElement);
    
    Stz*  szNote(void);
    void  vNote(Stz* szNote);
  
    Property& rxProperty(void);
    
    Fact* pSuper(void);
    void  vSuper(Fact* pSuper);
    
  private:
    Footer* aFooter(void);
  };

  Fact* _pFact(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
