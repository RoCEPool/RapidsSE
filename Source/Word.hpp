/*============================================================================*/
/* Word.hpp -- Word in Dictionary                                             */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class AttachMap;
}

namespace Dictionaries
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
  
  static Str* aWT[]
  {
    /*  0 */ "NONE"_Str,
    /*  1 */ "OBJECT"_Str,
    /*  2 */ "PRIMITIVE"_Str,
    /*  3 */ "ROUTINE"_Str,
    /*  4 */ "LOCAL"_Str,
    /*  5 */ "ENUMS"_Str,
  };
  
  enum eDT // distributed type
  {
    /*  0 */ DT_NONE=0,          // none
    /*  1 */ DT_ALLPARTICIPANTS, // word is distributed to all participants
    /*  2 */ DT_ONEPARTICIPANT,  // word is distributed to one participants
    /*  3 */ DT_NOPARTICIPANT,   // word is not distributed to any participants
    /*  4 */ DT_LOCALCALLS,      // word only issues local calls
    /*  5 */ DT_ENUMS
  };
  
  static const char* const pDT[]
  {
    /*  0 */ "NONE",
    /*  1 */ "ALLPARTICIPANTS",
    /*  2 */ "ONEPARTICIPANT",
    /*  3 */ "NOPARTICIPANT",
    /*  4 */ "LOCALCALLS",
    /*  5 */ "ENUMS",
  };
  
  class Word : public Object
  {
    friend class Dic;
    
  private:
    s32      m_sName;        // the name of this word
    eWT      m_eWordType;    // type of word
    eDT      m_eDistributed; // distributed
    i8       m_iEpoch;       // epoch no. used by bp analyzer
    Dic*     m_pDic;         // which dictionary this word in
    Word*    m_pKind;        // kind of word
    Word*    m_pParticipant; // allParticipant/oneParticipant word
    bool     m_bIndirect;    // word is an indirect type
    bool     m_bImmediate;   // immediate execution during compile?
    i8       m_iParams;      // number of parameters
    s32      m_sFinalRoutine; // final routine to be called after all participants repond
    Word*    m_pPrevWord;    // previous word in dictionary
    i8       m_iVariable;    // variable number
//  i8       m_iDataBytes;   // number of data bytes
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
    void  operator delete(void* pWordAddress);
    
    Word(const char* sName);
    ~Word(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* aTab);
    
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
    void  vDistributed(eDT eDistributed);
    eDT   eDistributed(void);
    void  vEncode(const char* aWord);
    void  vEncode(const char* aWord, Cell* pCell);
    void  vEncode(const char* aWord, Word* pWord);
    void  vEncode(const char* aWord, Value* pValue);
    void  vEncode(TaskStep* ppEntryStep);
    void  vFinalRoutine(const char* pRoutine);
    char* pFinalRoutine(void);
    Cell* pFirstCell(void);
    bool  bHasCell(Cell* pCell);
    bool  bImmediate(void);
    bool  bIsName(StrVal& sName);
    void  vImmediate(bool bImmediate);
    bool  bIndirect(void);
    void  vIndirect(bool bIndirect);
    Word* pKind(void);
    void  vKind(Word* pKind);
    Cell* pLastCell(void);
    StrVal&    sName(void);
    void  vObjectDump(const char* aTab);
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
    
    Footer* pFooter(void);
  };
  
  Word* _pWord(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
