/*============================================================================*/
/* Dic.hpp -- Dictionary (symbol table of words)                              */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Dic : public Object
  {
  private:
    Article*    m_pArticle;  // the article
  
    s32         m_sName;     // name of this dictionary
    ForthWord*  m_pDicKind;  // kind of dictionary

    ForthWord*  m_pLastWord;    //   last word in dictionary
    i8     m_nWords;       //   number of words in dictionary
    
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
    
    void  vAddWord(ForthWord* pWord);
    ForthWord* pDicKind(void);
    void  vDicKind(ForthWord* pDicKind);
    ForthWord* pEachWord(ForthWord* pKind, ForthWord* pPrior);
    ForthWord* pFindWord(Stz* aName);
    ForthWord* pFindStep(StepFp fpStep);
    bool  bHasWords(void);
    void  vInit(void);
    bool  bIsName(s32& sName);
    ForthWord* pLastWord(void);
    void  vListWords(void);
    s32&  sName(void);
    void  vName(Stz* aName);
    void  vRefDec(void);
    void  vRefInc(void);
    bool  bRefOut(void);
    ForthWord* pRemoveWord(s32& sName);
    i8    nWords(void);
    
    Footer* aFooter(void);
  };
  
  Dic* _pDic(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
