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
  
    if (CN_CARD_FILTER == pCard->eGetClassNo()) {
      Card_Filter* pCard_Filter = _pCard_Filter(pCard);

      Card_ComparePred* pCard_ComparePred
        = _pCard_ComparePred(pCard_Filter->pFindSubCard(CN_CARD_COMPAREPRED));

      if (! pCard_ComparePred)
        return;
      
      vReplaceOrdinals(pCard_ComparePred,pStreamDef_recent);
      return;
    }

    if (CN_CARD_SCAN == pCard->eGetClassNo()) {
      Card_Scan* pCard_Scan = _pCard_Scan(pCard);

      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pCard_Scan->pFindSubCard(CN_CARD_STREAMDEF));
      
      pStreamDef_recent = pCard_StreamDef->pStreamDef();
      return;
    }
  
    if (CN_CARD_INDEXJOIN == pCard->eGetClassNo()) {
      Card_IndexJoin* pCard_IndexJoin = _pCard_IndexJoin(pCard);

      Card_StreamDef* pCard_StreamDef
        = _pCard_StreamDef(pCard_IndexJoin->pFindSubCard(CN_CARD_STREAMDEF));
      
      pStreamDef_recent = pCard_StreamDef->pStreamDef();
      return;
    }
  };

  pBench->vEachCard(fCardClosure);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* is atom an ordinal?                                                        */
/*----------------------------------------------------------------------------*/
bool Tools::bIsOrdinalAtom(Card_Atom* pCard_Atom)
{
  _static_
  
  Stz* szAtom = pCard_Atom->szAtom();
  char acFirst = *(szAtom->azChar());
  
  if ('$' == acFirst)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get ordical value                                                          */
/*----------------------------------------------------------------------------*/
i8 Tools::iOrdinalValue(Card_Atom* pCard_Atom)
{
  _static_

  Stz* szAtom = pCard_Atom->szAtom();

  i8 i_Ord = StrLib::iGleanInt(szAtom->azChar());
  
  return i_Ord;
}

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
  
    if (! bIsOrdinalAtom(pCard_Atom))
      return;
  
    i8 i_Ord = iOrdinalValue(pCard_Atom);
    i8 i_Col = i_Ord - 1;

    Column* pColumn = pStreamDef->pGetColumn(i_Col);
    Stz* szColNm = pColumn->szColNm();
  
    pCard_Atom->vAtom(szColNm);
  };
  
  pCard_ComparePred->vEachSubCard(fCardClosure);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
