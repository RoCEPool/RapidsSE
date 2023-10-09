/*============================================================================*/
/* Dic.cpp - dictionary of words                                              */
/*============================================================================*/

#include "BASE.h"

CompareFp g_fp_bWordCompare = bWordCompare;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Dic,CN_DIC)
__aNewFunction(Dic,CN_DIC)
__vDeleteFunction(Dic,CN_DIC)
__aFooterFunction(Dic)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dic::Dic(Stz* szDicNm)
: Object(CN_DIC)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_sDicNm   = szDicNm;
  m_eDicKind = DK_NONE;
    
  m_pWordSkipList = new SkipListNL(C_UNQIUE_KEY,bWordCompare,vWordPrint);

  _checkInitialised_(CN_DIC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dic::~Dic(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  delete m_pWordSkipList;
}

/*----------------------------------------------------------------------------*/
/* add an existing SkipNode to this dictionary (used during reload from disk) */
/*----------------------------------------------------------------------------*/
void Dic::vAddSkipNode(SkipNode *pSkipNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  bool bTrace = false;

  Word* pWord = _pWord(pSkipNode->m_pObject);
  RASSERT(pWord);

  m_okLock.vGrabWrite();

  bool b = m_pWordSkipList->bInsertWithNode(pWord, pSkipNode, bTrace);
  RASSERT(b);

  SkipNode* pSkipNodeNewly = m_pWordSkipList->pNewly();
  RASSERT(pSkipNodeNewly == pSkipNode);

  // This step is probably not needed
  pWord->m_pSkipNode = pSkipNodeNewly;

  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* add word to this dictionary                                                */
/*----------------------------------------------------------------------------*/
void Dic::vAddWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  ASSERT(pWord->eGetClassNo() == CN_WORD);
      
  //vTrace("Add Word : %s\n"_Stz,pWord->szWordNm());
  
  bool bTrace = false;
  
  m_okLock.vGrabWrite();

  bool b = m_pWordSkipList->bInsert(pWord,bTrace);
  //ASSERT(b); watch out here Myvan
  UNUSED(b);
  
  SkipNode* pSkipNode = m_pWordSkipList->pNewly();
  pWord->m_pSkipNode = pSkipNode;

  // Note: During unload/reload, we need an easy way to figure out whether
  // a word is part of the table dictionary (which is part of root_dic in AM).
  // Hence, we use a bit to store this information. This allows us to quickly
  // reconnect the words and their dictionaries back to the root dic during
  // a reload operation (while walking the Persistent Tiny Heap).
  if(m_sDicNm.bSame("tables"_Stz))
  {
    pWord->m_bTableBit = true;
  }
  
  m_okLock.vDropWrite();
  
  ASSERT(pFindWord(pWord->szWordNm()));
}

/*----------------------------------------------------------------------------*/
/* get dictionary kind (enum)                                                 */
/*----------------------------------------------------------------------------*/
eDK Dic::eDicKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  return m_eDicKind;
}

/*----------------------------------------------------------------------------*/
/* get name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
Stz* Dic::szDicKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return aDK[m_eDicKind];
}

/*----------------------------------------------------------------------------*/
/* set dictionary kind                                                        */
/*----------------------------------------------------------------------------*/
void Dic::vDicKind(eDK eDicKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  m_eDicKind = eDicKind;
}

/*----------------------------------------------------------------------------*/
/* get name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
Stz* Dic::szDicNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_sDicNm;
}

/*----------------------------------------------------------------------------*/
/* set name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
void Dic::vDicNm(Stz* szDicNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_sDicNm = szDicNm;
}

/*----------------------------------------------------------------------------*/
/* drop lock for read                                                         */
/*----------------------------------------------------------------------------*/
void Dic::vDropRead(void)
{
  _precondition_(_MAGIC_,CN_DIC,_FOOTER_);
  
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Dic::vDropWrite(void)
{
  _precondition_(_MAGIC_,CN_DIC,_FOOTER_);

  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Dic::vDump(eCN eClass, Object* pObject, Stz* szTab)
{
  UNUSED(eClass);
  UNUSED(szTab);
  
  Dic* pDic = _pDic(pObject);
  
  vTrace("name %s \n"_Stz,
         pDic->m_sDicNm.azChar());
  
  //for (Word* pWord = m_pLastWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Dic::vTree(Dic* pDic, i8 iTabs)
{
  vTrace("%sD:%s %s\n"_Stz, Stz::szTab(iTabs),
         pDic->m_sDicNm.azChar(),
         aDK[pDic->m_eDicKind]->azChar());

  Word* pWord = nullptr;
  pWord = pDic->pEachWord(pWord);

  while ( pWord )
  {
    vTrace("%sW%d:%s %s  %s\n"_Stz, Stz::szTab(iTabs),
           pWord->bGetPersistent(),
           pWord->szWordNm(),
           aWC[pWord->eCategoryEnum()]->azChar(),
           pWord->m_pObject ? aCN[pWord->m_pObject->eGetClassNo()]->azChar() : "");

    if(pWord->m_pObject && pWord->m_pObject->eGetClassNo() == CN_DIC)
    {
      Dic* pSubDic = _pDic(pWord->m_pObject);
      Dic::vTree(pSubDic, iTabs+1);
    }

    pWord = pDic->pEachWord(pWord);
  }
}

/*----------------------------------------------------------------------------*/
/* set SkipList compare function pointer                                      */
/*----------------------------------------------------------------------------*/
void Dic::vDicCompareFp(Dic* pDic)
{
  _static_

  pDic->m_pWordSkipList->m_fpCompare = bWordCompare;
}

/*----------------------------------------------------------------------------*/
/* convert Dictionary Kind to Word Category                                   */
/*----------------------------------------------------------------------------*/
eWC Dic::eDKToWC(eDK eDicKind)
{
  _static_

  switch(eDicKind)
  {
    case DK_NONE:
      return WC_NONE;

    case DK_CATALOG:
      return WC_CATALOG;

    case DK_FOLDER:
      return WC_FOLDER;

    case DK_SCHEMA:
      return WC_SCHEMA;

    case DK_TABLE:
      return WC_TABLE;

    case DK_TENANT:
      return WC_TENANT;

    default:
      return WC_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* convert Word Category to Dictionary Kind                                   */
/*----------------------------------------------------------------------------*/
eDK Dic::eWCToDK(eWC eWordCtg)
{
  _static_

  switch(eWordCtg)
  {
    case WC_NONE:
      return DK_NONE;

    case WC_CATALOG:
      return DK_CATALOG;

    case WC_FOLDER:
      return DK_FOLDER;

    case WC_SCHEMA:
      return DK_SCHEMA;

    case WC_TABLE:
      return DK_TABLE;

    case WC_TENANT:
      return DK_TENANT;

    default:
      return DK_NONE;
  }
}

/*----------------------------------------------------------------------------*/
/* progressively return each word                                             */
/*----------------------------------------------------------------------------*/
Word* Dic::pEachWord(Word* pPrior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_okLock.vGrabRead();
  
  Word* pNextWord = pPrior ? pPrior->pNextWord() : pFirstWord();

  m_okLock.vDropRead();

  return pNextWord;
}

/*----------------------------------------------------------------------------*/
/* progressively return each word which is of given word category             */
/*----------------------------------------------------------------------------*/
Word* Dic::pEachWordWc(Word* pPrior, eWC eCategory)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  m_okLock.vGrabRead();

  Word* pNextWord = pPrior ? pPrior->pNextWord() : pFirstWord();

  for ( ; pNextWord; pNextWord = pNextWord->pNextWord()) {
    if (pNextWord->eCategoryEnum() == eCategory) {
      m_okLock.vDropRead();
      return pNextWord;
    }
  }

  m_okLock.vDropRead();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a word given name                                                     */
/*----------------------------------------------------------------------------*/
Word* Dic::pFindWord(Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  if (szWordNm->nLength() > 31)
    return nullptr;

  Task* pThisTask = AM::pThisTLSTask();
  Word* pFoundWord = nullptr;

  if (pThisTask) {
    Word* pSearchWord = pThisTask->pSearchWord();
    pSearchWord->vName(szWordNm);
    pFoundWord = pFindWord(pSearchWord, szWordNm);
  }
  else {
    Word *pSearchWord = new(nullptr) Word(szWordNm);
    pFoundWord = pFindWord(pSearchWord, szWordNm);
    delete pSearchWord;
  }

  return pFoundWord;
}

/*----------------------------------------------------------------------------*/
/* find a word given name using word from task for teh search                 */
/*----------------------------------------------------------------------------*/
Word* Dic::pFindWord(Word* pSearchWord, Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  bool bTrace = false;

  if (szWordNm->nLength() > 31)
    return nullptr;

  m_okLock.vGrabRead();

  Word *pFoundWord =
      _pWord(m_pWordSkipList->pFind(pSearchWord, bTrace, g_fp_bWordCompare));

  m_okLock.vDropRead();

  return pFoundWord;
}

/*----------------------------------------------------------------------------*/
/* find a word given pointer to step function                                 */
/*----------------------------------------------------------------------------*/
Word* Dic::pFindStep(fpSTEPFN fpStepFn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_okLock.vGrabRead();
  
  for (Word* pWord = pFirstWord(); pWord; pWord = pWord->pNextWord()) {
    if (WK_INTRINSIC == pWord->eWord() && fpStepFn == pWord->fpEntryStep()) {
      m_okLock.vDropRead();
      return pWord;
    }
  }
  
  m_okLock.vDropRead();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first word in dictionary                                               */
/*----------------------------------------------------------------------------*/
Word* Dic::pFirstWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  SkipNode* pFirstNode = m_pWordSkipList->pFirst(0);
  
  if (! pFirstNode)
    return nullptr;
  
  Word* pFirstWord = _pWord(pFirstNode->m_pObject);

  return pFirstWord;
}

/*----------------------------------------------------------------------------*/
/* grab lock for read                                                         */
/*----------------------------------------------------------------------------*/
void Dic::vGrabRead(void)
{
  _precondition_(_MAGIC_,CN_DIC,_FOOTER_);
  
  m_okLock.vGrabRead();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void Dic::vGrabWrite(void)
{
  _precondition_(_MAGIC_,CN_DIC,_FOOTER_);

  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* are there words in the dictionary?                                         */
/*----------------------------------------------------------------------------*/
bool Dic::bHasWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  if (nWords())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this dic a catalog?                                                     */
/*----------------------------------------------------------------------------*/
bool Dic::bIsCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  if (m_eDicKind == DK_CATALOG)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is this dic a folder?                                                      */
/*----------------------------------------------------------------------------*/
bool Dic::bIsFolder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  if (m_eDicKind == DK_FOLDER)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is given name?                                                             */
/*----------------------------------------------------------------------------*/
bool Dic::bIsName(Stz* szDicNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  bool b = m_sDicNm.bSame(szDicNm);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is this dic a schema?                                                      */
/*----------------------------------------------------------------------------*/
bool Dic::bIsSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  if (m_eDicKind == DK_SCHEMA)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get last word                                                              */
/*----------------------------------------------------------------------------*/
Word* Dic::pLastWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return pFirstWord();
}

/*----------------------------------------------------------------------------*/
/* is this dic a table?                                                       */
/*----------------------------------------------------------------------------*/
bool Dic::bIsTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  if (m_eDicKind == DK_TABLE)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is this dic a tenant?                                                      */
/*----------------------------------------------------------------------------*/
bool Dic::bIsTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  if (m_eDicKind == DK_TENANT)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* list all words in this dic                                                 */
/*----------------------------------------------------------------------------*/
void Dic::vListContents(i8 i_Depth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_okLock.vGrabRead();
  
  
  for (Word* pWord = pFirstWord(); pWord; pWord = pWord->pNextWord()) {
    
    Object* pObject = pWord->pObject();
    if (!pObject)
      continue;
    
    AM::WORD::vListContents(pWord,i_Depth);
  }
  
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* list all words in this dic                                                 */
/*----------------------------------------------------------------------------*/
void Dic::vListWords(i8 i_Depth, bool bListContents)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_okLock.vGrabRead();
  
  for (Word* pWord = pFirstWord(); pWord; pWord = pWord->pNextWord()) {

    vTrace("%s: %s %s\n"_Stz,
           Stz::szTab(i_Depth),
           pWord->szWordNm(),
           aWC[pWord->eCategoryEnum()]->azChar());
    
    if (bListContents) {
      Object* pObject = pWord->pObject();
      if (pObject && CN_DIC == pObject->eGetClassNo()) {
        ((Dic*)pObject)->vListWords(i_Depth+1,bListContents);
      }
    }
  }
  
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* decrement references to this dictionary                                    */
/*----------------------------------------------------------------------------*/
void Dic::vRefDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  -- m_ixRefCount;
}

/*----------------------------------------------------------------------------*/
/* increment references to this dictionary                                    */
/*----------------------------------------------------------------------------*/
void Dic::vRefInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  ++ m_ixRefCount;
}

/*----------------------------------------------------------------------------*/
/* test for outstanding references to this dictionary                         */
/*----------------------------------------------------------------------------*/
bool Dic::bRefOut(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  u8 iRef = * m_ixRefCount;
  
  return iRef ? true : false;
}

/*----------------------------------------------------------------------------*/
/* forget a word given name                                                   */
/*----------------------------------------------------------------------------*/
Word* Dic::pRemoveWord(Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  Word* pWordRemove = pFindWord(szWordNm);
  if (! pWordRemove)
    return pWordRemove;
  
  m_okLock.vGrabWrite();
  
  bool b = m_pWordSkipList->bRemove(pWordRemove);
  UNUSED(b);
  
  m_okLock.vDropWrite();
  
  return pWordRemove;
}

/*----------------------------------------------------------------------------*/
/* get number of words                                                        */
/*----------------------------------------------------------------------------*/
i8 Dic::nWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_pWordSkipList->nItems();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
