﻿// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.000000.0
#pragma once
#include "winrt/Component.h"
#include "winrt/Component.Fast.h"
namespace winrt::Component::Fast::implementation
{
    template <typename D, typename... I>
    struct WINRT_EBO SlowClass_base : implements<D, Component::Fast::ISlowClass, Component::Fast::ISlowClass2, Component::INotExclusive, I...>
    {
        using base_type = SlowClass_base;
        using class_type = Component::Fast::SlowClass;
        using implements_type = typename SlowClass_base::implements_type;
        using implements_type::implements_type;
        
        operator impl::producer_ref<class_type> const() const noexcept
        {
            return { to_abi<default_interface<class_type>>(this) };
        }

        hstring GetRuntimeClassName() const
        {
            return L"Component.Fast.SlowClass";
        }
    };
}
namespace winrt::Component::Fast::factory_implementation
{
    template <typename D, typename T, typename... I>
    struct WINRT_EBO SlowClassT : implements<D, Windows::Foundation::IActivationFactory, Component::Fast::ISlowClassStatics, I...>
    {
        using instance_type = Component::Fast::SlowClass;

        hstring GetRuntimeClassName() const
        {
            return L"Component.Fast.SlowClass";
        }
        Windows::Foundation::IInspectable ActivateInstance() const
        {
            return make<T>();
        }
        hstring StaticMethod()
        {
            return T::StaticMethod();
        }
    };
}

#if defined(WINRT_FORCE_INCLUDE_SLOWCLASS_XAML_G_H) || __has_include("Component.Fast.SlowClass.xaml.g.h")
#include "Component.Fast.SlowClass.xaml.g.h"
#else

namespace winrt::Component::Fast::implementation
{
    template <typename D, typename... I>
    using SlowClassT = SlowClass_base<D, I...>;
}

#endif
