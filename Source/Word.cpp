/*============================================================================*/
/* Word.cpp -- Word in Dictionary                                             */
/*============================================================================*/

#include "Tasks.h" // needed for Task::vEnter

extern BufferPool*  gpBufferPool;
extern Dictionary*  gpMainDic;
extern Word*        gpWord;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Word* Dictionaries::_pWord(void* pAddr)
{
  Word* pWord = reinterpret_cast<Word*>(pAddr);
  
  ASSERT(pWord);
  ASSERT(CN_WORD == pWord->eClass());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Word::operator new(size_t iSize, Dictionary* pDictionary)
{
  RASSERT(iSize);

  Word* pWord = pDictionary->pAllocWord(CN_WORD);
  
  pWord->m_pDictionary = pDictionary;
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void Word::operator delete(void* pWordAddress)
{
  Word* pWord = reinterpret_cast<Word*>(pWordAddress);
  
  Dictionary* pDictionary = pWord->m_pDictionary;
  
  pDictionary->vFreeWord(pWord);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Word::Word(const char* sName)
: Object(CN_WORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD);
  
  m_sName       = sName;
  m_eWordType   = WT_ROUTINE;
  //m_pDictionary = pDictionary;
  m_pKind       = gpWord ? gpWord : this;
  m_bImmediate  = false;
  m_iDataBytes  = 0;
  
  _setFooter_();
  
  m_pDoesStep = &Tasks::Task::vEnterRoutine;
  m_pDoesArg  = pFirstCell();
  
  _checkInitialised_(sizeof(Word));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Word::~Word(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* allot data space                                                           */
/*----------------------------------------------------------------------------*/
char* Word::pAllot(i8 iBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  char* pAddress = pcAdd(pFirstCell(),m_iDataBytes);
  
  //gpMainDic->vExtendLinear(iBytes);
  
  m_iDataBytes += iBytes;

  _setFooter_();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return pAddress;
}

/*----------------------------------------------------------------------------*/
/* append a pointer to cell                                                   */
/*----------------------------------------------------------------------------*/
void Word::vAppend(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* pAddress = pAppend(pCell);
  ASSERT(pAddress);
}

/*----------------------------------------------------------------------------*/
/* append a pointer to cell                                                   */
/*----------------------------------------------------------------------------*/
void* Word::pAppend(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* pAllottedCell = (Cell*)pAllot(sizeof(pCell));
  
  pAllottedCell->m_pJumpTo = pCell;
  
  return pAllottedCell;
}

/*----------------------------------------------------------------------------*/
/* append an i8 integer                                                       */
/*----------------------------------------------------------------------------*/
void Word::vAppend(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* pAddress = pAppend(iInt);
  ASSERT(pAddress);
}

/*----------------------------------------------------------------------------*/
/* append an i8 integer                                                       */
/*----------------------------------------------------------------------------*/
void* Word::pAppend(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  i8* pAllottedInt = (i8*)pAllot(sizeof(i8));
  
  *pAllottedInt = iInt;
  
  return pAllottedInt;
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void Word::vAppend(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  void* pAddress = pAppend(pValue);
  ASSERT(pAddress);
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void* Word::pAppend(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  i8 iSize = pValue->iSize();
  
  char* pValueAddress = pAllot(iSize);
  
  Memory::vCopy(pValueAddress,pValue,iSize);
  
  return pValueAddress;
}

/*----------------------------------------------------------------------------*/
/* append address of pointer to task step                                     */
/*----------------------------------------------------------------------------*/
void Word::vAppend(TaskStep* ppCalledWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  void* pAddress = pAppend(ppCalledWord);
  ASSERT(pAddress);
}

/*----------------------------------------------------------------------------*/
/* append address of pointer to task step                                     */
/*----------------------------------------------------------------------------*/
void* Word::pAppend(TaskStep* ppCalledWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  TaskStep** pppCalledWord = (TaskStep**)pAllot(sizeof(TaskStep*));

  *pppCalledWord = ppCalledWord;

  return pppCalledWord;
}

/*----------------------------------------------------------------------------*/
/* get which dictionary                                                       */
/*----------------------------------------------------------------------------*/
Dictionary* Word::pDictionary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pDictionary;
}

/*----------------------------------------------------------------------------*/
/* get step ptr                                                               */
/*----------------------------------------------------------------------------*/
TaskStep Word::pDoesStep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pDoesStep;
}

/*----------------------------------------------------------------------------*/
/* set does step                                                              */
/*----------------------------------------------------------------------------*/
void Word::vDoesStep(TaskStep pStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pDoesStep = pStep;
}

/*----------------------------------------------------------------------------*/
/* get ptr to data area                                                       */
/*----------------------------------------------------------------------------*/
char* Word::pDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  char* pDataArea = pcAdd(this,sizeof(Word));
  
  return pPoolCheck<char*>((void*)pDataArea);
}

/*----------------------------------------------------------------------------*/
/* get number of data bytes                                                   */
/*----------------------------------------------------------------------------*/
i8 Word::iDataBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_iDataBytes;
}

/*----------------------------------------------------------------------------*/
/* get current end of data                                                    */
/*----------------------------------------------------------------------------*/
char* Word::pDataEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  char* pDataEnd = _pc(this) + sizeof(Word) + m_iDataBytes;
  
  return pPoolCheck<char*>((void*)pDataEnd);
}

/*----------------------------------------------------------------------------*/
/* get does argument                                                          */
/*----------------------------------------------------------------------------*/
Cell* Word::pDoesArg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pDoesArg;
}

/*----------------------------------------------------------------------------*/
/* set does argument                                                          */
/*----------------------------------------------------------------------------*/
void Word::vDoesArg(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pDoesArg = pCell;
}

/*----------------------------------------------------------------------------*/
/* get address of does step pointer                                           */
/*----------------------------------------------------------------------------*/
TaskStep* Word::ppDoesStep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return &m_pDoesStep;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Word::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Word* pStepWord = gpMainDic->pFindStep(m_pDoesStep);
  
  vTrace("Word %p: %s %s Kind %s Immediate %d DataBytes %lld "
         "Step %p %s ..Data.. %p\n",
         this,
         m_sName.a(),
         pWT[m_eWordType],
         m_pKind->m_sName.a(),
         m_bImmediate,
         m_iDataBytes,
         m_pDoesStep,
         pStepWord ? pStepWord->sName().a() : "<not-found>",
         pFirstCell()
         );
  }

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Word::vDump(const char* pTab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vTrace("%sWord %p: %s %s %s DataBytes %lld "
         "Step %p ..Data.. %p\n",
         pTab,
         this,
         m_sName.a(),
         pWT[m_eWordType],
         m_bImmediate ? "Immediate" : "Not Immediate",
         m_iDataBytes,
         m_pDoesStep,
         pFirstCell()
         );
}

/*----------------------------------------------------------------------------*/
/* encode call to word                                                        */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Word* pCalledWord = gpMainDic->pFind(aWord);
  
  vAppend(pCalledWord->ppDoesStep());
}

/*----------------------------------------------------------------------------*/
/* encode call to word, with cell* argument                                   */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord, Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWord);
  
  vAppend(pCell);
}

/*----------------------------------------------------------------------------*/
/* encode call to word, with cell* argument                                   */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord, TaskStep* pStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWord);
  
  vAppend(pStep);
}

/*----------------------------------------------------------------------------*/
/* encode call to word, with value argument                                   */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord, Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  vEncode(aWord);
    
  vAppend(pValue);
}

/*----------------------------------------------------------------------------*/
/* encode call to word, with word* argument                                   */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord, Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWord);
    
  PtrVal xPtrWord;
  xPtrWord = pWord;
  
  vAppend(&xPtrWord);
}

/*----------------------------------------------------------------------------*/
/* encode call to word does                                                   */
/*----------------------------------------------------------------------------*/
void Word::vEncode(TaskStep* ppCalledDoes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vAppend(ppCalledDoes);
}

/*----------------------------------------------------------------------------*/
/* get pointer to first cell in data area                                     */
/*----------------------------------------------------------------------------*/
Cell* Word::pFirstCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  Cell* pFirst = reinterpret_cast<Cell*>(pcAdd((void*)this,sizeof(Word)));
                                         
  return pPoolCheck<Cell*>(pFirst);
}

/*----------------------------------------------------------------------------*/
/* does this word have given cell?                                            */
/*----------------------------------------------------------------------------*/
bool Word::bHasCell(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

//  if ((char*)pCell >= pDataArea() && (char*)pCell <= pDataEnd())
//    return true;

  if (pCell >= pFirstCell() && pCell <= pLastCell())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get immediate flag                                                         */
/*----------------------------------------------------------------------------*/
bool Word::bImmediate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_bImmediate;
}

/*----------------------------------------------------------------------------*/
/* set immediate flag                                                         */
/*----------------------------------------------------------------------------*/
void Word::vImmediate(bool bImmediate)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_bImmediate = bImmediate;
}

/*----------------------------------------------------------------------------*/
/* is given name?                                                             */
/*----------------------------------------------------------------------------*/
bool Word::bIsName(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  bool b = m_sName.bIsEqual(sName,C_NO_CASE);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get word kind                                                              */
/*----------------------------------------------------------------------------*/
Word* Word::pKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pKind;
}

/*----------------------------------------------------------------------------*/
/* set word kind                                                              */
/*----------------------------------------------------------------------------*/
void Word::vKind(Word* pKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  m_pKind = pKind;
}

/*----------------------------------------------------------------------------*/
/* get pointer to last cell                                                   */
/*----------------------------------------------------------------------------*/
Cell* Word::pLastCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* pLast = reinterpret_cast<Cell*>(pcAdd((void*)this,sizeof(Word) + m_iDataBytes));
  
  return pPoolCheck<Cell*>(pLast);
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
s& Word::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* get previous word                                                          */
/*----------------------------------------------------------------------------*/
Word* Word::pPrevWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  return pPoolCheck<Word*>(m_pPrevWord);
}

/*----------------------------------------------------------------------------*/
/* set previous word                                                          */
/*----------------------------------------------------------------------------*/
void Word::vPrevWord(Word* pPrevWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pPrevWord = pPrevWord;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this word                                             */
/*----------------------------------------------------------------------------*/
i8 Word::iSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  i8 nBytes = sizeof(Word) - 1 + m_iDataBytes;

  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get word type                                                              */
/*----------------------------------------------------------------------------*/
eWT Word::eWordType(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_eWordType;
}

/*----------------------------------------------------------------------------*/
/* set word type                                                              */
/*----------------------------------------------------------------------------*/
void Word::vWordType(eWT eWordType)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_eWordType = eWordType;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the outer word of this data ptr                                        */
/*----------------------------------------------------------------------------*/
Word* Word::pToWord(void* pData)
{
  // static function
  
  Word* pWord = _pWord( _pc(pData) - sizeof(Word) );
  
  ASSERT(pWord->eClass() == CN_WORD);
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Word::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD);
  
  Footer* pFooter = static_cast<Footer*>(pvAdd(this,sizeof(Word)+m_iDataBytes));

  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/