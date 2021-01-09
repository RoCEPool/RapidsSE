/*============================================================================*/
/* Dic.cpp - dictionary of words                                              */
/*============================================================================*/

#include "BASE.h"

// !DIC_AS_SKIPLIST
//#define _DIC_SKIPLIST

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
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  m_pArticle = new Article(C_NO_NAME,this,nullptr);
  
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

  delete m_pArticle;
  
  delete m_pWordSkipList;
}

/*----------------------------------------------------------------------------*/
/* add word to this dictionary                                                */
/*----------------------------------------------------------------------------*/
#ifdef _DIC_SKIPLIST
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
  
  m_pArticle->vGrabWrite();

  bool b = m_pWordSkipList->bInsert(pWord,bTrace);
  ASSERT(b);

  SkipNode* pSkipNode = m_pWordSkipList->pNewly();
  pWord->m_pSkipNode = pSkipNode;
  
  m_pArticle->vDropWrite();
  
  ASSERT(pFindWord(pWord->szWordNm()));
}
/*----------------------------------------------------------------------------*/
#else
void Dic::vAddWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  ASSERT(pWord->eGetClassNo() == CN_WORD);
  
  m_pArticle->vGrabWrite();
  
//  vTrace("Word : %s\n"_Stz,pWord->szWordNm());
//  if (pWord->szWordNm()->bSame("job.addDemon"_Stz))
//    vDebugPoint();

  m_pArticle->vAppend(pWord->pArticle());
    
  m_pArticle->vDropWrite();
}
#endif

/*----------------------------------------------------------------------------*/
/* get dictionary article                                                     */
/*----------------------------------------------------------------------------*/
Article* Dic::pArticle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_pArticle;
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
/* set dictionary kind                                                        */
/*----------------------------------------------------------------------------*/
void Dic::vDicKind(Word* pDicKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pDicKind = pDicKind;
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Dic::vDump(eCN eClass, Object* pObject, Stz* szTab)
{
  Dic* pDic = _pDic(pObject);
  
  vTrace("name %s count %lld\n"_Stz,
         pDic->m_sDicNm.azChar(),
         pDic->m_pArticle->nItems());
  
  //for (Word* pWord = m_pLastWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* progressively return each word                                             */
/*----------------------------------------------------------------------------*/
#ifdef _DIC_SKIPLIST
Word* Dic::pEachWord(Word* pPrior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  Word* pNextWord = pPrior ? pPrior->pNextWord() : pFirstWord();

  m_pArticle->vDropRead();

  return pNextWord;
}
/*----------------------------------------------------------------------------*/
#else
Word* Dic::pEachWord(Word* pPrior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  Word* pWord = (pPrior) ? pPrior->pPrevWord() : pLastWord();
  
  m_pArticle->vDropRead();

  return pWord;
}
#endif

/*----------------------------------------------------------------------------*/
/* progressively return each word which is of given kind                      */
/*----------------------------------------------------------------------------*/
#ifdef _DIC_SKIPLIST
Word* Dic::pEachWord(Word* pPrior, Word* pCategory)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  Word* pNextWord = pPrior ? pPrior->pNextWord() : pFirstWord();

  for ( ; pNextWord; pNextWord = pNextWord->pNextWord()) {
    
    if (pNextWord->pCategory() == pCategory) {
      m_pArticle->vDropRead();
      return pNextWord;
    }
  }
  
  m_pArticle->vDropRead();
  
  return nullptr;
}
/*----------------------------------------------------------------------------*/
#else
Word* Dic::pEachWord(Word* pPrior, Word* pCategory)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  Word* pWord = (pPrior) ? pPrior->pPrevWord() : pLastWord();
  
  for ( ; pWord; pWord = pWord->pPrevWord()) {
    
    if (pWord->pCategory() == pCategory) {
      m_pArticle->vDropRead();
      return pWord;
    }
  }
  
  m_pArticle->vDropRead();
  
  return nullptr;
}
#endif

/*----------------------------------------------------------------------------*/
/* find a word given name                                                     */
/*----------------------------------------------------------------------------*/
#ifdef _DIC_SKIPLIST
Word* Dic::pFindWord(Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  bool bTrace = false;
  if (szWordNm->bSame("pipeline.pipeout"_Stz)) {
    vListWords();
    vTrace("\n\n"_Stz);
    vTrace("Find Word : %s\n"_Stz,szWordNm);
    bTrace = true;
  }
  
  Word* pSearchWord = new (nullptr) Word(szWordNm);
  
  m_pArticle->vGrabRead();
  
  Word* pFoundWord = _pWord(m_pWordSkipList->pFind(pSearchWord,bTrace));
  
  m_pArticle->vDropRead();
  
  delete pSearchWord;
  
  return pFoundWord;
}
/*----------------------------------------------------------------------------*/
#else
Word* Dic::pFindWord(Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  for (Word* pWord = pLastWord(); pWord; pWord = pWord->pPrevWord()) {
    
    //void* aEndWord = aAdd<void*>(pWord,sizeof(Word));
    
    if (pWord->szWordNm()->bSame(szWordNm)) {
      m_pArticle->vDropRead();

      if (szWordNm->bSame("whereAmI"_Stz))
        vTrace(": %s is in dictionary %s ---\n"_Stz,szWordNm,m_sDicNm.szStz());

      return pWord;
    }
  }
  
  m_pArticle->vDropRead();
  
  return nullptr;
}
#endif

/*----------------------------------------------------------------------------*/
/* find a word given pointer to step function                                 */
/*----------------------------------------------------------------------------*/
#ifdef _DIC_SKIPLIST
Word* Dic::pFindStep(fpSTEPFN fpStepFn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  for (Word* pWord = pFirstWord(); pWord; pWord = pWord->pNextWord()) {
    if (WK_INTRINSIC == pWord->eWord() && fpStepFn == pWord->fpEntryStep()) {
      m_pArticle->vDropRead();
      return pWord;
    }
  }
  
  m_pArticle->vDropRead();
  
  return nullptr;
}
/*----------------------------------------------------------------------------*/
#else
Word* Dic::pFindStep(fpSTEPFN fpStepFn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  for (Word* pWord = pLastWord(); pWord; pWord = pWord->pPrevWord()) {
    if (WK_INTRINSIC == pWord->eWord() && fpStepFn == pWord->fpEntryStep()) {
      m_pArticle->vDropRead();
      return pWord;
    }
  }
  
  m_pArticle->vDropRead();
  
  return nullptr;
}
#endif

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
/* is given name?                                                             */
/*----------------------------------------------------------------------------*/
bool Dic::bIsName(Stz* szDicNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  bool b = m_sDicNm.bSame(szDicNm);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get last word                                                              */
/*----------------------------------------------------------------------------*/
Word* Dic::pLastWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  Article* pLastArticle = m_pArticle->pTail();
  if (! pLastArticle)
    return nullptr;
  
  return (Word*)pLastArticle->pObject();
}

/*----------------------------------------------------------------------------*/
/* list all words in this dic                                                 */
/*----------------------------------------------------------------------------*/
#ifdef _DIC_SKIPLIST
void Dic::vListWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  for (Word* pWord = pFirstWord(); pWord; pWord = pWord->pNextWord()) {
    
    vTrace(": %s\n"_Stz,pWord->szWordNm());
  }
  
  m_pArticle->vDropRead();
}
/*----------------------------------------------------------------------------*/
#else
void Dic::vListWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  for (Word* pWord = pLastWord(); pWord; pWord = pWord->pPrevWord()) {
    
    vTrace(": %s (dictionary %s)"_Stz,
           pWord->szWordNm(),
           m_sDicNm.szStz());
  }
  
  m_pArticle->vDropRead();
}
#endif

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
  
  m_pArticle->vName(m_sDicNm);
}

/*----------------------------------------------------------------------------*/
/* decrement references to this dictionary                                    */
/*----------------------------------------------------------------------------*/
void Dic::vRefDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  m_pArticle->vRefDec();
}

/*----------------------------------------------------------------------------*/
/* increment references to this dictionary                                    */
/*----------------------------------------------------------------------------*/
void Dic::vRefInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  if (m_pArticle)
    m_pArticle->vRefInc();
}

/*----------------------------------------------------------------------------*/
/* test for outstanding references to this dictionary                         */
/*----------------------------------------------------------------------------*/
bool Dic::bRefOut(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  bool bIsRefed = m_pArticle->bRefOut();
  
  return bIsRefed;
}

/*----------------------------------------------------------------------------*/
/* forget a word given name                                                   */
/*----------------------------------------------------------------------------*/
#ifdef _DIC_SKIPLIST
Word* Dic::pRemoveWord(Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  Word* pWordRemove = pFindWord(szWordNm);
  if (! pWordRemove)
    return pWordRemove;
  
  m_pArticle->vGrabWrite();
  
  bool b = m_pWordSkipList->bRemove(pWordRemove);
  
  m_pArticle->vDropWrite();
  
  return pWordRemove;
}
/*----------------------------------------------------------------------------*/
#else
Word* Dic::pRemoveWord(Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  Word* pWordRemove = pFindWord(szWordNm);
  if (! pWordRemove)
    return pWordRemove;
  
  m_pArticle->vGrabWrite();
  
  m_pArticle->vRemove(pWordRemove->pArticle());
  
  m_pArticle->vDropWrite();
  
  return pWordRemove;
}
#endif

/*----------------------------------------------------------------------------*/
/* get number of words                                                        */
/*----------------------------------------------------------------------------*/
i8 Dic::nWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_pArticle->nItems();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
