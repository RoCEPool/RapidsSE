/*============================================================================*/
/* IndexColumnMetadata.hpp -- Metadata of Column                                   */
/*============================================================================*/
#pragma once

#include "Row.hpp"

namespace STORE
{
  // refer: C_TDEF_COLUMNDEFS
  class IndexColumnMetadata : public Row
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

    inline i8 iColumnNumber(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(4);
    }

    inline Stz* szColumnName(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return szColVal_ofs(5);
    }

    inline i8 iTypeEnum(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(6);
    }

    inline Stz* szColumnType(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return szColVal_ofs(7);
    }

    inline i8 iColumnLength(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(8);
    }

    inline i8 iColumnScale(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(9);
    }

    inline bool bIsNullable(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(10);
    }

    inline bool bIsPartitionKey(void)
    {
      _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
      return iColVal_int(11);
    }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
