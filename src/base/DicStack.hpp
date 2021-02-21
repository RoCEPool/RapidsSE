/*============================================================================*/
/* DicStack.hpp -- Dictionary TypedStack                                           */
/*============================================================================*/
#pragma once

namespace BASE
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
    Dic*  pFindDic(Stz* szDicNm);
    Word* pFindWord(Stz* szWordNm);
    Word* pFindInCurrent(Stz* szWordNm);
    Word* pLocateRoutine(CodeCell* aCell);
    Word* pLocateStepWord(fpSTEPFN fpStepFn);
    Dic*  pPick(i8 i_Dic);
    void  vPop(void);
    void  vPrint(void);
    void  vPush(Dic* pDic);
    void  vPushNamed(Stz* szDicNm);
    void  vPush_boray(void);
    void  vPush_catalogs(void);
    void  vPush_public(void);
    void  vPush_rapidsse(void);
    void  vPush_schemas(void);
    void  vPush_tenants(void);
    Word* pRemoveWord(Stz* szWordNm);
    void  vReset(void);
    bool  bTest(Stz* szCatalogNm);
    bool  bTest(Stz* szCatalogNm, Stz* szSchemaNm);
    bool  bTest(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm);
    Dic*  pTop(void);
    void  vToPathStr(Stz* szPath);

    Footer* aFooter(void);
  };
  
  DicStack* _pDicStack(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
