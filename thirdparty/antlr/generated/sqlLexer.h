
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
    ALL = 21, AND = 22, ANTLR = 23, ANY = 24, AS = 25, ASC = 26, AVG = 27, 
    BETWEEN = 28, BOTH = 29, BY = 30, CATALOG = 31, COLLATE = 32, CORRESPONDING = 33, 
    CREATE = 34, CROSS = 35, CURRENT_CATALOG = 36, CURRENT_SCHEMA = 37, 
    CURRENT_TIMESTAMP = 38, CURRENT_USER = 39, DATA = 40, DATE = 41, DELETE = 42, 
    DESC = 43, DETAIL = 44, DISTINCT = 45, DROP = 46, ESCAPE = 47, EXCEPT = 48, 
    EXPLAIN = 49, SQL_FALSE = 50, FEDERATION = 51, FOR = 52, FORMAT = 53, 
    FULL = 54, FROM = 55, GROUP = 56, HAVING = 57, IN = 58, INDEX = 59, 
    INNER = 60, INSERT = 61, INTERSECT = 62, INTO = 63, IS = 64, JOIN = 65, 
    KEY = 66, LEADING = 67, LEFT = 68, LIKE = 69, LIMIT = 70, MAX = 71, 
    MIN = 72, MONOTONIC = 73, NO = 74, NODE = 75, NOT = 76, NOW = 77, SQL_NULL = 78, 
    OFFSET = 79, ON = 80, OR = 81, ORDER = 82, OUTER = 83, OVER = 84, OVERLAY = 85, 
    PARTITION = 86, PLACING = 87, PRETTY = 88, PRIMARY = 89, REFERENCE = 90, 
    RIGHT = 91, ROW = 92, SCHEMA = 93, SELECT = 94, SET = 95, STAGE = 96, 
    STATS = 97, STATISTICS = 98, SUM = 99, TABLE = 100, TO = 101, TOP = 102, 
    TRAILING = 103, TRUNCATE = 104, SQL_TRUE = 105, TYPE = 106, UNION = 107, 
    UNIQUE = 108, UPDATE = 109, USING = 110, VALUES = 111, WHERE = 112, 
    WITH = 113, XOR = 114, BOOLEAN = 115, INTEGER = 116, DECIMAL = 117, 
    FLOAT = 118, TIME = 119, TIMESTAMP = 120, INTERVAL = 121, CHAR = 122, 
    CHARACTER = 123, VARCHAR = 124, TEXT = 125, BINARY = 126, VARBINARY = 127, 
    BLOB = 128, VARYING = 129, BIT = 130, BITS = 131, BYTE = 132, BYTES = 133, 
    CHARS = 134, DIGITS = 135, CASE = 136, CAST = 137, COALESCE = 138, CONCAT = 139, 
    COUNT = 140, DECODE = 141, EXTRACT = 142, EXISTS = 143, IF = 144, IFNULL = 145, 
    NULLIF = 146, POSITION = 147, SUBSTR = 148, SUBSTRING = 149, TRIM = 150, 
    TYPEOF = 151, WHEN = 152, THEN = 153, ELSE = 154, END = 155, DAY = 156, 
    DAYS = 157, HOUR = 158, HOURS = 159, MINUTE = 160, MINUTES = 161, MONTH = 162, 
    MONTHS = 163, QUARTER = 164, QUARTERS = 165, SECOND = 166, SECONDS = 167, 
    WEEK = 168, WEEKS = 169, YEAR = 170, YEARS = 171, BLOCK = 172, BLOOM = 173, 
    BTREE = 174, HASH = 175, LOOP = 176, SKIPLIST = 177, TREE = 178, VALUE = 179, 
    IDENTIFIER = 180, PLAIN_IDENTIFIER = 181, BAKTIKD_IDENTIFIER = 182, 
    DUBLQUOTED_IDENTIFIER = 183, STRING_LITERAL = 184, SINGLQUOTED_STRING = 185, 
    DUBLQUOTED_STRING = 186, FLOAT_LITERAL = 187, DECIMAL_LITERAL = 188, 
    INTEGER_LITERAL = 189, HINT_START = 190, HINT_END = 191, SLASH_COMMENT = 192, 
    SQL_COMMENT = 193, BLOCK_COMMENT = 194, WHITESPACE = 195
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
