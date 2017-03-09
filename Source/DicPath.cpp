/*============================================================================*/
/* DicPath.cpp - dictionary stack                                            */
/*============================================================================*/

#include "Dictionaries.h"

extern Dic* gpMainDic;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
DicPath* DicPath::_pDicStack(void* aAddr)
{
  DicPath* pDicPath = reinterpret_cast<DicPath*>(aAddr);
  
  RASSERT( !pDicPath || CN_DICPATH == pDicPath->eClass() );
  
  return pDicPath;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* DicPath::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: DictionaryStack class cannot be new'd"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void DicPath::operator delete(void* aAddress)
{
  RASSERT(aAddress);
  vFatal("fatal: DictionaryStack class cannot be deleted"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DicPath::DicPath(void)
: Object(CN_DICPATH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);

  m_iTop = -1;

  for (i8 i = 0; i < C_DICTIONARY_STACK; i++)
    m_pDic[i] = nullptr;
  
  _checkInitialised_(CN_DICPATH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DicPath::~DicPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  while (m_iTop >= 0)
    vDiscard();
}

/*----------------------------------------------------------------------------*/
/* get number of dictionaries in the stack                                    */
/*----------------------------------------------------------------------------*/
i8 DicPath::iCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  return m_iTop + 1;
}

/*----------------------------------------------------------------------------*/
/* dump stack of dictionaries                                                 */
/*----------------------------------------------------------------------------*/
void DicPath::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  for (i8 i = m_iTop; i >= 0; i--)
    vTrace("Dic %p %lld %s\n"_Str,m_pDic[i],i,m_pDic[i]->sName().aStr());
}

/*----------------------------------------------------------------------------*/
/* discard top dictionary                                                     */
/*----------------------------------------------------------------------------*/
void DicPath::vDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DICTIONARY_STACK);
  
  Dic* pDic = m_pDic[m_iTop];

  if (pDic)
    pDic->vRefDec();

  m_pDic[m_iTop] = nullptr;
  
  m_iTop--;
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void DicPath::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_DICPATH);
  ASSERT(pObject->eClass() == CN_DICPATH);
  
  DicPath* pDicPath = reinterpret_cast<DicPath*>(pObject);
  
  vTrace("top %p:"_Str,pDicPath->m_iTop);
  //for (i8 i = pDicPath->m_iTop; i >= 0; i--)
    //vTrace("%s  Dic %p %lld %s\n"_Str,aTab, pDicPath->m_pDic[i],i,
           //pDicPath->m_pDic[i]->sName().aStr());
  
  for (i8 i = pDicPath->m_iTop; i >= 0; i--) {
    vTrace(" dic %p %lld %s"_Str,aTab, pDicPath->m_pDic[i],i,
           pDicPath->m_pDic[i]->sName().aStr());
    //Dic::vDump(CN_DIC, pDicPath->m_pDic[i], aTab);
  }

  vTrace("\n"_Str);

  //for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DicPath::pFindWord(StrVal& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  Word* pWord = pFindWord(sName.aStrArea());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DicPath::pFindWord(Str* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  Word* pWord;
  
  for (i8 iDic = m_iTop; iDic >= 0; iDic--) {
    
    Dic* pDic = m_pDic[iDic];
    
    if ( (pWord = pDic->pFindWord((Str*)aName)) )
      return pWord;
  }
  
  if ( (pWord = gpMainDic->pFindWord((Str*)aName)) )
    return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find word in current dictionary                                            */
/*----------------------------------------------------------------------------*/
Word* DicPath::pFindInCurrent(Str* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  
  Word* pWord;
  
  if ( (pWord = m_pDic[m_iTop]->pFindWord((Str*)aName)) )
    return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a word given step                                                     */
/*----------------------------------------------------------------------------*/
Word* DicPath::pFindStep(TaskStep pStep)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  Word* pWord;
  
  for (i8 i = m_iTop; i >= 0; i--)
    for (pWord = m_pDic[i]->pLastWord(); pWord; pWord = pWord->pPrevWord())
      if (WT_PRIMITIVE == pWord->eWordType() && pStep == pWord->pEntryStep())
        return pWord;

  for (pWord = gpMainDic->pLastWord(); pWord; pWord = pWord->pPrevWord())
    if (WT_PRIMITIVE == pWord->eWordType() && pStep == pWord->pEntryStep())
      return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* forget word in stack of dictionaries                                       */
/*----------------------------------------------------------------------------*/
bool DicPath::bForget(StrVal& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  for (i8 i = m_iTop; i >= 0; i--)
    if (m_pDic[i]->bForget(sName))
      return true;

  if (gpMainDic->bForget(sName))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* locate word in stack of dictionaries                                       */
/*----------------------------------------------------------------------------*/
Word* DicPath::pLocate(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);

  Word* pWord;
  
  for (i8 i = m_iTop; i >= 0; i--)
    for (pWord = m_pDic[i]->pLastWord(); pWord; pWord = pWord->pPrevWord())
      if (pWord->bHasCell(pCell))
        return pWord;

  for (pWord = gpMainDic->pLastWord(); pWord; pWord = pWord->pPrevWord())
    if (pWord->bHasCell(pCell))
      return pWord;
  
  return nullptr;
  
}

/*----------------------------------------------------------------------------*/
/* get a particular dictionary on the stack                                   */
/*----------------------------------------------------------------------------*/
Dic* DicPath::pPick(i8 iPick)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  Dic* pDic = m_pDic[m_iTop - iPick];
  
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* push dictionary onto stack                                                 */
/*----------------------------------------------------------------------------*/
void DicPath::vPush(Dic* pDic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  ASSERT(m_iTop >= -1);
  ASSERT(m_iTop <  C_DICTIONARY_STACK - 1);
  
  ASSERT(CN_DIC == pDic->eClass());
  
  pDic->vRefInc();
  
  m_iTop++;
  
  m_pDic[m_iTop] = pDic;
}

/*----------------------------------------------------------------------------*/
/* get topmost dictionary                                                     */
/*----------------------------------------------------------------------------*/
Dic* DicPath::pTop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);

  ASSERT(m_iTop >= 0);
  ASSERT(m_iTop <  C_DICTIONARY_STACK);
  
  Dic* pTop = m_pDic[m_iTop];
  
  return pTop;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* DicPath::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
