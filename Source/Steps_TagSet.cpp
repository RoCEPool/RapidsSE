/*============================================================================*/
/* Steps_TagSet.cpp - tag-set step group                                      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TagSet(void)
{
  vAddStep("tagSet::new"_Stz,       &Step::vTagSetNew);
  
  vAddStep("tagSet.appendInt"_Stz,  &Step::vTagSetAppendInt);
  vAddStep("tagSet.appendStr"_Stz,  &Step::vTagSetAppendStr);
  vAddStep("tagSet.asStr"_Stz,      &Step::vTagSetAsStr);
  vAddStep("tagSet.copy"_Stz,       &Step::vTagSetCopy);
  vAddStep("tagSet.getInt"_Stz,     &Step::vTagSetGetInt);
  vAddStep("tagSet.getStr"_Stz,     &Step::vTagSetGetStr);
  vAddStep("tagSet.hasName"_Stz,    &Step::vTagSetHasName);
  vAddStep("tagSet.insertInt"_Stz,  &Step::vTagSetInsertInt);
  vAddStep("tagSet.insertStr"_Stz,  &Step::vTagSetInsertStr);
  vAddStep("tagSet.replaceInt"_Stz, &Step::vTagSetReplaceInt);
  vAddStep("tagSet.replaceStr"_Stz, &Step::vTagSetReplaceStr);
  vAddStep("tagSet.upsertStr"_Stz,  &Step::vTagSetUpsertStr);
}

/*----------------------------------------------------------------------------*/
/* TagSet::new -- new ename-set                                               */
/*----------------------------------------------------------------------------*/
void Step::vTagSetNew(void)
{
  ENTRY();
  
  TagSet* pTagSet = new TagSet;
  
  m_pDataStack->vPushPtr(pTagSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* tagSet.appendInt -- append a name-integer pair to ename-set                */
/*----------------------------------------------------------------------------*/
void Step::vTagSetAppendInt(void)
{
  ENTRY();
  
  TagSet* pTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));
  
  i8 iValue = m_pDataStack->iPopInt(__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pTagSet->vAppend(sName,iValue);
  // pTagSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tagSet.AppendStr -- append a name-string pair to ename-set                 */
/*----------------------------------------------------------------------------*/
void Step::vTagSetAppendStr(void)
{
  ENTRY();
  
  TagSet* pTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));

  s256 sValue;
  m_pDataStack->vPopStr(sValue,__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pTagSet->vAppend(sName,sValue);
  //pTagSet->vPrint();
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* TagSet.asStr -- get ename-set as a single string                           */
/*----------------------------------------------------------------------------*/
void Step::vTagSetAsStr(void)
{
  ENTRY();
  
  TagSet* pTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));
  
  sSTMT sStr;
  SP_TagSet spTagSet;
  sStr = spTagSet.sz(pTagSet);
  
  m_pDataStack->vPushStr(sStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.copy -- copy source set into this destination set                   */
/*----------------------------------------------------------------------------*/
void Step::vTagSetCopy(void)
{
  ENTRY();
  
  TagSet* pTagSet_dst = _pTagSet(m_pDataStack->pPopPtr(__func__));
  TagSet* pTagSet_src = _pTagSet(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pTagSet_dst->vCopy(pTagSet_src);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.getInt -- get int value from ename-set with given name              */
/*----------------------------------------------------------------------------*/
void Step::vTagSetGetInt(void)
{
  ENTRY();
  
  TagSet* pTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  i8 iValue;
  iValue = pTagSet->iValue(sName);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.getStr -- get str value from ename-set with given name              */
/*----------------------------------------------------------------------------*/
void Step::vTagSetGetStr(void)
{
  ENTRY();
  
  TagSet* pTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // translate name into value
  
  sSTMT sValue = ""_Stz;
  
  if (pTagSet->bHasName(sName)) {
    sValue = pTagSet->szValue(sName);
  }
  
  m_pDataStack->vPushStr(sValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.hasName -- test if ename-set has a pair with given name             */
/*----------------------------------------------------------------------------*/
void Step::vTagSetHasName(void)
{
  ENTRY();
  
  TagSet* pTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);

  bool bHas = pTagSet->bHasName(sName);
  
  m_pDataStack->vPushInt(bHas);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tagSet.insertInt -- add a name-integer pair to ename-set                   */
/*----------------------------------------------------------------------------*/
void Step::vTagSetInsertInt(void)
{
  ENTRY();
  
  TagSet* pTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));
  
  i8 iValue = m_pDataStack->iPopInt(__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pTagSet->vInsert(sName,iValue);
  // pTagSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tagSet.insertStr -- add a name-string pair to ename-set                    */
/*----------------------------------------------------------------------------*/
void Step::vTagSetInsertStr(void)
{
  ENTRY();
  
  TagSet* pTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));

  s256 sValue;
  m_pDataStack->vPopStr(sValue,__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pTagSet->vInsert(sName,sValue);
  //pTagSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.replaceInt -- replace a name-integer pair to ename-set              */
/*----------------------------------------------------------------------------*/
void Step::vTagSetReplaceInt(void)
{
  ENTRY();
  
  TagSet* pTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));
  
  i8 iValue = m_pDataStack->iPopInt(__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pTagSet->vReplace(sName,iValue);
  //pTagSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.replaceStr -- replace a name-string pair to ename-set               */
/*----------------------------------------------------------------------------*/
void Step::vTagSetReplaceStr(void)
{
  ENTRY();
  
  TagSet* pTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));

  s256 sValue;
  m_pDataStack->vPopStr(sValue,__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pTagSet->vReplace(sName,sValue);
  //pTagSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.upsertStr -- replace/add a name-string pair to ename-set            */
/*----------------------------------------------------------------------------*/
void Step::vTagSetUpsertStr(void)
{
  ENTRY();
  
  TagSet* pTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));
  
  s256 sValue;
  m_pDataStack->vPopStr(sValue,__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  bool bHas = pTagSet->bHasName(sName);
  
  if (bHas) {
    pTagSet->vReplace(sName,sValue);
  }
  else {
    pTagSet->vInsert(sName,sValue);
  }
  
  //pTagSet->vPrint();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
