/*============================================================================*/
/* Word.cpp - word in dictionary                                              */
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
  ASSERT(iSize);

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
  
  pFooter()->vSetFooter();
  
  m_xCode.m_pFunc = &Tasks::Task::vEnter;
  m_xBody.m_pCell = pFirstCell();
  
  Memory::vCheckInitialised(this,sizeof(Word));
  
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
/* set addr pointer                                                           */
/*----------------------------------------------------------------------------*/
void Word::vAddr(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_xBody.m_pCell = pCell;
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

  pFooter()->vSetFooter();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return pAddress;
}

/*----------------------------------------------------------------------------*/
/* append word                                                                */
/*----------------------------------------------------------------------------*/
void Word::vAppend(i8 wVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* pAddress = pAppend(wVal);
  ASSERT(pAddress);
}

/*----------------------------------------------------------------------------*/
/* append word                                                                */
/*----------------------------------------------------------------------------*/
void* Word::pAppend(i8 wVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  char* pAddress = pAllot(8);
  
  Memory::vCopy(pAddress,&wVal,8);
  
  return pAddress;
}

/*----------------------------------------------------------------------------*/
/* append cell                                                                */
/*----------------------------------------------------------------------------*/
void Word::vAppend(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* pAddress = pAppend(pCell);
  ASSERT(pAddress);
}

/*----------------------------------------------------------------------------*/
/* append cell                                                                */
/*----------------------------------------------------------------------------*/
void* Word::pAppend(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* pAllottedCell = (Cell*)pAllot(sizeof(pCell));
  
  pAllottedCell->m_pJumpLocn = pCell;
  
  return pAllottedCell;
}

/*----------------------------------------------------------------------------*/
/* append code                                                                */
/*----------------------------------------------------------------------------*/
void Word::vAppend(CodeField* pCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* pAddress = pAppend(pCode);
  ASSERT(pAddress);
}

/*----------------------------------------------------------------------------*/
/* append code                                                                */
/*----------------------------------------------------------------------------*/
void* Word::pAppend(CodeField* pCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* pAllottedCell = (Cell*)pAllot(sizeof(pCode));
  
  pAllottedCell->m_pCodeField = pCode;
  
  return pAllottedCell;
}

/*----------------------------------------------------------------------------*/
/* append value                                                               */
/*----------------------------------------------------------------------------*/
void Word::vAppend(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  void* pAddress = pAppend(pValue);
  ASSERT(pAddress);
}

/*----------------------------------------------------------------------------*/
/* append value                                                               */
/*----------------------------------------------------------------------------*/
void* Word::pAppend(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  i8 iSize = pValue->iSize();
  
  char* pAddress = pAllot(iSize);
  
  Memory::vCopy(pAddress,pValue,iSize);
  
  return pAddress;
}

/*----------------------------------------------------------------------------*/
/* append pointer                                                             */
/*----------------------------------------------------------------------------*/
void Word::vAppend(pFUN pFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  void* pAddress = pAppend(pFunc);
  ASSERT(pAddress);
}

/*----------------------------------------------------------------------------*/
/* append pointer                                                             */
/*----------------------------------------------------------------------------*/
void* Word::pAppend(pFUN pFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  char* pAddress = pAllot(sizeof(pFunc));
  
  Memory::vCopy(pAddress,&pFunc,sizeof(pFunc));
  
  return pAddress;
}

/*----------------------------------------------------------------------------*/
/* append pointer                                                             */
/*----------------------------------------------------------------------------*/
void Word::vAppend(pFUN* ppFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  void* pAddress = pAppend(ppFunc);
  ASSERT(pAddress);
}

/*----------------------------------------------------------------------------*/
/* append pointer                                                             */
/*----------------------------------------------------------------------------*/
void* Word::pAppend(pFUN* ppFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  char* pAddress = pAllot(sizeof(ppFunc));
  
  Memory::vCopy(pAddress,&ppFunc,sizeof(ppFunc));
  
  return pAddress;
}

/*----------------------------------------------------------------------------*/
/* get pointer to code field                                                  */
/*----------------------------------------------------------------------------*/
CodeField* Word::pCodeField(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return &m_xCode;
}

/*----------------------------------------------------------------------------*/
/* get pointer to code field                                                  */
/*----------------------------------------------------------------------------*/
BodyField* Word::pBodyField(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return &m_xBody;
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
/* get func ptr                                                               */
/*----------------------------------------------------------------------------*/
pFUN Word::pFunc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_xCode.m_pFunc;
}

/*----------------------------------------------------------------------------*/
/* set func pointer                                                           */
/*----------------------------------------------------------------------------*/
void Word::vFunc(pFUN pFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_xCode.m_pFunc = pFunc;
}

/*----------------------------------------------------------------------------*/
/* get ptr to body area                                                       */
/*----------------------------------------------------------------------------*/
char* Word::pBodyArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  char* pAddress = pcAdd(this,sizeof(Word));
  
  return pPoolCheck<char*>((void*)pAddress);
}

/*----------------------------------------------------------------------------*/
/* byte dump                                                                  */
/*----------------------------------------------------------------------------*/
void Word::vByteDump(i8 iSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Memory::vHexDump(this, iSize);
}

/*----------------------------------------------------------------------------*/
/* get end of current data                                                    */
/*----------------------------------------------------------------------------*/
char* Word::pDataEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  char* pAddress = reinterpret_cast<char*>(this) + sizeof(Word) + m_iDataBytes;
  
  return pPoolCheck<char*>((void*)pAddress);
}

/*----------------------------------------------------------------------------*/
/* get data bytes                                                             */
/*----------------------------------------------------------------------------*/
i8 Word::iDataBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_iDataBytes;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Word::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  s32 sFuncName;
  
  Word* pFuncWord = gpMainDic->pFindFunc(m_xCode.m_pFunc);
  vTrace("Word %p: %s %s Kind %s Immediate %d DataBytes %lld "
         "Func %p %s ..Data.. %p\n",
         this,
         m_sName.a(),
         pWT[m_eWordType],
         m_pKind->m_sName.a(),
         m_bImmediate,
         m_iDataBytes,
         m_xCode.m_pFunc,
         pFuncWord ? pFuncWord->aName() : "<not-found>",
         pFirstCell()
         );
  }

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Word::vDump(const char* pTab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  s32 sFuncName;
  
  vTrace("%sWord %p: %s %s %s DataBytes %lld "
         "Func %p ..Data.. %p\n",
         pTab,
         this,
         m_sName.a(),
         pWT[m_eWordType],
         m_bImmediate ? "Immediate" : "Not Immediate",
         m_iDataBytes,
         m_xCode.m_pFunc,
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
  
  vAppend(pCalledWord->pCodeField());
}

/*----------------------------------------------------------------------------*/
/* encode call to word                                                        */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord, Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWord);
  
  // needs to be corrected
  vAppend(pCell);
}

/*----------------------------------------------------------------------------*/
/* encode call to word                                                        */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord, CodeField* pCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWord);
  
  // needs to be corrected
  vAppend(pCode);
}

/*----------------------------------------------------------------------------*/
/* encode call to word                                                        */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord, Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  vEncode(aWord);
    
  vAppend(pValue);
}

/*----------------------------------------------------------------------------*/
/* encode call to word                                                        */
/*----------------------------------------------------------------------------*/
void Word::vEncode(const char* aWord, Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWord);
  
//  WrdVal xWord;
//  xWord = pWord;
  
  PtrVal xPtrWord;
  xPtrWord = pWord;
  
  vAppend(&xPtrWord);
}

/*----------------------------------------------------------------------------*/
/* encode call to word                                                        */
/*----------------------------------------------------------------------------*/
void Word::vEncode(CodeField* pCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  // needs to be corrected
  vAppend(pCode);
}

/*----------------------------------------------------------------------------*/
/* get pointer to first cell                                                  */
/*----------------------------------------------------------------------------*/
Cell* Word::pFirstCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  Cell* pFirst = reinterpret_cast<Cell*>(pcAdd((void*)this,sizeof(Word)));
                                         
  return pPoolCheck<Cell*>(pFirst);
}

/*----------------------------------------------------------------------------*/
/* does this word have given cell location?                                   */
/*----------------------------------------------------------------------------*/
bool Word::bHasLocation(Cell* pCell) 
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  if ((char*)pCell >= pBodyArea() && (char*)pCell <= pDataEnd())
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
char* Word::aName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  return pPoolCheck<char*>((void*)m_sName.a());
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
/* set name                                                                   */
/*----------------------------------------------------------------------------*/
void Word::vName(const char* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_sName = aName;
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
  
  return sizeof(Word) - 1 + m_iDataBytes;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Value* Word::pValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Value* pValue = reinterpret_cast<Value*>(pFirstCell());
  
  return pPoolCheck<Value*>(pValue);
}

/*----------------------------------------------------------------------------*/
/* word dump                                                                  */
/*----------------------------------------------------------------------------*/
void Word::vWordDump(i8 iSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Memory::vWordDump(this, iSize);
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
/* convert body ptr to word ptr                                               */
/*----------------------------------------------------------------------------*/
Word* Word::pToWord(void* pBody)
{
  // static function
  
  Word* pWord = reinterpret_cast<Word*>(pcAdd(pBody, - sizeof(Word)));
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