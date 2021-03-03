/*============================================================================*/
/* Metadata.hpp -- Metadata                                                   */
/*============================================================================*/
#pragma once

namespace STORE { class InfoSet; }

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
  
    void vSearchContents(InfoSet* pInfoSet,
                         Stz* szCatalogName,
                         Stz* szSchemaName,
                         Stz* szTableName,
                         Dic* pDic = nullptr,
                         Dic* pCatalog = nullptr,
                         Dic* pSchema = nullptr);

  void vSearchContents(InfoSet* pInfoSet,
                       Dic* pCatalog,
                       Dic* pSchema,
                       Table* pTable);

    InfoSet* pColumnDefs(Stz* szCatalogName,
                         Stz* szSchemaName,
                         Stz* szTableName,
                         Stz* szColumnName);
  
    InfoSet* pLookupCatalog(Stz* szCatalogName);

    InfoSet* pLookupSchema(Stz* szCatalogName,
                             Stz* szSchemaName);

    InfoSet* pLookupTable(Stz* szCatalogName,
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
