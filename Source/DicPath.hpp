/*============================================================================*/
/* DicPath.hpp - dictionary stack                                            */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

namespace Dictionaries
{
  class DicPath : public Object
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
    
    DicPath(void);
    ~DicPath(void);
    
    static DicPath* _pDicStack(void* pAddr);
    static void vDump(eCN eClass, Object* pObject, const char* pTab);

    i8   iCount(void);

    void vContentDump(void);
    void vDiscard(void);
    Word* pFindWord(StrVal& sName);
    Word* pFindWord(const char* aName);
    Word* pFindInCurrent(const char* aName);
    Word* pFindStep(TaskStep pStep);
    bool  bForget(StrVal& sName);
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
