/*============================================================================*/
/* IndexMetadata.hpp -- Metadata of Index                                     */
/*============================================================================*/
#pragma once

#include "Row.hpp"

namespace STORE
{
  // refer: C_TDEF_LOOKUPINDEX
  class IndexMetadata : public Row
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

    inline Stz* szTableName(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return szColVal_ofs(2);
    }

    inline Stz* szIndexName(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return szColVal_ofs(3);
    }

    inline bool bIsUnique(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(4);
    }

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
