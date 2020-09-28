/*============================================================================*/
/* Kbase.cpp - Kbase                                                          */
/*============================================================================*/

#include "Facts.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Facts,Kbase,CN_KBASE)
__aNewFunction(Kbase,CN_KBASE)
__vDeleteFunction(Kbase,CN_KBASE)
__aFooterFunction(Kbase)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Kbase::Kbase(Stz* szKbName)
: Object(CN_KBASE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  m_pCurrent   = nullptr;
  m_pMarker    = nullptr;
  m_pTransform = nullptr;
  m_pPaste     = nullptr;

  vAprioriFacts();
 
  _checkInitialised_(CN_KBASE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Kbase::~Kbase(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  vTrace("~Kbase %p\n"_Stz,this);

  Fact::vDeleteRecursively(&m_pFact);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pAha(Stz* szConceptNm, Fact* pSuperConcept, eFU eUsage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pConcept = pSuperConcept->pNewInstance(szConceptNm);
  
  if (FU_NONE != eUsage)
    pConcept->rxValue().m_eUsage = eUsage;

  m_p_Concept_->vElementAppend(pConcept);

  return pConcept;
}

/*----------------------------------------------------------------------------*/
/* add a new quality attribute to given element                               */
/*----------------------------------------------------------------------------*/
void Kbase::vAtr(Stz* szConceptNm, Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pAttribute = pConceptInstance(szConceptNm);

  pElement->vAttributeAppend(pAttribute);
}

/*----------------------------------------------------------------------------*/
/* add a new int attribute to given element                                   */
/*----------------------------------------------------------------------------*/
void Kbase::vAtr(Stz* szConceptNm, i8 iVal, Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pAttribute = pConceptInstance(szConceptNm);
  pAttribute->rxValue().m_iInt = iVal;

  pElement->vAttributeAppend(pAttribute);
}

/*----------------------------------------------------------------------------*/
/* add a new text attribute to given element                                  */
/*----------------------------------------------------------------------------*/
void Kbase::vAtr(Stz* szConceptNm, Stz* szVal, Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pAttribute = pConceptInstance(szConceptNm);
  pAttribute->rxValue().m_pText = new Text;
  pAttribute->rxValue().m_pText->vSet(szVal);
  
  pElement->vAttributeAppend(pAttribute);
}

/*----------------------------------------------------------------------------*/
/* add a new xref attribute to given element                                  */
/*----------------------------------------------------------------------------*/
void Kbase::vAtr(Stz* szConceptNm, Fact* pXref, Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pAttribute = pConceptInstance(szConceptNm);
  pAttribute->rxValue().m_pXref = pXref;

  pElement->vAttributeAppend(pAttribute);
}

/*----------------------------------------------------------------------------*/
/* new attribute instance to named set                                        */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pAttributeInstance(Fact* pFact, Stz* szAtrSuperNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pAttribute = pConceptInstance(szAtrSuperNm);
  
  pFact->vAttributeAppend(pAttribute);
  
  return pAttribute;
}

/*----------------------------------------------------------------------------*/
/* new attribute instance to named set                                        */
/*----------------------------------------------------------------------------*/
void Kbase::vAttributeInstance(Fact* pFact, Stz* szAtrSuperNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pAttribute = pAttributeInstance(pFact,szAtrSuperNm);
}

/*----------------------------------------------------------------------------*/
/* new attribute instance to named set                                        */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pAttributeInstance(Fact* pFact, Stz* szAtrSuperNm, i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pAttribute = pConceptInstance(szAtrSuperNm);
  pAttribute->rxValue().m_iInt = iVal;

  pFact->vAttributeAppend(pAttribute);
  
  return pAttribute;
}

/*----------------------------------------------------------------------------*/
/* new attribute instance to named set                                        */
/*----------------------------------------------------------------------------*/
void Kbase::vAttributeInstance(Fact* pFact, Stz* szAtrSuperNm, i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pAttribute = pAttributeInstance(pFact,szAtrSuperNm,iVal);
}

/*----------------------------------------------------------------------------*/
/* new attribute instance to named set                                        */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pAttributeInstance(Fact* pFact, Stz* szAtrSuperNm, Stz* szVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pAttribute = pConceptInstance(szAtrSuperNm);
  pAttribute->rxValue().m_pText = new Text;
  pAttribute->rxValue().m_pText->vSet(szVal);

  pFact->vAttributeAppend(pAttribute);
  
  return pAttribute;
}

/*----------------------------------------------------------------------------*/
/* new attribute instance to named set                                        */
/*----------------------------------------------------------------------------*/
void Kbase::vAttributeInstance(Fact* pFact, Stz* szAtrSuperNm, Stz* szVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pAttribute = pAttributeInstance(pFact,szAtrSuperNm,szVal);
}

/*----------------------------------------------------------------------------*/
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pConceptAdd(Stz* szFactNm, eFU eUsage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pConcept = m_p_Concept_->pNewInstance(szFactNm);
  
  if (FU_NONE != eUsage)
    pConcept->rxValue().m_eUsage = eUsage;
  
  m_p_Concept_->vElementAppend(pConcept);

  return pConcept;
}

/*----------------------------------------------------------------------------*/
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
void Kbase::vConceptAdd(Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pConcept = m_p_Concept_->pNewInstance(szFactNm);
  
  m_p_Concept_->vElementAppend(pConcept);
}

/*----------------------------------------------------------------------------*/
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pConceptAdd(Fact* pSuper, Stz* szFactNm, eFU eUsage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pConcept = pSuper->pNewInstance(szFactNm);
  
  if (FU_NONE != eUsage)
    pConcept->rxValue().m_eUsage = eUsage;

  m_p_Concept_->vElementAppend(pConcept);

  return pConcept;
}

/*----------------------------------------------------------------------------*/
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
void Kbase::vConceptAdd(Fact* pSuper, Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pConcept = pSuper->pNewInstance(szFactNm);
  
  m_p_Concept_->vElementAppend(pConcept);
}

/*----------------------------------------------------------------------------*/
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
void Kbase::vConceptAdd(Stz* szSuperName, Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pSuper = m_p_Concept_->pElementFindNm(szSuperName);
  
  Fact* pConcept = pSuper->pNewInstance(szFactNm);
  
  m_p_Concept_->vElementAppend(pConcept);
}

/*----------------------------------------------------------------------------*/
/* new fact instance                                                          */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pConceptInstance(Stz* szAhaSuperNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pSuper = m_p_Concept_->pElementFindNm(szAhaSuperNm);
  
  Fact* pConcept = pSuper->pNewInstance();
  
  return pConcept;
}

/*----------------------------------------------------------------------------*/
/* get current working fact                                                   */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pCurrent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  return m_pCurrent;
}

/*----------------------------------------------------------------------------*/
/* set current working fact                                                   */
/*----------------------------------------------------------------------------*/
void Kbase::vCurrent(Fact* pCurrent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  m_pCurrent = pCurrent;
}

/*----------------------------------------------------------------------------*/
/* add an instance of a concept as an element to given set                    */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pElm(Stz* szConceptNm, Fact* pSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pElement = pConceptInstance(szConceptNm);
  
  pSet->vElementAppend(pElement);
  
  return pElement;
}

/*----------------------------------------------------------------------------*/
/* add an instance of a concept as an element to named set                    */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pElm(Stz* szConceptNm, Stz* szSetNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pElement = pConceptInstance(szConceptNm);
  
  Fact* pSet = pFactFind(szSetNm);
  
  pSet->vElementAppend(pElement);
  
  return pElement;
}

/*----------------------------------------------------------------------------*/
/* find element                                                               */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pElementFind(Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pElement = m_pContents->pElementFindNm(szNote);

  return pElement;
}

/*----------------------------------------------------------------------------*/
/* find element                                                               */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pElementFindAttr(Stz* szAttr, Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pElement = m_pContents->pElementFindNmVal(szAttr,szText);

  return pElement;
}

/*----------------------------------------------------------------------------*/
/* new element instance to given set                                          */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pElementInstance(Fact* pSet, Stz* szElmSuperNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pElement = pConceptInstance(szElmSuperNm);
  
  pSet->vElementAppend(pElement);
  
  return pElement;
}

/*----------------------------------------------------------------------------*/
/* new element instance to named set                                          */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pElementInstance(Stz* szSetNm, Stz* szElmSuperNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  Fact* pElement = pConceptInstance(szElmSuperNm);
  
  Fact* pSet = m_pContents->pElementFindNm(szSetNm);

  pSet->vElementAppend(pElement);
  
  return pElement;
}

/*----------------------------------------------------------------------------*/
/* get root fact                                                              */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pFact(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  return m_pFact;
}

/*----------------------------------------------------------------------------*/
/* find fact matching given fact no                                           */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pFactFind(i8 iFactNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pFound = m_pFact->pFind(iFactNo);
  
  return pFound;
}

/*----------------------------------------------------------------------------*/
/* find fact matching given fact name                                         */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pFactFind(Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pFound = m_pFact->pFind(szFactNm);

  return pFound;
}

/*----------------------------------------------------------------------------*/
/* new fact                                                                   */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pFactNew(Fact* pSuper, Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pFact = new Fact(this, pSuper, szFactNm);
  
  return pFact;
}

/*----------------------------------------------------------------------------*/
/* delete a fact                                                              */
/*----------------------------------------------------------------------------*/
void Kbase::vFactDelete(Fact* pFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  delete pFact;
}

/*----------------------------------------------------------------------------*/
/* find call with highest priority                                            */
/*----------------------------------------------------------------------------*/
void Kbase::vHighestCall(Fact*& rpCall)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);

  rpCall = nullptr;
  
  m_pContents->vHighestCall(rpCall);
}

/*----------------------------------------------------------------------------*/
/* get current working marker                                                 */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pMarker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  return m_pMarker;
}

/*----------------------------------------------------------------------------*/
/* set current working marker                                                 */
/*----------------------------------------------------------------------------*/
void Kbase::vMarker(Fact* pMarker)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  m_pMarker = pMarker;
}

/*----------------------------------------------------------------------------*/
/* add a new method as an attribute to given element                          */
/*----------------------------------------------------------------------------*/
Method* Kbase::pMth(Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pMethod = m_p_Method_->pNewInstance();
  pElement->vAttributeAppend(pMethod);
  
  pMethod->rxValue().m_pMethod = new Method(this,pElement);

  return pMethod->rxValue().m_pMethod;
}

/*----------------------------------------------------------------------------*/
/* generate next fact no                                                      */
/*----------------------------------------------------------------------------*/
i8 Kbase::iNextFactNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  i8 iFactNo = ++ m_ixFactNo;
  
  return iFactNo;
}

/*----------------------------------------------------------------------------*/
/* get pasted fact                                                            */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pPaste(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  return m_pPaste;
}

/*----------------------------------------------------------------------------*/
/* set pasted fact                                                            */
/*----------------------------------------------------------------------------*/
void Kbase::vPaste(Fact* pPaste)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  m_pPaste = pPaste;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Kbase::vPrint(File* pFile)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
//  Fact* pIntention = m_pContents->pElementFindNm("intention"_Stz);
//
//  pIntention->vPrint_r(pFile,0);

  m_pFact->vPrint_r(pFile,0);
}

/*----------------------------------------------------------------------------*/
/* get current working transform                                              */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pTransform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  return m_pTransform;
}

/*----------------------------------------------------------------------------*/
/* set current working transform                                              */
/*----------------------------------------------------------------------------*/
void Kbase::vTransform(Fact* pTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  m_pTransform = pTransform;
}

/*----------------------------------------------------------------------------*/
/* new a call to a transform for testing purposes                             */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pTransformCall(Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* p_Call_ = pFactFind("_call_"_Stz);
  Fact* pCall = new Fact(this,p_Call_,"call"_Stz);
  
  Fact* p_Name_ = pFactFind("_name_"_Stz);
  Fact* pName = new Fact(this,p_Name_,"name"_Stz);
  pName->vSetText(szFactNm);

  Fact* p_Arg_ = pFactFind("_arg_"_Stz);
  Fact* p_nArgs_ = pFactFind("_nArgs_"_Stz);

  i8 nArgs = 2;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Arg = 0; i_Arg < nArgs; i_Arg++) {

    s32 sArgId;
    sArgId.vSprint("arg%lld"_Stz,i_Arg);
    Fact* pArg = new Fact(this,p_Arg_,sArgId);
    
    s32 sArgNm;
    sArgNm.vSprint("%c"_Stz,'A' + i_Arg);

    pArg->vSetText(sArgNm);

    pCall->vAttributeAppend(pArg);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pnArgs = new Fact(this,p_nArgs_,"nArgs"_Stz);
  pnArgs->vSetInt(nArgs);
  pCall->vAttributeInsert(pnArgs);

  pCall->vAttributeInsert(pName);

  return pCall;
}

/*----------------------------------------------------------------------------*/
/* trim                                                                       */
/*----------------------------------------------------------------------------*/
void Kbase::vTrim(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pIntention = m_pContents->pElementFindNm("intention"_Stz);

  while (Fact* pElement = pIntention->pElements()) {
    //pIntention->vElementCut(pElement);
    Fact::vDeleteRecursively(&pElement);
  }
}

/*----------------------------------------------------------------------------*/
/* troll the kbase for calls to transforms and execute first one found        */
/*----------------------------------------------------------------------------*/
bool Kbase::bTroll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pCall = nullptr;
  
  vHighestCall(pCall);
  if (! pCall)
    return false; // don't call again

  Fact* pCallName = pCall->pAttributeFindNm("name"_Stz);
  
  if (! pCallName)
    return false; // don't call again
  
  s32 sName;
  sName = pCallName->rxValue().m_pText->szGet();
  Fact* pCalled = pElementFindAttr("transformNm"_Stz,sName);
  
  Fact* pMethodFact = pCalled->pAttributeFindNm("method"_Stz);
  
  Method* pMethod = pMethodFact->rxValue().m_pMethod;
  
  pMethod->vExecute(pCall);
  
  return true; // call again
}
/*----------------------------------------------------------------------------*/
//bool Kbase::bTroll(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
//  
//  Fact* pCall = pFactFind("call"_Stz);
//  
//  if (! pCall)
//    return false; // don't call again
//  
//  Fact* pCallName = pCall->pAttributeFindNm("name"_Stz);
//  
//  if (! pCallName)
//    return false; // don't call again
//  
//  s32 sName;
//  sName = pCallName->rxValue().m_pText->szGet();
//  Fact* pCalled = pElementFindAttr("transformNm"_Stz,sName);
//  
//  Fact* pMethodFact = pCalled->pAttributeFindNm("method"_Stz);
//  
//  Method* pMethod = pMethodFact->rxValue().m_pMethod;
//  
//  pMethod->vExecute(pCall);
//  
//  return true; // call again
//}

/*----------------------------------------------------------------------------*/
/* get property fact                                                          */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  return m_p_Value_;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* import state                                                               */
/*----------------------------------------------------------------------------*/
void Kbase::vImportStatement(Stz* szStmtToImport)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pStatement = m_pContents->pElementFindNm("statement"_Stz);

  Fact* p_SubQry_ = m_p_Concept_->pElementFindNm("_subQry_"_Stz);
  Fact* pSubQry = p_SubQry_->pNewInstance();
  pStatement->vElementAppend(pSubQry);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Fact* p_ProjectClause_ = m_p_Concept_->pElementFindNm("_projectClause_"_Stz);
  Fact* pProjectClause = p_ProjectClause_->pNewInstance();
  pSubQry->vElementAppend(pProjectClause);
  
  Fact* p_ExprList_ = m_p_Concept_->pElementFindNm("_exprList_"_Stz);
  Fact* pExprList = p_ExprList_->pNewInstance();
  pProjectClause->vElementAppend(pExprList);
  
  Fact* p_Expr_ = m_p_Concept_->pElementFindNm("_expr_"_Stz);
  Fact* pExpr = p_Expr_->pNewInstance();
  pExprList->vElementAppend(pExpr);
  
  Fact* p_Term_ = m_p_Concept_->pElementFindNm("_term_"_Stz);
  Fact* pTerm = p_Term_->pNewInstance();
  pExpr->vElementAppend(pTerm);
  
  Fact* p_Atom_ = m_p_Concept_->pElementFindNm("_atom_"_Stz);
  Fact* pAtom = p_Atom_->pNewInstance();
  pTerm->vElementAppend(pAtom);
  
  Fact* p_Text_ = m_p_Concept_->pElementFindNm("_text_"_Stz);
  Fact* pText = p_Text_->pNewInstance();
  pText->rxValue().m_pText = new Text;
  pText->rxValue().m_pText->vSet("*"_Stz);
  pAtom->vAttributeAppend(pText);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Fact* p_FromList_ = m_p_Concept_->pElementFindNm("_fromList_"_Stz);
  Fact* pFromList = p_FromList_->pNewInstance();
  pSubQry->vElementAppend(pFromList);
  
  Fact* p_FromTable_ = m_p_Concept_->pElementFindNm("_fromTable_"_Stz);
  Fact* pFromTable = p_FromTable_->pNewInstance();
  pFromList->vElementAppend(pFromTable);

  Fact* p_TableNm_ = m_p_Concept_->pElementFindNm("_tableNm_"_Stz);
  Fact* pTableNm = p_TableNm_->pNewInstance();
  pTableNm->rxValue().m_pText = new Text;
  pTableNm->rxValue().m_pText->vSet("lineitem"_Stz);
  pFromTable->vAttributeAppend(pTableNm);
}

/*----------------------------------------------------------------------------*/
/* import table                                                               */
/*----------------------------------------------------------------------------*/
void Kbase::vImportTable(Table* pTableToImport)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  vTrace("Import %s ...\n"_Stz,pTableToImport->szTableNm());
    
  Fact* pTable = pElementInstance("tables"_Stz,"_table_"_Stz);

  vAttributeInstance(pTable,"_tableNm_"_Stz,pTableToImport->szTableNm());

  Fact* pColumns = pElementInstance(pTable,"_columns_"_Stz);

  StreamDef* pStmDef = pTableToImport->pStreamDef();
  
  i8 nCols = pStmDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = pStmDef->pGetColumn(i_Col);
    eCT eColType = pCol->eColumnType();
    s32 s_ColType_;
    s_ColType_.vSprint("_%s_"_Stz,aCT[eColType]);
    
    Fact* pColumn = pElementInstance(pColumns,"_column_"_Stz);
    
    Fact* pColumnNo = pAttributeInstance(pColumn,"_columnNo_"_Stz,
                                         pCol->iColumnNo());
    //RASSERT(pColumnNo->bQuestion("isSubClassOf"_Stz,"_value_"_Stz));

    vAttributeInstance(pColumn,"_columnNm_"_Stz,pCol->szColNm());

    vAttributeInstance(pColumn,s_ColType_);

    vAttributeInstance(pColumn,"_length_"_Stz,pCol->iColumnLength());

    vAttributeInstance(pColumn,"_scale_"_Stz,pCol->iColumnScale());
    
    if (pCol->bIsNullable())
      vAttributeInstance(pColumn,"_nullable_"_Stz);
    else
      vAttributeInstance(pColumn,"_notNullable_"_Stz);
    
    if (pCol->bIsPtnKey())
      vAttributeInstance(pColumn,"_ptnKey_"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* test                                                                       */
/*----------------------------------------------------------------------------*/
void Kbase::vTest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  
  Fact* p_Shape_ = pConceptAdd(m_p_Folder_,"_shape_"_Stz);
  Fact* pShape = pElementInstance("intention"_Stz,"_shape_"_Stz);
  
  Fact* p_Bag_ = pConceptAdd(m_p_Folder_,"_bag_"_Stz);
  Fact* p_Die_ = pConceptAdd(m_p_Folder_,"_die_"_Stz);

  for (i8 i_Bag = 0; i_Bag < 5; i_Bag++) {
    
    
    if (1 == i_Bag) {
      
      Fact* pCall = pTransformCall("cut"_Stz);
      
      pShape->vElementAppend(pCall);
    }

    Fact* pBag = pElementInstance(pShape,"_bag_"_Stz);
    s32 sBagNm;
    sBagNm.vSprint("Bag%c"_Stz,'A'+i_Bag);
    vAttributeInstance(pBag,"_name_"_Stz,sBagNm);

    for (i8 i_Die = 0; i_Die < 4; i_Die++) {
      
      Fact* pDie = pElementInstance(pBag,"_die_"_Stz);
      s32 sDieNm;
      sDieNm.vSprint("Die%lld"_Stz,i_Die);
      vAttributeInstance(pDie,"_name_"_Stz,sDieNm);
    }
    
  }
 
  m_pFact->vPrint_r(nullptr,0);

  //m_p_Concept_->vPrint_r(nullptr,0,0);

  //m_pReference->vPrint_r(nullptr,0,1);
  
  //m_pContents->vPrint_r(nullptr,0,1);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add apriori facts                                                          */
/*----------------------------------------------------------------------------*/
void Kbase::vAprioriFacts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);

  m_pFact = pFactNew(nullptr,"fact"_Stz);
  
  m_p_Concept_ = pFactNew(m_pFact,"_concept_"_Stz);
  m_pFact->vElementAppend(m_p_Concept_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //m_p_Property_ = pConceptAdd(m_p_Concept_,"_property_"_Stz);
  m_p_Property_ = pAha("_property_"_Stz,m_p_Concept_);

  m_p_Value_    = pAha("_value_"_Stz,   m_p_Concept_, FU_USAGE);
  m_p_Int_      = pAha("_int_"_Stz,     m_p_Value_,   FU_INT);
  m_p_Function_ = pAha("_function_"_Stz,m_p_Value_,   FU_FUNCTION);
  m_p_Method_   = pAha("_method_"_Stz,  m_p_Value_,   FU_METHOD);
  m_p_Object_   = pAha("_object_"_Stz,  m_p_Value_,   FU_OBJECT);
  m_p_Opaque_   = pAha("_opaque_"_Stz,  m_p_Value_,   FU_OPAQUE);
  m_p_Text_     = pAha("_text_"_Stz,    m_p_Value_,   FU_TEXT);
  m_p_Word_     = pAha("_word_"_Stz,    m_p_Value_,   FU_WORD);
  m_p_Xref_     = pAha("_xref_"_Stz,    m_p_Value_,   FU_XREF);
  m_p_Name_     = pAha("_name_"_Stz,    m_p_Text_);
  m_p_No_       = pAha("_no_"_Stz,      m_p_Int_);

  m_p_Folder_   = pAha("_folder_"_Stz,  m_p_Concept_);
  m_p_Tuple_    = pAha("_tuple_"_Stz,   m_p_Concept_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pAha("_transforms_"_Stz, m_p_Concept_);
  pAha("_transform_"_Stz,  m_p_Concept_);
  pAha("_transformNm_"_Stz,m_p_Name_);
  pAha("_priority_"_Stz,   m_p_Int_);

  pAha("_arg_"_Stz,        m_p_Text_);
  pAha("_nArgs_"_Stz,      m_p_Int_);

//  pAha("_action_"_Stz,     m_p_Concept_);
  pAha("_call_"_Stz,       m_p_Concept_);
//  pAha("_chunk_"_Stz,      m_p_Concept_);
//  pAha("_condition_"_Stz,  m_p_Concept_);
//  pAha("_marker_"_Stz,     m_p_Concept_);
//  pAha("_pattern_"_Stz,    m_p_Concept_);
//  pAha("_rule_"_Stz,       m_p_Concept_);
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* p_Format_     = pAha("_format_"_Stz,    m_p_Text_);
  Fact* p_NoSeq_      = pAha("_noSeq_"_Stz,     m_p_Int_);
  Fact* p_NameSeq_    = pAha("_nameSeq_"_Stz,   m_p_Text_);

//  Fact* p_Questions_  = pAha("_questions_"_Stz, m_p_Folder_);
//  Fact* p_Question_   = pAha("_question_"_Stz,  p_Questions_);
//  Fact* p_QuestionNm_ = pAha("_questionNm_"_Stz,m_p_Name_);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Fact* p_Type_       =  pAha("_type_"_Stz,        m_p_Concept_);
  Fact* p_ColumnType_ =  pAha("_columnType_"_Stz,  p_Type_);
                         pAha("_BOOLEAN_"_Stz,     p_ColumnType_);
                         pAha("_DATE_"_Stz,        p_ColumnType_);
                         pAha("_DECIMAL_"_Stz,     p_ColumnType_);
                         pAha("_FLOAT_"_Stz,       p_ColumnType_);
                         pAha("_INTEGER_"_Stz,     p_ColumnType_);
                         pAha("_INTERVAL_"_Stz,    p_ColumnType_);
                         pAha("_TIME_"_Stz,        p_ColumnType_);
                         pAha("_TIMESTAMP_"_Stz,   p_ColumnType_);
                         pAha("_VARCHAR_"_Stz,     p_ColumnType_);
 
  Fact* p_Nullability_ = pAha("_nullability_"_Stz, m_p_Property_);
                         pAha("_nullable_"_Stz,    p_Nullability_);
                         pAha("_notNullable_"_Stz, p_Nullability_);

                         pAha("_ptnKey_"_Stz,      m_p_Property_);

                         pAha("_columns_"_Stz, m_p_Folder_);
                         pAha("_column_"_Stz,  m_p_Tuple_);
                         pAha("_columnNm_"_Stz,m_p_Name_);
                         pAha("_columnNo_"_Stz,m_p_Int_);
                         pAha("_length_"_Stz,  m_p_Int_);
                         pAha("_scale_"_Stz,   m_p_Int_);
 
                         pAha("_indexes_"_Stz, m_p_Folder_);
                         pAha("_index_"_Stz,   m_p_Folder_);
  
  Fact* p_Tables_ =      pAha("_tables_"_Stz,  m_p_Folder_);
                         pAha("_table_"_Stz,   m_p_Folder_);
                         pAha("_tableNm_"_Stz, m_p_Name_);
  
  Fact* p_TableRef_ =    pAha("_tableRef_"_Stz,  m_p_Xref_);
  Fact* p_ColumnRef_ =   pAha("_ColumnRef_"_Stz, m_p_Xref_);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* p_Statement_ = pAha("_statement_"_Stz,     m_p_Folder_);
                       pAha("_subQry_"_Stz,        m_p_Folder_);
  
                       pAha("_fromList_"_Stz,      m_p_Folder_);
                       pAha("_fromSeq_"_Stz,       p_NoSeq_);
                       pAha("_from_"_Stz,          m_p_Folder_);
                       pAha("_fromNm_"_Stz,        m_p_Name_);
                       pAha("_fromNo_"_Stz,        m_p_No_);
                       pAha("_fromTable_"_Stz,     p_TableRef_);
  
                       pAha("_projectList_"_Stz,   m_p_Folder_);
                       pAha("_projectSeq_"_Stz,    p_NoSeq_);
                       pAha("_project_"_Stz,       m_p_Folder_);
                       pAha("_projectNo_"_Stz,     m_p_Int_);
                       pAha("_projectNm_"_Stz,     m_p_Name_);
                       pAha("_projectColumn_"_Stz, p_TableRef_);
  
  Fact* p_AccessBy_  = pAha("_accessBy_"_Stz,      m_p_Property_);
                       pAha("_byScan"_Stz,         p_AccessBy_);
  Fact* p_BufferType_ = pAha("_bufferType_"_Stz,   m_p_Property_);
                       pAha("_crwBuffer"_Stz,      p_BufferType_);

                       pAha("_streamDefOut_"_Stz,  p_ColumnRef_);
  
                       pAha("_whereClause_"_Stz,   m_p_Folder_);
                       pAha("_groupByClause_"_Stz, m_p_Folder_);
                       pAha("_orderByClause_"_Stz, m_p_Folder_);

                       pAha("_exprList_"_Stz,      m_p_Folder_);
                       pAha("_expr_"_Stz,          m_p_Folder_);
                       pAha("_term_"_Stz,          m_p_Folder_);
                       pAha("_atom_"_Stz,          m_p_Tuple_);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

                       pAha("_plan_"_Stz,          m_p_Folder_);
                       pAha("_planPrime_"_Stz,     m_p_Int_);
  
                       pAha("_pipeline_"_Stz,      m_p_Folder_);
                       pAha("_pipelineNm_"_Stz,    m_p_Name_);
  
                       pAha("_stageList_"_Stz,     m_p_Folder_);
                       pAha("_stageSeq_"_Stz,      p_NoSeq_);
                       pAha("_stage_"_Stz,         m_p_Folder_);
                       pAha("_stageNm_"_Stz,       m_p_Name_);
                       pAha("_stageNo_"_Stz,       m_p_No_);
  
  Fact* p_Cardinality_ = pAha("_cardinality_"_Stz,           m_p_Property_);
                         pAha("all nodes"_Stz,               p_Cardinality_);
                         pAha("all partitions"_Stz,          p_Cardinality_);
                         pAha("all partitions on origin"_Stz,p_Cardinality_);
                         pAha("origin"_Stz,                  p_Cardinality_);
                         pAha("specific node"_Stz,           p_Cardinality_);
                         pAha("specific partition"_Stz,      p_Cardinality_);

                       pAha("_operatorList_"_Stz,  m_p_Folder_);
                       pAha("_operatorSeq_"_Stz,   p_NoSeq_);
                       pAha("_operator_"_Stz,      m_p_Folder_);
                       pAha("_operatorNo_"_Stz,    m_p_No_);
                       pAha("_operatorNm_"_Stz,    m_p_Name_);
  
                       pAha("_routine_"_Stz,       m_p_Folder_);
                       pAha("_routineNm_"_Stz,     m_p_Name_);

                       pAha("_handlerList_"_Stz,   m_p_Folder_);
                       pAha("_handlerSeq_"_Stz,    p_NoSeq_);
                       pAha("_handler_"_Stz,       m_p_Folder_);
                       pAha("_handlerNm_"_Stz,     m_p_Name_);
                       pAha("_handlerNo_"_Stz,     m_p_No_);

                       pAha("_subroutineList_"_Stz,m_p_Folder_);
                       pAha("_subroutineSeq_"_Stz, p_NoSeq_);
                       pAha("_subroutine_"_Stz,    m_p_Tuple_);
                       pAha("_subroutineNo_"_Stz,  m_p_No_);
                       pAha("_subroutineNm_"_Stz,  m_p_Name_);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  Fact* p_Operator_ =  pAha("_operator_"_Stz,      m_p_Tuple_);
                       pAha("_op_"_Stz,            p_Operator_);
                       pAha("_sqlOp_"_Stz,         p_Operator_);
  
                       pAha("_sourceCode_"_Stz,    m_p_Text_);
            
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  Fact* p_Nodes_      = pAha("_nodes_"_Stz,        m_p_Int_);
  Fact* p_Partitions_ = pAha("_partitions_"_Stz,   m_p_Int_);
  Fact* p_Grid_       = pAha("_grid_"_Stz,         m_p_Folder_);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  Fact* p_Contents_   = pAha("_contents_"_Stz,     m_p_Folder_);
  Fact* p_Reference_  = pAha("_reference_"_Stz,    m_p_Folder_);
  Fact* p_Intention_  = pAha("_intention_"_Stz,    m_p_Folder_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // /contents
  
  m_pContents         = pElm("_contents_"_Stz,     m_pFact);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  
  // /contents/reference/questions
  
//  Fact* pQuestions    = pElm("_questions_"_Stz,    pReference);
//
//  Fact* pQuestion     = pElm("_question_"_Stz,     pQuestions);
//                          vAtr("_questionNm_"_Stz,"isSubClassOf"_Stz,pQuestion);
//  Method* pMethod =       pMth(pQuestion);
//                            pMethod->vAddCode(TQ_IS_SUBCLASS_OF);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // /contents/reference/transforms
  
  Fact* pTransforms   = pElm("_transforms_"_Stz,   m_pContents);

  // /contents/reference/transforms/transform cut = Method{ cut }
  
//  Fact* pTransform    = pElm("_transform_"_Stz,    pTransforms);
//                          vAtr("_transformNm_"_Stz,"cut"_Stz,pTransform);
//  Method* pMethod =         pMth(pTransform);
//                            pMethod->vAddCode(TQ_CUT);
//                            pMethod->vAddCode(TQ_RETURN);

  // /contents/reference/transforms/transform paste = Method{ paste }
  
//  pTransform          = pElm("_transform_"_Stz,    pTransforms);
//                          vAtr("_transformNm_"_Stz,"paste"_Stz,pTransform);
//  pMethod =                 pMth(pTransform);
//                            pMethod->vAddCode(TQ_PASTE);
//                            pMethod->vAddCode(TQ_RETURN);

  // /contents/reference/transforms/transform schedule = Method{ schedule }
  
  Fact* pTransform    = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"schedule"_Stz,pTransform);
  Method* pMethod =         pMth(pTransform);
                            pMethod->vAddCode(TQ_SCHEDULE);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_ACCESS_METHODS"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_ACCESS_METHODS);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_ADD_ASM_HANDLERS"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_ADD_ASM_HANDLERS);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_ADD_ASM_SUBROUTINES"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_ADD_ASM_SUBROUTINES);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_ADD_OPERATOR"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_ADD_OPERATOR);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_ADD_OUTPUT_STAGE"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_ADD_OUTPUT_STAGE);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_ADD_PIPELINE"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_ADD_PIPELINE);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_ADD_START_STAGE"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_ADD_START_STAGE);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_FROM_RESOLUTION"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_FROM_RESOLUTION);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_IMPORT_TABLES"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_IMPORT_TABLES);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_INITIAL_PLAN"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_INITIAL_PLAN);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_OUTPUT_STREAMDEF"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_OUTPUT_STREAMDEF);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_PROJECT_RESOLUTION"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_PROJECT_RESOLUTION);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_WRITE_EOASM"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_WRITE_EOASM);
                            pMethod->vAddCode(TQ_RETURN);

  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"TQ_WRITE_PIPELINE"_Stz,pTransform);
  pMethod =                 pMth(pTransform);
                            pMethod->vAddCode(TQ_WRITE_PIPELINE);
                            pMethod->vAddCode(TQ_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // /contents/reference
  
  Fact* pReference    = pElm("_reference_"_Stz,    m_pContents);
  
  // /contents/reference/grid atr{ nodes = Int i partitions = Int i }
  
  Fact* pGrid         = pElm("_grid_"_Stz,         pReference);
                          vAtr("_nodes_"_Stz,AM::nDefinedNodes(), pGrid);
                          vAtr("_partitions_"_Stz,AM::nPartitions(),pGrid);

  
  // /contents/reference/tables
  
  pElm("_tables_"_Stz,pReference);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // /contents/intention
  
  //Fact* pIntention    = pElm("_intention_"_Stz,    m_pContents);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // /contents/intention/statement
  
  pElm("_statement_"_Stz,m_pContents);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // /contents/intention/pipeline

//  Fact* pPipeline = pElm("_pipeline_"_Stz,pIntention);
//
//  // /contents/intention/pipeline/stages atr{ nameseq = Text , format = Text }
//
//  Fact* pStages = pElm("_stageList_"_Stz,pPipeline);
//                    vAtr("_nameSeq_"_Stz,"A"_Stz,pStages);
//                    vAtr("_format_"_Stz,"stage%s"_Stz,pStages);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // /contents/intention/routines
  
//  Fact* pRoutines = pElm("_routine_"_Stz,pIntention);
//
//  // /contents/intention/subroutine
//
//  pElm("_subroutine_"_Stz,pRoutines);
}


/*
 
 fact
 
 /_concept_
   ...
 
 /contents

   /reference
     /questions
     /transforms

   /intention
     /grid
     /tables
     /statement
     /pipeline
     /routine
       /subroutine

 */

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/



/*----------------------------------------------------------------------------*/
/* add apriori facts                                                          */
/*----------------------------------------------------------------------------*/
/*
void Kbase::vAprioriFacts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);

  m_pFact = pFactNew(nullptr,"fact"_Stz);
  
  m_p_Concept_ = pFactNew(m_pFact,"_concept_"_Stz);
  m_pFact->vElementAppend(m_p_Concept_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //m_p_Property_ = pConceptAdd(m_p_Concept_,"_property_"_Stz);
  m_p_Property_ = pAha("_property_"_Stz,m_p_Concept_);

  m_p_Value_    = pAha("_value_"_Stz,   m_p_Concept_, FU_USAGE);
  m_p_Int_      = pAha("_int_"_Stz,     m_p_Value_,   FU_INT);
  m_p_Function_ = pAha("_function_"_Stz,m_p_Value_,   FU_FUNCTION);
  m_p_Method_   = pAha("_method_"_Stz,  m_p_Value_,   FU_METHOD);
  m_p_Object_   = pAha("_object_"_Stz,  m_p_Value_,   FU_OBJECT);
  m_p_Opaque_   = pAha("_opaque_"_Stz,  m_p_Value_,   FU_OPAQUE);
  m_p_Text_     = pAha("_text_"_Stz,    m_p_Value_,   FU_TEXT);
  m_p_Word_     = pAha("_word_"_Stz,    m_p_Value_,   FU_WORD);
  m_p_Xref_     = pAha("_xref_"_Stz,    m_p_Value_,   FU_XREF);
  m_p_Name_     = pAha("_name_"_Stz,    m_p_Text_);
  m_p_No_       = pAha("_no_"_Stz,      m_p_Int_);

  m_p_Folder_   = pAha("_folder_"_Stz,  m_p_Concept_);
  m_p_Tuple_    = pAha("_tuple_"_Stz,   m_p_Concept_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pAha("_action_"_Stz,     m_p_Concept_);
  pAha("_call_"_Stz,       m_p_Concept_);
  pAha("_chunk_"_Stz,      m_p_Concept_);
  pAha("_condition_"_Stz,  m_p_Concept_);
  pAha("_marker_"_Stz,     m_p_Concept_);
  pAha("_pattern_"_Stz,    m_p_Concept_);
  pAha("_rule_"_Stz,       m_p_Concept_);
  pAha("_transforms_"_Stz, m_p_Concept_);
  pAha("_transform_"_Stz,  m_p_Concept_);
  
  pAha("_arg_"_Stz,        m_p_Text_);
  pAha("_transformNm_"_Stz,m_p_Name_);
  pAha("_nArgs_"_Stz,      m_p_Int_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* p_NoSeq_      = pAha("_noSeq_"_Stz,     m_p_Int_);
  Fact* p_NameSeq_    = pAha("_nameSeq_"_Stz,   m_p_Text_);
  Fact* p_Format_     = pAha("_format_"_Stz,    m_p_Text_);

  Fact* p_Questions_  = pAha("_questions_"_Stz, m_p_Folder_);
  Fact* p_Question_   = pAha("_question_"_Stz,  p_Questions_);
  Fact* p_QuestionNm_ = pAha("_questionNm_"_Stz,m_p_Name_);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Fact* p_Type_       =  pAha("_type_"_Stz,        m_p_Concept_);
  Fact* p_ColumnType_ =  pAha("_columnType_"_Stz,  p_Type_);
                         pAha("_BOOLEAN_"_Stz,     p_ColumnType_);
                         pAha("_DATE_"_Stz,        p_ColumnType_);
                         pAha("_DECIMAL_"_Stz,     p_ColumnType_);
                         pAha("_FLOAT_"_Stz,       p_ColumnType_);
                         pAha("_INTEGER_"_Stz,     p_ColumnType_);
                         pAha("_INTERVAL_"_Stz,    p_ColumnType_);
                         pAha("_TIME_"_Stz,        p_ColumnType_);
                         pAha("_TIMESTAMP_"_Stz,   p_ColumnType_);
                         pAha("_VARCHAR_"_Stz,     p_ColumnType_);
 
  Fact* p_Nullability_ = pAha("_nullability_"_Stz, m_p_Property_);
                         pAha("_nullable_"_Stz,    p_Nullability_);
                         pAha("_notNullable_"_Stz, p_Nullability_);

                         pAha("_ptnKey_"_Stz,      m_p_Property_);

                         pAha("_columns_"_Stz, m_p_Folder_);
                         pAha("_column_"_Stz,  m_p_Tuple_);
                         pAha("_columnNm_"_Stz,m_p_Name_);
                         pAha("_columnNo_"_Stz,m_p_Int_);
                         pAha("_length_"_Stz,  m_p_Int_);
                         pAha("_scale_"_Stz,   m_p_Int_);
 
                         pAha("_indexes_"_Stz, m_p_Folder_);
                         pAha("_index_"_Stz,   m_p_Folder_);
  
  Fact* p_Tables_ =      pAha("_tables_"_Stz,  m_p_Folder_);
                         pAha("_table_"_Stz,   m_p_Folder_);
                         pAha("_tableNm_"_Stz, m_p_Name_);
  
  Fact* p_TableRef_ =    pAha("_tableRes_"_Stz,  m_p_Xref_);
  Fact* p_ColumnRef_ =   pAha("_ColumnRes_"_Stz, m_p_Xref_);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* p_Statement_ = pAha("_statement_"_Stz,     m_p_Folder_);
                       pAha("_subQry_"_Stz,        m_p_Folder_);
  
                       pAha("_fromList_"_Stz,      m_p_Folder_);
                       pAha("_fromSeq_"_Stz,       p_NoSeq_);
                       pAha("_from_"_Stz,          m_p_Folder_);
                       pAha("_fromNm_"_Stz,        m_p_Name_);
                       pAha("_fromNo_"_Stz,        m_p_No_);
                       pAha("_fromTable_"_Stz,     p_TableRef_);
  
                       pAha("_projectList_"_Stz,   m_p_Folder_);
                       pAha("_projectSeq_"_Stz,    p_NoSeq_);
                       pAha("_project_"_Stz,       m_p_Folder_);
                       pAha("_projectNo_"_Stz,     m_p_Int_);
                       pAha("_projectNm_"_Stz,     m_p_Name_);
                       pAha("_projectColumn_"_Stz, p_TableRef_);
  
  Fact* p_AccessBy_  = pAha("_accessBy_"_Stz,      m_p_Property_);
                       pAha("_byScan"_Stz,         p_AccessBy_);
  Fact* p_BufferType_ = pAha("_bufferType_"_Stz,   m_p_Property_);
                       pAha("_crwBuffer"_Stz,      p_BufferType_);

                       pAha("_streamDefOut_"_Stz,  p_ColumnRef_);
  
                       pAha("_whereClause_"_Stz,   m_p_Folder_);
                       pAha("_groupByClause_"_Stz, m_p_Folder_);
                       pAha("_orderByClause_"_Stz, m_p_Folder_);

                       pAha("_exprList_"_Stz,      m_p_Folder_);
                       pAha("_expr_"_Stz,          m_p_Folder_);
                       pAha("_term_"_Stz,          m_p_Folder_);
                       pAha("_atom_"_Stz,          m_p_Tuple_);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

                       pAha("_plan_"_Stz,          m_p_Folder_);
                       pAha("_planPrime_"_Stz,     m_p_Int_);
  
                       pAha("_pipeline_"_Stz,      m_p_Folder_);
                       pAha("_pipelineNm_"_Stz,    m_p_Name_);
  
                       pAha("_stageList_"_Stz,     m_p_Folder_);
                       pAha("_stageSeq_"_Stz,      p_NoSeq_);
                       pAha("_stage_"_Stz,         m_p_Folder_);
                       pAha("_stageNm_"_Stz,       m_p_Name_);
                       pAha("_stageNo_"_Stz,       m_p_No_);
  
  Fact* p_Cardinality_ = pAha("_cardinality_"_Stz,           m_p_Property_);
                         pAha("all nodes"_Stz,               p_Cardinality_);
                         pAha("all partitions"_Stz,          p_Cardinality_);
                         pAha("all partitions on origin"_Stz,p_Cardinality_);
                         pAha("origin"_Stz,                  p_Cardinality_);
                         pAha("specific node"_Stz,           p_Cardinality_);
                         pAha("specific partition"_Stz,      p_Cardinality_);

                       pAha("_operatorList_"_Stz,  m_p_Folder_);
                       pAha("_operatorSeq_"_Stz,   p_NoSeq_);
                       pAha("_operator_"_Stz,      m_p_Folder_);
                       pAha("_operatorNo_"_Stz,    m_p_No_);
                       pAha("_operatorNm_"_Stz,    m_p_Name_);
  
                       pAha("_routine_"_Stz,       m_p_Folder_);
                       pAha("_routineNm_"_Stz,     m_p_Name_);

                       pAha("_handlerList_"_Stz,   m_p_Folder_);
                       pAha("_handlerSeq_"_Stz,    p_NoSeq_);
                       pAha("_handler_"_Stz,       m_p_Folder_);
                       pAha("_handlerNm_"_Stz,     m_p_Name_);
                       pAha("_handlerNo_"_Stz,     m_p_No_);

                       pAha("_subroutineList_"_Stz,m_p_Folder_);
                       pAha("_subroutineSeq_"_Stz, p_NoSeq_);
                       pAha("_subroutine_"_Stz,    m_p_Tuple_);
                       pAha("_subroutineNo_"_Stz,  m_p_No_);
                       pAha("_subroutineNm_"_Stz,  m_p_Name_);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  Fact* p_Operator_ =  pAha("_operator_"_Stz,      m_p_Tuple_);
                       pAha("_op_"_Stz,            p_Operator_);
                       pAha("_sqlOp_"_Stz,         p_Operator_);
  
                       pAha("_sourceCode_"_Stz,    m_p_Text_);
            
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  Fact* p_Nodes_      = pAha("_nodes_"_Stz,        m_p_Int_);
  Fact* p_Partitions_ = pAha("_partitions_"_Stz,   m_p_Int_);
  Fact* p_Grid_       = pAha("_grid_"_Stz,         m_p_Folder_);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  Fact* p_Contents_   = pAha("_contents_"_Stz,     m_p_Folder_);
  Fact* p_Reference_  = pAha("_reference_"_Stz,    m_p_Folder_);
  Fact* p_Intention_  = pAha("_intention_"_Stz,    m_p_Folder_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // /contents
  
  m_pContents         = pElm("_contents_"_Stz,     m_pFact);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // /contents/reference
  
  Fact* pReference    = pElm("_reference_"_Stz,    m_pContents);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // /contents/reference/questions
  
  Fact* pQuestions    = pElm("_questions_"_Stz,    pReference);
  
  Fact* pQuestion     = pElm("_question_"_Stz,     pQuestions);
                          vAtr("_questionNm_"_Stz,"isSubClassOf"_Stz,pQuestion);
//  Fact* pMethod = m_p_Method_->pNewInstance();
//  pQuestion->vAttributeAppend(pMethod);
//  pMethod->rxValue().m_pMethod = new Method(this,pQuestion);
//  pMethod->rxValue().m_pMethod->vAddCode(TQ_IS_SUBCLASS_OF);
  Method* pMethod =       pMth(pQuestion);
                            pMethod->vAddCode(TQ_IS_SUBCLASS_OF);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // /contents/reference/transforms
  
  Fact* pTransforms   = pElm("_transforms_"_Stz,   pReference);

  // /contents/reference/transforms/transform cut = Method{ cut }
  
  Fact* pTransform    = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"cut"_Stz,pTransform);
//  pMethod = m_p_Method_->pNewInstance();
//  pTransform->vAttributeAppend(pMethod);
//  pMethod->rxValue().m_pMethod = new Method(this,pTransform);
//  pMethod->rxValue().m_pMethod->vAddCode(TQ_CUT);
//  pMethod->rxValue().m_pMethod->vAddCode(TQ_RETURN);
  pMethod =               pMth(pTransform);
                            pMethod->vAddCode(TQ_CUT);
                            pMethod->vAddCode(TQ_RETURN);

  // /contents/reference/transforms/transform paste = Method{ paste }
  
  pTransform          = pElm("_transform_"_Stz,    pTransforms);
                          vAtr("_transformNm_"_Stz,"paste"_Stz,pTransform);
//  pMethod = m_p_Method_->pNewInstance();
//  pTransform->vAttributeAppend(pMethod);
//  pMethod->rxValue().m_pMethod = new Method(this,pTransform);
//  pMethod->rxValue().m_pMethod->vAddCode(TQ_PASTE);
//  pMethod->rxValue().m_pMethod->vAddCode(TQ_RETURN);
  pMethod =               pMth(pTransform);
                            pMethod->vAddCode(TQ_PASTE);
                            pMethod->vAddCode(TQ_RETURN);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // /contents/intention
  
  Fact* pIntention    = pElm("_intention_"_Stz,    m_pContents);

  // /contents/intention/grid atr{ nodes = Int i partitions = Int i }
  
  Fact* pGrid         = pElm("_grid_"_Stz,         pIntention);
                          vAtr("_nodes_"_Stz,AM::nDefinedNodes(), pGrid);
                          vAtr("_partitions_"_Stz,AM::nPartitions(),pGrid);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // /contents/intention/tables
  
  pElm("_tables_"_Stz,pIntention);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // /contents/intention/statement
  
  pElm("_statement_"_Stz,pIntention);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // /contents/intention/pipeline

  Fact* pPipeline = pElm("_pipeline_"_Stz,pIntention);
  
  // /contents/intention/pipeline/stages atr{ nameseq = Text , format = Text }

  Fact* pStages = pElm("_stageList_"_Stz,pPipeline);
                    vAtr("_nameSeq_"_Stz,"A"_Stz,pStages);
                    vAtr("_format_"_Stz,"stage%s"_Stz,pStages);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // /contents/intention/routines
  
  Fact* pRoutines = pElm("_routine_"_Stz,pIntention);
  
  // /contents/intention/subroutine
  
  pElm("_subroutine_"_Stz,pRoutines);
}

*/
