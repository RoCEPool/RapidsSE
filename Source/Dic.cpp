/*============================================================================*/
/* Dic.cpp - dictionary of words                                              */
/*============================================================================*/

#include "Dictionaries.h"

extern Heap*       gpMediumHeap;
extern Heap*       gpSmallHeap;
extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Dic* Dictionaries::_pDic(void* aAddr)
{
  Dic* pDic = reinterpret_cast<Dic*>(aAddr);
  
  RASSERT( !pDic || CN_DIC == pDic->eClass() );

  return pDic;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Dic::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Dic* pDic = (Dic*)(Heap::pAllocate(CN_DIC));
  
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Dic::operator new(size_t iSize, Dic* pDic)
{
  RASSERT(iSize);
  
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Dic::operator delete(void* pDicAddress)
{
  RASSERT(pDicAddress);
  
  Dic* pDic = reinterpret_cast<Dic*>(pDicAddress);
  
  Heap::vFree(pDic);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dic::Dic(void)
: Object(CN_DIC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_sName    = "<no-name>"_Str;
  m_pDicKind = nullptr;
  m_bFrozen  = false;
  m_zRefCount.vCheckInit();

  m_xLock.vInit("Dic::m_xLock"_Str);
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
Dic::~Dic(void)
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
  
  m_xLock.vGrabWrite();
  
  ASSERT(!m_bFrozen);
  
  pWord->m_pPrevWord = m_pLastWord;
  
  m_pLastWord = pWord;
  
  m_nWords++;
  
  m_xLock.vDropWrite();
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
void Dic::vDump(eCN eClass, Object* pObject, const char* aTab)
{
  ASSERT(eClass == CN_DIC);
  ASSERT(pObject->eClass() == CN_DIC);
  
  Dic* pDic = reinterpret_cast<Dic*>(pObject);
  
  vTrace("name %s lastWord %p\n"_Str,
         pDic->m_sName.aStr(),
         pDic->m_pLastWord);
  
  //for (Word* pWord = m_pLastWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* find a word given name                                                     */
/*----------------------------------------------------------------------------*/
Word* Dic::pFindWord(Str* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_xLock.vGrabRead();
  
  for (Word* pWord = m_pLastWord; pWord; pWord = pWord->pPrevWord()) {
    if (StrLib::bEqualTo(aName->a(),pWord->sName().aStr(),C_NO_CASE)) {
      m_xLock.vDropRead();
      return pWord;
    }
  }
  
  m_xLock.vDropRead();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a word given function                                                 */
/*----------------------------------------------------------------------------*/
Word* Dic::pFindStep(TaskStep pStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_xLock.vGrabRead();
  
  for (Word* pWord = m_pLastWord; pWord; pWord = pWord->pPrevWord()) {
    if (WT_PRIMITIVE == pWord->eWordType() && pStep == pWord->pEntryStep()) {
      m_xLock.vDropRead();
      return pWord;
    }
  }
  
  m_xLock.vDropRead();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* forget a word given name                                                   */
/*----------------------------------------------------------------------------*/
bool Dic::bForget(StrVal& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  Word* pWordDelete = nullptr;
  
  m_xLock.vGrabWrite();
  
  if (m_pLastWord->bIsName(sName)) {
    pWordDelete = m_pLastWord;
    m_pLastWord = m_pLastWord->pPrevWord();
    delete pWordDelete;
    m_nWords--;
    m_xLock.vDropWrite();
    return true;
  }
  
  for (Word* pWord = m_pLastWord; pWord; pWord = pWord->pPrevWord()) {
    
    Word* pPrevWord = pWord->pPrevWord();
    
    if (!pPrevWord) {
      m_xLock.vDropWrite();
      return false;
    }
    
    if (!pPrevWord->bIsName(sName))
      continue;

    pWordDelete = pPrevWord;
    
    pWord->vPrevWord(pPrevWord->pPrevWord());

    delete pWordDelete;
    m_nWords--;

    m_xLock.vDropWrite();
    
    return true;
  }
  
  m_xLock.vDropWrite();

  return false;
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
  
  m_xLock.vGrabWrite();
  
  m_bFrozen = bFrozen;
  
  m_xLock.vDropWrite();
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
bool Dic::bIsName(StrVal& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  bool b = m_sName.bIsEqual(sName,C_NO_CASE);
  
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
StrVal& Dic::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
void Dic::vName(const char* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_sName = (Str*)aName;
}

/*----------------------------------------------------------------------------*/
/* decrement references to this dictionary                                    */
/*----------------------------------------------------------------------------*/
void Dic::vRefDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  u8 iRef = --m_zRefCount;
  
  //vTrace("DIC: %s --ref : %lld\n"_Str,m_sName.aStr(),iRef);
}

/*----------------------------------------------------------------------------*/
/* increment references to this dictionary                                    */
/*----------------------------------------------------------------------------*/
void Dic::vRefInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  u8 iRef = ++m_zRefCount;
  
  //vTrace("DIC: %s ++ref : %lld\n"_Str,m_sName.aStr(),iRef);
}

/*----------------------------------------------------------------------------*/
/* test for outstanding references to this dictionary                         */
/*----------------------------------------------------------------------------*/
bool Dic::bRefOut(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  u8 iRef = *m_zRefCount;
  
  return iRef ? true : false;
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
Footer* Dic::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
