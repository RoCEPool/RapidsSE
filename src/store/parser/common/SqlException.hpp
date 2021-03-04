/*============================================================================*/
/* ParserException.hpp - exceptions that can occur while parsing a SQL stmt   */
/*============================================================================*/
#pragma once

#include <exception>
#include "BASE.h"
#include "InfoSet.hpp"
#include "TableMetadata.hpp"
#include "ColumnMetadata.hpp"
#include "TableDef.hpp"

namespace Parser
{
  // Base class for exceptions that can be thrown during the sql parsing
  class SqlException : public std::runtime_error
  {
  protected:
    // Constructor
    SqlException(const Stz* szErrorMsg)
      : std::runtime_error(const_cast<Stz*>(szErrorMsg)->azChar())    { /* noop */ }

  public:
    Stz* szError()    { return (Stz*) this->what(); }
  };

  // An error with the format of the input
  class SyntaxException : public SqlException
  {
  private:
    i8 m_iLine;
    i8 m_iColumn;

  public:
    // Constructor
    SyntaxException(i8 iLine, i8 iColumn, const Stz* szErrorMsg)
      : SqlException(szErrorMsg), m_iLine(iLine), m_iColumn(iColumn)
      { /* noop */ }

    // Creates and throws a SyntaxException
    static void vThrow(i8 iLine, i8 iColumn, Stz* szFormat, ...) {
      s256 sErrMsg;
      va_list vaArgs;

      va_start(vaArgs,szFormat);
      sErrMsg.vSprint(vaArgs, szFormat);
      va_end(vaArgs);

      throw SyntaxException(iLine, iColumn, sErrMsg.szStz());
    }

    i8 iLine()            { return m_iLine;   }
    i8 iColumn()          { return m_iColumn; }
  };

  // An error with the meaning of the input
  class SemanticException : public SqlException
  {
  public:
    // Constructor
    SemanticException(const Stz* szErrorMsg) : SqlException(szErrorMsg)   { /* noop */ }

    // Creates and throws a SemanticException
    static void vThrow(Stz* szFormat, ...) {
      s256 sErrMsg;
      va_list vaArgs;

      va_start(vaArgs,szFormat);
      sErrMsg.vSprint(vaArgs, szFormat);
      va_end(vaArgs);

      throw SemanticException(sErrMsg.szStz());
    }
  };


  // An error looking up a name in the system
  class NameValidationException : public SqlException
  {
  public:
    // Constructor
    NameValidationException(const Stz* szErrorMsg) : SqlException(szErrorMsg)   { /* noop */ }

    // Creates and throws a NameValidationException
    static void vThrowUnknownTable(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName)
    {
      s256 sErrMsg;
      sErrMsg.vConcat("Unknown table name: "_Stz);

      if (szCatalogName) {
        sErrMsg.vCatSprint("%s.%s.%s"_Stz, szCatalogName, szSchemaName, szTableName);
      } else if (szSchemaName) {
        sErrMsg.vCatSprint("%s.%s"_Stz, szSchemaName, szTableName);
      } else {
        sErrMsg.vCatSprint("%s"_Stz, szTableName);
      }
      throw NameValidationException(sErrMsg.szStz());
    }

    static void vThrowAmbiguousTable(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName, STORE::InfoSet* pInfoSet)
    {
      s256 sErrMsg;
      sErrMsg.vConcat("Ambiguous table name: "_Stz);

      if (szCatalogName) {
        sErrMsg.vCatSprint("%s.%s.%s"_Stz, szCatalogName, szSchemaName, szTableName);
      } else if (szSchemaName) {
        sErrMsg.vCatSprint("%s.%s"_Stz, szSchemaName, szTableName);
      } else {
        sErrMsg.vCatSprint("%s"_Stz, szTableName);
      }

      sErrMsg.vConcat(". Could be one of:"_Stz);

      for (Row* pRow = pInfoSet->pFirstRow();
           pRow;
           pRow = pInfoSet->pNextRow(pRow)) {

        TableMetadata* pTableMeta = static_cast<TableMetadata*>(pRow);
        sErrMsg.vCatSprint("\n  %s.%s.%s"_Stz,
                           pTableMeta->szCatalogName(),
                           pTableMeta->szSchemaName(),
                           pTableMeta->szTableName());
      }

      delete pInfoSet;
      throw NameValidationException(sErrMsg.szStz());
    }

    static void vThrowUnknownColumn(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName, Stz* szColumnName) {
      s256 sErrMsg;
      sErrMsg.vConcat("Unknown column name: "_Stz);

      if (szCatalogName) {
        sErrMsg.vCatSprint("%s.%s.%s.%s"_Stz, szCatalogName, szSchemaName, szTableName, szColumnName);
      } else if (szSchemaName) {
        sErrMsg.vCatSprint("%s.%s.%s"_Stz, szSchemaName, szTableName, szColumnName);
      } else if (szTableName) {
        sErrMsg.vCatSprint("%s.%s"_Stz, szTableName, szColumnName);
      } else {
        sErrMsg.vCatSprint("%s"_Stz, szColumnName);
      }
      throw NameValidationException(sErrMsg.szStz());
    }

    static void vThrowAmbiguousColumn(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                                      Stz* szColumnName, PlanNodeList* pTableDefList)
    {
      ASSERT(pTableDefList->nItems() > 1);

      s256 sErrMsg;
      sErrMsg.vConcat("Ambiguous column name: "_Stz);

      if (szCatalogName) {
        sErrMsg.vCatSprint("%s.%s.%s.%s"_Stz, szCatalogName, szSchemaName, szTableName, szColumnName);
      } else if (szSchemaName) {
        sErrMsg.vCatSprint("%s.%s.%s"_Stz, szSchemaName, szTableName, szColumnName);
      } else if (szTableName){
        sErrMsg.vCatSprint("%s.%s"_Stz, szTableName, szColumnName);
      } else {
        sErrMsg.vCatSprint("%s"_Stz, szColumnName);
      }

      sErrMsg.vConcat(". Could be one of:"_Stz);

      PlanNodeIter* pIter = pTableDefList->pNewIter();
      while (pIter->bHasNext()) {
        QueryNode* pNode = static_cast<QueryNode*>(pIter->pNext());

        sErrMsg.vCatSprint("\n  %s.%s.%s.%s"_Stz,
                           TableDef::szCatalogName(pNode),
                           TableDef::szSchemaName(pNode),
                           TableDef::szTableName(pNode),
                           szColumnName);
      }

      delete pIter;
      pTableDefList->vRemoveAll();
      delete pTableDefList;   // yes, delete the list we were passed

      throw NameValidationException(sErrMsg.szStz());
    }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
