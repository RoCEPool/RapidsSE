/*============================================================================*/
/* DicPath.cpp - dictionary stack                                            */
/*============================================================================*/

#include "Dictionaries.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
  vFatal("fatal: DictionaryStack class cannot be new'd"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void DicPath::operator delete(void* aAddress)
{
  RASSERT(aAddress);
  vFatal("fatal: DictionaryStack class cannot be deleted"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DicPath::DicPath(void)
: Object(CN_DICPATH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);

  m_iTop = -1;

  for (i8 i_Dic = 0; i_Dic < C_DICTIONARY_STACK; i_Dic++)
    m_pDic[i_Dic] = nullptr;
  
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
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--)
    vTrace("Dic %p %lld %s\n"_Stz,m_pDic[i_Dic],i_Dic,m_pDic[i_Dic]->sName().aChar());
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
void DicPath::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_DICPATH);
  ASSERT(pObject->eClass() == CN_DICPATH);
  
  DicPath* pDicPath = reinterpret_cast<DicPath*>(pObject);
  
  vTrace("top %p:"_Stz,pDicPath->m_iTop);
  //for (i8 i_Dic = pDicPath->m_iTop; i_Dic >= 0; i_Dic --)
    //vTrace("%s  Dic %p %lld %s\n"_Stz,aTab, pDicPath->m_pDic[i_Dic],i,
           //pDicPath->m_pDic[i_Dic]->sName().s());
  
  for (i8 i_Dic = pDicPath->m_iTop; i_Dic >= 0; i_Dic--) {
    vTrace(" dic %p %lld %s"_Stz,aTab, pDicPath->m_pDic[i_Dic],i_Dic,
           pDicPath->m_pDic[i_Dic]->sName().aChar());
    //Dic::vDump(CN_DIC, pDicPath->m_pDic[i], aTab);
  }

  vTrace("\n"_Stz);

  //for (Word* pWord = m_pCurrentWord; pWord; pWord = pWord->pPrevWord())
  //  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DicPath::pFindWord(StrVal& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  Word* pWord = pFindWord(sName.aStr());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* find word in stack of dictionaries                                         */
/*----------------------------------------------------------------------------*/
Word* DicPath::pFindWord(Stz* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  Word* pWord;
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = m_pDic[i_Dic];
    
    if ( (pWord = pDic->pFindWord((Stz*)aName)) )
      return pWord;
  }
  
  if ( (pWord = gpAttachMap->m_xMainDic.pFindWord((Stz*)aName)) )
    return pWord;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find word in current dictionary                                            */
/*----------------------------------------------------------------------------*/
Word* DicPath::pFindInCurrent(Stz* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH,_FOOTER_);
  
  ASSERT(m_iTop >= 0);
  
  Word* pWord;
  
  if ( (pWord = m_pDic[m_iTop]->pFindWord((Stz*)aName)) )
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
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--)
    for (pWord = m_pDic[i_Dic]->pLastWord(); pWord; pWord = pWord->pPrevWord())
      if (WT_PRIMITIVE == pWord->eWordType() && pStep == pWord->pEntryStep())
        return pWord;

  for (pWord = gpAttachMap->m_xMainDic.pLastWord(); pWord; pWord = pWord->pPrevWord())
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
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--)
    if (m_pDic[i_Dic]->bForget(sName))
      return true;

  if (gpAttachMap->m_xMainDic.bForget(sName))
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
  
  for (i8 i_Dic = m_iTop; i_Dic >= 0; i_Dic--)
    for (pWord = m_pDic[i_Dic]->pLastWord(); pWord; pWord = pWord->pPrevWord())
      if (pWord->bHasCell(pCell))
        return pWord;

  for (pWord = gpAttachMap->m_xMainDic.pLastWord(); pWord; pWord = pWord->pPrevWord())
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
Footer* DicPath::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DICPATH);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
