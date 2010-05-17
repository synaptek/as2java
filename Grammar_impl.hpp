//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef AS2JAVA_JAVA_GRAMMAR_IMPL_HPP_INCLUDED
#define AS2JAVA_JAVA_GRAMMAR_IMPL_HPP_INCLUDED

/// @note Only include this from Parser.cpp

#include "Grammar.hpp"

#include <boost/bind.hpp>

#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/escape_char.hpp>
#include <boost/spirit/utility/lists.hpp>
#include <boost/spirit/core/primitives/primitives.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct handler
{
    template <typename ScannerT, typename ErrorT>
    boost::spirit::error_status<>
    operator()(ScannerT const& scan, ErrorT const& error) const
    {
        std::cout << "Missing semicolon" << std::endl;
        return boost::spirit::error_status<>(boost::spirit::error_status<>::accept);
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<typename ScannerT>
Grammar::definition<ScannerT>::definition(struct Grammar const& self)
:   expect_semicolon(0)
{
#ifdef BOOST_SPIRIT_DEBUG
    
BOOST_SPIRIT_DEBUG_RULE(static_class_initializer);
BOOST_SPIRIT_DEBUG_RULE(enum_declaration);
BOOST_SPIRIT_DEBUG_RULE(illegal_variable_declaration);
BOOST_SPIRIT_DEBUG_RULE(reserved_words_not_names_rule);
BOOST_SPIRIT_DEBUG_RULE(member_type_declaration);
BOOST_SPIRIT_DEBUG_RULE(name_rule);
BOOST_SPIRIT_DEBUG_RULE(package_name_rule);
BOOST_SPIRIT_DEBUG_RULE(simple_type_rule);
BOOST_SPIRIT_DEBUG_RULE(type_rule);
BOOST_SPIRIT_DEBUG_RULE(visibility_rule);
BOOST_SPIRIT_DEBUG_RULE(package_statement);
BOOST_SPIRIT_DEBUG_RULE(import_statement);
BOOST_SPIRIT_DEBUG_RULE(annotation_rule);
BOOST_SPIRIT_DEBUG_RULE(annotation_body);
BOOST_SPIRIT_DEBUG_RULE(parameter_rule);
BOOST_SPIRIT_DEBUG_RULE(parm_list_rule);
BOOST_SPIRIT_DEBUG_RULE(method_body);
BOOST_SPIRIT_DEBUG_RULE(empty_body);
BOOST_SPIRIT_DEBUG_RULE(nested_enclosure);
BOOST_SPIRIT_DEBUG_RULE(nested_enclosures);
BOOST_SPIRIT_DEBUG_RULE(anything_but_curlies);
BOOST_SPIRIT_DEBUG_RULE(open_curly);
BOOST_SPIRIT_DEBUG_RULE(close_curly);
BOOST_SPIRIT_DEBUG_RULE(empty_parm_list_rule);
BOOST_SPIRIT_DEBUG_RULE(non_empty_parm_list_rule);
BOOST_SPIRIT_DEBUG_RULE(method_parm_list_rule);
BOOST_SPIRIT_DEBUG_RULE(rest_of_method_rule);
BOOST_SPIRIT_DEBUG_RULE(rest_of_variable_rule);
BOOST_SPIRIT_DEBUG_RULE(variable_decl_list_rule);
BOOST_SPIRIT_DEBUG_RULE(variable_decl_rule);
BOOST_SPIRIT_DEBUG_RULE(member_type_declaration);
BOOST_SPIRIT_DEBUG_RULE(member_declaration);
BOOST_SPIRIT_DEBUG_RULE(empty_method_body);
BOOST_SPIRIT_DEBUG_RULE(member_implementation);
BOOST_SPIRIT_DEBUG_RULE(import_statements);
BOOST_SPIRIT_DEBUG_RULE(class_definition);
BOOST_SPIRIT_DEBUG_RULE(class_body);
BOOST_SPIRIT_DEBUG_RULE(interface_definition);
BOOST_SPIRIT_DEBUG_RULE(interface_body);
BOOST_SPIRIT_DEBUG_RULE(interface_member_declaration);
BOOST_SPIRIT_DEBUG_RULE(extends_or_implements_rule);
BOOST_SPIRIT_DEBUG_RULE(extends_rule);
BOOST_SPIRIT_DEBUG_RULE(implements_rule);
BOOST_SPIRIT_DEBUG_RULE(junk_chars);
#endif // BOOST_SPIRIT_DEBUG

    anything_but_curlies =
            *(
                boost::spirit::anychar_p - self.OpenCloseCurly
            )
    ;

    open_curly =
        boost::spirit::str_p("{");

    close_curly =
        boost::spirit::str_p("}");

    nested_enclosure =
            open_curly
        >>  anything_but_curlies
        >>  *nested_enclosure
        >>  close_curly
        >>  anything_but_curlies
    ;

    nested_enclosures =
            anything_but_curlies
        >>  *nested_enclosure
    ;

    empty_method_body =
            open_curly
        >>  close_curly
    ;

    method_body = 
            empty_method_body
        |   boost::spirit::confix_p("{", nested_enclosures, "}")
    ;

    reserved_words_not_names_rule =
            boost::spirit::str_p("public")
        |   boost::spirit::str_p("private")
        |   boost::spirit::str_p("static")
    ;

    name_rule =
            boost::spirit::lexeme_d
            [
                    (self.Letter | '_' ) 
                >>  *(self.NameChar)
            ]
        -   reserved_words_not_names_rule
    ;

    package_name_rule =
            name_rule
        >>  !( 
                    boost::spirit::str_p(".")
                >>  package_name_rule
            )
    ;

    package_statement = 
            boost::spirit::as_lower_d["package"]
        >>  package_name_rule                   [boost::bind(&ParserState::packageName, &self.m_parserState, _1, _2)]
    ;

    import_statement = 
            boost::spirit::as_lower_d["import"] [boost::bind(&ParserState::beginImport, &self.m_parserState, _1, _2)]
        >>  !boost::spirit::str_p("static")     [boost::bind(&ParserState::staticImport, &self.m_parserState, _1, _2)]
        >>  (
                package_name_rule
            >>  (
                        boost::spirit::str_p(";")
                    |   boost::spirit::str_p(".*;")
                )
            )                                   [boost::bind(&ParserState::importPackage, &self.m_parserState, _1, _2)]
    ;

    import_statements =
            +import_statement
    ;

    simple_type_rule =
            boost::spirit::as_lower_d["int"]
        |   boost::spirit::as_lower_d["void"]
            // Punt, anything with a valid name will work here.
        |   (
                    package_name_rule
            )
    ;

    type_rule =
            simple_type_rule
        >>  !boost::spirit::str_p("[]")
    ;

    parameter_rule =
            type_rule                                   [boost::bind(&ParserState::parameterType, &self.m_parserState, _1, _2)]
        >>  !boost::spirit::str_p("...")
        >>  package_name_rule                           [boost::bind(&ParserState::parameterName, &self.m_parserState, _1, _2)]
    ;

    parm_list_rule =
        (boost::spirit::list_p
            (
                parameter_rule, 
                boost::spirit::str_p(",")
            )
        )
    ;

    non_empty_parm_list_rule =
            boost::spirit::confix_p("(", (!parm_list_rule), ")")
    ;

    empty_parm_list_rule =
            boost::spirit::str_p("(")
        >>  boost::spirit::str_p(")")
    ;

    method_parm_list_rule =
            (
                    empty_parm_list_rule
                | non_empty_parm_list_rule
            )
    ;

    throws_rule =
            boost::spirit::str_p("throws")
        >>  boost::spirit::list_p(package_name_rule, ",")
    ;

    rest_of_method_rule =
            method_parm_list_rule
        >>  !throws_rule
        >>  method_body
    ;

    variable_decl_rule =
            name_rule                                   [boost::bind(&ParserState::variableName, &self.m_parserState, _1, _2)]
        >>  boost::spirit::str_p(":")
        >>  type_rule
        >>  !(
                boost::spirit::str_p("=")
                >> (
                            (
                                    boost::spirit::list_p(
                                        boost::spirit::confix_p("\"", *(boost::spirit::lex_escape_ch_p | boost::spirit::anychar_p), "\""),
                                        boost::spirit::str_p("+")
                                    )
                            )
                        |   (
                                    boost::spirit::str_p("new")
                                >>  type_rule
                                >>  boost::spirit::str_p("()")
                            )
                        | *(
                                    boost::spirit::anychar_p 
                                -   (
                                            boost::spirit::str_p(";")
                                        |   boost::spirit::str_p(",")
                                    )
                            )
                    )
            )
    ;

    variable_decl_list_rule =
            variable_decl_rule
        >>  *(
                    boost::spirit::str_p(",")
                >>  variable_decl_list_rule
            )
    ;

    visibility_rule =
            boost::spirit::as_lower_d["public"]
        |   boost::spirit::as_lower_d["private"]
        |   boost::spirit::as_lower_d["protected"]
    ;

    illegal_variable_declaration =
            (
                    boost::spirit::str_p("public")
                >>  boost::spirit::str_p("static")
                >>  boost::spirit::str_p("_JC_POST_STRUCT")
            )
        |   (
                    type_rule
                >>  boost::spirit::str_p("+")
                >>  type_rule
                >>  type_rule
                >>  boost::spirit::str_p(";")
            )
    ;

    member_type_declaration_modifier =
            boost::spirit::as_lower_d["public"]         [boost::bind(&ParserState::publicMember, &self.m_parserState, _1, _2)]
        |   boost::spirit::as_lower_d["private"]
        |   boost::spirit::as_lower_d["protected"]
        |   boost::spirit::as_lower_d["static"]         [boost::bind(&ParserState::staticMember, &self.m_parserState, _1, _2)]
        |   boost::spirit::as_lower_d["final"]
        |   boost::spirit::as_lower_d["abstract"]
    ;

    member_type_declaration_modifiers =
        *member_type_declaration_modifier
    ;

    member_type_declaration =
            member_type_declaration_modifiers
        >>  type_rule                              [boost::bind(&ParserState::beginMemberType, &self.m_parserState, _1, _2)]
    ;

    member_declaration =
            member_type_declaration
        >>  !name_rule
    ;

    enum_declaration =
            !visibility_rule
        >>  boost::spirit::str_p("enum")
        >>  name_rule
        >>  open_curly
        >>  anything_but_curlies
        >>  close_curly
    ;

    member_implementation =
            *annotation_rule
        >>  member_type_declaration_modifiers
        >>  (
        		    // function
                    (
                    		boost::spirit::str_p("function")
                            // Optional for constructors.  If we're parsing a constructor then
                            // the member_type_declaration->type_rule matched the constructor name
                        >>  !name_rule                  [boost::bind(&ParserState::methodName, &self.m_parserState, _1, _2)]
                        >>  rest_of_method_rule         [boost::bind(&ParserState::endMethod, &self.m_parserState, _1, _2)]
                    )
                    // Variable declaration lists
                |   (
							boost::spirit::str_p("var")
                        >>  variable_decl_list_rule
                        >>  (
                                    boost::spirit::str_p(";")
                                |   (*boost::spirit::anychar_p - boost::spirit::str_p(";"))
                                                        [boost::bind(&ParserState::missingSemicolon, &self.m_parserState, _1, _2)]
                            )
                            /*
                            (   
                                    expect_semicolon(boost::spirit::str_p(";"))
                            )
                            */
                                                        
                    )                                   [boost::bind(&ParserState::endVariables, &self.m_parserState, _1, _2)]
            )
    ;

    static_class_initializer =
            !visibility_rule
        >>  !boost::spirit::as_lower_d["static"]
        >>  method_body
    ;

    class_body = 
        *(
                member_implementation           [boost::bind(&ParserState::endValidMember, &self.m_parserState, _1, _2)]
            |   enum_declaration                [boost::bind(&ParserState::endValidMember, &self.m_parserState, _1, _2)]
            |   static_class_initializer        [boost::bind(&ParserState::endValidMember, &self.m_parserState, _1, _2)]
            |   illegal_variable_declaration
        )                                       [boost::bind(&ParserState::endMember, &self.m_parserState, _1, _2)]
    ;

    extends_rule = 
            boost::spirit::as_lower_d["extends"]
    >>  (
                type_rule                       [boost::bind(&ParserState::superClassName, &self.m_parserState, _1, _2)]
        )
    ;

    implements_rule =
            boost::spirit::as_lower_d["implements"]
        >>  boost::spirit::list_p
            (
                    package_name_rule           [boost::bind(&ParserState::implementsInterface, &self.m_parserState, _1, _2)]
                , ","
            )
    ;

    extends_or_implements_rule =
            !extends_rule
        >>  *implements_rule
    ;

    interface_member_declaration =
            (
                    member_declaration
                >>  method_parm_list_rule
                >>  boost::spirit::str_p(";")
            )
        |   member_implementation
    ;

    interface_body = 
        *interface_member_declaration
    ;

    interface_definition =
            *annotation_rule
        >>  !visibility_rule
        >>  !boost::spirit::as_lower_d["final"]
        >>  boost::spirit::as_lower_d["interface"]
        >>  name_rule
        >>  !extends_rule
        >>  (
                    empty_method_body
                |   boost::spirit::confix_p("{", interface_body, "}")
            )
    ;

    annotation_parms =
            *(
                boost::spirit::anychar_p - boost::spirit::str_p(")")
            )
    ;

    // Parse everything inside of quotes, except for the quotes, and allow for C style escapes
    inner_string_parser = 
        *(
                boost::spirit::lex_escape_ch_p 
            |   (
                        boost::spirit::anychar_p 
                )
        )
    ;

    category_handle_string =
        name_rule                               [boost::bind(&ParserState::categoryHandle, &self.m_parserState, _1, _2)]
    ;

    category_category_string =
        name_rule                               [boost::bind(&ParserState::categoryCategory, &self.m_parserState, _1, _2)]
    ;

    category_strategy =
        name_rule                               [boost::bind(&ParserState::categoryStrategy, &self.m_parserState, _1, _2)]
    ;

    category_class_parms =
            boost::spirit::str_p("handle")
        >>  boost::spirit::str_p("=")
        >>  boost::spirit::confix_p("\"", category_handle_string, "\"")
        >>  boost::spirit::str_p(",")
        >>  boost::spirit::str_p("category")
        >>  boost::spirit::str_p("=")
        >>  boost::spirit::confix_p("\"", category_category_string, "\"")
        >>  boost::spirit::str_p(",")
        >>  boost::spirit::str_p("strategy")
        >>  boost::spirit::str_p("=")
        >>  category_strategy
    ;

    annotation_body =
    		boost::spirit::str_p("TypeHint")
		>>  boost::spirit::confix_p("("
				, (
				        boost::spirit::str_p("type=")
					>>  boost::spirit::confix_p("\"", package_name_rule, "\"")
				  )
				, ")")
	;

    annotation_rule = 
        boost::spirit::confix_p("[", annotation_body, "]")
    ;

    class_definition = 
            *annotation_rule
        >>  !visibility_rule
        >>  !boost::spirit::as_lower_d["abstract"]
        >>  boost::spirit::as_lower_d["class"]
        >>  (
                    name_rule                   [boost::bind(&ParserState::className, &self.m_parserState, _1, _2)]
            )
        >>  extends_or_implements_rule
        >>  (
                    empty_method_body
                |   boost::spirit::confix_p("{", class_body, "}")
            )
    ;

    junk_chars = 
        self.JunkChars
    ;

    package_body =
    	    !import_statements
		// Class definition is optional due to some files being empty
		>>  !(
					class_definition
		        |   interface_definition
			)
	;

    parser =
            !package_statement
        >>  boost::spirit::confix_p("{", package_body, "}")
        >>  *junk_chars
        >>  boost::spirit::end_p
    ;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // AS2JAVA_JAVA_GRAMMAR_IMPL_HPP_INCLUDED
