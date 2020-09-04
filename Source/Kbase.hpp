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
  
    Fact*  m_pAssociation;
    Fact*  m_pInt;
    Fact*  m_pObject;
    Fact*  m_pOpaque;
    Fact*  m_pText;
    Fact*  m_pWord;

    Fact*  m_pName;

    Fact*  m_pQuality; //
    Fact*  m_pType;    //
    Fact*  m_pTuple;   //
    Fact*  m_pFolder;  //

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
  
    i8 iNextFactNo(void);
  
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
