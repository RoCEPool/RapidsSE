/*============================================================================*/
/* Word.cpp -- Word in Dictionary                                             */
/*============================================================================*/

#include "Tasks.h" // needed for Task::vEnter
#include "AttachMap.hpp"

extern BufferPool* gpBufferPool;
extern Dic*        gpMainDic;
extern Word*       gpWord;

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Word* Dictionaries::_pWord(void* pAddr)
{
  Word* pWord = reinterpret_cast<Word*>(pAddr);
  
  RASSERT( !pWord || CN_WORD == pWord->eClass() );

  return pWord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Word::operator new(size_t iSize, Dic* pDic)
{
  RASSERT(iSize);

  Word* pWord = (Word*)(Heap::pAllocate(CN_WORD));
  
  pWord->m_pDic = pDic;
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void Word::operator delete(void* pWordAddress)
{
  Word* pWord = reinterpret_cast<Word*>(pWordAddress);
 
  Heap::vFree(pWord);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Word::Word(const char* sName)
: Object(CN_WORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD);
  
  m_sName        = sName;
  m_eWordType    = WT_ROUTINE;
  m_eDistributed = DT_NOPARTICIPANT;
  //m_pDic       = pDic;
  m_pKind        = gpWord ? gpWord : this;
  m_iEpoch       = gpAttachMap->m_iEpoch;
  m_pParticipant = nullptr;
  m_bImmediate   = false;
  m_iParams      = 0;
  m_iDataBytes   = 0;
  m_pPrevWord    = nullptr;
  m_iLocal       = 0;

  _setFooter_();
  
  m_pEntryStep   = &Tasks::Task::vEnterRoutine;
  m_pEntryCell    = pFirstCell();

  _checkInitialised_(CN_WORD);
  
  m_pDic->vAddWord(this);
  
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
Dic* Word::pDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pDic;
}

/*----------------------------------------------------------------------------*/
/* get entry step                                                             */
/*----------------------------------------------------------------------------*/
TaskStep Word::pEntryStep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pEntryStep;
}

/*----------------------------------------------------------------------------*/
/* set entry step                                                             */
/*----------------------------------------------------------------------------*/
void Word::vEntryStep(TaskStep pStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pEntryStep = pStep;
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
/* get which cell to start executing from                                     */
/*----------------------------------------------------------------------------*/
Cell* Word::pEntryCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pEntryCell;
}

/*----------------------------------------------------------------------------*/
/* set which cell to start executing from                                     */
/*----------------------------------------------------------------------------*/
void Word::vEntryCell(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pEntryCell = pCell;
}

/*----------------------------------------------------------------------------*/
/* get address of this words entry step                                       */
/*----------------------------------------------------------------------------*/
TaskStep* Word::ppEntryStep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return &m_pEntryStep;
}

/*----------------------------------------------------------------------------*/
/* get distributed type                                                       */
/*----------------------------------------------------------------------------*/
eDT Word::eDistributed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_eDistributed;
}

/*----------------------------------------------------------------------------*/
/* set distributed type                                                       */
/*----------------------------------------------------------------------------*/
void Word::vDistributed(eDT eDistributed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_eDistributed = eDistributed;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Word::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Word* pStepWord = gpMainDic->pFindStep(m_pEntryStep);
  
  vTrace("Word %p: %s %s Kind %s Immediate %d DataBytes %lld "
         "Step %p %s ..Data.. %p\n",
         this,
         m_sName.a(),
         pWT[m_eWordType],
         m_pKind->m_sName.a(),
         m_bImmediate,
         m_iDataBytes,
         m_pEntryStep,
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
  
  vTrace("%sWord %p: epoch:%lld %s (%s) %s %s DataBytes %lld "
         "Step %p ..Data.. %p\n",
         pTab,
         this,
         m_iEpoch,
         m_sName.a(),
         m_pDic->sName().a(),
         pWT[m_eWordType],
         m_bImmediate ? "Immediate" : "Not Immediate",
         m_iDataBytes,
         m_pEntryStep,
         pFirstCell()
         );
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Word::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_WORD);
  ASSERT(pObject->eClass() == CN_WORD);
  
  Word* pWord = reinterpret_cast<Word*>(pObject);
  
  // dump only the objects
  if (WT_OBJECT == pWord->m_eWordType)
    pWord->vDump(pTab);
  
  //for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* encode call to named word                                                  */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Word* pCalledWord = gpMainDic->pFind(aWord);
  ASSERT(pCalledWord);
  
  
  
  vAppend(pCalledWord->ppEntryStep());
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a cell* argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord, Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWord);
  
  vAppend(pCell);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a value argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord, Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  vEncode(aWord);
    
  vAppend(pValue);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a word* argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord, Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  PtrVal xPtrWord;
  xPtrWord = pWord;
  
  vEncode(aWord,&xPtrWord);
}

/*----------------------------------------------------------------------------*/
/* encode call of entry step                                                  */
/*----------------------------------------------------------------------------*/
void Word::vEncode(TaskStep* ppEntryStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vAppend(ppEntryStep);
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
/* set local variable number                                                  */
/*----------------------------------------------------------------------------*/
void Word::vLocal(i8 iLocal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_iLocal = iLocal;
}

/*----------------------------------------------------------------------------*/
/* get local variable number                                                  */
/*----------------------------------------------------------------------------*/
i8 Word::iLocal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_iLocal;
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
/* set number of parameters                                                   */
/*----------------------------------------------------------------------------*/
void Word::vParams(i8 iParams)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_iParams = iParams;
}

/*----------------------------------------------------------------------------*/
/* get number of parameters                                                   */
/*----------------------------------------------------------------------------*/
i8 Word::iParams(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_iParams;
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
