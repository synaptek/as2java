//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef AS2JAVA_JAVA_GRAMMAR_HPP_INCLUDED
#define AS2JAVA_JAVA_GRAMMAR_HPP_INCLUDED

#include <boost/spirit/core.hpp>
#include <boost/spirit/dynamic.hpp>
#include <boost/spirit/utility/chset.hpp>
#include <boost/spirit/iterator/file_iterator.hpp>
#include <boost/spirit/error_handling/exceptions.hpp>

#include "ParserState.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct Grammar
:   public boost::spirit::grammar<Grammar>
{
    /// @name Types
    /// @{
public:
    typedef Grammar                             self_t;

    typedef char                                    char_t;
    typedef boost::spirit::file_iterator<char_t>    iterator_t;
    typedef boost::spirit::scanner<iterator_t>      scanner_t;
    typedef boost::spirit::rule<scanner_t>          rule_t;

    typedef boost::spirit::chset<unsigned char>     chset_t;
    typedef boost::spirit::chlit<unsigned char>     chlit_t;
    /// @}

    /// @name Scanner
    /// @{
public:
    template <typename ScannerT>
    struct definition
    {
        /// @name definition implementation
        /// @{
    public:
        boost::spirit::rule<ScannerT> const&
        start() const { return parser; }
        /// @}

        /// @name 'Structors
        /// @{
    public:
        definition(Grammar const& self);
        /// @}

        /// @name Rules 
        /// @{
    public:
        boost::spirit::rule<ScannerT>
            reserved_words_not_names_rule,
            name_rule, 
            package_name_rule,
            simple_type_rule,
            type_rule,
            visibility_rule,
            package_statement,
            package_body,
            declare_namespace,
            use_statement,
            import_statement,
            parameter_rule, 
            parm_list_rule, 
            method_body, 
            empty_body,
            nested_enclosure, 
            nested_enclosures, 
            anything_but_curlies, 
            open_curly, 
            close_curly,
            empty_parm_list_rule, 
            non_empty_parm_list_rule, 
            method_parm_list_rule,
            rest_of_variable_rule,
            simple_string,
            concatentated_strings,
            argument_list,
            construct_object,
            table_initializer,
            variable_decl_list_rule, 
            variable_decl_rule,
            illegal_variable_declaration,
            static_class_initializer,
            enum_declaration,
            member_type_declaration_modifiers,
            member_type_declaration_modifier,
            member_type_declaration,
            type_specifier,
            method_declaration,
            empty_method_body, 
            member_implementation,
            import_statements,
            type_hint_annotation,
            true_or_false,
            ignore_attribute,
            default_value_attribute,
            reference_type_attribute,
            edit_as_attribute,
            type_hint_attribute,
            extensions_attribute,
            editor_data_annotation,
            bindable_annotation,
            category_attribute,
            format_attribute,
            inspectable_annotation,
            annotation_rule,
            annotation_body,
            annotation_parms,
            inner_string_parser,
            class_modifiers,
            class_definition, 
            class_body,
            interface_definition, 
            interface_body, 
            interface_member_declaration,
            name_attribute,
            type_attribute,
            event_definition_attribute,
            event_definition_body,
            event_definition,
            extends_or_implements_rule, 
            //throws_rule,
            extends_rule, 
            implements_rule,
            junk_chars,
            parser;

        boost::spirit::assertion<int>   expect_semicolon;
        boost::spirit::guard<int>       missing_semicolon_guard;
        /// @}

    };  // struct definition
    /// @}

    /// @name 'Structors
    /// @{
public:
    Grammar(ParserState& _parserState);
    /// @}

    /// @name Member Variables
    /// @{
public:
    chset_t Char;
    chset_t Letter;
    chset_t Digit;
    chset_t NameChar;
    chset_t WhiteSpace;
    chset_t OpenCloseCurly;
    chset_t JunkChars;

    ParserState&    m_parserState;
    /// @}

};  // class Grammar

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // AS2JAVA_JAVA_GRAMMAR_HPP_INCLUDED
