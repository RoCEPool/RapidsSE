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
    TaskStep m_pEntryStep;   // entry step for this word
    Cell*    m_pEntryCell;   // the cell to start executing from
                             //   for normal words this will be the first cell
                             //     in the data area
                             //   for objects this will be the cell after
                             //     does in the defining word
                             //   for primitives this will be null
    // ..data..
  
    WspFxd<C_SIZE_MEDIUM_OBJECT - C_1KB> m_xBody;
  
  Footer m_xFooter;
  
   public:
    void* operator new(size_t, Dic* pDic);
    void* operator new(size_t, Dic* pDic, bool bPersistent);
    void  operator delete(void* pWordAddress);
  
    Word(Stz* sName);
    ~Word(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    char* pAllot(i8 iBytes);
    void* pAppend(Cell* pCell);
    void  vAppend(Cell* pCell);
    void* pAppend(i8 iInt);
    void  vAppend(i8 iInt);
    void* pAppend(TaskStep* ppStep);
    void  vAppend(TaskStep* ppStep);
    void* pAppend(Value* pValue);
    void  vAppend(Value* pValue);
    void  vContentDump(void);
  
    char* aBodyArea(void);
    char* aBodyLatter(void);
    i8    nBodyUsed(void);
  
    Dic*  pDic(void);
    i8    iEpoch(void);
    void  vEntryCell(Cell* pCell);
    Cell* pEntryCell(void);
    void  vEntryStep(TaskStep pStep);
    TaskStep pEntryStep(void);
    TaskStep* ppEntryStep(void);
    void  vEncode(Stz* aWord);
    void  vEncode(Stz* aWord, Cell* pCell);
    void  vEncode(Stz* aWord, Word* pWord);
    void  vEncode(Stz* aWord, Value* pValue);
    void  vEncode(Stz* aWord, GenVar* pValWrk);
    void  vEncode(TaskStep* ppEntryStep);
    Cell* pFirstCell(void);
    bool  bHasCell(Cell* pCell);
    bool  bImmediate(void);
    bool  bIsName(StrVal& sName);
    void  vImmediate(bool bImmediate);
    Word* pKind(void);
    void  vKind(Word* pKind);
    Cell* pLastCell(void);
    StrVal&    sName(void);
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
