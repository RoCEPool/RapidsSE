
// Generated from ./sql.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"


namespace Parser {


class  sqlLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, ALL = 20, 
    AND = 21, ANY = 22, AS = 23, ASC = 24, BETWEEN = 25, BOTH = 26, BY = 27, 
    CATALOG = 28, COLLATE = 29, CORRESPONDING = 30, CREATE = 31, CROSS = 32, 
    CURRENT_CATALOG = 33, CURRENT_SCHEMA = 34, CURRENT_TIMESTAMP = 35, CURRENT_USER = 36, 
    DATA = 37, DATE = 38, DELETE = 39, DESC = 40, DETAIL = 41, DISTINCT = 42, 
    DROP = 43, ESCAPE = 44, EXCEPT = 45, EXPLAIN = 46, FALSE = 47, FEDERATION = 48, 
    FOR = 49, FULL = 50, FROM = 51, GROUP = 52, HAVING = 53, IN = 54, INDEX = 55, 
    INNER = 56, INSERT = 57, INTERSECT = 58, INTO = 59, IS = 60, JOIN = 61, 
    KEY = 62, LEADING = 63, LEFT = 64, LIKE = 65, LIMIT = 66, MONOTONIC = 67, 
    NO = 68, NODE = 69, NOT = 70, NOW = 71, NULL = 72, OFFSET = 73, ON = 74, 
    OR = 75, ORDER = 76, OUTER = 77, OVER = 78, OVERLAY = 79, PARTITION = 80, 
    PLACING = 81, PRIMARY = 82, REFERENCE = 83, RIGHT = 84, SCHEMA = 85, 
    SELECT = 86, SET = 87, STATS = 88, STATISTICS = 89, TABLE = 90, TO = 91, 
    TOP = 92, TRAILING = 93, TRUNCATE = 94, TRUE = 95, TYPE = 96, UNION = 97, 
    UNIQUE = 98, UPDATE = 99, USING = 100, VALUES = 101, WHERE = 102, WITH = 103, 
    BOOLEAN = 104, INTEGER = 105, DECIMAL = 106, FLOAT = 107, TIMESTAMP = 108, 
    INTERVAL = 109, CHAR = 110, CHARACTER = 111, VARCHAR = 112, TEXT = 113, 
    BINARY = 114, VARBINARY = 115, BLOB = 116, VARYING = 117, BIT = 118, 
    BITS = 119, BYTE = 120, BYTES = 121, CHARS = 122, DIGITS = 123, CASE = 124, 
    CAST = 125, COALESCE = 126, CONCAT = 127, COUNT = 128, DECODE = 129, 
    EXTRACT = 130, EXISTS = 131, IF = 132, IFNULL = 133, NULLIF = 134, POSITION = 135, 
    SUBSTR = 136, SUBSTRING = 137, TRIM = 138, TYPEOF = 139, WHEN = 140, 
    THEN = 141, ELSE = 142, END = 143, DAY = 144, DAYS = 145, HOUR = 146, 
    HOURS = 147, MINUTE = 148, MINUTES = 149, MONTH = 150, MONTHS = 151, 
    QUARTER = 152, QUARTERS = 153, SECOND = 154, SECONDS = 155, WEEK = 156, 
    WEEKS = 157, YEAR = 158, YEARS = 159, BLOCK = 160, BLOOM = 161, BTREE = 162, 
    HASH = 163, LOOP = 164, SKIPLIST = 165, TREE = 166, VALUE = 167, IDENTIFIER = 168, 
    PLAIN_IDENTIFIER = 169, BAKTIKD_IDENTIFIER = 170, DUBLQUOTED_IDENTIFIER = 171, 
    STRING_LITERAL = 172, SINGLQUOTED_STRING = 173, DUBLQUOTED_STRING = 174, 
    FLOAT_LITERAL = 175, DECIMAL_LITERAL = 176, INTEGER_LITERAL = 177, HINT_START = 178, 
    HINT_END = 179, SLASH_COMMENT = 180, SQL_COMMENT = 181, BLOCK_COMMENT = 182, 
    WHITESPACE = 183
  };

  sqlLexer(antlr4::CharStream *input);
  ~sqlLexer();


    public boolean isDublQuoteId = true;

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
