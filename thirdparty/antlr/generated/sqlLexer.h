
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
    ALL = 21, AND = 22, ANTLR = 23, ANY = 24, AS = 25, ASC = 26, BETWEEN = 27, 
    BOTH = 28, BY = 29, CATALOG = 30, COLLATE = 31, CORRESPONDING = 32, 
    CREATE = 33, CROSS = 34, CURRENT_CATALOG = 35, CURRENT_SCHEMA = 36, 
    CURRENT_TIMESTAMP = 37, CURRENT_USER = 38, DATA = 39, DATE = 40, DELETE = 41, 
    DESC = 42, DETAIL = 43, DISTINCT = 44, DROP = 45, ESCAPE = 46, EXCEPT = 47, 
    EXPLAIN = 48, SQL_FALSE = 49, FEDERATION = 50, FOR = 51, FULL = 52, 
    FROM = 53, GROUP = 54, HAVING = 55, IN = 56, INDEX = 57, INNER = 58, 
    INSERT = 59, INTERSECT = 60, INTO = 61, IS = 62, JOIN = 63, KEY = 64, 
    LEADING = 65, LEFT = 66, LIKE = 67, LIMIT = 68, MONOTONIC = 69, NO = 70, 
    NODE = 71, NOT = 72, NOW = 73, SQL_NULL = 74, OFFSET = 75, ON = 76, 
    OR = 77, ORDER = 78, OUTER = 79, OVER = 80, OVERLAY = 81, PARTITION = 82, 
    PLACING = 83, PRIMARY = 84, REFERENCE = 85, RIGHT = 86, SCHEMA = 87, 
    SELECT = 88, SET = 89, STATS = 90, STATISTICS = 91, TABLE = 92, TO = 93, 
    TOP = 94, TRAILING = 95, TRUNCATE = 96, SQL_TRUE = 97, TYPE = 98, UNION = 99, 
    UNIQUE = 100, UPDATE = 101, USING = 102, VALUES = 103, WHERE = 104, 
    WITH = 105, BOOLEAN = 106, INTEGER = 107, DECIMAL = 108, FLOAT = 109, 
    TIME = 110, TIMESTAMP = 111, INTERVAL = 112, CHAR = 113, CHARACTER = 114, 
    VARCHAR = 115, TEXT = 116, BINARY = 117, VARBINARY = 118, BLOB = 119, 
    VARYING = 120, BIT = 121, BITS = 122, BYTE = 123, BYTES = 124, CHARS = 125, 
    DIGITS = 126, CASE = 127, CAST = 128, COALESCE = 129, CONCAT = 130, 
    COUNT = 131, DECODE = 132, EXTRACT = 133, EXISTS = 134, IF = 135, IFNULL = 136, 
    NULLIF = 137, POSITION = 138, SUBSTR = 139, SUBSTRING = 140, TRIM = 141, 
    TYPEOF = 142, WHEN = 143, THEN = 144, ELSE = 145, END = 146, DAY = 147, 
    DAYS = 148, HOUR = 149, HOURS = 150, MINUTE = 151, MINUTES = 152, MONTH = 153, 
    MONTHS = 154, QUARTER = 155, QUARTERS = 156, SECOND = 157, SECONDS = 158, 
    WEEK = 159, WEEKS = 160, YEAR = 161, YEARS = 162, BLOCK = 163, BLOOM = 164, 
    BTREE = 165, HASH = 166, LOOP = 167, SKIPLIST = 168, TREE = 169, VALUE = 170, 
    IDENTIFIER = 171, PLAIN_IDENTIFIER = 172, BAKTIKD_IDENTIFIER = 173, 
    DUBLQUOTED_IDENTIFIER = 174, STRING_LITERAL = 175, SINGLQUOTED_STRING = 176, 
    DUBLQUOTED_STRING = 177, FLOAT_LITERAL = 178, DECIMAL_LITERAL = 179, 
    INTEGER_LITERAL = 180, HINT_START = 181, HINT_END = 182, SLASH_COMMENT = 183, 
    SQL_COMMENT = 184, BLOCK_COMMENT = 185, WHITESPACE = 186
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
