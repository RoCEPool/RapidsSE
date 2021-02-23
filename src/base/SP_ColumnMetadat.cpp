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
    ,"%16s : type %s length %5lld scale %2lld %s %s"_Stz
                  
    //,(i8)pColMeta->m_uNumber
    ,pColMeta->szColumnName()
    ,pColMeta->szColumnType()
                  
    ,pColMeta->iColumnLength()
    ,pColMeta->iColumnScale()
    ,pColMeta->szColumnType()

    ,pColMeta->bIsNullable()     ? "nullable"_Stz : "not null"_Stz
    ,pColMeta->bIsPartitionKey() ? "ptnKey"_Stz   : ""_Stz
                  
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
