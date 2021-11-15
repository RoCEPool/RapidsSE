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

  pBench->vWalkCards(fCardClosure);
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
  
    if (_bIsCard_ProjectList(pCard)) {
      Card_ProjectList* pCard_ProjectList = _pCard_ProjectList(pCard);
      
      pStreamDef_recent = pReproject(pStreamDef_recent,pCard_ProjectList);

      return;
    }
  
    // ??
  };

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
  
  pBench->vWalkSubCards(fCardClosure);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
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

  pCard_Pred->vEachSubCard(fCardClosure);
}

/*----------------------------------------------------------------------------*/
/* re-preoject the stream def given the projection                            */
/*----------------------------------------------------------------------------*/
StreamDef* Tools::pReproject(StreamDef* pStreamDef,
                             Card_ProjectList* pCard_ProjectList)
{
  _static_

  StreamDef* pStreamDef_revised = new StreamDef(pStreamDef);

  Card_StreamDef* pCard_StreamDef = new Card_StreamDef(pStreamDef);
  pCard_ProjectList->vAppend(pCard_StreamDef);
  
  auto fCardClosure = [](Card* pCard)
  {
    vTrace("pReproject: %s\n"_Stz,pCard->szCardNm());
  
    Card_As* pCard_As = _pCard_As(pCard->pFindSubCard(CN_CARD_AS));
    if (pCard_As) {
      vTrace("pReproject: As %s\n"_Stz,pCard_As->szAsNm());
    }
    else {
      
    }
  };
  
  pCard_ProjectList->vEachSubCard(fCardClosure,CN_CARD_PROJECT);
  
  return pStreamDef_revised;
}


/*----------------------------------------------------------------------------*/
/*-- Non-Static --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
