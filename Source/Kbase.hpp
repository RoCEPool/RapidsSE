/*============================================================================*/
/* Kbase.hpp - Kbase                                                          */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Kbase : public Object
  {
  friend class Fact;
  friend class Question;
  friend class Transform;
  private:

    Atomic m_ixFactNo; //
  
    Fact*  m_pFact;        //
    Fact*  m_p_Concept_;   //

    Fact*  m_p_Value_;     //
    Fact*  m_p_Int_;       //
    Fact*  m_p_Function_;  //
    Fact*  m_p_Method_;    //
    Fact*  m_p_Object_;    //
    Fact*  m_p_Opaque_;    //
    Fact*  m_p_Text_;      //
    Fact*  m_p_Word_;      //
    Fact*  m_p_Xref_;      //

    Fact*  m_p_Name_;      //
  
    Fact*  m_pContents;    //
  
    Fact*  m_pCurrent;     // current working fact

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Kbase(Stz* szKbName);
   ~Kbase(void) noexcept(false);
  
    Fact* pConceptAdd(Fact* pSuper, Stz* szName);
    void  vConceptAdd(Fact* pSuper, Stz* szName);
    void  vConceptAdd(Stz* szSuperName, Stz* szName);

    Fact* pCurrent(void);
    void vCurrent(Fact* pCurrent);
  
    Fact* pElementFind(Stz* szNote);
    
    Fact* pFact(void);
  
    Fact* pFactNew(Fact* pSuper, Stz* szNote);
    void vFactDelete(Fact* pFact);
  
    i8 iNextFactNo(void);
  
    void vPrint(File* pFile);
  
    Question* pQuestionBorrow(void);
    void vQuestionReturn(Question** ppQuestion);
    
    Transform* pTransformBorrow(void);
    void vTransformReturn(Transform** ppTransform);
  
    Fact* pValue(void);
    
    // -- -- -- -- -- -- --

    void vImportStatement(Stz* szStatement);
    void vImportTable(Table* pTable);

    void vTest(void);
  
  private:
    void vAprioriFacts(void);
  
    Footer* aFooter(void);
  };

  Kbase* _pKbase(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
