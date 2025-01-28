#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>

// void programB_FunB1(void (*callback)())函数指针作为参数，注意声明出参数列和返回值
// {
//     callback();
// }
/////暂时无法完成处理可变参数的任务////////
////////////////////////////////////////
template <typename T> // 后续改进:改为单例、使用function，bind
class myThreadPool                      // 可以通过 std::packaged_task 或 std::future 来支持任务返回值。
{
public:
    // 构造函数，创建线程
    myThreadPool(int);
    // 析构函数
    ~myThreadPool();
    // 向任务队列添加任务的函数
    void addTask(T (*)()); // Args已经被声明为了一个包，那么在使用的时候就要展开，只写Args是没有意义的

private:
    // 工作队列
    std::vector<std::thread> workers_v;
    // 任务队列
    std::queue<T (*)()> task_queue; // auto只能用作类型推导，不能用作模板参数类型的一部分，模板参数中必须显示指定类型
    // 线程池关闭标志
    bool stop;
    // 任务函数
    // static void run(void(*)());这里企图将work函数作为回调函数，但work函数是非静态成员函数，传参比较复杂，而且它还是私有的
    static void run(myThreadPool &);
    void work();
    // 锁
    std::mutex mux;
    // 条件变量
    std::condition_variable cv;
};
// 在创建线程的时候要传一个函数作为参数，若这个函数是普通成员函数，编译器会自动加上一个this参数，导致类型不匹配无法创建线程，所以只能使用静态成员函数，但是静态成员函数使用不了锁等非静态成员，所以可以给这个静态成员函数传递一个对象指针，通过这个指针调用非静态成员函数，或者直接传递这个非静态成员函数的指针
template <typename T>
myThreadPool<T>::myThreadPool(int num) : stop(false), free_worker(num)
{
    if (num <= 0)
    {
        std::cout << "数量无效" << std::endl;
        return;
    }
    for (int i = 0; i < num; i++)
    {
        /////////////////////// workers_v.emplace_back(run, *this);thread默认按值传递(?)
        workers_v.emplace_back(run, std::ref(*this)); // 直接在容器尾部创建，省去了拷贝或移动的开销
    }
}

template <typename T>
void myThreadPool<T>::run(myThreadPool &pool)
{
    pool.work();
}

template <typename T>
void myThreadPool<T>::work()
{
    // 在多线程环境下，stop 的修改与访问可能会引发竞态条件（？？）
    while (!stop)/////////将stop声明为std::atomic<bool>,以保证线程安全
    {
        std::unique_lock<std::mutex> lck(mux);
        //如果任务队列不为空，那么当线程运行到这里时就不会阻塞，它会自行取出任务并执行，因此不用担心条件变量通知时由于没有空闲线程而导致的少量任务堆积
        cv.wait(lck, [this]{ return !task_queue.empty(); });//////////需要捕获this指针访问成员变量
        T(*task)()= task_queue.front();
        task_queue.pop();
        task();
    }
}

template <typename T>
void myThreadPool<T>::addTask(T (*task)())
{
    std::lock_guard<std::mutex> lck(mux);
    task_queue.push(task);
    cv.notify_one(); // 当线程被通知唤醒是，该线程会从条件变量的阻塞队列移动到锁的等待队列，它会先去尝试获得锁，直到它获得了锁，才会去判断pred，如果为true则执行，如果为false则释放锁等待下次通知
    // pred的判断永远是在持有锁的状态下进行的
}

template <typename T>
myThreadPool<T>::~myThreadPool()
{
    stop = true;
    cv.notify_all();//只是通知，但是当任务队列为空时仍然会阻塞
    for (auto &ww : workers_v)
        ww.join();
}
