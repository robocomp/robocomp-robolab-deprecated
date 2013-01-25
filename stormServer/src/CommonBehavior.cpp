// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.1
// Generated from file `CommonBehavior.ice'

#include <CommonBehavior.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 303
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

static const ::std::string __RoboCompCommonBehavior__CommonBehavior__getPeriod_name = "getPeriod";

static const ::std::string __RoboCompCommonBehavior__CommonBehavior__setPeriod_name = "setPeriod";

static const ::std::string __RoboCompCommonBehavior__CommonBehavior__timeAwake_name = "timeAwake";

static const ::std::string __RoboCompCommonBehavior__CommonBehavior__killYourSelf_name = "killYourSelf";

static const ::std::string __RoboCompCommonBehavior__CommonBehavior__getParameterList_name = "getParameterList";

static const ::std::string __RoboCompCommonBehavior__CommonBehavior__setParameterList_name = "setParameterList";

static const ::std::string __RoboCompCommonBehavior__CommonBehavior__reloadConfig_name = "reloadConfig";

static const ::std::string __RoboCompCommonBehavior__CommonBehavior__getState_name = "getState";

::Ice::Object* IceInternal::upCast(::RoboCompCommonBehavior::CommonBehavior* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::RoboCompCommonBehavior::CommonBehavior* p) { return p; }

void
RoboCompCommonBehavior::__read(::IceInternal::BasicStream* __is, ::RoboCompCommonBehavior::CommonBehaviorPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::RoboCompCommonBehavior::CommonBehavior;
        v->__copyFrom(proxy);
    }
}

void
RoboCompCommonBehavior::__write(::IceInternal::BasicStream* __os, ::RoboCompCommonBehavior::State v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 2);
}

void
RoboCompCommonBehavior::__read(::IceInternal::BasicStream* __is, ::RoboCompCommonBehavior::State& v)
{
    ::Ice::Byte val;
    __is->read(val, 2);
    v = static_cast< ::RoboCompCommonBehavior::State>(val);
}

bool
RoboCompCommonBehavior::Parameter::operator==(const Parameter& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(editable != __rhs.editable)
    {
        return false;
    }
    if(value != __rhs.value)
    {
        return false;
    }
    if(type != __rhs.type)
    {
        return false;
    }
    return true;
}

bool
RoboCompCommonBehavior::Parameter::operator<(const Parameter& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(editable < __rhs.editable)
    {
        return true;
    }
    else if(__rhs.editable < editable)
    {
        return false;
    }
    if(value < __rhs.value)
    {
        return true;
    }
    else if(__rhs.value < value)
    {
        return false;
    }
    if(type < __rhs.type)
    {
        return true;
    }
    else if(__rhs.type < type)
    {
        return false;
    }
    return false;
}

void
RoboCompCommonBehavior::Parameter::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(editable);
    __os->write(value);
    __os->write(type);
}

void
RoboCompCommonBehavior::Parameter::__read(::IceInternal::BasicStream* __is)
{
    __is->read(editable);
    __is->read(value);
    __is->read(type);
}

void
RoboCompCommonBehavior::__writeParameterList(::IceInternal::BasicStream* __os, const ::RoboCompCommonBehavior::ParameterList& v)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::RoboCompCommonBehavior::ParameterList::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        p->second.__write(__os);
    }
}

void
RoboCompCommonBehavior::__readParameterList(::IceInternal::BasicStream* __is, ::RoboCompCommonBehavior::ParameterList& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::std::string, ::RoboCompCommonBehavior::Parameter> pair;
        __is->read(const_cast< ::std::string&>(pair.first));
        ::RoboCompCommonBehavior::ParameterList::iterator __i = v.insert(v.end(), pair);
        __i->second.__read(__is);
    }
}

::Ice::Int
IceProxy::RoboCompCommonBehavior::CommonBehavior::getPeriod(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__RoboCompCommonBehavior__CommonBehavior__getPeriod_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompCommonBehavior::CommonBehavior* __del = dynamic_cast< ::IceDelegate::RoboCompCommonBehavior::CommonBehavior*>(__delBase.get());
            return __del->getPeriod(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RoboCompCommonBehavior::CommonBehavior::setPeriod(::Ice::Int period, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompCommonBehavior::CommonBehavior* __del = dynamic_cast< ::IceDelegate::RoboCompCommonBehavior::CommonBehavior*>(__delBase.get());
            __del->setPeriod(period, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::Int
IceProxy::RoboCompCommonBehavior::CommonBehavior::timeAwake(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__RoboCompCommonBehavior__CommonBehavior__timeAwake_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompCommonBehavior::CommonBehavior* __del = dynamic_cast< ::IceDelegate::RoboCompCommonBehavior::CommonBehavior*>(__delBase.get());
            return __del->timeAwake(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, 0, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RoboCompCommonBehavior::CommonBehavior::killYourSelf(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompCommonBehavior::CommonBehavior* __del = dynamic_cast< ::IceDelegate::RoboCompCommonBehavior::CommonBehavior*>(__delBase.get());
            __del->killYourSelf(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::RoboCompCommonBehavior::ParameterList
IceProxy::RoboCompCommonBehavior::CommonBehavior::getParameterList(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__RoboCompCommonBehavior__CommonBehavior__getParameterList_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompCommonBehavior::CommonBehavior* __del = dynamic_cast< ::IceDelegate::RoboCompCommonBehavior::CommonBehavior*>(__delBase.get());
            return __del->getParameterList(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RoboCompCommonBehavior::CommonBehavior::setParameterList(const ::RoboCompCommonBehavior::ParameterList& l, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompCommonBehavior::CommonBehavior* __del = dynamic_cast< ::IceDelegate::RoboCompCommonBehavior::CommonBehavior*>(__delBase.get());
            __del->setParameterList(l, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

void
IceProxy::RoboCompCommonBehavior::CommonBehavior::reloadConfig(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompCommonBehavior::CommonBehavior* __del = dynamic_cast< ::IceDelegate::RoboCompCommonBehavior::CommonBehavior*>(__delBase.get());
            __del->reloadConfig(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::RoboCompCommonBehavior::State
IceProxy::RoboCompCommonBehavior::CommonBehavior::getState(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
#if defined(__BCPLUSPLUS__) && (__BCPLUSPLUS__ >= 0x0600) // C++Builder 2009 compiler bug
            IceUtil::DummyBCC dummy;
#endif
            __checkTwowayOnly(__RoboCompCommonBehavior__CommonBehavior__getState_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::RoboCompCommonBehavior::CommonBehavior* __del = dynamic_cast< ::IceDelegate::RoboCompCommonBehavior::CommonBehavior*>(__delBase.get());
            return __del->getState(__ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::RoboCompCommonBehavior::CommonBehavior::ice_staticId()
{
    return ::RoboCompCommonBehavior::CommonBehavior::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::RoboCompCommonBehavior::CommonBehavior::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::RoboCompCommonBehavior::CommonBehavior);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::RoboCompCommonBehavior::CommonBehavior::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::RoboCompCommonBehavior::CommonBehavior);
}

::IceProxy::Ice::Object*
IceProxy::RoboCompCommonBehavior::CommonBehavior::__newInstance() const
{
    return new CommonBehavior;
}

::Ice::Int
IceDelegateM::RoboCompCommonBehavior::CommonBehavior::getPeriod(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompCommonBehavior__CommonBehavior__getPeriod_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::Ice::Int __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RoboCompCommonBehavior::CommonBehavior::setPeriod(::Ice::Int period, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompCommonBehavior__CommonBehavior__setPeriod_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(period);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::Ice::Int
IceDelegateM::RoboCompCommonBehavior::CommonBehavior::timeAwake(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompCommonBehavior__CommonBehavior__timeAwake_name, ::Ice::Idempotent, __context);
    bool __ok = __og.invoke();
    ::Ice::Int __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RoboCompCommonBehavior::CommonBehavior::killYourSelf(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompCommonBehavior__CommonBehavior__killYourSelf_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::RoboCompCommonBehavior::ParameterList
IceDelegateM::RoboCompCommonBehavior::CommonBehavior::getParameterList(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompCommonBehavior__CommonBehavior__getParameterList_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    ::RoboCompCommonBehavior::ParameterList __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::RoboCompCommonBehavior::__readParameterList(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::RoboCompCommonBehavior::CommonBehavior::setParameterList(const ::RoboCompCommonBehavior::ParameterList& l, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompCommonBehavior__CommonBehavior__setParameterList_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::RoboCompCommonBehavior::__writeParameterList(__os, l);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

void
IceDelegateM::RoboCompCommonBehavior::CommonBehavior::reloadConfig(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompCommonBehavior__CommonBehavior__reloadConfig_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    if(!__og.is()->b.empty())
    {
        try
        {
            if(!__ok)
            {
                try
                {
                    __og.throwUserException();
                }
                catch(const ::Ice::UserException& __ex)
                {
                    ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                    throw __uue;
                }
            }
            __og.is()->skipEmptyEncaps();
        }
        catch(const ::Ice::LocalException& __ex)
        {
            throw ::IceInternal::LocalExceptionWrapper(__ex, false);
        }
    }
}

::RoboCompCommonBehavior::State
IceDelegateM::RoboCompCommonBehavior::CommonBehavior::getState(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __RoboCompCommonBehavior__CommonBehavior__getState_name, ::Ice::Normal, __context);
    bool __ok = __og.invoke();
    ::RoboCompCommonBehavior::State __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::RoboCompCommonBehavior::__read(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Int
IceDelegateD::RoboCompCommonBehavior::CommonBehavior::getPeriod(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompCommonBehavior::CommonBehavior* servant = dynamic_cast< ::RoboCompCommonBehavior::CommonBehavior*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getPeriod(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompCommonBehavior__CommonBehavior__getPeriod_name, ::Ice::Idempotent, __context);
    ::Ice::Int __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RoboCompCommonBehavior::CommonBehavior::setPeriod(::Ice::Int period, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int period, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_period(period)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompCommonBehavior::CommonBehavior* servant = dynamic_cast< ::RoboCompCommonBehavior::CommonBehavior*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->setPeriod(_m_period, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int _m_period;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompCommonBehavior__CommonBehavior__setPeriod_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(period, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::Ice::Int
IceDelegateD::RoboCompCommonBehavior::CommonBehavior::timeAwake(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Int& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompCommonBehavior::CommonBehavior* servant = dynamic_cast< ::RoboCompCommonBehavior::CommonBehavior*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->timeAwake(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Int& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompCommonBehavior__CommonBehavior__timeAwake_name, ::Ice::Idempotent, __context);
    ::Ice::Int __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RoboCompCommonBehavior::CommonBehavior::killYourSelf(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompCommonBehavior::CommonBehavior* servant = dynamic_cast< ::RoboCompCommonBehavior::CommonBehavior*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->killYourSelf(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompCommonBehavior__CommonBehavior__killYourSelf_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(__current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::RoboCompCommonBehavior::ParameterList
IceDelegateD::RoboCompCommonBehavior::CommonBehavior::getParameterList(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RoboCompCommonBehavior::ParameterList& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompCommonBehavior::CommonBehavior* servant = dynamic_cast< ::RoboCompCommonBehavior::CommonBehavior*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getParameterList(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::RoboCompCommonBehavior::ParameterList& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompCommonBehavior__CommonBehavior__getParameterList_name, ::Ice::Normal, __context);
    ::RoboCompCommonBehavior::ParameterList __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

void
IceDelegateD::RoboCompCommonBehavior::CommonBehavior::setParameterList(const ::RoboCompCommonBehavior::ParameterList& l, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::RoboCompCommonBehavior::ParameterList& l, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _m_l(l)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompCommonBehavior::CommonBehavior* servant = dynamic_cast< ::RoboCompCommonBehavior::CommonBehavior*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->setParameterList(_m_l, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        const ::RoboCompCommonBehavior::ParameterList& _m_l;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompCommonBehavior__CommonBehavior__setParameterList_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(l, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

void
IceDelegateD::RoboCompCommonBehavior::CommonBehavior::reloadConfig(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompCommonBehavior::CommonBehavior* servant = dynamic_cast< ::RoboCompCommonBehavior::CommonBehavior*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            servant->reloadConfig(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompCommonBehavior__CommonBehavior__reloadConfig_name, ::Ice::Normal, __context);
    try
    {
        _DirectI __direct(__current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
}

::RoboCompCommonBehavior::State
IceDelegateD::RoboCompCommonBehavior::CommonBehavior::getState(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::RoboCompCommonBehavior::State& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::RoboCompCommonBehavior::CommonBehavior* servant = dynamic_cast< ::RoboCompCommonBehavior::CommonBehavior*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->getState(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::RoboCompCommonBehavior::State& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __RoboCompCommonBehavior__CommonBehavior__getState_name, ::Ice::Normal, __context);
    ::RoboCompCommonBehavior::State __result;
    try
    {
        _DirectI __direct(__result, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::ObjectPtr
RoboCompCommonBehavior::CommonBehavior::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __RoboCompCommonBehavior__CommonBehavior_ids[2] =
{
    "::Ice::Object",
    "::RoboCompCommonBehavior::CommonBehavior"
};

bool
RoboCompCommonBehavior::CommonBehavior::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__RoboCompCommonBehavior__CommonBehavior_ids, __RoboCompCommonBehavior__CommonBehavior_ids + 2, _s);
}

::std::vector< ::std::string>
RoboCompCommonBehavior::CommonBehavior::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__RoboCompCommonBehavior__CommonBehavior_ids[0], &__RoboCompCommonBehavior__CommonBehavior_ids[2]);
}

const ::std::string&
RoboCompCommonBehavior::CommonBehavior::ice_id(const ::Ice::Current&) const
{
    return __RoboCompCommonBehavior__CommonBehavior_ids[1];
}

const ::std::string&
RoboCompCommonBehavior::CommonBehavior::ice_staticId()
{
    return __RoboCompCommonBehavior__CommonBehavior_ids[1];
}

::Ice::DispatchStatus
RoboCompCommonBehavior::CommonBehavior::___getPeriod(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = getPeriod(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompCommonBehavior::CommonBehavior::___setPeriod(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::Ice::Int period;
    __is->read(period);
    __is->endReadEncaps();
    setPeriod(period, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompCommonBehavior::CommonBehavior::___timeAwake(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Int __ret = timeAwake(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompCommonBehavior::CommonBehavior::___killYourSelf(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    killYourSelf(__current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompCommonBehavior::CommonBehavior::___getParameterList(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::RoboCompCommonBehavior::ParameterList __ret = getParameterList(__current);
    ::RoboCompCommonBehavior::__writeParameterList(__os, __ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompCommonBehavior::CommonBehavior::___setParameterList(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::RoboCompCommonBehavior::ParameterList l;
    ::RoboCompCommonBehavior::__readParameterList(__is, l);
    __is->endReadEncaps();
    setParameterList(l, __current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompCommonBehavior::CommonBehavior::___reloadConfig(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    reloadConfig(__current);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
RoboCompCommonBehavior::CommonBehavior::___getState(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::RoboCompCommonBehavior::State __ret = getState(__current);
    ::RoboCompCommonBehavior::__write(__os, __ret);
    return ::Ice::DispatchOK;
}

static ::std::string __RoboCompCommonBehavior__CommonBehavior_all[] =
{
    "getParameterList",
    "getPeriod",
    "getState",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "killYourSelf",
    "reloadConfig",
    "setParameterList",
    "setPeriod",
    "timeAwake"
};

::Ice::DispatchStatus
RoboCompCommonBehavior::CommonBehavior::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__RoboCompCommonBehavior__CommonBehavior_all, __RoboCompCommonBehavior__CommonBehavior_all + 12, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __RoboCompCommonBehavior__CommonBehavior_all)
    {
        case 0:
        {
            return ___getParameterList(in, current);
        }
        case 1:
        {
            return ___getPeriod(in, current);
        }
        case 2:
        {
            return ___getState(in, current);
        }
        case 3:
        {
            return ___ice_id(in, current);
        }
        case 4:
        {
            return ___ice_ids(in, current);
        }
        case 5:
        {
            return ___ice_isA(in, current);
        }
        case 6:
        {
            return ___ice_ping(in, current);
        }
        case 7:
        {
            return ___killYourSelf(in, current);
        }
        case 8:
        {
            return ___reloadConfig(in, current);
        }
        case 9:
        {
            return ___setParameterList(in, current);
        }
        case 10:
        {
            return ___setPeriod(in, current);
        }
        case 11:
        {
            return ___timeAwake(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
RoboCompCommonBehavior::CommonBehavior::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
RoboCompCommonBehavior::CommonBehavior::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
RoboCompCommonBehavior::CommonBehavior::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type RoboCompCommonBehavior::CommonBehavior was not generated with stream support";
    throw ex;
}

void
RoboCompCommonBehavior::CommonBehavior::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type RoboCompCommonBehavior::CommonBehavior was not generated with stream support";
    throw ex;
}

void 
RoboCompCommonBehavior::__patch__CommonBehaviorPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::RoboCompCommonBehavior::CommonBehaviorPtr* p = static_cast< ::RoboCompCommonBehavior::CommonBehaviorPtr*>(__addr);
    assert(p);
    *p = ::RoboCompCommonBehavior::CommonBehaviorPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::RoboCompCommonBehavior::CommonBehavior::ice_staticId(), v->ice_id());
    }
}

bool
RoboCompCommonBehavior::operator==(const ::RoboCompCommonBehavior::CommonBehavior& l, const ::RoboCompCommonBehavior::CommonBehavior& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
RoboCompCommonBehavior::operator<(const ::RoboCompCommonBehavior::CommonBehavior& l, const ::RoboCompCommonBehavior::CommonBehavior& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
