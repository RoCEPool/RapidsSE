
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
    COMPACTIDX = 52, CONCAT = 53, CORRESPONDING = 54, COUNT = 55, CREATE = 56, 
    CROSS = 57, CURRENT_CATALOG = 58, CURRENT_SCHEMA = 59, CURRENT_TIMESTAMP = 60, 
    CURRENT_USER = 61, DATA = 62, DATE = 63, DATE_FORMAT = 64, DAY = 65, 
    DAYS = 66, DECIMAL = 67, DECODE = 68, DELETE = 69, DESC = 70, DETAIL = 71, 
    DIGITS = 72, DISTINCT = 73, DLOAD = 74, DROP = 75, ECHO = 76, ELSE = 77, 
    END = 78, ESCAPE = 79, ESCAPE_CHAR = 80, EXCEPT = 81, EXISTS = 82, EXPLAIN = 83, 
    EXTRACT = 84, SQL_FALSE = 85, FEDERATION = 86, FIELD_DELIMITER = 87, 
    FILE = 88, FLOAT = 89, FOR = 90, FORCE = 91, FORMAT = 92, FULL = 93, 
    FROM = 94, GROUP = 95, HASH = 96, HASHKEY = 97, HAVING = 98, HEAP = 99, 
    HOUR = 100, HOURS = 101, IF = 102, IFNULL = 103, IN = 104, INDEX = 105, 
    INNER = 106, INSERT = 107, INTEGER = 108, INTERSECT = 109, INTERVAL_DT = 110, 
    INTERVAL_YM = 111, INTERVAL = 112, INTO = 113, IS = 114, JOIN = 115, 
    KEY = 116, LEADING = 117, LEFT = 118, LIKE = 119, LIMIT = 120, LOADREPLICATED = 121, 
    LOAD = 122, LOG = 123, LOOP = 124, LSCOL = 125, LSIDX = 126, LSIDXCOL = 127, 
    LSTBL = 128, MAX = 129, MEMORYLEAKS = 130, MILLIS = 131, MILLISECOND = 132, 
    MILLISECONDS = 133, MIN = 134, MINUTE = 135, MINUTES = 136, MONOTONIC = 137, 
    MONTH = 138, MONTHS = 139, MS = 140, NATURAL = 141, NO = 142, NODES = 143, 
    NODE = 144, NOOP = 145, NOP = 146, NOT = 147, NOW = 148, SQL_NULL = 149, 
    NULLIF = 150, OBJECTS = 151, OFF = 152, OFFSET = 153, ON = 154, OR = 155, 
    ORDER = 156, ORIGIN = 157, OUTER = 158, OVER = 159, OVERLAY = 160, PARTITIONS = 161, 
    PARTITION = 162, PATHS = 163, PIPELINE = 164, PLACING = 165, POSITION = 166, 
    PRETTY = 167, PRIMARY = 168, QUARTER = 169, QUARTERS = 170, RECURSIVE = 171, 
    REFERENCE = 172, REGIONS = 173, REPLICATED = 174, RIGHT = 175, ROW = 176, 
    ROWSIZE = 177, SCHEMA = 178, SEC = 179, SECOND = 180, SECONDS = 181, 
    SELECT = 182, SEMI = 183, SET = 184, SKIPLIST = 185, SLEEP = 186, SPACE = 187, 
    STAGE = 188, STATS = 189, STATISTICS = 190, STATUS = 191, SUBSTR = 192, 
    SUBSTRING = 193, SUM = 194, SZIDX = 195, SZTBL = 196, S_ONLY = 197, 
    TABLE = 198, TABLESIZE = 199, TEXT = 200, THEN = 201, TIME = 202, TIMESTAMP = 203, 
    TO = 204, TOP = 205, TRACKMEMORY = 206, TREE = 207, TRIM = 208, TRAILING = 209, 
    TRUNCATE = 210, SQL_TRUE = 211, TYPE = 212, TYPEOF = 213, UNION = 214, 
    UNIQUE = 215, UNLOAD = 216, UPDATE = 217, USING = 218, VALUE = 219, 
    VALUES = 220, VARBINARY = 221, VARCHAR = 222, VARYING = 223, WEEK = 224, 
    WEEKS = 225, WHEN = 226, WHERE = 227, WITH_NAMES = 228, WITH_TYPES = 229, 
    WITH = 230, XOR = 231, YEAR = 232, YEARS = 233, PLAIN_IDENTIFIER = 234, 
    BAKTIKD_IDENTIFIER = 235, DUBLQUOTED_IDENTIFIER = 236, PARAM_IDENTIFIER = 237, 
    SINGLQUOTED_STRING = 238, DUBLQUOTED_STRING = 239, FLOAT_LITERAL = 240, 
    DECIMAL_LITERAL = 241, INTEGER_LITERAL = 242, HINT_START = 243, HINT_END = 244, 
    SLASH_COMMENT = 245, SQL_COMMENT = 246, BLOCK_COMMENT = 247, WHITESPACE = 248
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
