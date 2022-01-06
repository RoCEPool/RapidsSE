/*============================================================================*/
/* SP_IndexColumnMetadata.hpp - Column Metadata Sprint Helper                      */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_IndexColumnMetadata : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_IndexColumnMetadata(void);
   ~SP_IndexColumnMetadata(void) noexcept(false);
  
    Stz* sz(IndexColumnMetadata* pIndexColumnMeta);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
