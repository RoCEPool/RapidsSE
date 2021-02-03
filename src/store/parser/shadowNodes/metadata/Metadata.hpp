/*============================================================================*/
/* Metadata.hpp - a ShadowNode for the /__root__/metadata node and subtree    */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class MetadataNode;
  class PlanNode;

  class Metadata : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    Metadata();

  public:

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static MetadataNode* pCatalog(Stz* szCatalogName);
    static MetadataNode* pCreateCatalog(Stz* szCatalogName);

    // MetaCatalog
    // MetaSchema
    // MetaTable
    // MetaIndex
    // MetaColumn
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
