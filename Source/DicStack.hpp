/*============================================================================*/
/* DicStack.hpp -- Dictionary Stack                                           */
/*============================================================================*/
#pragma once

namespace SenVM
{
  static const i8 C_DIC_STACKDEPTH = 16;

  class DicStack : public Object
  {
  private:
    
  private:
    i8    m_iTop;
    Dic*  m_pDic[C_DIC_STACKDEPTH];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    DicStack(void);
   ~DicStack(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* szTab);

    i8    iCount(void);
    void  vContentDump(void);
    void  vCopy(DicStack* pDicStackSrc);
    void  vDiscard(void);
    Word* pFindWord(Stz* szWordNm);
    Word* pFindInCurrent(Stz* szWordNm);
    Word* pLocateRoutineWord(Cell* aCell);
    Word* pLocateStepWord(StepFp fpStep);
    Word* pRemoveWord(Stz* szWordNm);
    Dic*  pPick(i8 iDic);
    void  vPop(void);
    void  vPush(Dic* pDic);
    void  vReset(void);
    Dic*  pTop(void);
    void  vToPathStr(Stz* szPath);

    Footer* aFooter(void);
  };
  
  DicStack* _pDicStack(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
