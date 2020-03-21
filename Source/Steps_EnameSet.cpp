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
  
  vAddStep("enameSet.addInt"_Stz,     &Step::vEnameSetAddInt);
  vAddStep("enameSet.addStr"_Stz,     &Step::vEnameSetAddStr);
  vAddStep("enameSet.asStr"_Stz,      &Step::vEnameSetAsStr);
  vAddStep("enameSet.copy"_Stz,       &Step::vEnameSetCopy);
  vAddStep("enameSet.getInt"_Stz,     &Step::vEnameSetGetInt);
  vAddStep("enameSet.getStr"_Stz,     &Step::vEnameSetGetStr);
  vAddStep("enameSet.hasName"_Stz,    &Step::vEnameSetHasName);
  vAddStep("enameSet.replaceInt"_Stz, &Step::vEnameSetReplaceInt);
  vAddStep("enameSet.replaceStr"_Stz, &Step::vEnameSetReplaceStr);
  vAddStep("enameSet.upsertStr"_Stz,  &Step::vEnameSetUpsertStr);
}

/*----------------------------------------------------------------------------*/
/* EnameSet::new -- new ename-set                                             */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetNew(void)
{
  ENTRY(); // ENTRY("vEnameSetNew"_Stz);
  
  EnameSet* pEnameSet = new EnameSet;
  
  m_pDataStack->vPushPtr(pEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* EnameSet.addInt -- add a name-integer pair to ename-set                    */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetAddInt(void)
{
  ENTRY(); // ENTRY("vEnameSetAddInt"_Stz);
  
  EnameSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  i8 iValue = m_pDataStack->iPopInt(__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pEnameSet->vInsert(sName,iValue);
  // pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.addStr -- add a name-string pair to ename-set                     */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetAddStr(void)
{
  ENTRY(); // ENTRY("vEnameSetAddStr"_Stz);
  
  EnameSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));

  s256 sValue;
  m_pDataStack->vPopStr(sValue,__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pEnameSet->vInsert(sName,sValue);
  //pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.asStr -- get ename-set as a single string                         */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetAsStr(void)
{
  ENTRY(); // ENTRY("vEnameSetAsStr"_Stz);
  
  EnameSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  s4K sStr;
  SP_EnmSet spEnameSet;
  sStr = spEnameSet.sz(pEnameSet);
  
  m_pDataStack->vPushStr(sStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.copy -- copy source set into this destination set                 */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetCopy(void)
{
  ENTRY(); // ENTRY("vEnameSetCopy"_Stz);
  
  EnameSet* pEnameSet_dst = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  EnameSet* pEnameSet_src = _pEnameSet(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pEnameSet_dst->vCopy(pEnameSet_src);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.getInt -- get int value from ename-set with given name            */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetGetInt(void)
{
  ENTRY(); // ENTRY("vEnameSetGetInt"_Stz);
  
  EnameSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  i8 iValue;
  iValue = pEnameSet->iValue(sName);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.getStr -- get str value from ename-set with given name            */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetGetStr(void)
{
  ENTRY(); // ENTRY("vEnameSetGetStr"_Stz);
  
  EnameSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
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
/* EnameSet.hasName -- test if ename-set has a pair with given name           */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetHasName(void)
{
  ENTRY(); // ENTRY("vEnameSetHasName"_Stz);
  
  EnameSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);

  bool bHas = pEnameSet->bHasName(sName);
  
  m_pDataStack->vPushInt(bHas);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.replaceInt -- replace a name-integer pair to ename-set            */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetReplaceInt(void)
{
  ENTRY(); // ENTRY("vEnameSetReplaceInt"_Stz);
  
  EnameSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  i8 iValue = m_pDataStack->iPopInt(__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pEnameSet->vReplace(sName,iValue);
  //pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.replaceStr -- replace a name-string pair to ename-set             */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetReplaceStr(void)
{
  ENTRY(); // ENTRY("vEnameSetReplaceStr"_Stz);
  
  EnameSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));

  s256 sValue;
  m_pDataStack->vPopStr(sValue,__func__);
  
  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pEnameSet->vReplace(sName,sValue);
  //pEnameSet->vPrint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* EnameSet.upsertStr -- replace/add a name-string pair to ename-set          */
/*----------------------------------------------------------------------------*/
void Step::vEnameSetUpsertStr(void)
{
  ENTRY(); // ENTRY("vEnameSetAddStr"_Stz);
  
  EnameSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
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
