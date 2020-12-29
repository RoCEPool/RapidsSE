/*============================================================================*/
/* ColumnRef.hpp - an expression representing a reference to a column         */
/*============================================================================*/
#pragma once

namespace Parser
{
  class ColumnRef : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ColumnRef();

  public:
    static QueryNode* pCreate(Text* pCatalogName, Text* pSchemaName,
                              Text* pTableName, Text* pColumnName);

    static void       vPrint(QueryNode* pStmt, s256& sBuf);


  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
