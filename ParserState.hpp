//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef AS2JAVA_PARSER_STATE_HPP_INCLUDED
#define AS2JAVA_PARSER_STATE_HPP_INCLUDED

#include <boost/spirit/iterator/file_iterator.hpp>

#include <boost/noncopyable.hpp>

#include <string>
#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Class;
class Method;

class ParserState
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef char                                    char_t;
    typedef boost::spirit::file_iterator<char_t>    iterator_t;

    typedef std::list<std::string>                  variable_names_type;
    /// @}

    /// @name ParserState implementation
    /// @{
public:
    /// Indicate that a member (variable or method) is public
    void publicMember(iterator_t _First, iterator_t const& _Last);

    /// Indicate that a member (variable or method) is public
    void staticMember(iterator_t _First, iterator_t const& _Last);

    /// Name of a method
    void methodName(iterator_t _First, iterator_t const& _Last);

    /// Name of a variable
    void variableName(iterator_t _First, iterator_t const& _Last);

    /// Name of a method
    void packageName(iterator_t _First, iterator_t const& _Last);

    /// Begin an import clause
    void beginImport(iterator_t _First, iterator_t const& _Last);

    /// Next import is a static import
    void staticImport(iterator_t _First, iterator_t const& _Last);

    /// Package being imported
    void importPackage(iterator_t _First, iterator_t const& _Last);

    /// Name of the category handle (applies to the next class, not the current one)
    void categoryHandle(iterator_t _First, iterator_t const& _Last);

    /// Name of the category (applies to the next class, not the current one)
    void categoryCategory(iterator_t _First, iterator_t const& _Last);

    /// Name of the category strategy (applies to the next class, not the current one)
    void categoryStrategy(iterator_t _First, iterator_t const& _Last);

    /// Name of a method
    void className(iterator_t _First, iterator_t const& _Last);

    /// Name of the super class
    void superClassName(iterator_t _First, iterator_t const& _Last);

    /// Name of an interface that this class implements
    void implementsInterface(iterator_t _First, iterator_t const& _Last);

    /// Beginning of one or more valid types
    void beginMemberType(iterator_t _First, iterator_t const& _Last);

    /// Method parameter type
    void parameterType(iterator_t _First, iterator_t const& _Last);

    /// Method parameter name
    void parameterName(iterator_t _First, iterator_t const& _Last);

    /// End of valid method
    void endMethod(iterator_t _First, iterator_t const& _Last);

    /// End of valid list of variables
    void endVariables(iterator_t _First, iterator_t const& _Last);

    /// End of a valid member
    void endValidMember(iterator_t _First, iterator_t const& _Last);

    /// End of any member
    void endMember(iterator_t _First, iterator_t const& _Last);

    /// Missing semicolon
    void missingSemicolon(iterator_t _First, iterator_t const& _Last);

    void init();
    /// @}

    /// @name 'Structors
    /// @{
public:
             ParserState(Class* _pClass);
    virtual ~ParserState();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Class*              	m_pClass;
    bool                    m_valid;
    bool                    m_static;
    bool                    m_public;
    bool                    m_staticImport;
    std::string             m_memberType;
    std::string             m_methodName;
    std::string             m_packageName;
    std::string             m_className;
    std::string             m_superClassName;

    variable_names_type     m_variableNames;

    Method*             m_pCurrentMethod;
    std::string             m_parmType;
    /// @}

};  // class ParserState

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // AS2JAVA_PARSER_STATE_HPP_INCLUDED
