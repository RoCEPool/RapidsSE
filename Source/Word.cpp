/*============================================================================*/
/* Word.cpp -- Forth SingleWord in Dictionary                                  */
/*============================================================================*/

#include "Forth.h"

extern StepFp gfpEntryStep;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Word* Forth::_pWord(void* aAddr)
{
  Word* pWord = reinterpret_cast<Word*>(aAddr);
  
  RASSERT_TYPE(pWord,CN_WORD);

  return pWord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Word::operator new(size_t, Dic* pDic)
{
  Word* pWord = (Word*)(HeapMem::pAllocate(CN_WORD));
  
  pWord->m_bPersistent = false;
  pWord->m_pDic = pDic;
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Word::operator new(size_t, Dic* pDic, bool bPersistent)
{
  RASSERT(bPersistent || ! bPersistent);
  
  Word* pWord = (Word*)(HeapMem::pAllocate(CN_WORD,bPersistent));
  
  pWord->m_bPersistent = bPersistent;
  pWord->m_pDic = pDic;
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Word::operator delete(void* pWordAddress)
{
  Word* pWord = reinterpret_cast<Word*>(pWordAddress);
 
  HeapMem::vFree(pWord,pWord->m_bPersistent);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Word::Word(Stz* szName)
: Object(CN_WORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD);
  
  m_pArticle      = new Article(szName,this);

  m_sName         = szName;
  m_eWordType     = WT_ROUTINE;
  //m_bPersistent   = false;
  m_iTrace        = 0;
  //m_pDic        = pDic;
  m_pKind         = AM::pRootWord() ? AM::pRootWord() : this;
  m_iEpoch        = AM::iEpoch();
  m_bImmediate    = false;
  m_iParams       = 0;
  m_bPrompt       = false;
  m_iLocalNo      = 0;
  
  m_pEnmSet       = nullptr;

  m_xRoutine.vZero(CN_WORD);
  
  _setFooter_();
  
  m_fpEntryStep   = gfpEntryStep;
  m_aEntryCell    = aFirstCell();

  _checkInitialised_(CN_WORD);
  
  if (m_pDic)
    m_pDic->vAddWord(this);
  else
    vDebugPoint();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Word::~Word(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  // invoke the finalize function for embedded objects
  if (m_eWordType == WT_OBJECT) {
    Object* pEmbObject = reinterpret_cast<Object*>(m_xRoutine.ayArea());
    eCN eClass = pEmbObject->eClass();
    pFINALIZE* pFinalizeFun = gpFinalize[eClass];
    (pFinalizeFun)(eClass, pEmbObject);
  }
  
  delete m_pEnmSet;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Word::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Word* pWord = _pWord(pObject);
  
  // dump only the objects
  if (WT_OBJECT == pWord->m_eWordType)
    pWord->vObjectDump(aTab);
  
  //for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* get the outer word of this data ptr                                        */
/*----------------------------------------------------------------------------*/
Word* Word::pToWord(void* pBody)
{
  // static function
  
  Word* pWord = (Word*)aAdd<Word*>(pBody,
                                   -( sizeof(Word)
                                      - sizeof(Wsp_t<C_WSP_WORD>)
                                      - sizeof(Footer)
                                    )
                                  );
  
  ASSERT(pWord->eClass() == CN_WORD);
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot data space                                                           */
/*----------------------------------------------------------------------------*/
byte* Word::ayAllot(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  byte* ayAddress = m_xRoutine.ayAllot(nBytes);
  
  _setFooter_();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return ayAddress;
}

/*----------------------------------------------------------------------------*/
/* append a pointer to cell                                                   */
/*----------------------------------------------------------------------------*/
void Word::vAppend(Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* aAddress = aAppend(aCell);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append a pointer to cell                                                   */
/*----------------------------------------------------------------------------*/
void* Word::aAppend(Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* aAllottedCell = (Cell*)ayAllot(sizeof(aCell));
  
  aAllottedCell->m_aJumpTo = aCell;
  
  return aAllottedCell;
}

/*----------------------------------------------------------------------------*/
/* append an i8 integer                                                       */
/*----------------------------------------------------------------------------*/
void Word::vAppend(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* aAddress = aAppend(iInt);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append an i8 integer                                                       */
/*----------------------------------------------------------------------------*/
void* Word::aAppend(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  i8* aAllottedInt = (i8*)ayAllot(sizeof(i8));
  
  *aAllottedInt = iInt;
  
  return aAllottedInt;
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void Word::vAppend(GenVar* pGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* aAddress = aAppend(pGenVar);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void* Word::aAppend(GenVar* pGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* aValueAddress = aAppend(pGenVar->aNestedValue());
  
  return aValueAddress;
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void Word::vAppend(ForthValue* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  void* aAddress = aAppend(aValue);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void* Word::aAppend(ForthValue* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  i8 iSize = aValue->nSize();
  
  char* aValueAddress = (char*)ayAllot(iSize);
  
  MemLib::vCopy(aValueAddress,aValue,iSize,CN_WORD);
  
  return aValueAddress;
}

/*----------------------------------------------------------------------------*/
/* append address of pointer to task step                                     */
/*----------------------------------------------------------------------------*/
void Word::vAppend(StepFp* afpStepFp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  void* aAddress = aAppend(afpStepFp);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append address of pointer to task step                                     */
/*----------------------------------------------------------------------------*/
void* Word::aAppend(StepFp* afpStepFp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  StepFp** aafpStepFp = (StepFp**)ayAllot(sizeof(StepFp*));

  *aafpStepFp = afpStepFp;

  return aafpStepFp;
}

/*----------------------------------------------------------------------------*/
/* get article                                                                */
/*----------------------------------------------------------------------------*/
Article* Word::pArticle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pArticle;
}

/*----------------------------------------------------------------------------*/
/* set article                                                                */
/*----------------------------------------------------------------------------*/
void Word::vArticle(Article* pArticle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pArticle = pArticle;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Word::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Dic* pRootDic = AM::pRootDic();
  Word* pStepWord = pRootDic->pFindStep(m_fpEntryStep);
  
  vTrace("Word %p: %s %s Kind %s Immediate %d DataBytes %lld "_Stz
         "Step %p %s ..Rows.. %p\n"_Stz,
         this,
         m_sName.aChar(),
         aWT[m_eWordType]->aChar(),
         m_pKind->m_sName.aChar(),
         m_bImmediate,
         m_xRoutine.nUsed(),
         m_fpEntryStep,
         pStepWord ? pStepWord->sName().aChar() : "<not-found>",
         aFirstCell()
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
StepFp Word::fpEntryStep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_fpEntryStep;
}

/*----------------------------------------------------------------------------*/
/* set entry step                                                             */
/*----------------------------------------------------------------------------*/
void Word::vEntryStep(StepFp fpStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_fpEntryStep = fpStep;
}

/*----------------------------------------------------------------------------*/
/* get which cell to start executing from                                     */
/*----------------------------------------------------------------------------*/
Cell* Word::aEntryCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_aEntryCell;
}

/*----------------------------------------------------------------------------*/
/* set which cell to start executing from                                     */
/*----------------------------------------------------------------------------*/
void Word::vEntryCell(Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_aEntryCell = aCell;
}

/*----------------------------------------------------------------------------*/
/* get address of this words entry step                                       */
/*----------------------------------------------------------------------------*/
StepFp* Word::afpEntryStep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return &m_fpEntryStep;
}

/*----------------------------------------------------------------------------*/
/* encode call to named word                                                  */
/*----------------------------------------------------------------------------*/
void Word::vEncode(Stz* aWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Dic* pRootDic = AM::pRootDic();
  Word* pCalledWord = pRootDic->pFindWord(aWordNm);
  ASSERT(pCalledWord);
  
  vAppend(pCalledWord->afpEntryStep());
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a cell* argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncode(Stz* aWordNm, Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWordNm);
  
  vAppend(aCell);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a value argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncode(Stz* aWordNm, GenVar* pGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWordNm);
  
  vAppend(pGenVar);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a word* argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncode(Stz* aWordNm, Word* pCalledWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  GenVar xVar;
  xVar = pCalledWord;
  
  vEncode(aWordNm,&xVar);
}

/*----------------------------------------------------------------------------*/
/* encode call of entry step                                                  */
/*----------------------------------------------------------------------------*/
void Word::vEncode(StepFp* afpEntryStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vAppend(afpEntryStep);
}

/*----------------------------------------------------------------------------*/
/* get ename set                                                              */
/*----------------------------------------------------------------------------*/
EnmSet* Word::pEnmSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pEnmSet;
}

/*----------------------------------------------------------------------------*/
/* set ename set                                                              */
/*----------------------------------------------------------------------------*/
void Word::vEnmSet(EnmSet* pEnmSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pEnmSet = pEnmSet;
}

/*----------------------------------------------------------------------------*/
/* get exit hook                                                              */
/*----------------------------------------------------------------------------*/
Stz* Word::szExitHook(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_sExitHook.aStz();
}

/*----------------------------------------------------------------------------*/
/* set exit hook                                                              */
/*----------------------------------------------------------------------------*/
void Word::vExitHook(Stz* szExitHook)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_sExitHook = szExitHook;
}

/*----------------------------------------------------------------------------*/
/* get pointer to first cell in data area                                     */
/*----------------------------------------------------------------------------*/
Cell* Word::aFirstCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* aFirst = reinterpret_cast<Cell*>(m_xRoutine.ayArea());
  
  return pPoolCheck<Cell*>(aFirst);
}

/*----------------------------------------------------------------------------*/
/* does this word have given cell?                                            */
/*----------------------------------------------------------------------------*/
bool Word::bHasCell(Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  if (aCell >= aFirstCell() && aCell <= aLastCell())
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
bool Word::bIsName(s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  bool b = m_sName.bSame(sName);
  
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
Cell* Word::aLastCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* aLast = reinterpret_cast<Cell*>(m_xRoutine.ayLatter());
  
  return pPoolCheck<Cell*>(aLast);
}

/*----------------------------------------------------------------------------*/
/* get local  number                                                          */
/*----------------------------------------------------------------------------*/
i8 Word::iLocalNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_iLocalNo;
}

/*----------------------------------------------------------------------------*/
/* set local number                                                           */
/*----------------------------------------------------------------------------*/
void Word::vLocalNo(i8 iLocalNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_iLocalNo = iLocalNo;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
s32& Word::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name                                                                   */
/*----------------------------------------------------------------------------*/
void Word::vName(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_sName = szName;
  
  m_pArticle->vName(szName);
}

/*----------------------------------------------------------------------------*/
/* dump object word                                                           */
/*----------------------------------------------------------------------------*/
void Word::vObjectDump(Stz* aTab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vTrace("epoch %lld %s (%s) %s %s DataBytes %lld "_Stz
         "Step %p ..Rows.. %p\n"_Stz,
         m_iEpoch,
         m_sName.aChar(),
         m_pDic ? m_pDic->szName() : C_NO_NAME,
         aWT[m_eWordType]->aChar(),
         m_bImmediate ? "Immediate"_Stz : "Not Immediate"_Stz,
         m_xRoutine.nUsed(),
         m_fpEntryStep,
         aFirstCell()
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

  Article* pPrevArticle = m_pArticle->pPrevItem();
  if (! pPrevArticle)
    return nullptr;
  
  Word* pPrevWord = _pWord(pPrevArticle->pObject());
  
  return pPrevWord;
}

/*----------------------------------------------------------------------------*/
/* get prompt flag                                                            */
/*----------------------------------------------------------------------------*/
bool Word::bPrompt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_bPrompt;
}

/*----------------------------------------------------------------------------*/
/* set prompt flag                                                            */
/*----------------------------------------------------------------------------*/
void Word::vPrompt(bool bPrompt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_bPrompt = bPrompt;
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
/* get ptr to data area                                                       */
/*----------------------------------------------------------------------------*/
char* Word::aWspArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  char* aBody = (char*)m_xRoutine.ayArea();
  
  return pPoolCheck<char*>((void*)aBody);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Word::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
