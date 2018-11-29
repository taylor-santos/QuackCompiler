// A Bison parser, made by GNU Bison 3.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "quack.tab.cxx" // lalr1.cc:407

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "quack.tab.hxx"

// User implementation prologue.

#line 51 "quack.tab.cxx" // lalr1.cc:415
// Unqualified %code blocks.
#line 27 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:416

    #include "build/lex.yy.h"
    #undef yylex
    #define yylex lexer.yylex  /* Within bison's parse() we should invoke lexer.yylex(), not the global yylex() */

#line 59 "quack.tab.cxx" // lalr1.cc:416


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 154 "quack.tab.cxx" // lalr1.cc:491

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (yy::Lexer& lexer_yyarg, AST::Program **root_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      lexer (lexer_yyarg),
      root (root_yyarg)
  {}

  parser::~parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol ()
    : value ()
    , location ()
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value (other.value)
    , location (other.location)
  {
  }

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::clear ()
  {
    Base::clear ();
  }

  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  parser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  parser::by_state::by_state ()
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  void
  parser::by_state::clear ()
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s)
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (const stack_symbol_type& that)
    : super_type (that.state, that.location)
  {
    value = that.value;
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.value, that.location)
  {
    // that is emptied.
    that.type = empty_symbol;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  void
  parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  void
  parser::yypop_ (unsigned n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Default location.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 91 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { 
    (yylhs.value.prgm) = new AST::Program((yystack_[1].value.classes), (yystack_[0].value.stmts));
    *root = (yylhs.value.prgm); 
}
#line 619 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 3:
#line 96 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.classes) = (yystack_[0].value.classes); }
#line 625 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 4:
#line 97 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.classes) = new std::vector<AST::Class*>(); }
#line 631 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 5:
#line 99 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.classes) = new std::vector<AST::Class*>(); (yylhs.value.classes)->push_back((yystack_[0].value.clazz)); }
#line 637 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 6:
#line 100 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.classes) = (yystack_[1].value.classes); (yylhs.value.classes)->push_back((yystack_[0].value.clazz)); }
#line 643 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 7:
#line 102 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmts) = (yystack_[0].value.stmts); }
#line 649 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 8:
#line 103 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmts) = new std::vector<AST::Statement*>(); }
#line 655 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 9:
#line 105 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmts) = new std::vector<AST::Statement*>(); (yylhs.value.stmts)->push_back((yystack_[0].value.stmt)); (yystack_[0].value.stmt)->set_position(yystack_[0].location.begin.line, yystack_[0].location.begin.column, yystack_[0].location.end.line, yystack_[0].location.end.column, yystack_[0].location.begin.filename); }
#line 661 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 10:
#line 106 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmts) = (yystack_[1].value.stmts); (yylhs.value.stmts)->push_back((yystack_[0].value.stmt)); (yystack_[0].value.stmt)->set_position(yystack_[0].location.begin.line, yystack_[0].location.begin.column, yystack_[0].location.end.line, yystack_[0].location.end.column, yystack_[0].location.begin.filename); }
#line 667 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 11:
#line 108 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.clazz) = new AST::Class((yystack_[8].value.str), (yystack_[6].value.typedArgs), (yystack_[4].value.str), (yystack_[2].value.stmts), (yystack_[1].value.mthds)); (yylhs.value.clazz)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 673 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 12:
#line 109 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.clazz) = new AST::Class((yystack_[7].value.str), (yystack_[5].value.typedArgs), "Obj", (yystack_[2].value.stmts), (yystack_[1].value.mthds)); (yylhs.value.clazz)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 679 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 13:
#line 111 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.str) = (yystack_[0].value.str); }
#line 685 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 14:
#line 113 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.typedArgs) = (yystack_[0].value.typedArgs); }
#line 691 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 15:
#line 114 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.typedArgs) = new std::vector<AST::TypedArg*>(); }
#line 697 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 16:
#line 116 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.typedArgs) = new std::vector<AST::TypedArg*>(); (yylhs.value.typedArgs)->push_back((yystack_[0].value.typedArg)); }
#line 703 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 17:
#line 117 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.typedArgs) = (yystack_[2].value.typedArgs); (yylhs.value.typedArgs)->push_back((yystack_[0].value.typedArg)); }
#line 709 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 18:
#line 119 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.typedArg) = new AST::TypedArg((yystack_[2].value.str), (yystack_[0].value.str)); (yylhs.value.typedArg)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 715 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 19:
#line 121 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.mthds) = (yystack_[0].value.mthds); }
#line 721 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 20:
#line 122 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.mthds) = new std::vector<AST::Method*>(); }
#line 727 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 21:
#line 124 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.mthds) = new std::vector<AST::Method*>(); (yylhs.value.mthds)->push_back((yystack_[0].value.mthd)); }
#line 733 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 22:
#line 125 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.mthds) = (yystack_[1].value.mthds); (yylhs.value.mthds)->push_back((yystack_[0].value.mthd)); }
#line 739 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 23:
#line 127 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.mthd) = new AST::Method((yystack_[8].value.str), (yystack_[6].value.typedArgs), (yystack_[3].value.str), (yystack_[1].value.stmts)); (yylhs.value.mthd)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 745 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 24:
#line 128 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.mthd) = new AST::Method((yystack_[6].value.str), (yystack_[4].value.typedArgs), (yystack_[1].value.stmts));     (yylhs.value.mthd)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 751 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 25:
#line 130 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmt) = (yystack_[0].value.if_block); }
#line 757 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 26:
#line 131 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmt) = new AST::While((yystack_[3].value.rExpr), (yystack_[1].value.stmts)); (yylhs.value.stmt)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 763 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 27:
#line 132 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmt) = new AST::Assignment(new AST::LExpr((yystack_[3].value.str)), (yystack_[1].value.rExpr)); (yylhs.value.stmt)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 769 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 28:
#line 133 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmt) = new AST::Assignment((yystack_[3].value.field), (yystack_[1].value.rExpr)); (yylhs.value.stmt)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 775 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 29:
#line 134 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmt) = new AST::Assignment(new AST::LExpr((yystack_[5].value.str)), (yystack_[3].value.str), (yystack_[1].value.rExpr)); (yylhs.value.stmt)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 781 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 30:
#line 135 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmt) = new AST::Assignment((yystack_[5].value.field), (yystack_[3].value.str), (yystack_[1].value.rExpr)); (yylhs.value.stmt)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 787 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 31:
#line 136 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmt) = (yystack_[1].value.rExpr); }
#line 793 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 32:
#line 137 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmt) = new AST::Return(); (yylhs.value.stmt)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 799 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 33:
#line 138 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmt) = new AST::Return((yystack_[1].value.rExpr)); (yylhs.value.stmt)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 805 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 34:
#line 139 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmt) = (yystack_[0].value.typecase); }
#line 811 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 35:
#line 142 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { 
            (yylhs.value.if_block) = new AST::If((yystack_[5].value.rExpr), (yystack_[3].value.stmts)); 
            AST::If *curr = (yylhs.value.if_block);
            for (AST::If *elif : *(yystack_[1].value.ifs)) {
                std::vector<AST::Statement*> *new_else = new std::vector<AST::Statement*>();
                new_else->push_back(elif);
                curr->set_else(new_else);
                curr = elif;
            }
            curr->set_else((yystack_[0].value.stmts));
            delete (yystack_[1].value.ifs);
            (yylhs.value.if_block)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename);
        }
#line 829 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 36:
#line 156 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.ifs) = (yystack_[0].value.ifs);}
#line 835 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 37:
#line 157 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.ifs) = new std::vector<AST::If*>();  }
#line 841 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 38:
#line 159 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.ifs) = new std::vector<AST::If*>(); (yylhs.value.ifs)->push_back((yystack_[0].value.if_block)); }
#line 847 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 39:
#line 160 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.ifs) = (yystack_[1].value.ifs); (yylhs.value.ifs)->push_back((yystack_[0].value.if_block)); }
#line 853 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 40:
#line 162 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.if_block) = new AST::If((yystack_[3].value.rExpr), (yystack_[1].value.stmts), NULL); (yylhs.value.if_block)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 859 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 41:
#line 164 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmts) = new std::vector<AST::Statement*>(); (yylhs.value.stmts) = (yystack_[0].value.stmts); }
#line 865 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 42:
#line 165 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmts) = new std::vector<AST::Statement*>(); }
#line 871 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 43:
#line 167 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.stmts) = (yystack_[1].value.stmts); }
#line 877 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 44:
#line 169 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.actuals) = new std::vector<AST::RExpr*>(); (yylhs.value.actuals) = (yystack_[1].value.actuals); }
#line 883 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 45:
#line 170 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.actuals) = new std::vector<AST::RExpr*>(); }
#line 889 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 46:
#line 172 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.actuals) = new std::vector<AST::RExpr*>(); (yylhs.value.actuals)->push_back((yystack_[0].value.rExpr)); }
#line 895 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 47:
#line 173 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.actuals) = (yystack_[2].value.actuals); (yylhs.value.actuals)->push_back((yystack_[0].value.rExpr));  }
#line 901 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 48:
#line 175 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.rExpr) = new AST::StrLit((yystack_[0].value.str)); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 907 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 49:
#line 176 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.rExpr) = new AST::IntLit((yystack_[0].value.num)); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 913 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 50:
#line 177 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.rExpr) = new AST::LExpr((yystack_[0].value.str)); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 919 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 51:
#line 178 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.rExpr) = (yystack_[0].value.field); }
#line 925 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 52:
#line 179 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.rExpr) = new AST::Constructor((yystack_[1].value.str), (yystack_[0].value.actuals)); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 931 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 53:
#line 180 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.rExpr) = (yystack_[1].value.rExpr); }
#line 937 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 54:
#line 181 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { std::vector<AST::RExpr*> *args = new std::vector<AST::RExpr*>(); args->push_back((yystack_[0].value.rExpr)); (yylhs.value.rExpr) = new AST::Call((yystack_[2].value.rExpr), "PLUS",    args); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 943 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 55:
#line 182 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { std::vector<AST::RExpr*> *args = new std::vector<AST::RExpr*>(); args->push_back((yystack_[0].value.rExpr)); (yylhs.value.rExpr) = new AST::Call((yystack_[2].value.rExpr), "MINUS",   args); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 949 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 56:
#line 183 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { std::vector<AST::RExpr*> *args = new std::vector<AST::RExpr*>(); args->push_back((yystack_[0].value.rExpr)); (yylhs.value.rExpr) = new AST::Call((yystack_[2].value.rExpr), "TIMES",   args); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 955 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 57:
#line 184 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { std::vector<AST::RExpr*> *args = new std::vector<AST::RExpr*>(); args->push_back((yystack_[0].value.rExpr)); (yylhs.value.rExpr) = new AST::Call((yystack_[2].value.rExpr), "DIVIDE",  args); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 961 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 58:
#line 185 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { std::vector<AST::RExpr*> *args = new std::vector<AST::RExpr*>(); args->push_back((yystack_[0].value.rExpr)); (yylhs.value.rExpr) = new AST::Call((yystack_[2].value.rExpr), "EQUALS",  args); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 967 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 59:
#line 186 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { std::vector<AST::RExpr*> *args = new std::vector<AST::RExpr*>(); args->push_back((yystack_[0].value.rExpr)); (yylhs.value.rExpr) = new AST::Call((yystack_[2].value.rExpr), "ATMOST",  args); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 973 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 60:
#line 187 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { std::vector<AST::RExpr*> *args = new std::vector<AST::RExpr*>(); args->push_back((yystack_[0].value.rExpr)); (yylhs.value.rExpr) = new AST::Call((yystack_[2].value.rExpr), "LESS",    args); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 979 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 61:
#line 188 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { std::vector<AST::RExpr*> *args = new std::vector<AST::RExpr*>(); args->push_back((yystack_[0].value.rExpr)); (yylhs.value.rExpr) = new AST::Call((yystack_[2].value.rExpr), "ATLEAST", args); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 985 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 62:
#line 189 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { std::vector<AST::RExpr*> *args = new std::vector<AST::RExpr*>(); args->push_back((yystack_[0].value.rExpr)); (yylhs.value.rExpr) = new AST::Call((yystack_[2].value.rExpr), "MORE",    args); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 991 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 63:
#line 190 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.rExpr) = new AST::And((yystack_[2].value.rExpr), (yystack_[0].value.rExpr)); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 997 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 64:
#line 191 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.rExpr) = new AST::Or((yystack_[2].value.rExpr), (yystack_[0].value.rExpr)); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 1003 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 65:
#line 192 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.rExpr) = new AST::Not((yystack_[0].value.rExpr)); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 1009 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 66:
#line 193 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.rExpr) = new AST::Call((yystack_[0].value.rExpr), "NEGATE", new std::vector<AST::RExpr*>()); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 1015 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 67:
#line 194 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.rExpr) = new AST::Call((yystack_[3].value.rExpr), (yystack_[1].value.str), (yystack_[0].value.actuals)); (yylhs.value.rExpr)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 1021 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 68:
#line 196 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.field) = new AST::LExpr((yystack_[2].value.rExpr), (yystack_[0].value.str)); (yylhs.value.field)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 1027 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 69:
#line 198 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.typecase) = new AST::Typecase((yystack_[3].value.rExpr), (yystack_[1].value.typeAlts)); (yylhs.value.typecase)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 1033 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 70:
#line 200 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.typeAlts) = new std::vector<AST::TypeAlt*>(); (yylhs.value.typeAlts)->push_back((yystack_[0].value.typeAlt)); }
#line 1039 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 71:
#line 201 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.typeAlts) = (yystack_[1].value.typeAlts); (yylhs.value.typeAlts)->push_back((yystack_[0].value.typeAlt)); }
#line 1045 "quack.tab.cxx" // lalr1.cc:870
    break;

  case 72:
#line 203 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:870
    { (yylhs.value.typeAlt) = new AST::TypeAlt((yystack_[5].value.str), (yystack_[3].value.str), (yystack_[1].value.stmts)); (yylhs.value.typeAlt)->set_position(yylhs.location.begin.line, yylhs.location.begin.column, yylhs.location.end.line, yylhs.location.end.column, yylhs.location.begin.filename); }
#line 1051 "quack.tab.cxx" // lalr1.cc:870
    break;


#line 1055 "quack.tab.cxx" // lalr1.cc:870
            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -59;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
       3,    17,    31,    86,     3,   -59,    26,   -59,    22,   -59,
     -59,    48,    82,    82,    82,    82,    82,    82,   -59,    86,
     -59,   -59,   105,    29,   -59,   -59,    42,    44,    56,    82,
     -59,    40,   -59,   142,   -59,   227,     1,   247,   267,    96,
     307,   -59,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    66,   -59,    71,    82,    -4,    46,    45,
     -59,   -59,   -11,   326,    50,   159,   -59,    75,    86,    86,
     -59,   118,   118,   118,     1,     1,   118,   118,    96,    96,
      67,    67,    40,    59,   176,    97,     0,    42,   -59,    82,
      82,   -59,    68,    12,   -59,    73,    76,   -59,    82,   -59,
     -59,   108,    86,    83,   -59,   326,   193,   112,   -59,   -59,
     106,   -59,   210,   -59,   126,    86,   -59,   104,    82,   116,
     106,   -59,   -59,   135,   115,   126,   -59,   126,    86,   287,
     109,   -59,   -59,   -59,   120,   -59,   -59,   119,   127,    86,
      86,    42,   -59,   -59,   128,   129,   122,   -59,   -59,    28,
      86,   146,   130,   132,   -59,    86,   144,   -59
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,     0,     0,     8,     3,     5,     0,     1,    50,    48,
      49,     0,     0,     0,     0,     0,     0,     0,     2,     7,
       9,    25,     0,    51,    34,     6,    15,     0,     0,     0,
      52,    50,    32,     0,    51,     0,    65,     0,     0,    66,
       0,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    31,     0,     0,     0,     0,    14,
      16,    45,     0,    46,     0,     0,    33,     0,     8,     8,
      53,    58,    59,    61,    63,    64,    60,    62,    55,    54,
      56,    57,    68,     0,     0,     0,     0,     0,    44,     0,
       0,    27,     0,     0,    70,     0,     0,    67,     0,    28,
      18,     0,     8,     0,    17,    47,     0,     0,    69,    71,
      37,    26,     0,    13,    20,     8,    29,     0,     0,    42,
      36,    38,    30,     0,     0,    19,    21,    20,     8,     0,
       0,    35,    41,    39,     0,    12,    22,     0,     0,     8,
       8,    15,    11,    72,     0,     0,     0,    40,    43,     0,
       8,     0,     0,     0,    24,     8,     0,    23
  };

  const short
  parser::yypgoto_[] =
  {
     -59,   -59,   -59,   -59,   -58,   -59,   172,   -59,    37,   -59,
      92,    65,   -59,    70,   174,   -59,   -59,   -59,    89,   -59,
     -59,   114,   -59,   -10,    -3,   -59,   -59,   117
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,     2,     3,     4,    18,    19,     5,   103,    58,    59,
      60,   124,   125,   126,    20,    21,   119,   120,   121,   131,
     132,    30,    62,    22,    34,    24,    93,    94
  };

  const unsigned char
  parser::yytable_[] =
  {
      23,    33,    35,    36,    37,    38,    39,    40,   101,     1,
      95,    96,    42,    43,    44,    92,    23,    63,    88,    65,
       6,    89,    47,    48,    49,    50,    51,    52,    53,    85,
     102,     7,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,   108,   114,    57,    84,    31,     9,    10,
      27,    31,     9,    10,    26,    28,    29,   127,   150,    64,
      13,   151,    55,    56,    13,    23,    23,    16,    27,    82,
     138,    16,    17,    61,    83,    86,    17,    87,    92,   105,
     106,   144,   145,    32,    90,    31,     9,    10,   112,     8,
       9,    10,   152,    98,    53,    11,    12,   156,    13,    23,
     100,   107,    13,    14,   110,    16,    15,   111,   129,    16,
      17,   113,    23,   115,    17,   117,    42,    43,    44,    45,
      46,    51,    52,    53,   118,    23,    47,    48,    49,    50,
      51,    52,    53,   123,   128,   130,    23,    23,   134,   140,
      54,    49,    50,    51,    52,    53,   135,    23,   141,   153,
     142,   149,    23,    42,    43,    44,    45,    46,   143,   147,
     148,   154,   155,    47,    48,    49,    50,    51,    52,    53,
      42,    43,    44,    45,    46,   157,    25,    66,   146,   104,
      47,    48,    49,    50,    51,    52,    53,    42,    43,    44,
      45,    46,   137,    41,    91,   136,    97,    47,    48,    49,
      50,    51,    52,    53,    42,    43,    44,    45,    46,   133,
     109,    99,     0,     0,    47,    48,    49,    50,    51,    52,
      53,    42,    43,    44,    45,    46,     0,     0,   116,     0,
       0,    47,    48,    49,    50,    51,    52,    53,    42,    43,
      44,    45,    46,     0,     0,   122,     0,     0,    47,    48,
      49,    50,    51,    52,    53,     0,     0,    67,    42,    43,
      44,    45,    46,     0,     0,     0,     0,     0,    47,    48,
      49,    50,    51,    52,    53,     0,     0,    68,    42,    43,
      44,    45,    46,     0,     0,     0,     0,     0,    47,    48,
      49,    50,    51,    52,    53,     0,     0,    69,    42,    43,
      44,    45,    46,     0,     0,     0,     0,     0,    47,    48,
      49,    50,    51,    52,    53,     0,     0,   139,    42,    43,
      44,    45,    46,     0,     0,     0,     0,     0,    47,    48,
      49,    50,    51,    52,    53,     0,    70,    42,    43,    44,
      45,    46,     0,     0,     0,     0,     0,    47,    48,    49,
      50,    51,    52,    53
  };

  const short
  parser::yycheck_[] =
  {
       3,    11,    12,    13,    14,    15,    16,    17,     8,     6,
      68,    69,    11,    12,    13,     3,    19,    27,    29,    29,
       3,    32,    21,    22,    23,    24,    25,    26,    27,    33,
      30,     0,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    31,   102,     3,    56,     3,     4,     5,
      28,     3,     4,     5,    28,    33,    34,   115,    30,     3,
      16,    33,    33,    34,    16,    68,    69,    23,    28,     3,
     128,    23,    28,    29,     3,    29,    28,    32,     3,    89,
      90,   139,   140,    35,    34,     3,     4,     5,    98,     3,
       4,     5,   150,    34,    27,     9,    10,   155,    16,   102,
       3,    33,    16,    17,    31,    23,    20,    31,   118,    23,
      28,     3,   115,    30,    28,     3,    11,    12,    13,    14,
      15,    25,    26,    27,    18,   128,    21,    22,    23,    24,
      25,    26,    27,     7,    30,    19,   139,   140,     3,    30,
      35,    23,    24,    25,    26,    27,    31,   150,    28,     3,
      31,    29,   155,    11,    12,    13,    14,    15,    31,    31,
      31,    31,    30,    21,    22,    23,    24,    25,    26,    27,
      11,    12,    13,    14,    15,    31,     4,    35,   141,    87,
      21,    22,    23,    24,    25,    26,    27,    11,    12,    13,
      14,    15,   127,    19,    35,   125,    82,    21,    22,    23,
      24,    25,    26,    27,    11,    12,    13,    14,    15,   120,
      93,    35,    -1,    -1,    21,    22,    23,    24,    25,    26,
      27,    11,    12,    13,    14,    15,    -1,    -1,    35,    -1,
      -1,    21,    22,    23,    24,    25,    26,    27,    11,    12,
      13,    14,    15,    -1,    -1,    35,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    30,    11,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    30,    11,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    30,    11,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    30,    11,    12,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    11,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    21,    22,    23,
      24,    25,    26,    27
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     6,    37,    38,    39,    42,     3,     0,     3,     4,
       5,     9,    10,    16,    17,    20,    23,    28,    40,    41,
      50,    51,    59,    60,    61,    42,    28,    28,    33,    34,
      57,     3,    35,    59,    60,    59,    59,    59,    59,    59,
      59,    50,    11,    12,    13,    14,    15,    21,    22,    23,
      24,    25,    26,    27,    35,    33,    34,     3,    44,    45,
      46,    29,    58,    59,     3,    59,    35,    30,    30,    30,
      29,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,     3,     3,    59,    33,    29,    32,    29,    32,
      34,    35,     3,    62,    63,    40,    40,    57,    34,    35,
       3,     8,    30,    43,    46,    59,    59,    33,    31,    63,
      31,    31,    59,     3,    40,    30,    35,     3,    18,    52,
      53,    54,    35,     7,    47,    48,    49,    40,    30,    59,
      19,    55,    56,    54,     3,    31,    49,    47,    40,    30,
      30,    28,    31,    31,    40,    40,    44,    31,    31,    29,
      30,    33,    40,     3,    31,    30,    40,    31
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    36,    37,    38,    38,    39,    39,    40,    40,    41,
      41,    42,    42,    43,    44,    44,    45,    45,    46,    47,
      47,    48,    48,    49,    49,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    51,    52,    52,    53,    53,
      54,    55,    55,    56,    57,    57,    58,    58,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    60,    61,
      62,    62,    63
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     0,     1,     2,     1,     0,     1,
       2,    10,     9,     2,     1,     0,     1,     3,     3,     1,
       0,     1,     2,    10,     8,     1,     5,     4,     4,     6,
       6,     2,     2,     3,     1,     7,     1,     0,     1,     2,
       5,     1,     0,     4,     3,     2,     1,     3,     1,     1,
       1,     1,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     4,     3,     5,
       1,     2,     6
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "$end", "error", "$undefined", "IDENT", "STRING_LIT", "INT_LIT",
  "CLASS", "DEF", "EXTENDS", "RETURN", "TYPECASE", "EQUALS", "ATMOST",
  "ATLEAST", "AND", "OR", "NOT", "IF", "ELIF", "ELSE", "WHILE", "'<'",
  "'>'", "'-'", "'+'", "'*'", "'/'", "'.'", "'('", "')'", "'{'", "'}'",
  "','", "':'", "'='", "';'", "$accept", "program", "opt_classes",
  "classes", "opt_statements", "statements", "class", "extends",
  "opt_typed_args", "typed_args", "typed_arg", "opt_methods", "methods",
  "method", "statement", "if_block", "opt_elifs", "elifs", "elif",
  "opt_else", "else", "opt_actuals", "actuals", "r_expr", "field",
  "typecase", "type_alternatives", "type_alternative", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,    91,    91,    96,    97,    99,   100,   102,   103,   105,
     106,   108,   109,   111,   113,   114,   116,   117,   119,   121,
     122,   124,   125,   127,   128,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   141,   156,   157,   159,   160,
     162,   164,   165,   167,   169,   170,   172,   173,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   196,   198,
     200,   201,   203
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  parser::token_number_type
  parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      28,    29,    25,    24,    32,    23,    27,    26,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    33,    35,
      21,    34,    22,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    30,     2,    31,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20
    };
    const unsigned user_token_number_max_ = 275;
    const token_number_type undef_token_ = 2;

    if (static_cast<int> (t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 1616 "quack.tab.cxx" // lalr1.cc:1181
#line 205 "/Users/alonzoaltamirano/Repos/FinalAST/AST/src/quack.yxx" // lalr1.cc:1182

#include "include/Messages.h"

void yy::parser::error(const location_type& loc, const std::string& msg)
{
  report::error_at(loc, msg);
}

