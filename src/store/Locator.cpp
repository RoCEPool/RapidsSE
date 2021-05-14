/*============================================================================*/
/* Locator.cpp - Item Locator                                                 */
/*============================================================================*/

#include "BASE.h"

#include "Locator.hpp"
#include "MY.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Locator,CN_LOCATION)
__aNewFunction(Locator,CN_LOCATION)
__vDeleteFunction(Locator,CN_LOCATION)
__aFooterFunction(Locator)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Locator::Locator(void)
: Object(CN_LOCATION)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  m_pTenant  = nullptr;
  m_pCatalog = nullptr;
  m_pSchema  = nullptr;
  
  _checkInitialised_(CN_LOCATION);
  
  _postcondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Locator::Locator(Location* pLocation)
: Object(CN_LOCATION)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  m_pTenant  = nullptr;
  m_pCatalog = nullptr;
  m_pSchema  = nullptr;
  
  _checkInitialised_(CN_LOCATION);
  
  vLocate(pLocation);
  
  _postcondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Locator::~Locator(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* folders list                                                               */
/*----------------------------------------------------------------------------*/
void Locator::vFlist(void)
{
  _static_
  
  vTrace("flist\n"_Stz);

  Word* pWord = AM::WORD::pFind("tenants"_Stz);
    
  Object* pObject = pWord->pObject();
 
  Dic* pDic = _pDic(pObject);

  AM::DIC::vListContents(pDic);

  //pDic->vListContents(0);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* locate given location                                                      */
/*----------------------------------------------------------------------------*/
void Locator::vLocate(Location* pLocation)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
//  vTrace("%s\n"_Stz,pLocation->szTenant());
//  vTrace("%s\n"_Stz,pLocation->szCatalog());
//  vTrace("%s\n"_Stz,pLocation->szSchema());
  
  Word* pWord = AM::WORD::pFind("tenants"_Stz);
  Object* pObject = pWord->pObject();
  Dic* pDic = _pDic(pObject);
  //vTrace("%s %s\n"_Stz,pWord->szWordNm(), pWord->pCategory()->szWordNm());

  
  pWord = pDic->pFindWord(pLocation->szTenant());
  if (!pWord)
    return;
  pObject = pWord->pObject();
  m_pTenant = pDic = _pDic(pObject);
  //vTrace("%s %s\n"_Stz,pDic->szDicNm(), pWord->pCategory()->szWordNm());
  
  pWord = pDic->pFindWord("catalogs"_Stz);
  if (!pWord)
    return;
  pObject = pWord->pObject();
  pDic = _pDic(pObject);
  //vTrace("%s %s\n"_Stz,pWord->szWordNm(), pWord->pCategory()->szWordNm());
  
  pWord = pDic->pFindWord(pLocation->szCatalog());
  pObject = pWord->pObject();
  m_pCatalog = pDic = _pDic(pObject);
  if (!pWord)
    return;
  //vTrace("%s %s\n"_Stz,pWord->szWordNm(), pWord->pCategory()->szWordNm());
  
  pWord = pDic->pFindWord("schemas"_Stz);
  if (!pWord)
    return;
  pObject = pWord->pObject();
  pDic = _pDic(pObject);
  //vTrace("%s %s\n"_Stz,pWord->szWordNm(), pWord->pCategory()->szWordNm());
  
  pWord = pDic->pFindWord(pLocation->szSchema());
  if (!pWord)
    return;
  pObject = pWord->pObject();
  m_pSchema = pDic = _pDic(pObject);
  //vTrace("%s %s\n"_Stz,pWord->szWordNm(), pWord->pCategory()->szWordNm());
  
  pWord = pDic->pFindWord("tables"_Stz);
  if (!pWord)
    return;
  pObject = pWord->pObject();
  pDic = _pDic(pObject);
  //vTrace("%s %s\n"_Stz,pWord->szWordNm(), pWord->pCategory()->szWordNm());
  
  
  vTrace("%s\n"_Stz,m_pTenant->szDicNm());
  vTrace("%s\n"_Stz,m_pCatalog->szDicNm());
  vTrace("%s\n"_Stz,m_pSchema->szDicNm());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get tenant                                                                 */
/*----------------------------------------------------------------------------*/
Dic* Locator::pTenant(void)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  return m_pTenant;
}

/*----------------------------------------------------------------------------*/
/* get catalog                                                                */
/*----------------------------------------------------------------------------*/
Dic* Locator::pCatalog(void)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  return m_pCatalog;
}

/*----------------------------------------------------------------------------*/
/* get schema                                                                 */
/*----------------------------------------------------------------------------*/
Dic* Locator::pSchema(void)
{
  _precondition_(_MAGIC_,CN_LOCATION,_FOOTER_);
  
  return m_pSchema;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
