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

    Fact*  m_p_Property_;  //

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
    Fact*  m_p_Folder_;    //
    Fact*  m_p_Tuple_;     //

    Fact*  m_pContents;    //
  
    Fact*  m_pCurrent;     // current working fact

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Kbase(Stz* szKbName);
   ~Kbase(void) noexcept(false);
  
    Fact* pAttributeInstance(Fact* pFact, Stz* szAtrSuperNm);
    void  vAttributeInstance(Fact* pFact, Stz* szAtrSuperNm);
    Fact* pAttributeInstance(Fact* pFact, Stz* szAtrSuperNm, i8 iVal);
    void  vAttributeInstance(Fact* pFact, Stz* szAtrSuperNm, i8 iVal);
    Fact* pAttributeInstance(Fact* pFact, Stz* szAtrSuperNm, Stz* szVal);
    void  vAttributeInstance(Fact* pFact, Stz* szAtrSuperNm, Stz* szVal);

    Fact* pConceptAdd(Stz* szName, eFU eUsage=FU_NONE);
    void  vConceptAdd(Stz* szName);
    Fact* pConceptAdd(Fact* pSuper, Stz* szName, eFU eUsage=FU_NONE);
    void  vConceptAdd(Fact* pSuper, Stz* szName);
    void  vConceptAdd(Stz* szSuperName, Stz* szName);
    Fact* pConceptInstance(Stz* szAhaSuperNm);

    Fact* pCurrent(void);
    void  vCurrent(Fact* pCurrent);
  
    Fact* pElementFind(Stz* szName);
    Fact* pElementInstance(Fact* pSet, Stz* szElmSuperNm);
    Fact* pElementInstance(Stz* szSetNm, Stz* szElmSuperNm);
    
    Fact* pFact(void);
    void  vFactDelete(Fact* pFact);
    Fact* pFactNew(Fact* pSuper, Stz* szNote);

    i8 iNextFactNo(void);
  
    void vPrint(File* pFile);
        
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
