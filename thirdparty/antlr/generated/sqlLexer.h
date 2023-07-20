
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
    AVG = 28, BETWEEN = 29, BINARY = 30, BIT = 31, BITS = 32, BLOB = 33, 
    BLOCK = 34, BLOOM = 35, BOTH = 36, BOOLEAN = 37, BPA = 38, BTREE = 39, 
    BY = 40, BYTE = 41, BYTES = 42, CASE = 43, CAST = 44, CATALOG = 45, 
    CHAR = 46, CHARS = 47, CHARACTER = 48, COALESCE = 49, COLLATE = 50, 
    CONCAT = 51, CORRESPONDING = 52, COUNT = 53, CREATE = 54, CROSS = 55, 
    CURRENT_CATALOG = 56, CURRENT_SCHEMA = 57, CURRENT_TIMESTAMP = 58, CURRENT_USER = 59, 
    DATA = 60, DATE = 61, DATE_FORMAT = 62, DAY = 63, DAYS = 64, DECIMAL = 65, 
    DECODE = 66, DELETE = 67, DESC = 68, DETAIL = 69, DIGITS = 70, DISTINCT = 71, 
    DLOAD = 72, DROP = 73, ELSE = 74, END = 75, ESCAPE = 76, ESCAPE_CHAR = 77, 
    EXCEPT = 78, EXISTS = 79, EXPLAIN = 80, EXTRACT = 81, SQL_FALSE = 82, 
    FEDERATION = 83, FIELD_DELIMITER = 84, FILE = 85, FLOAT = 86, FOR = 87, 
    FORCE = 88, FORMAT = 89, FULL = 90, FROM = 91, GROUP = 92, HASH = 93, 
    HASHKEY = 94, HAVING = 95, HEAP = 96, HOUR = 97, HOURS = 98, IF = 99, 
    IFNULL = 100, IN = 101, INDEX = 102, INNER = 103, INSERT = 104, INTEGER = 105, 
    INTERSECT = 106, INTERVAL = 107, INTO = 108, IS = 109, JOIN = 110, KEY = 111, 
    LEADING = 112, LEFT = 113, LIKE = 114, LIMIT = 115, LOAD = 116, LOOP = 117, 
    LSCOL = 118, LSIDX = 119, LSIDXCOL = 120, LSTBL = 121, MAX = 122, MEMORYLEAKS = 123, 
    MILLIS = 124, MILLISECOND = 125, MILLISECONDS = 126, MIN = 127, MINUTE = 128, 
    MINUTES = 129, MONOTONIC = 130, MONTH = 131, MONTHS = 132, MS = 133, 
    NATURAL = 134, NO = 135, NODE = 136, NOOP = 137, NOT = 138, NOW = 139, 
    SQL_NULL = 140, NULLIF = 141, OBJECTS = 142, OFFSET = 143, ON = 144, 
    OR = 145, ORDER = 146, OUTER = 147, OVER = 148, OVERLAY = 149, PARTITION = 150, 
    PATHS = 151, PLACING = 152, POSITION = 153, PRETTY = 154, PRIMARY = 155, 
    QUARTER = 156, QUARTERS = 157, RECURSIVE = 158, REFERENCE = 159, REGIONS = 160, 
    REPLICATED = 161, RIGHT = 162, ROW = 163, ROWSIZE = 164, SCHEMA = 165, 
    SEC = 166, SECOND = 167, SECONDS = 168, SELECT = 169, SEMI = 170, SET = 171, 
    SKIPLIST = 172, SLEEP = 173, SPACE = 174, STAGE = 175, STATS = 176, 
    STATISTICS = 177, STATUS = 178, SUBSTR = 179, SUBSTRING = 180, SUM = 181, 
    SZIDX = 182, SZTBL = 183, TABLE = 184, TABLESIZE = 185, TEXT = 186, 
    THEN = 187, TIME = 188, TIMESTAMP = 189, TO = 190, TOP = 191, TRACKMEMORY = 192, 
    TREE = 193, TRIM = 194, TRAILING = 195, TRUNCATE = 196, SQL_TRUE = 197, 
    TYPE = 198, TYPEOF = 199, UNION = 200, UNIQUE = 201, UPDATE = 202, USING = 203, 
    VALUE = 204, VALUES = 205, VARBINARY = 206, VARCHAR = 207, VARYING = 208, 
    WEEK = 209, WEEKS = 210, WHEN = 211, WHERE = 212, WITH_NAMES = 213, 
    WITH_TYPES = 214, WITH = 215, XOR = 216, YEAR = 217, YEARS = 218, PLAIN_IDENTIFIER = 219, 
    BAKTIKD_IDENTIFIER = 220, DUBLQUOTED_IDENTIFIER = 221, SINGLQUOTED_STRING = 222, 
    DUBLQUOTED_STRING = 223, FLOAT_LITERAL = 224, DECIMAL_LITERAL = 225, 
    INTEGER_LITERAL = 226, HINT_START = 227, HINT_END = 228, SLASH_COMMENT = 229, 
    SQL_COMMENT = 230, BLOCK_COMMENT = 231, WHITESPACE = 232
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
