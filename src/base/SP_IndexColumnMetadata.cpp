/*============================================================================*/
/* SP_IndexColumnMetadata.cpp - Column Metadata Sprint Helper                 */
/*============================================================================*/

#include "BASE.h"

#include "IndexColumnMetadata.hpp"

#include "SP_IndexColumnMetadata.hpp"

#include "eCX.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_IndexColumnMetadata::SP_IndexColumnMetadata(void)
: SP(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_IndexColumnMetadata::~SP_IndexColumnMetadata(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_IndexColumnMetadata::sz(IndexColumnMetadata* pIndexColumnMeta)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
          
  StrLib::vSprint(m_caResult
                  
    ,"%s/%s/%s/%s : [%02lld] %16s : type %2lld %7s length %5lld scale %2lld %s %s"_Stz
                  
    ,pIndexColumnMeta->szCatalogName()
    ,pIndexColumnMeta->szSchemaName()
    ,pIndexColumnMeta->szTableName()
    ,pIndexColumnMeta->szIndexName()

    ,pIndexColumnMeta->iColumnNumber()
    ,pIndexColumnMeta->szColumnName()
                  
    ,pIndexColumnMeta->iTypeEnum()
    ,pIndexColumnMeta->szColumnType()
    ,pIndexColumnMeta->iColumnLength()
    ,pIndexColumnMeta->iColumnScale()

    ,pIndexColumnMeta->bIsNullable()     ? "nullable"_Stz : "cantnull"_Stz
    ,pIndexColumnMeta->bIsPartitionKey() ? "partnKey"_Stz : ""_Stz
                  
    );

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_IndexColumnMetadata)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
