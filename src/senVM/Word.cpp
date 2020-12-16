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

#ifdef WITH_DEBUG
  if (szWordNm->bSame("word"_Stz)) {
    vDebugPoint();
    RASSERT(m_pDic);
    RASSERT(! m_pDic->pLastWord());
  }
#endif
  
  m_pArticle   = new Article(szWordNm,this,Word::vArtFun);

  m_sWordNm    = szWordNm;
  RASSERT(m_sWordNm.bSame(szWordNm));
  
  m_eWord      = WK_ROUTINE;

  m_bBreakPt   = false;
  m_pCategory  = AM::pRootWord() ? AM::pRootWord() : this;
  //m_pDic     = pDic;
  m_iEpoch     = AM::iEpoch();
  m_bImmediate = false;
  m_pInclude   = nullptr;
  m_iLocalNo   = 0;
  m_pObject    = nullptr;
  m_iParams    = 0;
  //m_bPersistent   = false;
  m_iTrace     = 0;
  m_pWrdTagSet = nullptr;

  m_pRoutine    = new Routine(this);
  
  m_fpIntrinsic = gfpEntryStep;
  m_aInxPtr     = aFirstCell();

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
    
  delete m_pArticle;
  delete m_pWrdTagSet;
  
  delete m_pRoutine;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* article print function                                                     */
/*----------------------------------------------------------------------------*/
void Word::vArtFun(Object* pObject, SP& spObject)
{
  _static_
  
  SP_Word spWord;
  
  spObject.vResult(spWord.sz(_pWord(pObject)));
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Word::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  _static_
  
  Word* pWord = _pWord(pObject);
}

/*----------------------------------------------------------------------------*/
/* get the outer word of given address (which should be within some word)     */
/*----------------------------------------------------------------------------*/
Word* Word::pToWord(void* aEmbeddedObject)
{
  _static_
    
  Word* pWord = Routine::pToWord((ForthCell*)aEmbeddedObject);
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot bytes in routine                                                     */
/*----------------------------------------------------------------------------*/
byte* Word::ayAllot(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  byte* ayAddress = m_pRoutine->ayAllot(nBytes);
  
  _setFooter_();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return ayAddress;
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
/* get break point                                                            */
/*----------------------------------------------------------------------------*/
bool Word::bBreakPt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_bBreakPt;
}

/*----------------------------------------------------------------------------*/
/* set break point                                                            */
/*----------------------------------------------------------------------------*/
void Word::vBreakPt(bool bBreakPt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_bBreakPt = bBreakPt;
}

/*----------------------------------------------------------------------------*/
/* get word category                                                          */
/*----------------------------------------------------------------------------*/
Word* Word::pCategory(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pCategory;
}

/*----------------------------------------------------------------------------*/
/* set word category                                                          */
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
  
  vTrace("Word %s %p Kind %s; Category %s; %s; DataBytes %lld; "_Stz
         "Step %p %s ..Rows.. %p\n"_Stz,
         m_sWordNm.azChar(),
         this,
         aWK[m_eWord]->azChar(),
         m_pCategory->m_sWordNm.azChar(),
         m_bImmediate ? "immediate" : "not-immediate",
         m_pRoutine->nWspUsed(),
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
/* encode a cell* argument                                                    */
/*----------------------------------------------------------------------------*/
void* Word::aEncodeArg(ForthCell* aArgCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  ForthCell* aAllottedCell = (ForthCell*)m_pRoutine->aAppend(aArgCell);
  
  return aAllottedCell;
}

/*----------------------------------------------------------------------------*/
/* encode call of entry step                                                  */
/*----------------------------------------------------------------------------*/
void Word::vEncodeCall(StepFp* afpEntryStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pRoutine->vAppend(afpEntryStep);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word                                                  */
/*----------------------------------------------------------------------------*/
void Word::vEncodeCall(Stz* aWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Dic* pRootDic = AM::pRootDic();
  Word* pCalledWord = pRootDic->pFindWord(aWordNm);
  ASSERT(pCalledWord);
  
  m_pRoutine->vAppend(pCalledWord->afpEntryStep());
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a cell* argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncodeJump(Stz* aWordNm, ForthCell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncodeCall(aWordNm);
  
  m_pRoutine->vAppend(aCell);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a value argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncodePush(AnyVal* aAnyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  eCN eClassNo_val = aAnyVal->eClass();
  
  if (CN_FIPVAL == eClassNo_val)
    vEncodeCall("pushFipVal"_Stz);
  else if (CN_INTVAL == eClassNo_val)
    vEncodeCall("pushIntVal"_Stz);
  else if (CN_PTRVAL == eClassNo_val)
    vEncodeCall("pushPtrVal"_Stz);
  else if (CN_STZVAL == eClassNo_val)
    vEncodeCall("pushStzVal"_Stz);
  else {
    NEVER_REACH;
  }

  m_pRoutine->vAppend(aAnyVal);
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
ForthCell* Word::aEntryCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_aInxPtr;
}

/*----------------------------------------------------------------------------*/
/* set which cell to start executing from                                     */
/*----------------------------------------------------------------------------*/
void Word::vEntryCell(ForthCell* aInxPtr)
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
/* get epoch when word was created                                            */
/*----------------------------------------------------------------------------*/
i8 Word::iEpoch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_iEpoch;
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
ForthCell* Word::aFirstCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  ForthCell* aFirst = reinterpret_cast<ForthCell*>(m_pRoutine->ayWspArea());
  
  return pPoolCheck<ForthCell*>(aFirst);
}

/*----------------------------------------------------------------------------*/
/* does this word have given cell?                                            */
/*----------------------------------------------------------------------------*/
bool Word::bHasCell(ForthCell* aCell)
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
/* get word include                                                           */
/*----------------------------------------------------------------------------*/
Word* Word::pInclude(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pInclude;
}

/*----------------------------------------------------------------------------*/
/* set word category                                                          */
/*----------------------------------------------------------------------------*/
void Word::vInclude(Word* pInclude)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pInclude = pInclude;
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
/* get pointer to last cell                                                   */
/*----------------------------------------------------------------------------*/
ForthCell* Word::aLastCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  ForthCell* aLast = reinterpret_cast<ForthCell*>(m_pRoutine->ayWspLatter());
  
  return pPoolCheck<ForthCell*>(aLast);
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
/* get object                                                                 */
/*----------------------------------------------------------------------------*/
Object* Word::pObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pObject;
}

/*----------------------------------------------------------------------------*/
/* set object                                                                 */
/*----------------------------------------------------------------------------*/
void Word::vObject(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pObject = pObject;
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
         m_pDic ? m_pDic->szDicNm() : C_NO_NAME,
         aWK[m_eWord]->azChar(),
         m_bImmediate ? "Immediate"_Stz : "Not Immediate"_Stz,
         m_pRoutine->nWspUsed(),
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
/* reset routine                                                              */
/*----------------------------------------------------------------------------*/
void Word::vResetRoutine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pRoutine->vReset();
}

/*----------------------------------------------------------------------------*/
/* get routine                                                                */
/*----------------------------------------------------------------------------*/
Routine* Word::pRoutine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  return m_pRoutine;
}

/*----------------------------------------------------------------------------*/
/* get routine size                                                           */
/*----------------------------------------------------------------------------*/
i8 Word::nRoutineUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  i8 nUsed = m_pRoutine->nWspUsed();
  
  return nUsed;
}

/*----------------------------------------------------------------------------*/
/* get word kind                                                              */
/*----------------------------------------------------------------------------*/
eWK Word::eWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_eWord;
}

/*----------------------------------------------------------------------------*/
/* set word kind                                                              */
/*----------------------------------------------------------------------------*/
void Word::vWord(eWK eWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_eWord = eWord;
}

/*----------------------------------------------------------------------------*/
/* get ename set                                                              */
/*----------------------------------------------------------------------------*/
TagSet* Word::pWrdTagSet(bool bCreateNew)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  if (! m_pWrdTagSet && bCreateNew)
    m_pWrdTagSet = new TagSet;
  
  return m_pWrdTagSet;
}

/*----------------------------------------------------------------------------*/
/* set ename set                                                              */
/*----------------------------------------------------------------------------*/
void Word::vWrdTagSet(TagSet* pWrdTagSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pWrdTagSet = pWrdTagSet;
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
  
  RASSERT(m_sWordNm.bSame(szWordNm));
  
  m_pArticle->vName(szWordNm);
}

/*----------------------------------------------------------------------------*/
/* get ptr to data area                                                       */
/*----------------------------------------------------------------------------*/
char* Word::aWspArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  char* aBody = (char*)m_pRoutine->ayWspArea();
  
  return pPoolCheck<char*>((void*)aBody);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Word)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
