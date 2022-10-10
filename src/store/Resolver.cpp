/*============================================================================*/
/* Resolver.cpp --                                                           */
/*============================================================================*/

#include "Resolver.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Resolver,CN_RESOLVER)
__aNewFunction(Resolver,CN_RESOLVER)
__vDeleteFunction(Resolver,CN_RESOLVER)
__aFooterFunction(Resolver)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Resolver::Resolver(Bench* pBench)
: TransformerTool(CN_RESOLVER, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESOLVER,_FOOTER_);

  m_pStream_l    = nullptr;
  m_pStream_r    = nullptr;

  _checkInitialised_(CN_RESOLVER);

  m_pBoard = m_pBench->pGetBoard(BK_QUERY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_RESOLVER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Resolver::~Resolver(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESOLVER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* resolve $n into $$c.n                                                      */
/*----------------------------------------------------------------------------*/
void Resolver::vResolve_streamColOrdinal_To_tableColOrdinal
                (String* psTableColOrdinal, Stz* szStreamColOrdinal)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_RESOLVER, _FOOTER_);

  RASSERT(BK_QUERY == m_pBoard->eBoard());

  i8 iStreamColNo = szStreamColOrdinal->iOrdinal();

  BoardIter xBI(m_pBoard);

  while ( (m_pMainCard = xBI.pEachMain()) ) {
    if (! (m_p_stream = _pSub_Stream(m_pMainCard)) )
      continue;

    i8 iOrdLo = m_p_stream->iGetProperty(C_PPY_OrdLo);
    i8 iOrdHi = m_p_stream->iGetProperty(C_PPY_OrdHi);

    if (! (iStreamColNo >= iOrdLo && iStreamColNo <= iOrdHi) )
      continue;

    i8 iTableColNo = iStreamColNo - iOrdLo + 1;

    m_p_table = _pSub_Table(m_pMainCard);
    m_p_tableSyn = _pSub_TableSyn(m_p_table);
    char cTableSyn = m_p_tableSyn->cTableSyn();

    Card_Term::vTableColOrdinal(psTableColOrdinal, cTableSyn, iTableColNo);
  }
}

/*----------------------------------------------------------------------------*/
/* resolve                                                                    */
/*----------------------------------------------------------------------------*/
void Resolver::vResolve(Card* pMainCard, Stz* szTerm,
                        String* pStreamNm, String* pStreamColNm, i8& i_StreamCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESOLVER,_FOOTER_);

  pStreamNm->vCopy(""_Stz);
  pStreamColNm->vCopy(""_Stz);

  vLocateStreams(pMainCard);

  i_StreamCol = StrLib::iGleanInt(szTerm->azChar()) - 1;

  Card_StreamCol* pCard_StreamCol = nullptr;

  if (! m_pStream_r) {
    // + one stream
    pStreamNm->vCopy("S"_Stz);
    pCard_StreamCol = m_pStream_l->pGet_streamCol(i_StreamCol);
  }
  else {
    // - two streams
    i8 nStreamCols_l = m_pStream_l->nStreamCols();
    if (i_StreamCol < nStreamCols_l) {
      // + in stream a
      pStreamNm->vCopy("L"_Stz);
      pCard_StreamCol = m_pStream_l->pGet_streamCol(i_StreamCol);
    }
    else {
      // + in stream b
      pStreamNm->vCopy("R"_Stz);
      i_StreamCol -= nStreamCols_l;
      pCard_StreamCol = m_pStream_r->pGet_streamCol(i_StreamCol);
    }
  }

  RASSERT(pCard_StreamCol);
  pStreamColNm->vCopy(pCard_StreamCol->szStreamColNm());
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* locate streams for given card                                              */
/*----------------------------------------------------------------------------*/
void Resolver::vLocateStreams(Card* pMainCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_RESOLVER, _FOOTER_);

  m_pStream_l = nullptr;
  m_pStream_r = nullptr;

  bool bSaw_AbsJoin = false;

  for (i8 i_Test = pMainCard->iCardNo(); i_Test >= 0; i_Test--) {
    Card *pTestCard = m_pBoard->pGetMain(i_Test);

    if (CK_ABSJOIN == pTestCard->eCard()) {
      bSaw_AbsJoin = true;
      continue;
    }

    if (CK_SCAN == pTestCard->eCard()) {
      if (! m_pStream_l) {
        m_pStream_l = _pCard_Stream(pTestCard->pFindSubCard(CK_STREAM));
        if (! bSaw_AbsJoin)
          break;
        continue;
      }
      m_pStream_r = m_pStream_l;
      m_pStream_l = _pCard_Stream(pTestCard->pFindSubCard(CK_STREAM));
      break;
    }
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
