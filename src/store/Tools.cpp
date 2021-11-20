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
/* expand stream definitions                                                  */
/*----------------------------------------------------------------------------*/
void Tools::vExpandStreamDefs(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  StreamDef* pStreamDef_recent = nullptr;
  
  auto fCardClosure = [&pStreamDef_recent](Card* pCard) mutable
  {
    //vTrace("vExpandStreamDefs: %s\n"_Stz,pCard->szCardNm());
  
    if (_bIsCard_Scan(pCard)) {
      Card_Scan* pCard_Scan = _pCard_Scan(pCard);

      Card_Table* pCard_Table
        = _pCard_Table(pCard_Scan->pFindSubCard(CN_CARD_TABLE));

      Table* pTable = pCard_Table->pTable();
      StreamDef* pStreamDef = new StreamDef(pTable->pStreamDef());
      Card_StreamDef* pCard_Stream = new Card_StreamDef(pStreamDef);
      
      pCard_Scan->vAppend(pCard_Stream);
      
      pStreamDef_recent = pStreamDef;
      return;
    }
  
    if (_bIsCard_IndexScan(pCard)) {
      Card_IndexScan* pCard_IndexScan = _pCard_IndexScan(pCard);

      Card_Table* pCard_Table
        = _pCard_Table(pCard_IndexScan->pFindSubCard(CN_CARD_TABLE));

      Table* pTable = pCard_Table->pTable();
      StreamDef* pStreamDef = new StreamDef(pTable->pStreamDef());
      Card_StreamDef* pCard_Stream = new Card_StreamDef(pStreamDef);
      
      pCard_IndexScan->vAppend(pCard_Stream);
      
      pStreamDef_recent = pStreamDef;
      return;
    }
  
    if (_bIsCard_IndexJoin(pCard)) {
      Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pCard);
      
      StreamDef* pStreamDef = new StreamDef(pStreamDef_recent);
      Table* pTable = pCard_IndexJoin->pIndex()->pTable();
      pStreamDef->vAppendStreamDef(pTable->pStreamDef());
      
      Card_StreamDef* pCard_Stream = new Card_StreamDef(pStreamDef);
      pCard_IndexJoin->vAppend(pCard_Stream);

      pStreamDef_recent = pStreamDef;
      return;
    }
    
    // ??
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pBench->vWalkCards(fCardClosure);
}

/*----------------------------------------------------------------------------*/
/* renumber cards                                                             */
/*----------------------------------------------------------------------------*/
void Tools::vRenumber(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  ListExoIter2_t<Board>* pBench_I = pBench->pNewIter();

  while (pBench_I->bHasNext()) {
    Board* pBoard = pBench_I->pNext();
    
    ListExoIter2_t<Card>* pBoard_I = pBoard->pNewIter();

    while (pBoard_I->bHasNext()) {
      Card* pCard = pBoard_I->pNext();
      
      pCard->vSubNo(-1);
      pCard->vRenumberSubCards();
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
  
  auto fCardClosure = [&pStreamDef_recent](Card* pCard) mutable
  {
    //vTrace("vExpandStreamDefs: %s\n"_Stz,pCard->szCardNm());
  
    if (_bIsCard_Scan(pCard) || _bIsCard_IndexJoin(pCard)) {
      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pCard->pFindSubCard(CN_CARD_STREAMDEF));

      pStreamDef_recent = pCard_StreamDef->pStreamDef();
      
      return;
    }
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    if (_bIsCard_AggregList(pCard)) {
      vTrace("vReproject : %s\n"_Stz,pCard->szCardNm());
      Card_AggregList* pCard_AggregList = _pCard_AggregList(pCard);
      pStreamDef_recent = pReproject(pCard_AggregList,pStreamDef_recent);
      return;
    }
  
    if (_bIsCard_OrdAggList(pCard)) {
      vTrace("vReproject : %s\n"_Stz,pCard->szCardNm());
      Card_OrdAggList* pCard_OrdAggList = _pCard_OrdAggList(pCard);
      pStreamDef_recent = pReproject(pCard_OrdAggList,pStreamDef_recent);
      return;
    }

    if (_bIsCard_ProjectList(pCard)) {
      Card_ProjectList* pCard_ProjectList = _pCard_ProjectList(pCard);
      
      pStreamDef_recent = pReproject(pCard_ProjectList,pStreamDef_recent);

      return;
    }
  
    // ??
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pBench->vWalkCards(fCardClosure);
}

/*----------------------------------------------------------------------------*/
/* refine stream def's with order                                             */
/*----------------------------------------------------------------------------*/
void Tools::vStreamOrder(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  StreamDef* pStreamDef_recent = nullptr;

  auto fCardClosure = [&pStreamDef_recent](Card* pCard) mutable
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pBench->vWalkCards(fCardClosure);
}

/*----------------------------------------------------------------------------*/
/* substitute stream column ordinals with stream column names                 */
/*----------------------------------------------------------------------------*/
void Tools::vSubstituteOrdinals(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  StreamDef* pStreamDef_recent = nullptr;
  Index* pIndex_recent = nullptr;

  auto fCardClosure = [&pStreamDef_recent,&pIndex_recent](Card* pCard) mutable
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

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    if (_bIsCard_AggregPred(pCard)) {
      Card_AggregPred* pCard_AggregPred = _pCard_AggregPred(pCard);
      
      vReplaceOrdinals(pCard_AggregPred,pStreamDef_recent);
      return;
    }

    if (_bIsCard_ComparePred(pCard)) {
      Card_ComparePred* pCard_ComparePred = _pCard_ComparePred(pCard);
      
      vReplaceOrdinals(pCard_ComparePred,pStreamDef_recent);
      return;
    }

    if (_bIsCard_IndexPred(pCard)) {
      Card_IndexPred* pCard_IndexPred = _pCard_IndexPred(pCard);
      
      vReplaceOrdinals(pCard_IndexPred,pStreamDef_recent,pIndex_recent);
      return;
    }

    if (_bIsCard_ProjectPred(pCard)) {
      Card_ProjectPred* pCard_ProjectPred = _pCard_ProjectPred(pCard);
      
      vReplaceOrdinals(pCard_ProjectPred,pStreamDef_recent);
      return;
    }

  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pBench->vWalkSubCards(fCardClosure);
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

  auto fCardClosure = [pStreamDef,pIndex](Card* pSubCard)
  {
    vTrace("vReplaceOrdinals: %s\n"_Stz,pSubCard->szCardNm());

    if (CN_CARD_ATOM != pSubCard->eGetClassNo())
      return;

    Card_Atom* pCard_Atom = _pCard_Atom(pSubCard);

    if (pCard_Atom->bIsOrdinalColumnAtom()) {

      i8 i_Ord = pCard_Atom->iOrdinalColumnValue();
      i8 i_Col = i_Ord - 1;

      Column* pColumn = pStreamDef->pGetColumn(i_Col);
      Stz* szColNm = pColumn->szColNm();

      pCard_Atom->vAtom(szColNm);

      return;
    }

    if (pIndex && pCard_Atom->bIsOrdinalIndexAtom()) {
      
      i8 i_IdxOrd = pCard_Atom->iOrdinalIndexValue();
      i8 i_IdxCol = i_IdxOrd - 1;

      Column* pIdxColumn = pIndex->pGetColumn(i_IdxCol);
      Stz* szIdxColNm = pIdxColumn->szColNm();
    
      pCard_Atom->vAtom(szIdxColNm);

      return;
    }
    // ??
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pCard_Pred->vEachSubCard(fCardClosure);
}

/*----------------------------------------------------------------------------*/
/* re-project the projection into a streamdef                                 */
/*----------------------------------------------------------------------------*/
StreamDef* Tools::pReproject(Card_ProjectList* pCard_ProjectList,
                             StreamDef* )
{
  _static_

  StreamDef* pStreamDef_reprojected = new StreamDef;

  Card_StreamDef* pCard_StreamDef_reprojected
    = new Card_StreamDef(pStreamDef_reprojected);
  pCard_ProjectList->vAppend(pCard_StreamDef_reprojected);
  
  auto fCardClosure = [&pStreamDef_reprojected](Card* pCard) mutable
  {
    vTrace("pReproject: %s\n"_Stz,pCard->szCardNm());
  
    s32 sColNm  = "(unknown)"_Stz;
    eCT eColumn = CT_NONE;;
    i8  iLength = 0;
    i8  iScale  = 0;
  
    Card_Type* pCard_Type = _pCard_Type(pCard->pFindSubCard(CN_CARD_TYPE));
    if (pCard_Type) {
      eColumn = pCard_Type->eColumnType();
      vTrace("pReproject: Type %s\n"_Stz,aCT[eColumn]);
    }
  
    Card_As* pCard_As = _pCard_As(pCard->pFindSubCard(CN_CARD_AS));
    if (pCard_As) {
      sColNm = pCard_As->szAsNm();
      vTrace("pReproject: As %s\n"_Stz,sColNm.azChar());
    }
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Card_ProjectPred* pCard_ProjectPred
      = _pCard_ProjectPred(pCard->pFindSubCard(CN_CARD_PROJECTPRED));

    if (pCard_ProjectPred) {
      Card_Atom* pCard_Atom = _pCard_Atom(pCard_ProjectPred->pGet(0));
      vTrace("Unk: %s\n"_Stz,pCard_Atom->szAtom());
      sColNm = pCard_Atom->szAtom();

      pStreamDef_reprojected->vAddColumn(sColNm,eColumn,iLength,iScale);
      
      return;
    }

    //  --  --  --  --  --  --  --  --  --  --  --  --
  
    Card_AggregPred* pCard_AggregPred
      = _pCard_AggregPred(pCard->pFindSubCard(CN_CARD_AGGREGPRED));
      
    if (pCard_AggregPred) {
      Card_Atom* pCard_Atom = _pCard_Atom(pCard_AggregPred->pGet(0));
      vTrace("Unk: %s\n"_Stz,pCard_Atom->szAtom());
      sColNm = pCard_Atom->szAtom();

      pStreamDef_reprojected->vAddColumn(sColNm,eColumn,iLength,iScale);
 
      return;
    }
  
    // ??
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ListExoIter2_t<Card>* pCard_I = pCard_ProjectList->pNewIter();

  while (pCard_I->bHasNext()) {
    Card* pSubCard = pCard_I->pNext();
    
    eCN eClassNo = pSubCard->eGetClassNo();
    if (CN_CARD_PROJECT == eClassNo ||
        CN_CARD_AGGREG  == eClassNo)
      fCardClosure(pSubCard);
  }
  
  delete pCard_I;

  return pStreamDef_reprojected;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
