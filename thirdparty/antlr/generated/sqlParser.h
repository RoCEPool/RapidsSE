
// Generated from /Users/alka/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"


namespace Parser {


class  sqlParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    ALL = 21, AND = 22, ANTI = 23, ANTLR = 24, ANY = 25, AS = 26, ASC = 27, 
    AVG = 28, BETWEEN = 29, BOTH = 30, BY = 31, CATALOG = 32, COLLATE = 33, 
    CORRESPONDING = 34, CREATE = 35, CROSS = 36, CURRENT_CATALOG = 37, CURRENT_SCHEMA = 38, 
    CURRENT_TIMESTAMP = 39, CURRENT_USER = 40, DATA = 41, DATE = 42, DELETE = 43, 
    DESC = 44, DETAIL = 45, DISTINCT = 46, DROP = 47, ESCAPE = 48, EXCEPT = 49, 
    EXPLAIN = 50, SQL_FALSE = 51, FEDERATION = 52, FOR = 53, FORMAT = 54, 
    FULL = 55, FROM = 56, GROUP = 57, HASHKEY = 58, HAVING = 59, IN = 60, 
    INDEX = 61, INNER = 62, INSERT = 63, INTERSECT = 64, INTO = 65, IS = 66, 
    JOIN = 67, KEY = 68, LEADING = 69, LEFT = 70, LIKE = 71, LIMIT = 72, 
    MAX = 73, MIN = 74, MONOTONIC = 75, NATURAL = 76, NO = 77, NODE = 78, 
    NOT = 79, NOW = 80, SQL_NULL = 81, OFFSET = 82, ON = 83, OR = 84, ORDER = 85, 
    OUTER = 86, OVER = 87, OVERLAY = 88, PARTITION = 89, PLACING = 90, PRETTY = 91, 
    PRIMARY = 92, RECURSIVE = 93, REFERENCE = 94, RIGHT = 95, ROW = 96, 
    SCHEMA = 97, SELECT = 98, SEMI = 99, SET = 100, STAGE = 101, STATS = 102, 
    STATISTICS = 103, SUM = 104, TABLE = 105, TO = 106, TOP = 107, TRAILING = 108, 
    TRUNCATE = 109, SQL_TRUE = 110, TYPE = 111, UNION = 112, UNIQUE = 113, 
    UPDATE = 114, USING = 115, VALUES = 116, WHERE = 117, WITH_NAMES = 118, 
    WITH_TYPES = 119, WITH = 120, XOR = 121, BOOLEAN = 122, INTEGER = 123, 
    DECIMAL = 124, FLOAT = 125, TIME = 126, TIMESTAMP = 127, INTERVAL = 128, 
    CHAR = 129, CHARACTER = 130, VARCHAR = 131, TEXT = 132, BINARY = 133, 
    VARBINARY = 134, BLOB = 135, VARYING = 136, BIT = 137, BITS = 138, BYTE = 139, 
    BYTES = 140, CHARS = 141, DIGITS = 142, CASE = 143, CAST = 144, COALESCE = 145, 
    CONCAT = 146, COUNT = 147, DECODE = 148, EXTRACT = 149, EXISTS = 150, 
    IF = 151, IFNULL = 152, NULLIF = 153, POSITION = 154, SUBSTR = 155, 
    SUBSTRING = 156, TRIM = 157, TYPEOF = 158, WHEN = 159, THEN = 160, ELSE = 161, 
    END = 162, DAY = 163, DAYS = 164, HOUR = 165, HOURS = 166, MINUTE = 167, 
    MINUTES = 168, MONTH = 169, MONTHS = 170, QUARTER = 171, QUARTERS = 172, 
    SECOND = 173, SECONDS = 174, WEEK = 175, WEEKS = 176, YEAR = 177, YEARS = 178, 
    BLOCK = 179, BLOOM = 180, BTREE = 181, HASH = 182, LOOP = 183, SKIPLIST = 184, 
    TREE = 185, VALUE = 186, IDENTIFIER = 187, PLAIN_IDENTIFIER = 188, BAKTIKD_IDENTIFIER = 189, 
    DUBLQUOTED_IDENTIFIER = 190, STRING_LITERAL = 191, SINGLQUOTED_STRING = 192, 
    DUBLQUOTED_STRING = 193, FLOAT_LITERAL = 194, DECIMAL_LITERAL = 195, 
    INTEGER_LITERAL = 196, HINT_START = 197, HINT_END = 198, SLASH_COMMENT = 199, 
    SQL_COMMENT = 200, BLOCK_COMMENT = 201, WHITESPACE = 202
  };

  enum {
    RuleStatement = 0, RuleStatementChoice = 1, RuleQueryStatement = 2, 
    RuleExplainStatement = 3, RuleExplainOptions = 4, RuleExplainStageList = 5, 
    RuleExplainRange = 6, RuleExplainRangeChoice = 7, RuleExplainFormat = 8, 
    RuleStatisticsStatement = 9, RuleQuery = 10, RuleQueryHint = 11, RuleHint = 12, 
    RuleSelectQuery = 13, RuleSelectExpr = 14, RuleSetQuery = 15, RuleSetExpr = 16, 
    RuleCorrClause = 17, RuleWithQuery = 18, RuleTableExpr = 19, RuleTableReference = 20, 
    RuleJoinType = 21, RuleExtendedJoinType = 22, RuleValuesExpr = 23, RuleInsertQuery = 24, 
    RuleDeleteQuery = 25, RuleUpdateQuery = 26, RuleCreateQuery = 27, RuleCreateTableQuery = 28, 
    RuleCreateIndexQuery = 29, RuleDropQuery = 30, RuleTruncateQuery = 31, 
    RuleTableDefinition = 32, RuleTablePartitioning = 33, RuleColumnDefinition = 34, 
    RuleColumnNameList = 35, RuleTableConstraint = 36, RuleIndexDefinition = 37, 
    RuleIndexMethod = 38, RuleDataType = 39, RuleSqlDataType = 40, RuleFixedLenChar = 41, 
    RuleVaryingLenChar = 42, RuleAsSubquery = 43, RuleSelectOrValues = 44, 
    RuleSelectList = 45, RuleSelectListItem = 46, RuleGroupByList = 47, 
    RuleGroupByListItem = 48, RuleOrderByList = 49, RuleOrderByListItem = 50, 
    RuleJoinUsingList = 51, RuleJoinUsingListItem = 52, RuleCorrList = 53, 
    RuleCorrListItem = 54, RuleColAliases = 55, RuleColAliasList = 56, RuleColAliasListItem = 57, 
    RulePropertyList = 58, RuleProperty = 59, RuleValueList = 60, RulePredicate = 61, 
    RuleJoinPredicate = 62, RuleCasePredicate = 63, RuleExpr = 64, RuleVariableName = 65, 
    RuleColumnRef = 66, RuleColumnWildcard = 67, RuleClassName = 68, RuleFunctionName = 69, 
    RuleMethodName = 70, RuleCatalogName = 71, RuleSchemaName = 72, RuleTableName = 73, 
    RuleTableAlias = 74, RuleColumnName = 75, RuleColumnAlias = 76, RuleCollationName = 77, 
    RuleIndexName = 78, RulePropertyName = 79, RuleHintName = 80, RuleExprHint = 81, 
    RulePropertyValue = 82, RuleHintValue = 83, RuleIdentifier = 84, RuleQualifier = 85, 
    RulePrecision = 86, RuleScale = 87, RuleSize = 88, RuleLiteral = 89, 
    RuleSimpleLiteral = 90, RuleBooleanLiteral = 91, RuleIntegerLiteral = 92, 
    RuleSignedIntegerLiteral = 93, RuleDecimalLiteral = 94, RuleSignedDecimalLiteral = 95, 
    RuleFloatLiteral = 96, RuleSignedFloatLiteral = 97, RuleDateLiteral = 98, 
    RuleTimeLiteral = 99, RuleTimestampLiteral = 100, RuleStringLiteral = 101, 
    RuleIntervalLiteral = 102, RuleLooseInterval = 103, RuleSql92CalendarInterval = 104, 
    RuleSql92TimeInterval = 105, RuleNonReservedWord = 106
  };

  sqlParser(antlr4::TokenStream *input);
  ~sqlParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class StatementContext;
  class StatementChoiceContext;
  class QueryStatementContext;
  class ExplainStatementContext;
  class ExplainOptionsContext;
  class ExplainStageListContext;
  class ExplainRangeContext;
  class ExplainRangeChoiceContext;
  class ExplainFormatContext;
  class StatisticsStatementContext;
  class QueryContext;
  class QueryHintContext;
  class HintContext;
  class SelectQueryContext;
  class SelectExprContext;
  class SetQueryContext;
  class SetExprContext;
  class CorrClauseContext;
  class WithQueryContext;
  class TableExprContext;
  class TableReferenceContext;
  class JoinTypeContext;
  class ExtendedJoinTypeContext;
  class ValuesExprContext;
  class InsertQueryContext;
  class DeleteQueryContext;
  class UpdateQueryContext;
  class CreateQueryContext;
  class CreateTableQueryContext;
  class CreateIndexQueryContext;
  class DropQueryContext;
  class TruncateQueryContext;
  class TableDefinitionContext;
  class TablePartitioningContext;
  class ColumnDefinitionContext;
  class ColumnNameListContext;
  class TableConstraintContext;
  class IndexDefinitionContext;
  class IndexMethodContext;
  class DataTypeContext;
  class SqlDataTypeContext;
  class FixedLenCharContext;
  class VaryingLenCharContext;
  class AsSubqueryContext;
  class SelectOrValuesContext;
  class SelectListContext;
  class SelectListItemContext;
  class GroupByListContext;
  class GroupByListItemContext;
  class OrderByListContext;
  class OrderByListItemContext;
  class JoinUsingListContext;
  class JoinUsingListItemContext;
  class CorrListContext;
  class CorrListItemContext;
  class ColAliasesContext;
  class ColAliasListContext;
  class ColAliasListItemContext;
  class PropertyListContext;
  class PropertyContext;
  class ValueListContext;
  class PredicateContext;
  class JoinPredicateContext;
  class CasePredicateContext;
  class ExprContext;
  class VariableNameContext;
  class ColumnRefContext;
  class ColumnWildcardContext;
  class ClassNameContext;
  class FunctionNameContext;
  class MethodNameContext;
  class CatalogNameContext;
  class SchemaNameContext;
  class TableNameContext;
  class TableAliasContext;
  class ColumnNameContext;
  class ColumnAliasContext;
  class CollationNameContext;
  class IndexNameContext;
  class PropertyNameContext;
  class HintNameContext;
  class ExprHintContext;
  class PropertyValueContext;
  class HintValueContext;
  class IdentifierContext;
  class QualifierContext;
  class PrecisionContext;
  class ScaleContext;
  class SizeContext;
  class LiteralContext;
  class SimpleLiteralContext;
  class BooleanLiteralContext;
  class IntegerLiteralContext;
  class SignedIntegerLiteralContext;
  class DecimalLiteralContext;
  class SignedDecimalLiteralContext;
  class FloatLiteralContext;
  class SignedFloatLiteralContext;
  class DateLiteralContext;
  class TimeLiteralContext;
  class TimestampLiteralContext;
  class StringLiteralContext;
  class IntervalLiteralContext;
  class LooseIntervalContext;
  class Sql92CalendarIntervalContext;
  class Sql92TimeIntervalContext;
  class NonReservedWordContext; 

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StatementChoiceContext *statementChoice();
    antlr4::tree::TerminalNode *ANTLR();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementContext* statement();

  class  StatementChoiceContext : public antlr4::ParserRuleContext {
  public:
    StatementChoiceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    QueryStatementContext *queryStatement();
    ExplainStatementContext *explainStatement();
    StatisticsStatementContext *statisticsStatement();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementChoiceContext* statementChoice();

  class  QueryStatementContext : public antlr4::ParserRuleContext {
  public:
    QueryStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    QueryContext *query();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QueryStatementContext* queryStatement();

  class  ExplainStatementContext : public antlr4::ParserRuleContext {
  public:
    ExplainStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXPLAIN();
    StatementChoiceContext *statementChoice();
    std::vector<QueryHintContext *> queryHint();
    QueryHintContext* queryHint(size_t i);
    std::vector<ExplainOptionsContext *> explainOptions();
    ExplainOptionsContext* explainOptions(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainStatementContext* explainStatement();

  class  ExplainOptionsContext : public antlr4::ParserRuleContext {
  public:
    ExplainOptionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExplainStageListContext *explainStageList();
    ExplainFormatContext *explainFormat();
    antlr4::tree::TerminalNode *WITH_NAMES();
    antlr4::tree::TerminalNode *WITH_TYPES();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainOptionsContext* explainOptions();

  class  ExplainStageListContext : public antlr4::ParserRuleContext {
  public:
    ExplainStageListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STAGE();
    std::vector<ExplainRangeContext *> explainRange();
    ExplainRangeContext* explainRange(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainStageListContext* explainStageList();

  class  ExplainRangeContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::ExplainRangeChoiceContext *begin = nullptr;;
    sqlParser::ExplainRangeChoiceContext *end = nullptr;;
    ExplainRangeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExplainRangeChoiceContext *> explainRangeChoice();
    ExplainRangeChoiceContext* explainRangeChoice(size_t i);
    antlr4::tree::TerminalNode *TO();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainRangeContext* explainRange();

  class  ExplainRangeChoiceContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *all = nullptr;;
    antlr4::Token *num = nullptr;;
    sqlParser::IdentifierContext *id = nullptr;;
    ExplainRangeChoiceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *INTEGER_LITERAL();
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainRangeChoiceContext* explainRangeChoice();

  class  ExplainFormatContext : public antlr4::ParserRuleContext {
  public:
    ExplainFormatContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FORMAT();
    antlr4::tree::TerminalNode *PRETTY();
    antlr4::tree::TerminalNode *ROW();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainFormatContext* explainFormat();

  class  StatisticsStatementContext : public antlr4::ParserRuleContext {
  public:
    StatisticsStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StatementChoiceContext *statementChoice();
    antlr4::tree::TerminalNode *STATS();
    antlr4::tree::TerminalNode *STATISTICS();
    std::vector<QueryHintContext *> queryHint();
    QueryHintContext* queryHint(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatisticsStatementContext* statisticsStatement();

  class  QueryContext : public antlr4::ParserRuleContext {
  public:
    QueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectQueryContext *selectQuery();
    SetQueryContext *setQuery();
    InsertQueryContext *insertQuery();
    DeleteQueryContext *deleteQuery();
    UpdateQueryContext *updateQuery();
    CreateQueryContext *createQuery();
    DropQueryContext *dropQuery();
    TruncateQueryContext *truncateQuery();
    std::vector<QueryHintContext *> queryHint();
    QueryHintContext* queryHint(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QueryContext* query();

  class  QueryHintContext : public antlr4::ParserRuleContext {
  public:
    QueryHintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HINT_START();
    antlr4::tree::TerminalNode *HINT_END();
    std::vector<HintContext *> hint();
    HintContext* hint(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QueryHintContext* queryHint();

  class  HintContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::HintNameContext *name = nullptr;;
    sqlParser::ExprHintContext *exprName = nullptr;;
    HintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    HintNameContext *hintName();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    ExprHintContext *exprHint();
    std::vector<HintValueContext *> hintValue();
    HintValueContext* hintValue(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HintContext* hint();

  class  SelectQueryContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::OrderByListContext *orderBy = nullptr;;
    antlr4::Token *limit = nullptr;;
    antlr4::Token *offset = nullptr;;
    SelectQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectExprContext *selectExpr();
    antlr4::tree::TerminalNode *WITH();
    std::vector<WithQueryContext *> withQuery();
    WithQueryContext* withQuery(size_t i);
    antlr4::tree::TerminalNode *ORDER();
    antlr4::tree::TerminalNode *BY();
    antlr4::tree::TerminalNode *LIMIT();
    OrderByListContext *orderByList();
    std::vector<antlr4::tree::TerminalNode *> INTEGER_LITERAL();
    antlr4::tree::TerminalNode* INTEGER_LITERAL(size_t i);
    antlr4::tree::TerminalNode *OFFSET();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectQueryContext* selectQuery();

  class  SelectExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *distinct = nullptr;;
    sqlParser::PredicateContext *where = nullptr;;
    sqlParser::GroupByListContext *groupBy = nullptr;;
    sqlParser::PredicateContext *having = nullptr;;
    SelectExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectListContext *selectList();
    antlr4::tree::TerminalNode *SELECT();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *FROM();
    TableExprContext *tableExpr();
    antlr4::tree::TerminalNode *WHERE();
    antlr4::tree::TerminalNode *GROUP();
    antlr4::tree::TerminalNode *BY();
    antlr4::tree::TerminalNode *HAVING();
    std::vector<PredicateContext *> predicate();
    PredicateContext* predicate(size_t i);
    GroupByListContext *groupByList();
    antlr4::tree::TerminalNode *DISTINCT();
    SelectExprContext *selectExpr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectExprContext* selectExpr();

  class  SetQueryContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::OrderByListContext *orderBy = nullptr;;
    antlr4::Token *limit = nullptr;;
    antlr4::Token *offset = nullptr;;
    SetQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SetExprContext *setExpr();
    antlr4::tree::TerminalNode *WITH();
    std::vector<WithQueryContext *> withQuery();
    WithQueryContext* withQuery(size_t i);
    antlr4::tree::TerminalNode *ORDER();
    antlr4::tree::TerminalNode *BY();
    antlr4::tree::TerminalNode *LIMIT();
    OrderByListContext *orderByList();
    std::vector<antlr4::tree::TerminalNode *> INTEGER_LITERAL();
    antlr4::tree::TerminalNode* INTEGER_LITERAL(size_t i);
    antlr4::tree::TerminalNode *OFFSET();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetQueryContext* setQuery();

  class  SetExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;;
    antlr4::Token *all = nullptr;;
    SetExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectExprContext *selectExpr();
    std::vector<SetExprContext *> setExpr();
    SetExprContext* setExpr(size_t i);
    antlr4::tree::TerminalNode *UNION();
    antlr4::tree::TerminalNode *EXCEPT();
    antlr4::tree::TerminalNode *INTERSECT();
    antlr4::tree::TerminalNode *DISTINCT();
    CorrClauseContext *corrClause();
    antlr4::tree::TerminalNode *ALL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetExprContext* setExpr();
  SetExprContext* setExpr(int precedence);
  class  CorrClauseContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *corr = nullptr;;
    CorrClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CORRESPONDING();
    antlr4::tree::TerminalNode *BY();
    CorrListContext *corrList();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CorrClauseContext* corrClause();

  class  WithQueryContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TableNameContext *name = nullptr;;
    WithQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AS();
    QueryContext *query();
    TableNameContext *tableName();
    antlr4::tree::TerminalNode *RECURSIVE();
    ColAliasesContext *colAliases();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WithQueryContext* withQuery();

  class  TableExprContext : public antlr4::ParserRuleContext {
  public:
    TableExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    TableExprContext() = default;
    void copyFrom(TableExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  SubqueryContext : public TableExprContext {
  public:
    SubqueryContext(TableExprContext *ctx);

    sqlParser::TableAliasContext *as = nullptr;
    QueryContext *query();
    TableAliasContext *tableAlias();
    antlr4::tree::TerminalNode *AS();
    ColAliasesContext *colAliases();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NaturalJoinContext : public TableExprContext {
  public:
    NaturalJoinContext(TableExprContext *ctx);

    sqlParser::JoinTypeContext *type = nullptr;
    std::vector<TableExprContext *> tableExpr();
    TableExprContext* tableExpr(size_t i);
    antlr4::tree::TerminalNode *NATURAL();
    antlr4::tree::TerminalNode *JOIN();
    std::vector<QueryHintContext *> queryHint();
    QueryHintContext* queryHint(size_t i);
    JoinTypeContext *joinType();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  JoinUsingContext : public TableExprContext {
  public:
    JoinUsingContext(TableExprContext *ctx);

    sqlParser::ExtendedJoinTypeContext *type = nullptr;
    std::vector<TableExprContext *> tableExpr();
    TableExprContext* tableExpr(size_t i);
    antlr4::tree::TerminalNode *JOIN();
    antlr4::tree::TerminalNode *USING();
    JoinUsingListContext *joinUsingList();
    std::vector<QueryHintContext *> queryHint();
    QueryHintContext* queryHint(size_t i);
    ExtendedJoinTypeContext *extendedJoinType();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  JoinOnContext : public TableExprContext {
  public:
    JoinOnContext(TableExprContext *ctx);

    sqlParser::ExtendedJoinTypeContext *type = nullptr;
    std::vector<TableExprContext *> tableExpr();
    TableExprContext* tableExpr(size_t i);
    antlr4::tree::TerminalNode *JOIN();
    antlr4::tree::TerminalNode *ON();
    JoinPredicateContext *joinPredicate();
    std::vector<QueryHintContext *> queryHint();
    QueryHintContext* queryHint(size_t i);
    ExtendedJoinTypeContext *extendedJoinType();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  FetchContext : public TableExprContext {
  public:
    FetchContext(TableExprContext *ctx);

    sqlParser::TableReferenceContext *ref = nullptr;
    sqlParser::TableAliasContext *alias = nullptr;
    TableReferenceContext *tableReference();
    TableAliasContext *tableAlias();
    antlr4::tree::TerminalNode *AS();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ValuesSubqueryContext : public TableExprContext {
  public:
    ValuesSubqueryContext(TableExprContext *ctx);

    sqlParser::TableAliasContext *as = nullptr;
    ValuesExprContext *valuesExpr();
    TableAliasContext *tableAlias();
    antlr4::tree::TerminalNode *AS();
    ColAliasesContext *colAliases();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  CrossJoinContext : public TableExprContext {
  public:
    CrossJoinContext(TableExprContext *ctx);

    std::vector<TableExprContext *> tableExpr();
    TableExprContext* tableExpr(size_t i);
    std::vector<QueryHintContext *> queryHint();
    QueryHintContext* queryHint(size_t i);
    antlr4::tree::TerminalNode *CROSS();
    antlr4::tree::TerminalNode *JOIN();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  TableExprContext* tableExpr();
  TableExprContext* tableExpr(int precedence);
  class  TableReferenceContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::CatalogNameContext *catalog = nullptr;;
    sqlParser::SchemaNameContext *schema = nullptr;;
    sqlParser::TableNameContext *table = nullptr;;
    TableReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TableNameContext *tableName();
    SchemaNameContext *schemaName();
    CatalogNameContext *catalogName();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableReferenceContext* tableReference();

  class  JoinTypeContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *outer = nullptr;;
    JoinTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INNER();
    antlr4::tree::TerminalNode *LEFT();
    antlr4::tree::TerminalNode *RIGHT();
    antlr4::tree::TerminalNode *FULL();
    antlr4::tree::TerminalNode *OUTER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  JoinTypeContext* joinType();

  class  ExtendedJoinTypeContext : public antlr4::ParserRuleContext {
  public:
    ExtendedJoinTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    JoinTypeContext *joinType();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *ANTI();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtendedJoinTypeContext* extendedJoinType();

  class  ValuesExprContext : public antlr4::ParserRuleContext {
  public:
    ValuesExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VALUES();
    std::vector<ValueListContext *> valueList();
    ValueListContext* valueList(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValuesExprContext* valuesExpr();

  class  InsertQueryContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TableReferenceContext *ref = nullptr;;
    InsertQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INSERT();
    TableReferenceContext *tableReference();
    ValuesExprContext *valuesExpr();
    SelectQueryContext *selectQuery();
    SetQueryContext *setQuery();
    antlr4::tree::TerminalNode *WITH();
    std::vector<WithQueryContext *> withQuery();
    WithQueryContext* withQuery(size_t i);
    antlr4::tree::TerminalNode *INTO();
    std::vector<ColumnNameContext *> columnName();
    ColumnNameContext* columnName(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertQueryContext* insertQuery();

  class  DeleteQueryContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TableReferenceContext *ref = nullptr;;
    DeleteQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DELETE();
    TableReferenceContext *tableReference();
    antlr4::tree::TerminalNode *WITH();
    std::vector<WithQueryContext *> withQuery();
    WithQueryContext* withQuery(size_t i);
    std::vector<antlr4::tree::TerminalNode *> FROM();
    antlr4::tree::TerminalNode* FROM(size_t i);
    antlr4::tree::TerminalNode *WHERE();
    PredicateContext *predicate();
    TableExprContext *tableExpr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeleteQueryContext* deleteQuery();

  class  UpdateQueryContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TableReferenceContext *ref = nullptr;;
    UpdateQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UPDATE();
    antlr4::tree::TerminalNode *SET();
    std::vector<ColumnNameContext *> columnName();
    ColumnNameContext* columnName(size_t i);
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    TableReferenceContext *tableReference();
    antlr4::tree::TerminalNode *WITH();
    std::vector<WithQueryContext *> withQuery();
    WithQueryContext* withQuery(size_t i);
    antlr4::tree::TerminalNode *WHERE();
    PredicateContext *predicate();
    antlr4::tree::TerminalNode *FROM();
    TableExprContext *tableExpr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UpdateQueryContext* updateQuery();

  class  CreateQueryContext : public antlr4::ParserRuleContext {
  public:
    CreateQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CreateTableQueryContext *createTableQuery();
    CreateIndexQueryContext *createIndexQuery();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateQueryContext* createQuery();

  class  CreateTableQueryContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *reference = nullptr;;
    sqlParser::TableReferenceContext *ref = nullptr;;
    antlr4::Token *noData = nullptr;;
    CreateTableQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *TABLE();
    TableReferenceContext *tableReference();
    TableDefinitionContext *tableDefinition();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *EXISTS();
    antlr4::tree::TerminalNode *REFERENCE();
    AsSubqueryContext *asSubquery();
    ColumnNameListContext *columnNameList();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *DATA();
    antlr4::tree::TerminalNode *NO();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateTableQueryContext* createTableQuery();

  class  CreateIndexQueryContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IndexNameContext *idxName = nullptr;;
    sqlParser::TableReferenceContext *tblName = nullptr;;
    sqlParser::IndexMethodContext *method = nullptr;;
    sqlParser::PropertyListContext *props = nullptr;;
    CreateIndexQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *ON();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *INDEX();
    antlr4::tree::TerminalNode *KEY();
    IndexNameContext *indexName();
    TableReferenceContext *tableReference();
    antlr4::tree::TerminalNode *UNIQUE();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *EXISTS();
    antlr4::tree::TerminalNode *USING();
    antlr4::tree::TerminalNode *WITH();
    IndexMethodContext *indexMethod();
    PropertyListContext *propertyList();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateIndexQueryContext* createIndexQuery();

  class  DropQueryContext : public antlr4::ParserRuleContext {
  public:
    DropQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    DropQueryContext() = default;
    void copyFrom(DropQueryContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  DropTableContext : public DropQueryContext {
  public:
    DropTableContext(DropQueryContext *ctx);

    sqlParser::TableReferenceContext *name = nullptr;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *TABLE();
    TableReferenceContext *tableReference();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DropIndexContext : public DropQueryContext {
  public:
    DropIndexContext(DropQueryContext *ctx);

    sqlParser::IndexNameContext *idxName = nullptr;
    sqlParser::TableReferenceContext *tblName = nullptr;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *INDEX();
    antlr4::tree::TerminalNode *KEY();
    IndexNameContext *indexName();
    TableReferenceContext *tableReference();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *EXISTS();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  DropQueryContext* dropQuery();

  class  TruncateQueryContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TableReferenceContext *name = nullptr;;
    TruncateQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TRUNCATE();
    TableReferenceContext *tableReference();
    antlr4::tree::TerminalNode *TABLE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TruncateQueryContext* truncateQuery();

  class  TableDefinitionContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::PropertyListContext *props = nullptr;;
    TableDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColumnDefinitionContext *> columnDefinition();
    ColumnDefinitionContext* columnDefinition(size_t i);
    std::vector<TableConstraintContext *> tableConstraint();
    TableConstraintContext* tableConstraint(size_t i);
    std::vector<IndexDefinitionContext *> indexDefinition();
    IndexDefinitionContext* indexDefinition(size_t i);
    antlr4::tree::TerminalNode *REFERENCE();
    TablePartitioningContext *tablePartitioning();
    antlr4::tree::TerminalNode *WITH();
    PropertyListContext *propertyList();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableDefinitionContext* tableDefinition();

  class  TablePartitioningContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *method = nullptr;;
    TablePartitioningContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PARTITION();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *BY();
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *VALUE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TablePartitioningContext* tablePartitioning();

  class  ColumnDefinitionContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::ColumnNameContext *name = nullptr;;
    sqlParser::DataTypeContext *type = nullptr;;
    antlr4::Token *notNullable = nullptr;;
    antlr4::Token *nullable = nullptr;;
    antlr4::Token *primary = nullptr;;
    antlr4::Token *unique = nullptr;;
    antlr4::Token *mono = nullptr;;
    sqlParser::PropertyListContext *props = nullptr;;
    ColumnDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnNameContext *columnName();
    DataTypeContext *dataType();
    antlr4::tree::TerminalNode *WITH();
    PropertyListContext *propertyList();
    std::vector<antlr4::tree::TerminalNode *> KEY();
    antlr4::tree::TerminalNode* KEY(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SQL_NULL();
    antlr4::tree::TerminalNode* SQL_NULL(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PRIMARY();
    antlr4::tree::TerminalNode* PRIMARY(size_t i);
    std::vector<antlr4::tree::TerminalNode *> UNIQUE();
    antlr4::tree::TerminalNode* UNIQUE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MONOTONIC();
    antlr4::tree::TerminalNode* MONOTONIC(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NOT();
    antlr4::tree::TerminalNode* NOT(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnDefinitionContext* columnDefinition();

  class  ColumnNameListContext : public antlr4::ParserRuleContext {
  public:
    ColumnNameListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColumnNameContext *> columnName();
    ColumnNameContext* columnName(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnNameListContext* columnNameList();

  class  TableConstraintContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *primary = nullptr;;
    antlr4::Token *unique = nullptr;;
    antlr4::Token *mono = nullptr;;
    TableConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *PRIMARY();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *MONOTONIC();
    antlr4::tree::TerminalNode *UNIQUE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableConstraintContext* tableConstraint();

  class  IndexDefinitionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *unique = nullptr;;
    sqlParser::IndexNameContext *name = nullptr;;
    sqlParser::IndexMethodContext *method = nullptr;;
    sqlParser::PropertyListContext *props = nullptr;;
    IndexDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INDEX();
    antlr4::tree::TerminalNode *KEY();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *USING();
    antlr4::tree::TerminalNode *WITH();
    antlr4::tree::TerminalNode *UNIQUE();
    IndexNameContext *indexName();
    IndexMethodContext *indexMethod();
    PropertyListContext *propertyList();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IndexDefinitionContext* indexDefinition();

  class  IndexMethodContext : public antlr4::ParserRuleContext {
  public:
    IndexMethodContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *TREE();
    antlr4::tree::TerminalNode *BTREE();
    antlr4::tree::TerminalNode *SKIPLIST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IndexMethodContext* indexMethod();

  class  DataTypeContext : public antlr4::ParserRuleContext {
  public:
    DataTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SqlDataTypeContext *sqlDataType();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DataTypeContext* dataType();

  class  SqlDataTypeContext : public antlr4::ParserRuleContext {
  public:
    SqlDataTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BOOLEAN();
    antlr4::tree::TerminalNode *INTEGER();
    PrecisionContext *precision();
    antlr4::tree::TerminalNode *BITS();
    antlr4::tree::TerminalNode *BYTES();
    std::vector<antlr4::tree::TerminalNode *> DIGITS();
    antlr4::tree::TerminalNode* DIGITS(size_t i);
    antlr4::tree::TerminalNode *DECIMAL();
    ScaleContext *scale();
    antlr4::tree::TerminalNode *FLOAT();
    antlr4::tree::TerminalNode *DATE();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *TIMESTAMP();
    FixedLenCharContext *fixedLenChar();
    VaryingLenCharContext *varyingLenChar();
    antlr4::tree::TerminalNode *BINARY();
    SizeContext *size();
    antlr4::tree::TerminalNode *BYTE();
    antlr4::tree::TerminalNode *VARBINARY();
    antlr4::tree::TerminalNode *BLOB();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SqlDataTypeContext* sqlDataType();

  class  FixedLenCharContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::CollationNameContext *collate = nullptr;;
    FixedLenCharContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SizeContext *size();
    std::vector<antlr4::tree::TerminalNode *> CHAR();
    antlr4::tree::TerminalNode* CHAR(size_t i);
    antlr4::tree::TerminalNode *CHARACTER();
    antlr4::tree::TerminalNode *COLLATE();
    antlr4::tree::TerminalNode *BYTE();
    antlr4::tree::TerminalNode *BYTES();
    antlr4::tree::TerminalNode *CHARS();
    CollationNameContext *collationName();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedLenCharContext* fixedLenChar();

  class  VaryingLenCharContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::CollationNameContext *collate = nullptr;;
    VaryingLenCharContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TEXT();
    antlr4::tree::TerminalNode *COLLATE();
    antlr4::tree::TerminalNode *VARCHAR();
    CollationNameContext *collationName();
    SizeContext *size();
    std::vector<antlr4::tree::TerminalNode *> CHAR();
    antlr4::tree::TerminalNode* CHAR(size_t i);
    antlr4::tree::TerminalNode *CHARACTER();
    antlr4::tree::TerminalNode *VARYING();
    antlr4::tree::TerminalNode *BYTE();
    antlr4::tree::TerminalNode *BYTES();
    antlr4::tree::TerminalNode *CHARS();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VaryingLenCharContext* varyingLenChar();

  class  AsSubqueryContext : public antlr4::ParserRuleContext {
  public:
    AsSubqueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectOrValuesContext *selectOrValues();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsSubqueryContext* asSubquery();

  class  SelectOrValuesContext : public antlr4::ParserRuleContext {
  public:
    SelectOrValuesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectOrValuesContext *selectOrValues();
    std::vector<QueryHintContext *> queryHint();
    QueryHintContext* queryHint(size_t i);
    SelectQueryContext *selectQuery();
    ValuesExprContext *valuesExpr();
    SetQueryContext *setQuery();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectOrValuesContext* selectOrValues();

  class  SelectListContext : public antlr4::ParserRuleContext {
  public:
    SelectListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SelectListItemContext *> selectListItem();
    SelectListItemContext* selectListItem(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectListContext* selectList();

  class  SelectListItemContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *as = nullptr;;
    sqlParser::ColumnWildcardContext *wild = nullptr;;
    SelectListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *AS();
    ColumnWildcardContext *columnWildcard();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectListItemContext* selectListItem();

  class  GroupByListContext : public antlr4::ParserRuleContext {
  public:
    GroupByListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<GroupByListItemContext *> groupByListItem();
    GroupByListItemContext* groupByListItem(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupByListContext* groupByList();

  class  GroupByListItemContext : public antlr4::ParserRuleContext {
  public:
    GroupByListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupByListItemContext* groupByListItem();

  class  OrderByListContext : public antlr4::ParserRuleContext {
  public:
    OrderByListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<OrderByListItemContext *> orderByListItem();
    OrderByListItemContext* orderByListItem(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrderByListContext* orderByList();

  class  OrderByListItemContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *desc = nullptr;;
    OrderByListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();
    antlr4::tree::TerminalNode *ASC();
    antlr4::tree::TerminalNode *DESC();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrderByListItemContext* orderByListItem();

  class  JoinUsingListContext : public antlr4::ParserRuleContext {
  public:
    JoinUsingListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<JoinUsingListItemContext *> joinUsingListItem();
    JoinUsingListItemContext* joinUsingListItem(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  JoinUsingListContext* joinUsingList();

  class  JoinUsingListItemContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    JoinUsingListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  JoinUsingListItemContext* joinUsingListItem();

  class  CorrListContext : public antlr4::ParserRuleContext {
  public:
    CorrListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<CorrListItemContext *> corrListItem();
    CorrListItemContext* corrListItem(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CorrListContext* corrList();

  class  CorrListItemContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    CorrListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CorrListItemContext* corrListItem();

  class  ColAliasesContext : public antlr4::ParserRuleContext {
  public:
    ColAliasesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColAliasListContext *colAliasList();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColAliasesContext* colAliases();

  class  ColAliasListContext : public antlr4::ParserRuleContext {
  public:
    ColAliasListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColAliasListItemContext *> colAliasListItem();
    ColAliasListItemContext* colAliasListItem(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColAliasListContext* colAliasList();

  class  ColAliasListItemContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    ColAliasListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColAliasListItemContext* colAliasListItem();

  class  PropertyListContext : public antlr4::ParserRuleContext {
  public:
    PropertyListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PropertyContext *> property();
    PropertyContext* property(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyListContext* propertyList();

  class  PropertyContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::PropertyNameContext *name = nullptr;;
    sqlParser::PropertyValueContext *value = nullptr;;
    PropertyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyNameContext *propertyName();
    PropertyValueContext *propertyValue();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyContext* property();

  class  ValueListContext : public antlr4::ParserRuleContext {
  public:
    ValueListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueListContext* valueList();

  class  PredicateContext : public antlr4::ParserRuleContext {
  public:
    PredicateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PredicateContext* predicate();

  class  JoinPredicateContext : public antlr4::ParserRuleContext {
  public:
    JoinPredicateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  JoinPredicateContext* joinPredicate();

  class  CasePredicateContext : public antlr4::ParserRuleContext {
  public:
    CasePredicateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CasePredicateContext* casePredicate();

  class  ExprContext : public antlr4::ParserRuleContext {
  public:
    ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExprContext() = default;
    void copyFrom(ExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  CountAllContext : public ExprContext {
  public:
    CountAllContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *ALL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExtractFuncContext : public ExprContext {
  public:
    ExtractFuncContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *EXTRACT();
    antlr4::tree::TerminalNode *FROM();
    ExprContext *expr();
    antlr4::tree::TerminalNode *SECOND();
    antlr4::tree::TerminalNode *MINUTE();
    antlr4::tree::TerminalNode *HOUR();
    antlr4::tree::TerminalNode *DAY();
    antlr4::tree::TerminalNode *MONTH();
    antlr4::tree::TerminalNode *YEAR();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ScalarFuncContext : public ExprContext {
  public:
    ScalarFuncContext(ExprContext *ctx);

    sqlParser::FunctionNameContext *func = nullptr;
    FunctionNameContext *functionName();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  UnaryOpContext : public ExprContext {
  public:
    UnaryOpContext(ExprContext *ctx);

    antlr4::Token *op = nullptr;
    ExprContext *expr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  CoalesceOpContext : public ExprContext {
  public:
    CoalesceOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *COALESCE();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  VarRefContext : public ExprContext {
  public:
    VarRefContext(ExprContext *ctx);

    VariableNameContext *variableName();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PositionFuncContext : public ExprContext {
  public:
    PositionFuncContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *POSITION();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *IN();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ColRefContext : public ExprContext {
  public:
    ColRefContext(ExprContext *ctx);

    ColumnRefContext *columnRef();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  CaseOpContext : public ExprContext {
  public:
    CaseOpContext(ExprContext *ctx);

    sqlParser::ExprContext *caseExpr = nullptr;
    sqlParser::ExprContext *elseExpr = nullptr;
    antlr4::tree::TerminalNode *CASE();
    antlr4::tree::TerminalNode *END();
    antlr4::tree::TerminalNode *ELSE();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> WHEN();
    antlr4::tree::TerminalNode* WHEN(size_t i);
    std::vector<CasePredicateContext *> casePredicate();
    CasePredicateContext* casePredicate(size_t i);
    std::vector<antlr4::tree::TerminalNode *> THEN();
    antlr4::tree::TerminalNode* THEN(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LiteralValContext : public ExprContext {
  public:
    LiteralValContext(ExprContext *ctx);

    LiteralContext *literal();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  OverlayFuncContext : public ExprContext {
  public:
    OverlayFuncContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *OVERLAY();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *PLACING();
    antlr4::tree::TerminalNode *FROM();
    antlr4::tree::TerminalNode *FOR();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IfNullOpContext : public ExprContext {
  public:
    IfNullOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *IFNULL();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SubqueryExistsOpContext : public ExprContext {
  public:
    SubqueryExistsOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *EXISTS();
    QueryContext *query();
    antlr4::tree::TerminalNode *NOT();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IsNullOpContext : public ExprContext {
  public:
    IsNullOpContext(ExprContext *ctx);

    ExprContext *expr();
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *SQL_NULL();
    antlr4::tree::TerminalNode *NOT();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  InOpContext : public ExprContext {
  public:
    InOpContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *IN();
    antlr4::tree::TerminalNode *NOT();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TrimFuncContext : public ExprContext {
  public:
    TrimFuncContext(ExprContext *ctx);

    sqlParser::ExprContext *trimExpr = nullptr;
    antlr4::tree::TerminalNode *TRIM();
    antlr4::tree::TerminalNode *FROM();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LEADING();
    antlr4::tree::TerminalNode *TRAILING();
    antlr4::tree::TerminalNode *BOTH();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LikeOpContext : public ExprContext {
  public:
    LikeOpContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LIKE();
    antlr4::tree::TerminalNode *NOT();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TypeOfOpContext : public ExprContext {
  public:
    TypeOfOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *TYPEOF();
    ExprContext *expr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NullIfOpContext : public ExprContext {
  public:
    NullIfOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *NULLIF();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  InlineCastContext : public ExprContext {
  public:
    InlineCastContext(ExprContext *ctx);

    DataTypeContext *dataType();
    ExprContext *expr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BinaryOpContext : public ExprContext {
  public:
    BinaryOpContext(ExprContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BetweenOpContext : public ExprContext {
  public:
    BetweenOpContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *BETWEEN();
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *NOT();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AggFuncContext : public ExprContext {
  public:
    AggFuncContext(ExprContext *ctx);

    antlr4::Token *func = nullptr;
    antlr4::Token *distinct = nullptr;
    ExprContext *expr();
    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *DISTINCT();
    antlr4::tree::TerminalNode *SUM();
    antlr4::tree::TerminalNode *AVG();
    antlr4::tree::TerminalNode *MIN();
    antlr4::tree::TerminalNode *MAX();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DecodeOpContext : public ExprContext {
  public:
    DecodeOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *DECODE();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ConcatOpContext : public ExprContext {
  public:
    ConcatOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *CONCAT();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SubqueryExprContext : public ExprContext {
  public:
    SubqueryExprContext(ExprContext *ctx);

    QueryContext *query();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IfOpContext : public ExprContext {
  public:
    IfOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *IF();
    CasePredicateContext *casePredicate();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LikeEscapeOpContext : public ExprContext {
  public:
    LikeEscapeOpContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LIKE();
    antlr4::tree::TerminalNode *ESCAPE();
    antlr4::tree::TerminalNode *NOT();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SubStringFuncContext : public ExprContext {
  public:
    SubStringFuncContext(ExprContext *ctx);

    sqlParser::ExprContext *src = nullptr;
    sqlParser::ExprContext *start = nullptr;
    sqlParser::ExprContext *length = nullptr;
    antlr4::tree::TerminalNode *SUBSTRING();
    antlr4::tree::TerminalNode *SUBSTR();
    antlr4::tree::TerminalNode *FROM();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *FOR();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SubqueryInOpContext : public ExprContext {
  public:
    SubqueryInOpContext(ExprContext *ctx);

    ExprContext *expr();
    antlr4::tree::TerminalNode *IN();
    QueryContext *query();
    antlr4::tree::TerminalNode *NOT();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LogicOpContext : public ExprContext {
  public:
    LogicOpContext(ExprContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *OR();
    antlr4::tree::TerminalNode *XOR();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  CastOpContext : public ExprContext {
  public:
    CastOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *CAST();
    ExprContext *expr();
    DataTypeContext *dataType();
    antlr4::tree::TerminalNode *AS();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ParentheticContext : public ExprContext {
  public:
    ParentheticContext(ExprContext *ctx);

    ExprContext *expr();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  MethodCallContext : public ExprContext {
  public:
    MethodCallContext(ExprContext *ctx);

    sqlParser::MethodNameContext *func = nullptr;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    MethodNameContext *methodName();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExprContext* expr();
  ExprContext* expr(int precedence);
  class  VariableNameContext : public antlr4::ParserRuleContext {
  public:
    VariableNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CURRENT_CATALOG();
    antlr4::tree::TerminalNode *CURRENT_SCHEMA();
    antlr4::tree::TerminalNode *CURRENT_TIMESTAMP();
    antlr4::tree::TerminalNode *CURRENT_USER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VariableNameContext* variableName();

  class  ColumnRefContext : public antlr4::ParserRuleContext {
  public:
    ColumnRefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnNameContext *columnName();
    TableNameContext *tableName();
    SchemaNameContext *schemaName();
    CatalogNameContext *catalogName();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnRefContext* columnRef();

  class  ColumnWildcardContext : public antlr4::ParserRuleContext {
  public:
    ColumnWildcardContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TableNameContext *tableName();
    SchemaNameContext *schemaName();
    CatalogNameContext *catalogName();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnWildcardContext* columnWildcard();

  class  ClassNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    ClassNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    std::vector<QualifierContext *> qualifier();
    QualifierContext* qualifier(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassNameContext* className();

  class  FunctionNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    FunctionNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionNameContext* functionName();

  class  MethodNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    MethodNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodNameContext* methodName();

  class  CatalogNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    CatalogNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CatalogNameContext* catalogName();

  class  SchemaNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    SchemaNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SchemaNameContext* schemaName();

  class  TableNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    TableNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableNameContext* tableName();

  class  TableAliasContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    TableAliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableAliasContext* tableAlias();

  class  ColumnNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    ColumnNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnNameContext* columnName();

  class  ColumnAliasContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    ColumnAliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnAliasContext* columnAlias();

  class  CollationNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    CollationNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CollationNameContext* collationName();

  class  IndexNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    IndexNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IndexNameContext* indexName();

  class  PropertyNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    PropertyNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyNameContext* propertyName();

  class  HintNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    HintNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HintNameContext* hintName();

  class  ExprHintContext : public antlr4::ParserRuleContext {
  public:
    ExprHintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HASHKEY();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExprHintContext* exprHint();

  class  PropertyValueContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::SimpleLiteralContext *lit = nullptr;;
    sqlParser::IdentifierContext *id = nullptr;;
    PropertyValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleLiteralContext *simpleLiteral();
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyValueContext* propertyValue();

  class  HintValueContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::SimpleLiteralContext *lit = nullptr;;
    sqlParser::IdentifierContext *id = nullptr;;
    sqlParser::ExprContext *expression = nullptr;;
    HintValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleLiteralContext *simpleLiteral();
    IdentifierContext *identifier();
    ExprContext *expr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HintValueContext* hintValue();

  class  IdentifierContext : public antlr4::ParserRuleContext {
  public:
    IdentifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    NonReservedWordContext *nonReservedWord();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdentifierContext* identifier();

  class  QualifierContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;;
    QualifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QualifierContext* qualifier();

  class  PrecisionContext : public antlr4::ParserRuleContext {
  public:
    PrecisionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerLiteralContext *integerLiteral();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrecisionContext* precision();

  class  ScaleContext : public antlr4::ParserRuleContext {
  public:
    ScaleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerLiteralContext *integerLiteral();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ScaleContext* scale();

  class  SizeContext : public antlr4::ParserRuleContext {
  public:
    SizeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerLiteralContext *integerLiteral();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SizeContext* size();

  class  LiteralContext : public antlr4::ParserRuleContext {
  public:
    LiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SQL_NULL();
    BooleanLiteralContext *booleanLiteral();
    SignedIntegerLiteralContext *signedIntegerLiteral();
    SignedDecimalLiteralContext *signedDecimalLiteral();
    SignedFloatLiteralContext *signedFloatLiteral();
    DateLiteralContext *dateLiteral();
    TimeLiteralContext *timeLiteral();
    StringLiteralContext *stringLiteral();
    TimestampLiteralContext *timestampLiteral();
    IntervalLiteralContext *intervalLiteral();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LiteralContext* literal();

  class  SimpleLiteralContext : public antlr4::ParserRuleContext {
  public:
    SimpleLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SQL_NULL();
    BooleanLiteralContext *booleanLiteral();
    SignedIntegerLiteralContext *signedIntegerLiteral();
    SignedDecimalLiteralContext *signedDecimalLiteral();
    SignedFloatLiteralContext *signedFloatLiteral();
    StringLiteralContext *stringLiteral();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleLiteralContext* simpleLiteral();

  class  BooleanLiteralContext : public antlr4::ParserRuleContext {
  public:
    BooleanLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SQL_TRUE();
    antlr4::tree::TerminalNode *SQL_FALSE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BooleanLiteralContext* booleanLiteral();

  class  IntegerLiteralContext : public antlr4::ParserRuleContext {
  public:
    IntegerLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER_LITERAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerLiteralContext* integerLiteral();

  class  SignedIntegerLiteralContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *neg = nullptr;;
    SignedIntegerLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER_LITERAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignedIntegerLiteralContext* signedIntegerLiteral();

  class  DecimalLiteralContext : public antlr4::ParserRuleContext {
  public:
    DecimalLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL_LITERAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DecimalLiteralContext* decimalLiteral();

  class  SignedDecimalLiteralContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *neg = nullptr;;
    SignedDecimalLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL_LITERAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignedDecimalLiteralContext* signedDecimalLiteral();

  class  FloatLiteralContext : public antlr4::ParserRuleContext {
  public:
    FloatLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FLOAT_LITERAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FloatLiteralContext* floatLiteral();

  class  SignedFloatLiteralContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *neg = nullptr;;
    SignedFloatLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FLOAT_LITERAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignedFloatLiteralContext* signedFloatLiteral();

  class  DateLiteralContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::StringLiteralContext *str = nullptr;;
    DateLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DATE();
    StringLiteralContext *stringLiteral();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DateLiteralContext* dateLiteral();

  class  TimeLiteralContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::StringLiteralContext *str = nullptr;;
    TimeLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TIME();
    StringLiteralContext *stringLiteral();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TimeLiteralContext* timeLiteral();

  class  TimestampLiteralContext : public antlr4::ParserRuleContext {
  public:
    TimestampLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TIMESTAMP();
    StringLiteralContext *stringLiteral();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TimestampLiteralContext* timestampLiteral();

  class  StringLiteralContext : public antlr4::ParserRuleContext {
  public:
    StringLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING_LITERAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StringLiteralContext* stringLiteral();

  class  IntervalLiteralContext : public antlr4::ParserRuleContext {
  public:
    IntervalLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Sql92CalendarIntervalContext *sql92CalendarInterval();
    Sql92TimeIntervalContext *sql92TimeInterval();
    LooseIntervalContext *looseInterval();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntervalLiteralContext* intervalLiteral();

  class  LooseIntervalContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::ExprContext *str = nullptr;;
    LooseIntervalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTERVAL();
    ExprContext *expr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LooseIntervalContext* looseInterval();

  class  Sql92CalendarIntervalContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *year = nullptr;;
    antlr4::Token *month = nullptr;;
    Sql92CalendarIntervalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTERVAL();
    StringLiteralContext *stringLiteral();
    antlr4::tree::TerminalNode *TO();
    antlr4::tree::TerminalNode *YEAR();
    antlr4::tree::TerminalNode *MONTH();
    PrecisionContext *precision();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Sql92CalendarIntervalContext* sql92CalendarInterval();

  class  Sql92TimeIntervalContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *day = nullptr;;
    antlr4::Token *hour = nullptr;;
    antlr4::Token *min = nullptr;;
    antlr4::Token *sec = nullptr;;
    Sql92TimeIntervalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTERVAL();
    StringLiteralContext *stringLiteral();
    antlr4::tree::TerminalNode *TO();
    antlr4::tree::TerminalNode *DAY();
    antlr4::tree::TerminalNode *HOUR();
    antlr4::tree::TerminalNode *MINUTE();
    antlr4::tree::TerminalNode *SECOND();
    PrecisionContext *precision();
    ScaleContext *scale();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Sql92TimeIntervalContext* sql92TimeInterval();

  class  NonReservedWordContext : public antlr4::ParserRuleContext {
  public:
    NonReservedWordContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ANTLR();
    antlr4::tree::TerminalNode *ASC();
    antlr4::tree::TerminalNode *BIT();
    antlr4::tree::TerminalNode *BITS();
    antlr4::tree::TerminalNode *BLOCK();
    antlr4::tree::TerminalNode *BLOOM();
    antlr4::tree::TerminalNode *BOTH();
    antlr4::tree::TerminalNode *BTREE();
    antlr4::tree::TerminalNode *BY();
    antlr4::tree::TerminalNode *BYTE();
    antlr4::tree::TerminalNode *BYTES();
    antlr4::tree::TerminalNode *CATALOG();
    antlr4::tree::TerminalNode *CHARS();
    antlr4::tree::TerminalNode *COALESCE();
    antlr4::tree::TerminalNode *CONCAT();
    antlr4::tree::TerminalNode *CORRESPONDING();
    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *CURRENT_CATALOG();
    antlr4::tree::TerminalNode *CURRENT_SCHEMA();
    antlr4::tree::TerminalNode *CURRENT_TIMESTAMP();
    antlr4::tree::TerminalNode *CURRENT_USER();
    antlr4::tree::TerminalNode *DATA();
    antlr4::tree::TerminalNode *DATE();
    antlr4::tree::TerminalNode *DAY();
    antlr4::tree::TerminalNode *DAYS();
    antlr4::tree::TerminalNode *DECODE();
    antlr4::tree::TerminalNode *DELETE();
    antlr4::tree::TerminalNode *DESC();
    antlr4::tree::TerminalNode *DETAIL();
    antlr4::tree::TerminalNode *DIGITS();
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *ESCAPE();
    antlr4::tree::TerminalNode *EXCEPT();
    antlr4::tree::TerminalNode *EXPLAIN();
    antlr4::tree::TerminalNode *EXTRACT();
    antlr4::tree::TerminalNode *FEDERATION();
    antlr4::tree::TerminalNode *FORMAT();
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *HASHKEY();
    antlr4::tree::TerminalNode *HOUR();
    antlr4::tree::TerminalNode *HOURS();
    antlr4::tree::TerminalNode *IFNULL();
    antlr4::tree::TerminalNode *INDEX();
    antlr4::tree::TerminalNode *INSERT();
    antlr4::tree::TerminalNode *INTO();
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *JOIN();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *LEFT();
    antlr4::tree::TerminalNode *LOOP();
    antlr4::tree::TerminalNode *MINUTE();
    antlr4::tree::TerminalNode *MINUTES();
    antlr4::tree::TerminalNode *MONOTONIC();
    antlr4::tree::TerminalNode *MONTH();
    antlr4::tree::TerminalNode *MONTHS();
    antlr4::tree::TerminalNode *NO();
    antlr4::tree::TerminalNode *NODE();
    antlr4::tree::TerminalNode *NULLIF();
    antlr4::tree::TerminalNode *OFFSET();
    antlr4::tree::TerminalNode *OUTER();
    antlr4::tree::TerminalNode *OVERLAY();
    antlr4::tree::TerminalNode *PARTITION();
    antlr4::tree::TerminalNode *POSITION();
    antlr4::tree::TerminalNode *PRETTY();
    antlr4::tree::TerminalNode *QUARTER();
    antlr4::tree::TerminalNode *QUARTERS();
    antlr4::tree::TerminalNode *REFERENCE();
    antlr4::tree::TerminalNode *RIGHT();
    antlr4::tree::TerminalNode *SCHEMA();
    antlr4::tree::TerminalNode *SECOND();
    antlr4::tree::TerminalNode *SECONDS();
    antlr4::tree::TerminalNode *SET();
    antlr4::tree::TerminalNode *SKIPLIST();
    antlr4::tree::TerminalNode *STAGE();
    antlr4::tree::TerminalNode *STATS();
    antlr4::tree::TerminalNode *STATISTICS();
    antlr4::tree::TerminalNode *SUBSTR();
    antlr4::tree::TerminalNode *SUBSTRING();
    antlr4::tree::TerminalNode *TABLE();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *TREE();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *UNION();
    antlr4::tree::TerminalNode *UNIQUE();
    antlr4::tree::TerminalNode *UPDATE();
    antlr4::tree::TerminalNode *USING();
    antlr4::tree::TerminalNode *VALUE();
    antlr4::tree::TerminalNode *VALUES();
    antlr4::tree::TerminalNode *VARYING();
    antlr4::tree::TerminalNode *WEEK();
    antlr4::tree::TerminalNode *WEEKS();
    antlr4::tree::TerminalNode *WITH_NAMES();
    antlr4::tree::TerminalNode *YEAR();
    antlr4::tree::TerminalNode *YEARS();
    antlr4::tree::TerminalNode *INTEGER();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *FLOAT();
    antlr4::tree::TerminalNode *TIMESTAMP();
    antlr4::tree::TerminalNode *CHARACTER();
    antlr4::tree::TerminalNode *CHAR();
    antlr4::tree::TerminalNode *BOOLEAN();
    antlr4::tree::TerminalNode *BINARY();
    antlr4::tree::TerminalNode *VARBINARY();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NonReservedWordContext* nonReservedWord();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool setExprSempred(SetExprContext *_localctx, size_t predicateIndex);
  bool tableExprSempred(TableExprContext *_localctx, size_t predicateIndex);
  bool exprSempred(ExprContext *_localctx, size_t predicateIndex);

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

}  // namespace Parser
