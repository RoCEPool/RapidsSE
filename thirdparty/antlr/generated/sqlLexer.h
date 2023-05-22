
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
    LSTBL = 107, MAX = 108, MIN = 109, MINUTE = 110, MINUTES = 111, MONOTONIC = 112, 
    MONTH = 113, MONTHS = 114, NATURAL = 115, NO = 116, NODE = 117, NOT = 118, 
    NOW = 119, SQL_NULL = 120, NULLIF = 121, OFFSET = 122, ON = 123, OR = 124, 
    ORDER = 125, OUTER = 126, OVER = 127, OVERLAY = 128, PARTITION = 129, 
    PLACING = 130, POSITION = 131, PRETTY = 132, PRIMARY = 133, QUARTER = 134, 
    QUARTERS = 135, RECURSIVE = 136, REFERENCE = 137, RIGHT = 138, ROW = 139, 
    SCHEMA = 140, SECOND = 141, SECONDS = 142, SELECT = 143, SEMI = 144, 
    SET = 145, SKIPLIST = 146, STAGE = 147, STATS = 148, STATISTICS = 149, 
    SUBSTR = 150, SUBSTRING = 151, SUM = 152, TABLE = 153, TEXT = 154, THEN = 155, 
    TIME = 156, TIMESTAMP = 157, TO = 158, TOP = 159, TREE = 160, TRIM = 161, 
    TRAILING = 162, TRUNCATE = 163, SQL_TRUE = 164, TYPE = 165, TYPEOF = 166, 
    UNION = 167, UNIQUE = 168, UPDATE = 169, USING = 170, VALUE = 171, VALUES = 172, 
    VARBINARY = 173, VARCHAR = 174, VARYING = 175, WEEK = 176, WEEKS = 177, 
    WHEN = 178, WHERE = 179, WITH_NAMES = 180, WITH_TYPES = 181, WITH = 182, 
    XOR = 183, YEAR = 184, YEARS = 185, IDENTIFIER = 186, PLAIN_IDENTIFIER = 187, 
    BAKTIKD_IDENTIFIER = 188, DUBLQUOTED_IDENTIFIER = 189, STRING_LITERAL = 190, 
    SINGLQUOTED_STRING = 191, DUBLQUOTED_STRING = 192, FLOAT_LITERAL = 193, 
    DECIMAL_LITERAL = 194, INTEGER_LITERAL = 195, HINT_START = 196, HINT_END = 197, 
    SLASH_COMMENT = 198, SQL_COMMENT = 199, BLOCK_COMMENT = 200, WHITESPACE = 201
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
