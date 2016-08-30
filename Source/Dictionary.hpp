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
    s32   m_sName;        // name of this dictionary
    bool  m_bFrozen;      // no more words can be added?
    Word* m_pCurrentWord; // last word
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Dictionary(void);
    ~Dictionary(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);

    void  vAddConstant(const char* aName, i8 iVal);
    void  vAddConstant(const char* aName, const char* aValue);
    Word* pAllocWord(i8 iBytes);
    Word* pCurrentWord(void);
    Word* pFind(const char* aName);
    Word* pFindFunc(Tasks::pFUN pFunc);
    bool  bForget(s& sName);
    void  vFreeWord(Word* pWord);
    bool  bFrozen(void);
    void  vFrozen(bool bFrozen);
    bool  bHasWords(void);
    void  vInit(void);
    bool  bIsName(s& sName);
    Word* pLocate(Cell* pCell);
    s&    sName(void);
    void  vName(const char* aName);
  };
  
  Dictionary* _pDic(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
