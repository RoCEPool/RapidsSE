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
  
    void vSearch(InfoSet* pInfoSet,
                 i8 iScope,
                 Stz* szCatalogName,
                 Stz* szSchemaName,
                 Stz* szTableName,
                 Stz* szColumnName,
                 Stz* szIndexName,
                 Stz* szIndexColumnName,
                 Dic* pDic = nullptr,
                 Dic* pCatalog = nullptr,
                 Dic* pSchema = nullptr);

    void vSearchCatalogs(InfoSet* pInfoSet,
                         Dic* pCatalog);
  
    void vSearchColumns(InfoSet* pInfoSet,
                        Dic* pCatalog,
                        Dic* pSchema,
                        Table* pTable,
                        Stz* szColumnName);

    void vSearchIndexColumns(InfoSet* pInfoSet,
                        Dic* pCatalog,
                        Dic* pSchema,
                        Table* pTable,
                        Index* pIndex,
                        Stz* szColumnName);

    void vSearchIndexes(InfoSet* pInfoSet,
                        Dic* pCatalog,
                        Dic* pSchema,
                        Table* pTable,
                        Stz* szIndexName);

    void vSearchSchemas(InfoSet* pInfoSet,
                        Dic* pCatalog,
                        Dic* pSchema);
  
    void vSearchTables(InfoSet* pInfoSet,
                       Dic* pCatalog,
                       Dic* pSchema,
                       Table* pTable);
  
    InfoSet* pColumnDefs(Stz* szCatalogName,
                         Stz* szSchemaName,
                         Stz* szTableName,
                         Stz* szColumnName);

    InfoSet* pIndexColumnDefs(Stz* szCatalogName,
                              Stz* szSchemaName,
                              Stz* szTableName,
                              Stz* szIndexName,
                              Stz* szColumnName);

    InfoSet* pPartitionKey(Stz* szCatalogName,
                         Stz* szSchemaName,
                         Stz* szTableName);

    InfoSet* pLookupCatalog(Stz* szCatalogName);

    InfoSet* pLookupIndex(Stz* szCatalogName,
                          Stz* szSchemaName,
                          Stz* szTableName,
                          Stz* szIndexName);

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
