/*============================================================================*/
/* Dictionary.hpp - symbol table of words                                     */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  class Word;
  
  class Dictionary : public Object
  
  {
  private:
    s32   m_sName;
    Word* m_pCurrentWord;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Dictionary(void);
    ~Dictionary(void);
    
    void  vAddConstant(const char* aName, i8 iVal);
    Word* pAllocWord(i8 iBytes);
    Word* pCurrentWord(void);
    void  vDump(void);
    Word* pFind(const char* aName);
    Word* pFindFunc(Tasking::pFUN pFunc);
    bool  bForget(s& sName);
    void  vFreeWord(Word* pWord);
    bool  bHasWords(void);
    void  vInit(void);
    bool  bIsName(s& sName);
    Word* pLocate(Cell* pCell);
    s&    sName(void);
    void  vName(const char* aName);
    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
