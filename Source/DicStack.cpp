/*============================================================================*/
/* DicStack.cpp - dictionary stack                                            */
/*============================================================================*/

#include "SenVM.h"


/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
DicStack* DicStack::_pDicStack(void* aAddr)
{
  DicStack* pDicStack = reinterpret_cast<DicStack*>(aAddr);
  
  RASSERT_TYPE(pDicStack,CN_DICSTACK);
  
  return pDicStack;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* DicStack::operator new(size_t)
{
  DicStack* pDicStack = (DicStack*)(HeapMem::pAllocate(CN_DICSTACK));
  
  return pDicStack;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void DicStack::operator delete(void* pDicAddress)
{
  DicStack* pDicStack = reinterpret_cast<DicStack*>(pDicAddress);
  
  HeapMem::vFree(pDicStack);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DicStack::DicStack(void)
: Object(CN_DICSTACK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  m_iTop = -1;

  for (i8 i_Dic = 0; i_Dic < C_DIC_PATHDEPTH; i_Dic++)
    m_pDic[i_Dic] = nullptr;
  
  _checkInitialised_(CN_DICSTACK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DicStack::~DicStack(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  while (m_iTop >= 0)
    vDiscard();
}

/*----------------------------------------------------------------------------*/
/* get number of dictionaries in the stack                                    */
/*----------------------------------------------------------------------------*/
i8 DicStack::iCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  return m_iTop + 1;
}

/*----------------------------------------------------------------------------*/
/* dump stack of dictionaries                                                 */
/*----------------------------------------------------------------------------*/
void DicStack::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--)
    vTrace("Dic %p %lld %s\n"_Stz,m_pDic[i_Dic],i_Dic,m_pDic[i_Dic]->szName());
}

/*----------------------------------------------------------------------------*/
/* discard top dictionary                                                     */
/*----------------------------------------------------------------------------*/
void DicStack::vDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DIC_PATHDEPTH);
  
  Dic* pDic = m_pDic[m_iTop];

  if (pDic)
    pDic->vRefDec();

  m_pDic[m_iTop] = nullptr;
  
  m_iTop--;
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void DicStack::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  DicStack* pDicStack = _pDicStack(pObject);
  
  vTrace("top %p:"_Stz,pDicStack->m_iTop);
  //for (i8 i_Dic = pDicStack->m_iTop; i_Dic >= 0; i_Dic --)
    //vTrace("%s  Dic %p %lld %s\n"_Stz,aTab, pDicStack->m_pDic[i_Dic],i,
           //pDicStack->m_pDic[i_Dic]->sName().s());
  
  for (i8 i_Dic = pDicStack->m_iTop; i_Dic >= 0; i_Dic--) {
    vTrace(" dic %p %lld %s"_Stz,aTab, pDicStack->m_pDic[i_Dic],i_Dic,
           pDicStack->m_pDic[i_Dic]->szName());
    //Dic::vDump(CN_DIC, pDicStack->m_pDic[i], aTab);
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DicStack::pFindWord(s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pWord = pFindWord(sName.szStz());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DicStack::pFindWord(Stz* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pWord;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    if ( (pWord = pDic->pFindWord((Stz*)aName)) )
      return pWord;
  }
  
  Dic* pRootDic = AM::pRootDic();

  if ( (pWord = pRootDic->pFindWord((Stz*)aName)) )
    return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find word in current dictionary                                            */
/*----------------------------------------------------------------------------*/
Word* DicStack::pFindInCurrent(Stz* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  
  Word* pWord;
  
  if ( (pWord = m_pDic[m_iTop]->pFindWord((Stz*)aName)) )
    return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* locate word in stack of dictionaries that has given cell                   */
/*----------------------------------------------------------------------------*/
Word* DicStack::pLocateWord(Cell* aHasCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pWord;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    for (pWord = pDic->pLastWord(); pWord; pWord = pWord->pPrevWord())
      if (pWord->bHasCell(aHasCell))
        return pWord;
  }
  
  Dic* pRootDic = AM::pRootDic();
  for (pWord = pRootDic->pLastWord(); pWord; pWord = pWord->pPrevWord())
    if (pWord->bHasCell(aHasCell))
      return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* locate word that has given step                                            */
/*----------------------------------------------------------------------------*/
Word* DicStack::pLocateWord(StepFp fpHasStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pWord;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    for (pWord = pDic->pLastWord(); pWord; pWord = pWord->pPrevWord())
      if (WK_INTRINSIC == pWord->eKind() &&
          fpHasStep == pWord->fpEntryStep())
        return pWord;
  }
  
  Dic* pRootDic = AM::pRootDic();
  for (pWord = pRootDic->pLastWord();
       pWord;
       pWord = pWord->pPrevWord())
    if (WK_INTRINSIC == pWord->eKind() &&
        fpHasStep == pWord->fpEntryStep())
      return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a particular dictionary on the stack                                   */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pPick(i8 iPick)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  //Dic* pDic = m_pDic[m_iTop - iPick];
  Dic* pDic = m_pDic[iPick];

  return pDic;
}

/*----------------------------------------------------------------------------*/
/* pop top dictionary                                                         */
/*----------------------------------------------------------------------------*/
void DicStack::vPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DIC_PATHDEPTH);
  
  // just pop the top
  m_pDic[m_iTop] = nullptr;
  
  m_iTop--;
}

/*----------------------------------------------------------------------------*/
/* push dictionary onto stack                                                 */
/*----------------------------------------------------------------------------*/
void DicStack::vPush(Dic* pDic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= -1);
  ASSERT(m_iTop <  C_DIC_PATHDEPTH - 1);
  
  ASSERT(CN_DIC == pDic->eClass());
  
  pDic->vRefInc();
  
  m_iTop++;
  
  m_pDic[m_iTop] = pDic;
}

/*----------------------------------------------------------------------------*/
/* forget word in stack of dictionaries                                       */
/*----------------------------------------------------------------------------*/
Word* DicStack::pRemoveWord(s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pRemovedWord = nullptr;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    if ( (pRemovedWord = pDic->pRemoveWord(sName)) )
      return pRemovedWord;
  }
  
  Dic* pRootDic = AM::pRootDic();
  if ( (pRemovedWord = pRootDic->pRemoveWord(sName)) )
    return pRemovedWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* set dictionary stack from dictionary path                                  */
/*----------------------------------------------------------------------------*/
void DicStack::vSetStack(DicPath* pDicPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  s32 sFolderNm;
  
  Dic* pDic = _pDic(AM::pTenants()->aWspArea());
  
  i8 nFolders = pDicPath->nDicNames();
  for (i8 i_Folder = 0; i_Folder < nFolders; i_Folder++) {
    
    sFolderNm = pDicPath->sDicName(i_Folder);
    
    if (sFolderNm.bSame("tenants"_Stz))
      continue;
    
    //pDic->vListWords();
    Word* pWord = pDic->pFindWord(sFolderNm);
    
    Dic* pSubDic = _pDic(pWord->aWspArea());
    
    vPush(pSubDic);
    
    pDic = pSubDic;
  }
}

/*----------------------------------------------------------------------------*/
/* get topmost dictionary                                                     */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pTop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DIC_PATHDEPTH);
  
  Dic* pTop = m_pDic[m_iTop];
  
  return pTop;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(DicStack)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
