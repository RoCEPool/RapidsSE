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
  
    void vSearch(RowBuffer* pInfpRowBufferoSet,
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

    void vSearchCatalogs(RowBuffer* pRowBuffer,
                         Dic* pCatalog);
  
    void vSearchColumns(RowBuffer* pRowBuffer,
                        Dic* pCatalog,
                        Dic* pSchema,
                        Table* pTable,
                        Stz* szColumnName);

    void vSearchIndexColumns(RowBuffer* pRowBuffer,
                        Dic* pCatalog,
                        Dic* pSchema,
                        Table* pTable,
                        Index* pIndex,
                        Stz* szColumnName);

    void vSearchIndexes(RowBuffer* pRowBuffer,
                        Dic* pCatalog,
                        Dic* pSchema,
                        Table* pTable,
                        Stz* szIndexName);

    void vSearchSchemas(RowBuffer* pRowBuffer,
                        Dic* pCatalog,
                        Dic* pSchema);
  
    void vSearchTables(RowBuffer* pRowBuffer,
                       Dic* pCatalog,
                       Dic* pSchema,
                       Table* pTable);

		RowBuffer* pColumnDefs(Stz* szCatalogName,
                         Stz* szSchemaName,
                         Stz* szTableName,
                         Stz* szColumnName);

		RowBuffer* pIndexColumnDefs(Stz* szCatalogName,
                              Stz* szSchemaName,
                              Stz* szTableName,
                              Stz* szIndexName,
                              Stz* szColumnName);

		RowBuffer* pPartitionKey(Stz* szCatalogName,
                         Stz* szSchemaName,
                         Stz* szTableName);

		RowBuffer* pLookupCatalog(Stz* szCatalogName);

		RowBuffer* pLookupIndex(Stz* szCatalogName,
                          Stz* szSchemaName,
                          Stz* szTableName,
                          Stz* szIndexName);

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
