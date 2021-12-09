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
#include "QueryNodeList.hpp"
#include "ProjectedColumn.hpp"
#include "DataSource.hpp"
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
      s2K sErrMsg;
      va_list vaArgs;

      va_start(vaArgs,szFormat);
      sErrMsg.vSprintVA(szFormat,vaArgs);
      va_end(vaArgs);

      throw SyntaxException(EC_SYNTAX_ERROR, sErrMsg.szStz(), iLine, iColumn);
    }

    // An error that indicates that the given syntax feature is not yet implemented
    static void vThrowNotImplemented(i8 iLine, i8 iColumn, Stz* szFeatureName)
    {
      s2K sErrMsg(aErrorMsg[EC_NOT_IMPLEMENTED]);
      sErrMsg.vCatSprint(": %s"_Stz, szFeatureName);

      throw SyntaxException(EC_NOT_IMPLEMENTED, sErrMsg.szStz(), iLine, iColumn);
    }

    static void vThrow(i8 iLine, i8 iColumn, eEC eErrorCode)
    {
      s2K sErrMsg(aErrorMsg[eErrorCode]);
      throw SyntaxException(eErrorCode, sErrMsg.szStz(), iLine, iColumn);
    }

    static void vThrow(i8 iLine, i8 iColumn, eEC eErrorCode, Stz* szParamFormat, ...)
    {
      // Get the default error message for this error code
      s2K sErrMsg(aErrorMsg[eErrorCode]);

      // Form a string from the ParamFormat and any arguments
      s2K sParamStr;
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
      s2K sErrMsg(aErrorMsg[eErrorCode]);
      va_list vaArgs;

      // Print the parameters to a string
      s2K sParamStr;
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
      s2K sErrMsg(aErrorMsg[EC_NO_SUCH_TABLE2]);

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
      s2K sErrMsg(aErrorMsg[EC_AMBIGUOUS_TABLE]);

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
      s2K sErrMsg(aErrorMsg[EC_NO_SUCH_COLUMN]);

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
                                      Stz* szColumnName, QueryNodeList* pDataSourceList)
    {
      ASSERT(pDataSourceList->nItems() > 1);

      s2K sErrMsg(aErrorMsg[EC_AMBIGUOUS_COLUMN]);

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

      QueryNodeIter* pIter = pDataSourceList->pNewIter();
      while (pIter->bHasNext()) {
        QueryNode* pNode = pIter->pNext();

        s2K sBuf;
        DataSource::vPrintName(pNode, sBuf);
        sErrMsg.vCatSprint("\n  %s.%s"_Stz,
                           sBuf.szStz(),
                           szColumnName);
      }

      delete pIter;
      pDataSourceList->vRemoveAll();
      delete pDataSourceList;   // yes, delete the list we were passed

      throw NameValidationException(EC_AMBIGUOUS_COLUMN, sErrMsg.szStz());
    }

    static void vThrowAmbiguousUsingColumn(Stz* szUsingColName, Stz* szColRef1, Stz* szColRef2)
    {
      s2K sErrMsg(aErrorMsg[EC_AMBIGUOUS_USING_COLUMN]);

      sErrMsg.vCatSprint(" (%s). Could be one of:\n  %s\n  %s"_Stz, szUsingColName, szColRef1, szColRef2);

      throw NameValidationException(EC_AMBIGUOUS_USING_COLUMN, sErrMsg.szStz());
                                      }

    static void vThrowAmbiguousColumn(Stz* szColumnName, QueryNodeList* pDataSourceList,
                                      QueryNodeList* pProjectedColumnList, PlanNode* pRootNode)
    {
      ASSERT(szColumnName);
      ASSERT(pDataSourceList);
      ASSERT(pDataSourceList->nItems() >= 1);
      ASSERT(pProjectedColumnList);
      ASSERT(pProjectedColumnList->nItems() >= 1);

      s2K sErrMsg(aErrorMsg[EC_AMBIGUOUS_COLUMN]);

      sErrMsg.vCatSprint(": %s. Could be one of:"_Stz, szColumnName);

      QueryNodeIter* pDataSourceIter = pDataSourceList->pNewIter();
      while (pDataSourceIter->bHasNext()) {
        QueryNode* pNode = pDataSourceIter->pNext();

        s2K sBuf;
        DataSource::vPrintName(pNode, sBuf);
        sErrMsg.vCatSprint("\n  %s.%s"_Stz,
                           sBuf.szStz(),
                           szColumnName);
      }

      delete pDataSourceIter;
      pDataSourceList->vRemoveAll();
      delete pDataSourceList;   // yes, delete the list we were passed


      QueryNodeIter* pProjectedColIter = pProjectedColumnList->pNewIter();
      while (pProjectedColIter->bHasNext()) {
        QueryNode* pNode = pProjectedColIter->pNext();
        ASSERT(ProjectedColumn::bIsOfType(pNode));

        sErrMsg.vConcat("\n  "_Stz);
        ProjectedColumn::vDescribe(pNode, sErrMsg, pRootNode);
      }

      delete pProjectedColIter;
      pProjectedColumnList->vDeleteAll();
      delete pProjectedColumnList;   // yes, delete the list we were passed

      throw NameValidationException(EC_AMBIGUOUS_COLUMN, sErrMsg.szStz());
    }

    static void vThrowAmbiguousAlias(Stz* szAliasName, QueryNodeList* pProjectedColumnList, PlanNode* pRootNode)
    {
      ASSERT(szAliasName);
      ASSERT(pProjectedColumnList);
      ASSERT(pProjectedColumnList->nItems() > 1);

      s2K sErrMsg(aErrorMsg[EC_AMBIGUOUS_ALIAS]);

      sErrMsg.vCatSprint(": %s. Could be one of:"_Stz, szAliasName);

      QueryNodeIter* pIter = pProjectedColumnList->pNewIter();
      while (pIter->bHasNext()) {
        QueryNode* pNode = pIter->pNext();
        ASSERT(ProjectedColumn::bIsOfType(pNode));

        sErrMsg.vConcat("\n  "_Stz);
        ProjectedColumn::vDescribe(pNode, sErrMsg, pRootNode);
      }

      delete pIter;
      pProjectedColumnList->vRemoveAll();
      delete pProjectedColumnList;   // yes, delete the list we were passed

      throw NameValidationException(EC_AMBIGUOUS_ALIAS, sErrMsg.szStz());
    }

    static void vThrowUnresolvableColumnWildcard(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName) {
      s2K sErrMsg(aErrorMsg[EC_UNRESOLVED_COLUMN_WILDCARD]);

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
