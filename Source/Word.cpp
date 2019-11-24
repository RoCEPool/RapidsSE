/*============================================================================*/
/* Word.cpp -- Forth Word in Dictionary                                       */
/*============================================================================*/

#include "SenVM.h"

extern StepFp gfpEntryStep;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Word* SenVM::_pWord(void* aAddr)
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
Word::Word(Stz* szWordNm)
: Object(CN_WORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD);
  
  //vTrace("WORD: %s\n"_Stz,szWordNm);
  
  m_pArticle   = new Article(szWordNm,this,Word::vArtFun);

  m_sWordNm    = szWordNm;
  m_eKind      = WK_ROUTINE;
  //m_bPersistent   = false;
  m_iTrace     = 0;
  //m_pDic     = pDic;
  m_pCategory      = AM::pRootWord() ? AM::pRootWord() : this;
  m_iEpoch     = AM::iEpoch();
  m_bImmediate = false;
  m_iParams    = 0;
  m_bPrompt    = false;
  m_iLocalNo   = 0;
  
  m_pEnameSet    = nullptr;

  m_fpIntrinsic = gfpEntryStep;
  m_aInxPtr  = aFirstCell();
  
  m_xRoutine.vZero(CN_WORD);
  
  _setFooter_();
  
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
  if (m_eKind == WK_OBJECT) {
    Object* pEmbObject = reinterpret_cast<Object*>(m_xRoutine.ayArea());
    eCN eClass = pEmbObject->eClass();
    pFINALIZE* pFinalizeFun = gpFinalize[eClass];
    (pFinalizeFun)(eClass, pEmbObject);
  }
  
  delete m_pEnameSet;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* article print function                                                     */
/*----------------------------------------------------------------------------*/
void Word::vArtFun(Object* pObject, SP& spObject)
{
  SP_Word spWord;
  
  spObject.vResult(spWord.sz(_pWord(pObject)));
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Word::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Word* pWord = _pWord(pObject);
  
  // dump only the objects
  if (WK_OBJECT == pWord->m_eKind)
    pWord->vObjectDump(aTab);
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
  
  void* aValueAddress = aAppend(pGenVar->aEmbeddedStkVal());
  
  return aValueAddress;
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void Word::vAppend(StkVal* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  void* aAddress = aAppend(aValue);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void* Word::aAppend(StkVal* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  i8 iSize = aValue->nStackBytes();
  
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
/* get word kind                                                              */
/*----------------------------------------------------------------------------*/
Word* Word::pCategory(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pCategory;
}

/*----------------------------------------------------------------------------*/
/* set word kind                                                              */
/*----------------------------------------------------------------------------*/
void Word::vCategory(Word* pCategory)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pCategory = pCategory;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Word::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Dic* pRootDic = AM::pRootDic();
  Word* pStepWord = pRootDic->pFindStep(m_fpIntrinsic);
  
  vTrace("Word %p: %s %s Category %s Immediate %d DataBytes %lld "_Stz
         "Step %p %s ..Rows.. %p\n"_Stz,
         this,
         m_sWordNm.azChar(),
         aWK[m_eKind]->azChar(),
         m_pCategory->m_sWordNm.azChar(),
         m_bImmediate,
         m_xRoutine.nUsed(),
         m_fpIntrinsic,
         pStepWord ? pStepWord->szWordNm() : "<not-found>"_Stz,
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
  
  return m_fpIntrinsic;
}

/*----------------------------------------------------------------------------*/
/* set entry step                                                             */
/*----------------------------------------------------------------------------*/
void Word::vEntryStep(StepFp fpIntrinsic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_fpIntrinsic = fpIntrinsic;
}

/*----------------------------------------------------------------------------*/
/* get which cell to start executing from                                     */
/*----------------------------------------------------------------------------*/
Cell* Word::aEntryCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_aInxPtr;
}

/*----------------------------------------------------------------------------*/
/* set which cell to start executing from                                     */
/*----------------------------------------------------------------------------*/
void Word::vEntryCell(Cell* aInxPtr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_aInxPtr = aInxPtr;
}

/*----------------------------------------------------------------------------*/
/* get address of this words entry step                                       */
/*----------------------------------------------------------------------------*/
StepFp* Word::afpEntryStep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return &m_fpIntrinsic;
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
EnameSet* Word::pEnameSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pEnameSet;
}

/*----------------------------------------------------------------------------*/
/* set ename set                                                              */
/*----------------------------------------------------------------------------*/
void Word::vEnameSet(EnameSet* pEnameSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pEnameSet = pEnameSet;
}

/*----------------------------------------------------------------------------*/
/* get exception hook                                                         */
/*----------------------------------------------------------------------------*/
Stz* Word::szExceptionHook(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_sExceptionHook.szStz();
}

/*----------------------------------------------------------------------------*/
/* set exception hook                                                         */
/*----------------------------------------------------------------------------*/
void Word::vExceptionHook(Stz* szExceptionHook)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_sExceptionHook = szExceptionHook;
}

/*----------------------------------------------------------------------------*/
/* get exit hook                                                              */
/*----------------------------------------------------------------------------*/
Stz* Word::szExitHook(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_sExitHook.szStz();
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
  
  bool b = m_sWordNm.bSame(sName);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get word kind                                                              */
/*----------------------------------------------------------------------------*/
eWK Word::eKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_eKind;
}

/*----------------------------------------------------------------------------*/
/* set word kind                                                              */
/*----------------------------------------------------------------------------*/
void Word::vKind(eWK eKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_eKind = eKind;
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
/* dump object word                                                           */
/*----------------------------------------------------------------------------*/
void Word::vObjectDump(Stz* aTab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vTrace("epoch %lld %s (%s) %s %s DataBytes %lld "_Stz
         "Step %p ..Rows.. %p\n"_Stz,
         m_iEpoch,
         m_sWordNm.azChar(),
         m_pDic ? m_pDic->szName() : C_NO_NAME,
         aWK[m_eKind]->azChar(),
         m_bImmediate ? "Immediate"_Stz : "Not Immediate"_Stz,
         m_xRoutine.nUsed(),
         m_fpIntrinsic,
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
/* see word contents                                                          */
/*----------------------------------------------------------------------------*/
void Word::vSee(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* get word name                                                              */
/*----------------------------------------------------------------------------*/
Stz* Word::szWordNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_sWordNm;
}

/*----------------------------------------------------------------------------*/
/* set name                                                                   */
/*----------------------------------------------------------------------------*/
void Word::vWordNm(Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_sWordNm = szWordNm;
  
  m_pArticle->vName(szWordNm);
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
__aFooterFunction(Word)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
