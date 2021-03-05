#include <iostream>
#include <unistd.h>
#include <thread>
#include <future>
#include <aubo_driver/aubo_driver.h>

using namespace std;
using namespace arcs::aubo_driver;

void getLineNumber(AuboDriver *m_instance)
{
    while (true) {
        m_instance->getLineNumber();
        m_instance->getLabelString();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main()
{
    auto aubo_driver = createAuboDriver(); //不传参数ip默认为127.0.0.1,如果需要改变需传参
    aubo_driver->connectToServer();
    if (aubo_driver->login("usr", "111", 50)) {
        aubo_driver->downloadScript("script1.py");

        std::thread m_thread =
            std::thread(std::bind(getLineNumber, aubo_driver));

        //缺少编译脚本sdk

        aubo_driver->setProgramRun();

        usleep(1000 * 1000 * 10); // 10s

        aubo_driver->setProgramPause();

        usleep(1000 * 1000 * 10); // 10s

        aubo_driver->setProgramResume();
    } else {
        cout << "login failed" << endl;
    }

    return 0;
}
