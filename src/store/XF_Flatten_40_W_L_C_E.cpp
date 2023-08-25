/*============================================================================*/
/* XF_Flatten_40_W_L_C_E.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_40_W_L_C_E.hpp"

#include "Fold.hpp"
#include "XF_Flatten_Smooth.hpp"
#include "XF_Flatten_Smoosh.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_40_W_L_C_E, CN_XF_FLATTEN_40_W_L_C_E)
__aNewFunction(XF_Flatten_40_W_L_C_E, CN_XF_FLATTEN_40_W_L_C_E)
__vDeleteFunction(XF_Flatten_40_W_L_C_E, CN_XF_FLATTEN_40_W_L_C_E)
__aFooterFunction(XF_Flatten_40_W_L_C_E)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_40_W_L_C_E::XF_Flatten_40_W_L_C_E(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_40_W_L_C_E, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_40_W_L_C_E, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_40_W_L_C_E);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_40_W_L_C_E, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_40_W_L_C_E::~XF_Flatten_40_W_L_C_E(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_40_W_L_C_E, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_40_W_L_C_E::vXF_Flatten_40_W_L_C_E(CardIter* pCI)
{
  _static_

  XF_Flatten_40_W_L_C_E* pXF_Flatten_40_W_L_C_E = new XF_Flatten_40_W_L_C_E(pCI);

  pXF_Flatten_40_W_L_C_E->vXF_Flatten_40_W_L_C_E();

  delete pXF_Flatten_40_W_L_C_E;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_40_W_L_C_E::vXF_Flatten_40_W_L_C_E(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_40_W_L_C_E, _FOOTER_);

 //// m_pCI->vPrintBoard(__LINE__);

  /* ARCHETYPE :
      select count(*)
        from orders
       where exists (select *
                       from lineitem
                      where L_ORDERKEY = O_ORDERKEY
                        and l_commitdate < l_receiptdate)
      ;
   */

  m_pCI->vTransform(__func__, XF_FLATTEN_40_W_L_C_E, "((vFlatten_40_W_L_C_E))"_Stz);

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  pFold->vFold_basic(m_pCI,"40_W_L_C_E"_Stz,CK_WHERE);
 // m_pCI->vPrintBoard(__LINE__);

  XF_Flatten_Smoosh* pSmoosh = new XF_Flatten_Smoosh;
  pSmoosh->vInto_KEYnotEXISTS(m_pCI);
 // m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vMarksClearRecursive();

  delete pSmoosh;
  delete pFold;

  m_pCI->vEndTransform();
  vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
