/*============================================================================*/
/* Question.hpp - Question                                                    */
/*============================================================================*/
#pragma once

namespace Facts
{
  class Question : public Object
  {
  private:

    Kbase* m_pKbase; //
 
    s32    m_sSetNm;
    s32    m_sAtrNm;
  
    bool   m_bInited;

    Fact*  m_pStart;
    Fact*  m_pElement;
    Fact*  m_pAttribute;
  
  
    Fact*  m_pOrigin;
    Fact*  m_pPrev;
  
    bool (*m_afbMatch)(Fact* pElm);
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Question(Kbase* pKbase);
    Question(Kbase* pKbase, Stz* szSetNm, Stz* szAtrNm);
   ~Question(void) noexcept(false);

    static Fact* pFindElm(Kbase* pKbase, Stz* szSetNm, Stz* szElmNm);
    static Fact* pFindElm(Kbase* pKbase, Fact* pSet, Stz* szElmNm);

    void  vAsk(Stz* szSetNm, Stz* szAtrNm);
  
    Fact* pAttribute(void);
  
    bool  bEach(void);

    Fact* pElement(void);
  
    i8    iValue(void);
    Stz*  szValue(void);
  
  private:
    bool  bFirst(void);
    bool  bNext(void);
    Fact* pSearch(Fact* pPrev, Fact** ppAttribute);
  
    Fact* pEachElmByAtrNm(Fact** ppPrev, Fact* pOrigin, Stz* szAtrNm);
    Fact* pEachElmByElmNm(Fact* pOrigin, Stz* szFactNm, Fact** ppPrev);

    bool  bMatchAtrNm(Fact* pElm);
    bool  bMatchElmNm(Fact* pElm);
    Fact* pNextMatchingElm(void);

    Footer* aFooter(void);
  };

  Question* _pQuestion(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
