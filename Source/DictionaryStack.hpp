/*============================================================================*/
/* DictionaryStack.hpp - dictionary stack                                     */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  class DictionaryStack
  {
  private:
    static const i8 C_DICTIONARY_STACK = 32;
    
  private:
    i8           m_iTop;
    Dictionary*  m_pDictionary[C_DICTIONARY_STACK];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    DictionaryStack(void);
    ~DictionaryStack(void);
    
    i8   iCount(void);

    void vDiscard(void);
    void vDump(void);
    Word* pFind(s& sName);
    Word* pFind(const char* aName);
    Word* pFindFunc(Tasks::pFUN pFunc);
    bool  bForget(s& sName);
    Word* pLocate(Cell* pCell);
    Dictionary* pPick(i8 iDic);
    void vPush(Dictionary* pDictionary);
    Dictionary* pTop(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
