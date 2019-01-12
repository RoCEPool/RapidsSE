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
  
  m_sName    = "<no-name>"_Stz;
  m_pDicKind = nullptr;
  m_bFrozen  = false;
  m_ziRefCount.vCheckInit();

  m_oLock.vInitLock(LK_DIC);
  
  m_pLastWord = nullptr;
  m_nWords = 0;
  
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
}

/*----------------------------------------------------------------------------*/
/* add word to this dictionary                                                */
/*----------------------------------------------------------------------------*/
void Dic::vAddWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  ASSERT(pWord->eClass() == CN_WORD);
  
  m_oLock.vGrabWrite();
  
  ASSERT(!m_bFrozen);
  
  pWord->m_pPrevWord = m_pLastWord;
  
  m_pLastWord = pWord;
  
  m_nWords++;
  
  m_oLock.vDropWrite();
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
void Dic::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Dic* pDic = _pDic(pObject);
  
  vTrace("name %s lastWord %p count %lld\n"_Stz,
         pDic->m_sName.aChar(),
         pDic->m_pLastWord,
         pDic->m_nWords);
  
  //for (Word* pWord = m_pLastWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
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
}

/*----------------------------------------------------------------------------*/
/* find a word given name                                                     */
/*----------------------------------------------------------------------------*/
Word* Dic::pFindWord(Stz* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_oLock.vGrabRead();
  
  for (Word* pWord = m_pLastWord; pWord; pWord = pWord->pPrevWord()) {
    
    void* pEndWord = pAdd<void*>(pWord,sizeof(Word));
    
    if (pWord->sName().bSame(aName)) {
      m_oLock.vDropRead();

      if (aName->bSame("whereAmI"_Stz))
        vTrace(": %s is in dictionary %s ---\n"_Stz,aName,m_sName.aStz());

      return pWord;
    }
  }
  
  m_oLock.vDropRead();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a word given function                                                 */
/*----------------------------------------------------------------------------*/
Word* Dic::pFindStep(StepFp pStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_oLock.vGrabRead();
  
  for (Word* pWord = m_pLastWord; pWord; pWord = pWord->pPrevWord()) {
    if (WT_PRIMITIVE == pWord->eWordType() && pStep == pWord->pEntryStep()) {
      m_oLock.vDropRead();
      return pWord;
    }
  }
  
  m_oLock.vDropRead();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* is dictionary frozen?                                                      */
/*----------------------------------------------------------------------------*/
bool Dic::bFrozen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_bFrozen;
}

/*----------------------------------------------------------------------------*/
/* set dictionary frozen?                                                     */
/*----------------------------------------------------------------------------*/
void Dic::vFrozen(bool bFrozen)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_oLock.vGrabWrite();
  
  m_bFrozen = bFrozen;
  
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* are there words in the dictionary?                                         */
/*----------------------------------------------------------------------------*/
bool Dic::bHasWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  if (m_pLastWord)
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
  
  m_pLastWord = nullptr;
  
  Word* pWord = Word::pToWord(this);
  
  //m_sName.vInit();
  m_sName = pWord->sName();
  
  m_nWords = 0;
}

/*----------------------------------------------------------------------------*/
/* is given name?                                                             */
/*----------------------------------------------------------------------------*/
bool Dic::bIsName(s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  bool b = m_sName.bSame(sName);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get last word                                                              */
/*----------------------------------------------------------------------------*/
Word* Dic::pLastWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_pLastWord;
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
void Dic::vName(Stz* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_sName = aName;
}

/*----------------------------------------------------------------------------*/
/* decrement references to this dictionary                                    */
/*----------------------------------------------------------------------------*/
void Dic::vRefDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  u8 iRef = --m_ziRefCount;
  
  //vTrace("DIC: %s --ref : %lld\n"_Stz,m_sName.s(),iRef);
}

/*----------------------------------------------------------------------------*/
/* increment references to this dictionary                                    */
/*----------------------------------------------------------------------------*/
void Dic::vRefInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  u8 iRef = ++m_ziRefCount;
  
  //vTrace("DIC: %s ++ref : %lld\n"_Stz,m_sName.s(),iRef);
}

/*----------------------------------------------------------------------------*/
/* test for outstanding references to this dictionary                         */
/*----------------------------------------------------------------------------*/
bool Dic::bRefOut(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  u8 iRef = *m_ziRefCount;
  
  return iRef ? true : false;
}

/*----------------------------------------------------------------------------*/
/* forget a word given name                                                   */
/*----------------------------------------------------------------------------*/
Word* Dic::pRemoveWord(s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  Word* pWordDelete = nullptr;
  
  m_oLock.vGrabWrite();
  
  if (m_pLastWord->bIsName(sName)) {
    pWordDelete = m_pLastWord;
    m_pLastWord = m_pLastWord->pPrevWord();
    //delete pWordDelete;
    m_nWords--;
    m_oLock.vDropWrite();
    return pWordDelete;
  }
  
  for (Word* pWord = m_pLastWord; pWord; pWord = pWord->pPrevWord()) {
    
    Word* pPrevWord = pWord->pPrevWord();
    
    if (!pPrevWord) {
      m_oLock.vDropWrite();
      return nullptr;
    }
    
    if (!pPrevWord->bIsName(sName))
      continue;
    
    pWordDelete = pPrevWord;
    
    pWord->vPrevWord(pPrevWord->pPrevWord());
    
    //delete pWordDelete;
    m_nWords--;
    
    m_oLock.vDropWrite();
    
    return pWordDelete;
  }
  
  m_oLock.vDropWrite();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get number of words                                                        */
/*----------------------------------------------------------------------------*/
i8 Dic::nWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_nWords;
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
