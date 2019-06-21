/*============================================================================*/
/* ForthWord.cpp -- Forth Word in Dictionary                                  */
/*============================================================================*/

#include "Forth.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ForthWord* Forth::_pWord(void* aAddr)
{
  ForthWord* pWord = reinterpret_cast<ForthWord*>(aAddr);
  
  RASSERT_TYPE(pWord,CN_WORD);

  return pWord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ForthWord::operator new(size_t, Dic* pDic)
{
  ForthWord* pWord = (ForthWord*)(HeapMem::pAllocate(CN_WORD));
  
  pWord->m_bPersistent = false;
  pWord->m_pDic = pDic;
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ForthWord::operator new(size_t, Dic* pDic, bool bPersistent)
{
  RASSERT(bPersistent || ! bPersistent);
  
  ForthWord* pWord = (ForthWord*)(HeapMem::pAllocate(CN_WORD,bPersistent));
  
  pWord->m_bPersistent = bPersistent;
  pWord->m_pDic = pDic;
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ForthWord::operator delete(void* pWordAddress)
{
  ForthWord* pWord = reinterpret_cast<ForthWord*>(pWordAddress);
 
  HeapMem::vFree(pWord,pWord->m_bPersistent);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ForthWord::ForthWord(Stz* sName)
: Object(CN_WORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD);
  
  m_sName         = sName;
  m_eWordType     = WT_ROUTINE;
  //m_bPersistent   = false;
  m_iTrace        = 0;
  //m_pDic        = pDic;
  m_pKind         = gpAttachMap->m_pWord ? gpAttachMap->m_pWord : this;
  m_iEpoch        = gpAttachMap->m_iEpoch;
  m_bImmediate    = false;
  m_iParams       = 0;
  m_pPrevWord     = nullptr;
  m_bPrompt       = false;
  m_iLocalNo      = 0;

  m_xRoutine.vZero(CN_WORD);
  
  _setFooter_();
  
  m_fpEntryStep   = &Steps::Step::vEnterRoutine;
  m_aEntryCell   = aFirstCell();

  _checkInitialised_(CN_WORD);
  
  if (m_pDic)
    m_pDic->vAddWord(this);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ForthWord::~ForthWord(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  // invoke the finalize function for embedded objects
  if (m_eWordType == WT_OBJECT) {
    Object* pEmbObject = reinterpret_cast<Object*>(m_xRoutine.ayArea());
    eCN eClass = pEmbObject->eClass();
    pFINALIZE* pFinalizeFun = gpFinalize[eClass];
    (pFinalizeFun)(eClass, pEmbObject);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void ForthWord::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ForthWord* pWord = _pWord(pObject);
  
  // dump only the objects
  if (WT_OBJECT == pWord->m_eWordType)
    pWord->vObjectDump(aTab);
  
  //for (ForthWord* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* get the outer word of this data ptr                                        */
/*----------------------------------------------------------------------------*/
ForthWord* ForthWord::pToWord(void* pBody)
{
  // static function
  
  ForthWord* pWord = (ForthWord*)aAdd<ForthWord*>(pBody,
                                   -( sizeof(ForthWord)
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
byte* ForthWord::ayAllot(i8 nBytes)
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
void ForthWord::vAppend(Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* aAddress = aAppend(aCell);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append a pointer to cell                                                   */
/*----------------------------------------------------------------------------*/
void* ForthWord::aAppend(Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* aAllottedCell = (Cell*)ayAllot(sizeof(aCell));
  
  aAllottedCell->m_aJumpTo = aCell;
  
  return aAllottedCell;
}

/*----------------------------------------------------------------------------*/
/* append an i8 integer                                                       */
/*----------------------------------------------------------------------------*/
void ForthWord::vAppend(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* aAddress = aAppend(iInt);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append an i8 integer                                                       */
/*----------------------------------------------------------------------------*/
void* ForthWord::aAppend(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  i8* aAllottedInt = (i8*)ayAllot(sizeof(i8));
  
  *aAllottedInt = iInt;
  
  return aAllottedInt;
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void ForthWord::vAppend(GenVar* pGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* aAddress = aAppend(pGenVar);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void* ForthWord::aAppend(GenVar* pGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  void* aValueAddress = aAppend(pGenVar->aNestedValue());
  
  return aValueAddress;
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void ForthWord::vAppend(ForthValue* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  void* aAddress = aAppend(aValue);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append a value  (the entire value, not the pointer to it)                  */
/*----------------------------------------------------------------------------*/
void* ForthWord::aAppend(ForthValue* aValue)
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
void ForthWord::vAppend(StepFp* afpStepFp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  void* aAddress = aAppend(afpStepFp);
  ASSERT(aAddress);
}

/*----------------------------------------------------------------------------*/
/* append address of pointer to task step                                     */
/*----------------------------------------------------------------------------*/
void* ForthWord::aAppend(StepFp* afpStepFp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  StepFp** aafpStepFp = (StepFp**)ayAllot(sizeof(StepFp*));

  *aafpStepFp = afpStepFp;

  return aafpStepFp;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ForthWord::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  ForthWord* pStepWord = gpAttachMap->m_oRootDic.pFindStep(m_fpEntryStep);
  
  vTrace("ForthWord %p: %s %s Kind %s Immediate %d DataBytes %lld "_Stz
         "Step %p %s ..Data.. %p\n"_Stz,
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
Dic* ForthWord::pDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pDic;
}

/*----------------------------------------------------------------------------*/
/* get epoch when word was created                                            */
/*----------------------------------------------------------------------------*/
i8 ForthWord::iEpoch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_iEpoch;
}

/*----------------------------------------------------------------------------*/
/* get entry step                                                             */
/*----------------------------------------------------------------------------*/
StepFp ForthWord::fpEntryStep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_fpEntryStep;
}

/*----------------------------------------------------------------------------*/
/* set entry step                                                             */
/*----------------------------------------------------------------------------*/
void ForthWord::vEntryStep(StepFp fpStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_fpEntryStep = fpStep;
}

/*----------------------------------------------------------------------------*/
/* get which cell to start executing from                                     */
/*----------------------------------------------------------------------------*/
Cell* ForthWord::aEntryCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_aEntryCell;
}

/*----------------------------------------------------------------------------*/
/* set which cell to start executing from                                     */
/*----------------------------------------------------------------------------*/
void ForthWord::vEntryCell(Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_aEntryCell = aCell;
}

/*----------------------------------------------------------------------------*/
/* get address of this words entry step                                       */
/*----------------------------------------------------------------------------*/
StepFp* ForthWord::afpEntryStep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return &m_fpEntryStep;
}

/*----------------------------------------------------------------------------*/
/* encode call to named word                                                  */
/*----------------------------------------------------------------------------*/
void ForthWord::vEncode(Stz* aWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  ForthWord* pCalledWord = gpAttachMap->m_oRootDic.pFindWord(aWordNm);
  ASSERT(pCalledWord);
  
  vAppend(pCalledWord->afpEntryStep());
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a cell* argument                       */
/*----------------------------------------------------------------------------*/
void ForthWord::vEncode(Stz* aWordNm, Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWordNm);
  
  vAppend(aCell);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a value argument                       */
/*----------------------------------------------------------------------------*/
void ForthWord::vEncode(Stz* aWordNm, GenVar* pGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vEncode(aWordNm);
  
  vAppend(pGenVar);
}

/*----------------------------------------------------------------------------*/
/* encode call to named word which has a word* argument                       */
/*----------------------------------------------------------------------------*/
void ForthWord::vEncode(Stz* aWordNm, ForthWord* pCalledWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  GenVar xVar;
  xVar = pCalledWord;
  
  vEncode(aWordNm,&xVar);
}

/*----------------------------------------------------------------------------*/
/* encode call of entry step                                                  */
/*----------------------------------------------------------------------------*/
void ForthWord::vEncode(StepFp* afpEntryStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vAppend(afpEntryStep);
}

/*----------------------------------------------------------------------------*/
/* get pointer to first cell in data area                                     */
/*----------------------------------------------------------------------------*/
Cell* ForthWord::aFirstCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* aFirst = reinterpret_cast<Cell*>(m_xRoutine.ayArea());
  
  return pPoolCheck<Cell*>(aFirst);
}

/*----------------------------------------------------------------------------*/
/* does this word have given cell?                                            */
/*----------------------------------------------------------------------------*/
bool ForthWord::bHasCell(Cell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  if (aCell >= aFirstCell() && aCell <= aLastCell())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get immediate flag                                                         */
/*----------------------------------------------------------------------------*/
bool ForthWord::bImmediate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_bImmediate;
}

/*----------------------------------------------------------------------------*/
/* set immediate flag                                                         */
/*----------------------------------------------------------------------------*/
void ForthWord::vImmediate(bool bImmediate)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_bImmediate = bImmediate;
}

/*----------------------------------------------------------------------------*/
/* is given name?                                                             */
/*----------------------------------------------------------------------------*/
bool ForthWord::bIsName(s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  bool b = m_sName.bSame(sName);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get word kind                                                              */
/*----------------------------------------------------------------------------*/
ForthWord* ForthWord::pKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_pKind;
}

/*----------------------------------------------------------------------------*/
/* set word kind                                                              */
/*----------------------------------------------------------------------------*/
void ForthWord::vKind(ForthWord* pKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  m_pKind = pKind;
}

/*----------------------------------------------------------------------------*/
/* get pointer to last cell                                                   */
/*----------------------------------------------------------------------------*/
Cell* ForthWord::aLastCell(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  Cell* aLast = reinterpret_cast<Cell*>(m_xRoutine.ayLatter());
  
  return pPoolCheck<Cell*>(aLast);
}

/*----------------------------------------------------------------------------*/
/* get local  number                                                          */
/*----------------------------------------------------------------------------*/
i8 ForthWord::iLocalNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_iLocalNo;
}

/*----------------------------------------------------------------------------*/
/* set local number                                                           */
/*----------------------------------------------------------------------------*/
void ForthWord::vLocalNo(i8 iLocalNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_iLocalNo = iLocalNo;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
s32& ForthWord::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* dump object word                                                           */
/*----------------------------------------------------------------------------*/
void ForthWord::vObjectDump(Stz* aTab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  vTrace("epoch %lld %s (%s) %s %s DataBytes %lld "_Stz
         "Step %p ..Data.. %p\n"_Stz,
         m_iEpoch,
         m_sName.aChar(),
         m_pDic ? m_pDic->sName().aChar() : "<none>",
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
void ForthWord::vParams(i8 iParams)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_iParams = iParams;
}

/*----------------------------------------------------------------------------*/
/* get number of parameters                                                   */
/*----------------------------------------------------------------------------*/
i8 ForthWord::iParams(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_iParams;
}

/*----------------------------------------------------------------------------*/
/* get previous word                                                          */
/*----------------------------------------------------------------------------*/
ForthWord* ForthWord::pPrevWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);

  return pPoolCheck<ForthWord*>(m_pPrevWord);
}

/*----------------------------------------------------------------------------*/
/* set previous word                                                          */
/*----------------------------------------------------------------------------*/
void ForthWord::vPrevWord(ForthWord* pPrevWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_pPrevWord = pPrevWord;
}

/*----------------------------------------------------------------------------*/
/* get prompt flag                                                            */
/*----------------------------------------------------------------------------*/
bool ForthWord::bPrompt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_bPrompt;
}

/*----------------------------------------------------------------------------*/
/* set prompt flag                                                            */
/*----------------------------------------------------------------------------*/
void ForthWord::vPrompt(bool bPrompt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_bPrompt = bPrompt;
}

/*----------------------------------------------------------------------------*/
/* get word type                                                              */
/*----------------------------------------------------------------------------*/
eWT ForthWord::eWordType(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  return m_eWordType;
}

/*----------------------------------------------------------------------------*/
/* set word type                                                              */
/*----------------------------------------------------------------------------*/
void ForthWord::vWordType(eWT eWordType)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD,_FOOTER_);
  
  m_eWordType = eWordType;
}

/*----------------------------------------------------------------------------*/
/* get ptr to data area                                                       */
/*----------------------------------------------------------------------------*/
char* ForthWord::aWspArea(void)
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
Footer* ForthWord::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
