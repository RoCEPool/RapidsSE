/*============================================================================*/
/* Dic.hpp - symbol table of words                                            */
/*============================================================================*/
#pragma once

namespace Dictionaries
{
  class Word;
  
  class Dic : public Object
  {
  private:
    s32    m_sName;        // name of this dictionary
    Word*  m_pDicKind;     // kind of dictionary
    bool   m_bFrozen;      // no more words can be added?
    Atomic m_zRefCount;    // reference count

    Lock   m_xLock;        // lock over:
    Word*  m_pLastWord;    //   last word in dictionary
    i8     m_nWords;       //   number of words in dictionary
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Dic* pDic);
    void  operator delete(void*);
    
    Dic(void);
    ~Dic(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* aTab);

    void  vAddWord(Word* pWord);
    Word* pDicKind(void);
    void  vDicKind(Word* pDicKind);
    Word* pFindWord(const char* aName);
    Word* pFindStep(TaskStep pStep);
    bool  bForget(StrVbl& sName);
    bool  bFrozen(void);
    void  vFrozen(bool bFrozen);
    bool  bHasWords(void);
    void  vInit(void);
    bool  bIsName(StrVbl& sName);
    Word* pLastWord(void);
    StrVbl&    sName(void);
    void  vName(const char* aName);
    void  vRefDec(void);
    void  vRefInc(void);
    bool  bRefOut(void);
    i8    nWords(void);
    
    Footer* pFooter(void);
  };
  
  Dic* _pDic(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
