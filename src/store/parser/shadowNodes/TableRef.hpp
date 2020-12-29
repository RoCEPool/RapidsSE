/*============================================================================*/
/* TableRef.hpp - a reference to a named table                                */
/*============================================================================*/
#pragma once

namespace Parser
{
  class TableRef : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    TableRef();

  public:
    static QueryNode* pCreate(Text* pCatalogName, Text* pSchemaName, Text* pTableName);
    static void       vDescribe(QueryNode* pTableRef, s2K& sBuf);
    static b1         bIsResolved(QueryNode* pTableRef);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
