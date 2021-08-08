#ifndef SHAREDPTR_H
#define SHAREDPTR_H

// for size_t
#include <stddef.h>

template <class Type>
class SharedPtr {

public:
    SharedPtr(Type *inp);
    SharedPtr(const SharedPtr& rhs);

    SharedPtr& operator=(const SharedPtr& rhs);
    ~SharedPtr();

    operator bool() const;
    Type& operator*() const;
    Type* operator->() const;


private:
    Type *ptr;
    size_t* refPtr;
};



template <class Type>
SharedPtr<Type>::SharedPtr(Type* inp)
    /*
     * Since this is the first time the object is being
     * constructed, we know that it must be its own object
     * so we can set the number pointed to by refPtr to be 1.
     */
    : ptr(inp), refPtr(new size_t(1))


{}


template <class Type>
SharedPtr<Type>::SharedPtr(const SharedPtr& rhs)
    : ptr(rhs.ptr), refPtr(rhs.refPtr)
{
    /*
     * We need to increment refPtr since we now have
     * an extra pointer pointing to the object, namely
     * this one.
     */
    ++*refPtr;
}

template <class Type>
SharedPtr<Type>& SharedPtr<Type>::operator=(const SharedPtr& rhs) {
    /*
     * We increment rhs.refPtr first to deal with the case
     * where we might be self assigning. If this is the case,
     * we won't ever end up entering the deletion if clause.
     */
    ++*rhs.refPtr;

    /*
     * If this instance of SharedPtr is the last one pointing
     * to the object, release the memory associated with that
     * object. This is because we are about to reassign this
     * instance to point somewhere else and so we will leak
     * memory if, being the last one,it doesn't release the memory.
     *
     * You can kind of think of it as the idea that the last
     * person leaving the room needs to close the lights.
     */
    if(--*refPtr == 0) {
        delete refPtr;
        delete ptr;
    }

    /*
     * Reassing the refPtr and ptr to point to the same things
     * that rhs.refPtr and rhs.ptr are pointing to.
     */
    refPtr = rhs.refPtr;
    ptr = rhs.ptr;
    return *this;
}

template <class Type>
SharedPtr<Type>::~SharedPtr() {
    /*
     * Same idea as above; if this is the last pointer to the
     * object, it needs to clear up the object as it gets
     * destructed because if it doesn't, no one will be able to.
     */
    if(--*refPtr == 0) {
        delete refPtr;
        delete ptr;
    }
}

template <class Type>
SharedPtr<Type>::operator bool() const {
    return ptr;
}

template <class Type>
Type& SharedPtr<Type>::operator*() const {
    if(ptr) {
        return *ptr;
    }
    throw std::runtime_error("Attempting to dereference unbound SharedPtr");
}

template <class Type>
Type* SharedPtr<Type>::operator->() const {
    if(ptr) {
        return ptr;
    }
    throw std::runtime_error("Attempting to dereference unbound SharedPtr");
}

#endif // SHAREDPTR_H

