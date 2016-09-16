/*============================================================================*/
/* DictionaryStack.cpp - dictionary stack                                     */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* DictionaryStack::operator new(size_t iSize)
{
  ASSERT(iSize);
  vFatal("fatal: DictionaryStack class cannot be new'd");
  
   ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void DictionaryStack::operator delete(void* pAddress)
{
  ASSERT(pAddress);
  vFatal("fatal: DictionaryStack class cannot be deleted");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DictionaryStack::DictionaryStack(void)
: Object(CN_DICTIONARYSTACK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);

  m_iTop = -1;

  for (i8 i = 0; i < C_DICTIONARY_STACK; i++)
    m_pDictionary[i] = nullptr;
  
  _checkInitialised_(sizeof(DictionaryStack));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DictionaryStack::~DictionaryStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get number of dictionaries in the stack                                    */
/*----------------------------------------------------------------------------*/
i8 DictionaryStack::iCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);
  
  return m_iTop + 1;
}

/*----------------------------------------------------------------------------*/
/* discard top dictionary                                                     */
/*----------------------------------------------------------------------------*/
void DictionaryStack::vDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DICTIONARY_STACK);
  
  m_pDictionary[m_iTop] = nullptr;
  
  m_iTop--;
}

/*----------------------------------------------------------------------------*/
/* dump stack of dictionaries                                                 */
/*----------------------------------------------------------------------------*/
void DictionaryStack::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);
  
  for (i8 i = m_iTop; i >= 0; i--)
    vTrace("Dic %p %lld %s\n",m_pDictionary[i],i,m_pDictionary[i]->sName().a());
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void DictionaryStack::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_DICTIONARYSTACK);
  ASSERT(pObject->eClass() == CN_DICTIONARYSTACK);
  
  DictionaryStack* pDictionaryStack = reinterpret_cast<DictionaryStack*>(pObject);
  
  vTrace("%sDictionary Stack %p:\n", pTab, pDictionaryStack);
  vTrace("%s  Top %p:\n", pTab, pDictionaryStack->m_iTop);
  for (i8 i = pDictionaryStack->m_iTop; i >= 0; i--)
    vTrace("%s  Dic %p %lld %s\n",pTab, pDictionaryStack->m_pDictionary[i],i,
           pDictionaryStack->m_pDictionary[i]->sName().a());
  
  //for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DictionaryStack::pFind(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);

  Word* pWord = pFind(sName.a());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DictionaryStack::pFind(const char* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  
  Word* pWord;
  
  for (i8 i = m_iTop; i >= 0; i--) {
    if ( (pWord = m_pDictionary[i]->pFind(aName)) )
      return pWord;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a word given step                                                     */
/*----------------------------------------------------------------------------*/
Word* DictionaryStack::pFindStep(TaskStep pStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);
  
  for (i8 i = m_iTop; i >= 0; i--) {
    for (Word* pWord = m_pDictionary[i]->pCurrentWord(); pWord; pWord = pWord->pPrevWord())
      if (WT_PRIMITIVE == pWord->eWordType() && pStep == pWord->pDoesStep())
        return pWord;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* forget word in stack of dictionaries                                       */
/*----------------------------------------------------------------------------*/
bool DictionaryStack::bForget(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
    
  for (i8 i = m_iTop; i >= 0; i--) {
    if ( m_pDictionary[i]->bForget(sName) )
      return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* locate word in stack of dictionaries                                       */
/*----------------------------------------------------------------------------*/
Word* DictionaryStack::pLocate(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);

  for (i8 i = m_iTop; i >= 0; i--) {
    for (Word* pWord = m_pDictionary[i]->pCurrentWord(); pWord; pWord = pWord->pPrevWord())
      if (pWord->bHasCell(pCell))
        return pWord;
  }
  
  return nullptr;
  
}

/*----------------------------------------------------------------------------*/
/* get a particular dictionary on the stack                                   */
/*----------------------------------------------------------------------------*/
Dictionary* DictionaryStack::pPick(i8 iPick)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);
  
  Dictionary* pDic = m_pDictionary[m_iTop - iPick];
  
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* push dictionary onto stack                                                 */
/*----------------------------------------------------------------------------*/
void DictionaryStack::vPush(Dictionary* pDictionary)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= -1);
  ASSERT(m_iTop <  C_DICTIONARY_STACK - 1);
  
  ASSERT(CN_DICTIONARY == pDictionary->eClass());
  
  m_iTop++;
  
  m_pDictionary[m_iTop] = pDictionary;
}

/*----------------------------------------------------------------------------*/
/* get topmost dictionary                                                     */
/*----------------------------------------------------------------------------*/
Dictionary* DictionaryStack::pTop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK,_FOOTER_);

  Dictionary* pTop = m_pDictionary[m_iTop];
  
  return pTop;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* DictionaryStack::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARYSTACK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/