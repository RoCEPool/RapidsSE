/*============================================================================*/
/* NodeNamer.hpp - Gives nodes unique names                                   */
/*============================================================================*/
#pragma once

namespace Parser
{
  class QueryNode;

  class NodeNamer
  {
  private:
    static Atomic* ms_pObjId;

    // Shalt never be instantiated. Static functions only.
    NodeNamer();

  public:
    // Generate a unique name for a node and save it in sNameBuf
    static void vGenerateName(QueryNode* pNode, s256& sNameBuf);
    static void vGenerateName(Stz* szNodeType, s256& sNameBuf);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
