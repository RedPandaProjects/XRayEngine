#pragma once
template<typename T, template<class Tt> class allocator >
class XrFastForwardList
{
public:
    struct TNode
    {
        T val;
        TNode* right;
    };
public:
    XrFastForwardList() :m_first(0), m_back(0) { m_size = 0; }
    inline ~XrFastForwardList() { clear(); }
    inline size_t size()const { return m_size; }
    inline T& push_back()
    {
        m_size++;
        if (m_first == nullptr)
        {
            m_first = m_allocator.allocate(1, 0);
            m_allocator.construct(m_first, TNode());
            m_first->right = nullptr;
            m_back = m_first;
            return m_first->val;
        }
        TNode* n = m_allocator.allocate(1, 0);
        m_allocator.construct(n, TNode());
        m_back->right = n;
        m_back = n;
        n->right = nullptr;
        return  n->val;
    }
    inline void push_back(const T& t)
    {
        m_size++;
        if (m_first == nullptr)
        {
            m_first = m_allocator.allocate(1, 0);
            m_allocator.construct(m_first, TNode());
            m_first->val = t;
            m_first->right = nullptr;
            m_back = m_first;
            return;
        }
        TNode* n = m_allocator.allocate(1, 0);
        m_allocator.construct(n, TNode());
        m_back->right = n;
        n->val = t;
        m_back = n;
        n->right = nullptr;
    }
    inline   void push_back(T&& t)
    {
        m_size++;
        if (m_first == nullptr)
        {
            m_first = m_allocator.allocate(1, 0);
            m_allocator.construct(m_first, TNode());
            m_first->val = t;
            m_back = m_first;
            return;
        }
        TNode*n = m_allocator.allocate(1, 0);
        m_allocator.construct(n, TNode());
        m_back->right = n;
        n->val = t;
        m_back = n;
    }
    inline void  setup(std::function<void(T*)>&& function)
    {
        TNode* n = m_first;
        while (n)
        {
            function(&n->val);
            n = n->right;
        }
    }
    inline void  setup(std::function<void(T*)>& function)
    {
        TNode* n = m_first;
        while (n)
        {
            function(&n->val);
            n = n->right;
        }
    }
    inline void clear()
    {
        while (m_first)
        {
            TNode* n = m_first;
            m_first = m_first->right;
            m_allocator.destroy(n);
            m_allocator.deallocate(n,1);
        }
        m_back = 0;
        m_size = 0;
    }

private:
    size_t m_size;
    allocator<TNode> m_allocator;
    TNode* m_first;
    TNode* m_back;
};