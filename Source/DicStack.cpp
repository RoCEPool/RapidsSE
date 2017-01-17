/*============================================================================*/
/* DicStack.cpp - dictionary stack                                            */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* DicStack::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: DictionaryStack class cannot be new'd");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void DicStack::operator delete(void* pAddress)
{
  RASSERT(pAddress);
  vFatal("fatal: DictionaryStack class cannot be deleted");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DicStack::DicStack(void)
: Object(CN_DICSTACK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  m_iTop = -1;

  for (i8 i = 0; i < C_DICTIONARY_STACK; i++)
    m_pDic[i] = nullptr;
  
  _checkInitialised_(CN_DICSTACK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DicStack::~DicStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  while (m_iTop >= 0)
    vDiscard();
}

/*----------------------------------------------------------------------------*/
/* get number of dictionaries in the stack                                    */
/*----------------------------------------------------------------------------*/
i8 DicStack::iCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  return m_iTop + 1;
}

/*----------------------------------------------------------------------------*/
/* dump stack of dictionaries                                                 */
/*----------------------------------------------------------------------------*/
void DicStack::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  for (i8 i = m_iTop; i >= 0; i--)
    vTrace("Dic %p %lld %s\n",m_pDic[i],i,m_pDic[i]->sName().a());
}

/*----------------------------------------------------------------------------*/
/* discard top dictionary                                                     */
/*----------------------------------------------------------------------------*/
void DicStack::vDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DICTIONARY_STACK);
  
  Dic* pDic = m_pDic[m_iTop];

  if (pDic)
    pDic->vRefDec();

  m_pDic[m_iTop] = nullptr;
  
  m_iTop--;
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void DicStack::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_DICSTACK);
  ASSERT(pObject->eClass() == CN_DICSTACK);
  
  DicStack* pDicStack = reinterpret_cast<DicStack*>(pObject);
  
  vTrace("%sDictionary Stack %p:\n", pTab, pDicStack);
  vTrace("%s  Top %p:\n", pTab, pDicStack->m_iTop);
  //for (i8 i = pDicStack->m_iTop; i >= 0; i--)
    //vTrace("%s  Dic %p %lld %s\n",pTab, pDicStack->m_pDic[i],i,
           //pDicStack->m_pDic[i]->sName().a());
  
  for (i8 i = pDicStack->m_iTop; i >= 0; i--) {
  vTrace("%s  Dic %p %lld %s\n",pTab, pDicStack->m_pDic[i],i,
         pDicStack->m_pDic[i]->sName().a());
    Dic::vDump(CN_DIC, pDicStack->m_pDic[i], pTab);
  }

  //for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DicStack::pFind(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);

  Word* pWord = pFind(sName.a());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DicStack::pFind(const char* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  
  Word* pWord;
  
  for (i8 i = m_iTop; i >= 0; i--) {
    if ( (pWord = m_pDic[i]->pFind(aName)) )
      return pWord;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find word in current dictionary                                            */
/*----------------------------------------------------------------------------*/
Word* DicStack::pFindInCurrent(const char* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  
  Word* pWord;
  
  if ( (pWord = m_pDic[m_iTop]->pFind(aName)) ) {
    return pWord;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a word given step                                                     */
/*----------------------------------------------------------------------------*/
Word* DicStack::pFindStep(TaskStep pStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  for (i8 i = m_iTop; i >= 0; i--) {
    for (Word* pWord = m_pDic[i]->pLastWord(); pWord; pWord = pWord->pPrevWord())
      if (WT_PRIMITIVE == pWord->eWordType() && pStep == pWord->pEntryStep())
        return pWord;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* forget word in stack of dictionaries                                       */
/*----------------------------------------------------------------------------*/
bool DicStack::bForget(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
    
  for (i8 i = m_iTop; i >= 0; i--) {
    if ( m_pDic[i]->bForget(sName) )
      return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* locate word in stack of dictionaries                                       */
/*----------------------------------------------------------------------------*/
Word* DicStack::pLocate(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  for (i8 i = m_iTop; i >= 0; i--) {
    for (Word* pWord = m_pDic[i]->pLastWord(); pWord; pWord = pWord->pPrevWord())
      if (pWord->bHasCell(pCell))
        return pWord;
  }
  
  return nullptr;
  
}

/*----------------------------------------------------------------------------*/
/* get a particular dictionary on the stack                                   */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pPick(i8 iPick)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Dic* pDic = m_pDic[m_iTop - iPick];
  
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* push dictionary onto stack                                                 */
/*----------------------------------------------------------------------------*/
void DicStack::vPush(Dic* pDic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= -1);
  ASSERT(m_iTop <  C_DICTIONARY_STACK - 1);
  
  ASSERT(CN_DIC == pDic->eClass());
  
  pDic->vRefInc();
  
  m_iTop++;
  
  m_pDic[m_iTop] = pDic;
}

/*----------------------------------------------------------------------------*/
/* get topmost dictionary                                                     */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pTop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  Dic* pTop = m_pDic[m_iTop];
  
  return pTop;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* DicStack::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
