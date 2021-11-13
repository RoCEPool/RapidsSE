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
  
    if (CN_CARD_SCAN == pCard->eGetClassNo()) {
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
  
    if (CN_CARD_INDEXJOIN == pCard->eGetClassNo()) {
      Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pCard);
      
      StreamDef* pStreamDef = new StreamDef(pStreamDef_recent);
      Table* pTable = pCard_IndexJoin->pIndex()->pTable();
      pStreamDef->vAppendStreamDef(pTable->pStreamDef());
      
      Card_StreamDef* pCard_Stream = new Card_StreamDef(pStreamDef);
      pCard_IndexJoin->vAppend(pCard_Stream);

      pStreamDef_recent = pStreamDef;
      return;
    }
  
    if (CN_CARD_PROJECT == pCard->eGetClassNo()) {
      Card_Project* pCard_Project = _pCard_Project(pCard);

     return;
    }
  
    // ??
  };

  pBench->vEachCard(fCardClosure);
}

/*----------------------------------------------------------------------------*/
/* substitute stream column ordinals with stream column names                 */
/*----------------------------------------------------------------------------*/
void Tools::vSubstituteOrdinals(Bench* pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  StreamDef* pStreamDef_recent = nullptr;
  
  auto fCardClosure = [&pStreamDef_recent](Card* pCard) mutable
  {
    vTrace("vSubstituteOrdinals: %s\n"_Stz,pCard->szCardNm());
  
    if (CN_CARD_SCAN == pCard->eGetClassNo()) {
      Card_Scan* pCard_Scan = _pCard_Scan(pCard);

      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pCard_Scan->pFindSubCard(CN_CARD_STREAMDEF));
      
      pStreamDef_recent = pCard_StreamDef->pStreamDef();
      return;
    }

    if (CN_CARD_FILTER == pCard->eGetClassNo()) {
      Card_Filter* pCard_Filter = _pCard_Filter(pCard);

      Card_ComparePred* pCard_ComparePred
        = _pCard_ComparePred(pCard_Filter->pFindSubCard(CN_CARD_COMPAREPRED));

      if (! pCard_ComparePred)
        return;
      
      vReplaceOrdinals(pCard_ComparePred,pStreamDef_recent);
      return;
    }

    if (CN_CARD_INDEXJOIN == pCard->eGetClassNo()) {
      Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pCard);

      
      Card_IndexPred* pCard_IndexPred
        = _pCard_IndexPred(pCard_IndexJoin->pFindSubCard(CN_CARD_INDEXPRED));

      Card_Index* pCard_Index
        = _pCard_Index(pCard_IndexJoin->pFindSubCard(CN_CARD_INDEX));

      Index* pIndex = pCard_Index->pIndex();

      vReplaceOrdinals(pCard_IndexPred,pStreamDef_recent,pIndex);

      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pCard_IndexJoin->pFindSubCard(CN_CARD_STREAMDEF));
      
      pStreamDef_recent = pCard_StreamDef->pStreamDef();
      return;
    }
  
    // ??
  };

  pBench->vEachCard(fCardClosure);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* replace ordinal atom with stream column name                               */
/*----------------------------------------------------------------------------*/
void Tools::vReplaceOrdinals(Card_ComparePred* pCard_ComparePred,
                             StreamDef* pStreamDef)
{
  _static_

  auto fCardClosure = [pStreamDef](Card* pSubCard)
  {
    vTrace("vReplaceOrdinals: %s\n"_Stz,pSubCard->szCardNm());

    if (CN_CARD_ATOM != pSubCard->eGetClassNo())
      return;
  
    Card_Atom* pCard_Atom = _pCard_Atom(pSubCard);
  
    if (! pCard_Atom->bIsOrdinalColumnAtom())
      return;
  
    i8 i_Ord = pCard_Atom->iOrdinalColumnValue();
    i8 i_Col = i_Ord - 1;

    Column* pColumn = pStreamDef->pGetColumn(i_Col);
    Stz* szColNm = pColumn->szColNm();
  
    pCard_Atom->vAtom(szColNm);
  };
  
  pCard_ComparePred->vEachSubCard(fCardClosure);
}

/*----------------------------------------------------------------------------*/
/* replace ordinal atom with stream column name                               */
/*----------------------------------------------------------------------------*/
void Tools::vReplaceOrdinals(Card_IndexPred* pCard_IndexPred,
                             StreamDef* pStreamDef,
                             Index* pIndex)
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
  
    if (pCard_Atom->bIsOrdinalIndexAtom()) {
      
      i8 i_IdxOrd = pCard_Atom->iOrdinalIndexValue();
      i8 i_IdxCol = i_IdxOrd - 1;

      Column* pIdxColumn = pIndex->pGetColumn(i_IdxCol);
      Stz* szIdxColNm = pIdxColumn->szColNm();
    
      pCard_Atom->vAtom(szIdxColNm);

      return;
    }
  
    // ??
  };
  
  pCard_IndexPred->vEachSubCard(fCardClosure);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
