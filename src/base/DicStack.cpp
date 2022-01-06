/*============================================================================*/
/* DicStack.cpp - dictionary stack                                            */
/*============================================================================*/

#include "BASE.h"

#include "SP_Dic.hpp"
#include "SP_Word.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,DicStack,CN_DICSTACK)
__aNewFunction(DicStack,CN_DICSTACK)
__vDeleteFunction(DicStack,CN_DICSTACK)
__aFooterFunction(DicStack)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DicStack::DicStack(void)
: Object(CN_DICSTACK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  m_iTop = -1;

  for (i8 i_Dic = 0; i_Dic < C_DIC_STACKDEPTH; i_Dic++)
    m_pDic[i_Dic] = nullptr;
  
  _checkInitialised_(CN_DICSTACK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DicStack::~DicStack(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  vDiscardAll();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get catalog dictionary                                                     */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pCatalogDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  for (i8 i_Dic = 0; i_Dic <= m_iTop; i_Dic++) {
    
    Dic* pDic = pPick(i_Dic);
    
    if (pDic->bIsCatalog())
      return pDic;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dump content of dictionary stack                                           */
/*----------------------------------------------------------------------------*/
void DicStack::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("Dictionary TypedStack\n"_Stz);
  vTrace("----------------\n"_Stz);

  vTrace("%p:\n"_Stz,this);

  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--)
    vTrace("[%02lld] %s\n"_Stz,i_Dic,m_pDic[i_Dic]->szDicNm());
}

/*----------------------------------------------------------------------------*/
/* copy given stack into this one                                             */
/*----------------------------------------------------------------------------*/
void DicStack::vCopy(DicStack* pDicStackSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  vDiscardAll();
  
  for (i8 i_Dic = pDicStackSrc->m_iTop; i_Dic >= 0; i_Dic--) {
    m_pDic[i_Dic] = pDicStackSrc->m_pDic[i_Dic];
  }
  
  m_iTop = pDicStackSrc->m_iTop;
}

/*----------------------------------------------------------------------------*/
/* discard top dictionary                                                     */
/*----------------------------------------------------------------------------*/
void DicStack::vDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DIC_STACKDEPTH);
  
  Dic* pDic = m_pDic[m_iTop];

  if (pDic)
    pDic->vRefDec();

  m_pDic[m_iTop] = nullptr;
  
  m_iTop--;
}

/*----------------------------------------------------------------------------*/
/* discard top dictionary                                                     */
/*----------------------------------------------------------------------------*/
void DicStack::vDiscardAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  while (m_iTop >= 0)
    vDiscard();
}

/*----------------------------------------------------------------------------*/
/* dump dictionary stack                                                      */
/*----------------------------------------------------------------------------*/
void DicStack::vDump(eCN eClass, Object* pObject, Stz* szTab)
{
  UNUSED(eClass);
  
  DicStack* pDicStack = _pDicStack(pObject);
  
  vTrace("top %p:"_Stz,pDicStack->m_iTop);
  //for (i8 i_Dic = pDicStack->m_iTop; i_Dic >= 0; i_Dic --)
    //vTrace("%s  Dic %p %lld %s\n"_Stz,aTab, pDicStack->m_pDic[i_Dic],i,
           //pDicStack->m_pDic[i_Dic]->sName().s());
  
  for (i8 i_Dic = pDicStack->m_iTop; i_Dic >= 0; i_Dic--) {
    vTrace(" dic %p %lld %s"_Stz,szTab, pDicStack->m_pDic[i_Dic],i_Dic,
           pDicStack->m_pDic[i_Dic]->szDicNm());
    //Dic::vDump(CN_DIC, pDicStack->m_pDic[i], aTab);
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* find sub-dic                                                               */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pFindDic(Stz* szDicNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
 
  Word* pWord = pFindInCurrent(szDicNm);
  if (! pWord)
    return nullptr;
  
  Dic* pDic = _pDic(pWord->pObject());
  
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DicStack::pFindWord(Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pWord;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    if ( (pWord = pDic->pFindWord((Stz*)szWordNm)) )
      return pWord;
  }
  
  Dic* pDicRoot = AM::DIC::pRoot();

  if ( (pWord = pDicRoot->pFindWord((Stz*)szWordNm)) )
    return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find word in current dictionary                                            */
/*----------------------------------------------------------------------------*/
Word* DicStack::pFindInCurrent(Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  
  Word* pWord;
  
  if ( (pWord = m_pDic[m_iTop]->pFindWord((Stz*)szWordNm)) )
    return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current index                                                          */
/*----------------------------------------------------------------------------*/
Index* DicStack::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  Dic* pDic = pTop();

  //SP_Dic spDic;
  //vTrace("DIC %s \n"_Stz,spDic.sz(pDic));

  //pDic->vListWords();

  Word* pIndexWd = pDic->pFindWord(pDic->szDicNm());

  //SP_Word spWord;
  //vTrace("WORD %s \n"_Stz,spWord.sz(pTableWd));

  Index* pIndex = _pIndex(pIndexWd->pObject());

  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* get number of dictionaries in the stack                                    */
/*----------------------------------------------------------------------------*/
i8 DicStack::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  return m_iTop + 1;
}

/*----------------------------------------------------------------------------*/
/* locate word in stack of dictionaries that has given cell                   */
/*----------------------------------------------------------------------------*/
Word* DicStack::pLocateRoutine(CodeCell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pWord;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    for (pWord = pDic->pFirstWord(); pWord; pWord = pWord->pNextWord())
      if (pWord->bHasCell(aCell))
        return pWord;
  }
  
  Dic* pDicRoot = AM::DIC::pRoot();
  for (pWord = pDicRoot->pFirstWord(); pWord; pWord = pWord->pNextWord())
    if (pWord->bHasCell(aCell))
      return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* locate word that has given step                                            */
/*----------------------------------------------------------------------------*/
Word* DicStack::pLocateStepWord(fpSTEPFN fpStepFn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pWord;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    for (pWord = pDic->pFirstWord(); pWord; pWord = pWord->pNextWord())
      if (WK_INTRINSIC == pWord->eWord() &&
          fpStepFn == pWord->fpEntryStep())
        return pWord;
  }
  
  Dic* pDicRoot = AM::DIC::pRoot();
  for (pWord = pDicRoot->pFirstWord();
       pWord;
       pWord = pWord->pNextWord())
    if (WK_INTRINSIC == pWord->eWord() &&
        fpStepFn == pWord->fpEntryStep())
      return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a particular dictionary on the stack                                   */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pPick(i8 iPick)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  //Dic* pDic = m_pDic[m_iTop - iPick];
  Dic* pDic = m_pDic[iPick];

  return pDic;
}

/*----------------------------------------------------------------------------*/
/* pop top dictionary                                                         */
/*----------------------------------------------------------------------------*/
void DicStack::vPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DIC_STACKDEPTH);
  
  Dic* pDic = m_pDic[m_iTop];

  if (pDic)
    pDic->vRefDec();

  m_pDic[m_iTop] = nullptr;
  
  m_iTop--;
}

/*----------------------------------------------------------------------------*/
/* print the details of the dic stack                                         */
/*----------------------------------------------------------------------------*/
void DicStack::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  for (i8 i_Dic = nItems() - 1; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = pPick(i_Dic);
    Stz* szDicNm = pDic->szDicNm();

    Word* pDicKind = pDic->pDicKind();
    Stz* szDicKindNm = pDicKind ? pDicKind->szWordNm() : "n/a"_Stz;

    vTrace("[%lld] %s %s"_Stz,
           i_Dic,
           szDicNm,
           szDicKindNm);
    
    if (pDic->bIsTenant())
      vTrace(" TENANT\n"_Stz);
    else
    if (pDic->bIsCatalog())
      vTrace(" CATALOG\n"_Stz);
    else
    if (pDic->bIsSchema())
      vTrace(" SCHEMA\n"_Stz);
    else
    if (pDic->bIsTable())
      vTrace(" TABLE\n"_Stz);
    else
    if (pDic->bIsFolder())
      vTrace(" FOLDER\n"_Stz);
    else
      vTrace(" n/a\n"_Stz);

  }

}

/*----------------------------------------------------------------------------*/
/* push dictionary onto stack                                                 */
/*----------------------------------------------------------------------------*/
void DicStack::vPush(Dic* pDic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= -1);
  ASSERT(m_iTop <  C_DIC_STACKDEPTH - 1);
  
  ASSERT(CN_DIC == pDic->eGetClassNo());
  
  if (pDic)
    pDic->vRefInc();
  
  m_iTop++;
  
  m_pDic[m_iTop] = pDic;
}

/*----------------------------------------------------------------------------*/
/* push onto this dic stack given catalog                                     */
/*----------------------------------------------------------------------------*/
bool DicStack::bPush(Stz* szCatalogNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  RASSERT(-1 == m_iTop);
    
  if (! bTest(szCatalogNm))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* push onto this dic stack given catalog, schema                             */
/*----------------------------------------------------------------------------*/
bool DicStack::bPush(Stz* szCatalogNm, Stz* szSchemaNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  RASSERT(-1 == m_iTop);
    
  if (! bTest(szCatalogNm,szSchemaNm))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* push onto this dic stack given catalog, schema, table                       */
/*----------------------------------------------------------------------------*/
bool DicStack::bPush(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  RASSERT(-1 == m_iTop);
    
  if (! bTest(szCatalogNm,szSchemaNm,szTableNm))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* push named dic onto this dic stack                                         */
/*----------------------------------------------------------------------------*/
bool DicStack::bPushNamed(Stz* szDicNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Dic* pDic = pFindDic(szDicNm);
  if (! pDic)
    return false;

  vPush(pDic);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push tenant boray onto this dic stack                                      */
/*----------------------------------------------------------------------------*/
bool DicStack::bPush_boray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Dic* pDic = pFindDic("boray"_Stz);
  if (! pDic)
    return false;

  vPush(pDic);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push catalogs dic onto this dic stack                                      */
/*----------------------------------------------------------------------------*/
bool DicStack::bPush_catalogs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Dic* pDic = pFindDic("catalogs"_Stz);
  if (! pDic)
    return false;

  vPush(pDic);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push indexes dic onto this dic stack                                       */
/*----------------------------------------------------------------------------*/
bool DicStack::bPush_indexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  Dic* pDic = pFindDic("indexes"_Stz);
  if (! pDic)
    return false;

  vPush(pDic);

  return true;
};

/*----------------------------------------------------------------------------*/
/* push public dic onto this dic stack                                        */
/*----------------------------------------------------------------------------*/
bool DicStack::bPush_public(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Dic* pDic = pFindDic("public"_Stz);
  if (! pDic)
    return false;

  vPush(pDic);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push rapidsse dic onto this dic stack                                      */
/*----------------------------------------------------------------------------*/
bool DicStack::bPush_rapidsse(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Dic* pDic = pFindDic("rapidsse"_Stz);
  if (! pDic)
    return false;

  vPush(pDic);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push schemas dic onto this dic stack                                       */
/*----------------------------------------------------------------------------*/
bool DicStack::bPush_schemas(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Dic* pDic = pFindDic("schemas"_Stz);
  if (! pDic)
    return false;
  
  vPush(pDic);
  
  return true;
};

/*----------------------------------------------------------------------------*/
/* push tables dic onto this dic stack                                        */
/*----------------------------------------------------------------------------*/
bool DicStack::bPush_tables(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Dic* pDic = pFindDic("tables"_Stz);
  if (! pDic)
    return false;
  
  vPush(pDic);
  
  return true;
};

/*----------------------------------------------------------------------------*/
/* push tenants dic onto this dic stack                                       */
/*----------------------------------------------------------------------------*/
bool DicStack::bPush_tenants(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  RASSERT(-1 == m_iTop);
  
  Word* pTenants = AM::pTenants();
  RASSERT(pTenants);

  Dic* pDic = _pDic(pTenants->pObject());
  vPush(pDic);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* forget word in stack of dictionaries                                       */
/*----------------------------------------------------------------------------*/
Word* DicStack::pRemoveWord(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pRemovedWord = nullptr;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    if ( (pRemovedWord = pDic->pRemoveWord(szName)) )
      return pRemovedWord;
  }
  
  Dic* pDicRoot = AM::DIC::pRoot();
  if ( (pRemovedWord = pDicRoot->pRemoveWord(szName)) )
    return pRemovedWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get schema dictionary                                                      */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pSchemaDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  for (i8 i_Dic = 0; i_Dic <= m_iTop; i_Dic++) {
    
    Dic* pDic = pPick(i_Dic);
    
    if (pDic->bIsSchema())
      return pDic;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current streamDef                                                      */
/*----------------------------------------------------------------------------*/
StreamDef* DicStack::pStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get table dictionary                                                       */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pTableDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  for (i8 i_Dic = 0; i_Dic <= m_iTop; i_Dic++) {
    Dic* pDic = pPick(i_Dic);
    vTrace("  %s %s\n"_Stz,pDic->szDicNm(),pDic->szDicKind());
  }
  
  for (i8 i_Dic = 0; i_Dic <= m_iTop; i_Dic++) {
    
    Dic* pDic = pPick(i_Dic);
    
    if (pDic->bIsTable())
      return pDic;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current table                                                          */
/*----------------------------------------------------------------------------*/
Table* DicStack::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Dic* pDic = pTop();
  
  //SP_Dic spDic;
  //vTrace("DIC %s \n"_Stz,spDic.sz(pDic));
  
  //pDic->vListWords();
  
  Word* pTableWd = pDic->pFindWord(pDic->szDicNm());

  //SP_Word spWord;
  //vTrace("WORD %s \n"_Stz,spWord.sz(pTableWd));
  
  Table* pTable = _pTable(pTableWd->pObject());

  return pTable;
}

/*----------------------------------------------------------------------------*/
/* get tenant dictionary                                                      */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pTenantDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  for (i8 i_Dic = 0; i_Dic <= m_iTop; i_Dic++) {
    
    Dic* pDic = pPick(i_Dic);
    
    if (pDic->bIsTenant())
      return pDic;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* use this dic stack to test for given catalog                               */
/*----------------------------------------------------------------------------*/
bool DicStack::bTest(Stz* szCatalogNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  RASSERT(-1 == m_iTop);
  
  if (! bPush_tenants())
    return false;
  if (! bPush_boray())
    return false;
  if (! bPush_catalogs())
    return false;

  if (! bPushNamed(szCatalogNm))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* use this dic stack to test for given catalog, schema                       */
/*----------------------------------------------------------------------------*/
bool DicStack::bTest(Stz* szCatalogNm, Stz* szSchemaNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  RASSERT(-1 == m_iTop);
    
  if (! bTest(szCatalogNm))
    return false;

  if (! bPush_schemas())
    return false;

  if (! bPushNamed(szSchemaNm))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* use this dic stack to test for given catalog, schema, table                */
/*----------------------------------------------------------------------------*/
bool DicStack::bTest(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  RASSERT(-1 == m_iTop);
    
  if (! bTest(szCatalogNm,szSchemaNm))
    return false;

  if (! bPush_tables())
    return false;

  if (! bPushNamed(szTableNm))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* use this dic stack to test for given catalog, schema, table                */
/*----------------------------------------------------------------------------*/
bool DicStack::bTest(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm, Stz* szIndexNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  RASSERT(-1 == m_iTop);

  if (! bTest(szCatalogNm,szSchemaNm))
    return false;

  if (! bPush_tables())
    return false;

  if (! bPushNamed(szTableNm))
    return false;

  if (! bPush_indexes())
    return false;

  if (! pFindWord(szIndexNm))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* get topmost dictionary                                                     */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pTop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  ASSERT(m_iTop <  C_DIC_STACKDEPTH);
  
  if (-1 == m_iTop)
    return nullptr;
  
  Dic* pTop = m_pDic[m_iTop];
  
  return pTop;
}

/*----------------------------------------------------------------------------*/
/* get topmost dictionary                                                     */
/*----------------------------------------------------------------------------*/
void DicStack::vToPathStr(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  szPath->vCopy(""_Stz);
  
  for (i8 i_Dic = 0; i_Dic <= m_iTop; i_Dic++) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    //vTrace("[%02lld] %s\n"_Stz,i_Dic,pDic->szName());
    
    //if (i_Dic != m_iTop)
    szPath->vConcat("/"_Stz);

    szPath->vConcat(pDic->szDicNm());
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
