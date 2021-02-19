/*============================================================================*/
/* Metadata.cpp -- Metadata                                                   */
/*============================================================================*/

#include "BASE.h"

#include "Metadata.hpp"
#include "Catalog.hpp"
#include "Schema.hpp"

#include "BucketMap.hpp"
#include "RowCoder.hpp"
#include "HeadLine.hpp"


#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "Crv.hpp"
#include "CrvBuffer.hpp"
#include "ReadApi.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

#include "Grid.hpp"

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
Metadata::Metadata(void)
: Object(CN_METADATA)
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
    
  DicStack* pDicStack = AM::pTaskDicStack();

  Stz* szCatalog = Catalog::szCurrCatalog(pDicStack);

  return szCatalog;
}

/*----------------------------------------------------------------------------*/
/* get name of current schema                                                 */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = AM::pTaskDicStack();

  Stz* szSchema = Schema::szCurrSchema(pDicStack);
  
  return szSchema;
}

/*----------------------------------------------------------------------------*/
/* get name of current table                                                  */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = AM::pTaskDicStack();

  Stz* szTable = Table::szCurrTable(pDicStack);
  
  return szTable;
}

/*----------------------------------------------------------------------------*/
/* determine whether a catalog exists and return its full name                */
/*----------------------------------------------------------------------------*/
RowBuffer* Metadata::pLookupCatalog(Stz* szCatalogName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = AM::pTaskDicStack();

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
