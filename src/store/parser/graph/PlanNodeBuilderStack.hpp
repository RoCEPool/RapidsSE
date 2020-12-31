/*============================================================================*/
/* PlanNodeBuilderStack.hpp - A stack of PlanNodeBuilders                     */
/*============================================================================*/
#pragma once

namespace Parser
{
  class PlanNodeBuilder;

  typedef StackNL_t<CN_PLANNODEBUILDERSTACK,PlanNodeBuilder,1024> PlanNodeBuilderStack;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
