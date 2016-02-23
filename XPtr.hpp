/*
 reference counting smartpointer
 ruijunx@microsoft.com
*/
namespace XPtr {
     //���ü���ģ�����L ��ʾ��ʹ�õ�ͬ���������ͣ��������ü�������ʱ��ͬ������
    template<typename L>
    class RefCounter {
    private:
        int counter;
        L _lock;

       //ͬ��������������࣬���캯���м��������������н���
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
    //���߳�ʱ��ͬ������ȫ��Ϊ�գ������߳�ͬ��
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