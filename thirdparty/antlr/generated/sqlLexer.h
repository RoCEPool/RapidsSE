
// Generated from /Users/myvanquoc/dev/src/SE/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.8

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
    PRIMARY = 91, RECURSIVE = 92, REFERENCE = 93, RIGHT = 94, ROW = 95, 
    SCHEMA = 96, SELECT = 97, SEMI = 98, SET = 99, STAGE = 100, STATS = 101, 
    STATISTICS = 102, SUM = 103, TABLE = 104, TO = 105, TOP = 106, TRAILING = 107, 
    TRUNCATE = 108, SQL_TRUE = 109, TYPE = 110, UNION = 111, UNIQUE = 112, 
    UPDATE = 113, USING = 114, VALUES = 115, WHERE = 116, WITH_NAMES = 117, 
    WITH = 118, XOR = 119, BOOLEAN = 120, INTEGER = 121, DECIMAL = 122, 
    FLOAT = 123, TIME = 124, TIMESTAMP = 125, INTERVAL = 126, CHAR = 127, 
    CHARACTER = 128, VARCHAR = 129, TEXT = 130, BINARY = 131, VARBINARY = 132, 
    BLOB = 133, VARYING = 134, BIT = 135, BITS = 136, BYTE = 137, BYTES = 138, 
    CHARS = 139, DIGITS = 140, CASE = 141, CAST = 142, COALESCE = 143, CONCAT = 144, 
    COUNT = 145, DECODE = 146, EXTRACT = 147, EXISTS = 148, IF = 149, IFNULL = 150, 
    NULLIF = 151, POSITION = 152, SUBSTR = 153, SUBSTRING = 154, TRIM = 155, 
    TYPEOF = 156, WHEN = 157, THEN = 158, ELSE = 159, END = 160, DAY = 161, 
    DAYS = 162, HOUR = 163, HOURS = 164, MINUTE = 165, MINUTES = 166, MONTH = 167, 
    MONTHS = 168, QUARTER = 169, QUARTERS = 170, SECOND = 171, SECONDS = 172, 
    WEEK = 173, WEEKS = 174, YEAR = 175, YEARS = 176, BLOCK = 177, BLOOM = 178, 
    BTREE = 179, HASH = 180, LOOP = 181, SKIPLIST = 182, TREE = 183, VALUE = 184, 
    IDENTIFIER = 185, PLAIN_IDENTIFIER = 186, BAKTIKD_IDENTIFIER = 187, 
    DUBLQUOTED_IDENTIFIER = 188, STRING_LITERAL = 189, SINGLQUOTED_STRING = 190, 
    DUBLQUOTED_STRING = 191, FLOAT_LITERAL = 192, DECIMAL_LITERAL = 193, 
    INTEGER_LITERAL = 194, HINT_START = 195, HINT_END = 196, SLASH_COMMENT = 197, 
    SQL_COMMENT = 198, BLOCK_COMMENT = 199, WHITESPACE = 200
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
