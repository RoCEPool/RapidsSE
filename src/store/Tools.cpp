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
  
  StreamDef* pStreamDef_prior = nullptr;
  
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* examine given generative work unit and form the stream defn it emits   */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vFormStreamDef = [&pStreamDef_prior](Card* pGenerativeWU) mutable
  {
    // a generative work unit can either be a SCAN

    if (_bIsCard_Scan(pGenerativeWU)) {
      Card_Scan* pCard_Scan = _pCard_Scan(pGenerativeWU);

      Card_Table* pCard_Table
        = _pCard_Table(pCard_Scan->pFindSubCard(CN_CARD_TABLE));

      Table* pTable = pCard_Table->pTable();
      StreamDef* pStreamDef = new StreamDef(pTable->pStreamDef());
      Card_StreamDef* pCard_Stream = new Card_StreamDef(pStreamDef);
      
      pCard_Scan->vAppend(pCard_Stream);
      
      pStreamDef_prior = pStreamDef;
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an INDEXSCAN
  
    if (_bIsCard_IndexScan(pGenerativeWU)) {
      Card_IndexScan* pCard_IndexScan = _pCard_IndexScan(pGenerativeWU);

      Card_Table* pCard_Table
        = _pCard_Table(pCard_IndexScan->pFindSubCard(CN_CARD_TABLE));

      Table* pTable = pCard_Table->pTable();
      StreamDef* pStreamDef = new StreamDef(pTable->pStreamDef());
      Card_StreamDef* pCard_Stream = new Card_StreamDef(pStreamDef);
      
      pCard_IndexScan->vAppend(pCard_Stream);
      
      pStreamDef_prior = pStreamDef;
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an INDEXJOIN

    if (_bIsCard_IndexJoin(pGenerativeWU)) {
      Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pGenerativeWU);
      
      StreamDef* pStreamDef = new StreamDef(pStreamDef_prior);
      Table* pTable = pCard_IndexJoin->pIndex()->pTable();
      pStreamDef->vAppendStreamDef(pTable->pStreamDef());
      
      Card_StreamDef* pCard_Stream = new Card_StreamDef(pStreamDef);
      pCard_IndexJoin->vAppend(pCard_Stream);

      pStreamDef_prior = pStreamDef;
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // otherwise ignore
    // ??
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/


  pBench->vWalkUnits(vFormStreamDef);
}

/*----------------------------------------------------------------------------*/
/* renumber all subcards                                                      */
/*----------------------------------------------------------------------------*/
void Tools::vRenumberSubCards(Bench* pBench)
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
/* reproject stream definitions                                               */
/*----------------------------------------------------------------------------*/
void Tools::vReproject(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  StreamDef* pStreamDef_recent = nullptr;
  
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /*                                                                        */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vProjectPile = [&pStreamDef_recent](Card* pPile) mutable
  {
    //vTrace("vExpandStreamDefs: %s\n"_Stz,pCard->szCardNm());
  
    if (_bIsCard_Scan(pPile) || _bIsCard_IndexJoin(pPile)) {
      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pPile->pFindSubCard(CN_CARD_STREAMDEF));

      pStreamDef_recent = pCard_StreamDef->pStreamDef();
      
      return;
    }
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    if (_bIsCard_AggregList(pPile)) {
      vTrace("vReproject : %s\n"_Stz,pPile->szCardNm());
      Card_AggregList* pCard_AggregList = _pCard_AggregList(pPile);
      pStreamDef_recent = pProjectStreamDef(pCard_AggregList);
      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
  
    if (_bIsCard_OrdAggList(pPile)) {
      vTrace("vReproject : %s\n"_Stz,pPile->szCardNm());
      Card_OrdAggList* pCard_OrdAggList = _pCard_OrdAggList(pPile);
      pStreamDef_recent = pProjectStreamDef(pCard_OrdAggList);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
  
    if (_bIsCard_ProjectList(pPile)) {
      Card_ProjectList* pCard_ProjectList = _pCard_ProjectList(pPile);
      
      pStreamDef_recent = pProjectStreamDef(pCard_ProjectList);

      return;
    }
  
    // ??
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  
  pBench->vWalkUnits(vProjectPile);
}

/*----------------------------------------------------------------------------*/
/* refine stream def's with order                                             */
/*----------------------------------------------------------------------------*/
void Tools::vStreamOrder(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  StreamDef* pStreamDef_recent = nullptr;

  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /*                                                                        */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vSetOrder = [&pStreamDef_recent](Card* pCard) mutable
  {
    vTrace("vStreamOrder: %s %s\n"_Stz,pCard->szCardNm()
                                      ,aCN[pCard->eGetClassNo()]);
  
    if (_bIsCard_IndexScan(pCard)) {
      Card_IndexScan* pCard_IndexScan = _pCard_IndexScan(pCard);
      vTrace("vStreamOrder : %s\n"_Stz,pCard_IndexScan->szCardNm());

      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pCard->pFindSubCard(CN_CARD_STREAMDEF));

      
      return;
    }
  
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
  /*                                                                        */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vSubstitute = [&pStreamDef_recent,&pIndex_recent](Card* pCard) mutable
  {
    vTrace("vSubstituteOrdinals: %s\n"_Stz,pCard->szCardNm());
  
    if (_bIsCard_StreamDef(pCard)) {
      Card_StreamDef* pCard_StreamDef = _pCard_StreamDef(pCard);
      
      pStreamDef_recent = pCard_StreamDef->pStreamDef();
      return;
    }
  
    if (_bIsCard_Index(pCard)) {
      Card_Index* pCard_Index = _pCard_Index(pCard);
    
      pIndex_recent = pCard_Index->pIndex();
      return;
    }

    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

    if (_bIsCard_AggregPred(pCard)) {
      Card_AggregPred* pCard_AggregPred = _pCard_AggregPred(pCard);
      
      vReplaceOrdinals(pCard_AggregPred,pStreamDef_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
  
    if (_bIsCard_ComparePred(pCard)) {
      Card_ComparePred* pCard_ComparePred = _pCard_ComparePred(pCard);
      
      vReplaceOrdinals(pCard_ComparePred,pStreamDef_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
  
    if (_bIsCard_IndexPred(pCard)) {
      Card_IndexPred* pCard_IndexPred = _pCard_IndexPred(pCard);
      
      vReplaceOrdinals(pCard_IndexPred,pStreamDef_recent,pIndex_recent);
      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
  
    if (_bIsCard_ProjectPred(pCard)) {
      Card_ProjectPred* pCard_ProjectPred = _pCard_ProjectPred(pCard);
      
      vReplaceOrdinals(pCard_ProjectPred,pStreamDef_recent);
      return;
    }

    // ??
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/


  pBench->vWalkSubCards(vSubstitute);
}

/*----------------------------------------------------------------------------*/
/*-- Private Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* replace ordinal atom with stream column name                               */
/*----------------------------------------------------------------------------*/
void Tools::vReplaceOrdinals(Card_Pred* pCard_Pred,
                             StreamDef* pStreamDef,
                             Index*     pIndex)
{
  _static_

  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* test subcard and replace a column ordinal or an inde ordinal           */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vReplaceAnOrdinal = [pStreamDef,pIndex](Card* pSubCard)
  {
    vTrace("vReplaceOrdinals: %s\n"_Stz,pSubCard->szCardNm());

    if (CN_CARD_ATOM != pSubCard->eGetClassNo())
      return;

    Card_Atom* pCard_Atom = _pCard_Atom(pSubCard);
  
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // atom is either a column ordinal
  
    if (pCard_Atom->bIsOrdinalColumnAtom()) {

      i8 i_Ord = pCard_Atom->iOrdinalColumnValue();
      i8 i_Col = i_Ord - 1;

      Column* pColumn = pStreamDef->pGetColumn(i_Col);
      Stz* szColNm = pColumn->szColNm();

      pCard_Atom->vAtom(szColNm);

      return;
    }

    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or an index ordinal
  
    if (pIndex && pCard_Atom->bIsOrdinalIndexAtom()) {
      
      i8 i_IdxOrd = pCard_Atom->iOrdinalIndexValue();
      i8 i_IdxCol = i_IdxOrd - 1;

      Column* pIdxColumn = pIndex->pGetColumn(i_IdxCol);
      Stz* szIdxColNm = pIdxColumn->szColNm();
    
      pCard_Atom->vAtom(szIdxColNm);

      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
    // ...or is neither???
  
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  
  pCard_Pred->vEachSubCard(vReplaceAnOrdinal);
}

/*----------------------------------------------------------------------------*/
/* produce the streamdef of a projectList                                     */
/*----------------------------------------------------------------------------*/
StreamDef* Tools::pProjectStreamDef(Card_ProjectList* pPile_Projection)
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
  pPile_Projection->vAppend(pCard_StreamDef_reprojected);
  
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  /* examine given projection and add its column to the stream def          */
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/
  auto vAddColumn = [&pStreamDef_projected](Card* pItem_Projection) mutable
  {
    // a projection card will either be an aggreg or a project
    ASSERT(_bIsCard_Aggreg(pItem_Projection) ||
           _bIsCard_Project(pItem_Projection));
  
    vTrace("pProjectStreamDef: %s\n"_Stz,pItem_Projection->szCardNm());
  
    s32 sColNm  = "(unknown)"_Stz;
    eCT eColumn = CT_NONE;;
    i8  iLength = 0;
    i8  iScale  = 0;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    // TYPE (mandatory)
    Card_Type* pSubItem_Type
      = _pCard_Type(pItem_Projection->pFindSubCard(CN_CARD_TYPE));
    if (pSubItem_Type) {
      eColumn = pSubItem_Type->eColumnType();
      vTrace("pProjectStreamDef: Type %s\n"_Stz,aCT[eColumn]);
    }
  
    // AS (optional)
    Card_As* pSubItem_As
      = _pCard_As(pItem_Projection->pFindSubCard(CN_CARD_AS));
    if (pSubItem_As) {
      sColNm = pSubItem_As->szAsNm();
      vTrace("pProjectStreamDef: As %s\n"_Stz,sColNm.azChar());
    }
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // a projection with have either a AGGREGPRED
    Card_AggregPred* pSubItem_AggregPred
      = _pCard_AggregPred(pItem_Projection->pFindSubCard(CN_CARD_AGGREGPRED));
      
    if (pSubItem_AggregPred) {
      Card_Atom* pCard_Atom = _pCard_Atom(pSubItem_AggregPred->pGet(0));
      sColNm = pCard_Atom->szAtom();
      vTrace("AGGREGPRED: %s %s\n"_Stz,sColNm.szStz(),aCT[eColumn]);

      pStreamDef_projected->vAddColumn(sColNm,eColumn,iLength,iScale);

      return;
    }
  
    //  ||  ||  ||  || ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||  ||
  
    // ...or a PROJECTPRED
    Card_ProjectPred* pSubItem_ProjectPred
      = _pCard_ProjectPred(pItem_Projection->pFindSubCard(CN_CARD_PROJECTPRED));

    if (pSubItem_ProjectPred) {
      Card_Atom* pCard_Atom = _pCard_Atom(pSubItem_ProjectPred->pGet(0));
      sColNm = pCard_Atom->szAtom();
      vTrace("PROJECTPRED: %s %s\n"_Stz,sColNm.szStz(),aCT[eColumn]);

      pStreamDef_projected->vAddColumn(sColNm,eColumn,iLength,iScale);
      
      return;
    }

    // ??
    NEVER_REACH
  };
  /*λλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλλ*/

  //
  // add the columns from all
  //   aggreg, ordagg and project cards of the given pile
  //   to the streamdef
  //
  
  ListExoIter2_t<Card>* pPile_I = pPile_Projection->pNewIter();

  while (pPile_I->bHasNext()) {
    Card* pPileItem = pPile_I->pNext();
    
    eCN eClassNo = pPileItem->eGetClassNo();
    if (CN_CARD_AGGREG  == eClassNo ||
        CN_CARD_ORDAGG  == eClassNo ||
        CN_CARD_PROJECT == eClassNo)
      vAddColumn(pPileItem);
  }
  
  delete pPile_I;

  return pStreamDef_projected;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
