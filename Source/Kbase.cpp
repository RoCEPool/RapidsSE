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
  
  m_pCurrent = nullptr;
  
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
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
Fact* Kbase::pConceptAdd(Stz* szName, eFU eUsage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pConcept = m_p_Concept_->pNewInstance(szName);
  
  if (FU_NONE != eUsage)
    pConcept->rxValue().m_eUsage = eUsage;
  
  m_p_Concept_->vElementAppend(pConcept);

  return pConcept;
}

/*----------------------------------------------------------------------------*/
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
void Kbase::vConceptAdd(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pConcept = m_p_Concept_->pNewInstance(szName);
  
  m_p_Concept_->vElementAppend(pConcept);
}

/*----------------------------------------------------------------------------*/
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pConceptAdd(Fact* pSuper, Stz* szName, eFU eUsage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pConcept = pSuper->pNewInstance(szName);
  
  if (FU_NONE != eUsage)
    pConcept->rxValue().m_eUsage = eUsage;

  m_p_Concept_->vElementAppend(pConcept);

  return pConcept;
}

/*----------------------------------------------------------------------------*/
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
void Kbase::vConceptAdd(Fact* pSuper, Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pConcept = pSuper->pNewInstance(szName);
  
  m_p_Concept_->vElementAppend(pConcept);
}

/*----------------------------------------------------------------------------*/
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
void Kbase::vConceptAdd(Stz* szSuperName, Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pSuper = m_p_Concept_->pElementFindNm(szSuperName);
  
  Fact* pConcept = pSuper->pNewInstance(szName);
  
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
/* find element                                                               */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pElementFind(Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pElement = m_pContents->pElementFindNm(szNote);

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
/* new fact                                                                   */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pFactNew(Fact* pSuper, Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pFact = new Fact(this, pSuper, szName);
  
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
/* generate next fact no                                                      */
/*----------------------------------------------------------------------------*/
i8 Kbase::iNextFactNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  i8 iFactNo = ++ m_ixFactNo;
  
  return iFactNo;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Kbase::vPrint(File* pFile)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  m_pContents->vPrint_r(pFile,0);
}

/*----------------------------------------------------------------------------*/
/* borrow/new a question                                                      */
/*----------------------------------------------------------------------------*/
Question* Kbase::pQuestionBorrow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Question* pQuestion = new Question(this);
  
  return pQuestion;
}

/*----------------------------------------------------------------------------*/
/* return/delete a question                                                   */
/*----------------------------------------------------------------------------*/
void Kbase::vQuestionReturn(Question** ppQuestion)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  delete *ppQuestion;
  
  *ppQuestion = nullptr;
}

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
  
  Fact* p_FromClause_ = m_p_Concept_->pElementFindNm("_fromClause_"_Stz);
  Fact* pFromClause = p_FromClause_->pNewInstance();
  pSubQry->vElementAppend(pFromClause);
  
  Fact* p_FromTable_ = m_p_Concept_->pElementFindNm("_fromTable_"_Stz);
  Fact* pFromTable = p_FromTable_->pNewInstance();
  pFromClause->vElementAppend(pFromTable);

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
    RASSERT(pColumnNo->bQuestion("isSubClassOf"_Stz,"_value_"_Stz));

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
  
  m_p_Property_ = pConceptAdd(m_p_Concept_,"_property_"_Stz);

  m_p_Value_ = pConceptAdd("_value_"_Stz,FU_USAGE);
  m_p_Int_ = pConceptAdd(m_p_Value_,"_int_"_Stz,FU_INT);
  m_p_Function_ = pConceptAdd(m_p_Value_,"_function_"_Stz,FU_FUNCTION);
  m_p_Method_ = pConceptAdd(m_p_Value_,"_method_"_Stz,FU_METHOD);
  m_p_Object_ = pConceptAdd(m_p_Value_,"_object_"_Stz,FU_OBJECT);
  m_p_Opaque_ = pConceptAdd(m_p_Value_,"_opaque_"_Stz,FU_OPAQUE);
  m_p_Transform_ = pConceptAdd(m_p_Value_,"_transform_"_Stz,FU_TRANSFORM);
  m_p_Text_ = pConceptAdd(m_p_Value_,"_text_"_Stz,FU_TEXT);
  m_p_Word_ = pConceptAdd(m_p_Value_,"_word_"_Stz,FU_WORD);
  m_p_Xref_ = pConceptAdd(m_p_Value_,"_xref_"_Stz,FU_XREF);
  m_p_Name_ = pConceptAdd(m_p_Text_,"_name_"_Stz);
  
  m_p_Folder_ = pConceptAdd(m_p_Concept_,"_folder_"_Stz);
  m_p_Tuple_ = pConceptAdd(m_p_Concept_,"_tuple_"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vConceptAdd("_chunk_"_Stz);
  vConceptAdd("_pattern_"_Stz);
  vConceptAdd("_transform_"_Stz);

  vConceptAdd(m_p_Concept_,"_rule_"_Stz);
  vConceptAdd(m_p_Concept_,"_condition_"_Stz);
  vConceptAdd(m_p_Concept_,"_action_"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Fact* p_NameSeq_ = pConceptAdd(m_p_Text_,"_nameSeq_"_Stz);
  Fact* p_Format_ = pConceptAdd(m_p_Text_,"_format_"_Stz);

  Fact* p_Questions_ = pConceptAdd(m_p_Folder_,"_questions_"_Stz);
  Fact* p_Question_ = pConceptAdd(p_Questions_,"_question_"_Stz);
  Fact* p_QuestionNm_ = pConceptAdd(m_p_Name_,"_questionNm_"_Stz);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Fact* p_Type_ = pConceptAdd(m_p_Concept_,"_type_"_Stz);
  Fact* p_ColumnType_ = pConceptAdd(p_Type_,"_columnType_"_Stz);
  vConceptAdd(p_ColumnType_,"_BOOLEAN_"_Stz);
  vConceptAdd(p_ColumnType_,"_DATE_"_Stz);
  vConceptAdd(p_ColumnType_,"_DECIMAL_"_Stz);
  vConceptAdd(p_ColumnType_,"_FLOAT_"_Stz);
  vConceptAdd(p_ColumnType_,"_INTEGER_"_Stz);
  vConceptAdd(p_ColumnType_,"_INTERVAL_"_Stz);
  vConceptAdd(p_ColumnType_,"_TIME_"_Stz);
  vConceptAdd(p_ColumnType_,"_TIMESTAMP_"_Stz);
  vConceptAdd(p_ColumnType_,"_VARCHAR_"_Stz);

  Fact* p_Nullability_ = pConceptAdd(m_p_Property_,"_nullability_"_Stz);
  vConceptAdd(p_Nullability_,"_nullable_"_Stz);
  vConceptAdd(p_Nullability_,"_notNullable_"_Stz);

  vConceptAdd(m_p_Property_,"_ptnKey_"_Stz);

  Fact* p_Cardinality_ = pConceptAdd(m_p_Property_,"_cardinality_"_Stz);
  vConceptAdd(p_Cardinality_,"all nodes"_Stz);
  vConceptAdd(p_Cardinality_,"all partitions"_Stz);
  vConceptAdd(p_Cardinality_,"all partitions on origin"_Stz);
  vConceptAdd(p_Cardinality_,"origin"_Stz);
  vConceptAdd(p_Cardinality_,"specific node"_Stz);
  vConceptAdd(p_Cardinality_,"specific partition"_Stz);

  vConceptAdd(m_p_Folder_,"_columns_"_Stz);
  vConceptAdd(m_p_Tuple_,"_column_"_Stz);
  vConceptAdd(m_p_Int_,"_columnNo_"_Stz);
  vConceptAdd(m_p_Name_,"_columnNm_"_Stz);
  vConceptAdd(m_p_Int_,"_length_"_Stz);
  vConceptAdd(m_p_Int_,"_scale_"_Stz);

  vConceptAdd(m_p_Folder_,"_indexes_"_Stz);
  vConceptAdd(m_p_Folder_,"_index_"_Stz);
  
  Fact* p_Tables_ = pConceptAdd(m_p_Folder_,"_tables_"_Stz);
  vConceptAdd(m_p_Folder_,"_table_"_Stz);
  vConceptAdd(m_p_Name_,"_tableNm_"_Stz);
  
  
  Fact* p_Statement_ = pConceptAdd(m_p_Folder_,"_statement_"_Stz);
  vConceptAdd(m_p_Folder_,"_subQry_"_Stz);
  vConceptAdd(m_p_Folder_,"_projectClause_"_Stz);
  vConceptAdd(m_p_Folder_,"_fromClause_"_Stz);
  vConceptAdd(m_p_Tuple_, "_fromTable_"_Stz);
  vConceptAdd(m_p_Folder_,"_whereClause_"_Stz);
  vConceptAdd(m_p_Folder_,"_groupByClause_"_Stz);
  vConceptAdd(m_p_Folder_,"_orderByClause_"_Stz);

  vConceptAdd(m_p_Folder_,"_exprList_"_Stz);
  vConceptAdd(m_p_Folder_,"_expr_"_Stz);
  vConceptAdd(m_p_Folder_,"_term_"_Stz);
  vConceptAdd(m_p_Tuple_,"_atom_"_Stz);

  vConceptAdd(m_p_Folder_,"_pipeline_"_Stz);
  vConceptAdd(m_p_Name_,"_pipelineNm_"_Stz);
  vConceptAdd(m_p_Folder_,"_stages_"_Stz);
  vConceptAdd(m_p_Folder_,"_stage_"_Stz);
  vConceptAdd(m_p_Name_,"_stageNm_"_Stz);
  
  Fact* p_Operator_ = pConceptAdd(m_p_Tuple_,"_operator_"_Stz);
  vConceptAdd(p_Operator_,"_op_"_Stz);
  vConceptAdd(p_Operator_,"_sqlOp_"_Stz);
  
  vConceptAdd(m_p_Folder_,"_routines_"_Stz);
  vConceptAdd(m_p_Tuple_,"_subroutine_"_Stz);
  vConceptAdd(m_p_Name_,"_subroutineNm_"_Stz);
  vConceptAdd(m_p_Property_,"_recallable_"_Stz);
  vConceptAdd(m_p_Text_,"_sourceCode_"_Stz);

  Fact* p_Nodes_ = pConceptAdd(m_p_Int_,"_nodes_"_Stz);
  Fact* p_Partitions_ = pConceptAdd(m_p_Int_,"_partitions_"_Stz);
  Fact* p_Grid_ = pConceptAdd(m_p_Folder_,"_grid_"_Stz);

  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  Fact* p_Contents_ = pConceptAdd(m_p_Folder_,"_contents_"_Stz);
  
  m_pContents = pElementInstance(m_pFact,"_contents_"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* p_Reference_ = pConceptAdd(m_p_Folder_,"_reference_"_Stz);
  
  Fact* pReference = pElementInstance(m_pContents,"_reference_"_Stz);
  
  Fact* pQuestions = pElementInstance(pReference,"_questions_"_Stz);
  
  Fact* pQuestion = pElementInstance(pQuestions,"_question_"_Stz);
  pAttributeInstance(pQuestion,"_questionNm_"_Stz,"isSubClassOf"_Stz);
  Fact* pMethod = m_p_Method_->pNewInstance();
  pQuestion->vAttributeAppend(pMethod);
  pMethod->rxValue().m_pMethod = new Method;
  pMethod->rxValue().m_pMethod->vAddCode(QC_IS_SUBCLASS_OF);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  
  Fact* pGrid = pElementInstance(m_pContents,"_grid_"_Stz);
  pAttributeInstance(pGrid,"_nodes_"_Stz,AM::nDefinedNodes());
  pAttributeInstance(pGrid,"_partitions_"_Stz,AM::nPartitions());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pElementInstance(m_pContents,"_tables_"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pElementInstance(m_pContents,"_statement_"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pPipeline = pElementInstance(m_pContents,"_pipeline_"_Stz);
  Fact* pStages = pElementInstance(pPipeline,"_stages_"_Stz);
  pAttributeInstance(pStages,"_nameSeq_"_Stz,"A"_Stz);
  pAttributeInstance(pStages,"_format_"_Stz,"stage%s"_Stz);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pRoutines = pElementInstance(m_pContents,"_routines_"_Stz);
  pElementInstance(pRoutines,"_subroutine_"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
