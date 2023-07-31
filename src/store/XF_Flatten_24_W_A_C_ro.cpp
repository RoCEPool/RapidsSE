/*============================================================================*/
/* XF_Flatten_24_W_A_C_ro.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_24_W_A_C_ro.hpp"

#include "Fold.hpp"
#include "XF_Smooth.hpp"
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
: XF_TRANSFORM(CN_XF_FLATTEN_24_W_A_C_RO, pCI)
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
void XF_Flatten_24_W_A_C_ro::vXF_Flatten_24_W_A_C_ro(CardIter* pCI)
{
  _static_

  XF_Flatten_24_W_A_C_ro* pXF_Flatten_24_W_A_C_ro = new XF_Flatten_24_W_A_C_ro(pCI);

  pXF_Flatten_24_W_A_C_ro->vXF_Flatten_24_W_A_C_ro();

  delete pXF_Flatten_24_W_A_C_ro;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_24_W_A_C_ro::vXF_Flatten_24_W_A_C_ro(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_24_W_A_C_RO, _FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

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

  m_pCI->vTransform(__func__, XF_FLATTEN_24_W_A_C_RO, "((vFlatten_24_W_A_C_ro))"_Stz);

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  pFold->vFold_basic(m_pCI,"24_W_A_C_ro"_Stz,CK_WHERE);
  m_pCI->vPrintBoard(__LINE__);

  CorrelatedJoin* pCorrelatedJoin = new CorrelatedJoin;
  pCorrelatedJoin->vVia_gbyAgg_Promotion(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  pFold->vDerive_Correleted_fold_projection(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vMarksClearRecursive();
  m_pCI->vPrintBoard(__LINE__);
  m_pCI->vEndTransform();

  delete pCorrelatedJoin;
  delete pFold;

  vValidate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
