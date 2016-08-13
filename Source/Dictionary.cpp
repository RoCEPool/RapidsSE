/*============================================================================*/
/* Dictionary.cpp - dictionary of words                                       */
/*============================================================================*/

#include "Dictionaries.h"

extern Heap*       gpSmallHeap;
extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Dictionary::operator new(size_t iSize)
{
  vFatal("fatal: Dictionary class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Dictionary::operator delete(void* pBufferAddress)
{
  vFatal("fatal: Dictionary class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dictionary::Dictionary(void)
//: SlabAllocator(CN_DICTIONARY)
: Object(CN_DICTIONARY)
{
  _precondition_(this, CN_DICTIONARY);
  
  m_pCurrentWord = nullptr;
  m_sName = "<main>";
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dictionary::~Dictionary(void)
{
  _precondition_(this, CN_DICTIONARY);
}

/*----------------------------------------------------------------------------*/
/* allocate a word                                                            */
/*----------------------------------------------------------------------------*/
Word* Dictionary::pAllocWord(i8 iBytes)
{
  _precondition_(this, CN_DICTIONARY);
  
  ASSERT(iBytes >= sizeof(Word));
  
  Word* pWord = reinterpret_cast<Word*>(gpSmallHeap->pAllocObject());
  
  pWord->vPrevWord(m_pCurrentWord);
  
  m_pCurrentWord = pWord;
  
  return pPoolCheck<Word*>(pWord);
}

/*----------------------------------------------------------------------------*/
/* get current word                                                           */
/*----------------------------------------------------------------------------*/
Word* Dictionary::pCurrentWord(void)
{
  _precondition_(this, CN_DICTIONARY);
  
  return m_pCurrentWord;
}

/*----------------------------------------------------------------------------*/
/* dump dictionary and all word                                               */
/*----------------------------------------------------------------------------*/
void Dictionary::vDump(void)
{
  _precondition_(this, CN_DICTIONARY);
  
  vTrace("Dictionary %p: LastWord %p\n",
         this,
         
         m_pCurrentWord
         );
  
  for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
    pWord->vDump();
  
}

/*----------------------------------------------------------------------------*/
/* find a word given name                                                     */
/*----------------------------------------------------------------------------*/
Word* Dictionary::pFind(const char* aName)
{
  _precondition_(this, CN_DICTIONARY);
  
  for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
    if (String::bEqualTo(aName,pWord->sName().a(),C_NO_CASE))
      return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a word given function                                                 */
/*----------------------------------------------------------------------------*/
Word* Dictionary::pFindFunc(Tasking::pFUN pFunc)
{
  _precondition_(this, CN_DICTIONARY);
  
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
  _precondition_(this, CN_DICTIONARY);

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
  _precondition_(this, CN_DICTIONARY);
  
  gpSmallHeap->vFreeObject(pWord);
}

/*----------------------------------------------------------------------------*/
/* are there words in the dictionary?                                         */
/*----------------------------------------------------------------------------*/
bool Dictionary::bHasWords(void)
{
  _precondition_(this, CN_DICTIONARY);

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
  _precondition_(this, CN_DICTIONARY);
  
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
  _poolAddress_(this);
  
  bool b = m_sName.bIsEqual(sName,C_NO_CASE);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
s& Dictionary::sName(void)
{
  _precondition_(this, CN_DICTIONARY);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
void Dictionary::vName(const char* aName)
{
  _precondition_(this, CN_DICTIONARY);
  
  m_sName = aName;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/