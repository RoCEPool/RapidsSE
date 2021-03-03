/*============================================================================*/
/* SchemaMetadata.hpp -- Metadata of Schema                                   */
/*============================================================================*/
#pragma once

#include "Row.hpp"

namespace STORE
{
  class SchemaMetadata : public Row
  {
  public:
    inline Stz* szCatalogName(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return szColVal_ofs(0);
    }
  
    inline Stz* szSchemaName(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return szColVal_ofs(1);
    }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
