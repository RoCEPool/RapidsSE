/*============================================================================*/
/* SeApi.hpp -- Se Reentrant API                                              */
/*============================================================================*/
#pragma once

namespace STORE { class InfoSet; }

namespace STORE
{
  class SeApi : public Object
  {
  private:
  
    s48K m_sSequence;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void* pSeApi);
    
    SeApi(void);
   ~SeApi(void) noexcept(false);
  
    static Stz* szCurrentFolderNm(String* szFolderNm);
    static Stz* szCurrentFolderPath(String* szFolderPath);
    static bool bIsKnownTableNm(Stz* szTableNm);
    static bool bIsKnownTableColumnNm(Stz* szTableNm,Stz* szColumnNm);
    static void vPrintAllColumnsOfTable(Stz* szTableNm);
    static void vPrintAllTablesInCurrentFolder(void);

    SeApi& operator |(SeApi* pArg);

    SeApi* pAdd(Stz* szOpNm, Stz* szOpArg);
    SeApi* pEncode(Stz* szAction);
    SeApi* pEncode(Stz* szAction, Stz* szArgument);

    Stz*   szGet(void);

    Dic* pCurrCatalog(void);
    Stz* szCurrCatalog(void);
    Dic* pCurrSchema(void);
    Stz* szCurrSchema(void);
    Dic* pCurrTable(void);
    Stz* szCurrTable(void);
    Dic* pCurrTenant(void);
    Stz* szCurrTenant(void);
  
    void vPrintMetadata(void);

		RowBuffer* pSearchCatalogs(Stz* szCatalogName);

		RowBuffer* pSearchColumns(Stz* szCatalogName,
                            Stz* szSchemaName,
                            Stz* szTableName,
                            Stz* szColumnName);

		RowBuffer* pSearchIndexes(Stz* szCatalogName,
                           Stz* szSchemaName,
                           Stz* szTableName,
                           Stz* szIndexName);

		RowBuffer* pSearchIndexColumns(Stz* szCatalogName,
                            Stz* szSchemaName,
                            Stz* szTableName,
                            Stz* szIndexName,
                            Stz* szColumnName);

		RowBuffer* pSearchSchemas(Stz* szCatalogName,
                            Stz* szSchemaName);

		RowBuffer* pSearchTables(Stz* szCatalogName,
                           Stz* szSchemaName,
                           Stz* szTableName);

		RowBuffer* pColumnDefs(Stz* szCatalogName,
                         Stz* szSchemaName,
                         Stz* szTableName,
                         Stz* szColumnName);

		RowBuffer* pIndexColumnDefs(Stz* szCatalogName,
                           Stz* szSchemaName,
                           Stz* szTableName,
                           Stz* szIndexName,
                           Stz* szColumnName);


		RowBuffer* pLookupCatalog(Stz* szCatalogName);

		RowBuffer* pLookupSchema(Stz* szCatalogName,
                           Stz* szSchemaName);

		RowBuffer* pLookupTable(Stz* szCatalogName,
                          Stz* szSchemaName,
                          Stz* szTableName);

		RowBuffer* pLookupIndex(Stz* szCatalogName,
                          Stz* szSchemaName,
                          Stz* szTableName,
                          Stz* szIndexName);

		RowBuffer* pPartitionKey(Stz* szCatalogName,
                           Stz* szSchemaName,
                           Stz* szTableName);
  
    bool bTest(Stz* szCatalogNm);
    bool bTest(Stz* szCatalogNm, Stz* szSchemaNm);
    bool bTest(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm);

  private:
    
    Footer* aFooter(void);
  };
  
  SeApi* _pSeApi(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
