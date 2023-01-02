/*============================================================================*/
/* CardIter.cpp --                                                            */
/*============================================================================*/

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CardIter,CN_CARDITER)
__aNewFunction(CardIter,CN_CARDITER)
__vDeleteFunction(CardIter,CN_CARDITER)
__aFooterFunction(CardIter)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(void)
    : Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = nullptr;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bDontDescend = false;

  G.m_bActive = false;

  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(eCN eClass)
    : Object(eClass)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = nullptr;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bDontDescend = false;

  G.m_bActive = false;

  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(Card*  pCard)
: Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = pCard;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bActive = true;

  _checkInitialised_(CN_CARDITER);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(CardIter* pCardIter_src)
    : Object(CN_CARDITER)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vSet(pCardIter_src);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardIter::CardIter(eCN eClass, Card*  pCard)
    : Object(eClass)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = pCard;
  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;

  G.m_bActive = true;

  _checkInitialised_(CN_CARDITER);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardIter::~CardIter(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vChangeCard(Card* pCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.m_pCard = pCard;

  vRestart();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardIter::i_Child(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return G.m_i_Child;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/* clear the M members                                                        */
/*----------------------------------------------------------------------------*/
void CardIter::G_vClearMembers(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  G.p_absJoin          = nullptr;
  G.p_aggElist         = nullptr;
  G.p_aggreg           = nullptr;
  G.p_as               = nullptr;
  G.p_bolPred          = nullptr;
  G.p_boolCalc         = nullptr;
  G.p_bound            = nullptr;
  G.p_bounds           = nullptr;
  G.p_calcOp           = nullptr;
  G.p_call             = nullptr;
  G.p_cast             = nullptr;
  G.p_clause           = nullptr;
  G.p_cmpPred          = nullptr;
  G.p_combined         = nullptr;
  G.p_comment          = nullptr;
  G.p_constraint       = nullptr;
  G.p_coPtnJoin        = nullptr;
  G.p_coPtnJoinList    = nullptr;
  G.p_coPtnPred        = nullptr;
  G.p_coPtnPredList    = nullptr;
  G.p_crossJoin        = nullptr;
  G.p_crossPtnJoin     = nullptr;
  G.p_crossPtnJoinList = nullptr;
  G.p_direction        = nullptr;
  G.p_eBound           = nullptr;
  G.p_expr             = nullptr;
  G.p_filter           = nullptr;
  G.p_fold             = nullptr;
  G.p_foldElist        = nullptr;
  G.p_gbyAgg           = nullptr;
  G.p_gbyElist         = nullptr;
  G.p_genesis          = nullptr;
  G.p_group            = nullptr;
  G.p_hash             = nullptr;
  G.p_having           = nullptr;
  G.p_idxCol           = nullptr;
  G.p_idxColList       = nullptr;
  G.p_idxColNm         = nullptr;
  G.p_index            = nullptr;
  G.p_indexJoin        = nullptr;
  G.p_indexList        = nullptr;
  G.p_indexNm          = nullptr;
  G.p_indexScan        = nullptr;
  G.p_joinOrder        = nullptr;
  G.p_joinOrdering     = nullptr;
  G.p_joinPredList     = nullptr;
  G.p_keyExists        = nullptr;
  G.p_keyNotExists     = nullptr;
  G.p_lBound           = nullptr;
  G.p_level            = nullptr;
  G.p_limit            = nullptr;
  G.p_manner           = nullptr;
  G.p_nCorrCols        = nullptr;
  G.p_next             = nullptr;
  G.p_note             = nullptr;
  G.p_notPred          = nullptr;
  G.p_nParmCols        = nullptr;
  G.p_on               = nullptr;
  G.p_ordAgg           = nullptr;
  G.p_ordElmOdn        = nullptr;
  G.p_order            = nullptr;
  G.p_param            = nullptr;
  G.p_paramList        = nullptr;
  G.p_partition        = nullptr;
  G.p_permutation      = nullptr;
  G.p_permutation_low  = nullptr;
  G.p_pmzWhere         = nullptr;
  G.p_project          = nullptr;
  G.p_projElist        = nullptr;
  G.p_ptnCol           = nullptr;
  G.p_ptnColList       = nullptr;
  G.p_ptnColNm         = nullptr;
  G.p_ptndJoin         = nullptr;
  G.p_ptndJoinSet      = nullptr;
  G.p_reason           = nullptr;
  G.p_relnOp           = nullptr;
  G.p_rpnCalc          = nullptr;
  G.p_sbyElist         = nullptr;
  G.p_scan             = nullptr;
  G.p_send             = nullptr;
  G.p_size             = nullptr;
  G.p_sort             = nullptr;
  G.p_stage            = nullptr;
  G.p_stmCol           = nullptr;
  G.p_stmColList       = nullptr;
  G.p_stmColNm         = nullptr;
  G.p_stmColOdn        = nullptr;
  G.p_stream           = nullptr;
  G.p_subSet           = nullptr;
  G.p_table            = nullptr;
  G.p_tableList        = nullptr;
  G.p_tableNm          = nullptr;
  G.p_tablePtn         = nullptr;
  G.p_tblColOdn        = nullptr;
  G.p_term             = nullptr;
  G.p_topology         = nullptr;
  G.p_roPred           = nullptr;
  G.p_twist            = nullptr;
  G.p_type             = nullptr;
  G.p_uBound           = nullptr;
  G.p_unique           = nullptr;
  G.p_where            = nullptr;
  G.p_xBound           = nullptr;

  G.p_av_aggP          = nullptr;
  G.p_av_aggPN         = nullptr;
  G.p_av_aggPNO        = nullptr;
  G.p_av_aggregP       = nullptr;
  G.p_av_aggregPN      = nullptr;
  G.p_av_aggregPNO     = nullptr;
  G.p_av_filter        = nullptr;
  G.p_av_filterProject = nullptr;
  G.p_av_gbyAggP       = nullptr;
  G.p_av_gbyAggPN      = nullptr;
  G.p_av_gbyPNO        = nullptr;
  G.p_av_joinSeries    = nullptr;
  G.p_av_limit         = nullptr;
  G.p_av_project       = nullptr;
  G.p_av_scanJoinSeries = nullptr;
  G.p_av_scanOnly      = nullptr;
  G.p_av_sort          = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vDontDescend(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.m_bDontDescend = true;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bEachChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (pEachChild())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  i8 i_Main;

  G.m_pChild = pEachChild(i_Main);

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(i8& ri_Main)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = 0;
  else
    G.m_i_Child++;

  if (G.m_i_Child >= G.m_pCard->nChildCards()) {
    G.m_pChild = nullptr;
    ri_Main  = C_UNKNOWN;
    G.m_bActive = false;

    return nullptr;
  }

  G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);
  ri_Main = G.m_i_Child;

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  i8 i_Main;

  Card* pMain = pEachChild(eCard, i_Main);

  return pMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(eCK eCard, i8& ri_Main)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = 0;
  else
    G.m_i_Child++;

  for (;;) {

    if (G.m_i_Child >= G.m_pCard->nChildCards()) {
      G.m_pChild  = nullptr;
      ri_Main   = C_UNKNOWN;
      G.m_bActive = false;
      return nullptr;
    }

    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

    eCK eCardMain = G.m_pChild->eCard();
    if (eCardMain == eCard) {
      ri_Main = G.m_i_Child;
      return G.m_pChild;
    }
    G.m_i_Child++;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild(i8& ri_Main, eCK& reCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = 0;
  else
    G.m_i_Child++;

  if (G.m_i_Child >= G.m_pCard->nChildCards()) {
    G.m_pChild  = nullptr;
    ri_Main   = C_UNKNOWN;
    reCard    = CK_NONE;
    G.m_bActive = false;
    return nullptr;
  }

  G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

  reCard = G.m_pChild->eCard();
  ri_Main = G.m_i_Child;

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pEachChild_reverse(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_i_Child = G.m_pCard->nChildCards() - 1;
  else
    G.m_i_Child--;

  for (;;) {

    if (G.m_i_Child < 0) {
      G.m_pChild = nullptr;
      return nullptr;
    }

    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);

    eCK eCardMain = G.m_pChild->eCard();
    if (eCardMain == eCard) {
      return G.m_pChild;
    }

    G.m_i_Child--;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vForward(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  Card* pChild = pNextChild();

  UNUSED(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pGetChild(i8 i_Child)
{
  _precondition_(_MAGIC_, _FOOTER_);

  Card* pChild = G.m_pCard->pGetChild(i_Child);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/* fetch M member by decoding given eMember                                   */
/*----------------------------------------------------------------------------*/
Card* CardIter::G_pGetMember(eCK eMember)
{
  _precondition_(_MAGIC_, _FOOTER_);

  switch (eMember) {

    case CK_ABSJOIN:          return G.p_absJoin;
    case CK_AGGELIST:         return G.p_aggElist;
    case CK_AGGREG:           return G.p_aggreg;
    case CK_AS:               return G.p_as;
    case CK_BOLPRED:          return G.p_bolPred;
    case CK_BOOLCALC:         return G.p_boolCalc;
    case CK_BOUND:            return G.p_bound;
    case CK_BOUNDS:           return G.p_bounds;
    case CK_CALCOP:           return G.p_calcOp;
    case CK_CALL:             return G.p_call;
    case CK_CAST:             return G.p_cast;
    case CK_CLAUSE:           return G.p_clause;
    case CK_CMPPRED:          return G.p_cmpPred;
    case CK_COMBINED:         return G.p_combined;
    case CK_COMMENT:          return G.p_comment;
    case CK_CONSTRAINT:       return G.p_constraint;
    case CK_COPTNJOIN:        return G.p_coPtnJoin;
    case CK_COPTNJOINLIST:    return G.p_coPtnJoinList;
    case CK_COPTNPRED:        return G.p_coPtnPred;
    case CK_COPTNPREDLIST:    return G.p_coPtnPredList;
    case CK_CROSSJOIN:        return G.p_crossJoin;
    case CK_CROSSPTNJOIN:     return G.p_crossPtnJoin;
    case CK_CROSSPTNJOINLIST: return G.p_crossPtnJoinList;
    case CK_DIRECTION:        return G.p_direction;
    case CK_EBOUND:           return G.p_eBound;
    case CK_EXPR:             return G.p_expr;
    case CK_FILTER:           return G.p_filter;
    case CK_FOLD:             return G.p_fold;
    case CK_FOLDELIST:        return G.p_foldElist;
    case CK_GBYAGG:           return G.p_gbyAgg;
    case CK_GBYELIST:         return G.p_gbyElist;
    case CK_GENESIS:          return G.p_genesis;
    case CK_GROUP:            return G.p_group;
    case CK_HASH:             return G.p_hash;
    case CK_HAVING:           return G.p_having;
    case CK_IDXCOL:           return G.p_idxCol;
    case CK_IDXCOLLIST:       return G.p_idxColList;
    case CK_IDXCOLNM:         return G.p_idxColNm;
    case CK_INDEX:            return G.p_index;
    case CK_INDEXJOIN:        return G.p_indexJoin;
    case CK_INDEXLIST:        return G.p_indexList;
    case CK_INDEXNM:          return G.p_indexNm;
    case CK_INDEXSCAN:        return G.p_indexScan;
    case CK_JOINORDER:        return G.p_joinOrder;
    case CK_JOINORDERING:     return G.p_joinOrdering;
    case CK_JOINPREDLIST:     return G.p_joinPredList;
    case CK_KEYEXISTS:        return G.p_keyExists;
    case CK_KEYNOTEXISTS:     return G.p_keyNotExists;
    case CK_LBOUND:           return G.p_lBound;
    case CK_LEVEL:            return G.p_level;
    case CK_LIMIT:            return G.p_limit;
    case CK_MANNER:           return G.p_manner;
    case CK_NCORRCOLS:        return G.p_nCorrCols;
    case CK_NEXT:             return G.p_next;
    case CK_NOTE:             return G.p_note;
    case CK_NOTPRED:          return G.p_notPred;
    case CK_NPARMCOLS:        return G.p_nParmCols;
    case CK_ON:               return G.p_on;
    case CK_ORDAGG:           return G.p_ordAgg;
    case CK_ORDELMODN:        return G.p_ordElmOdn;
    case CK_ORDER:            return G.p_order;
    case CK_PARAM:            return G.p_param;
    case CK_PARAMLIST:        return G.p_paramList;
    case CK_PARTITION:        return G.p_partition;
    case CK_PERMUTATION:      return G.p_permutation;
    case CK_PMZWHERE:         return G.p_pmzWhere;
    case CK_PROJECT:          return G.p_project;
    case CK_PROJELIST:        return G.p_projElist;
    case CK_PTNCOL:           return G.p_ptnCol;
    case CK_PTNCOLLIST:       return G.p_ptnColList;
    case CK_PTNCOLNM:         return G.p_ptnColNm;
    case CK_PTNDJOIN:         return G.p_ptndJoin;
    case CK_PTNDJOINSET:      return G.p_ptndJoinSet;
    case CK_REASON:           return G.p_reason;
    case CK_RELNOP:           return G.p_relnOp;
    case CK_RPNCALC:          return G.p_rpnCalc;
    case CK_SBYELIST:         return G.p_sbyElist;
    case CK_SCAN:             return G.p_scan;
    case CK_SEND:             return G.p_send;
    case CK_SIZE:             return G.p_size;
    case CK_SORT:             return G.p_sort;
    case CK_STAGE:            return G.p_stage;
    case CK_STMCOL:           return G.p_stmCol;
    case CK_STMCOLLIST:       return G.p_stmColList;
    case CK_STMCOLNM:         return G.p_stmColNm;
    case CK_STMCOLODN:        return G.p_stmColOdn;
    case CK_STREAM:           return G.p_stream;
    case CK_SUBSET:           return G.p_subSet;
    case CK_TABLE:            return G.p_table;
    case CK_TABLELIST:        return G.p_tableList;
    case CK_TABLENM:          return G.p_tableNm;
    case CK_TABLEPTN:         return G.p_tablePtn;
    case CK_TBLCOLODN:        return G.p_tblColOdn;
    case CK_TERM:             return G.p_term;
    case CK_TOPOLOGY:         return G.p_topology;
    case CK_ROPRED:           return G.p_roPred;
    case CK_TWIST:            return G.p_twist;
    case CK_TYPE:             return G.p_type;
    case CK_UBOUND:           return G.p_uBound;
    case CK_UNIQUE:           return G.p_unique;
    case CK_WHERE:            return G.p_where;
    case CK_XBOUND:           return G.p_xBound;

    case CK_AV_AGGP:          return G.p_av_aggP;
    case CK_AV_AGGPN:         return G.p_av_aggPN;
    case CK_AV_AGGPNO:        return G.p_av_aggPNO;
    case CK_AV_AGGREGP:       return G.p_av_aggregP;
    case CK_AV_AGGREGPN:      return G.p_av_aggregPN;
    case CK_AV_AGGREGPNO:     return G.p_av_aggregPNO;
    case CK_AV_FILTER:        return G.p_av_filter;
    case CK_AV_FILTERPROJECT: return G.p_av_filterProject;
    case CK_AV_GBYAGGP:       return G.p_av_gbyAggP;
    case CK_AV_GBYAGGPN:      return G.p_av_gbyAggPN;
    case CK_AV_GBYPNO:        return G.p_av_gbyPNO;
    case CK_AV_JOINSERIES:    return G.p_av_joinSeries;
    case CK_AV_LIMIT:         return G.p_av_limit;
    case CK_AV_PROJECT:       return G.p_av_project;
    case CK_AV_SCANJOINSERIES: return G.p_av_scanJoinSeries;
    case CK_AV_SCANONLY:      return G.p_av_scanOnly;
    case CK_AV_SORT:          return G.p_av_sort;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bJustRestarted(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  if (C_UNKNOWN == G.m_i_Child &&
      nullptr == G.m_pChild &&
      true == G.m_bActive)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bLess(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (G.m_i_Child - 1 >= 0)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* update member card                                                         */
/*----------------------------------------------------------------------------*/
Stz* CardIter::szMemberNm(eCK eMember)
{
  _precondition_( _MAGIC_, _FOOTER_);

  switch (eMember) {

    case CK_ABSJOIN:          return "G.p_absJoin"_Stz;
    case CK_AGGELIST:         return "G.p_aggElist"_Stz;
    case CK_AGGREG:           return "G.p_aggreg"_Stz;
    case CK_AS:               return "G.p_as"_Stz;
    case CK_BOLPRED:          return "G.p_bolPred"_Stz;
    case CK_BOOLCALC:         return "G.p_boolCalc"_Stz;
    case CK_BOUND:            return "G.p_bound"_Stz;
    case CK_BOUNDS:           return "G.p_bounds"_Stz;
    case CK_CALCOP:           return "G.p_calcOp"_Stz;
    case CK_CALL:             return "G.p_call"_Stz;
    case CK_CAST:             return "G.p_cast"_Stz;
    case CK_CLAUSE:           return "G.p_clause"_Stz;
    case CK_CMPPRED:          return "G.p_cmpPred"_Stz;
    case CK_COMBINED:         return "G.p_combined"_Stz;
    case CK_COMMENT:          return "G.p_comment"_Stz;
    case CK_CONSTRAINT:       return "G.p_constraint"_Stz;
    case CK_COPTNJOIN:        return "G.p_coPtnJoin"_Stz;
    case CK_COPTNJOINLIST:    return "G.p_coPtnJoinList"_Stz;
    case CK_COPTNPRED:        return "G.p_coPtnPred"_Stz;
    case CK_COPTNPREDLIST:    return "G.p_coPtnPredList"_Stz;
    case CK_CROSSJOIN:        return "G.p_crossJoin"_Stz;
    case CK_CROSSPTNJOIN:     return "G.p_crossPtnJoin"_Stz;
    case CK_CROSSPTNJOINLIST: return "G.p_crossPtnJoinList"_Stz;
    case CK_DIRECTION:        return "G.p_direction"_Stz;
    case CK_EBOUND:           return "G.p_eBound"_Stz;
    case CK_EXPR:             return "G.p_expr"_Stz;
    case CK_FILTER:           return "G.p_filter"_Stz;
    case CK_FOLD:             return "G.p_fold"_Stz;
    case CK_FOLDELIST:        return "G.p_foldElist"_Stz;
    case CK_GBYAGG:           return "G.p_gbyAgg"_Stz;
    case CK_GBYELIST:         return "G.p_gbyElist"_Stz;
    case CK_GENESIS:          return "G.p_genesis"_Stz;
    case CK_GROUP:            return "G.p_group"_Stz;
    case CK_HASH:             return "G.p_hash"_Stz;
    case CK_HAVING:           return "G.p_having"_Stz;
    case CK_IDXCOLLIST:       return "G.p_idxColList"_Stz;
    case CK_IDXCOLNM:         return "G.p_idxColNm"_Stz;
    case CK_INDEX:            return "G.p_index"_Stz;
    case CK_INDEXJOIN:        return "G.p_indexJoin"_Stz;
    case CK_INDEXLIST:        return "G.p_indexList"_Stz;
    case CK_INDEXNM:          return "G.p_indexNm"_Stz;
    case CK_INDEXSCAN:        return "G.p_indexScan"_Stz;
    case CK_JOINORDER:        return "G.p_joinOrder"_Stz;
    case CK_JOINORDERING:     return "G.p_joinOrdering"_Stz;
    case CK_JOINPREDLIST:     return "G.p_joinPredList"_Stz;
    case CK_KEYEXISTS:        return "G.p_keyExists"_Stz;
    case CK_KEYNOTEXISTS:     return "G.p_keyNotExists"_Stz;
    case CK_LBOUND:           return "G.p_lBound"_Stz;
    case CK_LEVEL:            return "G.p_level"_Stz;
    case CK_LIMIT:            return "G.p_limit"_Stz;
    case CK_MANNER:           return "G.p_manner"_Stz;
    case CK_NCORRCOLS:        return "G.p_nCorrCols"_Stz;
    case CK_NEXT:             return "G.p_next"_Stz;
    case CK_NOTE:             return "G.p_note"_Stz;
    case CK_NOTPRED:          return "G.p_notPred"_Stz;
    case CK_NPARMCOLS:        return "G.p_nParmCols"_Stz;
    case CK_ON:               return "G.p_on"_Stz;
    case CK_ORDAGG:           return "G.p_ordAgg"_Stz;
    case CK_ORDELMODN:        return "G.p_ordElmOdn"_Stz;
    case CK_ORDER:            return "G.p_order"_Stz;
    case CK_PARAM:            return "G.p_param"_Stz;
    case CK_PARAMLIST:        return "G.p_paramList"_Stz;
    case CK_PARTITION:        return "G.p_partition"_Stz;
    case CK_PERMUTATION:      return "G.p_permutation"_Stz;
    case CK_PMZWHERE:         return "G.p_pmzWhere"_Stz;
    case CK_PROJECT:          return "G.p_project"_Stz;
    case CK_PROJELIST:        return "G.p_projElist"_Stz;
    case CK_PTNCOL:           return "G.p_ptnCol"_Stz;
    case CK_PTNCOLLIST:       return "G.p_ptnColList"_Stz;
    case CK_PTNCOLNM:         return "G.p_ptnColNm"_Stz;
    case CK_PTNDJOIN:         return "G.p_ptndJoin"_Stz;
    case CK_PTNDJOINSET:      return "G.p_ptndJoinSet"_Stz;
    case CK_REASON:           return "G.p_reason"_Stz;
    case CK_RELNOP:           return "G.p_relnOp"_Stz;
    case CK_RPNCALC:          return "G.p_rpnCalc"_Stz;
    case CK_SBYELIST:         return "G.p_sbyElist"_Stz;
    case CK_SCAN:             return "G.p_scan"_Stz;
    case CK_SEND:             return "G.p_send"_Stz;
    case CK_SIZE:             return "G.p_size"_Stz;
    case CK_SORT:             return "G.p_sort"_Stz;
    case CK_STAGE:            return "G.p_stage"_Stz;
    case CK_STMCOL:           return "G.p_stmCol"_Stz;
    case CK_STMCOLLIST:       return "G.p_stmColList"_Stz;
    case CK_STMCOLNM:         return "G.p_stmColNm"_Stz;
    case CK_STMCOLODN:        return "G.p_stmColOdn"_Stz;
    case CK_STREAM:           return "G.p_stream"_Stz;
    case CK_SUBSET:           return "G.p_subSet"_Stz;
    case CK_TABLE:            return "G.p_table"_Stz;
    case CK_TABLELIST:        return "G.p_tableList"_Stz;
    case CK_TABLENM:          return "G.p_tableNm"_Stz;
    case CK_TABLEPTN:         return "G.p_tablePtn"_Stz;
    case CK_TBLCOLODN:        return "G.p_tblColOdn"_Stz;
    case CK_TERM:             return "G.p_term"_Stz;
    case CK_TOPOLOGY:         return "G.p_topology"_Stz;
    case CK_ROPRED:           return "G.p_roPred"_Stz;
    case CK_TWIST:            return "G.p_twist"_Stz;
    case CK_TYPE:             return "G.p_type"_Stz;
    case CK_UBOUND:           return "G.p_uBound"_Stz;
    case CK_UNIQUE:           return "G.p_unique"_Stz;
    case CK_WHERE:            return "G.p_where"_Stz;
    case CK_XBOUND:           return "G.p_xBound"_Stz;

    case CK_AV_AGGP:          return "G.p_av_aggP"_Stz;
    case CK_AV_AGGPN:         return "G.p_av_aggPN"_Stz;
    case CK_AV_AGGPNO:        return "G.p_av_aggPNO"_Stz;
    case CK_AV_AGGREGP:       return "G.p_av_aggregP"_Stz;
    case CK_AV_AGGREGPN:      return "G.p_av_aggregPN"_Stz;
    case CK_AV_AGGREGPNO:     return "G.p_av_aggregPNO"_Stz;
    case CK_AV_FILTER:        return "G.p_av_filter"_Stz;
    case CK_AV_FILTERPROJECT: return "G.p_av_filterProject"_Stz;
    case CK_AV_GBYAGGP:       return "G.p_av_gbyAggP"_Stz;
    case CK_AV_GBYAGGPN:      return "G.p_av_gbyAggPN"_Stz;
    case CK_AV_GBYPNO:        return "G.p_av_gbyPNO"_Stz;
    case CK_AV_JOINSERIES:    return "G.p_av_joinSeries"_Stz;
    case CK_AV_LIMIT:         return "G.p_av_limit"_Stz;
    case CK_AV_PROJECT:       return "G.p_av_project"_Stz;
    case CK_AV_SCANJOINSERIES: return "G.p_av_scanJoinSeries"_Stz;
    case CK_AV_SCANONLY:      return "G.p_av_scanOnly"_Stz;
    case CK_AV_SORT:          return "G.p_av_sort"_Stz;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bMore(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (G.m_i_Child + 1 < G.m_pCard->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChild(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  i8 i_Next = G.m_i_Child + 1;

  if (i_Next >= G.m_pCard->nChildCards())
    return nullptr;

  Card* pNextMain = G.m_pCard->pGetChild(i_Next);

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pNextChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  Card* pNextMain = this->pNextChild();
  if (! pNextMain || eCard != pNextMain->eCard())
    return nullptr;

  return pNextMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPrevChild(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  i8 i_Prev = G.m_i_Child - 1;

  if (i_Prev < 0)
    return nullptr;

  Card* pChild = G.m_pCard->pGetChild(i_Prev);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pPrevChildIf(eCK eCard)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  Card* pPrevMain = this->pPrevChild();
  if (! pPrevMain || eCard != pPrevMain->eCard())
    return nullptr;

  return pPrevMain;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pRecurseEach(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // descend into current child

  if (G.m_pChild && (! G.m_bDontDescend) ) {
    if (G.m_pChild->nChildCards()) {

      G.m_bRecursed = true;
      vPushRecurseIter();
      vChangeCard(G.m_pChild);
      G.m_pChild = pEachChild();

      return G.m_pChild;
    }
  }

  G.m_bDontDescend = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // visit next child

  if (bEachChild())
    return G.m_pChild;

  while (bPopRecurseIter()) {
    if (bEachChild())
      return G.m_pChild;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pRecurseEach(eCK eCard)
{
  _precondition_( _MAGIC_, _FOOTER_);

  while (Card* p_card = pRecurseEach())
    if (p_card->bIsCard(eCard))
      return p_card;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveChild(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (pChild == G.m_pChild)
    vRemoveCurr(G.m_pChild->eCard());
  else
    G.m_pCard->vRemoveChild(pChild);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteChild(Card*& rpChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  vRemoveChild(rpChild);

  delete rpChild;

  rpChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  RASSERT(eCrossCheck == G.m_pChild->eCard());
  G.m_pCard->vRemoveChild(G.m_pChild);
  G.m_i_Child--;
  if (G.m_i_Child < G.m_pCard->nChildCards())
    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);
  else
    G.m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRemoveDeleteCurr(eCK eCrossCheck)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  RASSERT(eCrossCheck == G.m_pChild->eCard());
  G.m_pCard->vRemoveChild(G.m_pChild);
  delete G.m_pChild;
  G.m_i_Child--;
  if (G.m_i_Child < G.m_pCard->nChildCards())
    G.m_pChild = G.m_pCard->pGetChild(G.m_i_Child);
  else
    G.m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRestart(void)
{
  _precondition_(_MAGIC_, _FOOTER_);

  G.m_i_Child = C_UNKNOWN;
  G.m_pChild  = nullptr;
  G.m_bActive = true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRewind(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  if (G.m_i_Child == 0) {
    G.m_i_Child = C_UNKNOWN;
    G.m_pChild = nullptr;
    return;
  }

  G.m_i_Child--;

  if (G.m_i_Child < G.m_pCard->nChildCards())
    G.m_pChild =  G.m_pCard->pGetChild(G.m_i_Child);
  else
    G.m_pChild = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardIter::pRewindChild(void)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (G.m_i_Child == 0) {
    G.m_i_Child = C_UNKNOWN;
    return nullptr;
  }

  G.m_i_Child--;

  if (G.m_i_Child < G.m_pCard->nChildCards())
    G.m_pChild =  G.m_pCard->pGetChild(G.m_i_Child);
  else
    G.m_pChild = nullptr;

  return G.m_pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vRestore(void)
{
  CardIterMembers* pRestore = pPop();

  G.vCopyFrom(pRestore);

  delete pRestore;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSave(void)
{
  CardIterMembers* pSave = new CardIterMembers;

  pSave->vCopyFrom(&G);

  vPush(pSave);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 CardIter::iSentinalsDepth(void)
{

  return this->nItems();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  G.m_i_Child = G.m_pCard->iFindChild(pChild);

  if (C_UNKNOWN == G.m_i_Child)
    G.m_pChild = nullptr;
  else
    G.m_pChild = pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(CardIter* pCardIter_src)
{
  _precondition_(_MAGIC_,_FOOTER_);

  G.m_pCard   = pCardIter_src->G.m_pCard;
  G.m_i_Child = pCardIter_src->G.m_i_Child;
  G.m_pChild  = pCardIter_src->G.m_pChild;

  G.m_bActive = pCardIter_src->G.m_bActive;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vSet(i8 iChild, Card* pChild)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(G.m_bActive);

  G.m_i_Child = iChild;
  G.m_pChild = pChild;
}

/*----------------------------------------------------------------------------*/
/* update member card                                                         */
/*----------------------------------------------------------------------------*/
void CardIter::G_vUpdateMember(eCK eMember, Card* pCard, const char* asFuncNm)
{
  _precondition_(_MAGIC_, _FOOTER_);

  RASSERT(!pCard || pCard->bIsCard(eMember));

  switch (eMember) {

    case CK_ABSJOIN:          *((Card**)&G.p_absJoin)          = pCard; break;
    case CK_AGGELIST:         *((Card**)&G.p_aggElist)         = pCard; break;
    case CK_AGGREG:           *((Card**)&G.p_aggreg)           = pCard; break;
    case CK_AS:               *((Card**)&G.p_as)               = pCard; break;
    case CK_BOLPRED:          *((Card**)&G.p_bolPred)          = pCard; break;
    case CK_BOOLCALC:         *((Card**)&G.p_boolCalc)         = pCard; break;
    case CK_BOUND:            *((Card**)&G.p_bound)            = pCard; break;
    case CK_BOUNDS:           *((Card**)&G.p_bounds)           = pCard; break;
    case CK_CALCOP:           *((Card**)&G.p_calcOp)           = pCard; break;
    case CK_CALL:             *((Card**)&G.p_call)             = pCard; break;
    case CK_CAST:             *((Card**)&G.p_cast)             = pCard; break;
    case CK_CLAUSE:           *((Card**)&G.p_clause)           = pCard; break;
    case CK_CMPPRED:          *((Card**)&G.p_cmpPred)          = pCard; break;
    case CK_COMBINED:         *((Card**)&G.p_combined)         = pCard; break;
    case CK_COMMENT:          *((Card**)&G.p_comment)          = pCard; break;
    case CK_CONSTRAINT:       *((Card**)&G.p_constraint)       = pCard; break;
    case CK_COPTNJOIN:        *((Card**)&G.p_coPtnJoin)        = pCard; break;
    case CK_COPTNJOINLIST:    *((Card**)&G.p_coPtnJoinList)    = pCard; break;
    case CK_COPTNPRED:        *((Card**)&G.p_coPtnPred)        = pCard; break;
    case CK_COPTNPREDLIST:    *((Card**)&G.p_coPtnPredList)    = pCard; break;
    case CK_CROSSJOIN:        *((Card**)&G.p_crossJoin)        = pCard; break;
    case CK_CROSSPTNJOIN:     *((Card**)&G.p_crossPtnJoin)     = pCard; break;
    case CK_CROSSPTNJOINLIST: *((Card**)&G.p_crossPtnJoinList) = pCard; break;
    case CK_DIRECTION:        *((Card**)&G.p_direction)        = pCard; break;
    case CK_EBOUND:           *((Card**)&G.p_eBound)           = pCard; break;
    case CK_EXPR:             *((Card**)&G.p_expr)             = pCard; break;
    case CK_FILTER:           *((Card**)&G.p_filter)           = pCard; break;
    case CK_FOLD:             *((Card**)&G.p_fold)             = pCard; break;
    case CK_FOLDELIST:        *((Card**)&G.p_foldElist)        = pCard; break;
    case CK_GBYAGG:           *((Card**)&G.p_gbyAgg)           = pCard; break;
    case CK_GBYELIST:         *((Card**)&G.p_gbyElist)         = pCard; break;
    case CK_GENESIS:          *((Card**)&G.p_genesis)          = pCard; break;
    case CK_GROUP:            *((Card**)&G.p_group)            = pCard; break;
    case CK_HASH:             *((Card**)&G.p_hash)             = pCard; break;
    case CK_HAVING:           *((Card**)&G.p_having)           = pCard; break;
    case CK_IDXCOL:           *((Card**)&G.p_idxCol)           = pCard; break;
    case CK_IDXCOLLIST:       *((Card**)&G.p_idxColList)       = pCard; break;
    case CK_IDXCOLNM:         *((Card**)&G.p_idxColNm)         = pCard; break;
    case CK_INDEX:            *((Card**)&G.p_index)            = pCard; break;
    case CK_INDEXJOIN:        *((Card**)&G.p_indexJoin)        = pCard; break;
    case CK_INDEXLIST:        *((Card**)&G.p_indexList)        = pCard; break;
    case CK_INDEXNM:          *((Card**)&G.p_indexNm)          = pCard; break;
    case CK_INDEXSCAN:        *((Card**)&G.p_indexScan)        = pCard; break;
    case CK_JOINORDER:        *((Card**)&G.p_joinOrder)        = pCard; break;
    case CK_JOINORDERING:     *((Card**)&G.p_joinOrdering)     = pCard; break;
    case CK_JOINPREDLIST:     *((Card**)&G.p_joinPredList)     = pCard; break;
    case CK_KEYEXISTS:        *((Card**)&G.p_keyExists)        = pCard; break;
    case CK_KEYNOTEXISTS:     *((Card**)&G.p_keyNotExists)     = pCard; break;
    case CK_LBOUND:           *((Card**)&G.p_lBound)           = pCard; break;
    case CK_LEVEL:            *((Card**)&G.p_level)            = pCard; break;
    case CK_LIMIT:            *((Card**)&G.p_limit)            = pCard; break;
    case CK_MANNER:           *((Card**)&G.p_manner)           = pCard; break;
    case CK_NCORRCOLS:        *((Card**)&G.p_nCorrCols)        = pCard; break;
    case CK_NEXT:             *((Card**)&G.p_next)             = pCard; break;
    case CK_NOTE:             *((Card**)&G.p_note)             = pCard; break;
    case CK_NOTPRED:          *((Card**)&G.p_notPred)          = pCard; break;
    case CK_NPARMCOLS:        *((Card**)&G.p_nParmCols)        = pCard; break;
    case CK_ON:               *((Card**)&G.p_on)               = pCard; break;
    case CK_ORDAGG:           *((Card**)&G.p_ordAgg)           = pCard; break;
    case CK_ORDELMODN:        *((Card**)&G.p_ordElmOdn)        = pCard; break;
    case CK_ORDER:            *((Card**)&G.p_order)            = pCard; break;
    case CK_PARAM:            *((Card**)&G.p_param)            = pCard; break;
    case CK_PARAMLIST:        *((Card**)&G.p_paramList)        = pCard; break;
    case CK_PARTITION:        *((Card**)&G.p_partition)        = pCard; break;
    case CK_PERMUTATION:      *((Card**)&G.p_permutation)      = pCard; break;
    case CK_PMZWHERE:         *((Card**)&G.p_pmzWhere)         = pCard; break;
    case CK_PROJECT:          *((Card**)&G.p_project)          = pCard; break;
    case CK_PROJELIST:        *((Card**)&G.p_projElist)        = pCard; break;
    case CK_PTNCOL:           *((Card**)&G.p_ptnCol)           = pCard; break;
    case CK_PTNCOLLIST:       *((Card**)&G.p_ptnColList)       = pCard; break;
    case CK_PTNCOLNM:         *((Card**)&G.p_ptnColNm)         = pCard; break;
    case CK_PTNDJOIN:         *((Card**)&G.p_ptndJoin)         = pCard; break;
    case CK_PTNDJOINSET:      *((Card**)&G.p_ptndJoinSet)      = pCard; break;
    case CK_REASON:           *((Card**)&G.p_reason)           = pCard; break;
    case CK_RELNOP:           *((Card**)&G.p_relnOp)           = pCard; break;
    case CK_RPNCALC:          *((Card**)&G.p_rpnCalc)          = pCard; break;
    case CK_SBYELIST:         *((Card**)&G.p_sbyElist)         = pCard; break;
    case CK_SCAN:             *((Card**)&G.p_scan)             = pCard; break;
    case CK_SEND:             *((Card**)&G.p_send)             = pCard; break;
    case CK_SIZE:             *((Card**)&G.p_size)             = pCard; break;
    case CK_SORT:             *((Card**)&G.p_sort)             = pCard; break;
    case CK_STAGE:            *((Card**)&G.p_stage)            = pCard; break;
    case CK_STMCOL:           *((Card**)&G.p_stmCol)           = pCard; break;
    case CK_STMCOLLIST:       *((Card**)&G.p_stmColList)       = pCard; break;
    case CK_STMCOLNM:         *((Card**)&G.p_stmColNm)         = pCard; break;
    case CK_STMCOLODN:        *((Card**)&G.p_stmColOdn)        = pCard; break;
    case CK_STREAM:           *((Card**)&G.p_stream)           = pCard; break;
    case CK_SUBSET:           *((Card**)&G.p_subSet)           = pCard; break;
    case CK_TABLE:            *((Card**)&G.p_table)            = pCard; break;
    case CK_TABLELIST:        *((Card**)&G.p_tableList)        = pCard; break;
    case CK_TABLENM:          *((Card**)&G.p_tableNm)          = pCard; break;
    case CK_TABLEPTN:         *((Card**)&G.p_tablePtn)         = pCard; break;
    case CK_TBLCOLODN:        *((Card**)&G.p_tblColOdn)        = pCard; break;
    case CK_TERM:             *((Card**)&G.p_term)             = pCard; break;
    case CK_TOPOLOGY:         *((Card**)&G.p_topology)         = pCard; break;
    case CK_ROPRED:           *((Card**)&G.p_roPred)           = pCard; break;
    case CK_TWIST:            *((Card**)&G.p_twist)            = pCard; break;
    case CK_TYPE:             *((Card**)&G.p_type)             = pCard; break;
    case CK_UBOUND:           *((Card**)&G.p_uBound)           = pCard; break;
    case CK_UNIQUE:           *((Card**)&G.p_unique)           = pCard; break;
    case CK_WHERE:            *((Card**)&G.p_where)            = pCard; break;
    case CK_XBOUND:           *((Card**)&G.p_xBound)           = pCard; break;

    case CK_AV_AGGP:          *((Card**)&G.p_av_aggP)          = pCard; break;
    case CK_AV_AGGPN:         *((Card**)&G.p_av_aggPN)         = pCard; break;
    case CK_AV_AGGPNO:        *((Card**)&G.p_av_aggPNO)        = pCard; break;
    case CK_AV_AGGREGP:       *((Card**)&G.p_av_aggregP)       = pCard; break;
    case CK_AV_AGGREGPN:      *((Card**)&G.p_av_aggregPN)      = pCard; break;
    case CK_AV_AGGREGPNO:     *((Card**)&G.p_av_aggregPNO)     = pCard; break;
    case CK_AV_FILTER:        *((Card**)&G.p_av_filter)        = pCard; break;
    case CK_AV_FILTERPROJECT: *((Card**)&G.p_av_filterProject) = pCard; break;
    case CK_AV_GBYAGGP:       *((Card**)&G.p_av_gbyAggP)       = pCard; break;
    case CK_AV_GBYAGGPN:      *((Card**)&G.p_av_gbyAggPN)      = pCard; break;
    case CK_AV_GBYPNO:        *((Card**)&G.p_av_gbyPNO)        = pCard; break;
    case CK_AV_JOINSERIES:    *((Card**)&G.p_av_joinSeries)    = pCard; break;
    case CK_AV_LIMIT:         *((Card**)&G.p_av_limit)         = pCard; break;
    case CK_AV_PROJECT:       *((Card**)&G.p_av_project)       = pCard; break;
    case CK_AV_SCANJOINSERIES: *((Card**)&G.p_av_scanJoinSeries) = pCard; break;
    case CK_AV_SCANONLY:      *((Card**)&G.p_av_scanOnly)      = pCard; break;
    case CK_AV_SORT:          *((Card**)&G.p_av_sort)          = pCard; break;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CardIter::bPopRecurseIter(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  CardIterMembers* pXsentinals2 = pPeek(0);
  if (! pXsentinals2 || ! pXsentinals2->m_bRecursed)
    return false;

  vRestore();

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CardIter::vPushRecurseIter(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  RASSERT(G.m_bRecursed);

  vSave();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
