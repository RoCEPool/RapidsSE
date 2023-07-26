
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
    T__20 = 21, ALL = 22, AND = 23, ANTI = 24, ANY = 25, AS = 26, ASC = 27, 
    AVG = 28, BEGIN = 29, BETWEEN = 30, BINARY = 31, BIT = 32, BITS = 33, 
    BLOB = 34, BLOCK = 35, BLOOM = 36, BOTH = 37, BOOLEAN = 38, BPA = 39, 
    BTREE = 40, BY = 41, BYTE = 42, BYTES = 43, CASE = 44, CAST = 45, CATALOG = 46, 
    CHAR = 47, CHARS = 48, CHARACTER = 49, COALESCE = 50, COLLATE = 51, 
    CONCAT = 52, CORRESPONDING = 53, COUNT = 54, CREATE = 55, CROSS = 56, 
    CURRENT_CATALOG = 57, CURRENT_SCHEMA = 58, CURRENT_TIMESTAMP = 59, CURRENT_USER = 60, 
    DATA = 61, DATE = 62, DATE_FORMAT = 63, DAY = 64, DAYS = 65, DECIMAL = 66, 
    DECODE = 67, DELETE = 68, DESC = 69, DETAIL = 70, DIGITS = 71, DISTINCT = 72, 
    DLOAD = 73, DROP = 74, ECHO = 75, ELSE = 76, END = 77, ESCAPE = 78, 
    ESCAPE_CHAR = 79, EXCEPT = 80, EXISTS = 81, EXPLAIN = 82, EXTRACT = 83, 
    SQL_FALSE = 84, FEDERATION = 85, FIELD_DELIMITER = 86, FILE = 87, FLOAT = 88, 
    FOR = 89, FORCE = 90, FORMAT = 91, FULL = 92, FROM = 93, GROUP = 94, 
    HASH = 95, HASHKEY = 96, HAVING = 97, HEAP = 98, HOUR = 99, HOURS = 100, 
    IF = 101, IFNULL = 102, IN = 103, INDEX = 104, INNER = 105, INSERT = 106, 
    INTEGER = 107, INTERSECT = 108, INTERVAL_DT = 109, INTERVAL_YM = 110, 
    INTERVAL = 111, INTO = 112, IS = 113, JOIN = 114, KEY = 115, LEADING = 116, 
    LEFT = 117, LIKE = 118, LIMIT = 119, LOAD = 120, LOG = 121, LOOP = 122, 
    LSCOL = 123, LSIDX = 124, LSIDXCOL = 125, LSTBL = 126, MAX = 127, MEMORYLEAKS = 128, 
    MILLIS = 129, MILLISECOND = 130, MILLISECONDS = 131, MIN = 132, MINUTE = 133, 
    MINUTES = 134, MONOTONIC = 135, MONTH = 136, MONTHS = 137, MS = 138, 
    NATURAL = 139, NO = 140, NODES = 141, NODE = 142, NOOP = 143, NOP = 144, 
    NOT = 145, NOW = 146, SQL_NULL = 147, NULLIF = 148, OBJECTS = 149, OFF = 150, 
    OFFSET = 151, ON = 152, OR = 153, ORDER = 154, ORIGIN = 155, OUTER = 156, 
    OVER = 157, OVERLAY = 158, PARTITIONS = 159, PARTITION = 160, PATHS = 161, 
    PIPELINE = 162, PLACING = 163, POSITION = 164, PRETTY = 165, PRIMARY = 166, 
    QUARTER = 167, QUARTERS = 168, RECURSIVE = 169, REFERENCE = 170, REGIONS = 171, 
    REPLICATED = 172, RIGHT = 173, ROW = 174, ROWSIZE = 175, SCHEMA = 176, 
    SEC = 177, SECOND = 178, SECONDS = 179, SELECT = 180, SEMI = 181, SET = 182, 
    SKIPLIST = 183, SLEEP = 184, SPACE = 185, STAGE = 186, STATS = 187, 
    STATISTICS = 188, STATUS = 189, SUBSTR = 190, SUBSTRING = 191, SUM = 192, 
    SZIDX = 193, SZTBL = 194, S_ONLY = 195, TABLE = 196, TABLESIZE = 197, 
    TEXT = 198, THEN = 199, TIME = 200, TIMESTAMP = 201, TO = 202, TOP = 203, 
    TRACKMEMORY = 204, TREE = 205, TRIM = 206, TRAILING = 207, TRUNCATE = 208, 
    SQL_TRUE = 209, TYPE = 210, TYPEOF = 211, UNION = 212, UNIQUE = 213, 
    UNLOAD = 214, UPDATE = 215, USING = 216, VALUE = 217, VALUES = 218, 
    VARBINARY = 219, VARCHAR = 220, VARYING = 221, WEEK = 222, WEEKS = 223, 
    WHEN = 224, WHERE = 225, WITH_NAMES = 226, WITH_TYPES = 227, WITH = 228, 
    XOR = 229, YEAR = 230, YEARS = 231, PLAIN_IDENTIFIER = 232, BAKTIKD_IDENTIFIER = 233, 
    DUBLQUOTED_IDENTIFIER = 234, PARAM_IDENTIFIER = 235, SINGLQUOTED_STRING = 236, 
    DUBLQUOTED_STRING = 237, FLOAT_LITERAL = 238, DECIMAL_LITERAL = 239, 
    INTEGER_LITERAL = 240, HINT_START = 241, HINT_END = 242, SLASH_COMMENT = 243, 
    SQL_COMMENT = 244, BLOCK_COMMENT = 245, WHITESPACE = 246
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
