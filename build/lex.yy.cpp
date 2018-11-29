// lex.yy.cpp generated by reflex 1.0.9 from /Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define REFLEX_OPTION_YYLTYPE             yy::location
#define REFLEX_OPTION_YYSTYPE             yy::parser::semantic_type
#define REFLEX_OPTION_bison_cc            true
#define REFLEX_OPTION_bison_cc_namespace  yy
#define REFLEX_OPTION_bison_cc_parser     parser
#define REFLEX_OPTION_bison_locations     true
#define REFLEX_OPTION_header_file         lex.yy.h
#define REFLEX_OPTION_lex                 yylex
#define REFLEX_OPTION_lexer               Lexer
#define REFLEX_OPTION_namespace           yy
#define REFLEX_OPTION_noyywrap            true
#define REFLEX_OPTION_outfile             lex.yy.cpp

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top{ user code %}                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 1 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"

/* Quack scanner
 *
 * 2018 Fall version for RE/flex,
 * skeleton.
 *
 */

#include "build/quack.tab.hxx"  /* Generated by bison. */
#include "include/Messages.h"


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ABSTRACT LEXER CLASS                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/abslexer.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LEXER CLASS                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

namespace yy {

class Lexer : public reflex::AbstractLexer<reflex::Matcher> {
 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  Lexer(
      const reflex::Input& input = reflex::Input(),
      std::ostream&        os    = std::cout)
    :
      AbstractBaseLexer(input, os)
  {
  }
  static const int INITIAL = 0;
  static const int comment = 1;
  static const int quote = 2;
  static const int triplequote = 3;
  virtual int yylex(yy::parser::semantic_type *yylval, yy::location *yylloc)
  {
    yylloc->step();
    int ret = yylex(*yylval);
    yylloc->end.line = matcher().lineno();
    yylloc->end.column = matcher().columno();
    return ret;
  }
  virtual int yylex(yy::parser::semantic_type& yylval);
};

} // namespace yy

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %{ user code %}                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 13 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"

/* Externally visible file name --
* for error messages.
* FIXME: Need to extract this from structures that reflex passes
* or else find another way to dig it out.
*/
std::string yyfilename = "What file is this, anyway?";

/* Some strings can't be matched in one gulp.  We need
 * a buffer in which to assemble them from parts.
 */
std::string string_buf = "";

void yyerror (const std::string &msg, yy::position* where) {
     std::cout << where << ": " << msg;
}

/* Some long messages that don't fit well in the code below */

std::string BAD_ESC_MSG =
  "Illegal escape code; only \\\\, \\0, \\t, \\n, \\r, \\n are permitted";
std::string BAD_NL_STR =
  "Unclosed string?  Encountered newline in quoted string.";


#line 43 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
/* You'll probably want scanner states for both multi-line
   * comments and for triple-quote strings.  Do comments first,
   * because they're easier (you just throw away the text).  Then
   * do triple-quote strings.  You might consider doing the same for
   * normal strings with a single pair of double quotes, to avoid
   * writing a really ugly regex.  'comment' here is an example of
   * how to declare an exclusive scanner state.
   */

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  BISON C++                                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 2: rules                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int yy::Lexer::yylex(yy::parser::semantic_type& yylval)
{
  static const reflex::Pattern PATTERN_INITIAL("(?m)([/]{2}.*)|(==)|([<]=)|([>]=)|([(-/:->\\x7b\\x7d])|(class)|(def)|(extends)|(if)|(elif)|(else)|(while)|(return)|(typecase)|(and)|(or)|(not)|([0-9]+)|([A-Z_a-z][0-9A-Z_a-z]*)|([\"]{3})|([\"])|([\\x09\\x0a\\x20]*)|([/][\\x2a])|(.)");
  static const reflex::Pattern PATTERN_comment("(?m)([^\\x2a]*)|([\\x2a][^/])|([\\x2a][/])");
  static const reflex::Pattern PATTERN_quote("(?m)([\"])|([\\x5c]0)|([\\x5c]b)|([\\x5c]t)|([\\x5c]n)|([\\x5c]r)|([\\x5c]f)|([\\x5c][\"])|([\\x5c]{2})|([\\x5c](?:.|[\\x0a\\x0d]))|([\\x0a\\x0d])|([^\\x0a\\x0d\"\\x5c]*)");
  static const reflex::Pattern PATTERN_triplequote("(?m)([\"]{3})|((?:[^\"]|[\\x0a\\x0d]|[\"]{1,2}[^\"])*)");
  if (!has_matcher())
  {
    matcher(new Matcher(PATTERN_INITIAL, stdinit(), this));
  }
  while (true)
  {
    switch (start())
    {
      case INITIAL:
        matcher().pattern(PATTERN_INITIAL);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 166 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return EOF; }

            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 58: [/]{2}.*
#line 58 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ /* Single-line comments */ }

  /* We start with some tokens that are not value-bearing,
   * i.e., the parser needs to know only which token was matched.
   */

  /* Multi-character punctuation with names */
            break;
          case 2: // rule at line 65: ==
#line 65 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::EQUALS; }
            break;
          case 3: // rule at line 66: [<]=
#line 66 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::ATMOST; }
            break;
          case 4: // rule at line 67: [>]=
#line 67 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::ATLEAST; }


   /* Single character punctuation.  Because a character
    * in C or C++ can be treated as an integer, Bison lets
    * us return a character as a token.
    * JFlex/Cup do not let you do this in Java.
    * Here are a few ... there are more.
    */

            break;
          case 5: // rule at line 77: [(-/:->\x7b\x7d]
#line 77 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return text()[0]; }

  /* Keywords are essentially another kind of punctuation,
   * but since they also match the identifier pattern, we
   * need to be sure we give their patterns *before* the
   * identifier pattern.  Most scanner generators work this
   * way, but WARNING for Python PLY users:  PLY prioritizes
   * by length of the regular expression!
   * Here are a couple of examples.
   */

            break;
          case 6: // rule at line 88: class
#line 88 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::CLASS; }
            break;
          case 7: // rule at line 89: def
#line 89 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::DEF; }
            break;
          case 8: // rule at line 90: extends
#line 90 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::EXTENDS; }
            break;
          case 9: // rule at line 91: if
#line 91 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::IF; }
            break;
          case 10: // rule at line 92: elif
#line 92 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::ELIF; }
            break;
          case 11: // rule at line 93: else
#line 93 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::ELSE; }
            break;
          case 12: // rule at line 94: while
#line 94 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::WHILE; }
            break;
          case 13: // rule at line 95: return
#line 95 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::RETURN; }
            break;
          case 14: // rule at line 96: typecase
#line 96 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::TYPECASE; }
            break;
          case 15: // rule at line 97: and
#line 97 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::AND; }
            break;
          case 16: // rule at line 98: or
#line 98 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::OR; }
            break;
          case 17: // rule at line 99: not
#line 99 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return parser::token::NOT; }


   /* The following tokens are value-bearing:
    * We pass a value back to the parser by copying
    * it into the yylval parameter.  The parser
    * expects string values for identifiers and
    * string literals in yylval.str.  It expects integer
    * values for integer literals in yylval.num.
    * I'll give pattern for integer literals.
    * You'll need another for identifiers.  Remember to
    * 'strdup' the value returned by 'text()', or else it will
    * be clobbered by subsequent calls.
    */

            break;
          case 18: // rule at line 114: [0-9]+
#line 114 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ yylval.num = atoi(text()); return parser::token::INT_LIT; }
            break;
          case 19: // rule at line 115: [A-Z_a-z][0-9A-Z_a-z]*
#line 115 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ yylval.str = strdup(text()); return parser::token::IDENT; }

            break;
          case 20: // rule at line 117: ["]{3}
#line 117 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ start(triplequote); }
            break;
          case 21: // rule at line 124: ["]
#line 124 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ string_buf = ""; start(quote); }
            break;
          case 22: // rule at line 153: [\x09\x0a\x20]*
#line 153 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ ; }

   /* Multi-line comments */
            break;
          case 23: // rule at line 156: [/][\x2a]
#line 156 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ start(comment); }
            break;
          case 24: // rule at line 161: .
#line 161 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{   report::error("Unexpected character '" + std::string(text()) + "'" +
       " at line " + std::to_string(lineno()) +
       ", column " + std::to_string(columno()));
   }

            break;
        }
        break;
      case comment:
        matcher().pattern(PATTERN_comment);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 166 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return EOF; }

            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 157: [^\x2a]*
#line 157 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ ; }
            break;
          case 2: // rule at line 158: [\x2a][^/]
#line 158 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ ; }
            break;
          case 3: // rule at line 159: [\x2a][/]
#line 159 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ start(INITIAL); }

            break;
        }
        break;
      case quote:
        matcher().pattern(PATTERN_quote);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 166 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ return EOF; }

            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 125: ["]
#line 125 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ yylval.str = strdup(string_buf.c_str()); start(INITIAL); return parser::token::STRING_LIT; }
            break;
          case 2: // rule at line 126: [\x5c]0
#line 126 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ string_buf += "\0"; }
            break;
          case 3: // rule at line 127: [\x5c]b
#line 127 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ string_buf += "\b"; }
            break;
          case 4: // rule at line 128: [\x5c]t
#line 128 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ string_buf += "\t"; }
            break;
          case 5: // rule at line 129: [\x5c]n
#line 129 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ string_buf += "\n"; }
            break;
          case 6: // rule at line 130: [\x5c]r
#line 130 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ string_buf += "\r"; }
            break;
          case 7: // rule at line 131: [\x5c]f
#line 131 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ string_buf += "\f"; }
            break;
          case 8: // rule at line 132: [\x5c]["]
#line 132 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ string_buf += "\""; }
            break;
          case 9: // rule at line 133: [\x5c]{2}
#line 133 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ string_buf += "\\"; }
            break;
          case 10: // rule at line 134: [\x5c](?:.|[\x0a\x0d])
#line 134 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ report::error("Illegal escape sequence '" + std::string(text()) + "'"+
                                                        " at line " + std::to_string(lineno()) +
                                                        ", column " + std::to_string(columno())); }
            break;
          case 11: // rule at line 137: [\x0a\x0d]
#line 137 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ report::error("Unexpected newline or end of string at line " + std::to_string(lineno()) +
                                                        ", column " + std::to_string(columno()));
                                                        yylval.str = strdup(string_buf.c_str());
                                                        start(INITIAL);
                                                        return parser::token::STRING_LIT; }
            break;
          case 12: // rule at line 142: [^\x0a\x0d"\x5c]*
#line 142 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ string_buf += std::string(text()); }

  /* You *can* write a one-line regular expression for matching a quoted string,
   * but you probably can't read it.  (I can't read mine.)  Scanner states and
   * simpler patterns are probably a better idea.
   * You almost certainly scanner states for triple quotes.
   * The tricky bit is catching errors, such as disallowed escape sequences
   * and unclosed quotes.
   */

  /* Ignore whitespace */
            break;
        }
        break;
      case triplequote:
        matcher().pattern(PATTERN_triplequote);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 119 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ report::error("Non-terminated string '" + std::string(yylval.str) + "' at line " + std::to_string(lineno()) + ", column " + std::to_string(columno())); start(INITIAL); }
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 118: ["]{3}
#line 118 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ start(INITIAL); return parser::token::STRING_LIT; }
            break;
          case 2: // rule at line 121: (?:[^"]|[\x0a\x0d]|["]{1,2}[^"])*
#line 121 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"
{ yylval.str = strdup(text()); }


            break;
        }
        break;
      default:
        return 0;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 3: user code                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 169 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.lxx"

/* No main program here */
