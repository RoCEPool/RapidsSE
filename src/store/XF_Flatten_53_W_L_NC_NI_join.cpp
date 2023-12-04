/*============================================================================*/
/* XF_Flatten_53_W_L_NC_NI_join.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_53_W_L_NC_NI_join.hpp"

#include "XF_Fold.hpp"
#include "XF_Flatten_Smooth.hpp"
#include "XF_Flatten_Smoosh.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_53_W_L_NC_NI_join, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN)
__aNewFunction(XF_Flatten_53_W_L_NC_NI_join, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN)
__vDeleteFunction(XF_Flatten_53_W_L_NC_NI_join, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN)
__aFooterFunction(XF_Flatten_53_W_L_NC_NI_join)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_53_W_L_NC_NI_join::XF_Flatten_53_W_L_NC_NI_join(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_53_W_L_NC_NI_JOIN);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_53_W_L_NC_NI_join::~XF_Flatten_53_W_L_NC_NI_join(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_join::vXF_Flatten_53_W_L_NC_NI_join(CardIter* pCI)
{
  _static_

  XF_Flatten_53_W_L_NC_NI_join* pXF_Flatten_53_W_L_NC_NI_join = new XF_Flatten_53_W_L_NC_NI_join(pCI);

  pXF_Flatten_53_W_L_NC_NI_join->vXF_Flatten_53_W_L_NC_NI_join();

  delete pXF_Flatten_53_W_L_NC_NI_join;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_53_W_L_NC_NI_join::vXF_Flatten_53_W_L_NC_NI_join(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_53_W_L_NC_NI_JOIN, _FOOTER_);

 // m_pCI->vPrintBoard(__LINE__);

  // W L NC NI
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

  m_pCI->vTransform(__func__, XF_FLATTEN_53_W_L_NC_NI_JOIN, "((XF_FLATTEN_53_W_L_NC_NI_JOIN))"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of exists subQuery


  XF_Fold* pFold = new XF_Fold(0, m_pCI->G.pDeck);

  pFold->vFold_basic(m_pCI, "W_L_NC_NI_join"_Stz, CK_o_WHERE);

 // m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smooshing

  XF_Flatten_Smoosh* pSmoosh = new XF_Flatten_Smoosh();

  pSmoosh->vInto_KEYnotEXISTS(m_pCI);
 // m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing

  m_pCI->vClearRestart();
  //RASSERT(m_pCI->G_bEachMainWithSub(CK_o_PROJECT,CK_t_EMIT));
  RASSERT(m_pCI->G_bEachMain(CK_c_FOLD));

  CarryForward* pCarryForward = new CarryForward;
  pCarryForward->Table.m_bReproject = true;

  XF_Flatten_Smooth* pSmooth = new XF_Flatten_Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

 // m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vMarksClearRecursive();

  delete pSmoosh;
  delete pSmooth;
  delete pCarryForward;
  delete pFold;

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
