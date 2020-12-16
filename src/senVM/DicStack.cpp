/*============================================================================*/
/* DicStack.cpp - dictionary stack                                            */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
DicStack* SenVM::_pDicStack(void* aAddr)
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

  for (i8 i_Dic = 0; i_Dic < C_DIC_STACKDEPTH; i_Dic++)
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
/* dump content of dictionary stack                                           */
/*----------------------------------------------------------------------------*/
void DicStack::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("Dictionary Stack\n"_Stz);
  vTrace("----------------\n"_Stz);

  vTrace("%p:\n"_Stz,this);

  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--)
    vTrace("[%02lld] %s\n"_Stz,i_Dic,m_pDic[i_Dic]->szDicNm());
}

/*----------------------------------------------------------------------------*/
/* copy given stack into this one                                             */
/*----------------------------------------------------------------------------*/
void DicStack::vCopy(DicStack* pDicStackSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  vReset();
  
  for (i8 i_Dic = pDicStackSrc->m_iTop; i_Dic >= 0; i_Dic--) {
    m_pDic[i_Dic] = pDicStackSrc->m_pDic[i_Dic];
  }
  
  m_iTop = pDicStackSrc->m_iTop;
}

/*----------------------------------------------------------------------------*/
/* discard top dictionary                                                     */
/*----------------------------------------------------------------------------*/
void DicStack::vDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DIC_STACKDEPTH);
  
  Dic* pDic = m_pDic[m_iTop];

  if (pDic)
    pDic->vRefDec();

  m_pDic[m_iTop] = nullptr;
  
  m_iTop--;
}

/*----------------------------------------------------------------------------*/
/* dump dictionary stack                                                      */
/*----------------------------------------------------------------------------*/
void DicStack::vDump(eCN eClass, Object* pObject, Stz* szTab)
{
  DicStack* pDicStack = _pDicStack(pObject);
  
  vTrace("top %p:"_Stz,pDicStack->m_iTop);
  //for (i8 i_Dic = pDicStack->m_iTop; i_Dic >= 0; i_Dic --)
    //vTrace("%s  Dic %p %lld %s\n"_Stz,aTab, pDicStack->m_pDic[i_Dic],i,
           //pDicStack->m_pDic[i_Dic]->sName().s());
  
  for (i8 i_Dic = pDicStack->m_iTop; i_Dic >= 0; i_Dic--) {
    vTrace(" dic %p %lld %s"_Stz,szTab, pDicStack->m_pDic[i_Dic],i_Dic,
           pDicStack->m_pDic[i_Dic]->szDicNm());
    //Dic::vDump(CN_DIC, pDicStack->m_pDic[i], aTab);
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DicStack::pFindWord(Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pWord;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    if ( (pWord = pDic->pFindWord((Stz*)szWordNm)) )
      return pWord;
  }
  
  Dic* pRootDic = AM::pRootDic();

  if ( (pWord = pRootDic->pFindWord((Stz*)szWordNm)) )
    return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find word in current dictionary                                            */
/*----------------------------------------------------------------------------*/
Word* DicStack::pFindInCurrent(Stz* szWordNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  
  Word* pWord;
  
  if ( (pWord = m_pDic[m_iTop]->pFindWord((Stz*)szWordNm)) )
    return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* locate word in stack of dictionaries that has given cell                   */
/*----------------------------------------------------------------------------*/
Word* DicStack::pLocateRoutineWord(ForthCell* aCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pWord;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    for (pWord = pDic->pLastWord(); pWord; pWord = pWord->pPrevWord())
      if (pWord->bHasCell(aCell))
        return pWord;
  }
  
  Dic* pRootDic = AM::pRootDic();
  for (pWord = pRootDic->pLastWord(); pWord; pWord = pWord->pPrevWord())
    if (pWord->bHasCell(aCell))
      return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* locate word that has given step                                            */
/*----------------------------------------------------------------------------*/
Word* DicStack::pLocateStepWord(StepFp fpStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pWord;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    for (pWord = pDic->pLastWord(); pWord; pWord = pWord->pPrevWord())
      if (WK_INTRINSIC == pWord->eWord() &&
          fpStep == pWord->fpEntryStep())
        return pWord;
  }
  
  Dic* pRootDic = AM::pRootDic();
  for (pWord = pRootDic->pLastWord();
       pWord;
       pWord = pWord->pPrevWord())
    if (WK_INTRINSIC == pWord->eWord() &&
        fpStep == pWord->fpEntryStep())
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
//void DicStack::vPop(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
//
//  ASSERT(m_iTop >= 0);
//  ASSERT(m_iTop <  C_DIC_STACKDEPTH);
//
//  // just pop the top
//  m_pDic[m_iTop] = nullptr;
//
//  m_iTop--;
//}
/*----------------------------------------------------------------------------*/
void DicStack::vPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DIC_STACKDEPTH);
  
  Dic* pDic = m_pDic[m_iTop];

  if (pDic)
    pDic->vRefDec();

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
  ASSERT(m_iTop <  C_DIC_STACKDEPTH - 1);
  
  ASSERT(CN_DIC == pDic->eClass());
  
  if (pDic)
    pDic->vRefInc();
  
  m_iTop++;
  
  m_pDic[m_iTop] = pDic;
}

/*----------------------------------------------------------------------------*/
/* forget word in stack of dictionaries                                       */
/*----------------------------------------------------------------------------*/
Word* DicStack::pRemoveWord(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  Word* pRemovedWord = nullptr;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    if ( (pRemovedWord = pDic->pRemoveWord(szName)) )
      return pRemovedWord;
  }
  
  Dic* pRootDic = AM::pRootDic();
  if ( (pRemovedWord = pRootDic->pRemoveWord(szName)) )
    return pRemovedWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* reset this dic stack                                                       */
/*----------------------------------------------------------------------------*/
void DicStack::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);
  
  m_iTop = -1;
}

/*----------------------------------------------------------------------------*/
/* get topmost dictionary                                                     */
/*----------------------------------------------------------------------------*/
Dic* DicStack::pTop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DIC_STACKDEPTH);
  
  Dic* pTop = m_pDic[m_iTop];
  
  return pTop;
}

/*----------------------------------------------------------------------------*/
/* get topmost dictionary                                                     */
/*----------------------------------------------------------------------------*/
void DicStack::vToPathStr(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICSTACK,_FOOTER_);

  szPath->vCopy(""_Stz);
  
  for (i8 i_Dic = 0; i_Dic <= m_iTop; i_Dic++) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    //vTrace("[%02lld] %s\n"_Stz,i_Dic,pDic->szName());
    
    //if (i_Dic != m_iTop)
    szPath->vConcat("/"_Stz);

    szPath->vConcat(pDic->szDicNm());
  }
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(DicStack)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
