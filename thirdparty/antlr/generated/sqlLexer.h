
// Generated from /Users/craig/repos/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.13.1

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
    DETAIL = 79, DIGITS = 80, DISCONNECT = 81, DISTINCT = 82, DLOAD = 83, 
    DOCOL = 84, DROP = 85, ECHO = 86, ELSE = 87, ENCLOSED_BY_LEADING = 88, 
    ENCLOSED_BY_TRAILING = 89, END = 90, ESCAPE = 91, ESCAPE_CHAR = 92, 
    EXCEPT = 93, EXISTS = 94, EXIT_TOKEN = 95, EXPLAIN = 96, EXPORT = 97, 
    EXTRACT = 98, SQL_FALSE = 99, FEDERATION = 100, FIELD_DELIMITER = 101, 
    FILE = 102, FLOAT = 103, FOR = 104, FORCE = 105, FORMAT = 106, FULL = 107, 
    FROM = 108, GROUP = 109, HASH = 110, HASHKEY = 111, HAVING = 112, HEAP = 113, 
    HOUR = 114, HOURS = 115, IF = 116, IFNULL = 117, IN = 118, INDEX = 119, 
    INNER = 120, INSERT = 121, INTEGER = 122, INTERSECT = 123, INTERVAL_DT = 124, 
    INTERVAL_YM = 125, INTERVAL = 126, INTO = 127, IS = 128, JOIN = 129, 
    KEY = 130, LEADING = 131, LEFT = 132, LIKE = 133, LIMIT = 134, LOADREPLICATED = 135, 
    LOAD = 136, LOG = 137, LOOP = 138, LSCOL = 139, LSIDXCOL = 140, LSIDX = 141, 
    LSPTNCOL = 142, LSTBL = 143, MAX = 144, MEMORYLEAKS = 145, MILLIS = 146, 
    MILLISECOND = 147, MILLISECONDS = 148, MIN = 149, MINUTE = 150, MINUTES = 151, 
    MONOTONIC = 152, MONTH = 153, MONTHS = 154, MS = 155, NATURAL = 156, 
    NO = 157, NODES = 158, NODE = 159, NOOP = 160, NOP = 161, NOT = 162, 
    NOW = 163, SQL_NULL = 164, NULLIF = 165, OBJECTS = 166, OFF = 167, OFFSET = 168, 
    ON = 169, OR = 170, ORDER = 171, ORIGIN = 172, OUTER = 173, OVER = 174, 
    OVERLAY = 175, PARTITIONS = 176, PARTITION = 177, PATHS = 178, PIPELINE = 179, 
    PLACING = 180, POOL = 181, POSITION = 182, PRETTY = 183, PRIMARY = 184, 
    QUARTER = 185, QUARTERS = 186, RECORD_ENDS = 187, RECORD_TERMINATOR = 188, 
    RECURSIVE = 189, REFERENCE = 190, REGIONS = 191, REPLICATED = 192, RIGHT = 193, 
    ROW = 194, ROWSIZE = 195, SCHEMA = 196, SEC = 197, SECOND = 198, SECONDS = 199, 
    SELECT = 200, SEMI = 201, SET = 202, SKIPLIST = 203, SLEEP = 204, SPACE = 205, 
    STAGE = 206, STATS = 207, STATISTICS = 208, STATUS = 209, SUBSTR = 210, 
    SUBSTRING = 211, SUM = 212, SZIDX = 213, SZTBL = 214, S_ONLY = 215, 
    TABLE = 216, TABLESIZES = 217, TABLESIZE = 218, TEXT = 219, THEN = 220, 
    TIME = 221, TIMESTAMP = 222, TO = 223, TOP = 224, TRACE = 225, TRACKMEMORY = 226, 
    TREE = 227, TRIM = 228, TRAILING = 229, TRUNCATE = 230, SQL_TRUE = 231, 
    TYPE = 232, TYPEOF = 233, UNION = 234, UNIQUE = 235, UNLOAD = 236, UPDATE = 237, 
    USING = 238, VALUE = 239, VALUES = 240, VARBINARY = 241, VARCHAR = 242, 
    VARYING = 243, WEEK = 244, WEEKS = 245, WHEN = 246, WHERE = 247, WITH_NAMES = 248, 
    WITH_TYPES = 249, WITH = 250, XOR = 251, YEAR = 252, YEARS = 253, PLAIN_IDENTIFIER = 254, 
    BAKTIKD_IDENTIFIER = 255, DUBLQUOTED_IDENTIFIER = 256, PARAM_IDENTIFIER = 257, 
    SINGLQUOTED_STRING = 258, DUBLQUOTED_STRING = 259, FLOAT_LITERAL = 260, 
    DECIMAL_LITERAL = 261, INTEGER_LITERAL = 262, HINT_START = 263, HINT_END = 264, 
    SLASH_COMMENT = 265, SQL_COMMENT = 266, BLOCK_COMMENT = 267, WHITESPACE = 268
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
