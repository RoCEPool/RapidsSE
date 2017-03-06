/*============================================================================*/
/* DicPath.hpp - dictionary stack                                            */
/*============================================================================*/
#pragma once

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
    
    static DicPath* _pDicStack(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, const char* aTab);

    i8   iCount(void);

    void vContentDump(void);
    void vDiscard(void);
    Word* pFindWord(StrVbl& sName);
    Word* pFindWord(const char* aName);
    Word* pFindInCurrent(const char* aName);
    Word* pFindStep(TaskStep pStep);
    bool  bForget(StrVbl& sName);
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
