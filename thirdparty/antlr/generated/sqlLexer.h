
// Generated from /Users/craig/repos/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.8

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
    COMPACTIDX = 58, CONCAT = 59, CORRESPONDING = 60, COUNT = 61, CREATE = 62, 
    CROSS = 63, CURRENT_CATALOG = 64, CURRENT_SCHEMA = 65, CURRENT_TIMESTAMP = 66, 
    CURRENT_USER = 67, DATA = 68, DATE = 69, DATE_FORMAT = 70, DAY = 71, 
    DAYS = 72, DECIMAL = 73, DECODE = 74, DELETE = 75, DESC = 76, DETAIL = 77, 
    DIGITS = 78, DISCONNECT = 79, DISTINCT = 80, DLOAD = 81, DOCOL = 82, 
    DROP = 83, ECHO = 84, ELSE = 85, END = 86, ESCAPE = 87, ESCAPE_CHAR = 88, 
    EXCEPT = 89, EXISTS = 90, EXIT_TOKEN = 91, EXPLAIN = 92, EXPORT = 93, 
    EXTRACT = 94, SQL_FALSE = 95, FEDERATION = 96, FIELD_DELIMITER = 97, 
    FILE = 98, FLOAT = 99, FOR = 100, FORCE = 101, FORMAT = 102, FULL = 103, 
    FROM = 104, GROUP = 105, HASH = 106, HASHKEY = 107, HAVING = 108, HEAP = 109, 
    HOUR = 110, HOURS = 111, IF = 112, IFNULL = 113, IN = 114, INDEX = 115, 
    INNER = 116, INSERT = 117, INTEGER = 118, INTERSECT = 119, INTERVAL_DT = 120, 
    INTERVAL_YM = 121, INTERVAL = 122, INTO = 123, IS = 124, JOIN = 125, 
    KEY = 126, LEADING = 127, LEFT = 128, LIKE = 129, LIMIT = 130, LOADREPLICATED = 131, 
    LOAD = 132, LOG = 133, LOOP = 134, LSCOL = 135, LSIDXCOL = 136, LSIDX = 137, 
    LSPTNCOL = 138, LSTBL = 139, MAX = 140, MEMORYLEAKS = 141, MILLIS = 142, 
    MILLISECOND = 143, MILLISECONDS = 144, MIN = 145, MINUTE = 146, MINUTES = 147, 
    MONOTONIC = 148, MONTH = 149, MONTHS = 150, MS = 151, NATURAL = 152, 
    NO = 153, NODES = 154, NODE = 155, NOOP = 156, NOP = 157, NOT = 158, 
    NOW = 159, SQL_NULL = 160, NULLIF = 161, OBJECTS = 162, OFF = 163, OFFSET = 164, 
    ON = 165, OR = 166, ORDER = 167, ORIGIN = 168, OUTER = 169, OVER = 170, 
    OVERLAY = 171, PARTITIONS = 172, PARTITION = 173, PATHS = 174, PIPELINE = 175, 
    PLACING = 176, POSITION = 177, PRETTY = 178, PRIMARY = 179, QUARTER = 180, 
    QUARTERS = 181, RECORD_ENDS = 182, RECURSIVE = 183, REFERENCE = 184, 
    REGIONS = 185, REPLICATED = 186, RIGHT = 187, ROW = 188, ROWSIZE = 189, 
    SCHEMA = 190, SEC = 191, SECOND = 192, SECONDS = 193, SELECT = 194, 
    SEMI = 195, SET = 196, SKIPLIST = 197, SLEEP = 198, SPACE = 199, STAGE = 200, 
    STATS = 201, STATISTICS = 202, STATUS = 203, SUBSTR = 204, SUBSTRING = 205, 
    SUM = 206, SZIDX = 207, SZTBL = 208, S_ONLY = 209, TABLE = 210, TABLESIZES = 211, 
    TABLESIZE = 212, TEXT = 213, THEN = 214, TIME = 215, TIMESTAMP = 216, 
    TO = 217, TOP = 218, TRACKMEMORY = 219, TREE = 220, TRIM = 221, TRAILING = 222, 
    TRUNCATE = 223, SQL_TRUE = 224, TYPE = 225, TYPEOF = 226, UNION = 227, 
    UNIQUE = 228, UNLOAD = 229, UPDATE = 230, USING = 231, VALUE = 232, 
    VALUES = 233, VARBINARY = 234, VARCHAR = 235, VARYING = 236, WEEK = 237, 
    WEEKS = 238, WHEN = 239, WHERE = 240, WITH_NAMES = 241, WITH_TYPES = 242, 
    WITH = 243, XOR = 244, YEAR = 245, YEARS = 246, PLAIN_IDENTIFIER = 247, 
    BAKTIKD_IDENTIFIER = 248, DUBLQUOTED_IDENTIFIER = 249, PARAM_IDENTIFIER = 250, 
    SINGLQUOTED_STRING = 251, DUBLQUOTED_STRING = 252, FLOAT_LITERAL = 253, 
    DECIMAL_LITERAL = 254, INTEGER_LITERAL = 255, HINT_START = 256, HINT_END = 257, 
    SLASH_COMMENT = 258, SQL_COMMENT = 259, BLOCK_COMMENT = 260, WHITESPACE = 261
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
