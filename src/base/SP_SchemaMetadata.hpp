/*============================================================================*/
/* SP_SchemaMetadata.hpp - Schema Metadata Sprint Helper                      */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_SchemaMetadata : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_SchemaMetadata(void);
   ~SP_SchemaMetadata(void) noexcept(false);
  
    Stz* sz(SchemaMetadata* pSchemaMeta);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
