//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef AS2JAVA_PARSER_HPP_INCLUDED
#define AS2JAVA_PARSER_HPP_INCLUDED

#include <boost/spirit/core.hpp>
#include <boost/spirit/symbols/symbols.hpp>
#include <boost/spirit/iterator/file_iterator.hpp>

#include <boost/filesystem/path.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Class;

class Parser
{
    /// @name Types
    /// @{
public:
    typedef char                    char_t;
    typedef boost::spirit::file_iterator<char_t>   iterator_t;
    typedef boost::spirit::scanner<iterator_t>     scanner_t;
    typedef boost::spirit::rule<scanner_t>         rule_t;
    /// @}


    /// @name Parser implementation
    /// @{
public:
    bool parse(const boost::filesystem::path& _path);
    /// @}

    /// @name 'Structors
    /// @{
public:
             Parser(Class* _pClass);
    virtual ~Parser();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// The class that this is parsing.
    Class*          m_pClass;
    /// @}

};  // class Parser

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // AS2JAVA_PARSER_HPP_INCLUDED
