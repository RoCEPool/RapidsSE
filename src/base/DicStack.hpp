/*============================================================================*/
/* DicStack.hpp -- Dictionary Stack                                           */
/*============================================================================*/
#pragma once

namespace STORE { class StreamDef; }

namespace BASE
{
  static const i8 C_DIC_STACKDEPTH = 16;

  class DicStack : public Object
  {
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

    Dic*  pCatalogDic(void);
    void  vContentDump(void);
    void  vCopy(DicStack* pDicStackSrc);
    void  vDiscard(void);
    void  vDiscardAll(void);
    Dic*  pFindDic(Stz* szDicNm);
    Word* pFindWord(Stz* szWordNm);
    Word* pFindInCurrent(Stz* szWordNm);
    Index* pIndex(void);
    i8    nItems(void);
    Word* pLocateRoutine(CodeCell* aCell);
    Word* pLocateStepWord(fpSTEPFN fpStepFn);
    Dic*  pPick(i8 i_Dic);
    void  vPop(void);
    void  vPrint(void);
    void  vPush(Dic* pDic);
    bool  bPush(Stz* szCatalogNm);
    bool  bPush(Stz* szCatalogNm, Stz* szSchemaNm);
    bool  bPush(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm);
    bool  bPushNamed(Stz* szDicNm);
    bool  bPush_boray(void);
    bool  bPush_catalogs(void);
    bool  bPush_indexes(void);
    bool  bPush_public(void);
    bool  bPush_rapidsse(void);
    bool  bPush_schemas(void);
    bool  bPush_tables(void);
    bool  bPush_tenants(void);
    Word* pRemoveWord(Stz* szWordNm);
    Dic*  pSchemaDic(void);
    StreamDef* pStreamDef(void);
    Table* pTable(void);
    Dic*  pTableDic(void);
    Dic*  pTenantDic(void);
    bool  bTest(Stz* szCatalogNm);
    bool  bTest(Stz* szCatalogNm, Stz* szSchemaNm);
    bool  bTest(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm);
    bool  bTest(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm, Stz* szIndexNm);
    Dic*  pTop(void);
    void  vToPathStr(Stz* szPath);

    Footer* aFooter(void);
  };
  
  DicStack* _pDicStack(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
