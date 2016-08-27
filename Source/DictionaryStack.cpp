/*============================================================================*/
/* DictionaryStack.cpp - dictionary stack                                     */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* DictionaryStack::operator new(size_t iSize)
{
  vFatal("fatal: DictionaryStack class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void DictionaryStack::operator delete(void* pAddress)
{
  vFatal("fatal: DictionaryStack class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DictionaryStack::DictionaryStack(void)
{
  _poolAddress_(this);

  m_iTop = -1;

  for (i8 i = 0; i < C_DICTIONARY_STACK; i++)
    m_pDictionary[i] = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DictionaryStack::~DictionaryStack(void)
{
  _poolAddress_(this);
}

/*----------------------------------------------------------------------------*/
/* get number of dictionaries in the stack                                    */
/*----------------------------------------------------------------------------*/
i8 DictionaryStack::iCount(void)
{
  _poolAddress_(this);
  
  return m_iTop + 1;
}

/*----------------------------------------------------------------------------*/
/* discard top dictionary                                                     */
/*----------------------------------------------------------------------------*/
void DictionaryStack::vDiscard(void)
{
  _poolAddress_(this);
  
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
  _poolAddress_(this);
  
  for (i8 i = m_iTop; i >= 0; i--)
    vTrace("Dic %p %lld %s\n",m_pDictionary[i],i,m_pDictionary[i]->sName().a());
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DictionaryStack::pFind(s& sName)
{
  _poolAddress_(this);
  
  ASSERT(m_iTop >= 0);

  Word* pWord = pFind(sName.a());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DictionaryStack::pFind(const char* aName)
{
  _poolAddress_(this);
  
  ASSERT(m_iTop >= 0);
  
  Word* pWord;
  
  for (i8 i = m_iTop; i >= 0; i--) {
    if ( (pWord = m_pDictionary[i]->pFind(aName)) )
      return pWord;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a word given function                                                 */
/*----------------------------------------------------------------------------*/
Word* DictionaryStack::pFindFunc(Tasks::pFUN pFunc)
{
  _poolAddress_(this);
  
  for (i8 i = m_iTop; i >= 0; i--) {
    for (Word* pWord = m_pDictionary[i]->pCurrentWord(); pWord; pWord = pWord->pPrevWord())
      if (WT_PRIMITIVE == pWord->eWordType() && pFunc == pWord->pFunc())
        return pWord;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* forget word in stack of dictionaries                                       */
/*----------------------------------------------------------------------------*/
bool DictionaryStack::bForget(s& sName)
{
  _poolAddress_(this);
  
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
  _poolAddress_(this);

  for (i8 i = m_iTop; i >= 0; i--) {
    for (Word* pWord = m_pDictionary[i]->pCurrentWord(); pWord; pWord = pWord->pPrevWord())
      if (pWord->bHasLocation(pCell))
        return pWord;
  }
  
  return nullptr;
  
}

/*----------------------------------------------------------------------------*/
/* get a particular dictionary on the stack                                   */
/*----------------------------------------------------------------------------*/
Dictionary* DictionaryStack::pPick(i8 iPick)
{
  _poolAddress_(this);
  
  Dictionary* pDic = m_pDictionary[m_iTop - iPick];
  
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* push dictionary onto stack                                                 */
/*----------------------------------------------------------------------------*/
void DictionaryStack::vPush(Dictionary* pDictionary)
{
  _poolAddress_(this);
  
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
  _poolAddress_(this);

  Dictionary* pTop = m_pDictionary[m_iTop];
  
  return pTop;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/