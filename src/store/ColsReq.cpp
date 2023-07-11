/*============================================================================*/
/* ColsReq.cpp -- Columns Required Metadata                               */
/*============================================================================*/

#include "ColsReq.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ColsReq,CN_COLSREQ)
__aNewFunction(ColsReq,CN_COLSREQ)
__vDeleteFunction(ColsReq,CN_COLSREQ)
__aFooterFunction(ColsReq)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ColsReq::ColsReq(void)
    : Object(CN_COLSREQ)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ, _FOOTER_);

  for (i8 i_Col = 0; i_Col < C_COLSREQ; i_Col++) {
    m_axCols[i_Col].m_i_EmittedCol = C_UNKNOWN;
    m_axCols[i_Col].m_i_Cell = C_UNKNOWN;
    m_axCols[i_Col].m_bPayload = false;
  }
  m_nCols = 0;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLSREQ,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ColsReq::~ColsReq(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLSREQ,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void ColsReq::vAddCol(i8 i_Col_base1, i8 i_Col2_base1, Stz *szColNm, bool bPayLoad)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ, _FOOTER_);

  m_axCols[m_nCols].m_i_EmittedCol = i_Col_base1;
  m_axCols[m_nCols].m_i_Cell = i_Col2_base1;
  m_axCols[m_nCols].m_sColNm      = szColNm;
  m_axCols[m_nCols].m_bPayload    = bPayLoad;

  m_nCols++;

  vValidate();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void ColsReq::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ, _FOOTER_);

  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    vTrace("               Col [%lld]: %lld %lld %s %s\n"_Stz,
           i_Col,
           m_axCols[i_Col].m_i_EmittedCol,
           m_axCols[i_Col].m_i_Cell,
           m_axCols[i_Col].m_sColNm.szStz(),
           m_axCols[i_Col].m_bPayload ? "payload" : "key");
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void ColsReq::vValidate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLSREQ, _FOOTER_);

  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    if (! StrLib::bIsSqlName(m_axCols[i_Col].m_sColNm.azChar())) {
      vDump();
      break;
    }
  }

  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    RASSERT(StrLib::bIsSqlName(m_axCols[i_Col].m_sColNm.azChar()));
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
