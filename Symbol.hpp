//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef AS2JAVA_SYMBOL_HPP_INCLUDED
#define AS2JAVA_SYMBOL_HPP_INCLUDED

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Class;

class Symbol
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    enum SymbolType
    {
        VARIABLE,
        METHOD,
        CLASS
    };
    /// @}

    /// @name Symbol implementation
    /// @{
public:
    const std::string& getName() const { return m_name; }

    /// Get the type of field or the return type of a method
    const std::string& getType() const { return m_type; }
    void setType(const std::string& _type) { m_type = _type; }

    const Class* getClass() const { return m_pClass; }

    bool isPublic() const { return m_isPublic; }
    bool isStatic() const { return m_isStatic; }
    SymbolType getSymbolType() const { return m_symbolType; }
    /// @}

    /// @name 'Structors
    /// @{
public:
             Symbol(Class* _pClass, const std::string& _name, bool _static, bool _public, SymbolType _symbolType);
    virtual ~Symbol();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Class*      m_pClass;
    std::string     m_name;
    std::string     m_type;
    bool            m_isStatic;
    bool            m_isPublic;
    SymbolType      m_symbolType;
    /// @}

};  // class Symbol

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // AS2JAVA_SYMBOL_HPP_INCLUDED
