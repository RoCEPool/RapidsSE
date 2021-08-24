/*============================================================================*/
/* Word.cpp -- Forth Word in Dictionary                                       */
/*============================================================================*/

#include "BASE.h"

#include "SP_Word.hpp"

extern fpSTEPFN gfpDoColon;


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Word,CN_WORD)
__vDeleteFunction(Word,CN_WORD)
__aFooterFunction(Word)


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Word::operator new(size_t, Dic* pDic)
{
  Word* pWord = (Word*)(Heaps::pAllocate(CN_WORD));
  
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
  
  Word* pWord = (Word*)(Heaps::pAllocate(CN_WORD,bPersistent));
  
  pWord->m_bPersistent = bPersistent;
  pWord->m_pDic = pDic;
  
  return pWord;
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
  if (szWordNm->bSame("\\word"_Stz)) {
    vDebugPoint();
    RASSERT(m_pDic);
    RASSERT(! m_pDic->pLastWord());
  }
#endif
  
  m_pSkipNode  = nullptr;
  
  m_sWordNm    = szWordNm;
  
  if (! m_sWordNm.bSame(szWordNm)) {
    RASSERT(m_sWordNm.bSame(szWordNm));
  }
  
  m_eWord      = WK_COLON;

  m_bBreakPt   = false;
  m_pCategory  = AM::pWordRoot() ? AM::pWordRoot() : this;
  //m_pDic     = pDic;
  m_iEpoch     = AM::iEpoch();
  m_bImmediate = false;
  m_iLocalNo   = 0;
  m_pObject    = nullptr;
  m_iParams    = 0;
  //m_bPersistent   = false;
  m_iTrace     = 0;
  m_pWrdTagSet = nullptr;

  m_pCodeBlock    = new CodeBlock(this);
  
  m_fpIntrinsic = gfpDoColon;
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
    
  delete m_pWrdTagSet;
  
  delete m_pCodeBlock;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Word::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  _static_
  UNUSED(eClass);
  UNUSED(aTab);

  Word* pWord = _pWord(pObject);
  UNUSED(pWord);
}

/*----------------------------------------------------------------------------*/
/* get the outer word of given address (which should be within some word)     */
/*----------------------------------------------------------------------------*/
Word* Word::pToWord(void* aEmbeddedObject)
{
  _static_
    
  Word* pWord = CodeBlock::pToWord((CodeCell*)aEmbeddedObject);
  
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
  
  byte* ayAddress = m_pCodeBlock->ayAllot(nBytes);
  
  _setFooter_();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return ayAddress;
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
  
  Dic* pDicRoot = AM::DIC::pRoot();
  Word* pStepWord = pDicRoot->pFindStep(m_fpIntrinsic);
  
  vTrace("Word %s %p Kind %s; Category %s; %s; DataBytes %lld; "_Stz
         "Step %p %s ..Rows.. %p\n"_Stz,
         m_sWordNm.azChar(),
         this,
         aWK[m_eWord]->azChar(),
         m_pCategory->m_sWordNm.azChar(),
         m_bImmediate ? "immediate" : "not-immediate",
         m_pCodeBlock->nWspUsed(),
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
void* Word::aEncodeArg(CodeCell* aArgCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  CodeCell* aAllottedCell = (CodeCell*)m_pCodeBlock->aAppend(aArgCell);
  
  return aAllottedCell;
}

/*----------------------------------------------------------------------------*/
/* encode call of entry step                                                  */
/*----------------------------------------------------------------------------*/
void Word::vEncodeCall(fpSTEPFN* afpEntryStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pCodeBlock->vAppend(afpEntryStep);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word                                                  */
/*----------------------------------------------------------------------------*/
void Word::vEncodeCall(Stz* aWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Dic* pDicRoot = AM::DIC::pRoot();
  Word* pCalledWord = pDicRoot->pFindWord(aWordNm);
  ASSERT(pCalledWord);
  
  m_pCodeBlock->vAppend(pCalledWord->afpEntryStep());
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a cell* argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncodeJump(Stz* aWordNm, CodeCell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncodeCall(aWordNm);
  
  m_pCodeBlock->vAppend(aCell);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a value argument                       */
/*----------------------------------------------------------------------------*/
void Word::vEncodePush(TypedCell* aTypedCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  eCN eClassNo_val = aTypedCell->eType();
  
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

  m_pCodeBlock->vAppend(aTypedCell);
}

/*----------------------------------------------------------------------------*/
/* get entry step                                                             */
/*----------------------------------------------------------------------------*/
fpSTEPFN Word::fpEntryStep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_fpIntrinsic;
}

/*----------------------------------------------------------------------------*/
/* set entry step                                                             */
/*----------------------------------------------------------------------------*/
void Word::vEntryStep(fpSTEPFN fpIntrinsic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_fpIntrinsic = fpIntrinsic;
}

/*----------------------------------------------------------------------------*/
/* get which cell to start executing from                                     */
/*----------------------------------------------------------------------------*/
CodeCell* Word::aEntryCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_aInxPtr;
}

/*----------------------------------------------------------------------------*/
/* set which cell to start executing from                                     */
/*----------------------------------------------------------------------------*/
void Word::vEntryCell(CodeCell* aInxPtr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_aInxPtr = aInxPtr;
}

/*----------------------------------------------------------------------------*/
/* get address of this words entry step                                       */
/*----------------------------------------------------------------------------*/
fpSTEPFN* Word::afpEntryStep(void)
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
CodeCell* Word::aFirstCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  CodeCell* aFirst = reinterpret_cast<CodeCell*>(m_pCodeBlock->ayWspArea());
  
  return pPoolCheck<CodeCell*>(aFirst,__func__);
}

/*----------------------------------------------------------------------------*/
/* does this word have given cell?                                            */
/*----------------------------------------------------------------------------*/
bool Word::bHasCell(CodeCell* aCell)
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
/* get pointer to last cell                                                   */
/*----------------------------------------------------------------------------*/
CodeCell* Word::aLastCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  CodeCell* aLast = reinterpret_cast<CodeCell*>(m_pCodeBlock->ayWspLatter());
  
  return pPoolCheck<CodeCell*>(aLast,__func__);
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
/* get next word in dic                                                       */
/*----------------------------------------------------------------------------*/
Word* Word::pNextWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Word* pNextWord = nullptr;
  
  SkipNode* pSkipNode = m_pSkipNode->m_pRight[0];
  if (pSkipNode)
    pNextWord = _pWord(pSkipNode->m_pObject);

  return pNextWord;
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
  UNUSED(aTab);
  
  vTrace("epoch %lld %s (%s) %s %s DataBytes %lld "_Stz
         "Step %p ..Rows.. %p\n"_Stz,
         m_iEpoch,
         m_sWordNm.azChar(),
         m_pDic ? m_pDic->szDicNm() : C_NO_NAME,
         aWK[m_eWord]->azChar(),
         m_bImmediate ? "Immediate"_Stz : "Not Immediate"_Stz,
         m_pCodeBlock->nWspUsed(),
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

  
  return pNextWord();
}

/*----------------------------------------------------------------------------*/
/* reset routine                                                              */
/*----------------------------------------------------------------------------*/
void Word::vResetCodeBlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pCodeBlock->vReset();
}

/*----------------------------------------------------------------------------*/
/* get routine                                                                */
/*----------------------------------------------------------------------------*/
CodeBlock* Word::pCodeBlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  return m_pCodeBlock;
}

/*----------------------------------------------------------------------------*/
/* get routine size                                                           */
/*----------------------------------------------------------------------------*/
i8 Word::nCodeBlockUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  i8 nUsed = m_pCodeBlock->nWspUsed();
  
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

}

/*----------------------------------------------------------------------------*/
/* get ptr to data area                                                       */
/*----------------------------------------------------------------------------*/
char* Word::aWspArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  char* aBody = (char*)m_pCodeBlock->ayWspArea();
  
  return pPoolCheck<char*>((void*)aBody,__func__);
}







/*----------------------------------------------------------------------------*/
/* compare two words (in a skiplist) by their word names                      */
/*----------------------------------------------------------------------------*/
bool bWordCompare(void* aWordBeingInserted, void* aWordScan, bool& bMatching)
{
  Word* pWordBeingInserted = _pWord(aWordBeingInserted);
  Word* pWordScan          = _pWord(aWordScan);
    
  if ( ! pWordScan )
    return false;
  
  if (pWordBeingInserted->szWordNm()->bSame(pWordScan->szWordNm()))
    bMatching = true;
  else
    bMatching = false;
  
  i8 iCmp = pWordBeingInserted->szWordNm()->iCompare(pWordScan->szWordNm());
  if (iCmp > 0)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* print word (in a skiplist)                                                 */
/*----------------------------------------------------------------------------*/
void vWordPrint(void* aWord)
{
  Word* pWord = _pWord(aWord);

  vTrace("%s"_Stz,pWord->szWordNm());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
