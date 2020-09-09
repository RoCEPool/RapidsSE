/*============================================================================*/
/* Fact.hpp - Fact                                                            */
/*============================================================================*/
#pragma once

namespace Facts
{
  union Property {
    i8    (*m_afFun)(void* aArg);
    i8      m_iInt;    // i8 value
    Method* m_pMethod; // pointer to kbase method
    Object* m_pObject; // pointer to proper object
    Opaque* m_aOpaque; // address of opaque memory
    Text*   m_pText;   // text
    eFU     m_eUsage;  // how is the fact property used?
    Word*   m_pWord;   // sen word
    Fact*   m_pXref;   // cross-ref to another fact
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

    Fact(Kbase* pKbase, Fact* pSuper, Stz* szNote=nullptr);
    ~Fact(void) noexcept(false);
  
    void  vAttributeAdd(Fact* pAttribute);
    Fact* pAttributeFind(Stz* szNote, Stz* szStz);
    Fact* pAttributeFind(Stz* szNote, Fact* pSuper=nullptr);
    Fact* pAttributeNext(void);
    void  vAttributeNext(Fact* pAttribute);
    Fact* pAttributeOf(void);
    void  vAttributeOf(Fact* pAttributeOf);
    Fact* pAttributes(void);
    void  vAttributes(Fact* pAttributes);
    Fact* pAttributeThatIsSubClass(Fact* pOfSuperFact);
  
    void  vElementAdd(Fact* pElement);
    Fact* pElementFind(Stz* szNote, Fact* pSuper=nullptr);
    Fact* pElementNext(void);
    void  vElementNext(Fact* pElement);
    Fact* pElementOf(void);
    void  vElementOf(Fact* pElementOf);
    Fact* pElements(void);
    void  vElements(Fact* pElements);
    Fact* pElementSearch(Stz* szNote);
    Fact* pElementThatIsSubClass(Fact* pOfSuperFact);

    i8    iFactNo(void);
    void  vFactNo(i8 iFactNo);
    

    Fact* pGetSuperChild(Fact* pOfFact);

    bool  bHasSpecificAttribute(Fact* pFact);
    bool  bHasSpecificElement(Fact* pFact);

    bool  bIsSubClass(Fact* pOfFact);
    bool  bIsSubClass(Stz* szNote);

    bool  bIsInt(void);
    bool  bIsObject(void);
    bool  bIsOpaque(void);
    bool  bIsText(void);
    bool  bIsWord(void);
    bool  bIsXref(void);

    Fact* pNewInstance(Stz* szNote=nullptr);

    Stz*  szNote(void);
    void  vNote(Stz* szNote);
  
    void  vPrint_r(i8 iDepth, i8 iAs);
  
    Property& rxProperty(void);
    
    bool  bQuestion(Fact* pQuestion, Opaque* aArg=nullptr);
    bool  bQuestion(Stz* szQuestion, Opaque* aArg=nullptr);

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
