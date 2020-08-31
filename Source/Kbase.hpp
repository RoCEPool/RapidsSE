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
    Fact*  m_pPrev;
    Fact*  m_pText;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Kbase(Stz* szKbName);
   ~Kbase(void) noexcept(false);
  
    Fact* pFactNew(void);
    void vFactDelete(Fact* pFact);
  
    Question* pQuestionBorrow(void);
    void vQuestionReturn(Question* pQuestion);
    
    Transform* pTransformBorrow(void);
    void vTransformReturn(Transform* pTransform);
  
    bool bIsFact(Fact* pFact)    { return (pFact == m_pFact);   }
    bool bIsFirst(Fact* pFact)   { return (pFact == m_pFirst);  }
    bool bIsInt(Fact* pFact)     { return (pFact == m_pInt);    }
    bool bIsObject(Fact* pFact)  { return (pFact == m_pObject); }
    bool bIsName(Fact* pFact)    { return (pFact == m_pName);   }
    bool bIsNext(Fact* pFact)    { return (pFact == m_pNext);   }
    bool bIsPrev(Fact* pFact)    { return (pFact == m_pPrev);   }
    bool bIsRoot(Fact* pFact)    { return (pFact == m_pRoot);   }
    bool bIsText(Fact* pFact)    { return (pFact == m_pText);   }

  private:
    void vAprioriFacts(void);
    Footer* aFooter(void);
  };

  Kbase* _pKbase(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
