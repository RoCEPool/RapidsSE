
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
    BOTH = 34, BOOLEAN = 35, BTREE = 36, BY = 37, BYTE = 38, BYTES = 39, 
    CASE = 40, CAST = 41, CATALOG = 42, CHAR = 43, CHARS = 44, CHARACTER = 45, 
    COALESCE = 46, COLLATE = 47, CONCAT = 48, CORRESPONDING = 49, COUNT = 50, 
    CREATE = 51, CROSS = 52, CURRENT_CATALOG = 53, CURRENT_SCHEMA = 54, 
    CURRENT_TIMESTAMP = 55, CURRENT_USER = 56, DATA = 57, DATE = 58, DATE_FORMAT = 59, 
    DAY = 60, DAYS = 61, DECIMAL = 62, DECODE = 63, DELETE = 64, DESC = 65, 
    DETAIL = 66, DIGITS = 67, DISTINCT = 68, DLOAD = 69, DROP = 70, ELSE = 71, 
    END = 72, ESCAPE = 73, ESCAPE_CHAR = 74, EXCEPT = 75, EXISTS = 76, EXPLAIN = 77, 
    EXTRACT = 78, SQL_FALSE = 79, FEDERATION = 80, FIELD_DELIMITER = 81, 
    FLOAT = 82, FOR = 83, FORMAT = 84, FULL = 85, FROM = 86, GROUP = 87, 
    HASH = 88, HASHKEY = 89, HAVING = 90, HOUR = 91, HOURS = 92, IF = 93, 
    IFNULL = 94, IN = 95, INDEX = 96, INNER = 97, INSERT = 98, INTEGER = 99, 
    INTERSECT = 100, INTERVAL = 101, INTO = 102, IS = 103, JOIN = 104, KEY = 105, 
    LEADING = 106, LEFT = 107, LIKE = 108, LIMIT = 109, LOAD = 110, LOOP = 111, 
    LSCOL = 112, LSIDX = 113, LSIDXCOL = 114, LSTBL = 115, MAX = 116, MEMORYLEAKS = 117, 
    MIN = 118, MINUTE = 119, MINUTES = 120, MONOTONIC = 121, MONTH = 122, 
    MONTHS = 123, NATURAL = 124, NO = 125, NODE = 126, NOT = 127, NOW = 128, 
    SQL_NULL = 129, NULLIF = 130, OFFSET = 131, ON = 132, OR = 133, ORDER = 134, 
    OUTER = 135, OVER = 136, OVERLAY = 137, PARTITION = 138, PATHS = 139, 
    PLACING = 140, POSITION = 141, PRETTY = 142, PRIMARY = 143, QUARTER = 144, 
    QUARTERS = 145, RECURSIVE = 146, REFERENCE = 147, REPLICATED = 148, 
    RIGHT = 149, ROW = 150, ROWSIZE = 151, SCHEMA = 152, SECOND = 153, SECONDS = 154, 
    SELECT = 155, SEMI = 156, SET = 157, SKIPLIST = 158, SPACE = 159, STAGE = 160, 
    STATS = 161, STATISTICS = 162, STATUS = 163, SUBSTR = 164, SUBSTRING = 165, 
    SUM = 166, SZIDX = 167, SZTBL = 168, TABLE = 169, TABLESIZE = 170, TEXT = 171, 
    THEN = 172, TIME = 173, TIMESTAMP = 174, TO = 175, TOP = 176, TRACKMEMORY = 177, 
    TREE = 178, TRIM = 179, TRAILING = 180, TRUNCATE = 181, SQL_TRUE = 182, 
    TYPE = 183, TYPEOF = 184, UNION = 185, UNIQUE = 186, UPDATE = 187, USING = 188, 
    VALUE = 189, VALUES = 190, VARBINARY = 191, VARCHAR = 192, VARYING = 193, 
    WEEK = 194, WEEKS = 195, WHEN = 196, WHERE = 197, WITH_NAMES = 198, 
    WITH_TYPES = 199, WITH = 200, XOR = 201, YEAR = 202, YEARS = 203, PLAIN_IDENTIFIER = 204, 
    BAKTIKD_IDENTIFIER = 205, DUBLQUOTED_IDENTIFIER = 206, SINGLQUOTED_STRING = 207, 
    DUBLQUOTED_STRING = 208, FLOAT_LITERAL = 209, DECIMAL_LITERAL = 210, 
    INTEGER_LITERAL = 211, HINT_START = 212, HINT_END = 213, SLASH_COMMENT = 214, 
    SQL_COMMENT = 215, BLOCK_COMMENT = 216, WHITESPACE = 217
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
