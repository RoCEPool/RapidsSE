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
  
    void vSearch(Pds* pInfoPdsSet,
                 StreamDef* pLookupStreamDef,
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

    void vSearchCatalogs(Pds* pPds,
                         StreamDef* pLookupStreamDef,
                         Dic* pCatalog);
  
    void vSearchColumns(Pds* pPds,
                        StreamDef* pLookupStreamDef,
                        Dic* pCatalog,
                        Dic* pSchema,
                        Table* pTable,
                        Stz* szColumnName);

    void vSearchIndexColumns(Pds* pPds,
                             StreamDef* pLookupStreamDef,
                             Dic* pCatalog,
                             Dic* pSchema,
                             Table* pTable,
                             Index* pIndex,
                             Stz* szColumnName);

    void vSearchIndexes(Pds* pPds,
                        StreamDef* pLookupStreamDef,
                        Dic* pCatalog,
                        Dic* pSchema,
                        Table* pTable,
                        Stz* szIndexName);

    void vSearchSchemas(Pds* pPds,
                        StreamDef* pLookupStreamDef,
                        Dic* pCatalog,
                        Dic* pSchema);
  
    void vSearchTables(Pds* pPds,
                       StreamDef* pLookupStreamDef,
                       Dic* pCatalog,
                       Dic* pSchema,
                       Table* pTable);

		Pds* pColumnDefs(Stz* szCatalogName,
                     Stz* szSchemaName,
                     Stz* szTableName,
                     Stz* szColumnName);

		Pds* pIndexColumnDefs(Stz* szCatalogName,
                          Stz* szSchemaName,
                          Stz* szTableName,
                          Stz* szIndexName,
                          Stz* szColumnName);

		Pds* pPartitionKey(Stz* szCatalogName,
                       Stz* szSchemaName,
                       Stz* szTableName);

		Pds* pLookupCatalog(Stz* szCatalogName);

		Pds* pLookupIndex(Stz* szCatalogName,
                      Stz* szSchemaName,
                      Stz* szTableName,
                      Stz* szIndexName);

		Pds* pLookupSchema(Stz* szCatalogName,
                       Stz* szSchemaName);

		Pds* pLookupTable(Stz* szCatalogName,
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
