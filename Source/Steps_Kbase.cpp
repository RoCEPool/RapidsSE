/*============================================================================*/
/* Steps_Kbase.cpp -- kbase                                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Kbase(void)
{
  vAddStep("Kbase[]"_Stz,              &Step::vKbaseSet);
    
  vAddStep("Kbase[].delete"_Stz,       &Step::vKbaseDelete);
  vAddStep("Kbase[].findFactNm"_Stz,   &Step::vKbaseFindFactNm);
  vAddStep("Kbase[].findFactNo"_Stz,   &Step::vKbaseFindFactNo);
  vAddStep("Kbase[].delete"_Stz,       &Step::vKbaseDelete);
  vAddStep("Kbase[].get"_Stz,          &Step::vKbaseGet);
  vAddStep("Kbase[].marker@"_Stz,      &Step::vKbaseMarkerFetch);
  vAddStep("Kbase[].marker!"_Stz,      &Step::vKbaseMarkerStore);
  vAddStep("Kbase[].markerNew"_Stz,    &Step::vKbaseMarkerNew);
  vAddStep("Kbase[].new"_Stz,          &Step::vKbaseNew);
  vAddStep("Kbase[].print"_Stz,        &Step::vKbasePrint);
  vAddStep("Kbase[].set"_Stz,          &Step::vKbaseSet);
  vAddStep("Kbase[].test"_Stz,         &Step::vKbaseTest);
  vAddStep("Kbase[].transform@"_Stz,   &Step::vKbaseTransformFetch);
  vAddStep("Kbase[].transform!"_Stz,   &Step::vKbaseTransformStore);
  vAddStep("Kbase[].transformNew"_Stz, &Step::vKbaseTransformNew);
  vAddStep("Kbase[].trim"_Stz,         &Step::vKbaseTrim);
}

/*----------------------------------------------------------------------------*/
/* Kbase[].delete -- delete kbase                                             */
/*----------------------------------------------------------------------------*/
void Step::vKbaseDelete(void)
{
  ENTRY();
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;
  m_pLogicUnit->m_xpKbase = nullptr;
  
  delete pKbase;
      
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].findFactNm -- find fact by name                                    */
/*----------------------------------------------------------------------------*/
void Step::vKbaseFindFactNm(void)
{
  ENTRY();
  
  s32 sFactNm;
  m_pDataStack->vPopStr(sFactNm,__func__);

  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;
      
  Fact* pFact = pKbase->pFactFind(sFactNm);
  
  m_pDataStack->vPushPtr(pFact);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].findFactNo -- find fact by factNo                                  */
/*----------------------------------------------------------------------------*/
void Step::vKbaseFindFactNo(void)
{
  ENTRY();
  
  i8 iFactNo = m_pDataStack->iPopInt(__func__);

  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;
  
  Fact* pFact = pKbase->pFactFind(iFactNo);
  
  m_pDataStack->vPushPtr(pFact);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].get -- return pointer to kbase                                     */
/*----------------------------------------------------------------------------*/
void Step::vKbaseGet(void)
{
  ENTRY();
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;

  m_pDataStack->vPushPtr(pKbase);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].marker@ -- fetch marker                                            */
/*----------------------------------------------------------------------------*/
void Step::vKbaseMarkerFetch(void)
{
  ENTRY();
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;
  
  Fact* pMarker = pKbase->pMarker();
  
  m_pDataStack->vPushPtr(pMarker);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].markerNew -- new marker                                            */
/*----------------------------------------------------------------------------*/
void Step::vKbaseMarkerNew(void)
{
  ENTRY();
    
  s32 sFactNm;
  m_pDataStack->vPopStr(sFactNm,__func__);

  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;
  
  
  Fact* p_Marker_ = pKbase->pFactFind("_marker_"_Stz);
  Fact* pMarker = new Fact(pKbase,p_Marker_,"marker"_Stz);
  
  
  Fact* p_Name_ = pKbase->pFactFind("_name_"_Stz);
  Fact* pName = new Fact(pKbase,p_Name_,"name"_Stz);
  pName->vSetText(sFactNm);
  
  pMarker->vAttributeAppend(pName);
  
  m_pDataStack->vPushPtr(pMarker);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].marker! -- store marker                                            */
/*----------------------------------------------------------------------------*/
void Step::vKbaseMarkerStore(void)
{
  ENTRY();
  
  Fact* pMarker  = _pFact(m_pDataStack->pPopPtr(__func__));

  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;

  pKbase->vMarker(pMarker);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].new -- create a new kbase                                          */
/*----------------------------------------------------------------------------*/
void Step::vKbaseNew(void)
{
  ENTRY();
  
  s32 sKbaseNm;
  m_pDataStack->vPopStr(sKbaseNm,__func__);
  
  m_pLogicUnit->m_xpKbase = new Kbase(sKbaseNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].print -- print kbase                                               */
/*----------------------------------------------------------------------------*/
void Step::vKbasePrint(void)
{
  ENTRY();
  
  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);
    
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;
  
  File* pFile = new File(nullptr);
  
  bool bResult = pFile->bOpenWrite(sPath);
  
  if (bResult) {
    
    pKbase->vPrint(pFile);
    
    pFile->vClose();
  }
  
  delete pFile;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].set -- set reg Kbase                                               */
/*----------------------------------------------------------------------------*/
void Step::vKbaseSet(void)
{
  ENTRY();
  
  Kbase* pKbase = _pKbase(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpKbase = pKbase;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].test -- run kbase test                                             */
/*----------------------------------------------------------------------------*/
void Step::vKbaseTest(void)
{
  ENTRY();
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;

  pKbase->vTest();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].transform@ -- fetch transform                                      */
/*----------------------------------------------------------------------------*/
void Step::vKbaseTransformFetch(void)
{
  ENTRY();
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;
  
  Fact* pTransform = pKbase->pTransform();
  
  m_pDataStack->vPushPtr(pTransform);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].transformNew -- new transform                                      */
/*----------------------------------------------------------------------------*/
void Step::vKbaseTransformNew(void)
{
  ENTRY();
    
  
  s32 sFactNm;
  m_pDataStack->vPopStr(sFactNm,__func__);

  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;
  
  
  Fact* p_Transform_ = pKbase->pFactFind("_transform_"_Stz);
  Fact* pTransform = new Fact(pKbase,p_Transform_,"transform"_Stz);
  
  
  Fact* p_Name_ = pKbase->pFactFind("_name_"_Stz);
  Fact* pName = new Fact(pKbase,p_Name_,"name"_Stz);
  pName->vSetText(sFactNm);
  
  
  Fact* p_Arg_ = pKbase->pFactFind("_arg_"_Stz);

  s32 sArg;
  
  for (;;) {
    m_pDataStack->vPopStr(sArg,__func__);
    if (sArg.bSame("!"_Stz))
      break;
  
    Fact* pArg = new Fact(pKbase,p_Arg_,"arg"_Stz);
    pArg->vSetText(sArg);

    pTransform->vAttributeInsert(pArg);
  }

  pTransform->vAttributeInsert(pName);
  
  m_pDataStack->vPushPtr(pTransform);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].transform! -- store transform                                      */
/*----------------------------------------------------------------------------*/
void Step::vKbaseTransformStore(void)
{
  ENTRY();
  
  Fact* pTransform  = _pFact(m_pDataStack->pPopPtr(__func__));

  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;

  pKbase->vTransform(pTransform);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Kbase[].trim -- trim kbase                                                 */
/*----------------------------------------------------------------------------*/
void Step::vKbaseTrim(void)
{
  ENTRY();
  
  Kbase* pKbase = * m_pLogicUnit->m_xpKbase;
  
  pKbase->vTrim();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
