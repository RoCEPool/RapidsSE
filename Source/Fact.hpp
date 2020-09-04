/*============================================================================*/
/* Fact.hpp - Fact                                                            */
/*============================================================================*/
#pragma once

namespace Facts
{
  union Property {
    Fact*   m_pAssociation; // association to another fact
    i8      m_iInt;         // i8 value
    Object* m_pObject;      // pointer to proper object
    void*   m_aOpaque;      // address of opaque object
    Text*   m_pText;        // text
    Word*   m_pWord;        // sen word
  };

  //----------------------------------------------------------------------------

  class Fact : public Object
  {
  private:
  
    Kbase*   m_pKbase;
  
    i8       m_iFactNo;      // unique number (synonym of this facts address)
    s256     m_sNote;        // optional textual decription
    Property m_xProperty;    // some property
    
    Fact*    m_pSubclassOf;  // subclass of which fact      (super)
    Fact*    m_pAttributeOf; // attribute of which fact     (across/column)
    Fact*    m_pElementOf;   // element of which fact set   (down/row)
 
    Fact*    m_pAttributes;  // attributes of this fact
    Fact*    m_pAttributeNext; // next attribute of fact

    Fact*    m_pElements;    // elements of this fact set
    Fact*    m_pElementNext; // next element in fact set
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Fact(i8 iFactNo, Fact* pSuper, Stz* szNote=nullptr);
    Fact(Kbase* pKbase, Fact* pSuper, Stz* szNote=nullptr);
    ~Fact(void) noexcept(false);
  
    void  vAttributeAdd(Fact* pAttribute);
    Fact* pAttributeNext(void);
    void  vAttributeNext(Fact* pAttribute);
    Fact* pAttributeOf(void);
    void  vAttributeOf(Fact* pAttributeOf);
    Fact* pAttributes(void);
    void  vAttributes(Fact* pAttributes);
    Fact* pAttributeThatIsSubClass(Fact* pOfSuperFact);
  
    Fact* pInstanceOf(Fact* pFact);

    void  vElementAdd(Fact* pElement);
    Fact* pElementNext(void);
    void  vElementNext(Fact* pElement);
    Fact* pElementOf(void);
    void  vElementOf(Fact* pElementOf);
    Fact* pElements(void);
    void  vElements(Fact* pElements);
    Fact* pElementThatIsSubClass(Fact* pOfSuperFact);

    i8    iFactNo(void);
    void  vFactNo(i8 iFactNo);
    
    bool  bHasSpecificAttribute(Fact* pFact);
    bool  bHasSpecificElement(Fact* pFact);

    bool  bIsSubClass(Fact* pOfFact);

    Stz*  szNote(void);
    void  vNote(Stz* szNote);
  
    Property& rxProperty(void);
    
    Fact* pSubclassOf(void);
    void  vSubclassOf(Fact* pSubclassOf);
    
  private:
    Footer* aFooter(void);
  };

  Fact* _pFact(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
