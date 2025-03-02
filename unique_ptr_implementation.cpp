#include <iostream>
#include <utility>

namespace ns
{
    template<class A>
    class unique_ptr
    {
        private:
            A* ptr;
            unique_ptr(unique_ptr &) = delete;
            void operator = (unique_ptr &) = delete;

        public:
            unique_ptr():ptr{nullptr}
            {}

            unique_ptr(A* a):ptr{a}
            {}

            unique_ptr(unique_ptr &&a): ptr{std::exchange(a.ptr,nullptr)}
            {}

            unique_ptr& operator = (unique_ptr &&a)
            {
                if(&a==this) return *this;
                if(ptr==a.ptr)
                {
                    a.ptr=nullptr;
                    return *this;
                }
                delete ptr;
                ptr=std::exchange(a.ptr,nullptr);
                return *this;
            }

            A* release()
            {
                return std::exchange(ptr,nullptr);
            }

            void reset(A* ptr2=nullptr)
            {
                delete std::exchange(ptr,ptr2);
            }

            void swap(unique_ptr &a)
            {
                std::swap(ptr,a.ptr);
            }

            A* get() const
            {
                return ptr;
            }

            operator bool () const
            {
                return (ptr!=nullptr);
            }

            A& operator * () const
            {
                if(ptr==nullptr){
                    std::cout<<"Accessing null ptr with *\n";
                }
                return *ptr;
            }

            A* operator -> () const
            {
                return ptr;
            }

            bool operator == (const unique_ptr &a) const
            {
                return ptr==a.ptr;
            }

            bool operator == (const A *ptr2) const
            {
                return ptr==ptr2;
            }

            bool operator != (const unique_ptr &a) const
            {
                return ptr!=a.ptr;
            }

            bool operator != (const A *ptr2) const
            {
                return ptr!=ptr2;
            }

            ~unique_ptr()
            {
                delete ptr;
            }

    };

    template<class A,class... Args>
    unique_ptr<A> make_unique(Args&&...args)
    {
        return unique_ptr<A>(new A(std::forward<Args>(args)...));
    }
}

struct A
{
    int a;
    double b;
    A(int p,double q):a{p},b{q}
    {}
};


int main()
{
    ns::unique_ptr<int> p1=ns::make_unique<int>(5);
    ns::unique_ptr<int> p2(new int(10));
    std::cout<<(p1!=nullptr)<<std::endl;
    std::cout<<(*p2==10)<<std::endl;

    p1.reset();
    p2.reset(new int(4));
    std::cout<<(p1==nullptr)<<std::endl;
    std::cout<<(p2==nullptr)<<std::endl;
    if(!p1){
        std::cout<<"empty\n";
    }

    auto p=p2.release();
    std::cout<<(p2==nullptr)<<std::endl;
    std::cout<<*p<<std::endl;

    ns::unique_ptr<A> p3;
    p3=ns::make_unique<A>(5,6.1);
    std::cout<<(p3!=nullptr)<<std::endl;
    std::cout<<p3->b<<" "<<(*p3).a<<std::endl;

    ns::unique_ptr<A> p4;
    p3.swap(p4);
    std::cout<<(p3!=nullptr)<<std::endl;
    std::cout<<p4->b<<" "<<(*p4).a<<std::endl;

    p3=std::move(p4);
    std::cout<<(p3!=nullptr)<<std::endl;
    std::cout<<p3->b<<" "<<(*p3).a<<std::endl;
    
}
