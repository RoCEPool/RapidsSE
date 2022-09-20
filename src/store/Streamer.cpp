/*============================================================================*/
/* Streamer.cpp --                                                           */
/*============================================================================*/

#include "Streamer.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Streamer,CN_STREAMER)
__aNewFunction(Streamer,CN_STREAMER)
__vDeleteFunction(Streamer,CN_STREAMER)
__aFooterFunction(Streamer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Streamer::Streamer(Bench* pBench)
: TransformerTool(CN_STREAMER, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);

  _checkInitialised_(CN_STREAMER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Streamer::~Streamer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append <stream> to all <absJoin>                                           */
/*----------------------------------------------------------------------------*/
void Streamer::vAppendStream_to_AbsJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STREAMER, _FOOTER_);

  Card_Table*  pCard_Table  = nullptr;
  Card_Stream* pCard_Stream = nullptr;
  Card_StreamColList *pCard_StreamColList = nullptr;

  Card_Scan* pCard_Scan_l = nullptr;
  Card_Scan* pCard_Scan_r = nullptr;

  vBeginIteration();

  while (Card *pMainCard = pEachMain()) {

    if (CK_SCAN == pMainCard->eCard()) {
      if (! pCard_Scan_l) {
        RASSERT(! pCard_Scan_r);
        pCard_Scan_l = _pCard_Scan(pMainCard);
      }
      else {
        RASSERT(! pCard_Scan_r);
        pCard_Scan_r = _pCard_Scan(pMainCard);
      }
      continue;
    }

    if (CK_ABSJOIN == pMainCard->eCard()) {
      Card_AbsJoin *pCard_absJoin = _pCard_AbsJoin(pMainCard);

      pCard_Stream = new Card_Stream;
      pCard_absJoin->vAppend(pCard_Stream);

      pCard_Table = _pCard_Table(pCard_Scan_l->pFindChildCard(CK_TABLE));
      if (pCard_Table) {
        StreamDef *pStreamDef = pCard_Table->pTable()->pStreamDef();
        pCard_StreamColList = new Card_StreamColList(pStreamDef);
        pCard_Stream->vAppend(pCard_StreamColList);
      }

      pCard_Table = _pCard_Table(pCard_Scan_r->pFindChildCard(CK_TABLE));
      if (pCard_Table) {
        StreamDef *pStreamDef = pCard_Table->pTable()->pStreamDef();
        Card_StreamColList *pCard_StreamColList_b = new Card_StreamColList(pStreamDef);
        pCard_StreamColList->vTransferAll(pCard_StreamColList_b);
        delete pCard_StreamColList_b;
      }

      pCard_Scan_l = nullptr;
      pCard_Scan_r = nullptr;
      continue;
    }
  }

  vEndIteration();

}

/*----------------------------------------------------------------------------*/
/* append <stream> to all <scan>                                              */
/*----------------------------------------------------------------------------*/
void Streamer::vAppendStream_to_Scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);

  vBeginIteration();

  while (Card *pMainCard = pEachMain()) {

    vTrace("Resolving MainCard %lld\n"_Stz,pMainCard->iCardNo());

    vResolveRecursive(pMainCard);
  }

  vEndIteration();
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Streamer::vResolveRecursive(Card* pAnyCard) // recursive
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STREAMER, _FOOTER_);

  eCK eCard = pAnyCard->eCard();

  //
  // subcards first
  //

  //pCard->vDump();

  i8 nSubCards = pAnyCard->nSubCards();
  for (i8 i_SubCard = 0; i_SubCard < nSubCards; i_SubCard++) {
    Card *pSubCard = pAnyCard->pGetSubCard(i_SubCard);
    vResolveRecursive(pSubCard);
  }

  //
  // this card
  //

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scan

  if (CK_SCAN == eCard) {
    Card_Table *pCard_Table = _pCard_Table(pAnyCard->pFindChildCard(CK_TABLE));
    if (pCard_Table) {

      StreamDef *pStreamDef = pCard_Table->pTable()->pStreamDef();

      Card_Stream *pCard_Stream = new Card_Stream;
      pAnyCard->vAppend(pCard_Stream);

      Card_StreamColList *pCard_StreamColList = new Card_StreamColList(pStreamDef);
      pCard_Stream->vAppend(pCard_StreamColList);
    }
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
