
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
    BOTH = 34, BOOLEAN = 35, BPA = 36, BTREE = 37, BY = 38, BYTE = 39, BYTES = 40, 
    CASE = 41, CAST = 42, CATALOG = 43, CHAR = 44, CHARS = 45, CHARACTER = 46, 
    COALESCE = 47, COLLATE = 48, CONCAT = 49, CORRESPONDING = 50, COUNT = 51, 
    CREATE = 52, CROSS = 53, CURRENT_CATALOG = 54, CURRENT_SCHEMA = 55, 
    CURRENT_TIMESTAMP = 56, CURRENT_USER = 57, DATA = 58, DATE = 59, DATE_FORMAT = 60, 
    DAY = 61, DAYS = 62, DECIMAL = 63, DECODE = 64, DELETE = 65, DESC = 66, 
    DETAIL = 67, DIGITS = 68, DISTINCT = 69, DLOAD = 70, DROP = 71, ELSE = 72, 
    END = 73, ESCAPE = 74, ESCAPE_CHAR = 75, EXCEPT = 76, EXISTS = 77, EXPLAIN = 78, 
    EXTRACT = 79, SQL_FALSE = 80, FEDERATION = 81, FIELD_DELIMITER = 82, 
    FLOAT = 83, FOR = 84, FORMAT = 85, FULL = 86, FROM = 87, GROUP = 88, 
    HASH = 89, HASHKEY = 90, HAVING = 91, HEAP = 92, HOUR = 93, HOURS = 94, 
    IF = 95, IFNULL = 96, IN = 97, INDEX = 98, INNER = 99, INSERT = 100, 
    INTEGER = 101, INTERSECT = 102, INTERVAL = 103, INTO = 104, IS = 105, 
    JOIN = 106, KEY = 107, LEADING = 108, LEFT = 109, LIKE = 110, LIMIT = 111, 
    LOAD = 112, LOOP = 113, LSCOL = 114, LSIDX = 115, LSIDXCOL = 116, LSTBL = 117, 
    MAX = 118, MEMORYLEAKS = 119, MIN = 120, MINUTE = 121, MINUTES = 122, 
    MONOTONIC = 123, MONTH = 124, MONTHS = 125, NATURAL = 126, NO = 127, 
    NODE = 128, NOT = 129, NOW = 130, SQL_NULL = 131, NULLIF = 132, OBJECT = 133, 
    OFFSET = 134, ON = 135, OR = 136, ORDER = 137, OUTER = 138, OVER = 139, 
    OVERLAY = 140, PARTITION = 141, PATHS = 142, PLACING = 143, POSITION = 144, 
    PRETTY = 145, PRIMARY = 146, QUARTER = 147, QUARTERS = 148, RECURSIVE = 149, 
    REFERENCE = 150, REGION = 151, REPLICATED = 152, RIGHT = 153, ROW = 154, 
    ROWSIZE = 155, SCHEMA = 156, SECOND = 157, SECONDS = 158, SELECT = 159, 
    SEMI = 160, SET = 161, SKIPLIST = 162, SPACE = 163, STAGE = 164, STATS = 165, 
    STATISTICS = 166, STATUS = 167, SUBSTR = 168, SUBSTRING = 169, SUM = 170, 
    SZIDX = 171, SZTBL = 172, TABLE = 173, TABLESIZE = 174, TEXT = 175, 
    THEN = 176, TIME = 177, TIMESTAMP = 178, TO = 179, TOP = 180, TRACKMEMORY = 181, 
    TREE = 182, TRIM = 183, TRAILING = 184, TRUNCATE = 185, SQL_TRUE = 186, 
    TYPE = 187, TYPEOF = 188, UNION = 189, UNIQUE = 190, UPDATE = 191, USING = 192, 
    VALUE = 193, VALUES = 194, VARBINARY = 195, VARCHAR = 196, VARYING = 197, 
    WEEK = 198, WEEKS = 199, WHEN = 200, WHERE = 201, WITH_NAMES = 202, 
    WITH_TYPES = 203, WITH = 204, XOR = 205, YEAR = 206, YEARS = 207, PLAIN_IDENTIFIER = 208, 
    BAKTIKD_IDENTIFIER = 209, DUBLQUOTED_IDENTIFIER = 210, SINGLQUOTED_STRING = 211, 
    DUBLQUOTED_STRING = 212, FLOAT_LITERAL = 213, DECIMAL_LITERAL = 214, 
    INTEGER_LITERAL = 215, HINT_START = 216, HINT_END = 217, SLASH_COMMENT = 218, 
    SQL_COMMENT = 219, BLOCK_COMMENT = 220, WHITESPACE = 221
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
