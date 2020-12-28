/*============================================================================*/
/* TableRef.hpp - a reference to a named table                                */
/*============================================================================*/
#pragma once

namespace Parser
{
  class TableRef : LogicalNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    TableRef();

  public:
    static QueryNode* pCreate(Text* pCatalogName, Text* pSchemaName, Text* pTableName);
    static void       vPrint(QueryNode* pTableRef, s256& buf);
    static b1         bIsResolved(QueryNode* pTableRef);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
