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
  
    Stz* szCurrCatalog(void);
    Stz* szCurrSchema(void);
    Stz* szCurrTable(void);

    RowBuffer* pLookupCatalog(Stz* szCatalogName);

    RowBuffer* pLookupSchema(Stz* szCatalogName,
                             Stz* szSchemaName);

    RowBuffer* pLookupTable(Stz* szCatalogName,
                            Stz* szSchemaName,
                            Stz* szTableName);

  private:
    Footer* aFooter(void);
  };

Metadata* _pMetadata(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
