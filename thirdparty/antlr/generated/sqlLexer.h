
// Generated from /Users/craig/repos/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"


namespace Parser {


class  sqlLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, ALL = 20, 
    AND = 21, ANTI = 22, ANY = 23, AS = 24, ASC = 25, AVG = 26, BETWEEN = 27, 
    BINARY = 28, BIT = 29, BITS = 30, BLOB = 31, BLOCK = 32, BLOOM = 33, 
    BOTH = 34, BOOLEAN = 35, BPA = 36, BTREE = 37, BY = 38, BYTE = 39, BYTES = 40, 
    CASE = 41, CAST = 42, CATALOG = 43, CHAR = 44, CHARS = 45, CHARACTER = 46, 
    COALESCE = 47, COLLATE = 48, CONCAT = 49, CORRESPONDING = 50, COUNT = 51, 
    CREATE = 52, CROSS = 53, CURRENT_CATALOG = 54, CURRENT_SCHEMA = 55, 
    CURRENT_TIMESTAMP = 56, CURRENT_USER = 57, DATA = 58, DATE = 59, DATE_FORMAT = 60, 
    DAY = 61, DAYS = 62, DECIMAL = 63, DECODE = 64, DELETE = 65, DESC = 66, 
    DETAIL = 67, DIGITS = 68, DISTINCT = 69, DLOAD = 70, DROP = 71, ELSE = 72, 
    END = 73, ESCAPE = 74, ESCAPE_CHAR = 75, EXCEPT = 76, EXISTS = 77, EXPLAIN = 78, 
    EXTRACT = 79, SQL_FALSE = 80, FEDERATION = 81, FIELD_DELIMITER = 82, 
    FILE = 83, FLOAT = 84, FOR = 85, FORCE = 86, FORMAT = 87, FULL = 88, 
    FROM = 89, GROUP = 90, HASH = 91, HASHKEY = 92, HAVING = 93, HEAP = 94, 
    HOUR = 95, HOURS = 96, IF = 97, IFNULL = 98, IN = 99, INDEX = 100, INNER = 101, 
    INSERT = 102, INTEGER = 103, INTERSECT = 104, INTERVAL = 105, INTO = 106, 
    IS = 107, JOIN = 108, KEY = 109, LEADING = 110, LEFT = 111, LIKE = 112, 
    LIMIT = 113, LOAD = 114, LOOP = 115, LSCOL = 116, LSIDX = 117, LSIDXCOL = 118, 
    LSTBL = 119, MAX = 120, MEMORYLEAKS = 121, MIN = 122, MINUTE = 123, 
    MINUTES = 124, MONOTONIC = 125, MONTH = 126, MONTHS = 127, NATURAL = 128, 
    NO = 129, NODE = 130, NOOP = 131, NOT = 132, NOW = 133, SQL_NULL = 134, 
    NULLIF = 135, OBJECTS = 136, OFFSET = 137, ON = 138, OR = 139, ORDER = 140, 
    OUTER = 141, OVER = 142, OVERLAY = 143, PARTITION = 144, PATHS = 145, 
    PLACING = 146, POSITION = 147, PRETTY = 148, PRIMARY = 149, QUARTER = 150, 
    QUARTERS = 151, RECURSIVE = 152, REFERENCE = 153, REGIONS = 154, REPLICATED = 155, 
    RIGHT = 156, ROW = 157, ROWSIZE = 158, SCHEMA = 159, SECOND = 160, SECONDS = 161, 
    SELECT = 162, SEMI = 163, SET = 164, SKIPLIST = 165, SPACE = 166, STAGE = 167, 
    STATS = 168, STATISTICS = 169, STATUS = 170, SUBSTR = 171, SUBSTRING = 172, 
    SUM = 173, SZIDX = 174, SZTBL = 175, TABLE = 176, TABLESIZE = 177, TEXT = 178, 
    THEN = 179, TIME = 180, TIMESTAMP = 181, TO = 182, TOP = 183, TRACKMEMORY = 184, 
    TREE = 185, TRIM = 186, TRAILING = 187, TRUNCATE = 188, SQL_TRUE = 189, 
    TYPE = 190, TYPEOF = 191, UNION = 192, UNIQUE = 193, UPDATE = 194, USING = 195, 
    VALUE = 196, VALUES = 197, VARBINARY = 198, VARCHAR = 199, VARYING = 200, 
    WEEK = 201, WEEKS = 202, WHEN = 203, WHERE = 204, WITH_NAMES = 205, 
    WITH_TYPES = 206, WITH = 207, XOR = 208, YEAR = 209, YEARS = 210, PLAIN_IDENTIFIER = 211, 
    BAKTIKD_IDENTIFIER = 212, DUBLQUOTED_IDENTIFIER = 213, SINGLQUOTED_STRING = 214, 
    DUBLQUOTED_STRING = 215, FLOAT_LITERAL = 216, DECIMAL_LITERAL = 217, 
    INTEGER_LITERAL = 218, HINT_START = 219, HINT_END = 220, SLASH_COMMENT = 221, 
    SQL_COMMENT = 222, BLOCK_COMMENT = 223, WHITESPACE = 224
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
