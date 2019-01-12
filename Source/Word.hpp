/*============================================================================*/
/* Word.hpp -- Word in Dictionary                                             */
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
    /*  1 */ WT_OBJECT,     // word is an object
    /*  2 */ WT_PRIMITIVE,  // word is a primitive (eg +, return)
    /*  3 */ WT_ROUTINE,    // word is a subroutine
    /*  4 */ WT_VARIABLE,      // word ia a local
    /*  5 */ WT_ENUMS
  };
  
  static Stz* aWT[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "OBJECT"_Stz,
    /*  2 */ "PRIMITIVE"_Stz,
    /*  3 */ "ROUTINE"_Stz,
    /*  4 */ "LOCAL"_Stz,
    /*  5 */ "ENUMS"_Stz,
  };
  
  static const i8 C_WSP_WORD = C_SIZE_MEDIUM_OBJECT - C_1KB;
  
  class Word : public Object
  {
    friend class Dic;
    
  
  private:
    s32      m_sName;        // the name of this word
    eWT      m_eWordType;    // type of word
    bool     m_bPersistent;  // is on persistent heap
    i8       m_iTrace;       // trace level
    i8       m_iEpoch;       // epoch no. used by bp analyzer
    Dic*     m_pDic;         // which dictionary this word in
    Word*    m_pKind;        // kind of word
    bool     m_bImmediate;   // immediate execution during compile?
    i8       m_iParams;      // number of parameters
    Word*    m_pPrevWord;    // previous word in dictionary
    i8       m_iVariable;    // variable number
    StepFp   m_fpEntryStep;  // entry step function ∫for this word
    Cell*    m_aEntryCell;   // the cell to start executing from
                             //   for normal words this will be the first cell
                             //     in the data area
                             //   for objects this will be the cell after
                             //     does in the defining word
                             //   for primitives this will be null
    // ..data..
  
    Wsp_t<C_WSP_WORD> m_xBody;
  
  Footer m_xFooter;
  
   public:
    void* operator new(size_t, Dic* pDic);
    void* operator new(size_t, Dic* pDic, bool bPersistent);
    void  operator delete(void* pWordAddress);
  
    Word(Stz* sName);
    ~Word(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    char* aAllot(i8 iBytes);
    void* aAppend(Cell* aCell);
    void  vAppend(Cell* aCell);
    void* aAppend(i8 iInt);
    void  vAppend(i8 iInt);
    void* aAppend(StepFp* aStepFp);
    void  vAppend(StepFp* aStepFp);
    void* aAppend(Value* aValue);
    void  vAppend(Value* aValue);
    void  vContentDump(void);
  
    char* aBodyArea(void);
    char* aBodyLatter(void);
    i8    nBodyUsed(void);
  
    Dic*  pDic(void);
    i8    iEpoch(void);
    void  vEntryCell(Cell* aCell);
    Cell* aEntryCell(void);
    void  vEntryStep(StepFp pStep);
    StepFp pEntryStep(void);
    StepFp* ppEntryStep(void);
    void  vEncode(Stz* aWordNm);
    void  vEncode(Stz* aWordNm, Cell* aCell);
    void  vEncode(Stz* aWordNm, Word* pWord);
    void  vEncode(Stz* aWordNm, Value* aValue);
    void  vEncode(Stz* aWordNm, GenVar* pGenVar);
    void  vEncode(StepFp* ppEntryStep);
    Cell* aFirstCell(void);
    bool  bHasCell(Cell* aCell);
    bool  bImmediate(void);
    bool  bIsName(s32& sName);
    void  vImmediate(bool bImmediate);
    Word* pKind(void);
    void  vKind(Word* pKind);
    Cell* aLastCell(void);
    s32&  sName(void);
    void  vObjectDump(Stz* aTab);
    void  vParams(i8 iParams);
    i8    iParams(void);
    Word* pPrevWord(void);
    void  vPrevWord(Word* pPrevWord);
    void  vReset(void);
    i8    iSize(void);
    i8    iVariable(void);
    void  vVariable(i8 iVariable);
    eWT   eWordType(void);
    void  vWordType(eWT eWordType);
    
    static Word* pToWord(void* pData);
    
    Footer* aFooter(void);
  };
  
  Word* _pWord(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
