/*============================================================================*/
/* Dic.cpp - dictionary of words                                              */
/*============================================================================*/

#include "Forth.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Dic* Forth::_pDic(void* aAddr)
{
  Dic* pDic = reinterpret_cast<Dic*>(aAddr);
  
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
Dic::Dic(void)
: Object(CN_DIC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  m_pArticle = new Article(C_NO_NAME,this);
  
  m_sName    = C_NO_NAME;
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
void Dic::vAddWord(ForthWord* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  ASSERT(pWord->eClass() == CN_WORD);
  
  m_pArticle->vGrabWrite();
  
  m_pArticle->vAppend(pWord->pArticle());
  
  m_pArticle->vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get dictionary kind                                                        */
/*----------------------------------------------------------------------------*/
ForthWord* Dic::pDicKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_pDicKind;
}

/*----------------------------------------------------------------------------*/
/* set dictionary kind                                                        */
/*----------------------------------------------------------------------------*/
void Dic::vDicKind(ForthWord* pDicKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pDicKind = pDicKind;
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Dic::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Dic* pDic = _pDic(pObject);
  
  vTrace("name %s count %lld\n"_Stz,
         pDic->m_sName.aChar(),
         pDic->m_pArticle->nItems());
  
  //for (ForthWord* pWord = m_pLastWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* progressively return each word which is of given kind                      */
/*----------------------------------------------------------------------------*/
ForthWord* Dic::pEachWord(ForthWord* pKind, ForthWord* pPrior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  ForthWord* pWord = (pPrior) ? pPrior->pPrevWord() : pLastWord();
  
  for ( ; pWord; pWord = pWord->pPrevWord()) {
    
    if (pWord->pKind() == pKind) {
      m_pArticle->vDropRead();
      return pWord;
    }
  }
  
  m_pArticle->vDropRead();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* finalize                                                                   */
/*----------------------------------------------------------------------------*/
void Dic::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_DIC);
  ASSERT(pObject->eClass() == CN_DIC);
  
  Dic* pDic = reinterpret_cast<Dic*>(pObject);
  ASSERT(pDic->bWordBody());
  
  //vTrace("%s %s finalize\n"_Stz, aCN[eClass], pDic->m_sName.s());
  
  delete pDic->m_pArticle;
}

/*----------------------------------------------------------------------------*/
/* find a word given name                                                     */
/*----------------------------------------------------------------------------*/
ForthWord* Dic::pFindWord(Stz* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  for (ForthWord* pWord = pLastWord(); pWord; pWord = pWord->pPrevWord()) {
    
    //void* aEndWord = aAdd<void*>(pWord,sizeof(ForthWord));
    
    if (pWord->sName().bSame(aName)) {
      m_pArticle->vDropRead();

      if (aName->bSame("whereAmI"_Stz))
        vTrace(": %s is in dictionary %s ---\n"_Stz,aName,m_sName.aStz());

      return pWord;
    }
  }
  
  m_pArticle->vDropRead();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a word given pointer to step function                                 */
/*----------------------------------------------------------------------------*/
ForthWord* Dic::pFindStep(StepFp fpStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  for (ForthWord* pWord = pLastWord(); pWord; pWord = pWord->pPrevWord()) {
    if (WT_INTRINSIC == pWord->eWordType() && fpStep == pWord->fpEntryStep()) {
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
  
  ForthWord* pWord = ForthWord::pToWord(this);
  
  //m_sName.vInit();
  m_sName = pWord->sName();
  
  m_pArticle = new Article(m_sName,this);
}

/*----------------------------------------------------------------------------*/
/* is given name?                                                             */
/*----------------------------------------------------------------------------*/
bool Dic::bIsName(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  bool b = m_sName.bSame(szName);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get last word                                                              */
/*----------------------------------------------------------------------------*/
ForthWord* Dic::pLastWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  Article* pLastArticle = m_pArticle->pTail();
  if (! pLastArticle)
    return nullptr;
  
  return (ForthWord*)pLastArticle->pObject();
}

/*----------------------------------------------------------------------------*/
/* find a word given name                                                     */
/*----------------------------------------------------------------------------*/
void Dic::vListWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_pArticle->vGrabRead();
  
  for (ForthWord* pWord = pLastWord(); pWord; pWord = pWord->pPrevWord()) {
    
    vTrace(": %s (dictionary %s)"_Stz,
           pWord->sName().aChar(),
           m_sName.aStz());
  }
  
  m_pArticle->vDropRead();
}

/*----------------------------------------------------------------------------*/
/* get name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
s32& Dic::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
void Dic::vName(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_sName = szName;
  
  m_pArticle->vName(m_sName);
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
ForthWord* Dic::pRemoveWord(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  ForthWord* pWordRemove = pFindWord(szName);
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

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Dic::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
