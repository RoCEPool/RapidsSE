/*============================================================================*/
/* QueryNodeList.hpp - A list of QueryNodes                                   */
/*============================================================================*/
#pragma once

namespace Parser
{
  class QueryNode;

  typedef ListExo_t<CN_QUERYNODELIST, QueryNode> QueryNodeList;
  typedef ListExoIter_t<QueryNode> QueryNodeIter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
