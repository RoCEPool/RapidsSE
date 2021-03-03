/*============================================================================*/
/* SP_CatalogMetadata.hpp - Column Metadata Sprint Helper                      */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_CatalogMetadata : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_CatalogMetadata(void);
   ~SP_CatalogMetadata(void) noexcept(false);
  
    Stz* sz(CatalogMetadata* pCatalogMeta);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
