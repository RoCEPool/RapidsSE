/*============================================================================*/
/* ColumnMetadata.hpp -- Metadata of Column                                   */
/*============================================================================*/
#pragma once

#include "Row.hpp"

namespace STORE
{
  class ColumnMetadata : public Row
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

    inline Stz* szColumnName(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return szColVal_ofs(3);
    }

    inline Stz* szColumnType(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return szColVal_ofs(4);
    }

    inline i8 iColumnLength(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(5);
    }

    inline i8 iColumnScale(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(6);
    }

    inline bool bIsNullable(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(7);
    }

    inline bool bIsPartitionKey(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(8);
    }

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
