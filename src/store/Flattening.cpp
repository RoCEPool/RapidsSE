/*============================================================================*/
/* Flattening.cpp -- Flattening                                           */
/*============================================================================*/

#include "Flattening.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Fold.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Flattening, CN_FLATTENING)
__aNewFunction(Flattening, CN_FLATTENING)
__vDeleteFunction(Flattening, CN_FLATTENING)
__aFooterFunction(Flattening)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Flattening::Flattening(CardIter* pCI)
: Object(CN_FLATTENING)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FLATTENING, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_FLATTENING);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_FLATTENING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Flattening::~Flattening(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FLATTENING, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Flattening::vFlatten_10_F(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTENING,_FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

  // F
  // (refer: Q7,8,9)

  /*
      select count(*)
        from (select *
                from orders, lineitem
               where L_ORDERKEY = O_ORDERKEY)
      ;
   */

  m_pCI->vTransform(__func__, "((vFlatten_10_F))"_Stz);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of from

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  CarryForward* pCarryForward = pFold->pFold_from("From"_Stz,m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing

  pCarryForward->Table.m_bReproject = true;
  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  //Card_Projection::vProjection_Analysis_v2(m_pCI);
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);


  // todo: discuss with Alka
  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_FOLD)) {
    if (m_pCI->G_bEachMain(CK_WHERE)) {
      Card_Level* p_level_where = m_pCI->G.p_where->pSub_level();
      if (m_pCI->G_bEachMainWithSub(CK_PROJECT, CK_LEVEL)) {
        p_level_where->Y_vSet(P_Level,m_pCI->G.p_level->Y_iGet(P_Level));
      }
    }
  }

  m_pCI->vEndTransform();

  delete pSmooth;
  delete pCarryForward;
  delete pFold;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Flattening::vFlatten_24_W_A_C_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTENING,_FOOTER_);

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

  m_pCI->vTransform(__func__, "((vFlatten_24_W_A_C_ro))"_Stz);

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  pFold->vFold_basic(m_pCI,"24_W_A_C_ro"_Stz,CK_WHERE);
  m_pCI->vPrintBoard(__LINE__);

  pFold->vDerive_Correleted_fold_projection(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  Smoosh* pSmoosh = new Smoosh;
  pSmoosh->vInto_GBYAGG(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vClearMarksRecursive();
  m_pCI->vPrintBoard(__LINE__);
  m_pCI->vEndTransform();

  delete pSmoosh;
  delete pFold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Flattening::vFlatten_32_W_A_NC_I(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTENING,_FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

  // W A NC I
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q18)

  /* ARCHETYPE :
      select count(*)
        from orders, lineitem
       where O_ORDERKEY = L_ORDERKEY
         and O_ORDERKEY in (select L_ORDERKEY
                              from lineitem
                             group by L_ORDERKEY
                            having sum(l_quantity) > 100)
      ;
   */

  m_pCI->vTransform(__func__, "((vFlatten_32_W_A_NC_I))"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of subQuery

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  CarryForward* pCarryForward =
      pFold->pFold_nc_subQuery(m_pCI, "W_A_NC_I"_Stz,
                               CK_WHERE, QO_IN_OP, SM_NONE);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing of: subQuery | Fold | Table* [Where] GbyAgg_PNO [Project (avg)] Having

  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Flattening::vFlatten_34_W_A_NC_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTENING,_FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

  // W A NC ro
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q15)

  /*
   */

  m_pCI->vTransform(__func__, "((vFlatten_34_W_A_NC_ro))"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of subQuery

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  CarryForward* pCarryForward =
      pFold->pFold_nc_subQuery(m_pCI, "W_A_NC_ro"_Stz,
                               CK_WHERE, QO_RELN_OP, SM_NONE);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing W_A_NC_ro

  // Fold | Table* Where GbyAgg_P GbyAgg_N GbyAgg_O

  m_pCI->vRestart();
  RASSERT(m_pCI->G_bEachMainWithSub(CK_PROJECT,CK_PROJECTION));
  RASSERT(m_pCI->G_bEachMain(CK_FOLD));

  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();

  delete pSmooth;
  delete pCarryForward;
  delete pFold;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Flattening::vFlatten_40_W_L_C_E(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTENING,_FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

  /* ARCHETYPE :
      select count(*)
        from orders
       where exists (select *
                       from lineitem
                      where L_ORDERKEY = O_ORDERKEY
                        and l_commitdate < l_receiptdate)
      ;
   */

  m_pCI->vTransform(__func__, "((vFlatten_40_W_L_C_E))"_Stz);

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  pFold->vFold_basic(m_pCI,"40_W_L_C_E"_Stz,CK_WHERE);
  m_pCI->vPrintBoard(__LINE__);

  Smoosh* pSmoosh = new Smoosh;
  pSmoosh->vInto_KEYnotEXISTS(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();

  delete pSmoosh;
  delete pFold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Flattening::vFlatten_52_W_L_NC_I_exists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FLATTENING, _FOOTER_);

  vFlatten_53_W_L_NC_NI_exists();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Flattening::vFlatten_52_W_L_NC_I_join(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FLATTENING, _FOOTER_);

  vFlatten_53_W_L_NC_NI_join();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Flattening::vFlatten_53_W_L_NC_NI_exists(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTENING,_FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

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

  m_pCI->vTransform(__func__, "((vFlatten_53_W_L_NC_NI_exists))"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of exists subQuery

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  CarryForward* pCarryForward = pFold->pFold_exists("W_L_NC_NI"_Stz,m_pCI);

  m_pCI->vPrintBoard(__LINE__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    m_pCI->G.p_where->Y_vSet(P_Smoothed);
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
    if (0 == m_pCI->G.p_boolCalc->nChildCards())
      m_pCI->G_vRemoveDeleteMain(CK_WHERE);
  }
  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();

  delete pCarryForward;
  delete pFold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Flattening::vFlatten_53_W_L_NC_NI_join(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTENING,_FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

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

  m_pCI->vTransform(__func__, "((vFlatten_53_W_L_NC_NI_exists))"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of exists subQuery


  Fold* pFold = new Fold(0,m_pCI->G.pDeck);

  pFold->vFold_basic(m_pCI,"W_L_NC_NI_join"_Stz,CK_WHERE);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smooshing

  Smoosh* pSmoosh = new Smoosh();

  pSmoosh->vInto_KEYnotEXISTS(m_pCI);
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing

  m_pCI->G_vRestart();
  //RASSERT(m_pCI->G_bEachMainWithSub(CK_PROJECT,CK_PROJECTION));
  RASSERT(m_pCI->G_bEachMain(CK_FOLD));

  CarryForward* pCarryForward = new CarryForward;
  pCarryForward->Table.m_bReproject = true;

  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G.pDeck->vClearMarksRecursive();

  m_pCI->vEndTransform();

  delete pSmooth;
  delete pCarryForward;
  delete pFold;
}

/*----------------------------------------------------------------------------*/
/* Folding a non-correlated aggregate subquery (via having clause)            */
/*----------------------------------------------------------------------------*/
void Flattening::vFlatten_74_H_A_NC_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FLATTENING,_FOOTER_);

  m_pCI->vPrintBoard(__LINE__);

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

  m_pCI->vTransform(__func__, "((vFlatten_74_H_A_NC_ro))"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fold of subQuery

  Fold* pFold = new Fold(0,m_pCI->G.pDeck);
  CarryForward* pCarryForward =
      pFold->pFold_nc_subQuery(m_pCI, "H_A_NC_ro"_Stz,
                               CK_HAVING, QO_RELN_OP, SM_NONE);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // smoothing of: subQuery | Fold | Table* [Where] GbyAgg_PNO [Project (avg)] Having

  Smooth* pSmooth = new Smooth;
  pSmooth->vSmooth(m_pCI,pCarryForward);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->G.pDeck->vClearMarksRecursive();

  delete pFold;
  delete pCarryForward;
  delete pSmooth;

  m_pCI->vEndTransform();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
