/*============================================================================*/
/* CorrBySquash.hpp - Push down ProjectOps that implement the CORRESPONDINB BY*/
/* clause and squash any redundant ProjectOps below it                        */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace STORE
{
  class SeApi;
}

namespace Parser
{
  class QueryNode;

  class CorrBySquash
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    CorrBySquash();

  public:
    // Searches for any CORRESPONDING BY ProjectOps and squashes them if possible
    static void vSearchAndSquash(PlanNode* pRootNode);


  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
