//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "Class.hpp"
#include "Symbol.hpp"
#include "ClassCollection.hpp"
#include "SymbolTable.hpp"

#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Class::Class(const std::string& _className, const std::string& _fullName, const std::string& _packageName, bool _isTarget)
:   m_className(_className)
,   m_fullName(_fullName)
,   m_packageName(_packageName)
,   m_isTarget(_isTarget)
,   m_depencyWalked(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Class::~Class()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Class::getClassName() const
{
    return m_className;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Class::getFullName() const
{
    return m_fullName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Class::getPackageName() const
{
    return m_packageName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::setSourcePath(const boost::filesystem::path& _path)
{
    m_hasSourcePath = true;
    m_sourcePath = _path;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const bool
Class::hasSourcePath() const
{
    return m_hasSourcePath;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::filesystem::path&
Class::getSourcePath() const
{
    return m_sourcePath;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::setSuperClass(const std::string& _superClassName)
{
    m_superClassName = _superClassName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::addImplementsInterface(const std::string& _interfaceName)
{
    m_interfaces.push_back(_interfaceName);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Class*
Class::getSuperClass(bool _loose) const
{
    if (m_superClassName.empty())
    {
        return NULL;
    }

    std::list<Symbol*> symbols;
    SymbolTable::getInstance().findSymbols(m_superClassName, false, symbols);

    if (symbols.size() == 1 || (symbols.size() > 1 && _loose))
    {
        return symbols.front()->getClass();
    }

    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Class::getSuperClassName() const
{
    return m_superClassName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::addSymbolImport(Symbol* _pSymbol)
{
    m_symbolsToImport.insert(_pSymbol);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::addMethod(Method* _pMethod)
{
    m_methods.push_back(_pMethod);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::addField(Symbol* _pField)
{
    m_fields.push_back(_pField);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::setCategoryHandle(const std::string& _handle)
{
    m_categoryHandle = _handle;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::setCategory(const std::string& _category)
{
    m_category = _category;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::setCategoryStrategy(const std::string& _strategy)
{
    m_categoryStrategy = _strategy;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::visitAllMethods(Class::I_MethodVisitor& _visitor) const
{
    _visitor.begin();

    for(method_list_type::const_iterator iter = m_methods.begin(); iter != m_methods.end(); iter++)
    {
        _visitor.visit(*iter);
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::visitAllFields(Class::I_FieldVisitor& _visitor) const
{
    _visitor.begin();

    for(field_list_type::const_iterator iter = m_fields.begin(); iter != m_fields.end(); iter++)
    {
        _visitor.visit(*iter);
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::visitAllInterfaces(Class::I_InterfaceVisitor& _visitor) const
{
    _visitor.begin();

    for(interfaces_type::const_iterator iter = m_interfaces.begin(); iter != m_interfaces.end(); iter++)
    {
        _visitor.visit(*iter);
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::rewrite()
{
    // Don't write this if it's not a target class
    if (!m_isTarget)
        return;

    // Short circuit if nothing to do
    if (m_symbolsToImport.size() == 0)
        return;

    // Read everything into individual lines, looking for marker /*~ END HEADER ~*/

    bool beforeMarker = true;

    std::ifstream input(m_sourcePath.string().c_str());

    char szLine[4096];
    std::string lineData;
    while(input)
    {
        input.getline(szLine, sizeof(szLine) - 1);
        lineData = szLine;

        if (!input)
        {
            if (!input.eof())
            {
                std::cout << "Error reading " << m_sourcePath.string() << std::endl;
                return;
            }
        }
        else
        {
            if (beforeMarker)
            {
                if (strcmp(lineData.c_str(), "/*~ END HEADER ~*/") == 0)
                {
                    beforeMarker = false;
                }
                else
                {
                    m_beforeMarker.push_back(lineData);
                }
            }
            else
            {
                m_afterMarker.push_back(lineData);
            }
        }
    }

    if (beforeMarker)
    {
        std::cout << "Error, /*~ END HEADER ~*/ not found" << std::endl;
        input.close();
        return;
    }

    for(symbols_to_import_type::iterator iter = m_symbolsToImport.begin(); iter != m_symbolsToImport.end(); iter++)
    {
        // Output the main line
        std::stringstream outputLine;

        outputLine << "import " << ((*iter)->isStatic() ? "static " : "")
            << (*iter)->getClass()->getFullName();

        // If importing a static method or member variable, drill down to the name.
        if ((*iter)->isStatic())
        {
            outputLine << "." << (*iter)->getName();
        }

        outputLine << ";" << ((*iter)->isPublic() && !(*iter)->isStatic() ? "" : " // JC_NON_PUBLIC_STATIC_IMPORT");

        m_linesToInsert.push_back(outputLine.str());
    }

    input.close();

    std::string tmpOutputFileName(m_sourcePath.string());
    //tmpOutputFileName += ".x";

    std::ofstream output(tmpOutputFileName.c_str(), std::ios::trunc);

    for(java_source_lines_type::iterator iter = m_beforeMarker.begin(); iter != m_beforeMarker.end(); iter++)
    {
        output << *(iter) << std::endl;
    }

    output << "/*~ END HEADER ~*/" << std::endl;

    for(java_source_lines_type::iterator iter = m_linesToInsert.begin(); iter != m_linesToInsert.end(); iter++)
    {
        output << *(iter) << std::endl;
    }

    for(java_source_lines_type::iterator iter = m_afterMarker.begin(); iter != m_afterMarker.end(); iter++)
    {
        output << *(iter) << std::endl;
    }

    output.close();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::importedPackage(const std::string& _importedPackage, bool _staticImport)
{
    // Take off the end; it's either .*; or ;
    boost::iterator_range<std::string::const_iterator> constEnd = boost::algorithm::find_last(_importedPackage, ".*;");

    // Didn't find ".*;", so not importing an entire package
    if (constEnd.begin() == _importedPackage.end())
    {
        constEnd = boost::algorithm::find_last(_importedPackage, ";");

        std::string fullSymbolName(_importedPackage.begin(), constEnd.begin());
        std::string importedClassName;

        if (_staticImport)
        {
        	// If this is a static import, erase everything from the trailing "."
        	// to the end of the string to remove the .member that's being
        	// imported statically.
            importedClassName = std::string(fullSymbolName.begin(), boost::algorithm::find_last(fullSymbolName, ".").begin());
        }
        else
        {
            importedClassName = fullSymbolName;
        }

        m_importedClasses.insert(importedClassName);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Class::findDependencies(std::set<Class*>& _dependencies)
{
    // Prevent walking if already walked in case there are circular references
    if (m_depencyWalked)
        return;

    m_depencyWalked = true;

    _dependencies.insert(this);

    // Iterate through the imported classes
    for(imported_symbols_type::iterator iter = m_importedClasses.begin(); iter != m_importedClasses.end(); iter++)
    {
        Class* const pClass = ClassCollection::getInstance().findClassByFullName(*iter);

        if (pClass != NULL)
        {
            pClass->findDependencies(_dependencies);
        }
    }
}
