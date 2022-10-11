/*============================================================================*/
/* CardMembers.cpp --                                                            */
/*============================================================================*/

#include "CardMembers.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardMembers::CardMembers(void)
{

  //_checkInitialised_(CN_CardMembers);

  _board_        = nullptr;

  m_pChildCard     = nullptr;
  m_i_Child        = C_UNKNOWN;

  m_pDupCard       = nullptr;

  m_pMainCard      = nullptr;
  m_i_Main         = C_UNKNOWN;

  m_pNextMain      = nullptr;
  m_i_Next         = C_UNKNOWN;

  m_pPoppedCard    = nullptr;
  m_i_Popped       = C_UNKNOWN;

  m_p_absJoin          = nullptr;
  m_p_aggreg           = nullptr;
  m_p_calcBool         = nullptr;
  m_p_combined         = nullptr;
  m_p_coPtnJoin        = nullptr;
  m_p_coPtnJoinList    = nullptr;
  m_p_coPtnPred        = nullptr;
  m_p_coPtnPredList    = nullptr;
  m_p_crossPtnJoin     = nullptr;
  m_p_crossPtnJoinList = nullptr;
  m_p_comment          = nullptr;
  m_p_expr             = nullptr;
  m_p_filter           = nullptr;
  m_p_gbyElist         = nullptr;
  m_p_idxColList       = nullptr;
  m_p_indexJoin        = nullptr;
  m_p_indexList        = nullptr;
  m_p_indexPtr         = nullptr;
  m_p_indexScan        = nullptr;
  m_p_joinOrder        = nullptr;
  m_p_joinOrderSeq     = nullptr;
  m_p_joinPred         = nullptr;
  m_p_joinPredList     = nullptr;
  m_p_lBound           = nullptr;
  m_p_permutation      = nullptr;
  m_p_permutation_low  = nullptr;
  m_p_predCmp          = nullptr;
  m_p_project          = nullptr;
  m_p_ptndJoin         = nullptr;
  m_p_ptndJoinSet      = nullptr;
  m_p_sbyElist         = nullptr;
  m_p_scan             = nullptr;
  m_p_scanPred         = nullptr;
  m_p_sort             = nullptr;
  m_p_stage            = nullptr;
  m_p_stage_a          = nullptr;
  m_p_stage_b          = nullptr;
  m_p_stageTopo        = nullptr;
  m_p_stageTopo_a      = nullptr;
  m_p_stageTopo_b      = nullptr;
  m_p_stream           = nullptr;
  m_p_streamColList    = nullptr;
  m_p_subSet           = nullptr;
  m_p_table            = nullptr;
  m_p_tableList        = nullptr;
  m_p_tablePtn         = nullptr;
  m_p_tablePtr         = nullptr;
  m_p_term             = nullptr;
  m_p_term_0           = nullptr;
  m_p_term_1           = nullptr;
  m_p_uBound           = nullptr;

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardMembers::~CardMembers(void) noexcept(false)
{

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
