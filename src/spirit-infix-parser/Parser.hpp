/* -*-c++-*-
 *
 *  Created by Keith Dennison on 26/07/2013.
 *  Copyright (c) 2013 Keith Dennison. All rights reserved.
 */

#pragma once

#include "boost/spirit/include/qi.hpp"
#include "boost/spirit/include/phoenix_core.hpp"
#include "boost/spirit/include/phoenix_operator.hpp"
#include "boost/spirit/include/phoenix_fusion.hpp"
#include "boost/spirit/include/phoenix_stl.hpp"
#include "boost/fusion/include/adapt_struct.hpp"

namespace kfl {

    namespace Parser {

        namespace qi = boost::spirit::qi;
        namespace ascii = boost::spirit::ascii;

        template <typename Iterator>
        struct expr_subgrammar
        {
            expr_subgrammar()
            {
                using qi::alnum;
                using qi::alpha;
                using qi::char_;
                using qi::lexeme;
                using qi::ulong_;

                var %= lexeme[(alpha >> *(alnum | char_('_'))) - keywords];
                num %= ulong_;

                constr %= "Pack{" >> num >> ',' >> num >> '}';

                aexpr %= constr | var | num | '(' >> expr >> ')';
                aexprlist %= +aexpr;

                alt %= '<' >> num >> '>' >> *var >> "->" >> expr;
                altlist %= alt % ';';

                defn %= var >> '=' >> expr;
                defnlist %= defn % ';';

                case_  %= "case" >> expr >> "of" >> altlist;
                lambda %= '\\' >> *var >> '.' >> expr;
                let    %= "let" >> defnlist >> "in" >> expr;
                letrec %= "letrec" >> defnlist >> "in" >> expr;

                cexpr %= case_ | lambda | let | letrec;

                expr %= aexprlist | cexpr;
            }

            struct keywords_ : qi::symbols<char, std::string> {
                keywords_() {
                    add ("case") ("in") ("let") ("letrec") ("of") ("Pack") ;
                }
            } keywords;

            qi::rule<Iterator, AstVar(), ascii::space_type> var;
            qi::rule<Iterator, AstNum(), ascii::space_type> num;

            qi::rule<Iterator, AstConstr(), ascii::space_type> constr;

            qi::rule<Iterator, AstExpr(), ascii::space_type> aexpr;
            qi::rule<Iterator, AstExprVec(), ascii::space_type> aexprlist;

            qi::rule<Iterator, AstAlt(), ascii::space_type> alt;
            qi::rule<Iterator, AstAltVec(), ascii::space_type> altlist;

            qi::rule<Iterator, AstDefn(), ascii::space_type> defn;
            qi::rule<Iterator, AstDefnVec(), ascii::space_type> defnlist;

            qi::rule<Iterator, AstCase(), ascii::space_type> case_;
            qi::rule<Iterator, AstLambda(), ascii::space_type> lambda;
            qi::rule<Iterator, AstLet(), ascii::space_type> let;
            qi::rule<Iterator, AstLetRec(), ascii::space_type> letrec;

            qi::rule<Iterator, AstExpr(), ascii::space_type> cexpr;
            qi::rule<Iterator, AstExpr(), ascii::space_type> expr;
        };

        template <typename Iterator>
        struct expr_grammar : qi::grammar<Iterator, AstExpr(), ascii::space_type>, expr_subgrammar<Iterator>
        {
            expr_grammar() : expr_grammar::base_type(expr), expr_subgrammar<Iterator>()
            {
            }

            using expr_subgrammar<Iterator>::expr;
        };

        template <typename Iterator>
        struct program_grammar : qi::grammar<Iterator, AstProgram(), ascii::space_type>, expr_subgrammar<Iterator>
        {
            program_grammar() : program_grammar::base_type(program), expr_subgrammar<Iterator>()
            {
                using qi::alnum;
                using qi::alpha;
                using qi::char_;
                using qi::lexeme;

                sc %= var >> *var >> '=' >> expr;
                sclist %= sc % ';';
                program %= sclist;
            }

            using expr_subgrammar<Iterator>::expr;
            using expr_subgrammar<Iterator>::var;

            qi::rule<Iterator, AstScDefn(), ascii::space_type> sc;
            qi::rule<Iterator, AstScDefnVec(), ascii::space_type> sclist;
            qi::rule<Iterator, AstProgram(), ascii::space_type> program;
        };

    } /* end namespace Parser */

} /* end namespace kfl */

BOOST_FUSION_ADAPT_STRUCT(
  kfl::Parser::AstAlt,
  (unsigned long, tag)
  (std::vector<std::string>, vars)
  (kfl::Parser::AstExpr, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
  kfl::Parser::AstCase,
  (kfl::Parser::AstExpr, expr)
  (kfl::Parser::AstAltVec, alts)
)

BOOST_FUSION_ADAPT_STRUCT(
  kfl::Parser::AstConstr,
  (unsigned long, tag)
  (unsigned long, arity)
)

BOOST_FUSION_ADAPT_STRUCT(
  kfl::Parser::AstDefn,
  (std::string, var)
  (kfl::Parser::AstExpr, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
  kfl::Parser::AstLambda,
  (std::vector<std::string>, args)
  (kfl::Parser::AstExpr, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
  kfl::Parser::AstLet,
  (kfl::Parser::AstDefnVec, defns)
  (kfl::Parser::AstExpr, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
  kfl::Parser::AstLetRec,
  (kfl::Parser::AstDefnVec, defns)
  (kfl::Parser::AstExpr, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
  kfl::Parser::AstScDefn,
  (kfl::Parser::AstVar, name)
  (kfl::Parser::AstVarVec, args)
  (kfl::Parser::AstExpr, expr)
)

template<class Iterator>
bool kfl::Parser::parseExpr(Iterator iter, Iterator end, AstExpr & ast)
{
  expr_grammar<Iterator> grm;
  using boost::spirit::ascii::space;
  bool r = phrase_parse(iter, end, grm, space, ast);
  if (iter != end) {
    r = false;
  }
  return r;
}

template<class Iterator>
bool kfl::Parser::parseProgram(Iterator iter, Iterator end, AstProgram & ast)
{
  program_grammar<Iterator> grm;
  using boost::spirit::ascii::space;
  bool r = phrase_parse(iter, end, grm, space, ast);
  if (iter != end) {
    r = false;
  }
  return r;
}
