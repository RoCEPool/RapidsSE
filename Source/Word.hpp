/*============================================================================*/
/* Word.hpp -- Word in Dictionary                                             */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  enum eWT // word type
  {
    /*  0 */ WT_NONE=0,     // none
    /*  1 */ WT_OBJECT,     // word is an object
    /*  2 */ WT_PRIMITIVE,  // word is a primitive (eg +, return)
    /*  3 */ WT_ROUTINE,    // word is a subroutine
    /*  4 */ WT_ENUMS
  };
  
  static const char* const pWT[]
  {
    /*  0 */ "NONE",
    /*  1 */ "OBJECT",
    /*  2 */ "PRIMITIVE",
    /*  3 */ "ROUTINE",
    /*  4 */ "ENUMS",
  };
  
  class Word : public Object
  {
    friend class Dic;
    
  private:
    s32      m_sName;       // the name of this word
    eWT      m_eWordType;   // type of word
    Dic*     m_pDic;        // which dictionary this word in
    Word*    m_pKind;       // kind of word
    bool     m_bImmediate;  // immediate execution during compile?
    Word*    m_pPrevWord;   // previous word in dictionary
    i8       m_iDataBytes;  // number of data bytes
    TaskStep m_pDoesStep;   // "entry step" for this word
    Cell*    m_pDoesArg;    // "entry step" argument
    // ..data.. (being an array of cells)
    
   public:
    void* operator new(size_t, Dic* pDic);
    void  operator delete(void* pWordAddress);
    
    Word(const char* sName);
    ~Word(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    char* pAllot(i8 iBytes);
    void* pAppend(Cell* pCell);
    void  vAppend(Cell* pCell);
    void* pAppend(i8 iInt);
    void  vAppend(i8 iInt);
    void* pAppend(TaskStep* ppStep);
    void  vAppend(TaskStep* ppStep);
    void* pAppend(Value* pValue);
    void  vAppend(Value* pValue);
    char* pDataArea(void);
    i8    iDataBytes(void);
    char* pDataEnd(void);
    Dic*  pDic(void);
    void  vDoesArg(Cell* pCell);
    Cell* pDoesArg(void);
    void  vDoesStep(TaskStep pStep);
    TaskStep pDoesStep(void);
    TaskStep* ppDoesStep(void);
    void  vDump(void);
    void  vDump(const char* pTab);
    void  vEncode(const char* aWord);
    void  vEncode(const char* aWord, Cell* pCell);
    void  vEncode(const char* aWord, TaskStep* ppCalledDoes);
    void  vEncode(const char* aWord, Value* pValue);
    void  vEncode(const char* aWord, Word* pWord);
    void  vEncode(TaskStep* pCode);
    Cell* pFirstCell(void);
    bool  bHasCell(Cell* pCell);
    bool  bImmediate(void);
    bool  bIsName(s& sName);
    void  vImmediate(bool bImmediate);
    Word* pKind(void);
    void  vKind(Word* pKind);
    Cell* pLastCell(void);
    s&    sName(void);
    Word* pPrevWord(void);
    void  vPrevWord(Word* pPrevWord);
    void  vReset(void);
    i8    iSize(void);
    eWT   eWordType(void);
    void  vWordType(eWT eWordType);
    
    static Word* pToWord(void* pData);
    
    Footer* pFooter(void);
  };
  
  Word* _pWord(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
