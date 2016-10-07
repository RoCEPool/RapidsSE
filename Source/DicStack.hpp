/*============================================================================*/
/* DicStack.hpp - dictionary stack                                            */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  class DicStack : public Object
  {
  private:
    static const i8 C_DICTIONARY_STACK = 32;
    
  private:
    i8    m_iTop;
    Dic*  m_pDic[C_DICTIONARY_STACK];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    DicStack(void);
    ~DicStack(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);

    i8   iCount(void);

    void vDiscard(void);
    void vDump(void);
    Word* pFind(s& sName);
    Word* pFind(const char* aName);
    Word* pFindInCurrent(const char* aName);
    Word* pFindStep(TaskStep pStep);
    bool  bForget(s& sName);
    Word* pLocate(Cell* pCell);
    Dic* pPick(i8 iDic);
    void vPush(Dic* pDic);
    Dic* pTop(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
