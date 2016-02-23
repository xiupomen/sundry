/*
 reference counting smartpointer
 ruijunx@microsoft.com
*/
namespace XPtr {
     //引用计数模板对象，L 表示所使用的同步对象类型，用作引用计数增减时做同步操作
    template<typename L>
    class RefCounter {
    private:
        int counter;
        L _lock;

       //同步对象操作辅助类，构造函数中加锁，析构函数中解锁
        class LockHelper {
        private:
            L _l;
        public:
            LockHelper(L &l) {
                _l = l;
                _l.lock();			}
            ~LockHelper() {	_l.unlock(); }
        };

    public:
        RefCounter():counter(0) {	}

        operator int() {
            return counter;
        }
        //pre
        int operator++() {
            LockHelper l(_lock);
            return ++counter;
        }
        //pre
        int operator--() {
            LockHelper l(_lock);
            return --counter;
        }
    };
    //单线程时的同步对象，全部为空，不做线程同步
    struct SingleThread {
        void lock() {};
        void unlock() {};
    };

    template<typename T, typename ThreadLock = SingleThread> class xptr{
    private:
        T * m_ptr;
        RefCounter<ThreadLock> *m_pCounter;

    public:
        xptr():m_ptr(0),m_pCounter(0)	{}

        template<typename P>
        xptr(P *p)	{
            create( new RefCounter<ThreadLock>(), p);
        }

        xptr(const xptr<T> &p) {
            create(p.m_pCounter,p.m_ptr);
        }
        const xptr<T>& operator= (const xptr<T> &p) {
            if(&p != this) {
                destory();
                create(p.m_pCounter, p.m_ptr);
            }
            return *this ;
        }
        ~xptr() {	destory();}
        T* operator->() const{	return m_ptr;}

    private:
        template<typename P>
        void create(RefCounter<ThreadLock> * pf , P* pp) {
            m_pCounter = pf;
            if(m_pCounter!=0)
                ++*m_pCounter;
            m_ptr = pp;
        }
        void destory() {
            if(m_pCounter!=0 && --*m_pCounter==0) {
                delete m_pCounter;
                delete m_ptr;
            }
        }
    };

}