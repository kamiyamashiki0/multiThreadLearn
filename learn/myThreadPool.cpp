#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>

// void programB_FunB1(void (*callback)())函数指针作为参数，注意声明出参数列和返回值
// {
//     callback();
// }
template<typename T,typename... Args>
class myThreadPool
{
public:
//构造函数，创建线程
    myThreadPool(int);
    // 析构函数
    ~myThreadPool();
    // 向任务队列添加任务的函数
    void addTask(void(*)(Args));

private:
//工作队列
    std::vector<std::thread> workers_v;
    // 任务队列
    std::queue<void(*)(Args...)> task_queue;//auto只能用作类型推导，不能用作模板参数类型的一部分，模板参数中必须显示指定类型
    // 线程池关闭标志
    bool stop;
    // 任务函数
    //static void run(void(*)());这里企图将work函数作为回调函数，但work函数是非静态成员函数，传参比较复杂，而且它还是私有的
    static void run(void *);
    void work();
    //锁
    std::mutex mux;
    //条件变量
    std::condition_variable cv;

};
//在创建线程的时候要传一个函数作为参数，若这个函数是普通成员函数，编译器会自动加上一个this参数，导致类型不匹配无法创建线程，所以只能使用静态成员函数，但是静态成员函数使用不了锁等非静态成员，所以可以给这个静态成员函数传递一个对象指针，通过这个指针调用非静态成员函数，或者直接传递这个非静态成员函数的指针
template<typename... Args>
class A
{
    public:
        std::queue<auto (*)(Args...)> q;
};
