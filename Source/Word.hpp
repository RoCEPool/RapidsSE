/*============================================================================*/
/* Word.hpp -- Forth SingleWord in Dictionary                                  */
/*============================================================================*/
#pragma once

namespace Forth
{
  static const i8 C_WSP_WORD = C_SIZE_MEDIUM_OBJECT - C_1KB;
  
  class Word : public Object
  {
    friend class Dic;

  private:
    Article* m_pArticle;     // the word article

    s32      m_sName;        // the name of this word
    eWT      m_eWordType;    // type of word
    bool     m_bPersistent;  // is on persistent heap
    i8       m_iTrace;       // trace level
    i8       m_iEpoch;       // epoch no. used by bp analyzer
    Dic*     m_pDic;         // which dictionary this word in
    Word*    m_pKind;        // kind of word
    bool     m_bImmediate;   // immediate execution during compile?
    i8       m_iParams;      // number of parameters
    bool     m_bPrompt;      // can be executed by prompt?
    i8       m_iLocalNo;     // local number (for a local variable)
    StepFp   m_fpEntryStep;  // entry step function for this word
    Cell*    m_aEntryCell;   // the cell to start executing from
                             //   for normal words this will be the first cell
                             //     in the data area
                             //   for objects this will be the cell after
                             //     does in the defining word
                             //   for primitives this will be null
  
    s32      m_sExitHook;    // routine to be called on exit
  
    EnmSet*  m_pEnmSet;      // optional ename set
  
    Routine  m_xRoutine;
  
    // must be last
    Footer m_xFooter;
  
   public:
    void* operator new(size_t, Dic* pDic);
    void* operator new(size_t, Dic* pDic, bool bPersistent);
    void  operator delete(void* pWordAddress);
  
    Word(Stz* sName);
    ~Word(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Word* pToWord(void* aAddrInsideWsp);

    byte* ayAllot(i8 iBytes);
    void* aAppend(Cell* aCell);
    void  vAppend(Cell* aCell);
    void* aAppend(GenVar* pGenVar);
    void  vAppend(GenVar* pGenVar);
    void* aAppend(i8 iInt);
    void  vAppend(i8 iInt);
    void* aAppend(StepFp* afpStep);
    void  vAppend(StepFp* afpStep);
    void* aAppend(ForthValue* aValue);
    void  vAppend(ForthValue* aValue);
    Article* pArticle(void);
    void  vArticle(Article* pArticle);
    void  vContentDump(void);
    Dic*  pDic(void);
    i8    iEpoch(void);
    void  vEntryCell(Cell* aCell);
    Cell* aEntryCell(void);
    void  vEntryStep(StepFp fpStep);
    StepFp fpEntryStep(void);
    StepFp* afpEntryStep(void);
    void  vEncode(Stz* aWordNm);
    void  vEncode(Stz* aWordNm, Cell* aCell);
    void  vEncode(Stz* aWordNm, Word* pCalledWord);
    void  vEncode(Stz* aWordNm, GenVar* pGenVar);
    void  vEncode(StepFp* afpEntryStep);
    EnmSet* pEnmSet(void);
    void  vEnmSet(EnmSet* pEnmSet);
    Stz*  szExitHook(void);
    void  vExitHook(Stz* szExitHook);
    Cell* aFirstCell(void);
    bool  bHasCell(Cell* aCell);
    bool  bImmediate(void);
    void  vImmediate(bool bImmediate);
    bool  bIsName(s32& sName);
    Word* pKind(void);
    void  vKind(Word* pKind);
    Cell* aLastCell(void);
    i8    iLocalNo(void);
    void  vLocalNo(i8 iLocalNo);
    s32&  sName(void);
    void  vName(Stz* szName);
    void  vObjectDump(Stz* aTab);
    void  vParams(i8 iParams);
    i8    iParams(void);
    Word* pPrevWord(void);
    bool  bPrompt(void);
    void  vPrompt(bool bImmediate);
    void  vReset(void);
    i8    iSize(void);
    eWT   eWordType(void);
    void  vWordType(eWT eWordType);
    char* aWspArea(void);

    Footer* aFooter(void);
  };
  
  Word* _pWord(void* aAddr);
}

void vEnterRoutine(Tasks::MP_Task mpTask);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
