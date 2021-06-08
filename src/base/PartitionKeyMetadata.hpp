/*============================================================================*/
/* PartitionKeyMetadata.hpp -- Metadata of Partition Key                      */
/*============================================================================*/
#pragma once

#include "Row.hpp"

namespace STORE
{
  // refer: C_TDEF_PARTITIONKEY
  class PartitionKeyMetadata : public Row
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

    inline i8 iColumnNumber(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(3);
    }

    inline Stz* szColumnName(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return szColVal_ofs(4);
    }

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
