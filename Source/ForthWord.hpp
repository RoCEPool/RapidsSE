/*============================================================================*/
/* ForthWord.hpp -- ForthWord in Dictionary                                             */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class AttachMap;
}

namespace Forth
{
  enum eWT // word type
  {
    /*  0 */ WT_NONE=0,     // none
    /*  1 */ WT_LOCAL,      // word ia a local
    /*  2 */ WT_OBJECT,     // word is an object
    /*  3 */ WT_PRIMITIVE,  // word is a primitive (eg +, return)
    /*  4 */ WT_ROUTINE,    // word is a routine
    /*  5 */ WT_ENUMS
  };
  
  static Stz* aWT[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "LOCAL"_Stz,
    /*  2 */ "OBJECT"_Stz,
    /*  3 */ "PRIMITIVE"_Stz,
    /*  4 */ "ROUTINE"_Stz,
    /*  5 */ "ENUMS"_Stz,
  };
  
  static const i8 C_WSP_WORD = C_SIZE_MEDIUM_OBJECT - C_1KB;
  
  class ForthWord : public Object
  {
    friend class Dic;

  private:
    s32      m_sName;        // the name of this word
    eWT      m_eWordType;    // type of word
    bool     m_bPersistent;  // is on persistent heap
    i8       m_iTrace;       // trace level
    i8       m_iEpoch;       // epoch no. used by bp analyzer
    Dic*     m_pDic;         // which dictionary this word in
    ForthWord*    m_pKind;        // kind of word
    bool     m_bImmediate;   // immediate execution during compile?
    i8       m_iParams;      // number of parameters
    ForthWord*    m_pPrevWord;    // previous word in dictionary
    bool     m_bPrompt;      // can be executed by prompt?
    i8       m_iLocalNo;     // local number (for a local variable)
    StepFp   m_fpEntryStep;  // entry step function ∫for this word
    Cell*    m_aEntryCell;   // the cell to start executing from
                             //   for normal words this will be the first cell
                             //     in the data area
                             //   for objects this will be the cell after
                             //     does in the defining word
                             //   for primitives this will be null  
    Routine  m_xRoutine;
  
    // must be last
    Footer m_xFooter;
  
   public:
    void* operator new(size_t, Dic* pDic);
    void* operator new(size_t, Dic* pDic, bool bPersistent);
    void  operator delete(void* pWordAddress);
  
    ForthWord(Stz* sName);
    ~ForthWord(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static ForthWord* pToWord(void* aAddrInsideWsp);

    byte* ayAllot(i8 iBytes);
    void* aAppend(Cell* aCell);
    void  vAppend(Cell* aCell);
    void* aAppend(GenVar* pGenVar);
    void  vAppend(GenVar* pGenVar);
    void* aAppend(i8 iInt);
    void  vAppend(i8 iInt);
    void* aAppend(StepFp* afpStep);
    void  vAppend(StepFp* afpStep);
    void* aAppend(Value* aValue);
    void  vAppend(Value* aValue);
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
    void  vEncode(Stz* aWordNm, ForthWord* pCalledWord);
    void  vEncode(Stz* aWordNm, GenVar* pGenVar);
    void  vEncode(StepFp* afpEntryStep);
    Cell* aFirstCell(void);
    bool  bHasCell(Cell* aCell);
    bool  bImmediate(void);
    void  vImmediate(bool bImmediate);
    bool  bIsName(s32& sName);
    ForthWord* pKind(void);
    void  vKind(ForthWord* pKind);
    Cell* aLastCell(void);
    i8    iLocalNo(void);
    void  vLocalNo(i8 iLocalNo);
    s32&  sName(void);
    void  vObjectDump(Stz* aTab);
    void  vParams(i8 iParams);
    i8    iParams(void);
    ForthWord* pPrevWord(void);
    void  vPrevWord(ForthWord* pPrevWord);
    bool  bPrompt(void);
    void  vPrompt(bool bImmediate);
    void  vReset(void);
    i8    iSize(void);
    eWT   eWordType(void);
    void  vWordType(eWT eWordType);
    char* aWspArea(void);

    Footer* aFooter(void);
  };
  
  ForthWord* _pWord(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
