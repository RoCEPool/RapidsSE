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
Dic* Dictionaries::_pDic(void* pAddr)
{
  Dic* pDic = reinterpret_cast<Dic*>(pAddr);
  ASSERT(pDic);
  ASSERT(CN_DIC == pDic->eClass());
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Dic::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Dictionary class cannot be new'd");
  
  RASSERT(0);
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
void Dic::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: Dictionary class cannot be deleted");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dic::Dic(void)
: Object(CN_DIC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  m_sName    = "<main>";
  m_pDicKind = nullptr;
  m_bFrozen  = false;
  m_pCurrentWord = nullptr;
  
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
/* add constant                                                               */
/*----------------------------------------------------------------------------*/
void Dic::vAddConstant(const char* aName, i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  Word* pNewWord = new(this) Word(aName);
  
  IntVal xIntVal;
  xIntVal = iVal;
  
  pNewWord->vEncode("push",&xIntVal);
  pNewWord->vEncode("exit");
}

/*----------------------------------------------------------------------------*/
/* add constant                                                               */
/*----------------------------------------------------------------------------*/
void Dic::vAddConstant(const char* aName, const char* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  Word* pNewWord = new(this) Word(aName);
  
  ValWsp xWspStr;
  xWspStr = aValue;
  
  pNewWord->vEncode("push",&xWspStr);
  pNewWord->vEncode("exit");
}

/*----------------------------------------------------------------------------*/
/* allocate a word                                                            */
/*----------------------------------------------------------------------------*/
Word* Dic::pAllocWord(eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
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
Word* Dic::pCurrentWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  return m_pCurrentWord;
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
void Dic::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_DIC);
  ASSERT(pObject->eClass() == CN_DIC);
  
  Dic* pDic = reinterpret_cast<Dic*>(pObject);
  
  vTrace("%sDictionary %p:\n", pTab, pDic);
  vTrace("%s  name %s \n", pTab, pDic->m_sName.a());
  vTrace("%s  lastWord %p\n\n", pTab, pDic->m_pCurrentWord);
  
  //for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* find a word given name                                                     */
/*----------------------------------------------------------------------------*/
Word* Dic::pFind(const char* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
    if (String::bEqualTo(aName,pWord->sName().a(),C_NO_CASE))
      return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a word given function                                                 */
/*----------------------------------------------------------------------------*/
Word* Dic::pFindStep(TaskStep pStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
    if (WT_PRIMITIVE == pWord->eWordType() && pStep == pWord->pDoesStep())
      return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* forget a word given name                                                   */
/*----------------------------------------------------------------------------*/
bool Dic::bForget(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

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
void Dic::vFreeWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  gpSmallHeap->vFreeObject(pWord);
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
  
  m_bFrozen = bFrozen;
}

/*----------------------------------------------------------------------------*/
/* are there words in the dictionary?                                         */
/*----------------------------------------------------------------------------*/
bool Dic::bHasWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);

  if (m_pCurrentWord)
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
  
  m_pCurrentWord = nullptr;
  
  Word* pWord = Word::pToWord(this);
  
  m_sName.vInit();
  m_sName = pWord->sName();
}

/*----------------------------------------------------------------------------*/
/* is given name?                                                             */
/*----------------------------------------------------------------------------*/
bool Dic::bIsName(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIC,_FOOTER_);
  
  bool b = m_sName.bIsEqual(sName,C_NO_CASE);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get name of this dictionary                                                */
/*----------------------------------------------------------------------------*/
s& Dic::sName(void)
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
  
  m_sName = aName;
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