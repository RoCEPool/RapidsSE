
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
    CURRENT_TIMESTAMP = 55, CURRENT_USER = 56, DATA = 57, DATE = 58, DAY = 59, 
    DAYS = 60, DECIMAL = 61, DECODE = 62, DELETE = 63, DESC = 64, DETAIL = 65, 
    DIGITS = 66, DISTINCT = 67, DROP = 68, ELSE = 69, END = 70, ESCAPE = 71, 
    EXCEPT = 72, EXISTS = 73, EXPLAIN = 74, EXTRACT = 75, SQL_FALSE = 76, 
    FEDERATION = 77, FLOAT = 78, FOR = 79, FORMAT = 80, FULL = 81, FROM = 82, 
    GROUP = 83, HASH = 84, HASHKEY = 85, HAVING = 86, HOUR = 87, HOURS = 88, 
    IF = 89, IFNULL = 90, IN = 91, INDEX = 92, INNER = 93, INSERT = 94, 
    INTEGER = 95, INTERSECT = 96, INTERVAL = 97, INTO = 98, IS = 99, JOIN = 100, 
    KEY = 101, LEADING = 102, LEFT = 103, LIKE = 104, LIMIT = 105, LOOP = 106, 
    LSIDX = 107, LSTBL = 108, MAX = 109, MIN = 110, MINUTE = 111, MINUTES = 112, 
    MONOTONIC = 113, MONTH = 114, MONTHS = 115, NATURAL = 116, NO = 117, 
    NODE = 118, NOT = 119, NOW = 120, SQL_NULL = 121, NULLIF = 122, OFFSET = 123, 
    ON = 124, OR = 125, ORDER = 126, OUTER = 127, OVER = 128, OVERLAY = 129, 
    PARTITION = 130, PLACING = 131, POSITION = 132, PRETTY = 133, PRIMARY = 134, 
    QUARTER = 135, QUARTERS = 136, RECURSIVE = 137, REFERENCE = 138, REPLICATED = 139, 
    RIGHT = 140, ROW = 141, ROWSIZE = 142, SCHEMA = 143, SECOND = 144, SECONDS = 145, 
    SELECT = 146, SEMI = 147, SET = 148, SKIPLIST = 149, STAGE = 150, STATS = 151, 
    STATISTICS = 152, SUBSTR = 153, SUBSTRING = 154, SUM = 155, TABLE = 156, 
    TABLESIZE = 157, TEXT = 158, THEN = 159, TIME = 160, TIMESTAMP = 161, 
    TO = 162, TOP = 163, TREE = 164, TRIM = 165, TRAILING = 166, TRUNCATE = 167, 
    SQL_TRUE = 168, TYPE = 169, TYPEOF = 170, UNION = 171, UNIQUE = 172, 
    UPDATE = 173, USING = 174, VALUE = 175, VALUES = 176, VARBINARY = 177, 
    VARCHAR = 178, VARYING = 179, WEEK = 180, WEEKS = 181, WHEN = 182, WHERE = 183, 
    WITH_NAMES = 184, WITH_TYPES = 185, WITH = 186, XOR = 187, YEAR = 188, 
    YEARS = 189, IDENTIFIER = 190, PLAIN_IDENTIFIER = 191, BAKTIKD_IDENTIFIER = 192, 
    DUBLQUOTED_IDENTIFIER = 193, STRING_LITERAL = 194, SINGLQUOTED_STRING = 195, 
    DUBLQUOTED_STRING = 196, FLOAT_LITERAL = 197, DECIMAL_LITERAL = 198, 
    INTEGER_LITERAL = 199, HINT_START = 200, HINT_END = 201, SLASH_COMMENT = 202, 
    SQL_COMMENT = 203, BLOCK_COMMENT = 204, WHITESPACE = 205
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
