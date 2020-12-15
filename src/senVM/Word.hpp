/*============================================================================*/
/* Word.hpp -- Forth Word in Dictionary                                       */
/*============================================================================*/
#pragma once

#include "eWK.h"

namespace SenVM
{
  static const i8 C_WSP_WORD = C_SIZE_MEDIUM_OBJECT - C_1KB;
  
  class Word : public Object
  {
    friend class Dic;

  private:
    Article*  m_pArticle;     // the word article

    s32       m_sWordNm;      // the name of this word
    eWK       m_eWord;        // kind of word

    bool      m_bBreakPt;     // this work gas a break point set
    Word*     m_pCategory;    // category of word
    Dic*      m_pDic;         // which dictionary this word in
    i8        m_iEpoch;       // epoch no. used by bp analyzer
    s32       m_sExitHook;    // routine to be called on exit
    s32       m_sExceptionHook; // routine to be called on exception
    bool      m_bImmediate;   // immediate execution during compile?
    Word*     m_pInclude;     // include group
    i8        m_iLocalNo;     // local number (for a local variable)
    Object*   m_pObject;      // optional associated object
    i8        m_iParams;      // number of parameters
    bool      m_bPersistent;  // is on persistent heap
    i8        m_iTrace;       // trace level
    TagSet*   m_pWrdTagSet;   // optional tag set

    //*
    //* the following two must be together in this order
    //
    StepFp    m_fpIntrinsic;  // intrinsic function for this word
                              //  IMPORTANT:
                              //  the address of this member
                              //    (and *not* the value of the member)
                              //    gets encoded into calling words routine
                              //  this allows the address of m_aInxPtr
                              //  to be computed
      
    Cell*     m_aInxPtr;      // the cell to start executing from for
                              //   - normal words: this will be the
                              //       first cell in the routine
                              //   - future words: this will be
                              //      the cell after the
                              //      in the defining word
                              //   - steps: this will be null
                              //   IMPORTANT:
                              //   m_aInxPtr must be immediately after
                              //     m_fpIntrinsic

    Routine*  m_pRoutine;    // the encoded routine
  
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
    static Word* pToWord(void* aEmbeddedObject);

    byte* ayAllot(i8 iBytes);
    Article* pArticle(void);
    void  vArticle(Article* pArticle);
    bool  bBreakPt(void);
    void  vBreakPt(bool bBreakPt);
    Word* pCategory(void);
    void  vCategory(Word* pCategory);
    void  vContentDump(void);
    Dic*  pDic(void);
    void* aEncodeArg(Cell* aArgCell);
    void  vEncodeCall(StepFp* afpEntryStep);
    void  vEncodeCall(Stz* aWordNm);
    void  vEncodeJump(Stz* aWordNm, Cell* aArgCell);
    void  vEncodePush(AnyVal* aAnyVal);
    void  vEntryCell(Cell* aInxPtr);
    Cell* aEntryCell(void);
    void  vEntryStep(StepFp fpIntrinsic);
    StepFp fpEntryStep(void);
    StepFp* afpEntryStep(void);
    i8    iEpoch(void);
    Stz*  szExceptionHook(void);
    void  vExceptionHook(Stz* szExceptionHook);
    Stz*  szExitHook(void);
    void  vExitHook(Stz* szExitHook);
    Cell* aFirstCell(void);
    bool  bHasCell(Cell* aCell);
    bool  bImmediate(void);
    void  vImmediate(bool bImmediate);
    Word* pInclude(void);
    void  vInclude(Word* pInclude);
    bool  bIsName(s32& sName);
    Cell* aLastCell(void);
    i8    iLocalNo(void);
    void  vLocalNo(i8 iLocalNo);
    Object* pObject(void);
    void  vObject(Object* pObject);
    void  vObjectDump(Stz* aTab);
    void  vParams(i8 iParams);
    i8    iParams(void);
    Word* pPrevWord(void);
    void  vResetRoutine(void);
    Routine* pRoutine(void);
    i8    nRoutineUsed(void);
    eWK   eWord(void);
    void  vWord(eWK eWord);
    Stz*  szWordNm(void);
    void  vWordNm(Stz* szWordNm);
    TagSet* pWrdTagSet(bool bCreateNew = false);
    void  vWrdTagSet(TagSet* pWrdTagSet);
    char* aWspArea(void);

  
  private:
    Footer* aFooter(void);
  };
  
  Word* _pWord(void* aAddr);
}

void vEnterRoutine(Forth* pForth);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
