/*============================================================================*/
/* PlanNodeBuilderList.hpp - A list of PlanNodeBuilders                       */
/*============================================================================*/
#pragma once

namespace Parser
{
  class PlanNodeBuilder;

  typedef ListExo_t<CN_PLANNODEBUILDERLIST, PlanNodeBuilder> PlanNodeBuilderList;
  typedef ListExoIter_t<PlanNodeBuilder> PlanNodeBuilderIter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
