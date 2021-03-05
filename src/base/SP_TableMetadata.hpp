/*============================================================================*/
/* SP_TableMetadata.hpp - Table Metadata Sprint Helper                        */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_TableMetadata : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_TableMetadata(void);
   ~SP_TableMetadata(void) noexcept(false);
  
    Stz* sz(TableMetadata* pTableMeta);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
