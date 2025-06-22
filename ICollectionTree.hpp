#ifndef ICOLLECTIONTREE_HPP
#define ICOLLECTIONTREE_HPP


template <typename T>
class ICollectionTree {
    public:
        virtual ~ICollectionTree() = default;
        virtual size_t GetSize() = 0;
        virtual void Add(T value) = 0;
        virtual void Remove(T value) = 0;
        virtual bool FindElement(T value) = 0;
};

#endif // ICOLLECTIONTREE_HPP
