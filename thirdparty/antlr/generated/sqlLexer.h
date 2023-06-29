
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
    LSCOL = 112, LSIDX = 113, LSIDXCOL = 114, LSTBL = 115, MAX = 116, MIN = 117, 
    MINUTE = 118, MINUTES = 119, MONOTONIC = 120, MONTH = 121, MONTHS = 122, 
    NATURAL = 123, NO = 124, NODE = 125, NOT = 126, NOW = 127, SQL_NULL = 128, 
    NULLIF = 129, OFFSET = 130, ON = 131, OR = 132, ORDER = 133, OUTER = 134, 
    OVER = 135, OVERLAY = 136, PARTITION = 137, PATHS = 138, PLACING = 139, 
    POSITION = 140, PRETTY = 141, PRIMARY = 142, QUARTER = 143, QUARTERS = 144, 
    RECURSIVE = 145, REFERENCE = 146, REPLICATED = 147, RIGHT = 148, ROW = 149, 
    ROWSIZE = 150, SCHEMA = 151, SECOND = 152, SECONDS = 153, SELECT = 154, 
    SEMI = 155, SET = 156, SKIPLIST = 157, SPACE = 158, STAGE = 159, STATS = 160, 
    STATISTICS = 161, STATUS = 162, SUBSTR = 163, SUBSTRING = 164, SUM = 165, 
    SZIDX = 166, SZTBL = 167, TABLE = 168, TABLESIZE = 169, TEXT = 170, 
    THEN = 171, TIME = 172, TIMESTAMP = 173, TO = 174, TOP = 175, TREE = 176, 
    TRIM = 177, TRAILING = 178, TRUNCATE = 179, SQL_TRUE = 180, TYPE = 181, 
    TYPEOF = 182, UNION = 183, UNIQUE = 184, UPDATE = 185, USING = 186, 
    VALUE = 187, VALUES = 188, VARBINARY = 189, VARCHAR = 190, VARYING = 191, 
    WEEK = 192, WEEKS = 193, WHEN = 194, WHERE = 195, WITH_NAMES = 196, 
    WITH_TYPES = 197, WITH = 198, XOR = 199, YEAR = 200, YEARS = 201, PLAIN_IDENTIFIER = 202, 
    BAKTIKD_IDENTIFIER = 203, DUBLQUOTED_IDENTIFIER = 204, SINGLQUOTED_STRING = 205, 
    DUBLQUOTED_STRING = 206, FLOAT_LITERAL = 207, DECIMAL_LITERAL = 208, 
    INTEGER_LITERAL = 209, HINT_START = 210, HINT_END = 211, SLASH_COMMENT = 212, 
    SQL_COMMENT = 213, BLOCK_COMMENT = 214, WHITESPACE = 215
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
