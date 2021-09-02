/*============================================================================*/
/* Word.hpp -- Forth Word in Dictionary                                       */
/*============================================================================*/
#pragma once

#include "eWK.h"

namespace BASE
{
  static const i8 C_WSP_WORD = C_SIZE_MEDIUM_OBJECT - C_1KB;
  
  class Word : public Object
  {
    friend class Dic;

  private:
    SkipNode* m_pSkipNode;    // the node of this word in dic skiplist

    s32       m_sWordNm;      // the name of this word
    eWK       m_eWord;        // kind of word

    bool      m_bBreakPt;     // this work gas a break point set
    Word*     m_pCategory;    // category of word
    Dic*      m_pDic;         // which dictionary this word in
    i8        m_iEpoch;       // epoch no. used by bp analyzer
    s32       m_sExitHook;    // routine to be called on exit
    bool      m_bImmediate;   // immediate execution during compile?
    i8        m_iLocalNo;     // local number (for a local variable)
    Object*   m_pObject;      // optional associated object
    i8        m_iParams;      // number of parameters
    bool      m_bPersistent;  // is on persistent heap
    i8        m_iTrace;       // trace level
    TagSet*   m_pWrdTagSet;   // optional tag set

    //*
    //* the following two must be together in this order
    //
    fpSTEPFN m_fpIntrinsic;  // intrinsic function for this word
                             //  IMPORTANT:
                             //  the address of this member
                             //    (and *not* the value of the member)
                             //    gets encoded into calling words routine
                             //  this allows the address of m_aInxPtr
                             //  to be computed 
      
    CodeCell* m_aInxPtr;    // the cell to start executing from for
                            //   - normal words: this will be the
                            //       first cell in the routine
                            //   - future words: this will be
                            //      the cell after the
                            //      in the defining word
                            //   - steps: this will be null
                            //   IMPORTANT:
                            //   m_aInxPtr must be immediately after
                            //     m_fpIntrinsic

    CodeBlock* m_pCodeBlock;  // the encoded : word
  
    // must be last
    Footer m_xFooter;
  
   public:
    void* operator new(size_t, Dic* pDic);
    void* operator new(size_t, Dic* pDic, bool bPersistent);
    void  operator delete(void* pWordAddress);
  
    Word(Stz* szName);
   ~Word(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Word* pToWord(void* aEmbeddedObject);

    byte* ayAllot(i8 iBytes);
    bool  bBreakPt(void);
    void  vBreakPt(bool bBreakPt);
    Word* pCategory(void);
    void  vCategory(Word* pCategory);
    CodeBlock* pCodeBlock(void);
    i8    nCodeBlockUsed(void);
    void  vContentDump(void);
    Dic*  pDic(void);
    void* aEncodeArg(CodeCell* aArgCell);
    void  vEncodeCall(fpSTEPFN* afpEntryStep);
    void  vEncodeCall(Stz* aWordNm);
    void  vEncodeJump(Stz* aWordNm, CodeCell* aArgCell);
    void  vEncodePush(TypedCell* aTypedCell);
    void  vEntryCell(CodeCell* aInxPtr);
    CodeCell* aEntryCell(void);
    void  vEntryStep(fpSTEPFN fpIntrinsic);
    fpSTEPFN fpEntryStep(void);
    fpSTEPFN* afpEntryStep(void);
    i8    iEpoch(void);
    Stz*  szExitHook(void);
    void  vExitHook(Stz* szExitHook);
    CodeCell* aFirstCell(void);
    bool  bHasCell(CodeCell* aCell);
    bool  bImmediate(void);
    void  vImmediate(bool bImmediate);
    bool  bIsName(s32& sName);
    CodeCell* aLastCell(void);
    i8    iLocalNo(void);
    void  vLocalNo(i8 iLocalNo);
    Word* pNextWord(void);
    Object* pObject(void);
    void  vObject(Object* pObject);
    void  vObjectDump(Stz* aTab);
    void  vParams(i8 iParams);
    i8    iParams(void);
    Word* pPrevWord(void);
    void  vResetCodeBlock(void);
    eWK   eWord(void);
    void  vWord(eWK eWord);
    Stz*  szWordNm(void);
    void  vWordNm(Stz* szWordNm);
    TagSet* pWrdTagSet(bool bCreateNew = false);
    void  vWrdTagSet(TagSet* pWrdTagSet);
    char* aWspArea(void);

  
  //private:
    Footer* aFooter(void);
  };
  
  Word* _pWord(void* aAddr);
}

bool bWordCompare(void* aWordBeingInserted, void* aWordScan, bool& bMatching);
void vWordPrint(void* aWordBeingInserted);

//void vDoColon(Step* pStep);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
