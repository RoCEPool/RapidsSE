
// Generated from /Users/alka/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"


namespace Parser {


class  sqlParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, ALL = 23, ANALYSE = 24, ANALYZE = 25, AND = 26, 
    ANIDX = 27, ANTBL = 28, ANTI = 29, ANY = 30, AS = 31, ASC = 32, AVG = 33, 
    BEGIN = 34, BETWEEN = 35, BINARY = 36, BIT = 37, BITS = 38, BLOB = 39, 
    BLOCK = 40, BLOOM = 41, BOTH = 42, BOOLEAN = 43, BPA = 44, BTREE = 45, 
    BYE = 46, BY = 47, BYTE = 48, BYTES = 49, CASE = 50, CAST = 51, CATALOG = 52, 
    CHAR = 53, CHARS = 54, CHARACTER = 55, COALESCE = 56, COLLATE = 57, 
    COMPACTIDX = 58, COMPACT = 59, CONCAT = 60, CONFIG = 61, CORRESPONDING = 62, 
    COUNT = 63, CREATE = 64, CROSS = 65, CURRENT_CATALOG = 66, CURRENT_SCHEMA = 67, 
    CURRENT_TIMESTAMP = 68, CURRENT_USER = 69, DATA = 70, DATE = 71, DATE_FORMAT = 72, 
    DAY = 73, DAYS = 74, DECIMAL = 75, DECODE = 76, DELETE = 77, DESC = 78, 
    DETAIL = 79, DIGITS = 80, DISABLE = 81, DISCONNECT = 82, DISTINCT = 83, 
    DLOAD = 84, DOCOL = 85, DROP = 86, ECHO = 87, ELSE = 88, ENABLE = 89, 
    ENCLOSED_BY_LEADING = 90, ENCLOSED_BY_TRAILING = 91, END = 92, ESCAPE = 93, 
    ESCAPE_CHAR = 94, EXCEPT = 95, EXISTS = 96, EXIT_TOKEN = 97, EXPLAIN = 98, 
    EXPORT = 99, EXTRACT = 100, SQL_FALSE = 101, FEDERATION = 102, FIELD_DELIMITER = 103, 
    FILE = 104, FLOAT = 105, FOR = 106, FORCE = 107, FORMAT = 108, FULL = 109, 
    FROM = 110, GROUP = 111, HASH = 112, HASHKEY = 113, HAVING = 114, HEAP = 115, 
    HOUR = 116, HOURS = 117, IF = 118, IFNULL = 119, IN = 120, INDEX = 121, 
    INNER = 122, INSERT = 123, INTEGER = 124, INTERSECT = 125, INTERVAL_DT = 126, 
    INTERVAL_YM = 127, INTERVAL = 128, INTO = 129, IS = 130, JOIN = 131, 
    KEY = 132, LEADING = 133, LEFT = 134, LIKE = 135, LIMIT = 136, LOADREPLICATED = 137, 
    LOAD = 138, LOG = 139, LOOP = 140, LSCOL = 141, LSIDXCOL = 142, LSIDX = 143, 
    LSPTNCOL = 144, LSTBL = 145, MAX = 146, MEMORYLEAKS = 147, METRICSCTL = 148, 
    MILLIS = 149, MILLISECOND = 150, MILLISECONDS = 151, MIN = 152, MINUTE = 153, 
    MINUTES = 154, MONOTONIC = 155, MONTH = 156, MONTHS = 157, MS = 158, 
    MULTIPLE = 159, NATURAL = 160, NO = 161, NODES = 162, NODE = 163, NOOP = 164, 
    NOP = 165, NOT = 166, NOW = 167, SQL_NULL = 168, NULLIF = 169, OBJECTS = 170, 
    OFF = 171, OFFSET = 172, ON = 173, OR = 174, ORDER = 175, ORIGIN = 176, 
    OUTER = 177, OVER = 178, OVERLAY = 179, PARTITIONS = 180, PARTITION = 181, 
    PATHS = 182, PIPELINE = 183, PLACING = 184, POOL = 185, POSITION = 186, 
    PRETTY = 187, PRIMARY = 188, QUARTER = 189, QUARTERS = 190, RECORD_ENDS = 191, 
    RECORD_TERMINATOR = 192, RECURSIVE = 193, REFERENCE = 194, REGIONS = 195, 
    REPLICATED = 196, RIGHT = 197, ROW = 198, ROWSIZE = 199, SCHEMA = 200, 
    SEC = 201, SECOND = 202, SECONDS = 203, SELECT = 204, SEMI = 205, SET = 206, 
    SKIPLIST = 207, SLEEP = 208, SPACE = 209, STAGE = 210, STATS = 211, 
    STATISTICS = 212, STATUS = 213, SUBSTR = 214, SUBSTRING = 215, SUM = 216, 
    SYSREPORT = 217, SZIDX = 218, SZTBL = 219, S_ONLY = 220, TABLE = 221, 
    TABLESIZES = 222, TABLESIZE = 223, TEXT = 224, THEN = 225, THREADS = 226, 
    TIME = 227, TIMESTAMP = 228, TO = 229, TOP = 230, TRACE = 231, TRACKMEMORY = 232, 
    TREE = 233, TRIM = 234, TRAILING = 235, TRUNCATE = 236, SQL_TRUE = 237, 
    TYPE = 238, TYPEOF = 239, UNION = 240, UNIQUE = 241, UNLOAD = 242, UPDATE = 243, 
    USING = 244, VALUE = 245, VALUES = 246, VARBINARY = 247, VARCHAR = 248, 
    VARYING = 249, WEEK = 250, WEEKS = 251, WHEN = 252, WHERE = 253, WITH_NAMES = 254, 
    WITH_TYPES = 255, WITH = 256, XOR = 257, YEAR = 258, YEARS = 259, PLAIN_IDENTIFIER = 260, 
    BAKTIKD_IDENTIFIER = 261, DUBLQUOTED_IDENTIFIER = 262, PARAM_IDENTIFIER = 263, 
    SINGLQUOTED_STRING = 264, DUBLQUOTED_STRING = 265, FLOAT_LITERAL = 266, 
    DECIMAL_LITERAL = 267, INTEGER_LITERAL = 268, HINT_START = 269, HINT_END = 270, 
    SLASH_COMMENT = 271, SQL_COMMENT = 272, BLOCK_COMMENT = 273, WHITESPACE = 274
  };

  enum {
    RuleStatements = 0, RuleStatement = 1, RuleStatementChoice = 2, RuleTags = 3, 
    RuleKeyValueTag = 4, RuleKeyOnlyTag = 5, RuleTagName = 6, RuleQueryCmd = 7, 
    RuleQuery = 8, RuleQueryHint = 9, RuleHint = 10, RuleSelectQuery = 11, 
    RuleSelectExpr = 12, RuleSetQuery = 13, RuleSetExpr = 14, RuleCorrClause = 15, 
    RuleWithQuery = 16, RuleTableExpr = 17, RuleTableReference = 18, RuleJoinType = 19, 
    RuleExtendedJoinType = 20, RuleValuesExpr = 21, RuleAnidxCmd = 22, RuleAntblCmd = 23, 
    RuleBpaCmd = 24, RuleCompactIdxCmd = 25, RuleCountCmd = 26, RuleCreateCmd = 27, 
    RuleCreateTableCmd = 28, RuleCreateIndexCmd = 29, RuleDeleteCmd = 30, 
    RuleDisconnectCmd = 31, RuleDloadCmd = 32, RuleDocolCmd = 33, RuleDropCmd = 34, 
    RuleDropTableCmd = 35, RuleDropIndexCmd = 36, RuleEmptyCmd = 37, RuleExplainCmd = 38, 
    RuleExplainOptions = 39, RuleExplainStageList = 40, RuleExplainRange = 41, 
    RuleExplainRangeChoice = 42, RuleExplainFormat = 43, RuleExportCmd = 44, 
    RuleFileCmd = 45, RuleInsertCmd = 46, RuleLoadCmd = 47, RuleLoadReplicatedCmd = 48, 
    RuleLoadOption = 49, RuleLoadOptionKeys = 50, RuleLogCmd = 51, RuleLscolCmd = 52, 
    RuleLsidxCmd = 53, RuleLsidxcolCmd = 54, RuleLsptncolCmd = 55, RuleLstblCmd = 56, 
    RuleMemoryleaksCmd = 57, RuleMetricsCtlCmd = 58, RuleNoopCmd = 59, RulePipelineCmd = 60, 
    RulePipelineStage = 61, RulePipelineOpSeq = 62, RulePipelineOperator = 63, 
    RulePipelineOpParam = 64, RulePipelineTypeNames = 65, RuleSetTraceCmd = 66, 
    RuleSleepCmd = 67, RuleSleepUnits = 68, RuleSpaceCmd = 69, RuleStatusCmd = 70, 
    RuleSysReportCmd = 71, RuleSzidxCmd = 72, RuleSztblCmd = 73, RuleTableSizesCmd = 74, 
    RuleTraceOption = 75, RuleTruncateCmd = 76, RuleTrackmemoryCmd = 77, 
    RuleUpdateCmd = 78, RuleUnloadCmd = 79, RuleTableDefinition = 80, RuleTablePartitioning = 81, 
    RuleColumnDefinition = 82, RuleColumnNameList = 83, RuleIndexDefinition = 84, 
    RuleIndexMethod = 85, RuleDataType = 86, RuleSqlDataType = 87, RuleIntervalType = 88, 
    RuleYearMonthIntervalType = 89, RuleDayTimeIntervalType = 90, RuleFixedLenChar = 91, 
    RuleVaryingLenChar = 92, RuleAsSubquery = 93, RuleSelectOrValues = 94, 
    RuleSelectList = 95, RuleSelectListItem = 96, RuleGroupByList = 97, 
    RuleGroupByListItem = 98, RuleOrderByList = 99, RuleOrderByListItem = 100, 
    RuleJoinUsingList = 101, RuleJoinUsingListItem = 102, RuleCorrList = 103, 
    RuleCorrListItem = 104, RuleColAliases = 105, RuleColAliasList = 106, 
    RuleColAliasListItem = 107, RulePropertyList = 108, RuleProperty = 109, 
    RuleValueList = 110, RulePredicate = 111, RuleJoinPredicate = 112, RuleCasePredicate = 113, 
    RuleExpr = 114, RuleVariableName = 115, RuleColumnRef = 116, RuleColumnWildcard = 117, 
    RuleClassName = 118, RuleFunctionName = 119, RuleMethodName = 120, RuleCatalogName = 121, 
    RuleSchemaName = 122, RuleTableName = 123, RuleTableAlias = 124, RuleColumnName = 125, 
    RuleColumnAlias = 126, RuleCollationName = 127, RuleIndexName = 128, 
    RulePropertyName = 129, RuleHintName = 130, RuleExprHint = 131, RulePropertyValue = 132, 
    RuleHintValue = 133, RuleIdentifier = 134, RuleQualifier = 135, RulePrecision = 136, 
    RuleScale = 137, RuleSize = 138, RuleLiteral = 139, RuleSimpleLiteral = 140, 
    RuleBooleanLiteral = 141, RuleIntegerLiteral = 142, RuleSignedIntegerLiteral = 143, 
    RuleDecimalLiteral = 144, RuleSignedDecimalLiteral = 145, RuleFloatLiteral = 146, 
    RuleSignedFloatLiteral = 147, RuleDateLiteral = 148, RuleTimeLiteral = 149, 
    RuleTimestampLiteral = 150, RuleStringLiteral = 151, RuleIntervalLiteral = 152, 
    RuleYearMonthInterval = 153, RuleDayTimeInterval = 154, RuleNonReservedWord = 155
  };

  explicit sqlParser(antlr4::TokenStream *input);

  sqlParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~sqlParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class StatementsContext;
  class StatementContext;
  class StatementChoiceContext;
  class TagsContext;
  class KeyValueTagContext;
  class KeyOnlyTagContext;
  class TagNameContext;
  class QueryCmdContext;
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
  class AnidxCmdContext;
  class AntblCmdContext;
  class BpaCmdContext;
  class CompactIdxCmdContext;
  class CountCmdContext;
  class CreateCmdContext;
  class CreateTableCmdContext;
  class CreateIndexCmdContext;
  class DeleteCmdContext;
  class DisconnectCmdContext;
  class DloadCmdContext;
  class DocolCmdContext;
  class DropCmdContext;
  class DropTableCmdContext;
  class DropIndexCmdContext;
  class EmptyCmdContext;
  class ExplainCmdContext;
  class ExplainOptionsContext;
  class ExplainStageListContext;
  class ExplainRangeContext;
  class ExplainRangeChoiceContext;
  class ExplainFormatContext;
  class ExportCmdContext;
  class FileCmdContext;
  class InsertCmdContext;
  class LoadCmdContext;
  class LoadReplicatedCmdContext;
  class LoadOptionContext;
  class LoadOptionKeysContext;
  class LogCmdContext;
  class LscolCmdContext;
  class LsidxCmdContext;
  class LsidxcolCmdContext;
  class LsptncolCmdContext;
  class LstblCmdContext;
  class MemoryleaksCmdContext;
  class MetricsCtlCmdContext;
  class NoopCmdContext;
  class PipelineCmdContext;
  class PipelineStageContext;
  class PipelineOpSeqContext;
  class PipelineOperatorContext;
  class PipelineOpParamContext;
  class PipelineTypeNamesContext;
  class SetTraceCmdContext;
  class SleepCmdContext;
  class SleepUnitsContext;
  class SpaceCmdContext;
  class StatusCmdContext;
  class SysReportCmdContext;
  class SzidxCmdContext;
  class SztblCmdContext;
  class TableSizesCmdContext;
  class TraceOptionContext;
  class TruncateCmdContext;
  class TrackmemoryCmdContext;
  class UpdateCmdContext;
  class UnloadCmdContext;
  class TableDefinitionContext;
  class TablePartitioningContext;
  class ColumnDefinitionContext;
  class ColumnNameListContext;
  class IndexDefinitionContext;
  class IndexMethodContext;
  class DataTypeContext;
  class SqlDataTypeContext;
  class IntervalTypeContext;
  class YearMonthIntervalTypeContext;
  class DayTimeIntervalTypeContext;
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
  class YearMonthIntervalContext;
  class DayTimeIntervalContext;
  class NonReservedWordContext; 

  class  StatementsContext : public antlr4::ParserRuleContext {
  public:
    StatementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementsContext* statements();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TagsContext *tags();
    StatementChoiceContext *statementChoice();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementContext* statement();

  class  StatementChoiceContext : public antlr4::ParserRuleContext {
  public:
    StatementChoiceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AnidxCmdContext *anidxCmd();
    AntblCmdContext *antblCmd();
    CompactIdxCmdContext *compactIdxCmd();
    CountCmdContext *countCmd();
    CreateCmdContext *createCmd();
    DisconnectCmdContext *disconnectCmd();
    DloadCmdContext *dloadCmd();
    DocolCmdContext *docolCmd();
    DropCmdContext *dropCmd();
    ExplainCmdContext *explainCmd();
    ExportCmdContext *exportCmd();
    LoadCmdContext *loadCmd();
    LoadReplicatedCmdContext *loadReplicatedCmd();
    LogCmdContext *logCmd();
    LscolCmdContext *lscolCmd();
    LsidxCmdContext *lsidxCmd();
    LsidxcolCmdContext *lsidxcolCmd();
    LsptncolCmdContext *lsptncolCmd();
    LstblCmdContext *lstblCmd();
    MetricsCtlCmdContext *metricsCtlCmd();
    PipelineCmdContext *pipelineCmd();
    QueryCmdContext *queryCmd();
    SetTraceCmdContext *setTraceCmd();
    SleepCmdContext *sleepCmd();
    SpaceCmdContext *spaceCmd();
    StatusCmdContext *statusCmd();
    SysReportCmdContext *sysReportCmd();
    SzidxCmdContext *szidxCmd();
    SztblCmdContext *sztblCmd();
    TableSizesCmdContext *tableSizesCmd();
    TruncateCmdContext *truncateCmd();
    UnloadCmdContext *unloadCmd();
    TrackmemoryCmdContext *trackmemoryCmd();
    MemoryleaksCmdContext *memoryleaksCmd();
    BpaCmdContext *bpaCmd();
    FileCmdContext *fileCmd();
    NoopCmdContext *noopCmd();
    EmptyCmdContext *emptyCmd();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementChoiceContext* statementChoice();

  class  TagsContext : public antlr4::ParserRuleContext {
  public:
    TagsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<KeyOnlyTagContext *> keyOnlyTag();
    KeyOnlyTagContext* keyOnlyTag(size_t i);
    std::vector<KeyValueTagContext *> keyValueTag();
    KeyValueTagContext* keyValueTag(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TagsContext* tags();

  class  KeyValueTagContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TagNameContext *keyName = nullptr;
    KeyValueTagContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StringLiteralContext *stringLiteral();
    antlr4::tree::TerminalNode *INTEGER_LITERAL();
    TagNameContext *tagName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeyValueTagContext* keyValueTag();

  class  KeyOnlyTagContext : public antlr4::ParserRuleContext {
  public:
    KeyOnlyTagContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TagNameContext *tagName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeyOnlyTagContext* keyOnlyTag();

  class  TagNameContext : public antlr4::ParserRuleContext {
  public:
    TagNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PLAIN_IDENTIFIER();
    antlr4::tree::TerminalNode *TIME();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TagNameContext* tagName();

  class  QueryCmdContext : public antlr4::ParserRuleContext {
  public:
    QueryCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    QueryContext *query();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QueryCmdContext* queryCmd();

  class  QueryContext : public antlr4::ParserRuleContext {
  public:
    QueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectQueryContext *selectQuery();
    SetQueryContext *setQuery();
    std::vector<QueryHintContext *> queryHint();
    QueryHintContext* queryHint(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QueryHintContext* queryHint();

  class  HintContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::HintNameContext *name = nullptr;
    sqlParser::ExprHintContext *exprName = nullptr;
    HintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    HintNameContext *hintName();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    ExprHintContext *exprHint();
    std::vector<HintValueContext *> hintValue();
    HintValueContext* hintValue(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HintContext* hint();

  class  SelectQueryContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::OrderByListContext *orderBy = nullptr;
    antlr4::Token *limit = nullptr;
    antlr4::Token *offset = nullptr;
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectQueryContext* selectQuery();

  class  SelectExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *distinct = nullptr;
    sqlParser::PredicateContext *where = nullptr;
    sqlParser::GroupByListContext *groupBy = nullptr;
    sqlParser::PredicateContext *having = nullptr;
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectExprContext* selectExpr();

  class  SetQueryContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::OrderByListContext *orderBy = nullptr;
    antlr4::Token *limit = nullptr;
    antlr4::Token *offset = nullptr;
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetQueryContext* setQuery();

  class  SetExprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *op = nullptr;
    antlr4::Token *all = nullptr;
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetExprContext* setExpr();
  SetExprContext* setExpr(int precedence);
  class  CorrClauseContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *corr = nullptr;
    CorrClauseContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CORRESPONDING();
    antlr4::tree::TerminalNode *BY();
    CorrListContext *corrList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CorrClauseContext* corrClause();

  class  WithQueryContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TableNameContext *name = nullptr;
    WithQueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AS();
    QueryContext *query();
    TableNameContext *tableName();
    antlr4::tree::TerminalNode *RECURSIVE();
    ColAliasesContext *colAliases();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  FetchContext : public TableExprContext {
  public:
    FetchContext(TableExprContext *ctx);

    sqlParser::TableReferenceContext *ref = nullptr;
    sqlParser::TableAliasContext *alias = nullptr;
    TableReferenceContext *tableReference();
    TableAliasContext *tableAlias();
    antlr4::tree::TerminalNode *AS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ValuesSubqueryContext : public TableExprContext {
  public:
    ValuesSubqueryContext(TableExprContext *ctx);

    sqlParser::TableAliasContext *as = nullptr;
    ValuesExprContext *valuesExpr();
    TableAliasContext *tableAlias();
    antlr4::tree::TerminalNode *AS();
    ColAliasesContext *colAliases();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  TableExprContext* tableExpr();
  TableExprContext* tableExpr(int precedence);
  class  TableReferenceContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::CatalogNameContext *catalog = nullptr;
    sqlParser::SchemaNameContext *schema = nullptr;
    sqlParser::TableNameContext *table = nullptr;
    TableReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TableNameContext *tableName();
    SchemaNameContext *schemaName();
    CatalogNameContext *catalogName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableReferenceContext* tableReference();

  class  JoinTypeContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *outer = nullptr;
    JoinTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INNER();
    antlr4::tree::TerminalNode *LEFT();
    antlr4::tree::TerminalNode *RIGHT();
    antlr4::tree::TerminalNode *FULL();
    antlr4::tree::TerminalNode *OUTER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  JoinTypeContext* joinType();

  class  ExtendedJoinTypeContext : public antlr4::ParserRuleContext {
  public:
    ExtendedJoinTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    JoinTypeContext *joinType();
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *ANTI();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtendedJoinTypeContext* extendedJoinType();

  class  ValuesExprContext : public antlr4::ParserRuleContext {
  public:
    ValuesExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VALUES();
    std::vector<ValueListContext *> valueList();
    ValueListContext* valueList(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValuesExprContext* valuesExpr();

  class  AnidxCmdContext : public antlr4::ParserRuleContext {
  public:
    AnidxCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TableNameContext *tableName();
    IndexNameContext *indexName();
    antlr4::tree::TerminalNode *ANIDX();
    antlr4::tree::TerminalNode *TABLE();
    antlr4::tree::TerminalNode *ANALYZE();
    antlr4::tree::TerminalNode *ANALYSE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AnidxCmdContext* anidxCmd();

  class  AntblCmdContext : public antlr4::ParserRuleContext {
  public:
    AntblCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TableNameContext *tableName();
    antlr4::tree::TerminalNode *ANTBL();
    antlr4::tree::TerminalNode *INDEX();
    antlr4::tree::TerminalNode *ANALYZE();
    antlr4::tree::TerminalNode *ANALYSE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AntblCmdContext* antblCmd();

  class  BpaCmdContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *type = nullptr;
    BpaCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BPA();
    antlr4::tree::TerminalNode *POOL();
    antlr4::tree::TerminalNode *REGIONS();
    antlr4::tree::TerminalNode *OBJECTS();
    antlr4::tree::TerminalNode *ALL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BpaCmdContext* bpaCmd();

  class  CompactIdxCmdContext : public antlr4::ParserRuleContext {
  public:
    CompactIdxCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TableNameContext *tableName();
    IndexNameContext *indexName();
    antlr4::tree::TerminalNode *COMPACTIDX();
    antlr4::tree::TerminalNode *COMPACT();
    antlr4::tree::TerminalNode *INDEX();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CompactIdxCmdContext* compactIdxCmd();

  class  CountCmdContext : public antlr4::ParserRuleContext {
  public:
    CountCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COUNT();
    TableNameContext *tableName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CountCmdContext* countCmd();

  class  CreateCmdContext : public antlr4::ParserRuleContext {
  public:
    CreateCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CreateTableCmdContext *createTableCmd();
    CreateIndexCmdContext *createIndexCmd();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateCmdContext* createCmd();

  class  CreateTableCmdContext : public antlr4::ParserRuleContext {
  public:
    CreateTableCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *TABLE();
    TableNameContext *tableName();
    TableDefinitionContext *tableDefinition();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateTableCmdContext* createTableCmd();

  class  CreateIndexCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IndexNameContext *idxName = nullptr;
    CreateIndexCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CREATE();
    antlr4::tree::TerminalNode *INDEX();
    antlr4::tree::TerminalNode *ON();
    TableNameContext *tableName();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    IndexNameContext *indexName();
    antlr4::tree::TerminalNode *UNIQUE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CreateIndexCmdContext* createIndexCmd();

  class  DeleteCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TableReferenceContext *ref = nullptr;
    DeleteCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeleteCmdContext* deleteCmd();

  class  DisconnectCmdContext : public antlr4::ParserRuleContext {
  public:
    DisconnectCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DISCONNECT();
    antlr4::tree::TerminalNode *BYE();
    antlr4::tree::TerminalNode *EXIT_TOKEN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DisconnectCmdContext* disconnectCmd();

  class  DloadCmdContext : public antlr4::ParserRuleContext {
  public:
    DloadCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DLOAD();
    TableNameContext *tableName();
    antlr4::tree::TerminalNode *PATHS();
    std::vector<StringLiteralContext *> stringLiteral();
    StringLiteralContext* stringLiteral(size_t i);
    std::vector<LoadOptionContext *> loadOption();
    LoadOptionContext* loadOption(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DloadCmdContext* dloadCmd();

  class  DocolCmdContext : public antlr4::ParserRuleContext {
  public:
    DocolCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOCOL();
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DocolCmdContext* docolCmd();

  class  DropCmdContext : public antlr4::ParserRuleContext {
  public:
    DropCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DropTableCmdContext *dropTableCmd();
    DropIndexCmdContext *dropIndexCmd();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DropCmdContext* dropCmd();

  class  DropTableCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TableNameContext *name = nullptr;
    DropTableCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *TABLE();
    TableNameContext *tableName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DropTableCmdContext* dropTableCmd();

  class  DropIndexCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IndexNameContext *idxName = nullptr;
    sqlParser::TableNameContext *tblName = nullptr;
    DropIndexCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DROP();
    antlr4::tree::TerminalNode *INDEX();
    antlr4::tree::TerminalNode *ON();
    IndexNameContext *indexName();
    TableNameContext *tableName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DropIndexCmdContext* dropIndexCmd();

  class  EmptyCmdContext : public antlr4::ParserRuleContext {
  public:
    EmptyCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EmptyCmdContext* emptyCmd();

  class  ExplainCmdContext : public antlr4::ParserRuleContext {
  public:
    ExplainCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXPLAIN();
    StatementChoiceContext *statementChoice();
    std::vector<QueryHintContext *> queryHint();
    QueryHintContext* queryHint(size_t i);
    std::vector<ExplainOptionsContext *> explainOptions();
    ExplainOptionsContext* explainOptions(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainCmdContext* explainCmd();

  class  ExplainOptionsContext : public antlr4::ParserRuleContext {
  public:
    ExplainOptionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExplainStageListContext *explainStageList();
    ExplainFormatContext *explainFormat();
    antlr4::tree::TerminalNode *WITH_NAMES();
    antlr4::tree::TerminalNode *WITH_TYPES();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainOptionsContext* explainOptions();

  class  ExplainStageListContext : public antlr4::ParserRuleContext {
  public:
    ExplainStageListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STAGE();
    std::vector<ExplainRangeContext *> explainRange();
    ExplainRangeContext* explainRange(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainStageListContext* explainStageList();

  class  ExplainRangeContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::ExplainRangeChoiceContext *begin = nullptr;
    sqlParser::ExplainRangeChoiceContext *end = nullptr;
    ExplainRangeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExplainRangeChoiceContext *> explainRangeChoice();
    ExplainRangeChoiceContext* explainRangeChoice(size_t i);
    antlr4::tree::TerminalNode *TO();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainRangeContext* explainRange();

  class  ExplainRangeChoiceContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *all = nullptr;
    antlr4::Token *num = nullptr;
    sqlParser::IdentifierContext *id = nullptr;
    ExplainRangeChoiceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *INTEGER_LITERAL();
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainRangeChoiceContext* explainRangeChoice();

  class  ExplainFormatContext : public antlr4::ParserRuleContext {
  public:
    ExplainFormatContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FORMAT();
    antlr4::tree::TerminalNode *PRETTY();
    antlr4::tree::TerminalNode *ROW();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExplainFormatContext* explainFormat();

  class  ExportCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::StringLiteralContext *filePath = nullptr;
    ExportCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXPORT();
    TableNameContext *tableName();
    StringLiteralContext *stringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExportCmdContext* exportCmd();

  class  FileCmdContext : public antlr4::ParserRuleContext {
  public:
    FileCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FILE();
    IdentifierContext *identifier();
    StringLiteralContext *stringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FileCmdContext* fileCmd();

  class  InsertCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TableReferenceContext *ref = nullptr;
    InsertCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InsertCmdContext* insertCmd();

  class  LoadCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::StringLiteralContext *filePath = nullptr;
    LoadCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOAD();
    TableNameContext *tableName();
    StringLiteralContext *stringLiteral();
    std::vector<LoadOptionContext *> loadOption();
    LoadOptionContext* loadOption(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LoadCmdContext* loadCmd();

  class  LoadReplicatedCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::StringLiteralContext *filePath = nullptr;
    LoadReplicatedCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOADREPLICATED();
    TableNameContext *tableName();
    StringLiteralContext *stringLiteral();
    std::vector<LoadOptionContext *> loadOption();
    LoadOptionContext* loadOption(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LoadReplicatedCmdContext* loadReplicatedCmd();

  class  LoadOptionContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::StringLiteralContext *loadValue = nullptr;
    LoadOptionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StringLiteralContext *stringLiteral();
    LoadOptionKeysContext *loadOptionKeys();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LoadOptionContext* loadOption();

  class  LoadOptionKeysContext : public antlr4::ParserRuleContext {
  public:
    LoadOptionKeysContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FIELD_DELIMITER();
    antlr4::tree::TerminalNode *ESCAPE_CHAR();
    antlr4::tree::TerminalNode *DATE_FORMAT();
    antlr4::tree::TerminalNode *RECORD_ENDS();
    antlr4::tree::TerminalNode *ENCLOSED_BY_LEADING();
    antlr4::tree::TerminalNode *ENCLOSED_BY_TRAILING();
    antlr4::tree::TerminalNode *RECORD_TERMINATOR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LoadOptionKeysContext* loadOptionKeys();

  class  LogCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::StringLiteralContext *msg = nullptr;
    LogCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LOG();
    StringLiteralContext *stringLiteral();
    antlr4::tree::TerminalNode *ECHO();
    antlr4::tree::TerminalNode *SQL_TRUE();
    antlr4::tree::TerminalNode *ON();
    antlr4::tree::TerminalNode *SQL_FALSE();
    antlr4::tree::TerminalNode *OFF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LogCmdContext* logCmd();

  class  LscolCmdContext : public antlr4::ParserRuleContext {
  public:
    LscolCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSCOL();
    TableNameContext *tableName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LscolCmdContext* lscolCmd();

  class  LsidxCmdContext : public antlr4::ParserRuleContext {
  public:
    LsidxCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSIDX();
    TableNameContext *tableName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LsidxCmdContext* lsidxCmd();

  class  LsidxcolCmdContext : public antlr4::ParserRuleContext {
  public:
    LsidxcolCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSIDXCOL();
    TableNameContext *tableName();
    IndexNameContext *indexName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LsidxcolCmdContext* lsidxcolCmd();

  class  LsptncolCmdContext : public antlr4::ParserRuleContext {
  public:
    LsptncolCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSPTNCOL();
    TableNameContext *tableName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LsptncolCmdContext* lsptncolCmd();

  class  LstblCmdContext : public antlr4::ParserRuleContext {
  public:
    LstblCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSTBL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LstblCmdContext* lstblCmd();

  class  MemoryleaksCmdContext : public antlr4::ParserRuleContext {
  public:
    MemoryleaksCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MEMORYLEAKS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MemoryleaksCmdContext* memoryleaksCmd();

  class  MetricsCtlCmdContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *value = nullptr;
    MetricsCtlCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *METRICSCTL();
    antlr4::tree::TerminalNode *ENABLE();
    antlr4::tree::TerminalNode *DISABLE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MetricsCtlCmdContext* metricsCtlCmd();

  class  NoopCmdContext : public antlr4::ParserRuleContext {
  public:
    NoopCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NOOP();
    antlr4::tree::TerminalNode *NOP();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NoopCmdContext* noopCmd();

  class  PipelineCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *pipelineName = nullptr;
    PipelineCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PIPELINE();
    antlr4::tree::TerminalNode *BEGIN();
    antlr4::tree::TerminalNode *END();
    IdentifierContext *identifier();
    std::vector<PipelineStageContext *> pipelineStage();
    PipelineStageContext* pipelineStage(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PipelineCmdContext* pipelineCmd();

  class  PipelineStageContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *stageName = nullptr;
    PipelineStageContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STAGE();
    antlr4::tree::TerminalNode *ON();
    PipelineOpSeqContext *pipelineOpSeq();
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *ORIGIN();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *NODES();
    antlr4::tree::TerminalNode *PARTITIONS();
    antlr4::tree::TerminalNode *MULTIPLE();
    antlr4::tree::TerminalNode *THREADS();
    IntegerLiteralContext *integerLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PipelineStageContext* pipelineStage();

  class  PipelineOpSeqContext : public antlr4::ParserRuleContext {
  public:
    PipelineOpSeqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PipelineOperatorContext *> pipelineOperator();
    PipelineOperatorContext* pipelineOperator(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PipelineOpSeqContext* pipelineOpSeq();

  class  PipelineOperatorContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *opName = nullptr;
    PipelineOperatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    std::vector<PipelineOpParamContext *> pipelineOpParam();
    PipelineOpParamContext* pipelineOpParam(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PipelineOperatorContext* pipelineOperator();

  class  PipelineOpParamContext : public antlr4::ParserRuleContext {
  public:
    PipelineOpParamContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *AS();
    antlr4::tree::TerminalNode *SUM();
    antlr4::tree::TerminalNode *AVG();
    antlr4::tree::TerminalNode *MIN();
    antlr4::tree::TerminalNode *MAX();
    antlr4::tree::TerminalNode *PARAM_IDENTIFIER();
    PipelineTypeNamesContext *pipelineTypeNames();
    BooleanLiteralContext *booleanLiteral();
    SignedIntegerLiteralContext *signedIntegerLiteral();
    SignedDecimalLiteralContext *signedDecimalLiteral();
    SignedFloatLiteralContext *signedFloatLiteral();
    DateLiteralContext *dateLiteral();
    TimeLiteralContext *timeLiteral();
    antlr4::tree::TerminalNode *DUBLQUOTED_IDENTIFIER();
    antlr4::tree::TerminalNode *DUBLQUOTED_STRING();
    TimestampLiteralContext *timestampLiteral();
    IntervalLiteralContext *intervalLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PipelineOpParamContext* pipelineOpParam();

  class  PipelineTypeNamesContext : public antlr4::ParserRuleContext {
  public:
    PipelineTypeNamesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BOOLEAN();
    antlr4::tree::TerminalNode *CHAR();
    antlr4::tree::TerminalNode *DATE();
    antlr4::tree::TerminalNode *DECIMAL();
    antlr4::tree::TerminalNode *FLOAT();
    antlr4::tree::TerminalNode *INTEGER();
    antlr4::tree::TerminalNode *INTERVAL_DT();
    antlr4::tree::TerminalNode *INTERVAL_YM();
    antlr4::tree::TerminalNode *TIME();
    antlr4::tree::TerminalNode *TIMESTAMP();
    antlr4::tree::TerminalNode *VARCHAR();
    antlr4::tree::TerminalNode *SQL_NULL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PipelineTypeNamesContext* pipelineTypeNames();

  class  SetTraceCmdContext : public antlr4::ParserRuleContext {
  public:
    SetTraceCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SET();
    antlr4::tree::TerminalNode *TRACE();
    TraceOptionContext *traceOption();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetTraceCmdContext* setTraceCmd();

  class  SleepCmdContext : public antlr4::ParserRuleContext {
  public:
    SleepCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SLEEP();
    IntegerLiteralContext *integerLiteral();
    SleepUnitsContext *sleepUnits();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SleepCmdContext* sleepCmd();

  class  SleepUnitsContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *seconds = nullptr;
    antlr4::Token *millis = nullptr;
    SleepUnitsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *S_ONLY();
    antlr4::tree::TerminalNode *SEC();
    antlr4::tree::TerminalNode *SECOND();
    antlr4::tree::TerminalNode *SECONDS();
    antlr4::tree::TerminalNode *MS();
    antlr4::tree::TerminalNode *MILLIS();
    antlr4::tree::TerminalNode *MILLISECOND();
    antlr4::tree::TerminalNode *MILLISECONDS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SleepUnitsContext* sleepUnits();

  class  SpaceCmdContext : public antlr4::ParserRuleContext {
  public:
    SpaceCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SPACE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SpaceCmdContext* spaceCmd();

  class  StatusCmdContext : public antlr4::ParserRuleContext {
  public:
    StatusCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STATUS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatusCmdContext* statusCmd();

  class  SysReportCmdContext : public antlr4::ParserRuleContext {
  public:
    SysReportCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SYSREPORT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SysReportCmdContext* sysReportCmd();

  class  SzidxCmdContext : public antlr4::ParserRuleContext {
  public:
    SzidxCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SZIDX();
    TableNameContext *tableName();
    IndexNameContext *indexName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SzidxCmdContext* szidxCmd();

  class  SztblCmdContext : public antlr4::ParserRuleContext {
  public:
    SztblCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SZTBL();
    TableNameContext *tableName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SztblCmdContext* sztblCmd();

  class  TableSizesCmdContext : public antlr4::ParserRuleContext {
  public:
    TableSizesCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TABLESIZES();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableSizesCmdContext* tableSizesCmd();

  class  TraceOptionContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *key = nullptr;
    TraceOptionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BooleanLiteralContext *booleanLiteral();
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TraceOptionContext* traceOption();

  class  TruncateCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TableNameContext *name = nullptr;
    TruncateCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TRUNCATE();
    TableNameContext *tableName();
    antlr4::tree::TerminalNode *TABLE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TruncateCmdContext* truncateCmd();

  class  TrackmemoryCmdContext : public antlr4::ParserRuleContext {
  public:
    TrackmemoryCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TRACKMEMORY();
    antlr4::tree::TerminalNode *FORCE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TrackmemoryCmdContext* trackmemoryCmd();

  class  UpdateCmdContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::TableReferenceContext *ref = nullptr;
    UpdateCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UpdateCmdContext* updateCmd();

  class  UnloadCmdContext : public antlr4::ParserRuleContext {
  public:
    UnloadCmdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UNLOAD();
    IdentifierContext *identifier();
    StringLiteralContext *stringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnloadCmdContext* unloadCmd();

  class  TableDefinitionContext : public antlr4::ParserRuleContext {
  public:
    TableDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColumnDefinitionContext *> columnDefinition();
    ColumnDefinitionContext* columnDefinition(size_t i);
    std::vector<TablePartitioningContext *> tablePartitioning();
    TablePartitioningContext* tablePartitioning(size_t i);
    std::vector<antlr4::tree::TerminalNode *> REPLICATED();
    antlr4::tree::TerminalNode* REPLICATED(size_t i);
    std::vector<antlr4::tree::TerminalNode *> ROWSIZE();
    antlr4::tree::TerminalNode* ROWSIZE(size_t i);
    std::vector<IntegerLiteralContext *> integerLiteral();
    IntegerLiteralContext* integerLiteral(size_t i);
    std::vector<antlr4::tree::TerminalNode *> TABLESIZE();
    antlr4::tree::TerminalNode* TABLESIZE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableDefinitionContext* tableDefinition();

  class  TablePartitioningContext : public antlr4::ParserRuleContext {
  public:
    TablePartitioningContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PARTITION();
    antlr4::tree::TerminalNode *BY();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TablePartitioningContext* tablePartitioning();

  class  ColumnDefinitionContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::ColumnNameContext *name = nullptr;
    sqlParser::DataTypeContext *type = nullptr;
    antlr4::Token *notNullable = nullptr;
    antlr4::Token *nullable = nullptr;
    ColumnDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColumnNameContext *columnName();
    DataTypeContext *dataType();
    std::vector<antlr4::tree::TerminalNode *> NOT();
    antlr4::tree::TerminalNode* NOT(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SQL_NULL();
    antlr4::tree::TerminalNode* SQL_NULL(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnDefinitionContext* columnDefinition();

  class  ColumnNameListContext : public antlr4::ParserRuleContext {
  public:
    ColumnNameListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColumnNameContext *> columnName();
    ColumnNameContext* columnName(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnNameListContext* columnNameList();

  class  IndexDefinitionContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *unique = nullptr;
    sqlParser::IndexNameContext *name = nullptr;
    sqlParser::IndexMethodContext *method = nullptr;
    sqlParser::PropertyListContext *props = nullptr;
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IndexMethodContext* indexMethod();

  class  DataTypeContext : public antlr4::ParserRuleContext {
  public:
    DataTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SqlDataTypeContext *sqlDataType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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
    IntervalTypeContext *intervalType();
    FixedLenCharContext *fixedLenChar();
    VaryingLenCharContext *varyingLenChar();
    antlr4::tree::TerminalNode *BINARY();
    SizeContext *size();
    antlr4::tree::TerminalNode *BYTE();
    antlr4::tree::TerminalNode *VARBINARY();
    antlr4::tree::TerminalNode *BLOB();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SqlDataTypeContext* sqlDataType();

  class  IntervalTypeContext : public antlr4::ParserRuleContext {
  public:
    IntervalTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTERVAL();
    YearMonthIntervalTypeContext *yearMonthIntervalType();
    DayTimeIntervalTypeContext *dayTimeIntervalType();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntervalTypeContext* intervalType();

  class  YearMonthIntervalTypeContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *year = nullptr;
    antlr4::Token *month = nullptr;
    YearMonthIntervalTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *YEAR();
    PrecisionContext *precision();
    antlr4::tree::TerminalNode *TO();
    antlr4::tree::TerminalNode *MONTH();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  YearMonthIntervalTypeContext* yearMonthIntervalType();

  class  DayTimeIntervalTypeContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *day = nullptr;
    antlr4::Token *hour = nullptr;
    antlr4::Token *min = nullptr;
    antlr4::Token *sec = nullptr;
    DayTimeIntervalTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DAY();
    PrecisionContext *precision();
    antlr4::tree::TerminalNode *TO();
    antlr4::tree::TerminalNode *HOUR();
    antlr4::tree::TerminalNode *MINUTE();
    antlr4::tree::TerminalNode *SECOND();
    ScaleContext *scale();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DayTimeIntervalTypeContext* dayTimeIntervalType();

  class  FixedLenCharContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::CollationNameContext *collate = nullptr;
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedLenCharContext* fixedLenChar();

  class  VaryingLenCharContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::CollationNameContext *collate = nullptr;
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VaryingLenCharContext* varyingLenChar();

  class  AsSubqueryContext : public antlr4::ParserRuleContext {
  public:
    AsSubqueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SelectOrValuesContext *selectOrValues();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectOrValuesContext* selectOrValues();

  class  SelectListContext : public antlr4::ParserRuleContext {
  public:
    SelectListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SelectListItemContext *> selectListItem();
    SelectListItemContext* selectListItem(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectListContext* selectList();

  class  SelectListItemContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *as = nullptr;
    sqlParser::ColumnWildcardContext *wild = nullptr;
    SelectListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();
    IdentifierContext *identifier();
    antlr4::tree::TerminalNode *AS();
    ColumnWildcardContext *columnWildcard();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectListItemContext* selectListItem();

  class  GroupByListContext : public antlr4::ParserRuleContext {
  public:
    GroupByListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<GroupByListItemContext *> groupByListItem();
    GroupByListItemContext* groupByListItem(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupByListContext* groupByList();

  class  GroupByListItemContext : public antlr4::ParserRuleContext {
  public:
    GroupByListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroupByListItemContext* groupByListItem();

  class  OrderByListContext : public antlr4::ParserRuleContext {
  public:
    OrderByListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<OrderByListItemContext *> orderByListItem();
    OrderByListItemContext* orderByListItem(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrderByListContext* orderByList();

  class  OrderByListItemContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *desc = nullptr;
    OrderByListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();
    antlr4::tree::TerminalNode *ASC();
    antlr4::tree::TerminalNode *DESC();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OrderByListItemContext* orderByListItem();

  class  JoinUsingListContext : public antlr4::ParserRuleContext {
  public:
    JoinUsingListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<JoinUsingListItemContext *> joinUsingListItem();
    JoinUsingListItemContext* joinUsingListItem(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  JoinUsingListContext* joinUsingList();

  class  JoinUsingListItemContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    JoinUsingListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  JoinUsingListItemContext* joinUsingListItem();

  class  CorrListContext : public antlr4::ParserRuleContext {
  public:
    CorrListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<CorrListItemContext *> corrListItem();
    CorrListItemContext* corrListItem(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CorrListContext* corrList();

  class  CorrListItemContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    CorrListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CorrListItemContext* corrListItem();

  class  ColAliasesContext : public antlr4::ParserRuleContext {
  public:
    ColAliasesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ColAliasListContext *colAliasList();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColAliasesContext* colAliases();

  class  ColAliasListContext : public antlr4::ParserRuleContext {
  public:
    ColAliasListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ColAliasListItemContext *> colAliasListItem();
    ColAliasListItemContext* colAliasListItem(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColAliasListContext* colAliasList();

  class  ColAliasListItemContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    ColAliasListItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColAliasListItemContext* colAliasListItem();

  class  PropertyListContext : public antlr4::ParserRuleContext {
  public:
    PropertyListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PropertyContext *> property();
    PropertyContext* property(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyListContext* propertyList();

  class  PropertyContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::PropertyNameContext *name = nullptr;
    sqlParser::PropertyValueContext *value = nullptr;
    PropertyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropertyNameContext *propertyName();
    PropertyValueContext *propertyValue();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyContext* property();

  class  ValueListContext : public antlr4::ParserRuleContext {
  public:
    ValueListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueListContext* valueList();

  class  PredicateContext : public antlr4::ParserRuleContext {
  public:
    PredicateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PredicateContext* predicate();

  class  JoinPredicateContext : public antlr4::ParserRuleContext {
  public:
    JoinPredicateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  JoinPredicateContext* joinPredicate();

  class  CasePredicateContext : public antlr4::ParserRuleContext {
  public:
    CasePredicateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ScalarFuncContext : public ExprContext {
  public:
    ScalarFuncContext(ExprContext *ctx);

    sqlParser::FunctionNameContext *func = nullptr;
    FunctionNameContext *functionName();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  UnaryOpContext : public ExprContext {
  public:
    UnaryOpContext(ExprContext *ctx);

    antlr4::Token *op = nullptr;
    ExprContext *expr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  CoalesceOpContext : public ExprContext {
  public:
    CoalesceOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *COALESCE();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  VarRefContext : public ExprContext {
  public:
    VarRefContext(ExprContext *ctx);

    VariableNameContext *variableName();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PositionFuncContext : public ExprContext {
  public:
    PositionFuncContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *POSITION();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *IN();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ColRefContext : public ExprContext {
  public:
    ColRefContext(ExprContext *ctx);

    ColumnRefContext *columnRef();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LiteralValContext : public ExprContext {
  public:
    LiteralValContext(ExprContext *ctx);

    LiteralContext *literal();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IfNullOpContext : public ExprContext {
  public:
    IfNullOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *IFNULL();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SubqueryExistsOpContext : public ExprContext {
  public:
    SubqueryExistsOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *EXISTS();
    QueryContext *query();
    antlr4::tree::TerminalNode *NOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IsNullOpContext : public ExprContext {
  public:
    IsNullOpContext(ExprContext *ctx);

    ExprContext *expr();
    antlr4::tree::TerminalNode *IS();
    antlr4::tree::TerminalNode *SQL_NULL();
    antlr4::tree::TerminalNode *NOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  InOpContext : public ExprContext {
  public:
    InOpContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *IN();
    antlr4::tree::TerminalNode *NOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LikeOpContext : public ExprContext {
  public:
    LikeOpContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LIKE();
    antlr4::tree::TerminalNode *NOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TypeOfOpContext : public ExprContext {
  public:
    TypeOfOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *TYPEOF();
    ExprContext *expr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NullIfOpContext : public ExprContext {
  public:
    NullIfOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *NULLIF();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  InlineCastContext : public ExprContext {
  public:
    InlineCastContext(ExprContext *ctx);

    DataTypeContext *dataType();
    ExprContext *expr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BinaryOpContext : public ExprContext {
  public:
    BinaryOpContext(ExprContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BetweenOpContext : public ExprContext {
  public:
    BetweenOpContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *BETWEEN();
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *NOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DecodeOpContext : public ExprContext {
  public:
    DecodeOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *DECODE();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ConcatOpContext : public ExprContext {
  public:
    ConcatOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *CONCAT();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SubqueryExprContext : public ExprContext {
  public:
    SubqueryExprContext(ExprContext *ctx);

    QueryContext *query();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IfOpContext : public ExprContext {
  public:
    IfOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *IF();
    CasePredicateContext *casePredicate();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LikeEscapeOpContext : public ExprContext {
  public:
    LikeEscapeOpContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LIKE();
    antlr4::tree::TerminalNode *ESCAPE();
    antlr4::tree::TerminalNode *NOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  SubqueryInOpContext : public ExprContext {
  public:
    SubqueryInOpContext(ExprContext *ctx);

    ExprContext *expr();
    antlr4::tree::TerminalNode *IN();
    QueryContext *query();
    antlr4::tree::TerminalNode *NOT();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LogicOpContext : public ExprContext {
  public:
    LogicOpContext(ExprContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *XOR();
    antlr4::tree::TerminalNode *OR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  CastOpContext : public ExprContext {
  public:
    CastOpContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *CAST();
    ExprContext *expr();
    DataTypeContext *dataType();
    antlr4::tree::TerminalNode *AS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ParentheticContext : public ExprContext {
  public:
    ParentheticContext(ExprContext *ctx);

    ExprContext *expr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  MethodCallContext : public ExprContext {
  public:
    MethodCallContext(ExprContext *ctx);

    sqlParser::MethodNameContext *func = nullptr;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    MethodNameContext *methodName();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnRefContext* columnRef();

  class  ColumnWildcardContext : public antlr4::ParserRuleContext {
  public:
    ColumnWildcardContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TableNameContext *tableName();
    SchemaNameContext *schemaName();
    CatalogNameContext *catalogName();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnWildcardContext* columnWildcard();

  class  ClassNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    ClassNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();
    std::vector<QualifierContext *> qualifier();
    QualifierContext* qualifier(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassNameContext* className();

  class  FunctionNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    FunctionNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionNameContext* functionName();

  class  MethodNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    MethodNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodNameContext* methodName();

  class  CatalogNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    CatalogNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CatalogNameContext* catalogName();

  class  SchemaNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    SchemaNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SchemaNameContext* schemaName();

  class  TableNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    TableNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableNameContext* tableName();

  class  TableAliasContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    TableAliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableAliasContext* tableAlias();

  class  ColumnNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    ColumnNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnNameContext* columnName();

  class  ColumnAliasContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    ColumnAliasContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ColumnAliasContext* columnAlias();

  class  CollationNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    CollationNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CollationNameContext* collationName();

  class  IndexNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    IndexNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IndexNameContext* indexName();

  class  PropertyNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    PropertyNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyNameContext* propertyName();

  class  HintNameContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    HintNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HintNameContext* hintName();

  class  ExprHintContext : public antlr4::ParserRuleContext {
  public:
    ExprHintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HASHKEY();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExprHintContext* exprHint();

  class  PropertyValueContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::SimpleLiteralContext *lit = nullptr;
    sqlParser::IdentifierContext *id = nullptr;
    PropertyValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleLiteralContext *simpleLiteral();
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PropertyValueContext* propertyValue();

  class  HintValueContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::SimpleLiteralContext *lit = nullptr;
    sqlParser::IdentifierContext *id = nullptr;
    sqlParser::ExprContext *expression = nullptr;
    HintValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleLiteralContext *simpleLiteral();
    IdentifierContext *identifier();
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  HintValueContext* hintValue();

  class  IdentifierContext : public antlr4::ParserRuleContext {
  public:
    IdentifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PLAIN_IDENTIFIER();
    antlr4::tree::TerminalNode *BAKTIKD_IDENTIFIER();
    antlr4::tree::TerminalNode *DUBLQUOTED_IDENTIFIER();
    NonReservedWordContext *nonReservedWord();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdentifierContext* identifier();

  class  QualifierContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::IdentifierContext *id = nullptr;
    QualifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierContext *identifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QualifierContext* qualifier();

  class  PrecisionContext : public antlr4::ParserRuleContext {
  public:
    PrecisionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerLiteralContext *integerLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrecisionContext* precision();

  class  ScaleContext : public antlr4::ParserRuleContext {
  public:
    ScaleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerLiteralContext *integerLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ScaleContext* scale();

  class  SizeContext : public antlr4::ParserRuleContext {
  public:
    SizeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerLiteralContext *integerLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleLiteralContext* simpleLiteral();

  class  BooleanLiteralContext : public antlr4::ParserRuleContext {
  public:
    BooleanLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SQL_TRUE();
    antlr4::tree::TerminalNode *SQL_FALSE();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BooleanLiteralContext* booleanLiteral();

  class  IntegerLiteralContext : public antlr4::ParserRuleContext {
  public:
    IntegerLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerLiteralContext* integerLiteral();

  class  SignedIntegerLiteralContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *neg = nullptr;
    SignedIntegerLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignedIntegerLiteralContext* signedIntegerLiteral();

  class  DecimalLiteralContext : public antlr4::ParserRuleContext {
  public:
    DecimalLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DecimalLiteralContext* decimalLiteral();

  class  SignedDecimalLiteralContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *neg = nullptr;
    SignedDecimalLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignedDecimalLiteralContext* signedDecimalLiteral();

  class  FloatLiteralContext : public antlr4::ParserRuleContext {
  public:
    FloatLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FLOAT_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FloatLiteralContext* floatLiteral();

  class  SignedFloatLiteralContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *neg = nullptr;
    SignedFloatLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FLOAT_LITERAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignedFloatLiteralContext* signedFloatLiteral();

  class  DateLiteralContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::StringLiteralContext *str = nullptr;
    DateLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DATE();
    StringLiteralContext *stringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DateLiteralContext* dateLiteral();

  class  TimeLiteralContext : public antlr4::ParserRuleContext {
  public:
    sqlParser::StringLiteralContext *str = nullptr;
    TimeLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TIME();
    StringLiteralContext *stringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TimeLiteralContext* timeLiteral();

  class  TimestampLiteralContext : public antlr4::ParserRuleContext {
  public:
    TimestampLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TIMESTAMP();
    StringLiteralContext *stringLiteral();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TimestampLiteralContext* timestampLiteral();

  class  StringLiteralContext : public antlr4::ParserRuleContext {
  public:
    StringLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SINGLQUOTED_STRING();
    antlr4::tree::TerminalNode *DUBLQUOTED_STRING();
    antlr4::tree::TerminalNode *DUBLQUOTED_IDENTIFIER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StringLiteralContext* stringLiteral();

  class  IntervalLiteralContext : public antlr4::ParserRuleContext {
  public:
    IntervalLiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    YearMonthIntervalContext *yearMonthInterval();
    DayTimeIntervalContext *dayTimeInterval();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntervalLiteralContext* intervalLiteral();

  class  YearMonthIntervalContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *neg = nullptr;
    antlr4::Token *year = nullptr;
    antlr4::Token *month = nullptr;
    YearMonthIntervalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTERVAL();
    StringLiteralContext *stringLiteral();
    antlr4::tree::TerminalNode *TO();
    antlr4::tree::TerminalNode *YEAR();
    antlr4::tree::TerminalNode *MONTH();
    PrecisionContext *precision();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  YearMonthIntervalContext* yearMonthInterval();

  class  DayTimeIntervalContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *neg = nullptr;
    antlr4::Token *day = nullptr;
    antlr4::Token *hour = nullptr;
    antlr4::Token *min = nullptr;
    antlr4::Token *sec = nullptr;
    DayTimeIntervalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
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


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DayTimeIntervalContext* dayTimeInterval();

  class  NonReservedWordContext : public antlr4::ParserRuleContext {
  public:
    NonReservedWordContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ANALYSE();
    antlr4::tree::TerminalNode *ANALYZE();
    antlr4::tree::TerminalNode *ANIDX();
    antlr4::tree::TerminalNode *ANTBL();
    antlr4::tree::TerminalNode *ASC();
    antlr4::tree::TerminalNode *BIT();
    antlr4::tree::TerminalNode *BITS();
    antlr4::tree::TerminalNode *BLOCK();
    antlr4::tree::TerminalNode *BLOOM();
    antlr4::tree::TerminalNode *BPA();
    antlr4::tree::TerminalNode *BTREE();
    antlr4::tree::TerminalNode *BYE();
    antlr4::tree::TerminalNode *BYTE();
    antlr4::tree::TerminalNode *BYTES();
    antlr4::tree::TerminalNode *CATALOG();
    antlr4::tree::TerminalNode *CHARS();
    antlr4::tree::TerminalNode *COMPACT();
    antlr4::tree::TerminalNode *COMPACTIDX();
    antlr4::tree::TerminalNode *CONCAT();
    antlr4::tree::TerminalNode *CONFIG();
    antlr4::tree::TerminalNode *DATA();
    antlr4::tree::TerminalNode *DATE_FORMAT();
    antlr4::tree::TerminalNode *DAYS();
    antlr4::tree::TerminalNode *DECODE();
    antlr4::tree::TerminalNode *DESC();
    antlr4::tree::TerminalNode *DETAIL();
    antlr4::tree::TerminalNode *DIGITS();
    antlr4::tree::TerminalNode *DISABLE();
    antlr4::tree::TerminalNode *DLOAD();
    antlr4::tree::TerminalNode *DOCOL();
    antlr4::tree::TerminalNode *ECHO();
    antlr4::tree::TerminalNode *ENABLE();
    antlr4::tree::TerminalNode *ENCLOSED_BY_LEADING();
    antlr4::tree::TerminalNode *ENCLOSED_BY_TRAILING();
    antlr4::tree::TerminalNode *ESCAPE_CHAR();
    antlr4::tree::TerminalNode *EXIT_TOKEN();
    antlr4::tree::TerminalNode *EXPLAIN();
    antlr4::tree::TerminalNode *EXPORT();
    antlr4::tree::TerminalNode *SQL_FALSE();
    antlr4::tree::TerminalNode *FEDERATION();
    antlr4::tree::TerminalNode *FIELD_DELIMITER();
    antlr4::tree::TerminalNode *FILE();
    antlr4::tree::TerminalNode *FORCE();
    antlr4::tree::TerminalNode *FORMAT();
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *HASHKEY();
    antlr4::tree::TerminalNode *HOURS();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *IFNULL();
    antlr4::tree::TerminalNode *INDEX();
    antlr4::tree::TerminalNode *INTERVAL_DT();
    antlr4::tree::TerminalNode *INTERVAL_YM();
    antlr4::tree::TerminalNode *KEY();
    antlr4::tree::TerminalNode *LIMIT();
    antlr4::tree::TerminalNode *LOAD();
    antlr4::tree::TerminalNode *LOADREPLICATED();
    antlr4::tree::TerminalNode *LOG();
    antlr4::tree::TerminalNode *LOOP();
    antlr4::tree::TerminalNode *LSIDX();
    antlr4::tree::TerminalNode *LSPTNCOL();
    antlr4::tree::TerminalNode *LSTBL();
    antlr4::tree::TerminalNode *MEMORYLEAKS();
    antlr4::tree::TerminalNode *METRICSCTL();
    antlr4::tree::TerminalNode *MILLIS();
    antlr4::tree::TerminalNode *MILLISECOND();
    antlr4::tree::TerminalNode *MILLISECONDS();
    antlr4::tree::TerminalNode *MINUTES();
    antlr4::tree::TerminalNode *MONOTONIC();
    antlr4::tree::TerminalNode *MONTHS();
    antlr4::tree::TerminalNode *MS();
    antlr4::tree::TerminalNode *MULTIPLE();
    antlr4::tree::TerminalNode *NODE();
    antlr4::tree::TerminalNode *NODES();
    antlr4::tree::TerminalNode *NOOP();
    antlr4::tree::TerminalNode *NOP();
    antlr4::tree::TerminalNode *NOW();
    antlr4::tree::TerminalNode *OBJECTS();
    antlr4::tree::TerminalNode *OFF();
    antlr4::tree::TerminalNode *OFFSET();
    antlr4::tree::TerminalNode *ORIGIN();
    antlr4::tree::TerminalNode *PARTITIONS();
    antlr4::tree::TerminalNode *PATHS();
    antlr4::tree::TerminalNode *PIPELINE();
    antlr4::tree::TerminalNode *PLACING();
    antlr4::tree::TerminalNode *POOL();
    antlr4::tree::TerminalNode *PRETTY();
    antlr4::tree::TerminalNode *QUARTER();
    antlr4::tree::TerminalNode *QUARTERS();
    antlr4::tree::TerminalNode *RECORD_ENDS();
    antlr4::tree::TerminalNode *RECORD_TERMINATOR();
    antlr4::tree::TerminalNode *REFERENCE();
    antlr4::tree::TerminalNode *REGIONS();
    antlr4::tree::TerminalNode *REPLICATED();
    antlr4::tree::TerminalNode *ROWSIZE();
    antlr4::tree::TerminalNode *S_ONLY();
    antlr4::tree::TerminalNode *SCHEMA();
    antlr4::tree::TerminalNode *SEC();
    antlr4::tree::TerminalNode *SECONDS();
    antlr4::tree::TerminalNode *SKIPLIST();
    antlr4::tree::TerminalNode *SLEEP();
    antlr4::tree::TerminalNode *SPACE();
    antlr4::tree::TerminalNode *STAGE();
    antlr4::tree::TerminalNode *STATS();
    antlr4::tree::TerminalNode *STATISTICS();
    antlr4::tree::TerminalNode *STATUS();
    antlr4::tree::TerminalNode *SUBSTR();
    antlr4::tree::TerminalNode *SYSREPORT();
    antlr4::tree::TerminalNode *SZIDX();
    antlr4::tree::TerminalNode *SZTBL();
    antlr4::tree::TerminalNode *TABLESIZE();
    antlr4::tree::TerminalNode *TEXT();
    antlr4::tree::TerminalNode *THREADS();
    antlr4::tree::TerminalNode *TOP();
    antlr4::tree::TerminalNode *TRACE();
    antlr4::tree::TerminalNode *TRACKMEMORY();
    antlr4::tree::TerminalNode *TREE();
    antlr4::tree::TerminalNode *TRUNCATE();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *TYPEOF();
    antlr4::tree::TerminalNode *UNLOAD();
    antlr4::tree::TerminalNode *VALUE();
    antlr4::tree::TerminalNode *WEEK();
    antlr4::tree::TerminalNode *WEEKS();
    antlr4::tree::TerminalNode *WITH_NAMES();
    antlr4::tree::TerminalNode *WITH_TYPES();
    antlr4::tree::TerminalNode *XOR();
    antlr4::tree::TerminalNode *YEARS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NonReservedWordContext* nonReservedWord();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool setExprSempred(SetExprContext *_localctx, size_t predicateIndex);
  bool tableExprSempred(TableExprContext *_localctx, size_t predicateIndex);
  bool exprSempred(ExprContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

}  // namespace Parser
