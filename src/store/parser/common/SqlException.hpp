/*============================================================================*/
/* ParserException.hpp - exceptions that can occur while parsing a SQL stmt   */
/*============================================================================*/
#pragma once

#include <exception>

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
    static void vThrowUnknownTable(Stz* szTableName) {
      s256 sErrMsg;
      sErrMsg.vSprint("Unknown table name: %s"_Stz, szTableName);
      throw NameValidationException(sErrMsg.szStz());
    }

    static void vThrowUnknownColumn(Stz* szColumnName) {
      s256 sErrMsg;
      sErrMsg.vSprint("Unknown column name: %s"_Stz, szColumnName);
      throw NameValidationException(sErrMsg.szStz());
    }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
