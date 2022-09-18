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
: OptimiserTool(CN_RESOLVER,pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESOLVER,_FOOTER_);

  m_pStream_l    = nullptr;
  m_pStream_r    = nullptr;

  _checkInitialised_(CN_RESOLVER);
  
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
/* resolve                                                                    */
/*----------------------------------------------------------------------------*/
void Resolver::vResolve(Card* pMainCard, Stz* szTerm,
                        String* pStreamNm, String* pStreamColNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RESOLVER,_FOOTER_);

  pStreamNm->vCopy(""_Stz);
  pStreamColNm->vCopy(""_Stz);

  vLocateStreams(pMainCard);

  i8 i_StreamColNo = StrLib::iGleanInt(szTerm->azChar()) - 1;

  Card_StreamCol* pCard_StreamCol = nullptr;

  if (! m_pStream_r) {
    // + one stream
    pStreamNm->vCopy("S"_Stz);
    pCard_StreamCol = m_pStream_l->pGet_streamCol(i_StreamColNo);
  }
  else {
    // - two streams
    i8 nStreamCols_l = m_pStream_l->nStreamCols();
    if (i_StreamColNo < nStreamCols_l) {
      // + in stream a
      pStreamNm->vCopy("L"_Stz);
      pCard_StreamCol = m_pStream_l->pGet_streamCol(i_StreamColNo);
    }
    else {
      // + in stream b
      pStreamNm->vCopy("R"_Stz);
      pCard_StreamCol = m_pStream_r->pGet_streamCol(i_StreamColNo - nStreamCols_l);
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
    Card *pTestCard = m_pBoard->pGetMainCard(i_Test);

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
