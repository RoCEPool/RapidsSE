/*============================================================================*/
/* Kbase.hpp - Kbase                                                          */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Kbase : public Object
  {
  friend class Question;
  friend class Transform;
  private:

    Atomic m_ixFactNo;   //
  
    Fact*  m_pFact;      //
    Fact*  m_pContents;  //
  
    Fact*  m_pAssociation;
    Fact*  m_pInt;
    Fact*  m_pObject;
    Fact*  m_pOpaque;
    Fact*  m_pText;

    Fact*  m_pSet;
    Fact*  m_pElement;
    Fact*  m_pFirst;
    Fact*  m_pLast;
    Fact*  m_pNext;
    Fact*  m_pPrev;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Kbase(Stz* szKbName);
   ~Kbase(void) noexcept(false);
  
    Fact* pFact(void);
  
    Fact* pFactNew(Stz* szNote, Fact* pParent);
    void vFactDelete(Fact* pFact);
  
    Question* pQuestionBorrow(void);
    void vQuestionReturn(Question** ppQuestion);
    
    Transform* pTransformBorrow(void);
    void vTransformReturn(Transform** ppTransform);
  
  private:
    void vAprioriFacts(void);
  
    Footer* aFooter(void);
  };

  Kbase* _pKbase(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
