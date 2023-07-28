
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
    T__20 = 21, T__21 = 22, ALL = 23, AND = 24, ANTI = 25, ANY = 26, AS = 27, 
    ASC = 28, AVG = 29, BEGIN = 30, BETWEEN = 31, BINARY = 32, BIT = 33, 
    BITS = 34, BLOB = 35, BLOCK = 36, BLOOM = 37, BOTH = 38, BOOLEAN = 39, 
    BPA = 40, BTREE = 41, BY = 42, BYTE = 43, BYTES = 44, CASE = 45, CAST = 46, 
    CATALOG = 47, CHAR = 48, CHARS = 49, CHARACTER = 50, COALESCE = 51, 
    COLLATE = 52, COMPACTIDX = 53, CONCAT = 54, CORRESPONDING = 55, COUNT = 56, 
    CREATE = 57, CROSS = 58, CURRENT_CATALOG = 59, CURRENT_SCHEMA = 60, 
    CURRENT_TIMESTAMP = 61, CURRENT_USER = 62, DATA = 63, DATE = 64, DATE_FORMAT = 65, 
    DAY = 66, DAYS = 67, DECIMAL = 68, DECODE = 69, DELETE = 70, DESC = 71, 
    DETAIL = 72, DIGITS = 73, DISTINCT = 74, DLOAD = 75, DROP = 76, ECHO = 77, 
    ELSE = 78, END = 79, ESCAPE = 80, ESCAPE_CHAR = 81, EXCEPT = 82, EXISTS = 83, 
    EXPLAIN = 84, EXTRACT = 85, SQL_FALSE = 86, FEDERATION = 87, FIELD_DELIMITER = 88, 
    FILE = 89, FLOAT = 90, FOR = 91, FORCE = 92, FORMAT = 93, FULL = 94, 
    FROM = 95, GROUP = 96, HASH = 97, HASHKEY = 98, HAVING = 99, HEAP = 100, 
    HOUR = 101, HOURS = 102, IF = 103, IFNULL = 104, IN = 105, INDEX = 106, 
    INNER = 107, INSERT = 108, INTEGER = 109, INTERSECT = 110, INTERVAL_DT = 111, 
    INTERVAL_YM = 112, INTERVAL = 113, INTO = 114, IS = 115, JOIN = 116, 
    KEY = 117, LEADING = 118, LEFT = 119, LIKE = 120, LIMIT = 121, LOADREPLICATED = 122, 
    LOAD = 123, LOG = 124, LOOP = 125, LSCOL = 126, LSIDXCOL = 127, LSIDX = 128, 
    LSPTNCOL = 129, LSTBL = 130, MAX = 131, MEMORYLEAKS = 132, MILLIS = 133, 
    MILLISECOND = 134, MILLISECONDS = 135, MIN = 136, MINUTE = 137, MINUTES = 138, 
    MONOTONIC = 139, MONTH = 140, MONTHS = 141, MS = 142, NATURAL = 143, 
    NO = 144, NODES = 145, NODE = 146, NOOP = 147, NOP = 148, NOT = 149, 
    NOW = 150, SQL_NULL = 151, NULLIF = 152, OBJECTS = 153, OFF = 154, OFFSET = 155, 
    ON = 156, OR = 157, ORDER = 158, ORIGIN = 159, OUTER = 160, OVER = 161, 
    OVERLAY = 162, PARTITIONS = 163, PARTITION = 164, PATHS = 165, PIPELINE = 166, 
    PLACING = 167, POSITION = 168, PRETTY = 169, PRIMARY = 170, QUARTER = 171, 
    QUARTERS = 172, RECURSIVE = 173, REFERENCE = 174, REGIONS = 175, REPLICATED = 176, 
    RIGHT = 177, ROW = 178, ROWSIZE = 179, SCHEMA = 180, SEC = 181, SECOND = 182, 
    SECONDS = 183, SELECT = 184, SEMI = 185, SET = 186, SKIPLIST = 187, 
    SLEEP = 188, SPACE = 189, STAGE = 190, STATS = 191, STATISTICS = 192, 
    STATUS = 193, SUBSTR = 194, SUBSTRING = 195, SUM = 196, SZIDX = 197, 
    SZTBL = 198, S_ONLY = 199, TABLE = 200, TABLESIZE = 201, TEXT = 202, 
    THEN = 203, TIME = 204, TIMESTAMP = 205, TO = 206, TOP = 207, TRACKMEMORY = 208, 
    TREE = 209, TRIM = 210, TRAILING = 211, TRUNCATE = 212, SQL_TRUE = 213, 
    TYPE = 214, TYPEOF = 215, UNION = 216, UNIQUE = 217, UNLOAD = 218, UPDATE = 219, 
    USING = 220, VALUE = 221, VALUES = 222, VARBINARY = 223, VARCHAR = 224, 
    VARYING = 225, WEEK = 226, WEEKS = 227, WHEN = 228, WHERE = 229, WITH_NAMES = 230, 
    WITH_TYPES = 231, WITH = 232, XOR = 233, YEAR = 234, YEARS = 235, PLAIN_IDENTIFIER = 236, 
    BAKTIKD_IDENTIFIER = 237, DUBLQUOTED_IDENTIFIER = 238, PARAM_IDENTIFIER = 239, 
    SINGLQUOTED_STRING = 240, DUBLQUOTED_STRING = 241, FLOAT_LITERAL = 242, 
    DECIMAL_LITERAL = 243, INTEGER_LITERAL = 244, HINT_START = 245, HINT_END = 246, 
    SLASH_COMMENT = 247, SQL_COMMENT = 248, BLOCK_COMMENT = 249, WHITESPACE = 250
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
