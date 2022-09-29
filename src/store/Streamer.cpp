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
/* calc ordinal ranges                                                        */
/*----------------------------------------------------------------------------*/
void Streamer::vOrdinal_Range(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);

  vBeginIteration();

  i8 iOrdLo = 1;
  i8 iOrdHi = C_UNKNOWN;

  char cStream = '0';

  while (Card* pMainCard = pEachMain()) {

    if (CK_SCAN == pMainCard->eCard()) {
      Card_Stream* pCard_stream = _pGetSub_Card_Stream(pMainCard);

      pCard_stream->vMark1('#');
      pCard_stream->vMark2(cStream++);

      i8 nStreamCols = pCard_stream->nStreamCols();
      pCard_stream->vAddProperty("iOrdLo"_Stz,iOrdLo);
      iOrdHi = iOrdLo + nStreamCols - 1;
      pCard_stream->vAddProperty("iOrdHi"_Stz,iOrdHi);
      iOrdLo = iOrdHi + 1;

      continue;
    }

    if (CK_ABSJOIN == pMainCard->eCard()) {
      Card_Scan*   pCard_scan   = _pGetSub_Card_Scan(pMainCard);
      Card_Stream* pCard_stream = _pGetSub_Card_Stream(pCard_scan);

      pCard_stream->vMark1('#');
      pCard_stream->vMark2(cStream++);

      i8 nStreamCols = pCard_stream->nStreamCols();
      pCard_stream->vAddProperty("iOrdLo"_Stz,iOrdLo);
      iOrdHi = iOrdLo + nStreamCols - 1;
      pCard_stream->vAddProperty("iOrdHi"_Stz,iOrdHi);
      iOrdLo = iOrdHi + 1;

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

  while (Card_Scan* pCard_scan = _pCard_Scan(pEachMain(CK_SCAN))) {
    Card_TablePtr *pCard_TablePtr = _pCard_TablePtr(pCard_scan->pFindChildCard(CK_TABLEPTR));
    if (pCard_TablePtr) {

      StreamDef *pStreamDef = pCard_TablePtr->pTable()->pStreamDef();

      Card_Stream *pCard_Stream = new Card_Stream;
      pCard_scan->vAppend(pCard_Stream);

      Card_StreamColList *pCard_StreamColList = new Card_StreamColList(pStreamDef);
      pCard_Stream->vAppend(pCard_StreamColList);
    }

  }

  vEndIteration();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
