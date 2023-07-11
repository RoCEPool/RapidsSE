
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
    FILE = 83, FLOAT = 84, FOR = 85, FORMAT = 86, FULL = 87, FROM = 88, 
    GROUP = 89, HASH = 90, HASHKEY = 91, HAVING = 92, HEAP = 93, HOUR = 94, 
    HOURS = 95, IF = 96, IFNULL = 97, IN = 98, INDEX = 99, INNER = 100, 
    INSERT = 101, INTEGER = 102, INTERSECT = 103, INTERVAL = 104, INTO = 105, 
    IS = 106, JOIN = 107, KEY = 108, LEADING = 109, LEFT = 110, LIKE = 111, 
    LIMIT = 112, LOAD = 113, LOOP = 114, LSCOL = 115, LSIDX = 116, LSIDXCOL = 117, 
    LSTBL = 118, MAX = 119, MEMORYLEAKS = 120, MIN = 121, MINUTE = 122, 
    MINUTES = 123, MONOTONIC = 124, MONTH = 125, MONTHS = 126, NATURAL = 127, 
    NO = 128, NODE = 129, NOOP = 130, NOT = 131, NOW = 132, SQL_NULL = 133, 
    NULLIF = 134, OBJECTS = 135, OFFSET = 136, ON = 137, OR = 138, ORDER = 139, 
    OUTER = 140, OVER = 141, OVERLAY = 142, PARTITION = 143, PATHS = 144, 
    PLACING = 145, POSITION = 146, PRETTY = 147, PRIMARY = 148, QUARTER = 149, 
    QUARTERS = 150, RECURSIVE = 151, REFERENCE = 152, REGIONS = 153, REPLICATED = 154, 
    RIGHT = 155, ROW = 156, ROWSIZE = 157, SCHEMA = 158, SECOND = 159, SECONDS = 160, 
    SELECT = 161, SEMI = 162, SET = 163, SKIPLIST = 164, SPACE = 165, STAGE = 166, 
    STATS = 167, STATISTICS = 168, STATUS = 169, SUBSTR = 170, SUBSTRING = 171, 
    SUM = 172, SZIDX = 173, SZTBL = 174, TABLE = 175, TABLESIZE = 176, TEXT = 177, 
    THEN = 178, TIME = 179, TIMESTAMP = 180, TO = 181, TOP = 182, TRACKMEMORY = 183, 
    TREE = 184, TRIM = 185, TRAILING = 186, TRUNCATE = 187, SQL_TRUE = 188, 
    TYPE = 189, TYPEOF = 190, UNION = 191, UNIQUE = 192, UPDATE = 193, USING = 194, 
    VALUE = 195, VALUES = 196, VARBINARY = 197, VARCHAR = 198, VARYING = 199, 
    WEEK = 200, WEEKS = 201, WHEN = 202, WHERE = 203, WITH_NAMES = 204, 
    WITH_TYPES = 205, WITH = 206, XOR = 207, YEAR = 208, YEARS = 209, PLAIN_IDENTIFIER = 210, 
    BAKTIKD_IDENTIFIER = 211, DUBLQUOTED_IDENTIFIER = 212, SINGLQUOTED_STRING = 213, 
    DUBLQUOTED_STRING = 214, FLOAT_LITERAL = 215, DECIMAL_LITERAL = 216, 
    INTEGER_LITERAL = 217, HINT_START = 218, HINT_END = 219, SLASH_COMMENT = 220, 
    SQL_COMMENT = 221, BLOCK_COMMENT = 222, WHITESPACE = 223
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
