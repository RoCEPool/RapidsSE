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
/* get current catalog                                                        */
/*----------------------------------------------------------------------------*/
Dic* Metadata::pCurrCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = AM::pTaskDicStack();

  for (i8 i_Dic = pDicStack->iCount() - 1; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = pDicStack->pPick(i_Dic);
    
    if (pDic->bIsCatalog())
      return pDic;

  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get name of current catalog                                                */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  Dic* pDic = pCurrCatalog();

  if (pDic)
    return pDic->szDicNm();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current schema                                                         */
/*----------------------------------------------------------------------------*/
Dic* Metadata::pCurrSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = AM::pTaskDicStack();

  for (i8 i_Dic = pDicStack->iCount() - 1; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = pDicStack->pPick(i_Dic);
    
    if (pDic->bIsSchema())
      return pDic;

  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get name of current schema                                                 */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  Dic* pDic = pCurrSchema();

  if (pDic)
    return pDic->szDicNm();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current table                                                          */
/*----------------------------------------------------------------------------*/
Dic* Metadata::pCurrTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = AM::pTaskDicStack();

  for (i8 i_Dic = pDicStack->iCount() - 1; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = pDicStack->pPick(i_Dic);
    
    if (pDic->bIsTable())
      return pDic;

  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get name of current table                                                  */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  Dic* pDic = pCurrTable();

  if (pDic)
    return pDic->szDicNm();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current tenant                                                         */
/*----------------------------------------------------------------------------*/
Dic* Metadata::pCurrTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = AM::pTaskDicStack();

  for (i8 i_Dic = pDicStack->iCount() - 1; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = pDicStack->pPick(i_Dic);
    
    if (pDic->bIsTenant())
      return pDic;

  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get name of current table                                                  */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  Dic* pDic = pCurrTenant();

  if (pDic)
    return pDic->szDicNm();

  return nullptr;
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
