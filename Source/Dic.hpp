/*============================================================================*/
/* Dic.hpp -- Dictionary (symbol table of words)                              */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Dic : public Object
  {
  private:
    Article*  m_pArticle;  // the dictionary article
  
    s32       m_sName;     // name of this dictionary
    Word*     m_pDicKind;  // kind of dictionary
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Dic* pDic);
    void  operator delete(void*);
    
    Dic(void);
   ~Dic(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    void  vAddWord(Word* pWord);
    Article* pArticle(void);
    Word* pDicKind(void);
    void  vDicKind(Word* pDicKind);
    Word* pEachWord(Word* pPrior);
    Word* pEachWord(Word* pCategory, Word* pPrior);
    Word* pFindWord(Stz* szName);
    Word* pFindStep(StepFp fpStep);
    bool  bHasWords(void);
    void  vInit(void);
    bool  bIsName(Stz* szName);
    Word* pLastWord(void);
    void  vListWords(void);
    Stz*  szName(void);
    void  vName(Stz* szName);
    void  vRefDec(void);
    void  vRefInc(void);
    bool  bRefOut(void);
    Word* pRemoveWord(Stz* szName);
    i8    nWords(void);
    
    Footer* aFooter(void);
  };
  
  Dic* _pDic(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
