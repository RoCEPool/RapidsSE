/*============================================================================*/
/* Steps_EnameSet.cpp - ename-set step group                                  */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_EnameSet(void)
{
  vAddStep("enameSet::new"_Stz,       &Step::vEnameSetNew);
  
  vAddStep("enameSet.appendInt"_Stz,  &Step::vEnameSetAppendInt);
  vAddStep("enameSet.appendStr"_Stz,  &Step::vEnameSetAppendStr);
  vAddStep("enameSet.asStr"_Stz,      &Step::vEnameSetAsStr);
  vAddStep("enameSet.copy"_Stz,       &Step::vEnameSetCopy);
  vAddStep("enameSet.getInt"_Stz,     &Step::vEnameSetGetInt);
  vAddStep("enameSet.getStr"_Stz,     &Step::vEnameSetGetStr);
  vAddStep("enameSet.hasName"_Stz,    &Step::vEnameSetHasName);
  vAddStep("enameSet.insertInt"_Stz,  &Step::vEnameSetInsertInt);
  vAddStep("enameSet.insertStr"_Stz,  &Step::vEnameSetInsertStr);
  vAddStep("enameSet.replaceInt"_Stz, &Step::vEnameSetReplaceInt);
  vAddStep("enameSet.replaceStr"_Stz, &Step::vEnameSetReplaceStr);
  vAddStep("enameSet.upsertStr"_Stz,  &Step::vEnameSetUpsertStr);
}

/*----------------------------------------------------------------------------*/
/* TagSet::new -- new ename-set                                             */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetNew(void)
{
  ENTRY();
  
  TagSet* pEnameSet = new TagSet;
  
  m_pDataStack->vPushPtr(pEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* enameSet.appendInt -- append a name-integer pair to ename-set              */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetAppendInt(void)
{
  ENTRY();
  
  TagSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  i8 iValue = m_pDataStack->iPopInt(__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pEnameSet->vAppend(sName,iValue);
  // pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enameSet.AppendStr -- append a name-string pair to ename-set               */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetAppendStr(void)
{
  ENTRY();
  
  TagSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));

  s256 sValue;
  m_pDataStack->vPopStr(sValue,__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pEnameSet->vAppend(sName,sValue);
  //pEnameSet->vPrint();
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* TagSet.asStr -- get ename-set as a single string                         */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetAsStr(void)
{
  ENTRY();
  
  TagSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  s4K sStr;
  SP_EnmSet spEnameSet;
  sStr = spEnameSet.sz(pEnameSet);
  
  m_pDataStack->vPushStr(sStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.copy -- copy source set into this destination set                 */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetCopy(void)
{
  ENTRY();
  
  TagSet* pEnameSet_dst = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  TagSet* pEnameSet_src = _pEnameSet(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pEnameSet_dst->vCopy(pEnameSet_src);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.getInt -- get int value from ename-set with given name            */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetGetInt(void)
{
  ENTRY();
  
  TagSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  i8 iValue;
  iValue = pEnameSet->iValue(sName);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.getStr -- get str value from ename-set with given name            */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetGetStr(void)
{
  ENTRY();
  
  TagSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // translate name into value
  
  s4K sValue = ""_Stz;
  
  if (pEnameSet->bHasName(sName)) {
    sValue = pEnameSet->szValue(sName);
  }
  
  m_pDataStack->vPushStr(sValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.hasName -- test if ename-set has a pair with given name           */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetHasName(void)
{
  ENTRY();
  
  TagSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);

  bool bHas = pEnameSet->bHasName(sName);
  
  m_pDataStack->vPushInt(bHas);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enameSet.insertInt -- add a name-integer pair to ename-set                 */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetInsertInt(void)
{
  ENTRY();
  
  TagSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  i8 iValue = m_pDataStack->iPopInt(__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pEnameSet->vInsert(sName,iValue);
  // pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enameSet.insertStr -- add a name-string pair to ename-set                     */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetInsertStr(void)
{
  ENTRY();
  
  TagSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));

  s256 sValue;
  m_pDataStack->vPopStr(sValue,__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pEnameSet->vInsert(sName,sValue);
  //pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.replaceInt -- replace a name-integer pair to ename-set            */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetReplaceInt(void)
{
  ENTRY();
  
  TagSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  i8 iValue = m_pDataStack->iPopInt(__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pEnameSet->vReplace(sName,iValue);
  //pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.replaceStr -- replace a name-string pair to ename-set             */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetReplaceStr(void)
{
  ENTRY();
  
  TagSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));

  s256 sValue;
  m_pDataStack->vPopStr(sValue,__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pEnameSet->vReplace(sName,sValue);
  //pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TagSet.upsertStr -- replace/add a name-string pair to ename-set          */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetUpsertStr(void)
{
  ENTRY();
  
  TagSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  s256 sValue;
  m_pDataStack->vPopStr(sValue,__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  bool bHas = pEnameSet->bHasName(sName);
  
  if (bHas) {
    pEnameSet->vReplace(sName,sValue);
  }
  else {
    pEnameSet->vInsert(sName,sValue);
  }
  
  //pEnameSet->vPrint();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
