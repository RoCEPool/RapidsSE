/*============================================================================*/
/* SP_ColumnMetadata.hpp - Column Metadata Sprint Helper                      */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_ColumnMetadata : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_ColumnMetadata(void);
   ~SP_ColumnMetadata(void) noexcept(false);
  
    Stz* sz(ColumnMetadata* pColumnMeta);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
