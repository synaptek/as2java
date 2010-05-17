//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef AS2JAVA_JAVA_METHOD_HPP_INCLUDED
#define AS2JAVA_JAVA_METHOD_HPP_INCLUDED

#include <string>
#include <list>
#include <utility>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Method
{
    /// @name Types
    /// @{
public:
    typedef std::list<std::pair<std::string, std::string> > parm_list_type;
    /// @}

    /// @name Method implementation
    /// @{
public:
    void addParameter(const std::string& _parmType, const std::string& _parmName);

    const parm_list_type& getParameters() const { return m_parameters; }

    class I_ArgsVisitor
    {
    public:
        virtual void begin() = 0;
        virtual void visit(const std::string& _type, const std::string& _name) = 0;
        virtual void end() = 0;
    };

    void visitAllArgs(I_ArgsVisitor& _visitor) const;

    const std::string& getName() const { return m_name; }
    const std::string& getType() const { return m_type; }
    bool isStatic() const { return m_static; }
    bool isPublic() const { return m_public; }
    /// @}

    /// @name 'Structors
    /// @{
public:
             Method(const std::string& _name, const std::string& _type, bool _static, bool _public);
    virtual ~Method();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string         m_name;
    std::string         m_type;
    bool                m_static;
    bool                m_public;

    parm_list_type      m_parameters;

    /// @}

};  // class Method

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // AS2JAVA_JAVA_METHOD_HPP_INCLUDED
