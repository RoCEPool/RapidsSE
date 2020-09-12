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
/* add a concept                                                              */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pConceptAdd(Fact* pSuper, Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pConcept = pSuper->pNewInstance(szNote);
  
  m_p_Concept_->vElementAppend(pConcept);

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
Fact* Kbase::pFactNew(Fact* pSuper, Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pFact = new Fact(this, pSuper, szNote);
  
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
/* find element                                                               */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pElementFind(Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pElement = m_pContents->pElementFindNm(szNote);

  return pElement;
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
    
  m_pContents->vPrint_r(pFile,0,1);
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
/* borrow/new a transform                                                     */
/*----------------------------------------------------------------------------*/
Transform* Kbase::pTransformBorrow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Transform* pTransform = new Transform(this);
  
  return pTransform;
}

/*----------------------------------------------------------------------------*/
/* return/delete a transform                                                  */
/*----------------------------------------------------------------------------*/
void Kbase::vTransformReturn(Transform** ppTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  delete *ppTransform;
  
  *ppTransform = nullptr;
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
  
  Fact* pTables  = m_pContents->pElementFindNm("tables"_Stz);
  Fact* p_Table_       = m_p_Concept_->pElementFindNm("_table_"_Stz);
  Fact* p_TableNm_     = m_p_Concept_->pElementFindNm("_tableNm_"_Stz);
  Fact* p_Columns_     = m_p_Concept_->pElementFindNm("_columns_"_Stz);
  Fact* p_Column_      = m_p_Concept_->pElementFindNm("_column_"_Stz);
  Fact* p_ColumnNm_    = m_p_Concept_->pElementFindNm("_columnNm_"_Stz);
  Fact* p_ColumnNo_    = m_p_Concept_->pElementFindNm("_columnNo_"_Stz);
  Fact* p_ColumnType_  = m_p_Concept_->pElementFindNm("_columnType_"_Stz);
  Fact* p_Length_      = m_p_Concept_->pElementFindNm("_length_"_Stz);
  Fact* p_Scale_       = m_p_Concept_->pElementFindNm("_scale_"_Stz);
  Fact* p_Nullable_    = m_p_Concept_->pElementFindNm("_nullable_"_Stz);
  Fact* p_NotNullable_ = m_p_Concept_->pElementFindNm("_notNullable_"_Stz);
  Fact* p_PtnKey_      = m_p_Concept_->pElementFindNm("_ptnKey_"_Stz);

  Fact* pTable = p_Table_->pNewInstance();
  pTables->vElementAppend(pTable);

  Fact* pTableNm = p_TableNm_->pNewInstance();
  pTableNm->rxValue().m_pText = new Text;
  pTableNm->rxValue().m_pText->vSet(pTableToImport->szTableNm());
  pTable->vAttributeAppend(pTableNm);

  Fact* pColumns = p_Columns_->pNewInstance();
  pTable->vElementAppend(pColumns);

  StreamDef* pStmDef = pTableToImport->pStreamDef();
  
  i8 nCols = pStmDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column* pCol = pStmDef->pGetColumn(i_Col);
    
    Fact* pColumn = p_Column_->pNewInstance();
    pColumns->vElementAppend(pColumn);
    
    i8 iColumnNo = pCol->iColumnNo();
    Fact* pColumnNo = p_ColumnNo_->pNewInstance();
    
    RASSERT(pColumnNo->bQuestion("isSubClassOf"_Stz,"_value_"_Stz));
    
    pColumnNo->rxValue().m_iInt = iColumnNo;
    pColumn->vAttributeAppend(pColumnNo);

    Fact* pColumnNm = p_ColumnNm_->pNewInstance();
    pColumnNm->rxValue().m_pText = new Text;
    pColumnNm->rxValue().m_pText->vSet(pCol->szColNm());
    pColumn->vAttributeAppend(pColumnNm);

    eCT eColType = pCol->eColumnType();
    s32 s_ColType_;
    s_ColType_.vSprint("_%s_"_Stz,aCT[eColType]);
    Fact* p_ColType_ = m_p_Concept_->pElementFindNm(s_ColType_);
    Fact* pColType = p_ColType_->pNewInstance();
    pColumn->vAttributeAppend(pColType);
    
    i8 iLength = pCol->iColumnLength();
    Fact* pLength = p_Length_->pNewInstance();
    pLength->rxValue().m_iInt = iLength;
    pColumn->vAttributeAppend(pLength);

    i8 iScale = pCol->iColumnScale();
    Fact* pScale = p_Scale_->pNewInstance();
    pScale->rxValue().m_iInt = iScale;
    pColumn->vAttributeAppend(pScale);
    
    bool bIsNullable = pCol->bIsNullable();
    if (bIsNullable) {
      Fact* pNullable = p_Nullable_->pNewInstance();
      pColumn->vAttributeAppend(pNullable);
    }
    else {
      Fact* pNotNullable = p_NotNullable_->pNewInstance();
      pColumn->vAttributeAppend(pNotNullable);
    }
    
    bool bIsPtnKey = pCol->bIsPtnKey();
    if (bIsPtnKey) {
      Fact* pPtnKey = p_PtnKey_->pNewInstance();
      pColumn->vAttributeAppend(pPtnKey);
    }

  }
}


/*----------------------------------------------------------------------------*/
/* test                                                                       */
/*----------------------------------------------------------------------------*/
void Kbase::vTest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
 
  m_pFact->vPrint_r(nullptr,0,0);

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
  
  m_p_Chunk_ = pConceptAdd(m_p_Concept_,"_chunk_"_Stz);
  m_p_Pattern_ = pConceptAdd(m_p_Concept_,"_pattern_"_Stz);
  m_p_Transform_ = pConceptAdd(m_p_Concept_,"_transform_"_Stz);

  
  m_p_Value_ = pConceptAdd(m_p_Concept_,"_value_"_Stz);
  m_p_Value_->rxValue().m_eUsage = FU_USAGE;

  m_p_Function_ = pConceptAdd(m_p_Value_,"_function_"_Stz);
  m_p_Function_->rxValue().m_eUsage = FU_FUNCTION;

  m_p_Int_ = pConceptAdd(m_p_Value_,"_int_"_Stz);
  m_p_Int_->rxValue().m_eUsage = FU_INT;

  m_p_Method_ = pConceptAdd(m_p_Value_,"_method_"_Stz);
  m_p_Method_->rxValue().m_eUsage = FU_METHOD;

  m_p_Object_ = pConceptAdd(m_p_Value_,"_object_"_Stz);
  m_p_Object_->rxValue().m_eUsage = FU_OBJECT;

  m_p_Opaque_ = pConceptAdd(m_p_Value_,"_opaque_"_Stz);
  m_p_Opaque_->rxValue().m_eUsage = FU_OPAQUE;

  m_p_Text_ = pConceptAdd(m_p_Value_,"_text_"_Stz);
  m_p_Text_->rxValue().m_eUsage = FU_TEXT;

  m_p_Word_ = pConceptAdd(m_p_Value_,"_word_"_Stz);
  m_p_Word_->rxValue().m_eUsage = FU_WORD;

  m_p_Xref_ = pConceptAdd(m_p_Value_,"_xref_"_Stz);
  m_p_Xref_->rxValue().m_eUsage = FU_XREF;

  
  m_p_Name_ = pConceptAdd(m_p_Text_,"_name_"_Stz);
  m_p_NameSeq_ = pConceptAdd(m_p_Text_,"_nameSeq_"_Stz);
  m_p_Format_ = pConceptAdd(m_p_Text_,"_format_"_Stz);

  m_p_Type_ = pConceptAdd(m_p_Concept_,"_type_"_Stz);
  m_p_Tuple_ = pConceptAdd(m_p_Concept_,"_tuple_"_Stz);
  m_p_Folder_ = pConceptAdd(m_p_Concept_,"_folder_"_Stz);

  m_p_Quality_ = pConceptAdd(m_p_Concept_,"_quality_"_Stz);
  
  m_p_Questions_ = pConceptAdd(m_p_Folder_,"_questions_"_Stz);
  m_p_Question_ = pConceptAdd(m_p_Questions_,"_question_"_Stz);
  Fact* p_QuestionNm_ = pConceptAdd(m_p_Name_,"_questionNm_"_Stz);
  
  m_p_Rule_ = pConceptAdd(m_p_Concept_,"_rule_"_Stz);
  m_p_Condition_ = pConceptAdd(m_p_Concept_,"_condition_"_Stz);
  m_p_Action_ = pConceptAdd(m_p_Concept_,"_action_"_Stz);

  Fact* p_Nullability_   = pConceptAdd(m_p_Quality_,"_nullability_"_Stz);
  Fact* p_ColumnType_    = pConceptAdd(m_p_Type_,"_columnType_"_Stz);
  Fact* p_Tables_        = pConceptAdd(m_p_Folder_,"_tables_"_Stz);
  
  Fact* p_Statement_     = pConceptAdd(m_p_Folder_,"_statement_"_Stz);
  Fact* p_SubQry_        = pConceptAdd(m_p_Folder_,"_subQry_"_Stz);
  Fact* p_ProjectClause_ = pConceptAdd(m_p_Folder_,"_projectClause_"_Stz);
  Fact* p_FromClause_    = pConceptAdd(m_p_Folder_,"_fromClause_"_Stz);
  Fact* p_FromTable_     = pConceptAdd(m_p_Tuple_, "_fromTable_"_Stz);
  Fact* p_WhereClause_   = pConceptAdd(m_p_Folder_,"_whereClause_"_Stz);
  Fact* p_GroupByClause_ = pConceptAdd(m_p_Folder_,"_groupByClause_"_Stz);
  Fact* p_OrderByClause_ = pConceptAdd(m_p_Folder_,"_orderByClause_"_Stz);

  Fact* p_ExprList_      = pConceptAdd(m_p_Folder_,"_exprList_"_Stz);
  Fact* p_Expr_          = pConceptAdd(m_p_Folder_,"_expr_"_Stz);
  Fact* p_Term_          = pConceptAdd(m_p_Folder_,"_term_"_Stz);
  Fact* p_Atom_          = pConceptAdd(m_p_Tuple_,"_atom_"_Stz);

  pConceptAdd(m_p_Folder_,"_pipeline_"_Stz);
  pConceptAdd(m_p_Name_,"_pipelineNm_"_Stz);
  pConceptAdd(m_p_Folder_,"_stages_"_Stz);
  pConceptAdd(m_p_Folder_,"_stage_"_Stz);
  pConceptAdd(m_p_Name_,"_stageNm_"_Stz);
  Fact* p_Cardinality_ = pConceptAdd(m_p_Quality_,"_cardinality_"_Stz);
  pConceptAdd(p_Cardinality_,"all nodes"_Stz);
  pConceptAdd(p_Cardinality_,"all partitions"_Stz);
  pConceptAdd(p_Cardinality_,"all partitions on origin"_Stz);
  pConceptAdd(p_Cardinality_,"origin"_Stz);
  pConceptAdd(p_Cardinality_,"specific node"_Stz);
  pConceptAdd(p_Cardinality_,"specific partition"_Stz);
  Fact* p_Operator_      = pConceptAdd(m_p_Tuple_,"_operator_"_Stz);
  pConceptAdd(p_Operator_,"_op_"_Stz);
  pConceptAdd(p_Operator_,"_sqlOp_"_Stz);
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pConceptAdd(p_ColumnType_,"_BOOLEAN_"_Stz);
  pConceptAdd(p_ColumnType_,"_DATE_"_Stz);
  pConceptAdd(p_ColumnType_,"_DECIMAL_"_Stz);
  pConceptAdd(p_ColumnType_,"_FLOAT_"_Stz);
  pConceptAdd(p_ColumnType_,"_INTEGER_"_Stz);
  pConceptAdd(p_ColumnType_,"_INTERVAL_"_Stz);
  pConceptAdd(p_ColumnType_,"_TIME_"_Stz);
  pConceptAdd(p_ColumnType_,"_TIMESTAMP_"_Stz);
  pConceptAdd(p_ColumnType_,"_VARCHAR_"_Stz);

  pConceptAdd(p_Nullability_,"_nullable_"_Stz);
  pConceptAdd(p_Nullability_,"_notNullable_"_Stz);

  pConceptAdd(m_p_Quality_,"_ptnKey_"_Stz);

  pConceptAdd(m_p_Folder_,"_table_"_Stz);
  pConceptAdd(m_p_Name_,"_tableNm_"_Stz);
  
  pConceptAdd(m_p_Folder_,"_columns_"_Stz);
  pConceptAdd(m_p_Tuple_,"_column_"_Stz);
  pConceptAdd(m_p_Int_,"_columnNo_"_Stz);
  pConceptAdd(m_p_Name_,"_columnNm_"_Stz);
  pConceptAdd(m_p_Int_,"_length_"_Stz);
  pConceptAdd(m_p_Int_,"_scale_"_Stz);

  pConceptAdd(m_p_Folder_,"_indexes_"_Stz);
  pConceptAdd(m_p_Folder_,"_index_"_Stz);
  
  pConceptAdd(m_p_Folder_,"_routines_"_Stz);
  pConceptAdd(m_p_Tuple_,"_subroutine_"_Stz);
  pConceptAdd(m_p_Name_,"_subroutineNm_"_Stz);
  pConceptAdd(m_p_Quality_,"_recallable_"_Stz);
  pConceptAdd(m_p_Text_,"_sourceCode_"_Stz);

  Fact* p_Nodes_ = pConceptAdd(m_p_Int_,"_nodes_"_Stz);
  Fact* p_Partitions_ = pConceptAdd(m_p_Int_,"_partitions_"_Stz);
  Fact* p_Grid_ = pConceptAdd(m_p_Folder_,"_grid_"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pContents = pConceptAdd(m_p_Folder_,"_contents_"_Stz);
  m_pContents = pContents->pNewInstance();//pFactNew(m_pFolder,"contents"_Stz);
  m_pFact->vElementAppend(m_pContents);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* p_Reference_ = pConceptAdd(m_p_Folder_,"_reference_"_Stz);
  m_pReference = p_Reference_->pNewInstance();
  m_pContents->vElementAppend(m_pReference);
  
  Fact* pQuestions = m_p_Questions_->pNewInstance();
  m_pReference->vElementAppend(pQuestions);

  Fact* pQuestion = m_p_Question_->pNewInstance();
  pQuestions->vElementAppend(pQuestion);
  Fact* pQuestionNm = p_QuestionNm_->pNewInstance();
  pQuestionNm->rxValue().m_pText = new Text;
  pQuestionNm->rxValue().m_pText->vSet("isSubClassOf"_Stz);
  pQuestion->vAttributeAppend(pQuestionNm);
  Fact* pMethod = m_p_Method_->pNewInstance();
  pMethod->rxValue().m_pMethod = new Method;
  pMethod->rxValue().m_pMethod->vAddCode(QC_IS_SUBCLASS_OF);
  pQuestion->vAttributeAppend(pMethod);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Fact* pGrid = p_Grid_->pNewInstance();
  m_pContents->vElementAppend(pGrid);
  Fact* pNodes = p_Nodes_->pNewInstance();
  pNodes->rxValue().m_iInt = AM::nDefinedNodes();
  pGrid->vAttributeAppend(pNodes);
  Fact* pPartitions = p_Partitions_->pNewInstance();
  pPartitions->rxValue().m_iInt = AM::nPartitions();
  pGrid->vAttributeAppend(pPartitions);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pTables = p_Tables_->pNewInstance();
  m_pContents->vElementAppend(pTables);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* pStatement = p_Statement_->pNewInstance();
  m_pContents->vElementAppend(pStatement);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* p_Pipeline_ = m_p_Concept_->pElementFindNm("_pipeline_"_Stz);
  Fact* pPipeline = p_Pipeline_->pNewInstance();
  m_pContents->vElementAppend(pPipeline);

  Fact* p_Stages_ = m_p_Concept_->pElementFindNm("_stages_"_Stz);
  Fact* pStages = p_Stages_->pNewInstance();
  pPipeline->vElementAppend(pStages);

  Fact* pNameSeq = m_p_NameSeq_->pNewInstance();
  pNameSeq->rxValue().m_pText = new Text;
  pNameSeq->rxValue().m_pText->vSet("A"_Stz);
  pStages->vAttributeAppend(pNameSeq);

  Fact* pFormat = m_p_Format_->pNewInstance();
  pFormat->rxValue().m_pText = new Text;
  pFormat->rxValue().m_pText->vSet("stage%s"_Stz);
  pStages->vAttributeAppend(pFormat);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Fact* p_Routines_ = m_p_Concept_->pElementFindNm("_routines_"_Stz);
  Fact* pRoutines = p_Routines_->pNewInstance();
  m_pContents->vElementAppend(pRoutines);

  Fact* p_Subroutines_ = m_p_Concept_->pElementFindNm("_subroutine_"_Stz);
  Fact* pSubroutines = p_Subroutines_->pNewInstance();
  pRoutines->vElementAppend(pSubroutines);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
