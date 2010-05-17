//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "ClassCollection.hpp"
#include "SymbolTable.hpp"
#include "Class.hpp"
#include "Parser.hpp"
#include "Symbol.hpp"

#include <fstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ClassCollection::ClassCollection()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ClassCollection::~ClassCollection()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ClassCollection&
ClassCollection::getInstance()
{
    static ClassCollection sm_instance;
    return sm_instance;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ClassCollection::push_back(const std::string& _className, const std::string& _fullName, const std::string& _packageName, const boost::filesystem::path& _path, bool _isTarget)
{
    Class* pClass = new Class(_className, _fullName, _packageName, _isTarget);
    pClass->setSourcePath(_path);

    m_mapNameToClass.insert(map_name_to_class_pair_type(_className, pClass));
    m_mapFullNameToClass.insert(map_name_to_class_pair_type(_fullName, pClass));

    SymbolTable::getInstance().push_back(new Symbol(pClass, _className, false, true, Symbol::CLASS));

    Parser parser(pClass);
    if (!parser.parse(_path))
    {
        m_errorClasses.push_back(pClass);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Class*
ClassCollection::findClassByFullName(const std::string& _fullName)
{
    map_name_to_class_type::iterator iter = m_mapFullNameToClass.find(_fullName);

    if (iter == m_mapFullNameToClass.end())
    {
        return NULL;
    }

    return iter->second;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ClassCollection::visitAll(I_ClassVisitor& _visitor)
{
    _visitor.begin();

    for(map_name_to_class_type::iterator iter = m_mapNameToClass.begin(); iter != m_mapNameToClass.end(); iter++)
    {
        _visitor.visit(iter->second);
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ClassCollection::dumpParseErrors(const std::string& _basePath)
{
    boost::filesystem::path path(_basePath);
    path /= "parseErrors.txt";
    path.normalize();

    std::ofstream out(path.string().c_str(), std::ios::trunc);

    for(std::list<Class*>::iterator iter = m_errorClasses.begin(); iter != m_errorClasses.end(); iter++)
    {
        out << (*iter)->getSourcePath() << std::endl;
    }

    out.close();
}
