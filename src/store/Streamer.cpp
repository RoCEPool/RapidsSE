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
void Streamer::vCalc_streamOrdinalRanges(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);

  vBeginIteration();

  i8 iOrdLo = 1;
  i8 iOrdHi = C_UNKNOWN;

  char cStream = '0';

  while (Card* pMainCard = pEachMain()) {

    if (CK_SCAN == pMainCard->eCard()) {
      Card_Stream* pCard_stream = _pGetSub_Stream(pMainCard);

      pCard_stream->vMark1('#');
      pCard_stream->vMark2(cStream++);

      i8 nStreamCols = pCard_stream->nStreamCols();
      pCard_stream->vAddProperty_int(C_CARD_OrdLo,iOrdLo);
      iOrdHi = iOrdLo + nStreamCols - 1;
      pCard_stream->vAddProperty_int(C_CARD_OrdHi,iOrdHi);
      iOrdLo = iOrdHi + 1;

      continue;
    }

    if (CK_ABSJOIN == pMainCard->eCard()) {
      Card_Scan*   pCard_scan   = _pGetSub_Scan(pMainCard);
      Card_Stream* pCard_stream = _pGetSub_Stream(pCard_scan);

      pCard_stream->vMark1('#');
      pCard_stream->vMark2(cStream++);

      i8 nStreamCols = pCard_stream->nStreamCols();
      pCard_stream->vAddProperty_int(C_CARD_OrdLo,iOrdLo);
      iOrdHi = iOrdLo + nStreamCols - 1;
      pCard_stream->vAddProperty_int(C_CARD_OrdHi,iOrdHi);
      iOrdLo = iOrdHi + 1;

      continue;
    }
  }

  vEndIteration();

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Streamer::vCalc_streamOrdinalRanges"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append <stream> to all <scan>                                              */
/*----------------------------------------------------------------------------*/
void Streamer::vAppend_stream_to_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);

  BoardIter xBI(m_pBoard);

  // E <scan>
  while ((m_p_scan = _pCard_Scan(xBI.pEachMain(CK_SCAN)))) {

    // ? <scan>/<tablePtr>
    if (! (m_p_tablePtr = m_p_scan->pSub_tablePtr()))
      continue;
    // ? <scan>/<tablePtr>:pStreamDef
    if (! (m_pStreamDef = m_p_tablePtr->pStreamDef()))
      continue;

    // + <scan>/<stream>
    m_p_scan->vAppend((m_p_stream = new Card_Stream));
    // + <scan>/<stream>/<streamColList>
    m_p_stream->vAppend((m_p_streamColList = new Card_StreamColList(m_pStreamDef)));
  }

  m_pCardCtl->vPrintFormSubCards(CK_STREAM, CP_DEFAULT, true);

  //m_pCardCtl->vPrintSubCards(CK_EXPR,0);

  m_pCardCtl->vPrintForm(CK_PREDAND, CP_CARDFORM);
  m_pCardCtl->vPrintForm(CK_PREDCMP, CP_CARDFORM);
  m_pCardCtl->vPrintForm(CK_PREDOR, CP_CARDFORM);
  m_pCardCtl->vPrintForm(CK_TERM, CP_CARDFORM);

  if (m_bInteractive)
    m_pBoard->vPrintBoard("Streamer::vAppend_stream_to_scan"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
