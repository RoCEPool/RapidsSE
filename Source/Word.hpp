/*============================================================================*/
/* Word.hpp - word in dictionary                                              */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  enum eWT // word type
  {
    /*  0 */ WT_NONE=0,    // none
    /*  1 */ WT_OBJECT,    // word is an object
    /*  2 */ WT_PRIMITIVE, // word is a primitive (eg +, return)
    /*  3 */ WT_ROUTINE,   // word is a subroutine
    /*  4 */ WT_ENUMS
  };
  
  static const char* const pWT[]
  {
    /*  0 */ "NONE",
    /*  1 */ "Object",
    /*  2 */ "PRIMITIVE",
    /*  3 */ "ROUTINE",
    /*  4 */ "ENUMS",
  };
  
  class Word : public Object
  {
    friend class Dictionary;
    
  private:
    s32         m_sName;       // the name of this word
    eWT         m_eWordType;   // type of word
    Dictionary* m_pDictionary; // which dictionary this word in
    Word*       m_pKind;       // kind of word
    bool        m_bImmediate;  // immediate execution during compile?
    i8          m_iDataBytes;  // number of data bytes
    Word*       m_pPrevWord;   // offset of previous word
    StepPtr     m_pStep;       // task step
    Cell*       m_pArg;        // task step argument
    // ..data..
    
   public:
    void* operator new(size_t, Dictionary* pDictionary);
    void  operator delete(void* pWordAddress);
    
    Word(const char* sName);
    ~Word(void);
    
    void  vAddr(Cell* pCell);
    
    char* pAllot(i8 iBytes);
    
    void* pAppend(i8 wVal);
    void  vAppend(i8 wVal);
    void* pAppend(Cell* pCell);
    void  vAppend(Cell* pCell);
    void* pAppend(Value* pValue);
    void  vAppend(Value* pValue);
    
    void* pAppend(StepPtr* ppStep);
    void  vAppend(StepPtr* ppStep);
    
    StepPtr* ppStep(void);

    Dictionary* pDictionary(void);
    
    Cell* pArg(void);
    
    StepPtr  pStep(void);
    void  vStep(StepPtr pStep);
    
    char* pBodyArea(void);
    void  vByteDump(i8 iSize);
    char* pDataEnd(void);
    i8    iDataBytes(void);
    void  vDump(void);
    void  vDump(const char* pTab);
    void  vEncode(const char* aWord);
    void  vEncode(const char* aWord, Cell* pCell);
    void  vEncode(const char* aWord, StepPtr* pCode);
    void  vEncode(const char* aWord, Value* pValue);
    void  vEncode(const char* aWord, Word* pWord);
    void  vEncode(StepPtr* pCode);
    
    Cell* pFirstCell(void);
    
    bool  bHasLocation(Cell* pCell);
    
    bool  bImmediate(void);
    bool  bIsName(s& sName);
    void  vImmediate(bool bImmediate);
    Word* pKind(void);
    void  vKind(Word* pKind);
    Cell* pLastCell(void);
    char* aName(void);
    s&    sName(void);
    void  vName(const char* sName);
    Word* pPrevWord(void);
    void  vPrevWord(Word* pPrevWord);
    void  vReset(void);
    i8    iSize(void);
    Value* pValue(void);
    void  vWordDump(i8 iSize);
    eWT   eWordType(void);
    void  vWordType(eWT eWordType);
    
    static Word* pToWord(void* pBody);
    
    Footer* pFooter(void);
  };
  
  Word* _pWord(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/