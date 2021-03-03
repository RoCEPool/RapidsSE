/*============================================================================*/
/* SP_ColumnMetadata.cpp - Column Metadata Sprint Helper                      */
/*============================================================================*/

#include "BASE.h"

#include "ColumnMetadata.hpp"

#include "SP_ColumnMetadata.hpp"

#include "eCX.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_ColumnMetadata::SP_ColumnMetadata(void)
: SP(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_ColumnMetadata::~SP_ColumnMetadata(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_ColumnMetadata::sz(ColumnMetadata* pColumnMeta)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
          
  StrLib::vSprint(m_caResult
                  
    ,"%s/%s/%s : [%02lld] %16s : type %7s length %5lld scale %2lld %s %s"_Stz
                  
    ,pColumnMeta->szCatalogName()
    ,pColumnMeta->szSchemaName()
    ,pColumnMeta->szTableName()

    ,pColumnMeta->iColumnNumber()
    ,pColumnMeta->szColumnName()
    ,pColumnMeta->szColumnType()
                  
    ,pColumnMeta->iColumnLength()
    ,pColumnMeta->iColumnScale()

    ,pColumnMeta->bIsNullable()     ? "nullable"_Stz : "cantnull"_Stz
    ,pColumnMeta->bIsPartitionKey() ? "partnKey"_Stz : ""_Stz
                  
    );

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_ColumnMetadata)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
