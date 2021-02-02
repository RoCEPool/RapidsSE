
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
    EXPLAIN = 49, SQL_FALSE = 50, FEDERATION = 51, FOR = 52, FULL = 53, 
    FROM = 54, GROUP = 55, HAVING = 56, IN = 57, INDEX = 58, INNER = 59, 
    INSERT = 60, INTERSECT = 61, INTO = 62, IS = 63, JOIN = 64, KEY = 65, 
    LEADING = 66, LEFT = 67, LIKE = 68, LIMIT = 69, MAX = 70, MIN = 71, 
    MONOTONIC = 72, NO = 73, NODE = 74, NOT = 75, NOW = 76, SQL_NULL = 77, 
    OFFSET = 78, ON = 79, OR = 80, ORDER = 81, OUTER = 82, OVER = 83, OVERLAY = 84, 
    PARTITION = 85, PLACING = 86, PRIMARY = 87, REFERENCE = 88, RIGHT = 89, 
    SCHEMA = 90, SELECT = 91, SET = 92, STATS = 93, STATISTICS = 94, SUM = 95, 
    TABLE = 96, TO = 97, TOP = 98, TRAILING = 99, TRUNCATE = 100, SQL_TRUE = 101, 
    TYPE = 102, UNION = 103, UNIQUE = 104, UPDATE = 105, USING = 106, VALUES = 107, 
    WHERE = 108, WITH = 109, XOR = 110, BOOLEAN = 111, INTEGER = 112, DECIMAL = 113, 
    FLOAT = 114, TIME = 115, TIMESTAMP = 116, INTERVAL = 117, CHAR = 118, 
    CHARACTER = 119, VARCHAR = 120, TEXT = 121, BINARY = 122, VARBINARY = 123, 
    BLOB = 124, VARYING = 125, BIT = 126, BITS = 127, BYTE = 128, BYTES = 129, 
    CHARS = 130, DIGITS = 131, CASE = 132, CAST = 133, COALESCE = 134, CONCAT = 135, 
    COUNT = 136, DECODE = 137, EXTRACT = 138, EXISTS = 139, IF = 140, IFNULL = 141, 
    NULLIF = 142, POSITION = 143, SUBSTR = 144, SUBSTRING = 145, TRIM = 146, 
    TYPEOF = 147, WHEN = 148, THEN = 149, ELSE = 150, END = 151, DAY = 152, 
    DAYS = 153, HOUR = 154, HOURS = 155, MINUTE = 156, MINUTES = 157, MONTH = 158, 
    MONTHS = 159, QUARTER = 160, QUARTERS = 161, SECOND = 162, SECONDS = 163, 
    WEEK = 164, WEEKS = 165, YEAR = 166, YEARS = 167, BLOCK = 168, BLOOM = 169, 
    BTREE = 170, HASH = 171, LOOP = 172, SKIPLIST = 173, TREE = 174, VALUE = 175, 
    IDENTIFIER = 176, PLAIN_IDENTIFIER = 177, BAKTIKD_IDENTIFIER = 178, 
    DUBLQUOTED_IDENTIFIER = 179, STRING_LITERAL = 180, SINGLQUOTED_STRING = 181, 
    DUBLQUOTED_STRING = 182, FLOAT_LITERAL = 183, DECIMAL_LITERAL = 184, 
    INTEGER_LITERAL = 185, HINT_START = 186, HINT_END = 187, SLASH_COMMENT = 188, 
    SQL_COMMENT = 189, BLOCK_COMMENT = 190, WHITESPACE = 191
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
