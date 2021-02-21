/*============================================================================*/
/* Metadata.hpp -- Metadata                                                   */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Metadata : public Object
  {
  private:
      
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Metadata(void);
   ~Metadata(void) noexcept(false);
  
    Dic* pCurrCatalog(void);
    Stz* szCurrCatalog(void);
  
    Dic* pCurrSchema(void);
    Stz* szCurrSchema(void);
  
    Dic* pCurrTable(void);
    Stz* szCurrTable(void);

    Dic* pCurrTenant(void);
    Stz* szCurrTenant(void);

    void vListContents(i8 i_Depth, Dic* pDic);

    RowBuffer* pLookupCatalog(Stz* szCatalogName);

    RowBuffer* pLookupSchema(Stz* szCatalogName,
                             Stz* szSchemaName);

    RowBuffer* pLookupTable(Stz* szCatalogName,
                            Stz* szSchemaName,
                            Stz* szTableName);
  
    bool bTest(Stz* szCatalogNm);
    bool bTest(Stz* szCatalogNm, Stz* szSchemaNm);
    bool bTest(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm);

  private:
    Footer* aFooter(void);
  };

Metadata* _pMetadata(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
