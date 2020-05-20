#pragma once
template<class K, class T, template<class Tt> class allocator  >
class XrFastMap
{
public:
    struct TNode
    {
        K key;
        T val;
        TNode* left, *right;
    };
    typedef std::function<void(T*)> callback;

private:
    size_t m_size;
    TNode* m_first_node;
    IC void recurseLR(TNode* N, callback &CB)
    {
        if (N->left) recurseLR(N->left, CB);
        CB(&N->val);
        if (N->right) recurseLR(N->right, CB);
    }
    IC void recurseRL(TNode* N, callback &CB)
    {
        if (N->right) recurseRL(N->right, CB);
        CB(&N->val);
        if (N->left) recurseRL(N->left, CB);
    }
    IC void clear(TNode* N)
    {
        if (N == nullptr)return;
        clear(N->left);
        clear(N->right);
        m_allocator.destroy(N);
        m_allocator.deallocate(N, 1);
    }
    IC void setup(TNode* N,callback& CB)
    {
        if (N == nullptr)return;
        CB(&N->val);
        setup(N->left, CB);
        setup(N->right, CB);

    }
   /* IC void getLR(TNode* N, xr_vector<T, typename allocator::template helper<T>::result>& D)
    {
        if (N->left) getLR(N->left, D);
        D.push_back(N->val);
        if (N->right) getLR(N->right, D);
    }
    IC void getRL(TNode* N, xr_vector<T, typename allocator::template helper<T>::result>& D)
    {
        if (N->right) getRL(N->right, D);
        D.push_back(N->val);
        if (N->left) getRL(N->left, D);
    }
    IC void getLR_P(TNode* N, xr_vector<TNode*, typename allocator::template helper<TNode*>::result>& D)
    {
        if (N->left) getLR_P(N->left, D);
        D.push_back(N);
        if (N->right) getLR_P(N->right, D);
    }
    IC void getRL_P(TNode* N, xr_vector<TNode*, typename allocator::template helper<TNode*>::result>& D)
    {
        if (N->right) getRL_P(N->right, D);
        D.push_back(N);
        if (N->left) getRL_P(N->left, D);
    }*/
public:
    XrFastMap()
    {
        m_size = 0;
        m_first_node = nullptr;
    }
    IC ~XrFastMap()
    {
        clear();
    }
    IC size_t size() const { return m_size; }
    IC TNode* insert(const K& k)
    {
        if (m_first_node)
        {
            TNode* node = m_first_node;

        once_more:
            if (k < node->key)
            {
                if (node->left)
                {
                    node = node->left;
                    goto once_more;
                }
                else
                {
                    m_size++;
                    TNode* N = m_allocator.allocate(1, 0);
                    m_allocator.construct(N, TNode());
                    N->key = k;
                    N->right = 0;
                    N->left = 0;
                    node->left = N;
                    return N;
                }
            }
            else if (k > node->key)
            {
                if (node->right)
                {
                    node = node->right;
                    goto once_more;
                }
                else
                {
                    m_size++;
                    TNode* N = m_allocator.allocate(1, 0);
                    m_allocator.construct(N, TNode());
                    N->key = k;
                    N->right = 0;
                    N->left = 0;
                    node->right = N;
                    return N;
                }
            }
            else return node;

        }
        else
        {
            m_size++;
            m_first_node = m_allocator.allocate(1, 0);
            m_allocator.construct(m_first_node, TNode());
            m_first_node->right = 0;
            m_first_node->left = 0;
            m_first_node->key = k;
            return m_first_node;
        }
    }
    IC TNode* insertInAnyWay(const K& k)
    {
        if (m_first_node)
        {
            TNode* node = m_first_node;

        once_more:
            if (k <= node->key)
            {
                if (node->left)
                {
                    node = node->left;
                    goto once_more;
                }
                else
                {
                    m_size++;
                    TNode* N = m_allocator.allocate(1, 0);
                    m_allocator.construct(N, TNode());
                    N->key = k;
                    N->right = 0;
                    N->left = 0;
                    node->left = N;
                    return N;
                }
            }
            else
            {
                if (node->right)
                {
                    node = node->right;
                    goto once_more;
                }
                else
                {
                    m_size++;
                    TNode* N = m_allocator.allocate(1, 0);
                    m_allocator.construct(N, TNode());
                    N->key = k;
                    N->right = 0;
                    N->left = 0;
                    node->right = N;
                    return N;
                }
            }
        }
        else
        {
            m_size++;
            m_first_node = m_allocator.allocate(1, 0);
            m_first_node->right = 0;
            m_first_node->left = 0;
            m_first_node->key = k;
            return m_first_node;
        }
    }
    IC TNode* insert(const K& k, const T& v)
    {
        TNode* N = insert(k);
        N->val = v;
        return N;
    }
    IC TNode* insertInAnyWay(const K& k, const T& v)
    {
        TNode* N = insertInAnyWay(k);
        N->val = v;
        return N;
    }

    IC void traverseLR(callback &CB)
    {
        if (m_first_node) recurseLR(m_first_node, CB);
    }
    IC void traverseLR(callback&& CB)
    {
        if (m_first_node) recurseLR(m_first_node, CB);
    }
    IC void traverseRL(callback& CB)
    {
        if (m_first_node) recurseRL(m_first_node, CB);
    }
    IC void traverseRL(callback&& CB)
    {
        if (m_first_node) recurseRL(m_first_node, CB);
    }
    IC void setup(callback&& CB)
    {
        setup( m_first_node,CB);
    }
    IC void setup(callback& CB)
    {
        setup(m_first_node, CB);
    }
    IC void clear()
    {
        m_size = 0;
        clear(m_first_node);
        m_first_node = nullptr;
    }

    /*IC void getLR(xr_vector<T, typename allocator::template helper<T>::result>& D)
    {
        if (pool) getLR(nodes, D);
    }
    IC void getLR_P(xr_vector<TNode*, typename allocator::template helper<TNode*>::result>& D)
    {
        if (pool) getLR_P(nodes, D);
    }
    IC void getRL(xr_vector<T, typename allocator::template helper<T>::result>& D)
    {
        if (pool) getRL(nodes, D);
    }
    IC void getRL_P(xr_vector<TNode*, typename allocator::template helper<TNode*>::result>& D)
    {
        if (pool) getRL_P(nodes, D);
    }
    IC void getANY(xr_vector<T, typename allocator::template helper<T>::result>& D)
    {
        TNode* _end = end();
        for (TNode* cur = begin(); cur != _end; cur++) D.push_back(cur->val);
    }
    IC void getANY_P(xr_vector<TNode*, typename allocator::template helper<TNode*>::result>& D)
    {
        D.resize(size());
        TNode** _it = &*D.begin();
        TNode* _end = end();
        for (TNode* cur = begin(); cur != _end; cur++, _it++) *_it = cur;
    }
    IC void getANY_P(xr_vector<void*, typename allocator::template helper<void*>::result>& D)
    {
        D.resize(size());
        void** _it = &*D.begin();
        TNode* _end = end();
        for (TNode* cur = begin(); cur != _end; cur++, _it++) *_it = cur;
    }
   */
private:

    allocator<TNode> m_allocator;
};