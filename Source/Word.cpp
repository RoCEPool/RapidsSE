/*============================================================================*/
/* Word.cpp -- Word in Dictionary                                             */
/*============================================================================*/

#include "Dictionaries.h"

#include "Tasks.h" // needed for Task::vEnter

#include "StorageEngine.h"

extern BufferPool* gpBufferPool;
extern Dic*        gpMainDic;
extern Word*       gpWord;

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Word* Dictionaries::_pWord(void* aAddr)
{
  Word* pWord = reinterpret_cast<Word*>(aAddr);
  
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
Word::Word(Str* sName)
: Object(CN_WORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD);
  
  m_sName         = sName;
  m_eWordType     = WT_ROUTINE;
  m_eDistributed  = DT_NOPARTICIPANT;
  //m_pDic        = pDic;
  m_pKind         = gpWord ? gpWord : this;
  m_iEpoch        = gpAttachMap->m_iEpoch;
  m_pParticipant  = nullptr;
  m_bIndirect     = false;
  m_bImmediate    = false;
  m_iParams       = 0;
  m_sFinalRoutine = ""_Str;
//  m_iDataBytes    = 0;
  m_pPrevWord     = nullptr;
  m_iVariable     = 0;

  m_xBody.vZero();
  
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
//char* Word::pAllot(i8 iBytes)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
//
//  char* aAddress = pAdd<char*>(pFirstCell(),m_iDataBytes);
//  
//  //gpMainDic->vExtendLinear(iBytes);
//  
//  m_iDataBytes += iBytes;
//
//  _setFooter_();
//  
//  _postcondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
//  
//  return aAddress;
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* Word::pAllot(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  char* aAddress = m_xBody.aAllot(nBytes);
  
  _setFooter_();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* append a pointer to cell                                                   */
/*----------------------------------------------------------------------------*/
void Word::vAppend(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* aAddress = pAppend(pCell);
  ASSERT(aAddress);
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
  
  void* aAddress = pAppend(iInt);
  ASSERT(aAddress);
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

  void* aAddress = pAppend(pValue);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void* Word::pAppend(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  i8 iSize = ValLib::iSize(pValue);
  
  char* pValueAddress = pAllot(iSize);
  
  MemLib::vCopy(pValueAddress,pValue,iSize);
  
  return pValueAddress;
}

/*----------------------------------------------------------------------------*/
/* append address of pointer to task step                                     */
/*----------------------------------------------------------------------------*/
void Word::vAppend(TaskStep* ppCalledWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  void* aAddress = pAppend(ppCalledWord);
  ASSERT(aAddress);
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
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Word::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Word* pStepWord = gpMainDic->pFindStep(m_pEntryStep);
  
  vTrace("Word %p: %s %s Kind %s Indirect %d Immediate %d DataBytes %lld "_Str
         "Step %p %s ..Data.. %p\n"_Str,
         this,
         m_sName.s(),
         aWT[m_eWordType]->a(),
         m_pKind->m_sName.s(),
         m_bIndirect,
         m_bImmediate,
//       m_iDataBytes,
         m_xBody.nUsed(),
         m_pEntryStep,
         pStepWord ? pStepWord->sName().s() : "<not-found>",
         pFirstCell()
         );
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
/* get epoch when word was created                                            */
/*----------------------------------------------------------------------------*/
i8 Word::iEpoch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_iEpoch;
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
//char* Word::aBodyArea(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
//  
//  char* aDataArea = pAdd<char*>(this,sizeof(Word));
//  
//  return pPoolCheck<char*>((void*)aDataArea);
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* Word::aBodyArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  char* aBody = m_xBody.sArea();
  
  return pPoolCheck<char*>((void*)aBody);
}

/*----------------------------------------------------------------------------*/
/* get latter part (current logical end) of body                              */
/*----------------------------------------------------------------------------*/
//char* Word::aBodyLatter(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
//  
//  char* pDataEnd = pAdd<char*>(this,sizeof(Word) + m_iDataBytes);
//  
//  return pPoolCheck<char*>((void*)pDataEnd);
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* Word::aBodyLatter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  char* aLatter = m_xBody.sLatter();
  
  return pPoolCheck<char*>((void*)aLatter);
}

/*----------------------------------------------------------------------------*/
/* get number of data bytes                                                   */
/*----------------------------------------------------------------------------*/
//i8 Word::nBodyUsed(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
//  
//  return m_iDataBytes;
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
i8 Word::nBodyUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_xBody.nUsed();
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
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Word::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_WORD);
  ASSERT(pObject->eClass() == CN_WORD);
  
  Word* pWord = reinterpret_cast<Word*>(pObject);
  
  // dump only the objects
  if (WT_OBJECT == pWord->m_eWordType)
    pWord->vObjectDump(aTab);
  
  //for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* encode call to named word                                                  */
/*----------------------------------------------------------------------------*/
void Word::vEncode(Str* aWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Word* pCalledWord = gpMainDic->pFindWord(aWord);
  ASSERT(pCalledWord);
  
  
  
  vAppend(pCalledWord->ppEntryStep());
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a cell* argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncode(Str* aWord, Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWord);
  
  vAppend(pCell);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a value argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncode(Str* aWord, Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  vEncode(aWord);
    
  vAppend(pValue);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a word* argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncode(Str* aWord, Word* pWord)
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
/* set the final routine (invoked after all participants have responded)      */
/*----------------------------------------------------------------------------*/
void Word::vFinalRoutine(Str* pRoutine)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_sFinalRoutine = (Str*)pRoutine;
}

/*----------------------------------------------------------------------------*/
/* set the final routine (invoked after all participants have responded)      */
/*----------------------------------------------------------------------------*/
char* Word::pFinalRoutine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_sFinalRoutine.s();
}

/*----------------------------------------------------------------------------*/
/* get pointer to first cell in data area                                     */
/*----------------------------------------------------------------------------*/
//Cell* Word::pFirstCell(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
//
//  Cell* pFirst = pAdd<Cell*>((void*)this,sizeof(Word));
//                                         
//  return pPoolCheck<Cell*>(pFirst);
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Cell* Word::pFirstCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* pFirst = reinterpret_cast<Cell*>(m_xBody.sArea());
  
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
/* set word as indirect type                                                  */
/*----------------------------------------------------------------------------*/
void Word::vIndirect(bool bIndirect)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_bIndirect = bIndirect;
}

/*----------------------------------------------------------------------------*/
/* set distributed type                                                       */
/*----------------------------------------------------------------------------*/
bool Word::bIndirect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_bIndirect;
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
bool Word::bIsName(StrVal& sName)
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
//Cell* Word::pLastCell(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
//  
//  Cell* pLast = pAdd<Cell*>((void*)this,sizeof(Word) + m_iDataBytes);
//  
//  return pPoolCheck<Cell*>(pLast);
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Cell* Word::pLastCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* pLast = reinterpret_cast<Cell*>(m_xBody.sLatter());
  
  return pPoolCheck<Cell*>(pLast);
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
StrVal& Word::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* dump object word                                                           */
/*----------------------------------------------------------------------------*/
void Word::vObjectDump(Str* aTab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vTrace("epoch %lld %s (%s) %s %s %s DataBytes %lld "_Str
         "Step %p ..Data.. %p\n"_Str,
         m_iEpoch,
         m_sName.s(),
         m_pDic->sName().s(),
         aWT[m_eWordType]->a(),
         m_bIndirect ? "Indirect"_Str : "Not Indirect"_Str,
         m_bImmediate ? "Immediate"_Str : "Not Immediate"_Str,
//       m_iDataBytes,
         m_xBody.nUsed(),
         m_pEntryStep,
         pFirstCell()
         );
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
//i8 Word::iSize(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
//  
//  i8 nBytes = sizeof(Word) - 1 + m_iDataBytes;
//
//  return nBytes;
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//i8 Word::iSize(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
//  
//  i8 nBytes = sizeof(Word);
//  
//  return nBytes;
//}

/*----------------------------------------------------------------------------*/
/* get variable number                                                        */
/*----------------------------------------------------------------------------*/
i8 Word::iVariable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_iVariable;
}

/*----------------------------------------------------------------------------*/
/* set variable number                                                        */
/*----------------------------------------------------------------------------*/
void Word::vVariable(i8 iVariable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_iVariable = iVariable;
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
//Word* Word::pToWord(void* pData)
//{
//  // static function
//  
//  Word* pWord = _pWord( (char*)(pData) - sizeof(Word) );
//  
//  ASSERT(pWord->eClass() == CN_WORD);
//  
//  return pWord;
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Word* Word::pToWord(void* pBody)
{
  // static function
  
  Word* pWord = pAdd<Word*>(pBody,
                            -( sizeof(Word)
                              - sizeof(WspFxd<C_SIZE_MEDIUM_OBJECT - C_1KB>)
                              - sizeof(Footer)
                              )
                            );
  
  ASSERT(pWord->eClass() == CN_WORD);
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
//Footer* Word::pFooter(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD);
//  
//  Footer* pFooter = pAdd<Footer*>(this,sizeof(Word)+m_iDataBytes);
//
//  return pFooter;
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Footer* Word::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
