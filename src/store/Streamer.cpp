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
void Streamer::vCalc_OrdinalRangesOf_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);

  vTransformation(OT_vCalc_OrdinalRangesOf_stream);
  vExplain("Blah"_Stz);

  i8 iOrdLo = 1;
  i8 iOrdHi = C_UNKNOWN;
  char cStream = '0';
  i8 iStream = 0;

  BoardIter xBI(m_pBoard);

  while ((m_pMainCard = xBI.pEachMain())) {

    if (CK_SCAN == m_pMainCard->eCard()) {
      m_p_stream = _pSub_Stream(m_pMainCard);

      //m_p_stream->vMark1('#');
      //m_p_stream->vMark2(cStream++);

      // <scan>/<stream>:iOrdLo,iOrdHi
      i8 nStreamCols = m_p_stream->nStreamCols();
      m_p_stream->vAddProperty_int(C_PPT_OrdLo,iOrdLo);
      iOrdHi = iOrdLo + nStreamCols - 1;
      m_p_stream->vAddProperty_int(C_PPT_OrdHi,iOrdHi);
      iOrdLo = iOrdHi + 1;
      m_p_stream->vAddProperty_int(C_PPT_Stream,iStream++);

      continue;
    }

    if (CK_ABSJOIN == m_pMainCard->eCard()) {
      m_p_scan   = _pSub_Scan(m_pMainCard);
      m_p_stream = _pSub_Stream(m_p_scan);

      //m_p_stream->vMark1('#');
      //m_p_stream->vMark2(cStream++);

      // <absJoin>/<stream>:iOrdLo,iOrdHi
      i8 nStreamCols = m_p_stream->nStreamCols();
      m_p_stream->vAddProperty_int(C_PPT_OrdLo,iOrdLo);
      iOrdHi = iOrdLo + nStreamCols - 1;
      m_p_stream->vAddProperty_int(C_PPT_OrdHi,iOrdHi);
      iOrdLo = iOrdHi + 1;
      m_p_stream->vAddProperty_int(C_PPT_Stream,iStream++);

      continue;
    }
  }

  vPrintBoard("Streamer::vCalc_OrdinalRangesOf_stream"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append <stream> to all <scan>                                              */
/*----------------------------------------------------------------------------*/
void Streamer::vAppend_stream_To_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STREAMER,_FOOTER_);

  vTransformation(OT_vAppend_stream_To_scan);
  vExplain("Blah"_Stz);

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

  vSetPrintSubCards(CK_STREAM, true);

  vPrintBoard("Streamer::vAppend_stream_To_scan"_Stz);

  vSetPrintSubCards(CK_STREAM, false);

}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
