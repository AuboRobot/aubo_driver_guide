#ifndef FREEDRIVE_H
#define FREEDRIVE_H
#include <vector>
#include <thread>
#include "Drag.h"
#include "Impendance.h"
#include "aubo_driver/aubo_driver.h"

class FreeDrive
{
public:
    FreeDrive();
    ~FreeDrive();
    void connectToRobot();
    void join();
    void setSim();
    void setReal();

private:
    void control();

private:
    arcs::aubo_driver::AuboDriver *real_robot_;
    arcs::aubo_driver::AuboDriver *sim_robot_;
    std::thread *control_thread_;
    bool is_sim_ = true;
    bool enable_ = true;
    Drag   drag_;
    Impendance  imp_;
    Limitdrive Limit_drive_;
};

#endif // FREEDRIVE_H
