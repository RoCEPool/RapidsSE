/*============================================================================*/
/* Fact.hpp - Fact                                                            */
/*============================================================================*/
#pragma once

namespace Facts
{
  union Value {
    i8       (*m_afFun)(void* aArg);
    i8         m_iInt;       // i8 value
    Method*    m_pMethod;    // pointer to kbase method
    Object*    m_pObject;    // pointer to proper object
    Opaque*    m_aOpaque;    // address of opaque memory
    Text*      m_pText;      // text
    Transform* m_pTransform; // transform
    eFU        m_eUsage;     // how is the fact property used?
    Word*      m_pWord;      // sen word
    Fact*      m_pXref;      // cross-ref to another fact
  };

  //----------------------------------------------------------------------------

  class Fact : public Object
  {
  private:
    
    Kbase* m_pKbase;
  
    i8     m_iFactNo; // unique number (synonym of this facts address)
    s32    m_sFactNm; // name of fact
    Value  m_xValue;  // some value
    
    Fact*  m_pSubclassOf;   // subclass of which fact      (super)
    Fact*  m_pAttributeOf;  // attribute of which fact     (across/column)
    Fact*  m_pElementOf;    // element of which fact set   (down/row)
 
    Fact*  m_pAttributes;   // attributes of this fact
    Fact*  m_pAttributeNext; // next attribute of fact

    Fact*  m_pElements;     // elements of this fact set
    Fact*  m_pElementNext;  // next element in fact set
   
    bool   m_bDeleteMethod; // ~Fact to delete method
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Fact(Kbase* pKbase, i8 iFactNo, Stz* szName);
    Fact(Kbase* pKbase, Fact* pSuper, Stz* szName=nullptr);
    ~Fact(void) noexcept(false);
  
    static void vDeleteRecursively(Fact** ppFact);
    static void vDeleteBelowRecursively(Fact* pFact);

    void  vAncestry(Stz* szDst);
  
    void  vAttributeAppend(Fact* pAttribute);
    void  vAttributeCut(Fact* pAttribute);
    Fact* pAttributeFindNm(Stz* szName);
    Fact* pAttributeFindNmVal(Stz* szName, Stz* szValue);
    void  vAttributeInsert(Fact* pAttribute);
    Fact* pAttributeNext(void);
    void  vAttributeNext(Fact* pAttribute);
    Fact* pAttributeOf(void);
    void  vAttributeOf(Fact* pAttributeOf);
    Fact* pAttributes(void);
    void  vAttributes(Fact* pAttributes);
    Fact* pAttributeThatIsSubClass(Fact* pOfSuperFact);
    i8    iAttributeValNm(Stz* szName);

    void  vElementAppend(Fact* pElement);
    void  vElementCut(Fact* pElement);
    Fact* pElementFindNm(Stz* szName);
    Fact* pElementFindNmVal(Stz* szName, Stz* szValue);
    void  vElementInsert(Fact* pElement);
    Fact* pElementNext(void);
    void  vElementNext(Fact* pElement);
    Fact* pElementOf(void);
    void  vElementOf(Fact* pElementOf);
    Fact* pElements(void);
    void  vElements(Fact* pElements);
    Fact* pElementThatIsSubClass(Fact* pOfSuperFact);

    i8    iFactNo(void);
    void  vFactNo(i8 iFactNo);
    
    Fact* pFind(i8 iFactNo);
    Fact* pFind(Stz* szFactNm);

    Fact* pGetSuperChild(Fact* pOfFact);

    bool  bHasSpecificAttribute(Fact* pFact);
    bool  bHasSpecificElement(Fact* pFact);
  
    void  vHighestCall(Fact*& rpCall);
  
    bool  bIsElementOf(Fact* pOfFact);
    bool  bIsElementOf(Stz* szName);
  
    bool  bIsSubClass(Fact* pOfFact);
    bool  bIsSubClass(Stz* szName);

    bool  bIsSubElement(Fact* pOfFact);
    bool  bIsSubElement(Stz* szName);

    bool  bIsInt(void);
    bool  bIsObject(void);
    bool  bIsOpaque(void);
    bool  bIsText(void);
    bool  bIsWord(void);
    bool  bIsXref(void);

    void  vMethodNew(void);
  
    Fact* pNewInstance(Stz* szName=nullptr);

    Stz*  szName(void);
    void  vName(Stz* szName);
  
    void  vPrint_r(File* pFile, i8 iDepth);
  
    bool  bQuestion(Fact* pQuestion, Opaque* aArg=nullptr);
    bool  bQuestion(Stz* szQuestion, Opaque* aArg=nullptr);

    void  vSetInt(i8 iInt);
    void  vSetText(Stz* szText);

    Fact* pSubclassOf(void);
    void  vSubclassOf(Fact* pSubclassOf);
  
    i8    iValue(void);
    void  vValue(i8 iValue);

    Value& rxValue(void);
    
  private:
    Footer* aFooter(void);
  };

  Fact* _pFact(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
