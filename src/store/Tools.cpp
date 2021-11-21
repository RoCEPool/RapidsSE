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

      Card_Table* pCard_Table
        = _pCard_Table(pCard_Scan->pFindSubCard(CN_CARD_TABLE));
      Table* pTable = pCard_Table->pTable();
      
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
        = _pCard_Table(pCard_IndexScan->pFindSubCard(CN_CARD_TABLE));
      Table* pTable = pCard_Table->pTable();
      
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
        = _pCard_StreamDef(pProjectiveUnit->pFindSubCard(CN_CARD_STREAMDEF));

      pStreamDef_prior = pCard_StreamDef->pStreamDef();
      return;
    }
  
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // a generative work unit will be either a AGGREGLIST

    if (_bIsCard_AggregList(pProjectiveUnit)) {
      vTrace("vReproject : %s\n"_Stz,pProjectiveUnit->szCardNm());
      Card_AggregList* pCard_AggregList = _pCard_AggregList(pProjectiveUnit);
      
      pStreamDef_prior = pProduceStreamDef(pCard_AggregList);
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an ORDAGGLIST

    if (_bIsCard_OrdAggList(pProjectiveUnit)) {
      vTrace("vReproject : %s\n"_Stz,pProjectiveUnit->szCardNm());
      Card_OrdAggList* pCard_OrdAggList = _pCard_OrdAggList(pProjectiveUnit);
      
      pStreamDef_prior = pProduceStreamDef(pCard_OrdAggList);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an PROJECTLIST

    if (_bIsCard_ProjectList(pProjectiveUnit)) {
      Card_ProjectList* pCard_ProjectList = _pCard_ProjectList(pProjectiveUnit);
      
      pStreamDef_prior = pProduceStreamDef(pCard_ProjectList);
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
/* renumber all cards                                                         */
/*----------------------------------------------------------------------------*/
void Tools::vRenumberCards(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  ListExoIter2_t<Board>* pBench_I = pBench->pNewIter();

  while (pBench_I->bHasNext()) {
    Board* pBoard = pBench_I->pNext();
    
    ListExoIter2_t<Card>* pBoard_I = pBoard->pNewIter();

    while (pBoard_I->bHasNext()) {
      Card* pWorkUnit = pBoard_I->pNext();
      
      pWorkUnit->vSubNo(-1);
      pWorkUnit->vRenumberSubCards();
    }
    delete pBoard_I;

  }
  delete pBench_I;
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
        = _pCard_StreamDef(pCard_IndexScan->pFindSubCard(CN_CARD_STREAMDEF));
      StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();
      
      Card_Index* pCard_Index
        = _pCard_Index(pCard_IndexScan->pFindSubCard(CN_CARD_INDEX));
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
        = _pCard_StreamDef(pCard_IndexJoin->pFindSubCard(CN_CARD_STREAMDEF));
      StreamDef* pStreamDef = pCard_StreamDef->pStreamDef();
      
      Card_Index* pCard_Index
        = _pCard_Index(pCard_IndexJoin->pFindSubCard(CN_CARD_INDEX));
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
//      Card_OrdAggList* pCard_OrdAggList = _pCard_OrdAggList(pUnit);
//      vTrace("vSetOrder : %s\n"_Stz,pCard_OrdAggList->szCardNm());
//
//      Card_StreamDef* pCard_StreamDef
//        = _pCard_StreamDef(pCard_OrdAggList->pFindSubCard(CN_CARD_STREAMDEF));
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

    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // PROJECTLIST

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
    vTrace("vSubstituteOrdinals: %s\n"_Stz,pCard->szCardNm());
  
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

    if (_bIsCard_ProjectPred(pCard)) {
      Card_ProjectPred* pCard_ProjectPred = _pCard_ProjectPred(pCard);
      
      vReplaceOrdinals(pCard_ProjectPred,pStreamDef_recent);
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
/*-- Private Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce the streamdef of a projective work unit                            */
/*----------------------------------------------------------------------------*/
StreamDef* Tools::pProduceStreamDef(Card_ProjectList* pProjectiveUnit)
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

  Card_StreamDef* pCard_StreamDef_reprojected
    = new Card_StreamDef(pStreamDef_projected);
  pProjectiveUnit->vAppend(pCard_StreamDef_reprojected);
  
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* examine given projective part and add its projected column to streamDef*/
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vAddColumn = [&pStreamDef_projected](Card* pProjectivePart) mutable
  {
    // a projection card will either be an aggreg or a project
    ASSERT(_bIsCard_Aggreg(pItem_Projection) ||
           _bIsCard_Project(pItem_Projection));
  
    vTrace("pProjectStreamDef: %s\n"_Stz,pProjectivePart->szCardNm());
  
    s32 sColNm  = "(unknown)"_Stz;
    eCT eColumn = CT_NONE;;
    i8  iLength = 0;
    i8  iScale  = 0;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    // TYPE (mandatory)
    Card_Type* pElement_Type
      = _pCard_Type(pProjectivePart->pFindSubCard(CN_CARD_TYPE));
    if (pElement_Type) {
      eColumn = pElement_Type->eColumnType();
      vTrace("pProjectStreamDef: Type %s\n"_Stz,aCT[eColumn]);
    }
  
    // AS (optional)
    Card_As* pElement_As
      = _pCard_As(pProjectivePart->pFindSubCard(CN_CARD_AS));
    if (pElement_As) {
      sColNm = pElement_As->szAsNm();
      vTrace("pProjectStreamDef: As %s\n"_Stz,sColNm.azChar());
    }
  
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // a projective part with have an AGGREGPRED element
  
    Card_AggregPred* pElement_AggregPred
      = _pCard_AggregPred(pProjectivePart->pFindSubCard(CN_CARD_AGGREGPRED));
      
    if (pElement_AggregPred) {
      Card_Atom* pCard_Atom = _pCard_Atom(pElement_AggregPred->pGet(0));
      sColNm = pCard_Atom->szAtom();
      vTrace("AGGREGPRED: %s %s\n"_Stz,sColNm.szStz(),aCT[eColumn]);

      pStreamDef_projected->vAddColumn(sColNm,eColumn,iLength,iScale);

      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or a PROJECTPRED element
  
    Card_ProjectPred* pElement_ProjectPred
      = _pCard_ProjectPred(pProjectivePart->pFindSubCard(CN_CARD_PROJECTPRED));

    if (pElement_ProjectPred) {
      Card_Atom* pCard_Atom = _pCard_Atom(pElement_ProjectPred->pGet(0));
      sColNm = pCard_Atom->szAtom();
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
    
    eCN eClassNo = pUnit->eGetClassNo();
    if (CN_CARD_AGGREG  == eClassNo ||
        CN_CARD_ORDAGG  == eClassNo ||
        CN_CARD_PROJECT == eClassNo)
      vAddColumn(pUnit);
  }
  
  delete pUnit_I;

  return pStreamDef_projected;
}

/*----------------------------------------------------------------------------*/
/* for given predicate replace any ordinal atom with stream column name       */
/*----------------------------------------------------------------------------*/
void Tools::vReplaceOrdinals(Card_Pred* pCard_Pred,
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

    if (CN_CARD_ATOM != pCard->eGetClassNo())
      return;

    Card_Atom* pCard_Atom = _pCard_Atom(pCard);
  
    //  ??  ??  ??  ?? ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??  ??
    // an ordinal can appear as a column ordinal
  
    if (pCard_Atom->bIsOrdinalColumnAtom()) {

      i8 i_Ord = pCard_Atom->iOrdinalColumnValue();
      i8 i_Col = i_Ord - 1;

      Column* pColumn = pStreamDef->pGetColumn(i_Col);
      Stz* szColNm = pColumn->szColNm();

      pCard_Atom->vAtom(szColNm);

      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or as an index ordinal
  
    if (pIndex && pCard_Atom->bIsOrdinalIndexAtom()) {
      
      i8 i_IdxOrd = pCard_Atom->iOrdinalIndexValue();
      i8 i_IdxCol = i_IdxOrd - 1;

      Column* pIdxColumn = pIndex->pGetColumn(i_IdxCol);
      Stz* szIdxColNm = pIdxColumn->szColNm();
    
      pCard_Atom->vAtom(szIdxColNm);

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
