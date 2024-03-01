/*============================================================================*/
/* XF_Flatten_74_H_A_NC_ro.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Flatten_74_H_A_NC_ro.hpp"

#include "XF_Fold.hpp"
#include "XF_Flatten_Smooth.hpp"
#include "XF_Flatten_Left_subQuery.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_74_H_A_NC_ro, CN_XF_FLATTEN_74_H_A_NC_RO)
__aNewFunction(XF_Flatten_74_H_A_NC_ro, CN_XF_FLATTEN_74_H_A_NC_RO)
__vDeleteFunction(XF_Flatten_74_H_A_NC_ro, CN_XF_FLATTEN_74_H_A_NC_RO)
__aFooterFunction(XF_Flatten_74_H_A_NC_ro)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_74_H_A_NC_ro::XF_Flatten_74_H_A_NC_ro(CardIter* pCI)
: XF_TRANSFORM(CN_XF_FLATTEN_74_H_A_NC_RO, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_74_H_A_NC_RO, _FOOTER_);

  _checkInitialised_(CN_XF_FLATTEN_74_H_A_NC_RO);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_74_H_A_NC_RO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_74_H_A_NC_ro::~XF_Flatten_74_H_A_NC_ro(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_74_H_A_NC_RO, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_74_H_A_NC_ro::vXF_Flatten_74_H_A_NC_ro(CardIter* pCI)
{
  _static_

  XF_Flatten_74_H_A_NC_ro* pXF_Flatten_74_H_A_NC_ro = new XF_Flatten_74_H_A_NC_ro(pCI);

  pXF_Flatten_74_H_A_NC_ro->vXF_Flatten_74_H_A_NC_ro();

  delete pXF_Flatten_74_H_A_NC_ro;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add level to op                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_74_H_A_NC_ro::vXF_Flatten_74_H_A_NC_ro(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_74_H_A_NC_RO, _FOOTER_);

 // m_pCI->vPrintBoard(__FILE__,__func__,__LINE__);

  // H A NC ro (refer: Q11)
  // - the nc subquery produces a single nc result, such as avg(col)
  // - that SQ result needs to be in every OQ row so the Having clause
  //     can use it to filter rows

  /* ARCHETYPE _r_a_b_HAVING:
      select l_orderkey, sum(l_quantity)
        from lineitem
       group by l_orderkey
      having sum(l_quantity) > (select avg(l_quantity)
                                  from lineitem);
  */

  m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_74_H_A_NC_RO);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of subQuery

  XF_Fold* pFold = new XF_Fold(0, m_pCI->G.pDeck);
  CarryForward* pCarryForward =
      pFold->pFold_nc_subQuery(m_pCI, "H_A_NC_ro"_Stz,
                               CK_o_HAVING, QO_RELN_OP, SM_NONE);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing of: subQuery | XF_Fold | Table* [Where] GbyAgg_PNO [Project (avg)] Having [Sort]

  XF_Flatten_Left_subQuery::vRotate_Next(m_pCI);

  m_pCI->vPrintBoard(__FILE__,__func__,__LINE__);

  XF_Flatten_Smooth* pSmooth = new XF_Flatten_Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->G.pDeck->vMarksClearRecursive();

  delete pFold;
  delete pCarryForward;
  delete pSmooth;

  m_pCI->vEndTransform(__LINE__);
//vValidateBench();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
