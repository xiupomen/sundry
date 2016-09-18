
////////////////////////////////////////////////
// lambda prototype
// by ruijunx@microsoft.com
///////////////////////////////////////////////

namespace XLambda {
	template <typename T> 
	class CXLambdaLiteral
	{
	public:
		CXLambdaLiteral(const T &t): m_data(t) {}
		template <typename V>
		inline V operator() (const V &x) const {return m_data;}
		template <typename V>
		inline V operator() (const V &x,const V &y) const {return m_data;}
		template <typename V>
		inline V operator() (const V &x,const V &y,const V &z) const {return m_data;}
	private:
		T m_data;
	};

	//template <typename T>
	class CXLambdaIdentity
	{
	public:
		template <typename V>
		inline V operator() (const V &x) const {return x;}
		template <typename V>
		inline V operator() (const V &x,const V &y) const {return x;}
		template <typename V>
		inline V operator() (const V &x,const V &y,const V &z) const {return x;}
	};

	class CXLambdaIdentity_2
	{
	public:
		template <typename V>
		inline V operator() (const V &x,const V &y) const {return y;}
		template <typename V>
		inline V operator() (const V &x,const V &y, const V &z) const {return y;}
	};

	class CXLambdaIdentity_3
	{
	public:
		template <typename V>
		inline V operator() (const V &x,const V &y,const V &z) const {return z;}
	};

	template <class A,class B,class Op>
	class CXLambdaBinOp
	{
	public:
		CXLambdaBinOp(const A &a,const B &b):m_a(a),m_b(b) {}
		template <class V>
		inline V operator() (const V &x) const 
		{return Op::apply(m_a(x),m_b(x));}

		template <class V>
		inline V operator() (const V &x,const V &y) const 
		{return Op::apply(m_a(x,y),m_b(x,y));}

		template <class V>
		inline V operator() (const V &x,const V &y,const V &z) const 
		{return Op::apply(m_a(x,y,z),m_b(x,y,z));}
	private:
		A m_a;
		B m_b;
	};

	//template <class A,class Op>
	//class CXLambdaUnaOp
	//{
	//public:
	//	CXLambdaUnaOp(const A &a):m_a(a) {}
	//	template <class V>
	//	inline V operator() (const V &x) const 
	//	{return Op::apply(m_a(x));}
	//private:
	//	A m_a;
	//};

	template <class T>
	class CXLambda
	{
	public:
		CXLambda(const T &t):m_t(t) {}

		CXLambda() {};
		template<class V>
		inline V operator()(const V &x) const 
		{return m_t(x);}

		template<class V>
		inline V operator()(const V &x, const V &y) const 
		{return m_t(x,y);}

		
		template<class V>
		inline V operator()(const V &x, const V &y, const V &z) const 
		{return m_t(x,y,z);}
	private:
		T m_t;
	};


/*
	/////////////////////////////////////////////
	struct XOpAdd {
		template<typename V>
		inline static V apply(const V&x, const V &y) {
			return x+y;
		}
	};

	template <typename A, typename B>
	CXLambda<CXLambdaBinOp<CXLambda<A>, CXLambda<B>, XOpAdd> > 
		operator +(const CXLambda<A> &a,const CXLambda<B> &b)
	{
		typedef CXLambdaBinOp<CXLambda<A>, CXLambda<B>, XOpAdd>  LamT;
		return CXLambda<LamT>(LamT(a,b));
	}

	template <typename A, typename B>
	CXLambda<CXLambdaBinOp<CXLambda<A>, CXLambda<CXLambdaLiteral<B> >, XOpAdd> > 
		operator +(const CXLambda<A> &a,const B &b)
	{
		typedef CXLambdaBinOp<CXLambda<A>, CXLambda<CXLambdaLiteral<B> >, XOpAdd>  LamT;
		return CXLambda<LamT>(LamT(a,CXLambdaLiteral<B>(b)));
	}
	template <typename A, typename B>
	CXLambda<CXLambdaBinOp<CXLambda<B>, CXLambda<CXLambdaLiteral<A> >, XOpAdd> > 
		operator +(const A &a,const CXLambda<B> &b)
	{
		typedef CXLambdaBinOp<CXLambda<B>, CXLambda<CXLambdaLiteral<A> >, XOpAdd>  LamT;
		return CXLambda<LamT>(LamT(b,CXLambdaLiteral<A>(a)));
	}
*/
	/////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	// macros below are actually defining the templates above
#define OP_BIN_APP(_name,_op) \
	struct XOp##_name {			\
	template<typename V>		\
	inline static V apply(const V &x, const V &y) {	\
	return x _op y;	 }};

#define OP_BIN_APP1(_name,_op, _ret) \
	struct XOp##_name {			\
	template<typename V>		\
	inline static _ret apply(const V &x, const V &y) {	\
	return x _op y;	}};

#define OP_BIN_OPS(_name,_op)	\
	template <typename A, typename B>	\
	CXLambda<CXLambdaBinOp<CXLambda<A>, CXLambda<B>, XOp##_name> > \
	operator _op (const CXLambda<A> &a,const CXLambda<B> &b) {		\
	typedef CXLambdaBinOp<CXLambda<A>, CXLambda<B>, XOp##_name>  LamT; \
	return CXLambda<LamT>(LamT(a,b)); }	\
	template <typename A, typename B>		\
	CXLambda<CXLambdaBinOp<CXLambda<A>, CXLambda<CXLambdaLiteral<B> >, XOp##_name> >	\
	operator _op (const CXLambda<A> &a,const B &b) {			\
	typedef CXLambdaBinOp<CXLambda<A>, CXLambda<CXLambdaLiteral<B> >, XOp##_name>  LamT;	\
	return CXLambda<LamT>(LamT(a,CXLambdaLiteral<B>(b))); }		\
	template <typename A, typename B>	\
	CXLambda<CXLambdaBinOp<CXLambda<B>, CXLambda<CXLambdaLiteral<A> >, XOp##_name> > \
	operator _op (const A &a,const CXLambda<B> &b) {	\
	typedef CXLambdaBinOp<CXLambda<B>, CXLambda<CXLambdaLiteral<A> >, XOp##_name>  LamT;\
	return CXLambda<LamT>(LamT(b,CXLambdaLiteral<A>(a)));}

#define OP_BIN(_name,_op)  \
	OP_BIN_APP(_name,_op)  \
	OP_BIN_OPS(_name,_op)

#define OP_BIN1(_name,_op,_ret)  \
	OP_BIN_APP1(_name,_op,_ret)  \
	OP_BIN_OPS(_name,_op)


	    OP_BIN(Add, +)
		OP_BIN(Sub, -)
		OP_BIN(Multiply, *)
		OP_BIN(Divide, /)
		OP_BIN1(Large, >, bool)
		OP_BIN1(Small, <, bool)
		OP_BIN1(LargeEqual, >=, bool)
		OP_BIN1(SmallEqual, <=, bool)
	/////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////


#define _1 CXLambda<CXLambdaIdentity>()
#define _2 CXLambda<CXLambdaIdentity_2>()
#define _3 CXLambda<CXLambdaIdentity_3>()

}

      //  //////////////////////////////////////////////// 
      // //使用测试 
      //  using namespace XLambda;
	  //  using namespace std;
      //  vector<int> t;
      //  for(int i=0;i<10;i++)
      //       t.push_back(i);  

      // //STL vector中的值就会根据这个计算式改变了！ 
      //  transform(t.begin(),t.end(),t.begin(),1000 + 100 * _1 + _1*10); 
      //  //或 
      //  transform(t.begin(),t.end(),t.begin(), _1 * _1 * _1);   

		    //  ///////////////////////////////////////////////////////////////////////////
      //// test, 一个三元参数的函数对象(一个匿名的functor)的诞生和计算
      //cout << (_1 + _2*_2  + _3*_3*_3+ 1 + _2*10)(1,2,3));
