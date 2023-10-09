
// Generated from /Users/arjun/Boray/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.8

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
    AVG = 28, BETWEEN = 29, BINARY = 30, BIT = 31, BITS = 32, BLOB = 33, 
    BLOCK = 34, BLOOM = 35, BOTH = 36, BOOLEAN = 37, BTREE = 38, BY = 39, 
    BYTE = 40, BYTES = 41, CASE = 42, CAST = 43, CATALOG = 44, CHAR = 45, 
    CHARS = 46, CHARACTER = 47, COALESCE = 48, COLLATE = 49, CONCAT = 50, 
    CORRESPONDING = 51, COUNT = 52, CREATE = 53, CROSS = 54, CURRENT_CATALOG = 55, 
    CURRENT_SCHEMA = 56, CURRENT_TIMESTAMP = 57, CURRENT_USER = 58, DATA = 59, 
    DATE = 60, DAY = 61, DAYS = 62, DECIMAL = 63, DECODE = 64, DELETE = 65, 
    DESC = 66, DETAIL = 67, DIGITS = 68, DISTINCT = 69, DROP = 70, ELSE = 71, 
    END = 72, ESCAPE = 73, EXCEPT = 74, EXISTS = 75, EXPLAIN = 76, EXTRACT = 77, 
    SQL_FALSE = 78, FEDERATION = 79, FLOAT = 80, FOR = 81, FORMAT = 82, 
    FULL = 83, FROM = 84, GROUP = 85, HASH = 86, HASHKEY = 87, HAVING = 88, 
    HOUR = 89, HOURS = 90, IF = 91, IFNULL = 92, IN = 93, INDEX = 94, INNER = 95, 
    INSERT = 96, INTEGER = 97, INTERSECT = 98, INTERVAL = 99, INTO = 100, 
    IS = 101, JOIN = 102, KEY = 103, LEADING = 104, LEFT = 105, LIKE = 106, 
    LIMIT = 107, LOOP = 108, MAX = 109, MIN = 110, MINUTE = 111, MINUTES = 112, 
    MONOTONIC = 113, MONTH = 114, MONTHS = 115, NATURAL = 116, NO = 117, 
    NODE = 118, NOT = 119, NOW = 120, SQL_NULL = 121, NULLIF = 122, OFFSET = 123, 
    ON = 124, OR = 125, ORDER = 126, OUTER = 127, OVER = 128, OVERLAY = 129, 
    PARTITION = 130, PLACING = 131, POSITION = 132, PRETTY = 133, PRIMARY = 134, 
    QUARTER = 135, QUARTERS = 136, RECURSIVE = 137, REFERENCE = 138, RIGHT = 139, 
    ROW = 140, SCHEMA = 141, SECOND = 142, SECONDS = 143, SELECT = 144, 
    SEMI = 145, SET = 146, SKIPLIST = 147, STAGE = 148, STATS = 149, STATISTICS = 150, 
    SUBSTR = 151, SUBSTRING = 152, SUM = 153, TABLE = 154, TEXT = 155, THEN = 156, 
    TIME = 157, TIMESTAMP = 158, TO = 159, TOP = 160, TREE = 161, TRIM = 162, 
    TRAILING = 163, TRUNCATE = 164, SQL_TRUE = 165, TYPE = 166, TYPEOF = 167, 
    UNION = 168, UNIQUE = 169, UPDATE = 170, USING = 171, VALUE = 172, VALUES = 173, 
    VARBINARY = 174, VARCHAR = 175, VARYING = 176, WEEK = 177, WEEKS = 178, 
    WHEN = 179, WHERE = 180, WITH_NAMES = 181, WITH_TYPES = 182, WITH = 183, 
    XOR = 184, YEAR = 185, YEARS = 186, IDENTIFIER = 187, PLAIN_IDENTIFIER = 188, 
    BAKTIKD_IDENTIFIER = 189, DUBLQUOTED_IDENTIFIER = 190, STRING_LITERAL = 191, 
    SINGLQUOTED_STRING = 192, DUBLQUOTED_STRING = 193, FLOAT_LITERAL = 194, 
    DECIMAL_LITERAL = 195, INTEGER_LITERAL = 196, HINT_START = 197, HINT_END = 198, 
    SLASH_COMMENT = 199, SQL_COMMENT = 200, BLOCK_COMMENT = 201, WHITESPACE = 202
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
