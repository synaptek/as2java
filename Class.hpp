//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef AS2JAVA_CLASS_HPP_INCLUDED
#define AS2JAVA_CLASS_HPP_INCLUDED

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <list>
#include <set>

class Symbol;
class Method;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Class
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::list<Method*>  method_list_type;
    typedef std::list<Symbol*>  field_list_type;
    /// @}

    /// @name Class implementation
    /// @{
public:
    /// Get the name of the class sans package name.
    const std::string& getClassName() const;

    /// Get the fully qualified class name including the package name.
    const std::string& getFullName() const;

    /// Get the package name
    const std::string& getPackageName() const;

    /// Set the path of the source file of the class, if it's known.
    void setSourcePath(const boost::filesystem::path& _path);

    /// @return true if setSourcePath has set a path.
    const bool hasSourcePath() const;

    const boost::filesystem::path& getSourcePath() const;

    void setSuperClass(const std::string& _superClassName);

    void addImplementsInterface(const std::string& _interfaceName);

    /// @param _loose - if true then if more than one class might be
    ///             the superclass then pick the first one.  If false
    ///             then NULL is returned if more than one class exists
    ///             by the super class's name
    const Class* getSuperClass(bool _loose = true) const;

    const std::string& getSuperClassName() const;

    void addSymbolImport(Symbol* _pSymbol);

    void addMethod(Method* _pMethod);

    void addField(Symbol* _pField);

    void setCategoryHandle(const std::string& _handle);
    void setCategory(const std::string& _category);
    void setCategoryStrategy(const std::string& _strategy);

    const std::string& getCategoryHandle() const { return m_categoryHandle; }
    const std::string& getCategory() const { return m_category; }
    const std::string& getCategoryStrategy() const { return m_categoryStrategy; }

    const method_list_type& getMethods() const { return m_methods; }

    class I_MethodVisitor
    {
    public:
        virtual void begin() = 0;
        virtual void visit(const Method* _pMethod) = 0;
        virtual void end() = 0;
    };

    void visitAllMethods(I_MethodVisitor& _visitor) const;

    class I_FieldVisitor
    {
    public:
        virtual void begin() = 0;
        virtual void visit(const Symbol* _pMethod) = 0;
        virtual void end() = 0;
    };
    void visitAllFields(I_FieldVisitor& _visitor) const;

    class I_InterfaceVisitor
    {
    public:
        virtual void begin() = 0;
        /// @param _interfaceName - The name of an interface that this class implements,
        ///     but the name is possibly not fully qualified as it is exactly what
        ///     was in the source file.
        virtual void visit(const std::string& _interfaceName) = 0;
        virtual void end() = 0;
    };
    /// Get all of the interfaces that this class implements
    void visitAllInterfaces(I_InterfaceVisitor& _visitor) const;

    void rewrite();

    /// Indicate that a package or class or member variable is being imported.
    void importedPackage(const std::string& _importPackage, bool _staticImport);

    void findDependencies(std::set<Class*>& _dependencies);
    /// @}

    /// @name 'Structors
    /// @{
public:
    /// @param _className Name of the class
    /// @param _fullName Fully qualified class name including the namespace.
    Class(const std::string& _className, const std::string& _fullName, const std::string& _packageName, bool _isTarget);

    virtual ~Class();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string             m_className;
    std::string             m_superClassName;
    std::string             m_fullName;
    std::string             m_packageName;
    std::string             m_categoryHandle;
    std::string             m_category;
    std::string             m_categoryStrategy;

    /// True if this is a target class and can be written
    bool                    m_isTarget;

    /// True if m_sourcePath has been set
    bool                    m_hasSourcePath;
    boost::filesystem::path m_sourcePath;

    typedef std::set<Symbol*>       symbols_to_import_type;
    typedef std::list<std::string>      java_source_lines_type;

    symbols_to_import_type  m_symbolsToImport;


    java_source_lines_type          m_beforeMarker;
    java_source_lines_type          m_afterMarker;
    java_source_lines_type          m_linesToInsert;

    method_list_type                m_methods;
    field_list_type                 m_fields;

    typedef std::set<std::string>   imported_symbols_type;

    /// Indicator that this class has been walked with findDependencies 
    bool                            m_depencyWalked;

    /// Collection of all of the symbols imported (as strings, not Class)
    imported_symbols_type           m_importedClasses;

    typedef std::list<std::string>  interfaces_type;
    interfaces_type                 m_interfaces;

    /// @}

};  // class Class
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // AS2JAVA_CLASS_HPP_INCLUDED
