
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
    TIMESTAMP = 110, INTERVAL = 111, CHAR = 112, CHARACTER = 113, VARCHAR = 114, 
    TEXT = 115, BINARY = 116, VARBINARY = 117, BLOB = 118, VARYING = 119, 
    BIT = 120, BITS = 121, BYTE = 122, BYTES = 123, CHARS = 124, DIGITS = 125, 
    CASE = 126, CAST = 127, COALESCE = 128, CONCAT = 129, COUNT = 130, DECODE = 131, 
    EXTRACT = 132, EXISTS = 133, IF = 134, IFNULL = 135, NULLIF = 136, POSITION = 137, 
    SUBSTR = 138, SUBSTRING = 139, TRIM = 140, TYPEOF = 141, WHEN = 142, 
    THEN = 143, ELSE = 144, END = 145, DAY = 146, DAYS = 147, HOUR = 148, 
    HOURS = 149, MINUTE = 150, MINUTES = 151, MONTH = 152, MONTHS = 153, 
    QUARTER = 154, QUARTERS = 155, SECOND = 156, SECONDS = 157, WEEK = 158, 
    WEEKS = 159, YEAR = 160, YEARS = 161, BLOCK = 162, BLOOM = 163, BTREE = 164, 
    HASH = 165, LOOP = 166, SKIPLIST = 167, TREE = 168, VALUE = 169, IDENTIFIER = 170, 
    PLAIN_IDENTIFIER = 171, BAKTIKD_IDENTIFIER = 172, DUBLQUOTED_IDENTIFIER = 173, 
    STRING_LITERAL = 174, SINGLQUOTED_STRING = 175, DUBLQUOTED_STRING = 176, 
    FLOAT_LITERAL = 177, DECIMAL_LITERAL = 178, INTEGER_LITERAL = 179, HINT_START = 180, 
    HINT_END = 181, SLASH_COMMENT = 182, SQL_COMMENT = 183, BLOCK_COMMENT = 184, 
    WHITESPACE = 185
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
