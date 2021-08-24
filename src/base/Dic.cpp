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
  m_pDicKind = nullptr;
    
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
/* add word to this dictionary                                                */
/*----------------------------------------------------------------------------*/
void Dic::vAddWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  ASSERT(pWord->eGetClassNo() == CN_WORD);
      
//  vTrace("Add Word : %s\n"_Stz,pWord->szWordNm());
  bool bTrace = false;
//  if (pWord->szWordNm()->bSame("defaultSeedDist_beg"_Stz)) {
//    vTrace("\n\n"_Stz);
//    vListWords();
//    vTrace("\n\n"_Stz);
//    //vTrace("Add Word : %s\n"_Stz,pWord->szWordNm());
//    bTrace = true;
//  }
  
  m_okLock.vGrabWrite();

  bool b = m_pWordSkipList->bInsert(pWord,bTrace);
  ASSERT(b);
  UNUSED(b);
  
  SkipNode* pSkipNode = m_pWordSkipList->pNewly();
  pWord->m_pSkipNode = pSkipNode;
  
  m_okLock.vDropWrite();
  
  ASSERT(pFindWord(pWord->szWordNm()));
}

/*----------------------------------------------------------------------------*/
/* get dictionary kind                                                        */
/*----------------------------------------------------------------------------*/
Word* Dic::pDicKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_pDicKind;
}

/*----------------------------------------------------------------------------*/
/* get name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
Stz* Dic::szDicKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  if (m_pDicKind)
    return m_pDicKind->szWordNm();
  else
    return "none"_Stz;
}

/*----------------------------------------------------------------------------*/
/* set dictionary kind                                                        */
/*----------------------------------------------------------------------------*/
void Dic::vDicKind(Word* pDicKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pDicKind = pDicKind;
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
/* progressively return each word which is of given kind                      */
/*----------------------------------------------------------------------------*/
Word* Dic::pEachWord(Word* pPrior, Word* pCategory)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_okLock.vGrabRead();
  
  Word* pNextWord = pPrior ? pPrior->pNextWord() : pFirstWord();

  for ( ; pNextWord; pNextWord = pNextWord->pNextWord()) {
    
    if (pNextWord->pCategory() == pCategory) {
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
  
  bool bTrace = false;
//  if (szWordNm->bSame("pipeline.pipeout"_Stz)) {
//    vListWords();
//    vTrace("\n\n"_Stz);
//    vTrace("Find Word : %s\n"_Stz,szWordNm);
//    bTrace = true;
//  }
  
  if (szWordNm->nLength() > 31)
    return nullptr;
  
  Word* pSearchWord = new (nullptr) Word(szWordNm);
  
  m_okLock.vGrabRead();
  
  Word* pFoundWord =
    _pWord(m_pWordSkipList->pFind(pSearchWord,bTrace,g_fp_bWordCompare));
  
  m_okLock.vDropRead();
  
  delete pSearchWord;
  
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
  
  Word* pKind = pDicKind();
  
  if (pKind && pKind->szWordNm()->bSame("\\catalog"_Stz))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is this dic a folder?                                                      */
/*----------------------------------------------------------------------------*/
bool Dic::bIsFolder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  Word* pKind = pDicKind();
  
  if (pKind && pKind->szWordNm()->bSame("\\folder"_Stz))
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
  
  Word* pKind = pDicKind();
  
  if (pKind && pKind->szWordNm()->bSame("\\schema"_Stz))
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
  
  Word* pKind = pDicKind();
  
  if (pKind && pKind->szWordNm()->bSame("\\table"_Stz))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is this dic a tenant?                                                      */
/*----------------------------------------------------------------------------*/
bool Dic::bIsTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  Word* pKind = pDicKind();
  
  if (pKind && pKind->szWordNm()->bSame("\\tenant"_Stz))
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
    
    Word* pCategory = pWord->pCategory();

    vTrace("%s: %s %s\n"_Stz,
           Stz::szTab(i_Depth),
           pWord->szWordNm(),
           pCategory ? pCategory->szWordNm() : "n/a"_Stz);
    
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
