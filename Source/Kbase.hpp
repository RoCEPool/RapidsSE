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
  
    Fact*  m_pFact;    //
  
    Fact*  m_p_Action_;    //
    Fact*  m_p_Chunk_;     //
    Fact*  m_p_Concept_;   //
    Fact*  m_p_Condition_; //
    Fact*  m_p_Folder_;    //
    Fact*  m_p_Format_;    //
    Fact*  m_p_Function_;  //
    Fact*  m_p_Int_;       //
    Fact*  m_p_Method_;    //
    Fact*  m_p_Name_;      //
    Fact*  m_p_NameSeq_;   //
    Fact*  m_p_Object_;    //
    Fact*  m_p_Opaque_;    //
    Fact*  m_p_Pattern_;   //
    Fact*  m_p_Value_;     //
    Fact*  m_p_Quality_;   //
    Fact*  m_p_Question_;  //
    Fact*  m_p_Questions_; //
    Fact*  m_p_Rule_;      //
    Fact*  m_p_Transform_; //
    Fact*  m_p_Text_;      //
    Fact*  m_p_Tuple_;     //
    Fact*  m_p_Type_;      //
    Fact*  m_p_Word_;      //
    Fact*  m_p_Xref_;      //

    Fact*  m_pContents;    //

    Fact*  m_pCurrent;     // current working fact

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Kbase(Stz* szKbName);
   ~Kbase(void) noexcept(false);
  
    Fact* pConceptAdd(Fact* pSuper, Stz* szNote);
  
    Fact* pCurrent(void);
    void vCurrent(Fact* pCurrent);
  
    Fact* pElementFind(Stz* szNote);
    
    Fact* pFact(void);
  
    Fact* pFactNew(Fact* pSuper, Stz* szNote);
    void vFactDelete(Fact* pFact);
  
    i8 iNextFactNo(void);
  
    Fact* pValue(void);
    
    Question* pQuestionBorrow(void);
    void vQuestionReturn(Question** ppQuestion);
    
    Transform* pTransformBorrow(void);
    void vTransformReturn(Transform** ppTransform);
  
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
