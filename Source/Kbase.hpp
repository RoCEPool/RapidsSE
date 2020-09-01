/*============================================================================*/
/* Kbase.hpp - Kbase                                                          */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Kbase : public Object
  {
  private:

    Fact*  m_pRoot; //
  
    Atomic m_ixFactNo;
  
    Fact*  m_pFact;
    Fact*  m_pFirst;
    Fact*  m_pInt;
    Fact*  m_pObject;
    Fact*  m_pName;
    Fact*  m_pNext;
    Fact*  m_pNode;
    Fact*  m_pPrev;
    Fact*  m_pText;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Kbase(Stz* szKbName);
   ~Kbase(void) noexcept(false);
  
    Fact* pFactNew(Fact* pFactKind, Stz* szFactNm=nullptr);
    void vFactDelete(Fact* pFact);
  
    Question* pQuestionBorrow(void);
    void vQuestionReturn(Question* pQuestion);
    
    Transform* pTransformBorrow(void);
    void vTransformReturn(Transform* pTransform);
  
  
    void  vAddFacet(Fact* pToFact, Facet* pFacet);
    void  vAddFacet(Fact* pToFact, Fact* pFactKind, Fact* pFact);

    Fact* pNewFacet(Fact* pToFact, Fact* pFacetKind, Stz* szFactNm = nullptr);

    Fact* pNewFact(Fact* pFactKind = nullptr, Stz* szFactNm=nullptr);
 
    //Fact* pNewFactv(Fact* pFactKind, Stz* szFactNm, ...);

  
    bool bIsFactKind(Fact* pFact)    { return (pFact == m_pFact);   }
    bool bIsFirstKind(Fact* pFact)   { return (pFact == m_pFirst);  }
    bool bIsIntKind(Fact* pFact)     { return (pFact == m_pInt);    }
    bool bIsNameKind(Fact* pFact)    { return (pFact == m_pName);   }
    bool bIsNextKind(Fact* pFact)    { return (pFact == m_pNext);   }
    bool bIsNodeKind(Fact* pFact)    { return (pFact == m_pNode);   }
    bool bIsObjectKind(Fact* pFact)  { return (pFact == m_pObject); }
    bool bIsPrevKind(Fact* pFact)    { return (pFact == m_pPrev);   }
    bool bIsRootKind(Fact* pFact)    { return (pFact == m_pRoot);   }
    bool bIsTextKind(Fact* pFact)    { return (pFact == m_pText);   }
  
    Fact*  pFactKind(void)    { return m_pFact;   }
    Fact*  pFirstKind(void)   { return m_pFirst;  }
    Fact*  pIntKind(void)     { return m_pInt;    }
    Fact*  pNameKind(void)    { return m_pName;   }
    Fact*  pNextKind(void)    { return m_pNext;   }
    Fact*  pNodeKind(void)    { return m_pNode;   }
    Fact*  pObjectKind(void)  { return m_pObject; }
    Fact*  pPrevKind(void)    { return m_pPrev;   }
    Fact*  pRootKind(void)    { return m_pRoot;   }
    Fact*  pTextKind(void)    { return m_pText;   }

  private:
    void vAprioriFacts(void);
    Footer* aFooter(void);
  };

  Kbase* _pKbase(void* aAddr);
}

#define K_KBASE(pKBASE) Kbase* __pKbase = pKBASE;

#define K_FACT   (__pKbase->pFactKind())
#define K_FIRST  (__pKbase->pFirstKind())
#define K_INT    (__pKbase->pIntKind())
#define K_NAME   (__pKbase->pNameKind())
#define K_NEXT   (__pKbase->pNextKind())
#define K_NODE   (__pKbase->pNodeKind())
#define K_OBJECT (__pKbase->pObjectKind())
#define K_PREV   (__pKbase->pPrevKind())
#define K_ROOT   (__pKbase->pRootKind())
#define K_TEXT   (__pKbase->pTextKind())

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
