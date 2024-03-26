//! -*- c++ -*-
#pragma once

//#include <type_traits>  // for enable_if_t, is_convertible, is_assignable

// Taken from the GSL Core Guidelines Support Library
namespace gsl
{

//
// owner
//
// `gsl::owner<T>` is designed as a safety mechanism for code that must deal directly with raw pointers that own memory.
// Ideally such code should be restricted to the implementation of low-level abstractions. `gsl::owner` can also be used
// as a stepping point in converting legacy code to use more modern RAII constructs, such as smart pointers.
//
// T must be a pointer type
// - disallow construction from any type other than pointer type
//
//template <class T, class = std::enable_if_t<std::is_pointer<T>::value>>
template<typename T>
using owner = T;

}// gsl::
