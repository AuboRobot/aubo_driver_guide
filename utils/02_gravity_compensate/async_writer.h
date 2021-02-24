// https://stackoverflow.com/questions/21126950/asynchronously-writing-to-a-file-in-c-unix
// 基于标准C++11实现的异步文件写入
#ifndef CSTONE_ASYNC_WRITER_H
#define CSTONE_ASYNC_WRITER_H
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <streambuf>
#include <string>
#include <thread>
#include <vector>

namespace arcs {
namespace cstone {
struct AsyncBuf : std::streambuf
{
    std::ofstream out;
    std::mutex mutex;
    std::condition_variable condition;
    std::queue<std::vector<char>> queue;
    std::vector<char> buffer;
    bool done;
    std::thread thread;

    void worker()
    {
        bool local_done(false);
        std::vector<char> buf;
        while (!local_done) {
            {
                std::unique_lock<std::mutex> guard(mutex);
                condition.wait(guard,
                               [this]() { return !queue.empty() || done; });
                if (!queue.empty()) {
                    buf.swap(queue.front());
                    queue.pop();
                }
                local_done = queue.empty() && done;
            }
            if (!buf.empty()) {
                out.write(buf.data(), std::streamsize(buf.size()));
                buf.clear();
            }
        }
        out.flush();
    }

public:
    AsyncBuf(std::string const &name, const bool &app = false)
        : buffer(128), done(false), thread(&AsyncBuf::worker, this)
    {
        if (app) {
            out.open(name, std::ios_base::out | std::ios_base::app);
        } else {
            out.open(name);
        }
        setp(buffer.data(), buffer.data() + buffer.size() - 1);
    }

    ~AsyncBuf()
    {
        {
            std::unique_lock<std::mutex> guard(mutex);
            done = true;
            condition.notify_one();
        }
        thread.join();
    }

    int overflow(int c) override
    {
        if (c != std::char_traits<char>::eof()) {
            *pptr() = std::char_traits<char>::to_char_type(c);
            pbump(1);
        }
        return sync() != -1 ? std::char_traits<char>::not_eof(c)
                            : std::char_traits<char>::eof();
    }

    int sync() override
    {
        if (pbase() != pptr()) {
            buffer.resize(std::size_t(pptr() - pbase()));
            {
                std::unique_lock<std::mutex> guard(mutex);
                queue.push(std::move(buffer));
            }
            condition.notify_one();
            buffer = std::vector<char>(128);
            setp(buffer.data(), buffer.data() + buffer.size() - 1);
        }
        return 0;
    }
};

} // namespace cstone
} // namespace arcs
#endif // CSTONE_ASYNC_WRITER_H
