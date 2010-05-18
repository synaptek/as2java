//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ParserState.hpp"
#include "SymbolTable.hpp"
#include "Symbol.hpp"
#include "Method.hpp"
#include "Class.hpp"

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ParserState::ParserState(Class* _pClass)
:   m_pClass(_pClass)
{
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ParserState::~ParserState()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::init()
{
    m_valid = false;
    m_static = false;
    m_public = false;
    m_staticImport = false;

    m_memberType = "";
    m_methodName = "";
    m_packageName = "";
    m_className = "";
    m_superClassName = "";

    m_variableNames.clear();

    m_pCurrentMethod = NULL;
    m_parmType = "";
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::publicMember(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    m_public = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::staticMember(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    m_static = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::packageName(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    m_packageName = argument;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::beginImport(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    // New import clause; reset the isStatic
    m_staticImport = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::staticImport(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    // This is a static import
    m_staticImport = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::importPackage(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    m_pClass->importedPackage(argument, m_staticImport);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::categoryHandle(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    m_pClass->setCategoryHandle(argument);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::categoryCategory(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    m_pClass->setCategory(argument);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::categoryStrategy(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    m_pClass->setCategoryStrategy(argument);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::className(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    m_className = argument;

    // The class is added to the symbol table in ClassCollection::push_back

    //std::cout << "Parsing " << m_packageName << "." << m_className << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::superClassName(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    // TODO if this is an interface, pass _First, _Last to implementsInterface(),
    // otherwise execute the following code.

    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    m_superClassName = argument;

    m_pClass->setSuperClass(argument);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::implementsInterface(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    m_pClass->addImplementsInterface(argument);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::methodName(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    m_methodName = argument;

    // TODO m_methodName might have a get/set + " " + methodName.
    // Need to change the first character of methodName to upper case, then
    // strip out the space resulting in getMethodName or setMethodName.

    m_pCurrentMethod = new Method(argument, m_memberType, m_static, m_public);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::variableName(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    m_variableNames.push_back(argument);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::initVariable(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    std::cout << "Initialized variable to " << argument << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::parameterType(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }    

    m_parmType = argument;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::parameterName(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    if (m_pCurrentMethod != NULL)
    {
        m_pCurrentMethod->addParameter(m_parmType, argument);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::beginMemberType(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }    

    m_memberType = argument;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::endMethod(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
#if 0
    std::cout << (m_static ? "Static " : "") << 
        "Method: " << m_memberType << " " << m_methodName << "(...)" << std::endl;
#endif

    // Method name is empty if it's a constructor, and since we're creating a symbol for constructors just ignore it constructors
    if (!m_methodName.empty())
    {
        SymbolTable::getInstance().push_back(new Symbol(m_pClass, m_methodName, m_static, m_public, Symbol::METHOD));
    }

    if (m_pCurrentMethod != NULL)
    {
        m_pClass->addMethod(m_pCurrentMethod);
    }

    m_pCurrentMethod = NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::endVariables(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
#if 0
    std::cout << (m_static ? "Static " : "") << 
        "Variables of type " << m_memberType << std::endl;
#endif

    for(variable_names_type::iterator iter = m_variableNames.begin(); iter != m_variableNames.end(); iter++)
    {
#if 0
        std::cout << "    " << (*iter) << std::endl;
#endif

        Symbol* pField = new Symbol(m_pClass, (*iter), m_static, m_public, Symbol::VARIABLE);
        SymbolTable::getInstance().push_back(pField);

        pField->setType(m_memberType);

        m_pClass->addField(pField);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::endValidMember(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    m_valid = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::endMember(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    if (!m_valid)
    {
        std::cout << "Invalid member in " << m_packageName << "." << m_className << std::endl;
    }
    init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ParserState::missingSemicolon(ParserState::iterator_t _First, const ParserState::iterator_t &_Last)
{
    std::string argument;

    while(_First != _Last)
    {
        argument += *_First++;
    }

    SymbolTable::getInstance().parseError(m_pClass);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

