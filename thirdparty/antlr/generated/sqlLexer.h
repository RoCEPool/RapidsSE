
// Generated from /Users/alka/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"


namespace Parser {


class  sqlLexer : public antlr4::Lexer {
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

  explicit sqlLexer(antlr4::CharStream *input);

  ~sqlLexer() override;


    public: bool isDublQuoteId = true;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.
  bool DUBLQUOTED_IDENTIFIERSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);

};

}  // namespace Parser
