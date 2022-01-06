/*============================================================================*/
/* SP_IndexMetadata.hpp - Index Metadata Sprint Helper                        */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_IndexMetadata : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_IndexMetadata(void);
   ~SP_IndexMetadata(void) noexcept(false);
  
    Stz* sz(IndexMetadata* pIndexMeta);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
