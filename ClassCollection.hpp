//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef AS2JAVA_JAVA_CLASSES_COLLECTION_HPP_INCLUDED
#define AS2JAVA_JAVA_CLASSES_COLLECTION_HPP_INCLUDED

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <map>
#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Class;

class ClassCollection
:   public boost::noncopyable
{

    /// @name ClassCollection implementation
    /// @{
public:
    void push_back(const std::string& _className, const std::string& _fullName, const std::string& _packageName, const boost::filesystem::path& _path, bool _isTarget);

    Class* findClassByFullName(const std::string& _fullName);

    class I_ClassVisitor 
    {
    public:
        virtual void begin() = 0;
        virtual void visit(const Class* _pClass) = 0;
        virtual void end() = 0;
    };

    void visitAll(I_ClassVisitor& _visitor);

    /// Dump the names of the files that had parse errors.
    void dumpParseErrors(const std::string& _basePath);
    /// @}

    /// @name Static methods
    /// @{
public:
    static ClassCollection& getInstance();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             ClassCollection();
    virtual ~ClassCollection();
    /// @}

    /// @name Member Variables
    /// @{
private:
    typedef std::multimap<std::string, Class*>  map_name_to_class_type;
    typedef std::pair<std::string, Class*>      map_name_to_class_pair_type;

    map_name_to_class_type  m_mapNameToClass;
    map_name_to_class_type  m_mapFullNameToClass;

    std::list<Class*>   m_errorClasses;
    /// @}

};  // class ClassCollection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // AS2JAVA_JAVA_CLASSES_COLLECTION_HPP_INCLUDED
