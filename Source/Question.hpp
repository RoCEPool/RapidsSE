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

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Question(Kbase* pKbase);
   ~Question(void) noexcept(false);

    void  vAsk(Stz* szSetNm, Stz* szAtrNm);
  
    Fact* pAttribute(void);
  
    bool  bEach(void);

    Fact* pElement(void);
  
    i8    iValue(void);
    Stz*  szValue(void);
  
  private:
    bool  bFirst(void);
    bool  bNext(void);

    Footer* aFooter(void);
  };

  Question* _pQuestion(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
