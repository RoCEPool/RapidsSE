/*============================================================================*/
/* Dic.cpp - dictionary of words                                              */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Dic* SenVM::_pDic(void* aAddr)
{
  RASSERT(aAddr);
  Dic* pDic = reinterpret_cast<Dic*>(aAddr);
  
  RASSERT(pDic);
  RASSERT_TYPE(pDic,CN_DIC);

  return pDic;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Dic::operator new(size_t)
{
  Dic* pDic = (Dic*)(HeapMem::pAllocate(CN_DIC));
  
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Dic::operator new(size_t, Dic* pDic)
{
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Dic::operator delete(void* pDicAddress)
{  
  Dic* pDic = reinterpret_cast<Dic*>(pDicAddress);
  
  HeapMem::vFree(pDic);
}

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
  
  // this is because of GlobalDictionary (ugh)
  //vInit();
  
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
}

/*----------------------------------------------------------------------------*/
/* add word to this dictionary                                                */
/*----------------------------------------------------------------------------*/
void Dic::vAddWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  ASSERT(pWord->eClass() == CN_WORD);
  
  m_pArticle->vGrabWrite();
  
  m_pArticle->vAppend(pWord->pArticle());
  
  m_pArticle->vDropWrite();
}

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
Word* Dic::pEachWord(Word* pPrior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  Word* pWord = (pPrior) ? pPrior->pPrevWord() : pLastWord();
  
  m_pArticle->vDropRead();

  return pWord;
}

/*----------------------------------------------------------------------------*/
/* progressively return each word which is of given kind                      */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* find a word given name                                                     */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* find a word given pointer to step function                                 */
/*----------------------------------------------------------------------------*/
Word* Dic::pFindStep(StepFp fpStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  for (Word* pWord = pLastWord(); pWord; pWord = pWord->pPrevWord()) {
    if (WK_INTRINSIC == pWord->eWord() && fpStep == pWord->fpEntryStep()) {
      m_pArticle->vDropRead();
      return pWord;
    }
  }
  
  m_pArticle->vDropRead();
  
  return nullptr;
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
/* initialize this dictionary                                                 */
/*----------------------------------------------------------------------------*/
void Dic::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  Word* pWord = Word::pToWord(this);
  
  //m_sDicNm.vInit();
  m_sDicNm = pWord->szWordNm();
  
  delete m_pArticle;
  m_pArticle = new Article(m_sDicNm,this,nullptr);
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
/* find a word given name                                                     */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* get number of words                                                        */
/*----------------------------------------------------------------------------*/
i8 Dic::nWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_pArticle->nItems();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Dic)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
