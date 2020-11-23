/*============================================================================*/
/* Dic.hpp -- Dictionary (symbol table of words)                              */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class Dic : public Object
  {
  private:
    Article*  m_pArticle;  // the dictionary article
  
    s32       m_sDicNm;    // name of this dictionary
    Word*     m_pDicKind;  // kind of dictionary
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Dic* pDic);
    void  operator delete(void*);
    
    Dic(Stz* szDicNm);
   ~Dic(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void  vAddWord(Word* pWord);
    Article* pArticle(void);
    Word* pDicKind(void);
    void  vDicKind(Word* pDicKind);
    Word* pEachWord(Word* pPrior);
    Word* pEachWord(Word* pPrior, Word* pCategory);
    Word* pFindWord(Stz* szWordNm);
    Word* pFindStep(StepFp fpStep);
    bool  bHasWords(void);
    void  vInit(void);
    bool  bIsName(Stz* szDicNm);
    Word* pLastWord(void);
    void  vListWords(void);
    Stz*  szDicNm(void);
    void  vDicNm(Stz* szDicNm);
    void  vRefDec(void);
    void  vRefInc(void);
    bool  bRefOut(void);
    Word* pRemoveWord(Stz* szWordNm);
    i8    nWords(void);
    
    Footer* aFooter(void);
  };
  
  Dic* _pDic(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
