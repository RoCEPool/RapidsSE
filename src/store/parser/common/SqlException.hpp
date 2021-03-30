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
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "eEC.h"

namespace Parser
{
  // Base class for exceptions that can be thrown during the sql parsing
  class SqlException : public std::runtime_error
  {
  protected:
    eEC m_eErrorCode;
    i8  m_iLine;
    i8  m_iColumn;

    // Constructor
    SqlException(eEC eErrorCode, const Stz* szErrorMsg)
      : std::runtime_error(const_cast<Stz*>(szErrorMsg)->azChar()),
        m_eErrorCode(eErrorCode),
        m_iLine(0),
        m_iColumn(0)
    { /* noop */ }

    // Constructor, with line and column information
    SqlException(eEC eErrorCode, const Stz* szErrorMsg, i8 iLine, i8 iColumn)
        : std::runtime_error(const_cast<Stz*>(szErrorMsg)->azChar()),
          m_eErrorCode(eErrorCode),
          m_iLine(iLine),
          m_iColumn(iColumn)
    { /* noop */ }

  public:
    Stz* szError()    { return (Stz*) this->what(); }
    eEC  eErrorCode() { return m_eErrorCode;        }
    i8   iLine()      { return m_iLine;             }
    i8   iColumn()    { return m_iColumn;           }
  };

  // An error with the format of the input
  class SyntaxException : public SqlException
  {
  protected:
    // Constructor. Can only be created by calling vThrow() methods
    SyntaxException(eEC eErrorCode, const Stz* szErrorMsg, i8 iLine, i8 iColumn)
      : SqlException(eErrorCode, szErrorMsg, iLine, iColumn)
      { /* noop */ }

  public:
    // Creates and throws a SyntaxException. The error message should come from ANTLR.
    static void vThrowAntlr(i8 iLine, i8 iColumn, Stz* szFormat, ...)
    {
      s256 sErrMsg;
      va_list vaArgs;

      va_start(vaArgs,szFormat);
      sErrMsg.vSprintVA(szFormat,vaArgs);
      va_end(vaArgs);

      throw SyntaxException(EC_SYNTAX_ERROR, sErrMsg.szStz(), iLine, iColumn);
    }

    // An error that indicates that the given syntax feature is not yet implemented
    static void vThrowNotImplemented(i8 iLine, i8 iColumn, Stz* szFeatureName)
    {
      s256 sErrMsg(aErrorMsg[EC_NOT_IMPLEMENTED]);
      sErrMsg.vCatSprint(": %s"_Stz, szFeatureName);

      throw SyntaxException(EC_NOT_IMPLEMENTED, sErrMsg.szStz(), iLine, iColumn);
    }

    static void vThrow(i8 iLine, i8 iColumn, eEC eErrorCode)
    {
      s256 sErrMsg(aErrorMsg[eErrorCode]);
      throw SyntaxException(eErrorCode, sErrMsg.szStz(), iLine, iColumn);
    }

    static void vThrow(i8 iLine, i8 iColumn, eEC eErrorCode, Stz* szParamFormat, ...)
    {
      // Get the default error message for this error code
      s256 sErrMsg(aErrorMsg[eErrorCode]);

      // Form a string from the ParamFormat and any arguments
      s256 sParamStr;
      va_list vaArgs;

      va_start(vaArgs,szParamFormat);
      sParamStr.vSprintVA(szParamFormat,vaArgs);
      va_end(vaArgs);

      // Combine the ParamFormat string with the default error message
      sErrMsg.vConcat(sParamStr);

      throw SyntaxException(eErrorCode, sErrMsg.szStz(), iLine, iColumn);
    }
  };

  // An error with the meaning of the input
  class SemanticException : public SqlException
  {
  protected:
    // Constructor
    SemanticException(eEC eErrorCode, Stz* szErrorMsg) : SqlException(eErrorCode, szErrorMsg)   { /* noop */ }

  public:
    // Creates and throws a type of SemanticException. No parameters are appended to the error message.
    static void vThrow(eEC eErrorCode)
    {
      throw SemanticException(eErrorCode, aErrorMsg[eErrorCode]);
    }

    // Creates and throws a type of SemanticException. Parameters are appended to the error message.
    static void vThrow(eEC eErrorCode, Stz* szParamFormat, ...)
    {
      s256 sErrMsg(aErrorMsg[eErrorCode]);
      va_list vaArgs;

      // Print the parameters to a string
      s256 sParamStr;
      va_start(vaArgs,szParamFormat);
      sParamStr.vSprintVA(szParamFormat,vaArgs);
      va_end(vaArgs);

      // Combine the base error message and the parameters
      sErrMsg.vConcat(sParamStr);

      throw SemanticException(eErrorCode, sErrMsg.szStz());
    }
  };


  // An error looking up a name in the system
  class NameValidationException : public SqlException
  {
  protected:
    // Constructor
    NameValidationException(eEC eErrorCode, const Stz* szErrorMsg)
      : SqlException(eErrorCode, szErrorMsg)
    { /* noop */ }

  public:
    // Convenience functions for creating and throwing errors with custom
    // formatting and appending of parameters

    static void vThrowUnknownTable(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName)
    {
      s256 sErrMsg(aErrorMsg[EC_NO_SUCH_TABLE2]);

      if (szCatalogName) {
        sErrMsg.vCatSprint(": %s.%s.%s"_Stz, szCatalogName, szSchemaName, szTableName);
      } else if (szSchemaName) {
        sErrMsg.vCatSprint(": %s.%s"_Stz, szSchemaName, szTableName);
      } else {
        sErrMsg.vCatSprint(": %s"_Stz, szTableName);
      }
      throw NameValidationException(EC_NO_SUCH_TABLE2, sErrMsg.szStz());
    }

    static void vThrowAmbiguousTable(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName, STORE::InfoSet* pInfoSet)
    {
      s256 sErrMsg(aErrorMsg[EC_AMBIGUOUS_TABLE]);

      if (szCatalogName) {
        sErrMsg.vCatSprint(": %s.%s.%s"_Stz, szCatalogName, szSchemaName, szTableName);
      } else if (szSchemaName) {
        sErrMsg.vCatSprint(": %s.%s"_Stz, szSchemaName, szTableName);
      } else {
        sErrMsg.vCatSprint(": %s"_Stz, szTableName);
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
      throw NameValidationException(EC_AMBIGUOUS_TABLE, sErrMsg.szStz());
    }

    static void vThrowUnknownColumn(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName, Stz* szColumnName) {
      s256 sErrMsg(aErrorMsg[EC_NO_SUCH_COLUMN]);

      if (szCatalogName) {
        sErrMsg.vCatSprint(": %s.%s.%s.%s"_Stz, szCatalogName, szSchemaName, szTableName, szColumnName);
      } else if (szSchemaName) {
        sErrMsg.vCatSprint(": %s.%s.%s"_Stz, szSchemaName, szTableName, szColumnName);
      } else if (szTableName) {
        sErrMsg.vCatSprint(": %s.%s"_Stz, szTableName, szColumnName);
      } else {
        sErrMsg.vCatSprint(": %s"_Stz, szColumnName);
      }
      throw NameValidationException(EC_NO_SUCH_COLUMN, sErrMsg.szStz());
    }

    static void vThrowAmbiguousColumn(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                                      Stz* szColumnName, PlanNodeList* pTableDefList)
    {
      ASSERT(pTableDefList->nItems() > 1);

      s256 sErrMsg(aErrorMsg[EC_AMBIGUOUS_COLUMN]);

      if (szCatalogName) {
        sErrMsg.vCatSprint(": %s.%s.%s.%s"_Stz, szCatalogName, szSchemaName, szTableName, szColumnName);
      } else if (szSchemaName) {
        sErrMsg.vCatSprint(": %s.%s.%s"_Stz, szSchemaName, szTableName, szColumnName);
      } else if (szTableName){
        sErrMsg.vCatSprint(": %s.%s"_Stz, szTableName, szColumnName);
      } else {
        sErrMsg.vCatSprint(": %s"_Stz, szColumnName);
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

      throw NameValidationException(EC_AMBIGUOUS_COLUMN, sErrMsg.szStz());
    }

    static void vThrowUnresolvableColumnWildcard(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName) {
      s256 sErrMsg(aErrorMsg[EC_UNRESOLVED_COLUMN_WILDCARD]);

      if (szCatalogName) {
        sErrMsg.vCatSprint(": %s.%s.%s.*"_Stz, szCatalogName, szSchemaName, szTableName);
      } else if (szSchemaName) {
        sErrMsg.vCatSprint(": %s.%s.*"_Stz, szSchemaName, szTableName);
      } else if (szTableName) {
        sErrMsg.vCatSprint(": %s.*"_Stz, szTableName);
      } else {
        sErrMsg.vConcat(": *"_Stz);
      }
      throw NameValidationException(EC_UNRESOLVED_COLUMN_WILDCARD, sErrMsg.szStz());
    }
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
