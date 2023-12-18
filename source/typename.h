#pragma once

#include <array>
#include <cstddef>

namespace impl
{
    template <typename T>
    constexpr const auto &RawTypeName()
    {
        #ifdef _MSC_VER
        return __FUNCSIG__;
        #else
        return __PRETTY_FUNCTION__;
        #endif
    }
    
    struct RawTypeNameFormat
    {
        std::size_t leading_junk = 0, leading_junk_struct = 0, leading_junk_class = 0, trailing_junk = 0;
    };

    // Returns `false` on failure.
    constexpr bool GetRawTypeNameFormat(RawTypeNameFormat *format)
    {
        const auto &str = RawTypeName<int>();
        for (std::size_t i = 0;; i++)
        {
            if (str[i] == 'i' && str[i+1] == 'n' && str[i+2] == 't')
            {
                if (format)
                {
                    format->leading_junk = i;
                    format->leading_junk_struct = i+7;
                    format->leading_junk_class = i+6;
                    format->trailing_junk = sizeof(str)-i-3-1; // `3` is the length of "int", `1` is the space for the null terminator.
                }
                return true;
            }
        }
        return false;
    }
    
    inline static constexpr RawTypeNameFormat format =
    []{
        static_assert(GetRawTypeNameFormat(nullptr), "Unable to figure out how to generate type names on this compiler.");
        RawTypeNameFormat format;
        GetRawTypeNameFormat(&format);
        return format;
    }();
}

#define is_char(idx, char) impl::RawTypeName<T>()[impl::format.leading_junk+idx] == char

// Returns the type name in a `std::array<char, N>` (null-terminated).
template <typename T>
[[nodiscard]] constexpr auto CexprTypeName()
{
    constexpr bool is_struct = is_char(0, 's') && 
						        is_char(1, 't') &&
						        is_char(2, 'r') &&
						        is_char(3, 'u') &&
						        is_char(4, 'c') &&
						        is_char(5, 't');

    constexpr bool is_class = is_char(0, 'c') && 
						        is_char(1, 'l') &&
						        is_char(2, 'a') &&
						        is_char(3, 's') &&
						        is_char(4, 's');

    constexpr std::size_t leading_junk = [&]{
	    if (is_struct)
			return impl::format.leading_junk_struct;
        if (is_class)
            return impl::format.leading_junk_class;
        return impl::format.leading_junk;
    }();

    constexpr std::size_t len = sizeof(impl::RawTypeName<T>()) - leading_junk - impl::format.trailing_junk;
    std::array<char, len> name{};
    for (std::size_t i = 0; i < len-1; i++)
        name[i] = impl::RawTypeName<T>()[i + leading_junk];

    return name;
}

template <typename T>
[[nodiscard]] constexpr const char *TypeName()
{
    static constexpr auto name = CexprTypeName<T>();
    return name.data();
}
template <>
[[nodiscard]] constexpr const char *TypeName<eastl::string8>()
{
    return "eastl::string8";
}
template <>
[[nodiscard]] constexpr const char *TypeName<eastl::string8*>()
{
    return "eastl::string8*";
}
template <>
[[nodiscard]] constexpr const char *TypeName<eastl::string16>()
{
    return "eastl::string16";
}
template <>
[[nodiscard]] constexpr const char *TypeName<eastl::string16*>()
{
    return "eastl::string16*";
}

template <typename T>
[[nodiscard]] constexpr const char *TypeName(const T &)
{
    return TypeName<T>();
}