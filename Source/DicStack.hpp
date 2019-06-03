/*============================================================================*/
/* DicStack.hpp -- Dictionary Stack                                           */
/*============================================================================*/
#pragma once

namespace Forth
{
  class DicStack : public Object
  {
  private:
    
  private:
    i8    m_iTop;
    Dic*  m_pDic[C_DIC_PATHDEPTH];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    DicStack(void);
    ~DicStack(void) noexcept(false);
    
    static DicStack* _pDicStack(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    i8    iCount(void);
    void  vContentDump(void);
    void  vDiscard(void);
    ForthWord* pFindWord(s32& sName);
    ForthWord* pFindWord(Stz* aName);
    ForthWord* pFindInCurrent(Stz* aName);
    ForthWord* pLocateWord(Cell* aHasCell);
    ForthWord* pLocateWord(StepFp fpHasStep);
    ForthWord* pRemoveWord(s32& sName);
    Dic*  pPick(i8 iDic);
    void  vPop(void);
    void  vPush(Dic* pDic);
    void  vSetStack(DicPath* pDicPath);
    Dic*  pTop(void);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
