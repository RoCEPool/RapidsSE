/*============================================================================*/
/* XF_Flatten_53_W_L_NC_NI_exists.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_53_W_L_NC_NI_exists.hpp"


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
: XF_FLATTEN(CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, pCI)
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
void XF_Flatten_53_W_L_NC_NI_exists::v53_W_L_NC_NI_exists(CardIter* pCI)
{
  _static_

  XF_Flatten_53_W_L_NC_NI_exists* pXF_Flatten_53_W_L_NC_NI_exists = new XF_Flatten_53_W_L_NC_NI_exists(pCI);

  pXF_Flatten_53_W_L_NC_NI_exists->vFlatten();

  delete pXF_Flatten_53_W_L_NC_NI_exists;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_exists::vFlatten(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS, _FOOTER_);

 // m_pCI->PRINTBOARD_dbg();

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

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_53_W_L_NC_NI_EXISTS);
  //m_pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vFold_subQuery(m_pCI, "W_L_NC_NI_exists"_Stz, CK_o_WHERE);
  vSet_smooth(m_pCI, CK_o_WHERE, QO_IN_OP);
  vSmooth(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.p_deck->vMarksClearRecursive();

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
