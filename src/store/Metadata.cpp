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
/* list all metadata                                                          */
/*----------------------------------------------------------------------------*/
void Metadata::vListContents(i8 i_Depth, Dic* pDic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
  
  if (! pDic) {
    Dic* pDic = pCurrTenant();
    vTrace("\nTENANT %s \\folder\n"_Stz,pDic->szDicNm());
    vListContents(0,pDic);
    return;
  }
  
  pDic->pArticle()->vGrabRead();
  
  for (Word* pWord = pDic->pFirstWord(); pWord; pWord = pWord->pNextWord()) {
    
    Word* pCategory = pWord->pCategory();

    vTrace("%s%s %s\n"_Stz,
           Stz::szTab(i_Depth),
           pWord->szWordNm(),
           pCategory ? pCategory->szWordNm() : "n/a"_Stz);
    
    Object* pObject = pWord->pObject();
    if (pObject && CN_DIC == pObject->eGetClassNo())
      vListContents(i_Depth+1,(Dic*)pObject);
    else
    if (pObject && CN_TABLE == pObject->eGetClassNo())
      ((Table*)pObject)->vListContents(i_Depth+1);
    else
    if (pObject && CN_INDEX == pObject->eGetClassNo())
      ((Index*)pObject)->vListContents(i_Depth+1);
  }
  
  pDic->pArticle()->vDropRead();
}

/*----------------------------------------------------------------------------*/
/* determine whether a catalog exists and return its full name                */
/*----------------------------------------------------------------------------*/
RowBuffer* Metadata::pLookupCatalog(Stz* szCatalogName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = new DicStack;

  delete pDicStack;
  
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

/*----------------------------------------------------------------------------*/
/* test for given catalog                                                     */
/*----------------------------------------------------------------------------*/
bool Metadata::bTest(Stz* szCatalogNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = new DicStack;

  bool bFound = pDicStack->bTest(szCatalogNm);
  
  delete pDicStack;
  
  return bFound;
}

/*----------------------------------------------------------------------------*/
/* test for given catalog, schema                                             */
/*----------------------------------------------------------------------------*/
bool Metadata::bTest(Stz* szCatalogNm, Stz* szSchemaNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = new DicStack;

  bool bFound = pDicStack->bTest(szCatalogNm,szSchemaNm);
  
  delete pDicStack;
  
  return bFound;
}

/*----------------------------------------------------------------------------*/
/* test for given catalog, schema, table                                      */
/*----------------------------------------------------------------------------*/
bool Metadata::bTest(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = new DicStack;

  bool bFound = pDicStack->bTest(szCatalogNm,szSchemaNm,szTableNm);
  
  delete pDicStack;
  
  return bFound;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
