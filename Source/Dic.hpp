/*============================================================================*/
/* Dic.hpp - symbol table of words                                            */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Word;
  
  class Dic : public Object
  {
  private:
    s32    m_sName;        // name of this dictionary
    Word*  m_pDicKind;     // kind of dictionary
    bool   m_bFrozen;      // no more words can be added?
    Atomic m_ziRefCount;    // reference count

    Lock   m_oLock;        // lock over:
    Word*  m_pLastWord;    //   last word in dictionary
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
    
    void  vAddWord(Word* pWord);
    Word* pDicKind(void);
    void  vDicKind(Word* pDicKind);
    Word* pFindWord(Stz* aName);
    Word* pFindStep(StepFp fpStep);
    Word* pRemoveWord(s32& sName);
    bool  bFrozen(void);
    void  vFrozen(bool bFrozen);
    bool  bHasWords(void);
    void  vInit(void);
    bool  bIsName(s32& sName);
    Word* pLastWord(void);
    s32&  sName(void);
    void  vName(Stz* aName);
    void  vRefDec(void);
    void  vRefInc(void);
    bool  bRefOut(void);
    i8    nWords(void);
    
    Footer* aFooter(void);
  };
  
  Dic* _pDic(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
