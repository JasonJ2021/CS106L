#ifndef UNIQUEPTR_H
#define UNIQUEPTR_H

// for size_t
#include <stddef.h>

template <class Type>
class UniquePtr {

public:
    UniquePtr(Type *inp);

    // Delete these two constructors to disallow copying
    UniquePtr(const UniquePtr& rhs) = delete;
    UniquePtr& operator=(const UniquePtr& rhs) = delete;

    ~UniquePtr();

    operator bool() const;
    Type& operator*() const;
    Type* operator->() const;


private:
    Type *ptr;
};



template <class Type>
UniquePtr<Type>::UniquePtr(Type* inp)
    /*
     * Set interal pointer to the passed in pointer
     */
    : ptr(inp)


{}


template <class Type>
UniquePtr<Type>::~UniquePtr() {
    /*
     * When going out of scope, we should delete the internal
     * pointer to releast that resource.
     */
    delete ptr;
}

template <class Type>
UniquePtr<Type>::operator bool() const {
    return ptr;
}

template <class Type>
Type& UniquePtr<Type>::operator*() const {
    if(ptr) {
        return *ptr;
    }
    throw std::runtime_error("Attempting to dereference unbound UniquePtr");
}

template <class Type>
Type* UniquePtr<Type>::operator->() const {
    if(ptr) {
        return ptr;
    }
    throw std::runtime_error("Attempting to dereference unbound UniquePtr");
}

#endif // UNIQUEPTR_H
