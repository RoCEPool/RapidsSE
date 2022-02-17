/*============================================================================*/
/* Tools.cpp -- Drafting Tools                                                */
/*============================================================================*/

#include "Tools.hpp"
#include "Bench.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Tools,CN_TOOLS)
__aNewFunction(Tools,CN_TOOLS)
__vDeleteFunction(Tools,CN_TOOLS)
__aFooterFunction(Tools)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tools::Tools(void)
: Object(CN_TOOLS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
    
  _checkInitialised_(CN_TOOLS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tools::~Tools(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find scan followed by a filter                                             */
/*----------------------------------------------------------------------------*/
Card_Scan* Tools::pFind_Scan_Filter(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  i8 nCards = pBoard->nItems();
  
  for (i8 i_TestScan = 0; i_TestScan < nCards-1; i_TestScan++) {
    
    Card* pTestScan = pBoard->pGet(i_TestScan);
    if (CK_SCAN != pTestScan->eCard())
      continue;

    Card* pTestFilter = pBoard->pGet(i_TestScan+1);
    if (CK_FILTER != pTestFilter->eCard())
      continue;
    
    return _pCard_Scan(pTestScan);
  }
  
  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* form the stream definitions for all generative work units                  */
/*----------------------------------------------------------------------------*/
void Tools::vGenerativeStreamDefs(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  /*

    A generative work unit can be one of the following:
   
      Scan .1          //  IndexScan .4     //  IndexJoin .6
      0)Table          //  0)Table          //  0)Table
             part      //           orders  //         lineitem
      ...              //  ...              //  ...
      0123456789ABCDE  //  0123456789ABCDE  //  0123456789ABCDE
  
  */
  
  StreamDef* pStreamDef_prior = nullptr;
  
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* examine given generative work unit and form the stream defn it emits   */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vFormStreamDef = [&pStreamDef_prior](Card* pGenerativeUnit) mutable
  {
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // a generative work unit can either be a SCAN
  
    if (_bIsCard_Scan(pGenerativeUnit)) {
      Card_Scan* pCard_Scan = _pCard_Scan(pGenerativeUnit);

//      Card_Table* pCard_Table
//        = _pCard_Table(pCard_Scan->pFindSubCard(CK_TABLE));
//      Table* pTable = _pTable(pCard_Table->pTable());
      
      Table* pTable = pCard_Scan->pTable();

      
      StreamDef* pStreamDef = new StreamDef(pTable->pStreamDef());
      Card_StreamDef* pCard_StreamDef = new Card_StreamDef(pStreamDef);
      
      pCard_Scan->vAppend(pCard_StreamDef);
      
      pStreamDef_prior = pStreamDef;
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an INDEXSCAN
  
    if (_bIsCard_IndexScan(pGenerativeUnit)) {
      Card_IndexScan* pCard_IndexScan = _pCard_IndexScan(pGenerativeUnit);

      Card_Table* pCard_Table
        = _pCard_Table(pCard_IndexScan->pFindSubCard(CK_TABLE));
      Table* pTable = _pTable(pCard_Table->pTable());
      
      StreamDef* pStreamDef = new StreamDef(pTable->pStreamDef());
      Card_StreamDef* pCard_StreamDef = new Card_StreamDef(pStreamDef);
      
      pCard_IndexScan->vAppend(pCard_StreamDef);
      
      pStreamDef_prior = pStreamDef;
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an INDEXJOIN

    if (_bIsCard_IndexJoin(pGenerativeUnit)) {
      Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pGenerativeUnit);
      Table* pTable = pCard_IndexJoin->pIndex()->pTable();

      StreamDef* pStreamDef = new StreamDef(pStreamDef_prior);
      Card_StreamDef* pCard_StreamDef = new Card_StreamDef(pStreamDef);
      
      pStreamDef->vAppendStreamDef(pTable->pStreamDef());
      pCard_IndexJoin->vAppend(pCard_StreamDef);

      pStreamDef_prior = pStreamDef;
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // otherwise ignored
    return;
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  //
  // walk all units, forming the stream definitions for the generative ones
  //
  
  pBench->vWalkUnits(vFormStreamDef);
}

/*----------------------------------------------------------------------------*/
/* pick best candidate index                                                  */
/*----------------------------------------------------------------------------*/
void Tools::vPickIndex(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  i8 i_Card;
  Card* pCard;
  Card_CandidateIndex* pCard_CandidateIndex_best = nullptr;

  i8 nCards = pBoard->nItems();
  for (i_Card = 0; i_Card < nCards; i_Card++) {
    pCard = pBoard->pGet(i_Card);
    if (_bIsCard_CandidateIndex(pCard)) {
      pCard_CandidateIndex_best = _pCard_CandidateIndex(pCard);
      break;
    }
  }
  
  RASSERT(pCard_CandidateIndex_best);
  
  Card_CandidateIndex* pCard_CandidateIndex_test = nullptr;

  for (/*none*/; i_Card < nCards; i_Card++) {
    pCard = pBoard->pGet(i_Card);
    if (! _bIsCard_CandidateIndex(pCard))
      break;
    
    pCard_CandidateIndex_test = _pCard_CandidateIndex(pCard);

    if (pCard_CandidateIndex_test->iRank() > pCard_CandidateIndex_best->iRank())
      pCard_CandidateIndex_best = pCard_CandidateIndex_test;
  }
  
  vCullIndex(pBoard,pCard_CandidateIndex_best);
}

/*----------------------------------------------------------------------------*/
/* pick best candidate index                                                  */
/*----------------------------------------------------------------------------*/
void Tools::vCullIndex(Board* pBoard, Card_CandidateIndex* pExcept)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  i8 i_Card;
  Card* pCard;
  
  for (i_Card = 0; i_Card < pBoard->nItems(); i_Card++) {
    pCard = pBoard->pGet(i_Card);
    
    if (! _bIsCard_CandidateIndex(pCard))
      continue;
    
    if (pCard == pExcept)
      continue;
    
    pBoard->vRemove(pCard);
    i_Card--;
  }
}

/*----------------------------------------------------------------------------*/
/* form the stream definitions for all projective work units                  */
/*----------------------------------------------------------------------------*/
void Tools::vProjectiveStreamDefs(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  /*

    A projective work unit can be one of the following:
   
      AggregList .1    //  OrdAggList .4    //  ProjectList .6
      ...              //  ...              //  ...
      0123456789ABCDE  //  0123456789ABCDE  //  0123456789ABCDE
  
  */

  StreamDef* pStreamDef_prior = nullptr;
  
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* examine given projective work unit and form the stream def it emits    */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vFormStreamDef = [&pStreamDef_prior](Card* pProjectiveUnit) mutable
  {
    //vTrace("vExpandStreamDefs: %s\n"_Stz,pCard->szCardNm());
  
    // get running streamDef from SCAN, INDEXSCAN or INDEXJOIN
    if (     _bIsCard_Scan(pProjectiveUnit) ||
        _bIsCard_IndexScan(pProjectiveUnit) ||
        _bIsCard_IndexJoin(pProjectiveUnit)) {
      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pProjectiveUnit->pFindSubCard(CK_STREAMDEF));

      pStreamDef_prior = pCard_StreamDef->pStreamDef();
      return;
    }
  
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // a projective work unit will be either a AGGREGLIST

    if (_bIsCard_Aggreg(pProjectiveUnit)) {
      vTrace("vReproject : %s\n"_Stz,pProjectiveUnit->szCardNm());
      Card_Aggreg* pCard_Aggreg = _pCard_Aggreg(pProjectiveUnit);
      
      pStreamDef_prior = pProduceStreamDef(pCard_Aggreg);
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an ORDAGGLIST

    if (_bIsCard_OrdAgg(pProjectiveUnit)) {
      vTrace("vReproject : %s\n"_Stz,pProjectiveUnit->szCardNm());
      Card_OrdAgg* pCard_OrdAgg = _pCard_OrdAgg(pProjectiveUnit);
      
      pStreamDef_prior = pProduceStreamDef(pCard_OrdAgg);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an PROJECTLIST

    if (_bIsCard_Project(pProjectiveUnit)) {
      Card_Project* pCard_Project = _pCard_Project(pProjectiveUnit);
      
      pStreamDef_prior = pProduceStreamDef(pCard_Project);
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // otherwise ignored
    return;
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  //
  // walk all units, forming the stream definitions for the projective ones
  //

  pBench->vWalkUnits(vFormStreamDef);
}


/*----------------------------------------------------------------------------*/
/* rank index given compare predicate                                         */
/*----------------------------------------------------------------------------*/
i8 Tools::iRankIndex(Index* pIndex, Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  IndexDef* pIndexDef = pIndex->pIndexDef();

  i8 nColumns = pIndex->nColumns();
  Column* pColumn = pIndex->pGetColumn(0);
  Card_Term* pCard_Term = _pCard_Term(pCard_ComparePred->pGet(0));
  
  if (pColumn->szColNm()->bSame(pCard_Term->szTerm()))
    return 1;
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* refine stream def's with order                                             */
/*----------------------------------------------------------------------------*/
void Tools::vStreamOrder(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  StreamDef* pStreamDef_prior = nullptr;

  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* examine given work unit and determine its streamDef order              */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vSetOrder = [&pStreamDef_prior](Card* pUnit) mutable
  {
    vTrace("vSetOrder: %s %s\n"_Stz,pUnit->szCardNm()
                                      ,aCN[pUnit->eGetClassNo()]);
  
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // an INDEXSCAN establishes an ordered stream
  
    if (_bIsCard_IndexScan(pUnit)) {
      Card_IndexScan* pCard_IndexScan = _pCard_IndexScan(pUnit);
      vTrace("vSetOrder : %s\n"_Stz,pCard_IndexScan->szCardNm());

      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pCard_IndexScan->pFindSubCard(CK_STREAMDEF));
      StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();
      
      Card_Index* pCard_Index
        = _pCard_Index(pCard_IndexScan->pFindSubCard(CK_INDEX));
      Index* pIndex = pCard_Index->pIndex();
      IndexDef* pIndexDef = pIndex->pIndexDef();

      i8 nColumns = pIndexDef->nColumns();
      for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
        Column* pIndexColumn = pIndex->pGetColumn(i_Col);
        Stz* szIndexColumnNm = pIndexColumn->szColNm();

        Column* pStreamColumn = pStreamDef->pFindColumn(szIndexColumnNm);
        i8 iColNo = pStreamColumn->iColumnNo();
        pStreamDef->vAddOrder(iColNo);
      }
      
      pStreamDef_prior = pStreamDef;
      return;
    }
  
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // an INDEXJOIN maintains the ordered stream
  
    if (_bIsCard_IndexJoin(pUnit)) {
      Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pUnit);
      vTrace("vSetOrder : %s\n"_Stz,pCard_IndexJoin->szCardNm());
      
      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pCard_IndexJoin->pFindSubCard(CK_STREAMDEF));
      StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();
      
      Card_Index* pCard_Index
        = _pCard_Index(pCard_IndexJoin->pFindSubCard(CK_INDEX));
      Index* pIndex = pCard_Index->pIndex();
      IndexDef* pIndexDef = pIndex->pIndexDef();

      i8 nColumns = pIndexDef->nColumns();
      for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
        Column* pIndexColumn = pIndex->pGetColumn(i_Col);
        Stz* szIndexColumnNm = pIndexColumn->szColNm();

        Column* pStreamColumn = pStreamDef->pFindColumn(szIndexColumnNm);
        i8 iColNo = pStreamColumn->iColumnNo();
        pStreamDef->vAddOrder(iColNo);
      }
      
//      // apply prior
//      i8 nOrders = pStreamDef_prior->nOrders();
//      for (i8 i_Ord = 0; i_Ord < nOrders; i_Ord++) {
//        Column* pOrderColumn = pStreamDef_prior->pGetOrder(i_Ord);
//        Stz* szOrderColumnNm = pOrderColumn->szColNm();
//
//        Column* pStreamColumn = pStreamDef->pFindColumn(szOrderColumnNm);
//        i8 iColNo = pStreamColumn->iColumnNo();
//        pStreamDef->vAddOrder(iColNo);
//      }
      
      // apply the join index
      // tb be done...
      
      pStreamDef_prior = pStreamDef;
      return;
    }
  

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // an ORDAGGLIST maintains the ordered stream
  
//    if (_bIsCard_OrdAggList(pUnit)) {
//      Card_OrdAgg* pCard_OrdAgg = _pCard_OrdAggList(pUnit);
//      vTrace("vSetOrder : %s\n"_Stz,pCard_OrdAgg->szCardNm());
//
//      Card_StreamDef* pCard_StreamDef
//        = _pCard_StreamDef(pCard_OrdAgg->pFindSubCard(CN_CARD_STREAMDEF));
//      StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();
//
//      // apply prior
//      i8 nOrders = pStreamDef_prior->nOrders();
//      for (i8 i_Ord = 0; i_Ord < nOrders; i_Ord++) {
//        Column* pOrderColumn = pStreamDef_prior->pGetOrder(i_Ord);
//        Stz* szOrderColumnNm = pOrderColumn->szColNm();
//
//        Column* pStreamColumn = pStreamDef->pFindColumn(szOrderColumnNm);
//        i8 iColNo = pStreamColumn->iColumnNo();
//        pStreamDef->vAddOrder(iColNo);
//      }
//
//      pStreamDef_prior = pStreamDef;
//      return;
//    }

    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // AGGREGLIST

    if (_bIsCard_Aggreg(pUnit)) {
      Card_Aggreg* pCard_Aggreg = _pCard_Aggreg(pUnit);
      vTrace("vSetOrder : %s\n"_Stz,pCard_Aggreg->szCardNm());

      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pCard_Aggreg->pFindSubCard(CK_STREAMDEF));
      StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();

      //pStreamDef->vApplyOrder(pStreamDef_prior);

      Card_ByList* pCard_ByList
        = _pCard_ByList(pCard_Aggreg->pFindSubCard(CK_BYLIST));

      vOrderStreamDef(pStreamDef,pCard_ByList);
      
      pStreamDef_prior = pStreamDef;
      return;
    }

    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // PROJECTLIST

    if (_bIsCard_Project(pUnit)) {
      Card_Project* pCard_Project = _pCard_Project(pUnit);
      vTrace("vSetOrder : %s\n"_Stz,pCard_Project->szCardNm());

      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pCard_Project->pFindSubCard(CK_STREAMDEF));
      StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();
      
      pStreamDef->vApplyOrder(pStreamDef_prior);
      
      pStreamDef_prior = pStreamDef;
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ??
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  
  pBench->vWalkUnits(vSetOrder);
}

/*----------------------------------------------------------------------------*/
/* substitute stream column ordinals with stream column names                 */
/*----------------------------------------------------------------------------*/
void Tools::vSubstituteOrdinals(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  StreamDef* pStreamDef_recent = nullptr;
  Index* pIndex_recent = nullptr;

  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* test card and for those with column ref's replace ordinals with names  */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vSubstitute = [&pStreamDef_recent,&pIndex_recent](Card* pCard) mutable
  {
    vTrace("vSubstitute: %s\n"_Stz,pCard->szCardNm());
  
    // get running streamDefn
    if (_bIsCard_StreamDef(pCard)) {
      Card_StreamDef* pCard_StreamDef = _pCard_StreamDef(pCard);
      
      pStreamDef_recent = pCard_StreamDef->pStreamDef();
      return;
    }
  
    // get running index
    if (_bIsCard_Index(pCard)) {
      Card_Index* pCard_Index = _pCard_Index(pCard);
    
      pIndex_recent = pCard_Index->pIndex();
      return;
    }

    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // an AGGREGPRED will contain columns
  
    if (_bIsCard_AggregPred(pCard)) {
      Card_AggregPred* pCard_AggregPred = _pCard_AggregPred(pCard);
      
      vReplaceOrdinals(pCard_AggregPred,pStreamDef_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // an BYPRED will contain columns

    if (_bIsCard_ByPred(pCard)) {
      Card_ByPred* pCard_ByPred = _pCard_ByPred(pCard);
      
      vReplaceOrdinals(pCard_ByPred,pStreamDef_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // an COMPAREPRED will contain columns

    if (_bIsCard_ComparePred(pCard)) {
      Card_ComparePred* pCard_ComparePred = _pCard_ComparePred(pCard);
      
      vReplaceOrdinals(pCard_ComparePred,pStreamDef_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // an INDEXPRED will contain columns

    if (_bIsCard_IndexPred(pCard)) {
      Card_IndexPred* pCard_IndexPred = _pCard_IndexPred(pCard);
      
      vReplaceOrdinals(pCard_IndexPred,pStreamDef_recent,pIndex_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // an PROJECTPRED will contain columns

    if (_bIsCard_CalcPred(pCard)) {
      Card_CalcPred* pCard_CalcPred = _pCard_CalcPred(pCard);
      
      vReplaceOrdinals(pCard_CalcPred,pStreamDef_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // otherwise ignore
    return;
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  
  //
  // walk all cars, substituting columns names for ordinals
  //

  pBench->vWalkAllCards(vSubstitute);
}



/*----------------------------------------------------------------------------*/
/* does filterPred have some bound wrt given index?                           */
/*----------------------------------------------------------------------------*/
bool Tools::bHasBounds(Index* pIndex, Card_BoolPred* pCard_BoolPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pPred = (Card_PRED*)pCard_BoolPred->pGet(0);
  if (bHasBounds_pred(pIndex,pPred))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does predicate have some bound wrt given index?                            */
/*----------------------------------------------------------------------------*/
bool Tools::bHasBounds_pred(Index* pIndex, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  eCK eCard = pCard_PRED->eCard();
  
  switch (eCard) {
    
    case CK_ORPRED:
      return bHasBounds_or(pIndex,_pCard_OrPred(pCard_PRED));
      
    case CK_ANDPRED:
      return bHasBounds_and(pIndex,_pCard_AndPred(pCard_PRED));
      
    case CK_COMPAREPRED:
      return bHasBounds_compare(pIndex,_pCard_ComparePred(pCard_PRED));

    default:
      vTrace("bHasBounds_pred: unexpected eCard %lld %s\n"_Stz
             ,(i8)eCard
             ,aCK[eCard]);
      RASSERT(0);
      break;
  }
  

  return false;
}

/*----------------------------------------------------------------------------*/
/* does orPred have some bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Tools::bHasBounds_or(Index* pIndex, Card_OrPred* pCard_OrPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pSubPred0 = (Card_PRED*)pCard_OrPred->pGet(0);
  if (bHasBounds_pred(pIndex,pSubPred0))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)pCard_OrPred->pGet(1);
  if (bHasBounds_pred(pIndex,pSubPred1))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does this andPred have some bound on index?                                */
/*----------------------------------------------------------------------------*/
bool Tools::bHasBounds_and(Index* pIndex, Card_AndPred* pCard_AndPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pSubPred0 = (Card_PRED*)pCard_AndPred->pGet(0);
  if (bHasBounds_pred(pIndex,pSubPred0))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)pCard_AndPred->pGet(1);
  if (bHasBounds_pred(pIndex,pSubPred1))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this comparePred a bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Tools::bHasBounds_compare(Index* pIndex, Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  //Card_Term* pTerm1 = _pCard_Term(pCard_ComparePred->pGet(1));
  
  Card_Term* pTerm2 = _pCard_Term(pCard_ComparePred->pGet(2));
  if (BK_NONE == pTerm2->eBound())
    return false;

  Card_Term* pTerm0 = _pCard_Term(pCard_ComparePred->pGet(0));
  Stz* szTerm0 = pTerm0->szTerm();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return false;
  
  return true;
}



/*----------------------------------------------------------------------------*/
/* find a comparePred in filterPred which is the eBound of given index        */
/*----------------------------------------------------------------------------*/
Card_ComparePred* Tools::pFindBound(Index* pIndex,
                                    eBK eBound,
                                    Card_BoolPred* pCard_BoolPred
                                    )
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pCard0 = (Card_PRED*)pCard_BoolPred->pGet(0);
  if (Card_ComparePred* pFound = pFindBound_pred(pIndex,eBound,pCard0))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eBound of given index                      */
/*----------------------------------------------------------------------------*/
Card_ComparePred* Tools::pFindBound_pred(Index* pIndex,
                                         eBK eBound,
                                         Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  eCK eCard = pCard_PRED->eCard();
  
  switch (eCard) {
    
    case CK_ORPRED:
      return pFindBound_or(pIndex,eBound,_pCard_OrPred(pCard_PRED));
      
    case CK_ANDPRED:
      return pFindBound_and(pIndex,eBound,_pCard_AndPred(pCard_PRED));
      
    case CK_COMPAREPRED:
      return pFindBound_compare(pIndex,eBound,_pCard_ComparePred(pCard_PRED));

    default:
      vTrace("pFindBound_pred: unexpected eCard %lld %s\n"_Stz
             ,(i8)eCard
             ,aCK[eCard]);
      RASSERT(0);
      break;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eBound of given index                      */
/*----------------------------------------------------------------------------*/
Card_ComparePred* Tools::pFindBound_or(Index* pIndex,
                                       eBK eBound,
                                       Card_OrPred* pCard_OrPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pSubPred0 = (Card_PRED*)pCard_OrPred->pGet(0);
  if (Card_ComparePred* pFound = pFindBound_pred(pIndex,eBound,pSubPred0))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)pCard_OrPred->pGet(1);
  if (Card_ComparePred* pFound = pFindBound_pred(pIndex,eBound,pSubPred1))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eBound of given index                      */
/*----------------------------------------------------------------------------*/
Card_ComparePred* Tools::pFindBound_and(Index* pIndex,
                                        eBK eBound,
                                        Card_AndPred* pCard_AndPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pSubPred0 = (Card_PRED*)pCard_AndPred->pGet(0);
  if (Card_ComparePred* pFound = pFindBound_pred(pIndex,eBound,pSubPred0))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)pCard_AndPred->pGet(1);
  if (Card_ComparePred* pFound = pFindBound_pred(pIndex,eBound,pSubPred1))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eBound of given index                      */
/*----------------------------------------------------------------------------*/
Card_ComparePred* Tools::pFindBound_compare(Index* pIndex,
                                            eBK eBound,
                                            Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_Term* pTerm2 = _pCard_Term(pCard_ComparePred->pGet(2));
  if (eBound != pTerm2->eBound())
    return nullptr;

  Card_Term* pTerm0 = _pCard_Term(pCard_ComparePred->pGet(0));
  Stz* szTerm0 = pTerm0->szTerm();
  Column* pCol0 = pIndex->pGetColumn(0);
  if (! pCol0->szColNm()->bSame(szTerm0))
    return nullptr;
  
  return pCard_ComparePred;
}




/*----------------------------------------------------------------------------*/
/* remove given comparePred from filterPred                                   */
/*----------------------------------------------------------------------------*/
bool Tools::bRemoveBound(Card_BoolPred*  pCard_BoolPred,
                         Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pPred = (Card_PRED*)pCard_BoolPred->pGet(0);
  
  if (pPred == pCard_ComparePred) {
    pCard_BoolPred->vRemove(pCard_ComparePred);
    vDeleteComparePred(pCard_ComparePred);
    return true;
  }

  return bRemoveBound_pred(pPred,pCard_ComparePred);
}

/*----------------------------------------------------------------------------*/
/* remove given comparePred                                                   */
/*----------------------------------------------------------------------------*/
bool Tools::bRemoveBound_pred(Card_PRED* pCard_PRED,
                              Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  eCK eCard = pCard_PRED->eCard();
  
  switch (eCard) {
    
    case CK_ORPRED:
      if (bRemoveBound_or(_pCard_OrPred(pCard_PRED),pCard_ComparePred))
        return true;
      
    case CK_ANDPRED:
      if (bRemoveBound_and(_pCard_AndPred(pCard_PRED),pCard_ComparePred))
        return true;
      
    default:
      vTrace("vCardPred: unexpected eCard %lld %s\n"_Stz
             ,(i8)eCard
             ,aCK[eCard]);
      RASSERT(0);
      break;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* remove given comparePred                                                   */
/*----------------------------------------------------------------------------*/
bool Tools::bRemoveBound_or(Card_OrPred* pCard_OrPred,
                            Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pSubPred0 = (Card_PRED*)pCard_OrPred->pGet(0);
  if (pSubPred0 == pCard_ComparePred) {
    pCard_OrPred->vRemove(pCard_ComparePred);
    vDeleteComparePred(pCard_ComparePred);
    return true;
  }

  Card_PRED* pSubPred1 = (Card_PRED*)pCard_OrPred->pGet(1);
  if (pSubPred1 == pCard_ComparePred) {
    pCard_OrPred->vRemove(pCard_ComparePred);
    vDeleteComparePred(pCard_ComparePred);
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* remove given comparePred                                                   */
/*----------------------------------------------------------------------------*/
bool Tools::bRemoveBound_and(Card_AndPred* pCard_AndPred,
                            Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pCard0 = (Card_PRED*)pCard_AndPred->pGet(0);
  if (pCard0 == pCard_ComparePred) {
    pCard_AndPred->vRemove(pCard_ComparePred);
    vDeleteComparePred(pCard_ComparePred);
    return true;
  }

  Card_PRED* pCard1 = (Card_PRED*)pCard_AndPred->pGet(1);
  if (pCard1 == pCard_ComparePred) {
    pCard_AndPred->vRemove(pCard_ComparePred);
    vDeleteComparePred(pCard_ComparePred);
    return true;
  }

  return false;
}


/*----------------------------------------------------------------------------*/
/* reduce given filterPred                                                    */
/*----------------------------------------------------------------------------*/
void Tools::vReduceFilter(Card_BoolPred*  pCard_BoolPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pSubCard = (Card_PRED*)pCard_BoolPred->pGet(0);
  if (bReduciblePred(pSubCard)) {
    vReducePred(pCard_BoolPred,pSubCard);
    return;
  }
  

  Card_PRED* pCard0 = (Card_PRED*)pCard_BoolPred->pGet(0);
  vReduceFilter_pred(pCard0);
}


/*----------------------------------------------------------------------------*/
/* remove pred                                                                */
/*----------------------------------------------------------------------------*/
void Tools::vReduceFilter_pred(Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pSubCard;
  
  eCK eCard = pCard_PRED->eCard();
  
  switch (eCard) {
    
    case CK_ORPRED:
      pSubCard = (Card_PRED*)pCard_PRED->pGet(0);
      if (bReduciblePred(pSubCard))
        vReducePred(pCard_PRED,pSubCard);
      else
        vReduceFilter_pred(pSubCard);
      break;
      
    case CK_ANDPRED:
      pSubCard = (Card_PRED*)pCard_PRED->pGet(0);
      if (bReduciblePred(pSubCard))
        vReducePred(pCard_PRED,pSubCard);
      else
        vReduceFilter_pred(pSubCard);
      break;

    default:
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* if predicate reducible?                                                    */
/*----------------------------------------------------------------------------*/
bool Tools::bReduciblePred(Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  eCK eCard = pCard_PRED->eCard();
  i8 nSubCards = pCard_PRED->nItems();
  
  if (1 == nSubCards && (CK_ORPRED == eCard || CK_ANDPRED == eCard))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* remove child pred from parent                                              */
/*----------------------------------------------------------------------------*/
void Tools::vReducePred(Card_PRED* pCard_PRED_parent,
                        Card_PRED* pCard_PRED_child)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  pCard_PRED_parent->vDump();
  pCard_PRED_child->vDump();
  
  // the child will only have one subcard
  i8 nChildSubCards = pCard_PRED_child->nItems();
  RASSERT(1 == nChildSubCards);
  
  //pCard_PRED_parent->vRemove(pCard_PRED_child);
  //pCard_PRED_parent->vDump();
  
  Card_PRED* pCard_PRED_kid = (Card_PRED*)pCard_PRED_child->pPop();
  pCard_PRED_kid->vDump();

  pCard_PRED_parent->vReplace(pCard_PRED_child,pCard_PRED_kid);
  pCard_PRED_parent->vDump();

  delete pCard_PRED_child;
  
  pCard_PRED_parent->vDump();

}


/*----------------------------------------------------------------------------*/
/* delete given comparePred                                                   */
/*----------------------------------------------------------------------------*/
void Tools::vDeleteComparePred(Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  vDeleteRecursive(pCard_ComparePred);
}

/*----------------------------------------------------------------------------*/
/* delete card recursively                                                    */
/*----------------------------------------------------------------------------*/
void Tools::vDeleteRecursive(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
//  i8 nSubCards = pCard->nItems();
//  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
//    Card* pSubCard = pCard->pGet(i_SubCard);
//
//    vDeleteRecursive(pSubCard);
//  }
  
  
  while (Card* pSubCard = pCard->pPop()) {
    vDeleteRecursive(pSubCard);
  }
  
  delete pCard;
}



/*----------------------------------------------------------------------------*/
/*-- Private Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* order stream def by given byList                                           */
/*----------------------------------------------------------------------------*/
void Tools::vOrderStreamDef(StreamDef* pStreamDef,
                            Card_ByList* pCard_ByList)
{
  _static_

  ListExoIter2_t<Card>* pByList_I = pCard_ByList->pNewIter();

  while (pByList_I->bHasNext()) {
    Card* pPart = pByList_I->pNext();
    Card_By* pCard_By = _pCard_By(pPart);

    Card_ByPred* pCard_ByPred
      = _pCard_ByPred(pCard_By->pFindSubCard(CK_BYPRED));
    if (! pCard_ByPred)
      return;
    
    Card_Term* pCard_Term = _pCard_Term(pCard_ByPred->pGet(0));
    Stz* szByColumnNm =pCard_Term->szGetProperty("szTerm"_Stz);
    
    Column* pStreamColumn = pStreamDef->pFindColumn(szByColumnNm);
    i8 iColNo = pStreamColumn->iColumnNo();
    pStreamDef->vAddOrder(iColNo);
  }
  
  delete pByList_I;
}

/*----------------------------------------------------------------------------*/
/* produce the streamdef of a projective work unit                            */
/*----------------------------------------------------------------------------*/
StreamDef* Tools::pProduceStreamDef(Card_Project* pProjectiveUnit)
{
  _static_

  /*

    A projection pile can be one of the following:
   
      AggregList .7    //  OrdAggList .4    //  ProjectList .6
              AL_NODE  //          AL_NONE  //  .0)Project
      .0)Project       //  .0)Project       //  ..0)Type
      ..0)Type         //  ..0)Type         //          INTEGER
              INTEGER  //          INTEGER  //  ..1)As
      ..1)As           //  ..1)PrjPred      //          CUSTKEY
              CUSTKEY  //       L_ORDERKEY  //  ..2)PrjPred
      ..2)PrjPred      //                   //     L_LINENUMBER
           L_ORDERKEY  //                   //
                       //                   //
      .4)Aggreg        //  .1)Aggreg        //
              AL_NONE  //          AL_NONE  //
      ..0)Type         //  ..0)Type         //
              DECIMAL  //          DECIMAL  //
      ..1)As           //  ..1)AggPred      //
              SUM_QTY  //                )  //
      ..2)AggPred      //       L_QUANTITY  //
                    )  //              sum  //
           L_QUANTITY  //                   //
                  sum  //                   //
      0123456789ABCDE  //  0123456789ABCDE  //  0123456789ABCDE
  
  */
  
  StreamDef* pStreamDef_projected = new StreamDef;

  Card_StreamDef* pCard_StreamDef_projected
    = new Card_StreamDef(pStreamDef_projected);
  pProjectiveUnit->vAppend(pCard_StreamDef_projected);
  
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* examine given projective part and add its projected column to streamDef*/
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vAddColumn = [&pStreamDef_projected](Card* pProjectivePart) mutable
  {
    // a projection card will either be an aggreg or a project
//    ASSERT(_bIsCard_Aggreg(pItem_Projection) ||
//           _bIsCard_Project(pItem_Projection));
  
    vTrace("vAddColumn: %s\n"_Stz,pProjectivePart->szCardNm());
    pProjectivePart->vDump();
  
    s32 sColNm  = "(unknown)"_Stz;
    eCT eColumn = CT_NONE;;
    i8  iLength = 0;
    i8  iScale  = 0;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    // TYPE (mandatory)
    Card_Type* pElement_Type
      = _pCard_Type(pProjectivePart->pFindSubCard(CK_TYPE));
    if (pElement_Type) {
      eColumn = Column::eColumn(pElement_Type->szType());
      vTrace("vAddColumn: Type %s\n"_Stz,pElement_Type->szType());
    }
  
    // AS (optional)
    Card_As* pElement_As
      = _pCard_As(pProjectivePart->pFindSubCard(CK_AS));
    if (pElement_As) {
      sColNm = pElement_As->szAsNm();
      vTrace("vAddColumn: As %s\n"_Stz,sColNm.azChar());
    }
  
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // a projective part with have an AGGREGPRED element
  
    Card_AggregPred* pElement_AggregPred
      = _pCard_AggregPred(pProjectivePart->pFindSubCard(CK_AGGREGPRED));
      
    if (pElement_AggregPred) {
      Card_Term* pCard_Term = _pCard_Term(pElement_AggregPred->pGet(0));
      sColNm = pCard_Term->szGetProperty("szTerm"_Stz);
      vTrace("AGGREGPRED: %s %s\n"_Stz,sColNm.szStz(),aCT[eColumn]);

      pStreamDef_projected->vAddColumn(sColNm,eColumn,iLength,iScale);

      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or a PROJECTPRED element
  
    Card_CalcPred* pElement_ProjectPred
      = _pCard_CalcPred(pProjectivePart->pFindSubCard(CK_CALCPRED));

    if (pElement_ProjectPred) {
      Card_Term* pCard_Term = _pCard_Term(pElement_ProjectPred->pGet(0));
      sColNm = pCard_Term->szGetProperty("szTerm"_Stz);
      vTrace("PROJECTPRED: %s %s\n"_Stz,sColNm.szStz(),aCT[eColumn]);

      pStreamDef_projected->vAddColumn(sColNm,eColumn,iLength,iScale);
      
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ??
    NEVER_REACH
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  //
  // add the columns from all
  //   aggreg, ordagg and project cards of the given pile
  //   to the streamdef
  //
  
  ListExoIter2_t<Card>* pUnit_I = pProjectiveUnit->pNewIter();

  while (pUnit_I->bHasNext()) {
    Card* pUnit = pUnit_I->pNext();
    
    eCK eCard = pUnit->eCard();
    if (CK_PROJECTION == eCard)
      vAddColumn(pUnit);
  }
  
  delete pUnit_I;

  return pStreamDef_projected;
}

/*----------------------------------------------------------------------------*/
/* for given predicate replace any ordinal atom with stream column name       */
/*----------------------------------------------------------------------------*/
void Tools::vReplaceOrdinals(Card_PRED* pCard_Pred,
                             StreamDef* pStreamDef,
                             Index*     pIndex)
{
  _static_

  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* test for atom card and replace a column ordinal or an index ordinal    */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vReplaceAnOrdinal = [pStreamDef,pIndex](Card* pCard)
  {
    vTrace("vReplaceOrdinals: %s\n"_Stz,pCard->szCardNm());

    if (CK_TERM != pCard->eCard())
      return;

    Card_Term* pCard_Term = _pCard_Term(pCard);
  
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // an ordinal can appear as a column ordinal
  
    if (pCard_Term->bIsOrdinalColumnTerm()) {

      i8 i_Ord = pCard_Term->iOrdinalColumnValue();
      i8 i_Col = i_Ord - 1;

      Column* pColumn = pStreamDef->pGetColumn(i_Col);
      Stz* szColNm = pColumn->szColNm();

      pCard_Term->vAddProperty("szTerm"_Stz,szColNm);

      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or as an index ordinal
  
    if (pIndex && pCard_Term->bIsOrdinalIndexTerm()) {
      
      i8 i_IdxOrd = pCard_Term->iOrdinalIndexValue();
      i8 i_IdxCol = i_IdxOrd - 1;

      Column* pIdxColumn = pIndex->pGetColumn(i_IdxCol);
      Stz* szIdxColNm = pIdxColumn->szColNm();
    
      pCard_Term->vAddProperty("szTerm"_Stz,szIdxColNm);

      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // otherwise the atom is not a column reference
    return;
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  //
  // walk all subcards of predicate, replacing columns ordinals with names
  //

  pCard_Pred->vEachSubCard(vReplaceAnOrdinal);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
