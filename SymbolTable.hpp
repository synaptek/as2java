//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to  Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef AS2JAVA_SYMBOL_TABLE_HPP_INCLUDED
#define AS2JAVA_SYMBOL_TABLE_HPP_INCLUDED

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <map>
#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Class;
class Symbol;

class SymbolTable
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::multimap<std::string, Symbol*>     map_name_to_symbol_type;
    typedef std::pair<std::string, Symbol*>         map_name_to_symbol_pair_type;
    /// @}

    /// @name SymbolTable implementation
    /// @{
public:
    void push_back(Symbol* _pSymbol);

    void parseError(Class* _pClass);

    /// @param _method - True if looking only for methods
    void findSymbols(const std::string& _symbol, bool _method, std::list<Symbol*>& _symbols);

    /// Defer the creation of a category handler for this class.
    /// @param _pBaseClass - Base category class
    /// @param _pClass - Category class that requires this handler
    void deferCategoryHandler(const Class* const _pBaseClass, const Class* const _pClass);

    void generateCategoryHandlers(const boost::filesystem::path& _stubPath);

    void dump(const std::string& _basePath);
    /// @}

    /// @name Static methods
    /// @{
public:
    static SymbolTable& getInstance();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             SymbolTable();
    virtual ~SymbolTable();
    /// @}

    /// @name Member Variables
    /// @{
private:
    map_name_to_symbol_type         m_mapNameToSymbol;

    typedef std::map<Class*, std::string>   parse_error_list;
    parse_error_list               		m_parseErrorList;

    typedef std::list<const Class*>                       category_class_list;
    typedef std::map<const Class*, category_class_list>  handler_class_map;

    handler_class_map                   m_handlersToGenerate;
    /// @}

};  // class ClassCollection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // AS2JAVA_SYMBOL_TABLE_HPP_INCLUDED
