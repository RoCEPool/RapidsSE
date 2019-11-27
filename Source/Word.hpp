/*============================================================================*/
/* Word.hpp -- Forth Word in Dictionary                                       */
/*============================================================================*/
#pragma once

namespace SenVM
{
  static const i8 C_WSP_WORD = C_SIZE_MEDIUM_OBJECT - C_1KB;
  
  class Word : public Object
  {
    friend class Dic;

  private:
    Article*  m_pArticle;     // the word article

    s32       m_sWordNm;      // the name of this word
  
    Word*     m_pCategory;    // category of word
    Dic*      m_pDic;         // which dictionary this word in
    EnameSet* m_pEnameSet;    // optional ename set
    i8        m_iEpoch;       // epoch no. used by bp analyzer
    s32       m_sExitHook;    // routine to be called on exit
    s32       m_sExceptionHook;    // routine to be called on exception
    bool      m_bImmediate;   // immediate execution during compile?
    eWK       m_eKind;        // kind of word
    i8        m_iLocalNo;     // local number (for a local variable)
    i8        m_iParams;      // number of parameters
    bool      m_bPersistent;  // is on persistent heap
    bool      m_bPrompt;      // can be executed by prompt?
    i8        m_iTrace;       // trace level

    // the following three must be together
    //struct {

    StepFp    m_fpIntrinsic;  // intrinsic function for this word
                              //  the address of this member (and not
                              //  the value of the member) gets encoded
                              //  into another words routine
      
    Cell*     m_aInxPtr;      // the cell to start executing from
                              //   for normal words this will be the first cell
                              //     in the routine
                              //   for objects this will be the cell after
                              //     does in the defining word
                              //   for primitives this will be null
    Routine   m_xRoutine;
    //};
  
    // must be last
    Footer m_xFooter;
  
   public:
    void* operator new(size_t, Dic* pDic);
    void* operator new(size_t, Dic* pDic, bool bPersistent);
    void  operator delete(void* pWordAddress);
  
    Word(Stz* szName);
   ~Word(void) noexcept(false);
  
    static void vArtFun(Object* pObject, SP& spObject);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Word* pToWord(void* aAddrInsideWsp);

    byte* ayAllot(i8 iBytes);
    void* aAppend(Cell* aCell);
    void  vAppend(Cell* aCell);
    void  vAppend(i8 iInt);
    void  vAppend(StepFp* afpStep);
    void  vAppend(StkVal* aStkVal);
    Article* pArticle(void);
    void  vArticle(Article* pArticle);
    Word* pCategory(void);
    void  vCategory(Word* pCategory);
    void  vContentDump(void);
    Dic*  pDic(void);
    void  vEntryCell(Cell* aInxPtr);
    Cell* aEntryCell(void);
    void  vEntryStep(StepFp fpIntrinsic);
    StepFp fpEntryStep(void);
    StepFp* afpEntryStep(void);
    EnameSet* pEnameSet(void);
    void  vEnameSet(EnameSet* pEnameSet);
    void  vEncode(Stz* aWordNm);
    void  vEncode(Stz* aWordNm, Cell* aCell);
    void  vEncode(Stz* aWordNm, Word* pCalledWord);
    void  vEncode(Stz* aWordNm, StkVal* aStkVal);
    void  vEncode(StepFp* afpEntryStep);
    i8    iEpoch(void);
    Stz*  szExceptionHook(void);
    void  vExceptionHook(Stz* szExceptionHook);
    Stz*  szExitHook(void);
    void  vExitHook(Stz* szExitHook);
    Cell* aFirstCell(void);
    bool  bHasCell(Cell* aCell);
    bool  bImmediate(void);
    void  vImmediate(bool bImmediate);
    bool  bIsName(s32& sName);
    eWK   eKind(void);
    void  vKind(eWK eKind);
    Cell* aLastCell(void);
    i8    iLocalNo(void);
    void  vLocalNo(i8 iLocalNo);
    void  vObjectDump(Stz* aTab);
    void  vParams(i8 iParams);
    i8    iParams(void);
    Word* pPrevWord(void);
    bool  bPrompt(void);
    void  vPrompt(bool bImmediate);
    void  vSee(void);
    i8    iSize(void);
    Stz*  szWordNm(void);
    void  vWordNm(Stz* szWordNm);
    char* aWspArea(void);

    Footer* aFooter(void);
  };
  
  Word* _pWord(void* aAddr);
}

void vEnterRoutine(Forth* pForth);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
