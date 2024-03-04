/*============================================================================*/
/* XF_Flatten_24_W_A_C_ro.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_24_W_A_C_ro.hpp"

#include "XF_Flatten_Smooth.hpp"
#include "CorrelatedJoin.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_24_W_A_C_ro, CN_XF_FLATTEN_24_W_A_C_RO)
__aNewFunction(XF_Flatten_24_W_A_C_ro, CN_XF_FLATTEN_24_W_A_C_RO)
__vDeleteFunction(XF_Flatten_24_W_A_C_ro, CN_XF_FLATTEN_24_W_A_C_RO)
__aFooterFunction(XF_Flatten_24_W_A_C_ro)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_24_W_A_C_ro::XF_Flatten_24_W_A_C_ro(CardIter* pCI)
: XF_FLATTEN(CN_XF_FLATTEN_24_W_A_C_RO, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_24_W_A_C_RO);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_24_W_A_C_ro::~XF_Flatten_24_W_A_C_ro(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vFlatten(CardIter* pCI)
{
  _static_

  XF_Flatten_24_W_A_C_ro* pXF_Flatten_24_W_A_C_ro = new XF_Flatten_24_W_A_C_ro(pCI);

  pXF_Flatten_24_W_A_C_ro->vFlatten();

  delete pXF_Flatten_24_W_A_C_ro;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vFlatten(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

 // m_pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // W A C ro

  /* ARCHETYPE :
    select count(*)
      from Lineitem, Part, PartSupp
     where P_PARTKEY  = L_PARTKEY
       and PS_PARTKEY = P_PARTKEY
       and l_quantity < (select avg(l_quantity)
                           from lineitem
                          where L_PARTKEY = P_PARTKEY)
    ;
 */

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_24_W_A_C_RO);

  vFold_basic(m_pCI, "24_W_A_C_ro"_Stz, CK_o_WHERE);

  CorrelatedJoin* pCorrelatedJoin = new CorrelatedJoin;
  pCorrelatedJoin->vVia_gbyAgg_Promotion(m_pCI);

  vDerive_Correlated_emit_fold(m_pCI);

  m_pCI->G.pDeck->vMarksClearRecursive();

  delete pCorrelatedJoin;

  m_pCI->vEndTransform(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* Derive correlated fold projection                                          */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vDerive_Correlated_emit_fold(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  pCI->vBeginEach();

  while (pCI->bEachMain_NONE()) {

    if (CK_c_FOLD == pCI->G.e_MainCard)
      break;

    if (Card_Emit* p_emit = pCI->G.p_MainCard->pSub_emit()) {
      //pCI->vPrintBoard(__func__,__FILE__,__LINE__);
      pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = p_emit;
    }
  }

  RASSERT(CK_c_FOLD == pCI->G.e_MainCard);

  pCI->G.p_stream = new Card_Stream;
  pCI->G.p_fold->vAppendChild(pCI->G.p_stream);
  pCI->G.p_stream->vAppendChild(pCI->Flux.m_pArriving->pDupCardRecursive());
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
