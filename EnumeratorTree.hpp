#ifndef ENUMERATORTREE_HPP
#define ENUMERATORTREE_HPP


template <typename T>
class IEnumeratorTree {
    public:
        virtual ~IEnumeratorTree() = default;
        virtual T Current() = 0;
        virtual void Reset() = 0;
        virtual bool MoveNext() = 0;
};

template <typename T>
class IEnumerableTree {
    public:
        virtual ~IEnumerableTree() = default;
        virtual class IEnumeratorTree<T>* GetEnumerator() = 0;
};

#endif // ENUMERATORTREE_HPP
