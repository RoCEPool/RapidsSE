/*============================================================================*/
/* Dictionary.cpp - dictionary of words                                       */
/*============================================================================*/

#include "Dictionaries.h"

extern Heap*       gpMediumHeap;
extern Heap*       gpSmallHeap;
extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Dictionary* Dictionaries::_pDic(void* pAddr)
{
  Dictionary* pDic = reinterpret_cast<Dictionary*>(pAddr);
  ASSERT(pDic);
  ASSERT(CN_DICTIONARY == pDic->eClass());
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Dictionary::operator new(size_t iSize)
{
  ASSERT(iSize);
  vFatal("fatal: Dictionary class cannot be new'd");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Dictionary::operator new(size_t iSize, Dictionary* pDictionary)
{
  ASSERT(iSize);
  return pDictionary;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Dictionary::operator delete(void* pBufferAddress)
{
  vFatal("fatal: Dictionary class cannot be deleted");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dictionary::Dictionary(void)
: Object(CN_DICTIONARY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  m_sName  = "<main>";
  m_bFrozen = false;
  m_pCurrentWord = nullptr;
  
  // this is because of GlobalDictionary (ugh)
  //vInit();
  
  Memory::vCheckInitialised(this,sizeof(Dictionary));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dictionary::~Dictionary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add constant                                                               */
/*----------------------------------------------------------------------------*/
void Dictionary::vAddConstant(const char* aName, i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  Word* pNewWord = new(this) Word(aName);
  
  IntVal xIntVal;
  xIntVal = iVal;
  
  pNewWord->vEncode("push",&xIntVal);
  pNewWord->vEncode("exit");
}

/*----------------------------------------------------------------------------*/
/* add constant                                                               */
/*----------------------------------------------------------------------------*/
void Dictionary::vAddConstant(const char* aName, const char* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  Word* pNewWord = new(this) Word(aName);
  
  StrVal xStrVal;
  xStrVal = aValue;
  
  pNewWord->vEncode("push",&xStrVal);
  pNewWord->vEncode("exit");
}

/*----------------------------------------------------------------------------*/
/* allocate a word                                                            */
/*----------------------------------------------------------------------------*/
Word* Dictionary::pAllocWord(eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  ASSERT(!m_bFrozen);
  
  ASSERT(eClassNo == CN_WORD);
  
  Word* pWord = reinterpret_cast<Word*>(gpMediumHeap->pAllocObject(CN_WORD));
  
  pWord->m_pPrevWord = m_pCurrentWord;
  
  m_pCurrentWord = pWord;
  
  return pPoolCheck<Word*>(pWord);
}

/*----------------------------------------------------------------------------*/
/* get current word                                                           */
/*----------------------------------------------------------------------------*/
Word* Dictionary::pCurrentWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  return m_pCurrentWord;
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Dictionary::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_DICTIONARY);
  ASSERT(pObject->eClass() == CN_DICTIONARY);
  
  Dictionary* pDictionary = reinterpret_cast<Dictionary*>(pObject);
  
  vTrace("%sDictionary %p:\n", pTab, pDictionary);
  vTrace("%s  name %s \n", pTab, pDictionary->m_sName.a());
  vTrace("%s  lastWord %p\n\n", pTab, pDictionary->m_pCurrentWord);
  
  //for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* find a word given name                                                     */
/*----------------------------------------------------------------------------*/
Word* Dictionary::pFind(const char* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
    if (String::bEqualTo(aName,pWord->sName().a(),C_NO_CASE))
      return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a word given function                                                 */
/*----------------------------------------------------------------------------*/
Word* Dictionary::pFindFunc(Tasks::pFUN pFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
    if (WT_PRIMITIVE == pWord->eWordType() && pFunc == pWord->pFunc())
      return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* forget a word given name                                                   */
/*----------------------------------------------------------------------------*/
bool Dictionary::bForget(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);

  Word* pWordDelete = nullptr;
  
  if (m_pCurrentWord->bIsName(sName)) {
    pWordDelete = m_pCurrentWord;
    m_pCurrentWord = m_pCurrentWord->pPrevWord();
    delete pWordDelete;
    return true;
  }
  
  for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord()) {
    
    Word* pPrevWord = pWord->pPrevWord();
    
    if (!pPrevWord)
      return false;
    
    if (!pPrevWord->bIsName(sName))
      continue;

    pWordDelete = pPrevWord;
    
    pWord->vPrevWord(pPrevWord->pPrevWord());

    delete pWordDelete;
    
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* free a word                                                                */
/*----------------------------------------------------------------------------*/
void Dictionary::vFreeWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  gpSmallHeap->vFreeObject(pWord);
}

/*----------------------------------------------------------------------------*/
/* is dictionary frozen?                                                      */
/*----------------------------------------------------------------------------*/
bool Dictionary::bFrozen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  return m_bFrozen;
}

/*----------------------------------------------------------------------------*/
/* set dictionary frozen?                                                     */
/*----------------------------------------------------------------------------*/
void Dictionary::vFrozen(bool bFrozen)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  m_bFrozen = bFrozen;
}

/*----------------------------------------------------------------------------*/
/* are there words in the dictionary?                                         */
/*----------------------------------------------------------------------------*/
bool Dictionary::bHasWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);

  if (m_pCurrentWord)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* initialize this dictionary                                                 */
/*----------------------------------------------------------------------------*/
void Dictionary::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  m_pCurrentWord = nullptr;
  
  Word* pWord = Word::pToWord(this);
  
  m_sName.vInit();
  m_sName = pWord->sName();
}

/*----------------------------------------------------------------------------*/
/* is given name?                                                             */
/*----------------------------------------------------------------------------*/
bool Dictionary::bIsName(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  bool b = m_sName.bIsEqual(sName,C_NO_CASE);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
s& Dictionary::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
void Dictionary::vName(const char* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY,_FOOTER_);
  
  m_sName = aName;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Dictionary::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICTIONARY);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/