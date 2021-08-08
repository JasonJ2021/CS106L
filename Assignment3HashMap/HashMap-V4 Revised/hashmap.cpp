/*
* Assignment 2: HashMap template implementation (STARTER CODE)
*      TODO: write a comment here.
*/

#include "hashmap.h"

// See milestone 2 about delegating constructors (when HashMap is called in the initalizer list below)
template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap() : HashMap{kDefaultBuckets} { }

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(size_t bucket_count, const H& hash) :
    _size{0},
    _hash_function{hash},
    _buckets_array{bucket_count, nullptr} { }

template <typename K, typename M, typename H>
template <typename InputIt>
HashMap<K,M,H>::HashMap(InputIt first, InputIt last, size_t bucket_count, const H& hash): HashMap(bucket_count,hash){
    auto itr = first;
    while(itr != last){
        insert(*itr);
        ++itr;
    }
}
template <typename K, typename M, typename H>
HashMap<K,M,H>::HashMap(std::initializer_list<value_type> init, size_t bucket_count, const H& hash): HashMap(bucket_count,hash){
    auto itr = init.begin();
    auto end = init.end();
    while(itr !=end){
        insert(*itr);
        ++itr;
    }
}
template <typename K, typename M, typename H>
HashMap<K, M, H>::~HashMap() {
    clear();
}

template <typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::size() const noexcept {
    return _size;
}

template <typename K, typename M, typename H>
inline bool HashMap<K, M, H>::empty() const noexcept {
    return size() == 0;
}

template <typename K, typename M, typename H>
inline float HashMap<K, M, H>::load_factor() const noexcept {
    return static_cast<float>(size())/bucket_count();
};

template <typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::bucket_count() const noexcept {
    return _buckets_array.size();
};

template <typename K, typename M, typename H>
M& HashMap<K, M, H>::at(const K& key) {
    auto [prev, node_found] = find_node(key);
            if (node_found == nullptr) {
        throw std::out_of_range("HashMap<K, M, H>::at: key not found");
    }
    return node_found->value.second;
}

template <typename K, typename M, typename H>
const M& HashMap<K, M, H>::at(const K& key) const {
    // see static_cast/const_cast trick explained in find().
    return static_cast<const M&>(const_cast<HashMap<K, M, H>*>(this)->at(key));
}

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::contains(const K& key) const noexcept {
    return find_node(key).second != nullptr;
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::clear() noexcept {
    for (auto& curr : _buckets_array) {
        while (curr != nullptr) {
            auto trash = curr;
            curr = curr->next;
            delete trash;
        }
    }
    _size = 0;
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::find(const K& key) {
    return make_iterator(find_node(key).second);
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::find(const K& key) const {
    // This is called the static_cast/const_cast trick, which allows us to reuse
    // the non-const version of find to implement the const version.
    // The idea is to cast this so it's pointing to a non-const HashMap, which
    // calls the overload above (and prevent infinite recursion).
    // Also note that we are calling the conversion operator in the iterator class!
    return static_cast<const_iterator>(const_cast<HashMap<K, M, H>*>(this)->find(key));
}

template <typename K, typename M, typename H>
std::pair<typename HashMap<K, M, H>::iterator, bool> HashMap<K, M, H>::insert(const value_type& value) {
    const auto& [key, mapped] = value;
    auto [prev, node_to_edit] = find_node(key);
    size_t index = _hash_function(key) % bucket_count();

    if (node_to_edit != nullptr) {
        return {make_iterator(node_to_edit), false};
    }
    //添加到链表的front
    auto temp = new node(value, _buckets_array[index]);
    _buckets_array[index] = temp;

    ++_size;
    return {make_iterator(temp), true};
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::node_pair HashMap<K, M, H>::find_node(const K& key) const {
    size_t index = _hash_function(key) % bucket_count();
    node* curr = _buckets_array[index];
    node* prev = nullptr; // if first node is the key, return {nullptr, front}
    while (curr != nullptr) {
        const auto& [found_key, found_mapped] = curr->value;
        if (found_key == key) {
            return {prev, curr};
        }
        prev = curr;
        curr = curr->next;
    }
    return {nullptr, nullptr}; // key not found at all.
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::begin() noexcept {
    size_t index = first_not_empty_bucket();
    if (index == bucket_count()) {
        return end();
    }
    return make_iterator(_buckets_array[index]);
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::end() noexcept {
    return make_iterator(nullptr);
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::begin() const noexcept {
    // see static_cast/const_cast trick explained in find().
    return static_cast<const_iterator>(const_cast<HashMap<K, M, H>*>(this)->begin());
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::end() const noexcept {
    // see static_cast/const_cast trick explained in find().
    return static_cast<const_iterator>(const_cast<HashMap<K, M, H>*>(this)->end());
}

template <typename K, typename M, typename H>
size_t HashMap<K, M, H>::first_not_empty_bucket() const noexcept {
    auto isNotNullptr = [ ](const auto& v){
        return v != nullptr;
    };

    auto found = std::find_if(_buckets_array.begin(), _buckets_array.end(), isNotNullptr);
    return found - _buckets_array.begin();
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::make_iterator(node* curr) {
    if (curr == nullptr) {
        return {&_buckets_array, curr, bucket_count()};
    }
    size_t index = _hash_function(curr->value.first) % bucket_count();
    return {&_buckets_array, curr, index};
}

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::erase(const K& key) {
    auto [prev, node_to_erase] = find_node(key);
    if (node_to_erase == nullptr) {
        return false;
    }
    size_t index = _hash_function(key) % bucket_count();
    (prev ? prev->next : _buckets_array[index]) = node_to_erase->next;
    --_size;
    return true;
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::iterator HashMap<K, M, H>::erase(typename HashMap<K, M, H>::const_iterator pos) {
    erase(pos++->first);
    return make_iterator(pos._node); // unfortunately we need a regular iterator, not a const_iterator
}

template <typename K, typename M, typename H>
    void HashMap<K, M, H>::debug() const {
    std::cout << std::setw(30) << std::setfill('-') << '\n' << std::setfill(' ')
          << "Printing debug information for your HashMap implementation\n"
          << "Size: " << size() << std::setw(15) << std::right
          << "Buckets: " << bucket_count() << std::setw(20) << std::right
          << "(load factor: " << std::setprecision(2) << load_factor() << ") \n\n";

    for (size_t i = 0; i < bucket_count(); ++i) {
        std::cout << "[" << std::setw(3) << i << "]:";
        node* curr = _buckets_array[i];
        while (curr != nullptr) {
            const auto& [key, mapped] = curr->value;
            // next line will not compile if << not supported for K or M
            std::cout <<  " -> " << key << ":" << mapped;
            curr = curr->next;
        }
        std::cout <<  " /" <<  '\n';
    }
    std::cout << std::setw(30) << std::setfill('-') << '\n' << std::setfill(' ');
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::rehash(size_t new_bucket_count) {
if (new_bucket_count == 0) {
    throw std::out_of_range("HashMap<K, M, H>::rehash: new_bucket_count must be positive.");
}

std::vector<node*> new_buckets_array(new_bucket_count);
    for (auto& curr : _buckets_array) { // short answer question is asking about this 'curr'
        while (curr != nullptr) {
            const auto& [key, mapped] = curr->value;
            size_t index = _hash_function(key) % new_bucket_count;

            auto temp = curr;
            curr = temp->next;
            temp->next = new_buckets_array[index];
            new_buckets_array[index] = temp;
        }
    }
    _buckets_array = std::move(new_buckets_array);
}

/* begin student code */

// Milestone 2 (optional) - iterator-based constructors
// You will have to type in your own function headers in both the .cpp and .h files.


// Milestone 3 (required) - operator overloading
// The function headers are provided for you.
template <typename K, typename M, typename H>
M& HashMap<K, M, H>::operator[](const K& key) {
    /*
     * The following code is returning a reference to memory that is on the stack.
     * This is a very common mistake, do not make the same mistake!
     * We wanted the starter code to compile without warnings, which is why
     * you see these weird compiler directives.
     * Please remove them before you start writing code.
     */

    // BEGIN STARTER CODE (remove these lines before you begin)
    return (*(insert({{key},{}}).first)).second;
    // END STARTER CODE

    // complete the function implementation (1 line of code)
    // isn't it funny how the bad starter code is longer than the correct answer?
}

template <typename K, typename M, typename H>
bool operator==(const HashMap<K, M, H>& lhs, const HashMap<K, M, H>& rhs) {

    // BEGIN STARTER CODE (remove these lines before you begin)
    bool result = true;
    for(auto itr = lhs.begin();itr!=lhs.end() ; itr++){
        if(!rhs.contains(itr->first) || itr->second != (rhs.find(itr->first)->second))result = false;
    }
    if(rhs.size() != lhs.size()){
        result = false;
    }
    // END STARTER CODE
    return result;
    // complete the function implementation (~4-5 lines of code)
}

template <typename K, typename M, typename H>
bool operator!=(const HashMap<K, M, H>& lhs, const HashMap<K, M, H>& rhs) {

    // BEGIN STARTER CODE (remove these lines before you begin)
    return !(lhs==rhs);
    // END STARTER CODE

    // complete the function implementation (1 line of code)
}

template <typename K, typename M, typename H>
std::ostream& operator<<(std::ostream& os, const HashMap<K, M, H>& rhs) {

    // BEGIN STARTER CODE (remove these lines before you begin)
    //prints either "{Avery:3, Anna:2}
    os<<"{";
    auto itr = rhs.begin();
    if(itr != rhs.end()){
        os<<itr->first<<":"<<itr->second;
        itr++;
    }
    while(itr != rhs.end()){
        os<<", "<<itr->first<<":"<<itr->second;
        itr++;
    }
    os<<"}";
    return os;
    // END STARTER CODE

    // complete the function implementation (~7 lines of code)
}

// Milestone 4 (required) - special member functions
// You will have to type in your own function headers in both the .cpp and .h files.
template <typename K, typename M, typename H>
HashMap<K,M,H>::HashMap(const HashMap &other):HashMap(other.bucket_count(),other._hash_function){
    for(auto itr = other.begin() ; itr != other.end();itr++){
        insert(*itr);
    }
}
template <typename K, typename M, typename H>
HashMap<K,M,H>& HashMap<K,M,H>::operator=(const HashMap &other){
    if(this == &other)return *this;
    clear();
    for(auto itr = other.begin() ; itr != other.end();itr++){
        insert(*itr);
    }
    return *this;
}
//move constructor
template <typename K, typename M, typename H>
HashMap<K,M,H>::HashMap(HashMap &&other):
    _size{std::move(other.size())},
    _hash_function{std::move(other._hash_function)},
    _buckets_array{std::move(other.bucket_count()), nullptr}
{

    for(size_t i = 0 ; i <bucket_count();i++){
        _buckets_array[i] = std::move(other._buckets_array[i]);
        other._buckets_array[i] = nullptr;
    }
    other._size = 0;
}

template <typename K, typename M, typename H>
HashMap<K,M,H>& HashMap<K,M,H>::operator=(HashMap &&other){
    if(&other == this)return *this;
    clear();
    this->_size=std::move(other._size);
    this->_hash_function=std::move(other._hash_function);
    this->_buckets_array.resize(other.bucket_count());
    for(size_t i = 0 ; i <bucket_count();i++){
        _buckets_array[i] = std::move(other._buckets_array[i]);
        other._buckets_array[i] = nullptr;
    }
    other._size = 0;
    return *this;
}
// provide the function headers and implementations (~35 lines of code)

/* end student code */
