/*============================================================================*/
/* SP_PartitionKeyMetadata.hpp - Partition Key Metadata Sprint Helper         */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_PartitionKeyMetadata : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_PartitionKeyMetadata(void);
   ~SP_PartitionKeyMetadata(void) noexcept(false);
  
    Stz* sz(PartitionKeyMetadata* pPartitionKeyMetadata);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
