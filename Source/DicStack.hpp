/*============================================================================*/
/* DicStack.hpp - dictionary stack                                            */
/*============================================================================*/
#pragma once

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
    
    static DicStack* _pDicStack(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    i8    iCount(void);
    void  vContentDump(void);
    void  vDiscard(void);
    Word* pFindWord(StrVal& sName);
    Word* pFindWord(Stz* aName);
    Word* pFindInCurrent(Stz* aName);
    Word* pFindStep(TaskStep pStep);
    Word* pRemoveWord(StrVal& sName);
    Word* pLocate(Cell* pCell);
    Dic*  pPick(i8 iDic);
    void  vPop(void);
    void  vPush(Dic* pDic);
    Dic*  pTop(void);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
