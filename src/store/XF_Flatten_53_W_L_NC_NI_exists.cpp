/*============================================================================*/
/* XF_Flatten_53_W_L_NC_NI_exists.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_53_W_L_NC_NI_exists.hpp"

#include "XF_Fold.hpp"
#include "XF_Flatten_Smooth.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_53_W_L_NC_NI_exists, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS)
__aNewFunction(XF_Flatten_53_W_L_NC_NI_exists, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS)
__vDeleteFunction(XF_Flatten_53_W_L_NC_NI_exists, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS)
__aFooterFunction(XF_Flatten_53_W_L_NC_NI_exists)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_53_W_L_NC_NI_exists::XF_Flatten_53_W_L_NC_NI_exists(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_53_W_L_NC_NI_exists::~XF_Flatten_53_W_L_NC_NI_exists(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_exists::vXF_Flatten_53_W_L_NC_NI_exists(CardIter* pCI)
{
  _static_

  XF_Flatten_53_W_L_NC_NI_exists* pXF_Flatten_53_W_L_NC_NI_exists = new XF_Flatten_53_W_L_NC_NI_exists(pCI);

  pXF_Flatten_53_W_L_NC_NI_exists->vXF_Flatten_53_W_L_NC_NI_exists();

  delete pXF_Flatten_53_W_L_NC_NI_exists;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_exists::vXF_Flatten_53_W_L_NC_NI_exists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, _FOOTER_);

 // m_pCI->vPrintBoard(__LINE__);

  // W L NC NI
  // push a (linear non-correlated not in) sub-query before the main-query
  // as a keyExists
  // (refer: Q16)

  /* ARCHETYPE :
      select count(*)
        from partsupp, part
       where PS_PARTKEY = P_PARTKEY
         and PS_SUPPKEY not in (select S_SUPPKEY
                                  from supplier
                                 where s_comment like '%Customer%Complaints%')
      ;
 */

  m_pCI->vTransform(__func__, XF_FLATTEN_53_W_L_NC_NI_EXISTS, "((vFlatten_53_W_L_NC_NI_exists))"_Stz);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold

  XF_Fold* pFold = new XF_Fold(0, m_pCI->G.pDeck);

  //pFold->vFold_basic(m_pCI,"W_L_NC_NI_exists"_Stz,CK_m_WHERE);
  CarryForward* pCarryForward =
      pFold->pFold_nc_subQuery(m_pCI, "W_L_NC_NI_exists"_Stz,
                               CK_m_WHERE, QO_IN_OP, SM_NONE);

 // m_pCI->vPrintBoard(__LINE__);


  XF_Flatten_Smooth* pSmooth = new XF_Flatten_Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);
 // m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->G.pDeck->vMarksClearRecursive();

  delete pSmooth;
  delete pCarryForward;
  delete pFold;

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
