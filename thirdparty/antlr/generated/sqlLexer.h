
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
    ALL = 21, AND = 22, ANTI = 23, ANTLR = 24, ANY = 25, AS = 26, ASC = 27, 
    AVG = 28, BETWEEN = 29, BOTH = 30, BY = 31, CATALOG = 32, COLLATE = 33, 
    CORRESPONDING = 34, CREATE = 35, CROSS = 36, CURRENT_CATALOG = 37, CURRENT_SCHEMA = 38, 
    CURRENT_TIMESTAMP = 39, CURRENT_USER = 40, DATA = 41, DATE = 42, DELETE = 43, 
    DESC = 44, DETAIL = 45, DISTINCT = 46, DROP = 47, ESCAPE = 48, EXCEPT = 49, 
    EXPLAIN = 50, SQL_FALSE = 51, FEDERATION = 52, FOR = 53, FORMAT = 54, 
    FULL = 55, FROM = 56, GROUP = 57, HAVING = 58, IN = 59, INDEX = 60, 
    INNER = 61, INSERT = 62, INTERSECT = 63, INTO = 64, IS = 65, JOIN = 66, 
    KEY = 67, LEADING = 68, LEFT = 69, LIKE = 70, LIMIT = 71, MAX = 72, 
    MIN = 73, MONOTONIC = 74, NATURAL = 75, NO = 76, NODE = 77, NOT = 78, 
    NOW = 79, SQL_NULL = 80, OFFSET = 81, ON = 82, OR = 83, ORDER = 84, 
    OUTER = 85, OVER = 86, OVERLAY = 87, PARTITION = 88, PLACING = 89, PRETTY = 90, 
    PRIMARY = 91, REFERENCE = 92, RIGHT = 93, ROW = 94, SCHEMA = 95, SELECT = 96, 
    SEMI = 97, SET = 98, STAGE = 99, STATS = 100, STATISTICS = 101, SUM = 102, 
    TABLE = 103, TO = 104, TOP = 105, TRAILING = 106, TRUNCATE = 107, SQL_TRUE = 108, 
    TYPE = 109, UNION = 110, UNIQUE = 111, UPDATE = 112, USING = 113, VALUES = 114, 
    WHERE = 115, WITH_NAMES = 116, WITH = 117, XOR = 118, BOOLEAN = 119, 
    INTEGER = 120, DECIMAL = 121, FLOAT = 122, TIME = 123, TIMESTAMP = 124, 
    INTERVAL = 125, CHAR = 126, CHARACTER = 127, VARCHAR = 128, TEXT = 129, 
    BINARY = 130, VARBINARY = 131, BLOB = 132, VARYING = 133, BIT = 134, 
    BITS = 135, BYTE = 136, BYTES = 137, CHARS = 138, DIGITS = 139, CASE = 140, 
    CAST = 141, COALESCE = 142, CONCAT = 143, COUNT = 144, DECODE = 145, 
    EXTRACT = 146, EXISTS = 147, IF = 148, IFNULL = 149, NULLIF = 150, POSITION = 151, 
    SUBSTR = 152, SUBSTRING = 153, TRIM = 154, TYPEOF = 155, WHEN = 156, 
    THEN = 157, ELSE = 158, END = 159, DAY = 160, DAYS = 161, HOUR = 162, 
    HOURS = 163, MINUTE = 164, MINUTES = 165, MONTH = 166, MONTHS = 167, 
    QUARTER = 168, QUARTERS = 169, SECOND = 170, SECONDS = 171, WEEK = 172, 
    WEEKS = 173, YEAR = 174, YEARS = 175, BLOCK = 176, BLOOM = 177, BTREE = 178, 
    HASH = 179, LOOP = 180, SKIPLIST = 181, TREE = 182, VALUE = 183, IDENTIFIER = 184, 
    PLAIN_IDENTIFIER = 185, BAKTIKD_IDENTIFIER = 186, DUBLQUOTED_IDENTIFIER = 187, 
    STRING_LITERAL = 188, SINGLQUOTED_STRING = 189, DUBLQUOTED_STRING = 190, 
    FLOAT_LITERAL = 191, DECIMAL_LITERAL = 192, INTEGER_LITERAL = 193, HINT_START = 194, 
    HINT_END = 195, SLASH_COMMENT = 196, SQL_COMMENT = 197, BLOCK_COMMENT = 198, 
    WHITESPACE = 199
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
