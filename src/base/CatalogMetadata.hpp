/*============================================================================*/
/* CatalogMetadata.hpp -- Metadata of Catalog                                 */
/*============================================================================*/
#pragma once

#include "Row.hpp"

namespace STORE
{
  class CatalogMetadata : public Row
  {
  public:
    inline Stz* szCatalogName(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return szColVal_ofs(0);
    }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
