
// Generated from /Users/alka/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.8

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
    PLACING = 180, POSITION = 181, PRETTY = 182, PRIMARY = 183, QUARTER = 184, 
    QUARTERS = 185, RECORD_ENDS = 186, RECORD_TERMINATOR = 187, RECURSIVE = 188, 
    REFERENCE = 189, REGIONS = 190, REPLICATED = 191, RIGHT = 192, ROW = 193, 
    ROWSIZE = 194, SCHEMA = 195, SEC = 196, SECOND = 197, SECONDS = 198, 
    SELECT = 199, SEMI = 200, SET = 201, SKIPLIST = 202, SLEEP = 203, SPACE = 204, 
    STAGE = 205, STATS = 206, STATISTICS = 207, STATUS = 208, SUBSTR = 209, 
    SUBSTRING = 210, SUM = 211, SZIDX = 212, SZTBL = 213, S_ONLY = 214, 
    TABLE = 215, TABLESIZES = 216, TABLESIZE = 217, TEXT = 218, THEN = 219, 
    TIME = 220, TIMESTAMP = 221, TO = 222, TOP = 223, TRACE = 224, TRACKMEMORY = 225, 
    TREE = 226, TRIM = 227, TRAILING = 228, TRUNCATE = 229, SQL_TRUE = 230, 
    TYPE = 231, TYPEOF = 232, UNION = 233, UNIQUE = 234, UNLOAD = 235, UPDATE = 236, 
    USING = 237, VALUE = 238, VALUES = 239, VARBINARY = 240, VARCHAR = 241, 
    VARYING = 242, WEEK = 243, WEEKS = 244, WHEN = 245, WHERE = 246, WITH_NAMES = 247, 
    WITH_TYPES = 248, WITH = 249, XOR = 250, YEAR = 251, YEARS = 252, PLAIN_IDENTIFIER = 253, 
    BAKTIKD_IDENTIFIER = 254, DUBLQUOTED_IDENTIFIER = 255, PARAM_IDENTIFIER = 256, 
    SINGLQUOTED_STRING = 257, DUBLQUOTED_STRING = 258, FLOAT_LITERAL = 259, 
    DECIMAL_LITERAL = 260, INTEGER_LITERAL = 261, HINT_START = 262, HINT_END = 263, 
    SLASH_COMMENT = 264, SQL_COMMENT = 265, BLOCK_COMMENT = 266, WHITESPACE = 267
  };

  sqlLexer(antlr4::CharStream *input);
  ~sqlLexer();


    public: bool isDublQuoteId = true;

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.
  bool DUBLQUOTED_IDENTIFIERSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

}  // namespace Parser
