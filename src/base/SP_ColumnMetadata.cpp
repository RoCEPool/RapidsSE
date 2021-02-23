/*============================================================================*/
/* SP_ColumnMetadata.cpp - Column Pix Sprint Helper                                   */
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
Stz* SP_ColumnMetadata::sz(ColumnMetadata* pColMeta)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
          
  StrLib::vSprint(m_caResult
                  
//  ,"%02lld %16s : kind %s type %s datum %s "
    ,"%s/%s/%s : [%02lld] %16s : type %7s length %5lld scale %2lld %s %s"_Stz
                  
    ,pColMeta->szCatalogName()
    ,pColMeta->szSchemaName()
    ,pColMeta->szTableName()

    ,pColMeta->iColumnNumber()
    ,pColMeta->szColumnName()
    ,pColMeta->szColumnType()
                  
    ,pColMeta->iColumnLength()
    ,pColMeta->iColumnScale()

    ,pColMeta->bIsNullable()     ? "nullable"_Stz : "cantnull"_Stz
    ,pColMeta->bIsPartitionKey() ? "partnKey"_Stz : ""_Stz
                  
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
