/*============================================================================*/
/* Metadata.cpp -- Metadata                                                   */
/*============================================================================*/

#include "BASE.h"

#include "Metadata.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Metadata,CN_METADATA)
__aNewFunction(Metadata,CN_METADATA)
__vDeleteFunction(Metadata,CN_METADATA)
__aFooterFunction(Metadata)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Metadata::Metadata(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Metadata::~Metadata(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get name of current catalog                                                */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get name of current schema                                                 */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get name of current table                                                  */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* determine whether a catalog exists and return its full name                */
/*----------------------------------------------------------------------------*/
RowBuffer* Metadata::pLookupCatalog(Stz* szCatalogName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* determine whether a schema exists and return its full name                 */
/*----------------------------------------------------------------------------*/
RowBuffer* Metadata::pLookupSchema(Stz* szCatalogName,
                                   Stz* szSchemaName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* determine whether a table exists and return its full name                  */
/*----------------------------------------------------------------------------*/
RowBuffer* Metadata::pLookupTable(Stz* szCatalogName,
                                  Stz* szSchemaName,
                                  Stz* szTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
