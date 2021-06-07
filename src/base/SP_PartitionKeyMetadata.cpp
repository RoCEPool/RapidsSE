/*============================================================================*/
/* SP_PartitionKeyMetadata.cpp - Partition Key Metadata Sprint Helper         */
/*============================================================================*/

#include "BASE.h"

#include "PartitionKeyMetadata.hpp"

#include "SP_PartitionKeyMetadata.hpp"

#include "eCX.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_PartitionKeyMetadata::SP_PartitionKeyMetadata(void)
: SP(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_PartitionKeyMetadata::~SP_PartitionKeyMetadata(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint catalog and return Stz* to result                                   */
/*----------------------------------------------------------------------------*/
Stz* SP_PartitionKeyMetadata::sz(PartitionKeyMetadata* pPartitionKeyMetadata)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
          
  StrLib::vSprint(m_caResult
                  
    ,"%s %s %s %lld %s"_Stz
                  
    ,pPartitionKeyMetadata->szCatalogName()
    ,pPartitionKeyMetadata->szSchemaName()
    ,pPartitionKeyMetadata->szTableName()

    ,pPartitionKeyMetadata->iColumnNumber()
    ,pPartitionKeyMetadata->szColumnName()
    );

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_PartitionKeyMetadata)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
