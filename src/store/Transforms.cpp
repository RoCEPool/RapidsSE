/*============================================================================*/
/* Transforms.cpp --                                                          */
/*============================================================================*/

#include "Transforms.hpp"
#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Transforms,CN_TRANSFORMS)
__aNewFunction(Transforms,CN_TRANSFORMS)
__vDeleteFunction(Transforms,CN_TRANSFORMS)
__aFooterFunction(Transforms)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Transforms::Transforms(Bench* pBench)
: Modifications(CN_TRANSFORMS, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  _checkInitialised_(CN_TRANSFORMS);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Transforms::~Transforms(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vAdd_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  i8 iAggreg    = 0;
  i8 iIndexJoin = 0;
  i8 iFilter    = 0;
  i8 iOther     = 0;
  i8 iProject   = 0;
  i8 iScan      = 0;
  i8 iSort      = 0;

  BoardIter xBI(m_pBoard);

  i8 nMainCards = _deck_->nChildCards();
  while ( (m_pMainCard = xBI.pEachMain()) ) {

    m_pNextMain = xBI.pNextMain();
    if (m_pNextMain)
      m_eNextMain = m_pNextMain->eCard();
    else
      m_eNextMain = CK_NONE;

    switch (eDecodeMain(m_pMainCard)) {

      case CK_AGGREG:
        RASSERT(m_p_aggreg == m_pMainCard);
        iAggreg++;
        m_p_combined = new Card_Combined;
        _deck_->vInsertAfter(m_p_combined, m_p_aggreg);
        break;

      case CK_FILTER:
        RASSERT(m_p_filter == m_pMainCard);
        iFilter++;
        m_p_combined = new Card_Combined;
        _deck_->vInsertAfter(m_p_combined, m_p_filter);
        break;

      case CK_INDEXJOIN:
        RASSERT(m_p_indexJoin == m_pMainCard);
        iIndexJoin++;
        if (CK_AGGREG == m_eNextMain || CK_FILTER == m_eNextMain || CK_PROJECT == m_eNextMain)
          break;
        m_p_combined = new Card_Combined;
        _deck_->vInsertAfter(m_p_combined, m_p_indexJoin);
        break;

      case CK_PROJECT:
        RASSERT(m_p_project == m_pMainCard);
        iProject++;
        m_p_combined = new Card_Combined;
        _deck_->vInsertAfter(m_p_combined, m_p_project);
        break;

      case CK_SORT:
        RASSERT(m_p_sort == m_pMainCard);
        iSort++;
        m_p_combined = new Card_Combined;
        _deck_->vInsertAfter(m_p_combined, m_p_sort);
        break;

      default:
        iOther++;
        break;
    }
  }

  vSetPrintSubCards(CK_AGGELIST, true);
  vSetPrintSubCards(CK_GBYELIST, true);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vAdd_scanPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain(CK_TABLE)) ) {

    m_p_filter = m_pMainCard->pSub_filter();
    if (! m_p_filter)
      continue;

    m_p_indexList = m_pMainCard->pSub_indexList();
    if (! m_p_indexList)
      continue;

    m_p_scanPred = nullptr;

    // - - - - - - - - -
    auto bfX = [this](Card* pCard) {
      Card_CmpPred* p_predCmp = _pCard_CmpPred(pCard);
      if (! ('L' == p_predCmp->cMark1() || 'U' == p_predCmp->cMark1()) )
        return true;

      if (! this->m_p_scanPred) {
        m_p_scanPred = new Card_ScanPred;
        m_p_scan->vAppend(m_p_scanPred);
      }

      if ('L' == p_predCmp->cMark1()) {
        m_p_lBound = m_p_scanPred->pSub_lBound();
        if (! m_p_lBound) {
          m_p_lBound = new Card_LBound;
          m_p_scanPred->vInsert(m_p_lBound);
        }
        m_p_lBound->vAppend(p_predCmp);
      }
      else
      if ('U' == p_predCmp->cMark1()) {
        m_p_uBound = m_p_scanPred->pSub_uBound();
        if (! m_p_uBound) {
          m_p_uBound = new Card_UBound;
          m_p_scanPred->vAppend(m_p_uBound);
        }
        m_p_uBound->vAppend(p_predCmp);
      }

      return true;
    };

    // - - - - - - - - -

    m_p_filter->vVisitRecursive(CK_CMPPRED, bfX);

  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* linearize stages                                                           */
/*----------------------------------------------------------------------------*/
void Transforms::vAdd_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  i8 iFilter    = 0;
  i8 iIndexJoin = 0;
  i8 iOther     = 0;
  i8 iProject   = 0;
  i8 iScan      = 0;
  i8 iSort      = 0;
  i8 iStage     = 0;

  eYG eStageTopo = YG_NONE;
  Card_StageTopo* pCard_StageTopo = nullptr;

  BoardIter xBI(m_pBoard);
  while ( (m_pMainCard = xBI.pEachMain()) ) {

    if (! xBI.bMore())
      break;

    m_pNextMain = xBI.pNextMain();
    m_eNextMain = m_pNextMain->eCard();
    if (CK_STAGE == m_eNextMain)
      continue;

    switch (eDecodeMain(m_pMainCard)) {

      case CK_FILTER:
        RASSERT(m_p_filter == m_pMainCard);
        iFilter++;

        if (CK_INDEXJOIN == m_eNextMain)
          break;

        m_pChildCard = m_p_stage->pDupCardAndSubCards();
        _deck_->vInsertAfter(m_pChildCard, m_p_filter);
        xBI.vForward();
        break;

      case CK_INDEXJOIN:
        RASSERT(m_p_indexJoin == m_pMainCard);
        iIndexJoin++;

        if (CK_PROJECT == m_eNextMain || CK_FILTER == m_eNextMain)
          break;

        m_pDupCard = m_p_stage->pDupCardAndSubCards();
        _deck_->vInsertAfter(m_pDupCard, m_p_indexJoin);
        xBI.vForward();
        break;

      case CK_PROJECT:
        RASSERT(m_p_project == m_pMainCard);
        iProject++;

        m_pDupCard = m_p_stage->pDupCardAndSubCards();
        _deck_->vInsertAfter(m_pDupCard, m_p_project);
        xBI.vForward();
        break;

      case CK_SCAN:
        RASSERT(m_p_scan == m_pMainCard);
        iScan++;
        RASSERT(1 == iScan);
        break;

      case CK_SORT:
        RASSERT(m_p_sort == m_pMainCard);
        iSort++;

        m_pDupCard = m_p_stage->pDupCardAndSubCards();
        _deck_->vInsertAfter(m_pDupCard, m_p_sort);
        xBI.vForward();
        break;

      case CK_STAGE:
        RASSERT(m_p_stage == m_pMainCard);
        iStage++;

        m_p_stageTopo = m_p_stage->pSub_stageTopo();
        m_eStageTopo = m_p_stageTopo->eStageTopo();
        break;

      default:
        iOther++;
        break;
    }
  }

  i8 i_Stage = 1;
  xBI.vRestart();
  while ( (m_p_stage = _pCard_Stage(xBI.pEachMain(CK_STAGE))) )
    m_p_stage->vRenameStage(i_Stage++);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* add table partitions <tablePtn> to all <table>                             */
/*----------------------------------------------------------------------------*/
void Transforms::vAdd_tablePtn_ToAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  while (I_bEach_mainCard_(CK_TABLE)) {
    //m_p_tablePtn = new Card_TablePtn;
    A_vNew_card(CK_TABLEPTN);
    m_p_tablePtn->vAdd_TablePtn(m_p_table);
    m_p_table->vAppend(m_p_tablePtn);
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* add table synonyms <tableSyn> to all <table>                               */
/*----------------------------------------------------------------------------*/
void Transforms::vAdd_tableSyn_ToAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  char cTableSyn = 'a';

  BoardIter xBI(m_pBoard);

  while ( (m_p_table = _pCard_Table(xBI.pEachMain(CK_TABLE)) )) {

    m_p_tablePtr = _pSub_TablePtr(m_p_table);
    m_pTable = m_p_tablePtr->pTable();

    char cTableSyn_actual = cTableSyn;
    if (m_pTable->bIsReplicated())
      cTableSyn_actual += 'A'-'a';
    m_p_table->vPpy_chr(PPY_TableSyn,cTableSyn_actual);

    cTableSyn++;
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vAppend_indexList_ToAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ( (m_p_table = _pCard_Table(xBI.pEachMain(CK_TABLE))) ) {
    m_p_tablePtr = m_p_table->pSub_tablePtr();
    if ( (m_p_indexList = m_p_tablePtr->pNew_indexList()) )
      m_p_table->vAppend(m_p_indexList);
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* append <stream> to all <table>                                             */
/*----------------------------------------------------------------------------*/
void Transforms::vAppend_stream_ToAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  // E <scan>
  while ((m_p_table = _pCard_Table(xBI.pEachMain(CK_TABLE)))) {

    // ? <scan>/<tablePtr>
    if (! (m_p_tablePtr = m_p_table->pSub_tablePtr()))
      continue;
    // ? <scan>/<tablePtr>:pStreamDef
    if (! (m_pStreamDef = m_p_tablePtr->pStreamDef()))
      continue;

    // + <scan>/<stream>
    m_p_table->vAppend((m_p_stream = new Card_Stream));
    // + <scan>/<stream>/<streamColList>
    m_p_stream->vAppend((m_p_streamColList = new Card_StreamColList(m_pStreamDef)));
  }

  vSetPrintSubCards(CK_STREAM, true);

  vPrintBoard();

  vSetPrintByKind(CK_STREAM, true);
  vSetPrintProperties(CK_STREAM, false);
  vSetPrintSubCards(CK_STREAM, false);

}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <predCmp> after <scan>/<absJoin>  */
/*----------------------------------------------------------------------------*/
void Transforms::vAssignAll_predCmp_To_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_filter = _pCard_Filter(xBI.pEachMain(CK_FILTER))))
    vAssignFilterLeftwards(m_p_filter);

  //vSetPrintSubCards(CK_CMPPRED, true);
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* calc ordinal ranges                                                        */
/*----------------------------------------------------------------------------*/
void Transforms::vCalc_ordinalRanges_OfAll_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  i8 iOrdLo = 1;
  i8 iOrdHi = C_UNKNOWN;
  i8 nStreamCols = C_UNKNOWN;

  BoardIter xBI(m_pBoard);

  while ((m_p_table = _pCard_Table(xBI.pEachMain(CK_TABLE)))) {

    m_p_stream = _pSub_Stream(m_p_table);

    // <scan>/<stream>:iOrdLo,iOrdHi
    nStreamCols = m_p_stream->nStreamCols();
    m_p_stream->vPpy_int(PPY_OrdLo,iOrdLo);
    iOrdHi = iOrdLo + nStreamCols - 1;
    m_p_stream->vPpy_int(PPY_OrdHi,iOrdHi);
    iOrdLo = iOrdHi + 1;

  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vCompute_coPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  vSetupPartitionBoard();

  //--nn--------------==
  // coPtnJoinList

  m_p_coPtnJoinList = new Card_CoPtnJoinList;
  _deck_->vAppendMain(m_p_coPtnJoinList);

  m_p_coPtnPredList =
      _pCard_CoPtnPredList(_deck_->pFindChild(CK_COPTNPREDLIST));


  i8 nChildren = m_p_coPtnPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    m_p_coPtnPred =
        _pCard_CoPtnPred(m_p_coPtnPredList->pGetChild(i_Child));

    m_p_subSet = m_p_coPtnPred->pDupSubCards();

    if (! (m_p_coPtnJoin = m_p_coPtnJoinList->pFind_coPtn_for(m_p_coPtnPred)) ) {
      m_p_coPtnJoin = new Card_CoPtnJoin;
      m_p_coPtnJoinList->vAppend(m_p_coPtnJoin);
    }

    m_p_coPtnJoin->vTransferChildren(m_p_subSet);
    DELETE_CARD(m_p_subSet);
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vCompute_coPtnPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  vSetupPartitionBoard();

  //--nn--------------==
  // coPptnPredList

  m_p_coPtnPredList = new Card_CoPtnPredList;
  _deck_->vAppendMain(m_p_coPtnPredList);

  m_p_tableList =
      _pCard_TableList(_deck_->pFindChild(CK_TABLELIST));
  m_p_joinPredList =
      _pCard_JoinPredList(_deck_->pFindChild(CK_JOINPREDLIST));

  i8 nJoinPreds = m_p_joinPredList->nChildCards();
  for (i8 i_joinPred = 0; i_joinPred < nJoinPreds; i_joinPred++) {
    m_p_predCmp =
        _pCard_CmpPred(m_p_joinPredList->pGetChild(i_joinPred));

    if (! m_p_tableList->bPredCmpIsCoPtn(m_p_predCmp))
      continue;

    m_p_coPtnPred = new Card_CoPtnPred;
    m_p_coPtnPred->vAppend(m_p_predCmp->pDupCardAndSubCards());

    m_p_coPtnPredList->vAppend(m_p_coPtnPred);
  }

  m_pCardCtl->vPrintName(CK_COLUMNREF,false);
  m_pCardCtl->vPrintProperties(CK_COLUMNREF,false);

  //m_pCardCtl->vPrintForm(CK_COPTNJOIN,CP_COMPRESSED);
  //m_pCardCtl->vPrintForm(CK_CROSSPTNJOIN,CP_COMPRESSED);
  //m_pCardCtl->vPrintForm(CK_CMPPRED,CP_COMPRESSED);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vCompute_crossPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  vSetupPartitionBoard();

  //--nn--------------==
  // crossPtnJoinList

  m_p_joinPredList =
      _pCard_JoinPredList(_deck_->pFindChild(CK_JOINPREDLIST));

  m_p_crossPtnJoinList = new Card_CrossPtnJoinList;
  _deck_->vAppendMain(m_p_crossPtnJoinList);

  i8 nChildren = m_p_joinPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    m_p_predCmp =
        _pCard_CmpPred(m_p_joinPredList->pGetChild(i_Child));

    if (m_p_coPtnJoinList->bHasEqPredCmp(m_p_predCmp))
      continue;

    m_p_crossPtnJoin = new Card_CrossPtnJoin;
    m_p_crossPtnJoin->vAppend(m_p_predCmp->pDupCardAndSubCards());
    m_p_crossPtnJoinList->vAppend(m_p_crossPtnJoin);
  }

//  m_p_comment = new Card_Comment("((TO BE CODED))"_Stz);
//  m_p_crossPtnJoinList->vAppend(m_p_comment);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vCompute_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  vSetupPartitionBoard();

  //--nn--------------==
  // ptndJoinSet

  m_p_ptndJoinSet = new Card_PtndJoinSet;
  _deck_->vAppendMain(m_p_ptndJoinSet);

  m_p_coPtnJoinList =
      _pCard_CoPtnJoinList(_deck_->pFindChild(CK_COPTNJOINLIST));
  m_p_subSet = m_p_coPtnJoinList->pDupSubCards();
  m_p_ptndJoinSet->vTransferChildren(m_p_subSet);
  DELETE_CARD(m_p_subSet);

  m_p_crossPtnJoinList =
      _pCard_CrossPtnJoinList(_deck_->pFindChild(CK_CROSSPTNJOINLIST));
  m_p_subSet = m_p_crossPtnJoinList->pDupSubCards();
  m_p_ptndJoinSet->vTransferChildren(m_p_subSet);
  DELETE_CARD(m_p_subSet);

  i8 nChildren = m_p_ptndJoinSet->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    m_pChildCard = m_p_ptndJoinSet->pGetChild(i_Child);
    m_pChildCard->vCard(CK_PTNDJOIN);
    m_pChildCard->vPpy_int(PPY_PtndJoin,i_Child);
  }

  if (m_bInteractive)
    m_pBoard_partition->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* convert all <filter> from calc form to calcBool                            */
/*----------------------------------------------------------------------------*/
void Transforms::vConvert_calc_OfAll_filter_To_calcBool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_filter = _pCard_Filter(xBI.pEachMain(CK_FILTER))))
    m_p_filter->vCalc_to_calcBool();

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* markup index columns                                                       */
/*----------------------------------------------------------------------------*/
void Transforms::vConvertAll_idxColNm_To_streamColOrd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  _deck_->vClearMarksRecursive();

  BoardIter xBI(m_pBoard);

  while ((m_p_table = _pCard_Table(xBI.pEachMain(CK_TABLE)))) {
    if (! (m_p_indexJoin = _pSub_IndexJoin(m_p_table)))
      continue;

    m_p_stream = _pSub_Stream(m_p_table);
    m_p_streamColList = _pSub_StreamColList(m_p_table);
    m_p_idxColList = _pSub_IdxColList(m_p_indexJoin);

    m_p_idxColList->vOrdinate(m_p_stream);
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vCopy_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  vSetupPermuteBoard();

  m_pBench->pCardCtl()->vNextHorizon(CK_PERMUTATION, true);

  m_p_ptndJoinSet =
      _pCard_PtndJoinSet(_deck_->pFindChild(CK_PTNDJOINSET));

  _deck_->vAppendMain(new Card_Comment("PERMUTATIONS"_Stz));

  i8 i_PtdnJoin = 0;
  m_p_subSet = m_p_ptndJoinSet->pDupSubCards();
  while ((m_pPoppedCard = m_p_subSet->pPop())) {
    _deck_->vAppendMain(m_pPoppedCard);
  }
  DELETE_CARD(m_p_subSet);

  if (m_bInteractive)
    m_pBoard_permute->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* inspect all <absJoin>/<filter> and cull predicates                         */
/*----------------------------------------------------------------------------*/
void Transforms::vCullAll_filterPred_Of_indexJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_table = _pCard_Table(xBI.pEachMain(CK_TABLE)))) {
    if (! (m_p_filter = _pSub_Filter(m_p_table)))
      continue;

    m_p_calcBool = m_p_filter->pSub_calcBool();

    m_p_calcBool->vCullPred('^');
  }

  //m_pCardCtl->vPrintSubCards(CK_STREAM, true);
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* cull all <scan>                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vCullAll_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__),true);
  vExplain("Blah"_Stz);

//  while (I_bEach_mainCard_(CK_SCAN)) {
//    _deck_->vReplaceChildWithItsSubs(m_pMainCard);
//    DELETE_CARD(m_pMainCard);
//  }

  vCullAll(CK_SCAN);

  vPrintBoard();
}
/*----------------------------------------------------------------------------*/
//void Transforms::vCullAll_scan(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
//
//  vTransformation(BK_QUERY,(char*)(__func__),true);
//  vExplain("Blah"_Stz);
//
//  Card_Deck* p_board = m_pBoard->pCard_Board();
//
//  BoardIter xBI(m_pBoard);
//  while ((m_pMainCard = xBI.pEachMain(CK_SCAN))) {
//    //m_p_table = _pSub_Table(m_pMainCard);
//    p_board->vReplaceChildWithItsSubs(m_pMainCard);
//
//    DELETE_CARD(m_pMainCard);
//  }
//
//  vPrintBoard();
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vCull_markedPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_pMainCard = xBI.pEachMain(CK_TABLE))) {

    m_p_filter = m_pMainCard->pSub_filter();
    if (!m_p_filter)
      continue;

    m_p_filter->vCullPred('L');
    m_p_filter->vCullPred('U');

    if (! m_p_filter->bIsEmptyFilter())
      continue;

    m_pMainCard->vRemoveSubCard(m_p_filter);

    delete m_p_filter;
    m_p_filter = nullptr;
  }

  //m_pCardCtl->vPrintForm(CK_STREAM, CP_CARDFORM);
  m_pCardCtl->vPrintByKind(CK_STREAM, true);
  m_pCardCtl->vPrintSubCards(CK_STREAM, false);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* cull <sort> when <sbyElist> is same as <gbyElist>                          */
/*----------------------------------------------------------------------------*/
void Transforms::vCull_sby_SameAs_gby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ( (m_p_sort = _pCard_Sort(xBI.pEachMain(CK_SORT))) ) {

    if (! (m_p_aggreg = _pCard_Aggreg(xBI.pPrevMainIf(CK_AGGREG))) )
      return;

    m_p_gbyElist = m_p_aggreg->pSub_gbyElist();
    m_p_sbyElist = m_p_sort->pSub_sbyElist();

    if (m_p_gbyElist->bValuesSameRecursive(m_p_sbyElist)) {
      xBI.vRemoveDeleteCurr(CK_SORT);
      break;
    }
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete sends                                                               */
/*----------------------------------------------------------------------------*/
void Transforms::vDeleteAll_absJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__),false);
  vExplain("Blah"_Stz);

  vDeleteAll(CK_ABSJOIN);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete combines                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vDeleteAll_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__),false);
  vExplain("Blah"_Stz);

  vDeleteAll(CK_COMBINED);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete sends                                                               */
/*----------------------------------------------------------------------------*/
void Transforms::vDeleteAll_send(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__),false);
  vExplain("Blah"_Stz);

  vDeleteAll(CK_SEND);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete any <filter> which is effectively empty                             */
/*----------------------------------------------------------------------------*/
void Transforms::vDeleteAll_Empty_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_filter = _pCard_Filter(xBI.pEachMain(CK_FILTER)))) {
    m_p_calcBool = m_p_filter->pSub_calcBool();

    if (0 == m_p_calcBool->nChildCards()) {
      xBI.vRemoveDeleteCurr(CK_FILTER);
      break;
    }
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete any <filter> which is effectively empty                             */
/*----------------------------------------------------------------------------*/
void Transforms::vDeleteAll_EmptySub_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while (Card *pCard_main = xBI.pEachMain()) {

    Card_Filter* pCard_filter_sub = _pSub_Filter(pCard_main);
    if (! pCard_filter_sub)
      continue;

    if (! pCard_filter_sub->bIsEmptyFilter())
      continue;

    pCard_main->vRemoveChild(pCard_filter_sub);
    delete pCard_filter_sub;
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vEstimate_joinOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  i8 iLowestNetwork = 999999999;

  BoardIter xBI(m_pBoard_costEst);

  while ((m_p_permutation =
              _pCard_Permutation(xBI.pEachMain(CK_PERMUTATION)))) {
    i8 iNetwork = m_p_permutation->iPpy(PPY_Network);
    if (iNetwork >= iLowestNetwork)
      continue;
    m_p_permutation_low = m_p_permutation;
    iLowestNetwork = iNetwork;
  }

  _deck_->vAppendMain((m_p_joinOrderSeq = new Card_JoinOrderSeq));
  m_p_joinOrderSeq->vPpy_int(PPY_Network,iLowestNetwork);

  xBI.vSet(m_p_permutation_low);
  while ((m_pMainCard = xBI.pEachMain())) {
    if (CK_PTNDJOIN != m_pMainCard->eCard())
      break;

    m_p_ptndJoin = _pCard_PtndJoin(m_pMainCard);
    m_p_joinOrderSeq->vExtractJoinOrder(m_p_ptndJoin);
  }

  if (m_bInteractive)
    m_pBoard_costEst->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vEstimate_network(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard_costEst);

  i8 nNetwork = 0;
  i8 nSentOut = 0;
  m_p_permutation = nullptr;
  m_p_ptndJoin    = nullptr;

  while ((m_pMainCard = xBI.pEachMain())) {
    eCK eCard = m_pMainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      if (m_p_permutation)
        m_p_permutation->vPpy_int(PPY_Network, nNetwork);
      m_p_permutation = _pCard_Permutation(m_pMainCard);

      m_p_ptndJoin = nullptr;
      nNetwork = 0;
      nSentOut = 0;
    }
    else
    if (CK_PTNDJOIN == eCard) {
      if (m_p_ptndJoin) {
        nSentOut += m_p_ptndJoin->iPpy(PPY_JoinedBuff);
        nNetwork += nSentOut;
        m_p_ptndJoin->vPpy_int(PPY_SentOut, nSentOut);
      }
      m_p_ptndJoin = _pCard_PtndJoin(m_pMainCard);
    }
  }

  if (m_p_permutation)
    m_p_permutation->vPpy_int(PPY_Network, nNetwork);

  if (m_bInteractive)
    m_pBoard_costEst->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vEstimate_ptndJoins(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  m_p_tableList =
      _pCard_TableList(_deck_->pFindChild(CK_TABLELIST));


  BoardIter xBI(m_pBoard_costEst);

//  while (m_p_ptndJoin = _pCard_PtndJoin(xBI.pEachMain(CK_PTNDJOIN))) {
  while ((m_pMainCard = xBI.pEachMain())) {
    eCK eCard = m_pMainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      m_p_tableList->vDeletePropertyRecursive(PPY_TableUsed);
      continue;
    }
    if (CK_PTNDJOIN != eCard)
      continue;
    m_p_ptndJoin = _pCard_PtndJoin(m_pMainCard);
    m_p_ptndJoin->vJoinEst(m_p_tableList);
  }

  if (m_bInteractive)
    m_pBoard_costEst->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vExtract_joinOrderSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  _deck_->vAppendMain((m_p_joinOrderSeq = new Card_JoinOrderSeq));

  BoardIter xBI(m_pBoard_costEst);

  xBI.vSet(m_p_permutation_low);
  while ((m_pMainCard = xBI.pEachMain())) {
    if (CK_PTNDJOIN != m_pMainCard->eCard())
      break;

    m_p_ptndJoin = _pCard_PtndJoin(m_pMainCard);
    m_p_joinOrderSeq->vExtractJoinOrder(m_p_ptndJoin);
  }

  if (m_bInteractive)
    m_pBoard_costEst->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vExtract_joinPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  vSetupPartitionBoard();

  //--nn--------------==
  // joinPredList

  m_p_joinPredList = new Card_JoinPredList;
  _deck_->vAppendMain(m_p_joinPredList);

  BoardIter xBI_query(m_pBoard_query);
  while ((m_pMainCard = xBI_query.pEachMain(CK_FILTER))) {
    m_p_calcBool = m_pMainCard->pSub_calcBool();
    if (!m_p_calcBool)
      continue;

    m_p_joinPredList->vExtract_joinPredList(m_p_calcBool);
  }


  if (m_bInteractive)
    m_pBoard_partition->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vExtract_tableList_From_queryBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);


  vSetupPartitionBoard();

  //--nn--------------==
  // tableList

  BoardIter xBI_query(m_pBoard_query);

  m_p_tableList = new Card_TableList;
  _deck_->vAppendMain(m_p_tableList);

  while ( (m_pMainCard = xBI_query.pEachMain()) ) {
    if (! (m_p_table = m_pMainCard->pSub_table()))
      continue;

    m_p_tableList->vAppend(m_p_table->pDupCardAndSubCards());
  }


  m_pBoard_partition->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* form <joinPred>                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vFormAll_joinPred_of_indexJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_table = _pCard_Table(xBI.pEachMain(CK_TABLE)))) {
    if (! (m_p_filter = _pSub_Filter(m_p_table)))
      continue;

    m_p_expr     = _pCard_Expr(m_p_filter->pGetChild(0));
    m_p_calcBool = m_p_expr->pSub_calcBool();
    m_p_predCmp  = m_p_calcBool->pSub_predCmp();
    m_p_term_0   = _pCard_Term(m_p_predCmp->pGetChild(0));
    m_p_term_1   = _pCard_Term(m_p_predCmp->pGetChild(1));

    // - - - -

    m_p_joinPred = new Card_JoinPred;
    m_p_indexJoin = _pSub_IndexJoin(m_p_table);
    m_p_table->vInsertAfter(m_p_joinPred,m_p_indexJoin);

    m_p_term = new Card_Term("BOUND"_Stz);
    m_p_joinPred->vAppend(m_p_term);

    // - - - -

    // one of {pCard_term_0,pCard_term_1} is $In

    m_p_stream = _pSub_Stream(m_p_table);
    i8 iStreamOrdLo = m_p_stream->iPpy(PPY_OrdLo);
    i8 iStreamOrdHi = m_p_stream->iPpy(PPY_OrdHi);

    i8 i_iOrdLo_0 = m_p_term_0->iStreamColNo();
    i8 i_iOrdLo_1 = m_p_term_1->iStreamColNo();

    if (i_iOrdLo_0 >= iStreamOrdLo && i_iOrdLo_0 <= iStreamOrdHi) {
      m_p_term = new Card_Term("$I1"_Stz);
      m_p_joinPred->vAppend(m_p_term);
      m_p_term = new Card_Term(m_p_term_1->sz());
      m_p_joinPred->vAppend(m_p_term);
    }
    else
    if (i_iOrdLo_1 >= iStreamOrdLo && i_iOrdLo_1 <= iStreamOrdHi) {
      m_p_term = new Card_Term("$I1"_Stz);
      m_p_joinPred->vAppend(m_p_term);
      m_p_term = new Card_Term(m_p_term_1->sz());
      m_p_joinPred->vAppend(m_p_term);
    }
    else {
      NEVER_REACH
    }

    m_p_predCmp->vMark1('^');
    m_p_term = new Card_Term(m_p_predCmp->sz());
    m_p_joinPred->vAppend(m_p_term);
  }

  vSetPrintSubCards(CK_FILTER, true);
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vGenerate_ptndPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

//  PermGen::vTest(4);

  //--nn--------------==
  // x


  m_pPermGen = new PermGen(m_p_ptndJoinSet->nChildCards());

  m_p_subSet = new Card_SubSet;

  while (m_pPermGen->bNextPermutation()) {
    _deck_->vAppendMain((m_p_permutation = new Card_Permutation));
    m_p_permutation->vPpy_int(PPY_Permutation,m_pPermGen->nPermutations()-1);

    RASSERT(0 == m_p_subSet->nChildCards());
    for (i8 i_Item = 0; i_Item < m_pPermGen->nItems(); i_Item++) {
      i8 i_Child = m_pPermGen->iItem(i_Item);
      m_pChildCard = m_p_ptndJoinSet->pGetChild(i_Child);
      _deck_->vAppendMain(m_pChildCard->pDupCardAndSubCards());

      m_p_ptndJoin = _pCard_PtndJoin(m_pChildCard);
      if (! m_p_ptndJoin->bMayFollowAfter(m_p_subSet)) {
        _deck_->vAppendMain(new Card_Comment("**DOESN'T FOLLOW**"_Stz));
        m_p_permutation->vPpy_nameOnly(PPY_Invalid);
        break;
      }
      m_p_subSet->vAppend(m_pChildCard);
    }
    m_p_subSet->vRemoveAll();
  }

  DELETE_CARD(m_p_subSet);

  if (m_bInteractive)
    m_pBoard_permute->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* individuate <absJoin>/<filter> as main <filter>                            */
/*----------------------------------------------------------------------------*/
void Transforms::vIndividuate_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while (Card_Scan *pCard_scan =
      _pCard_Scan(xBI.pEachMain(CK_SCAN))) {

    Card_Filter* pCard_filter =
        _pSub_Filter(pCard_scan);

    if (! pCard_filter)
      continue;

    pCard_scan->vRemoveChild(pCard_filter);
    _deck_->vInsertAfter(pCard_filter, pCard_scan);
  }

  xBI.vRestart();

  while (Card_IndexJoin *pCard_indexJoin =
      _pCard_IndexJoin(xBI.pEachMain(CK_INDEXJOIN))) {

    Card_Filter* pCard_filter =
        _pSub_Filter(pCard_indexJoin);

    if (! pCard_filter)
      continue;

    pCard_indexJoin->vRemoveChild(pCard_filter);
    _deck_->vInsertAfter(pCard_filter, pCard_indexJoin);
  }
}

/*----------------------------------------------------------------------------*/
/* append preliminary <indexJoin>'s to all <absJoin>                          */
/*----------------------------------------------------------------------------*/
void Transforms::vInsert_indexJoin_IntoAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  _deck_->vClearMarksRecursive();

  BoardIter xBI(m_pBoard);

  while ((m_p_table = _pCard_Table(xBI.pEachMain(CK_TABLE)))) {

    if (! (m_p_filter = m_p_table->pSub_filter()))
      continue;

    m_p_tablePtr = _pSub_TablePtr(m_p_table);
    m_pTable = m_p_tablePtr->pTable();

    if (! (m_pIdxVec = m_pTable->pIdxVec()))
      return;

    i8 nIndexes = m_pIdxVec->nItems();
    for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
      m_pIndex = m_pIdxVec->pGet(i_Index);

      m_p_indexJoin = new Card_IndexJoin;
      m_p_table->vInsertBefore(m_p_indexJoin,m_p_filter);

      m_p_indexPtr = new Card_IndexPtr(m_pIndex);
      m_p_indexJoin->vAppend(m_p_indexPtr);

      Card_IdxColList* pCard_idxColList = new Card_IdxColList(m_pIndex);
      m_p_indexJoin->vAppend(pCard_idxColList);
    }
  }

  //m_pCardCtl->vPrintFormSubCards(CK_STREAM, CP_DEFAULT, true);
  vSetPrintSubCards(CK_FILTER, false);
  vSetPrintSubCards(CK_STREAM, false);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* linearize stages                                                           */
/*----------------------------------------------------------------------------*/
void Transforms::vLinearizeAll_stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__),false);
  vExplain("Linearize all the stages"_Stz);

  BoardIter xBI(m_pBoard);

  if (! (m_p_stage_a = _pCard_Stage(xBI.pEachMain(CK_STAGE))))
    goto L_Exit;
  if (! (m_p_stageTopo_a = m_p_stage_a->pSub_stageTopo()) )
    goto L_Exit;
  if (YG_ALL_PARTITIONS != m_p_stageTopo_a->eStageTopo())
    goto L_Exit;

  while ((m_p_stage_b = _pCard_Stage(xBI.pEachMain(CK_STAGE)))) {
    if (! (m_p_stageTopo_b = m_p_stage_b->pSub_stageTopo()) )
      break;
    if (YG_ALL_PARTITIONS != m_p_stageTopo_b->eStageTopo())
      break;
    xBI.vRemoveDeleteCurr(CK_STAGE);
  }

  L_Exit:

  xBI.vRestart();
  while ((m_p_stage = _pCard_Stage(xBI.pEachMain(CK_STAGE)))) {
    if (! (m_p_stageTopo = m_p_stage->pSub_stageTopo()) )
      continue;
    if (YG_ALL_PARTITIONS == m_p_stageTopo->eStageTopo())
      continue;

    m_p_stage->vPpy_nameOnly(PPY_Exclude);
  }


  _deck_->vRenumberRecursive();

  vSetPrintSubCards(CK_AGGELIST, false);
  vSetPrintSubCards(CK_GBYELIST, false);
  vSetPrintSubCards(CK_PROJELIST, false);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vMark_scan_pred_bounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  auto bfPredBounds = [](Card* pCard) {
    Card_CmpPred* p_predCmp = _pCard_CmpPred(pCard);
    p_predCmp->vMarkBound();
    return true;
  };

  // - - - - - - - - -

  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain(CK_TABLE)) ) {
    if (! (m_p_filter = m_pMainCard->pSub_filter()) )
      continue;
    m_p_filter->vVisitRecursive(CK_CMPPRED, bfPredBounds);
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vMark_scan_pred_ordinals(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain(CK_TABLE)) ) {
    if (! (m_p_filter = m_pMainCard->pSub_filter()) )
      continue;

    Card_Stream* p_stream = m_pMainCard->pSub_stream();
    if (! p_stream)
      continue;

    Card_IndexList* p_indexList = m_pMainCard->pSub_indexList();
    if (! p_indexList)
      continue;

    Card_IndexPtr* p_indexPtr = _pCard_IndexPtr(p_indexList->pGetChild(0));
    if (! p_indexPtr)
      continue;

    // - - - - - - - - -
    auto bfPredBounds = [p_stream,p_indexPtr](Card* pCard) {
      Card_Term* p_term = _pCard_Term(pCard);

      Stz* szTerm = p_term->sz();
      if (!szTerm->bIsStreamColOrd())
        return true;

      vTrace("vMark_scan_pred_ordinals:: %s\n"_Stz,p_term->sz());

      Card_StreamCol* p_streamCol = p_stream->pGet_streamCol(szTerm);
      if (! p_streamCol)
        return true;

      Stz* szColNm = p_streamCol->szStreamColNm();
      vTrace("vMark_scan_pred_ordinals:: %s\n"_Stz,szColNm);

      i8 i_IdxCol = C_UNKNOWN;
      Card_IdxColNm* p_idxCol = p_indexPtr->pFind_indexCol(szColNm, i_IdxCol);

      if (! p_idxCol)
        return true;

      vTrace("vMark_scan_pred_ordinals:: %lld\n"_Stz,i_IdxCol);

      p_term->vMark1('I');
      p_term->vMark2(i_IdxCol);
      return true;
    };
    // - - - - - - - - -

    m_p_filter->vVisitRecursive(CK_TERM, bfPredBounds);
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and copy <predCmp> to new <scan|absJoin>/<filter>     */
/*----------------------------------------------------------------------------*/
void Transforms::vMoveAll_predCmp_To_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_filter = _pCard_Filter(xBI.pEachMain(CK_FILTER))))
    vCopyFilterLeftwards(m_p_filter);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vPromote_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);

  while ((m_p_scan = _pCard_Scan(xBI.pEachMain(CK_SCAN)))) {

    m_p_scanPred = m_p_scan->pSub_scanPred();
    if (!m_p_scanPred)
      continue;

    m_p_scan->vCard(CK_INDEXSCAN);
  }

  if (m_bInteractive)
    m_pBoard->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* reduce absJoin                                                             */
/*----------------------------------------------------------------------------*/
void Transforms::vRefactorAll_table_Into_join(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  Card_Deck* p_board = m_pBoard->pCard_Board();

  BoardIter xBI(m_pBoard);

  while ((m_p_table = _pCard_Table(xBI.pEachMain(CK_TABLE)))) {

    m_p_indexJoin = _pSub_IndexJoin(m_p_table);
    if (! m_p_indexJoin) {

      m_p_scan = new Card_Scan;
      m_p_scan->vTransferChildren(m_p_table);

      p_board->vReplace(m_p_table,m_p_scan);
      m_p_scan->vInsert(m_p_table);
      continue;
    }

    m_p_table->vRemoveSubCard(m_p_indexJoin);
    m_p_indexJoin->vTransferChildren(m_p_table);

    p_board->vReplace(m_p_table,m_p_indexJoin);
    m_p_indexJoin->vInsert(m_p_table);
  }

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vRemove_invalidPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  i8 i_Main = 0;
  i8 i_Permutation = 0;
  while (i_Main < _deck_->nChildCards()) {
    m_pMainCard = _deck_->pGetChild(i_Main);

    if (CK_PERMUTATION != m_pMainCard->eCard()) {
      i_Main++;
      continue;
    }

    if (! m_pMainCard->bHasProperty(PPY_Invalid)) {
      m_pMainCard->vPpy_int(PPY_Permutation,i_Permutation++);
      i_Main++;
      continue;
    }

    _deck_->vRemoveChild(m_pMainCard);
    while (i_Main < _deck_->nChildCards()) {
      m_pMainCard = _deck_->pGetChild(i_Main);
      if (CK_PTNDJOIN != m_pMainCard->eCard() && CK_COMMENT != m_pMainCard->eCard())
        break;
      _deck_->vRemoveChild(m_pMainCard);
      delete m_pMainCard;
    }
  }

  if (m_bInteractive)
    m_pBoard_permute->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vSetup_costEst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  vSetupCostEstBoard();

  m_pBoard_costEst->vDuplicateBoard(m_pBoard_permute);

  while ((m_pMainCard = _deck_->pGetChild(0))) {

    if (CK_PERMUTATION == m_pMainCard->eCard())
      break;

    _deck_->vRemoveChild(m_pMainCard);
    delete m_pMainCard;
  }


  if (m_bInteractive)
    m_pBoard_costEst->vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* each <scan> <absJoin> becomes <absJoin>/<scan>                             */
/*----------------------------------------------------------------------------*/
void Transforms::vSubsume_scan_Under_absJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransformation(BK_QUERY,(char*)(__func__));
  vExplain("Blah"_Stz);

  BoardIter xBI(m_pBoard);
  while ((m_p_absJoin = _pCard_AbsJoin(xBI.pEachMain(CK_ABSJOIN))))
    m_p_absJoin->vDeleteChild(CK_CALC);

  xBI.vRestart();

  // E <scan>
  while ((m_p_scan = _pCard_Scan(xBI.pEachMain(CK_SCAN)))) {

    // ? <scan <absJoin>
    if (! (m_p_absJoin = _pCard_AbsJoin(xBI.pNextMainIf(CK_ABSJOIN))))
      continue;

    // <absJoin>/<scan>
    xBI.vRemoveCard(m_p_scan);
    m_p_absJoin->vAppend(m_p_scan);
  }

  vPrintBoard();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Transforms::vAssignFilterLeftwards(Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // get <filter>.<calcBool>
  //Card_CalcBool* pCard_bool =
  //    _pCard_CalcBool(pCard_filter->pFindSubCard(CK_CALCBOOL));

  Card_CalcBool* pCard_bool = pCard_filter->pSub_calcBool();

  Card_PRED* pCard_pred = static_cast<Card_PRED*>(pCard_bool->pGetChild(0));

  vAssignPredDrillDown(pCard_pred,pCard_filter);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Transforms::vAssignPredDrillDown(Card_PRED* pCard_pred, Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char cWhichTable = ' ';

  eCK eCard = pCard_pred->eCard();


  if (CK_ORPRED == eCard) {
    pCard_pred->vMark1('|');
    return;
  }

  if (CK_CMPPRED == eCard) {
    pCard_pred->vMark1('?');
    cWhichTable =
        cAssignPredLeft(pCard_pred,
                        pCard_filter);
    if (C_UNKNOWN != cWhichTable) {
      pCard_pred->vMark1('@');
      pCard_pred->vMark2(cWhichTable);
    }
    return;
  }

  i8 nSubCards = pCard_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_pred->pGetChild(i_Child);

    vAssignPredDrillDown(static_cast<Card_PRED *>(pSubCard), pCard_filter);
  }


  if (CK_ANDPRED == eCard) {
    pCard_pred->vMark1('&');
    Card *pSubCard_0 = pCard_pred->pGetChild(0);
    i8 iStream_0 = pSubCard_0->cMark2() - '0';

    Card *pSubCard_1 = pCard_pred->pGetChild(1);
    i8 iStream_1 = pSubCard_1->cMark2() - '0';

    if (iStream_0 == iStream_1) {
      pCard_pred->vMark1('@');
      pCard_pred->vMark2(iStream_0);
    }

    return;
  }
}

/*----------------------------------------------------------------------------*/
/* inspect <PRED>                                                             */
/*----------------------------------------------------------------------------*/
char Transforms::cAssignPredLeft(Card_PRED* pCard_PRED, Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nAbsJoin = 0;
  i8 nTable    = 0;
  char cWhichTable = C_UNKNOWN;

  //vTrace("iAssignPredLeft candidate %p\n"_Stz,pCard_PRED);
  //pCard_PRED->vDump();

  for (i8 i_Main = 0; /* none */ ; i_Main++) {
    Card *pMainCard = _deck_->pGetChild(i_Main);

    if (pMainCard == pCard_filter)
      return C_UNKNOWN;

    eCK eMainCard = pMainCard->eCard();

    if (CK_TABLE == eMainCard) {
      nTable++;
      cWhichTable = cAssignPredToTable(_pCard_Table(pMainCard), pCard_PRED);
      if (C_UNKNOWN != cWhichTable)
        return cWhichTable;
      continue;
    }

    // skip over any other card??
    vTrace("bMovePredLeft skipping mainCard %s\n"_Stz,aCK[eMainCard]);
  }

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Transforms::cAssignPredToTable(Card_Table* pCard_table, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_Stream* pCard_stream = _pSub_Stream(pCard_table);

  // are all $n of <pred> in this <stream>?
  if (pCard_stream->bContainsPred(pCard_PRED)) {
    char cTableSyn = pCard_table->cPpy(PPY_TableSyn);
    return cTableSyn;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Transforms::vCopyFilterLeftwards(Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_CalcBool* pCard_bool = pCard_filter->pSub_calcBool();


  while (pCard_bool->nChildCards()) {
    Card_PRED* pCard_pred = static_cast<Card_PRED*>(pCard_bool->pGetChild(0));
    bool bSubRemoved = bCopyPredDrillDown(pCard_pred,pCard_filter);
    if (! bSubRemoved)
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
bool Transforms::bCopyPredDrillDown(Card_PRED* pCard_pred, Card_Filter* pCard_filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_CalcBool* pCard_calcBool = pCard_filter->pSub_calcBool();

  if ('@' == pCard_pred->cMark1()) {
    Card* pMainCard;
//    i8 iStream = pCard_pred->iMark2();
//    Card_Stream* pCard_stream = m_pBoard->pFindStreamCard(iStream,pMainCard);
    char cTableSyn = pCard_pred->cMark2();
    Card_Table* pCard_table = _deck_->pFindTableCard(cTableSyn, pMainCard);

    Card_Filter* pCard_filter_existing =
        _pSub_Filter(pMainCard);

    if (pCard_filter_existing) {
      pCard_filter_existing->vMark1(pCard_table->cMark1());
      pCard_filter_existing->vMark2(pCard_table->cMark2());
      vCopyPredToExistingFilter(pCard_filter_existing,pCard_pred);
    }
    else {
      Card_Filter* pCard_filter_new = new Card_Filter;
      pCard_filter_new->vMark1(pCard_table->cMark1());
      pCard_filter_new->vMark2(pCard_table->cMark2());
      pMainCard->vAppend(pCard_filter_new);
      vCopyPredToNewFilter(pCard_filter_new,pCard_pred);
    }

    //pCard_stream->vMark1('#');


    pCard_calcBool->vRemovePred(pCard_pred);
    //m_pBoard->vPrintBoard();

    return true;
  }

  i8 nSubCards = pCard_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_pred->pGetChild(i_Child);

    bool bRemoved = bCopyPredDrillDown(static_cast<Card_PRED *>(pSubCard), pCard_filter);
    if (bRemoved)
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Transforms::vCopyPredToExistingFilter(Card_Filter* pCard_filter_existing, Card_PRED* pCard_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_CalcBool* pCard_calcBool = pCard_filter_existing->pSub_calcBool();

  Card* pCard_top = pCard_calcBool->pPop();

  Card_AndPred* pCard_predAnd = new Card_AndPred;
  pCard_predAnd->vAppend(pCard_top);
  pCard_predAnd->vAppend(pCard_pred);

  pCard_calcBool->vAppend(pCard_predAnd);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Transforms::vCopyPredToNewFilter(Card_Filter* pCard_filter_new, Card_PRED* pCard_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_Expr* pCard_expr = new Card_Expr;
  pCard_filter_new->vAppend(pCard_expr);

  Card_Type* pCard_type = new Card_Type("BOOLEAN"_Stz);
  pCard_expr->vAppend(pCard_type);
  Card_CalcBool* pCard_calcBool = new Card_CalcBool;
  pCard_expr->vAppend(pCard_calcBool);

  pCard_calcBool->vAppend(pCard_pred);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vMarkPredCardsAsMoved(Card* pPredCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nSubCards = pPredCard->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pPredCard->pGetChild(i_Child);

    //eCK eSubCard = pSubCard->eCard();

    // mark the

    vMarkPredCardsAsMoved(pSubCard);
  }
}

/*----------------------------------------------------------------------------*/
/* convert a calculation into a boolean calculaiton                           */
/*----------------------------------------------------------------------------*/
Card_IndexPred* Transforms::pTo_IndexPred(Card_AbsJoin* pCard_AbsJoin,
                                      Card_CmpPred* pCard_PredCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_IndexPred *pCard_IndexPred = new Card_IndexPred;

//  pCard_IndexPred->vAppend(new Card_Term("BOUND"_Stz));
//  pCard_IndexPred->vAppend(new Card_Term("$I1"_Stz));
//  pCard_IndexPred->vAppend(new Card_Term("$L1"_Stz));
//  pCard_IndexPred->vAppend(new Card_Term("=="_Stz));

  Resolver *pResolver = new Resolver(m_pBench);

  pCard_IndexPred->vAppend(new Card_Term("BOUND"_Stz));

  Card_Term *pCard_Term_0 = _pCard_Term(pCard_PredCmp->pGetChild(0));
  Stz *szTerm_0 = pCard_Term_0->sz();
  Card_Term *pCard_Term_1 = _pCard_Term(pCard_PredCmp->pGetChild(1));
  Stz *szTerm_1 = pCard_Term_1->sz();

  s256 sStreamNm_0;
  s256 sStreamColNm_0;
  i8   i_StreamColNo_0;

  pResolver->vResolve(pCard_AbsJoin, szTerm_0,
                      sStreamNm_0, sStreamColNm_0,i_StreamColNo_0);

  s256 sStreamNm_1;
  s256 sStreamColNm_1;
  i8   i_StreamColNo_1;

  pResolver->vResolve(pCard_AbsJoin, szTerm_1,
                      sStreamNm_1, sStreamColNm_1, i_StreamColNo_1);

  s256 sPred_0;
  s256 sPred_1;

  if (sStreamNm_0.bSame("L"_Stz)) {
    sPred_0.vSprint("$I%lld"_Stz,i_StreamColNo_1+1);
    sPred_1.vSprint("$%lld"_Stz,i_StreamColNo_0+1);
  }
  else {
    sPred_0.vSprint("$I%lld"_Stz,i_StreamColNo_0+1);
    sPred_1.vSprint("$%lld"_Stz,i_StreamColNo_1+1);
  }

  pCard_IndexPred->vAppend(new Card_Term(sPred_0));
  pCard_IndexPred->vAppend(new Card_Term(sPred_1));

  pCard_IndexPred->vAppend(new Card_Term(pCard_PredCmp->sz()));

  delete pResolver;

  return pCard_IndexPred;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
