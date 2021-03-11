#include "freedrive.h"
#include <unistd.h>

using namespace arcs::aubo_driver;
std::vector<double> operator+(const std::vector<double> &a,
                              const std::vector<double> &b)
{
    std::vector<double> res(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        res[i] = a[i] + b[i];
    }
    return res;
}

std::vector<double> operator-(const std::vector<double> &a,
                              const std::vector<double> &b)
{
    std::vector<double> res(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        res[i] = a[i] - b[i];
    }
    return res;
}

std::vector<double> operator/(const std::vector<double> &a, const double &b)
{
    std::vector<double> res(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        res[i] = a[i] / b;
    }
    return res;
}

template <size_t N>
class GlideFilter
{
public:
    explicit GlideFilter(const int &dof) : dof_(dof)
    {
        for (size_t i = 0; i < N; i++) {
            records_[i].assign(dof_, 0.);
        }
    }

    std::vector<double> eval(const std::vector<double> &sample)
    {
        records_[index_] = sample;
        if (index_ == (N - 1)) {
            index_ = 0;
        } else {
            index_++;
        }
        if (size_ < N) {
            size_++;
        }

        std::vector<double> total(dof_, 0);

        for (size_t i = 0; i < N; i++) {
            total = total + records_[i];
        }

        if (size_ < N) {
            return total / size_;
        }
        return total / N;
    }

private:
    std::array<std::vector<double>, N> records_;
    size_t index_ = 0;
    size_t size_ = 0;
    int dof_ = -1;
};

FreeDrive::FreeDrive()
{
    real_robot_ = createAuboDriver();
}

FreeDrive::~FreeDrive()
{
    if (control_thread_ && control_thread_->joinable()) {
        enable_ = false;
        control_thread_->join();
        delete control_thread_;
        control_thread_ = nullptr;
    }
    join();
    delete real_robot_;
    real_robot_ = nullptr;
    //    delete sim_robot_;
    //    sim_robot_ = nullptr;
}

void FreeDrive::connectToRobot()
{
    real_robot_->connectToServer(
        "192.168.100.100"); //不传参数ip默认为127.0.0.1,如果需要改变需传参
    if (real_robot_->login("user", "111", 50)) {
        std::cout << "Login to Real robot @ 192.168.100.100 SUCCEED"
                  << std::endl;
        std::vector<std::string> names;
        names.push_back("actual_q");
        names.push_back("actual_TCP_pose");
        names.push_back("actual_TCP_force");
        if (real_robot_->setRtdeOutputRecipe(0, names, 200)) {
            std::cout << "Real Robot setRtdeOutputRecipe SUCCEED" << std::endl;
        }
        real_robot_->startRtdeTransmission(0);
    } else {
        std::cout << "Login to Real robot @ 192.168.100.100 FAILED"
                  << std::endl;
    }
    real_robot_->syncRtdeTransmission(0, 50);
    // real_robot_->setSim();

    usleep(5000);
    drag_.pose = real_robot_->getTcpPose();
    Limit_drive_.setinitpose(drag_.pose);
    control_thread_ = new std::thread(&FreeDrive::control, this);
}

void FreeDrive::join()
{
    // sim_robot_->waitForTerminate();
    real_robot_->waitForTerminate();
}

void FreeDrive::setSim()
{
    is_sim_ = true;
}

void FreeDrive::setReal()
{
    is_sim_ = false;
}

std::ostream &operator<<(std::ostream &out, const std::vector<double> &a)
{
    // using std::operator<<;
    int i = 0;
    for (auto &&it : a) {
        out << it;
        if (i++ != (int)(a.size() - 1)) {
            out << ", ";
        }
    }
    return out;
}

void FreeDrive::control()
{
    double dt = 4.0 / 800; // 5ms
    std::vector<double> senddata;
    std::vector<double> changepose;
    std::vector<double> limitpose;
    limitpose.resize(6);
    Grav Cs;
    Gravity CGra;
    Cs = drag_.read_fileide();
    std::vector<double> GraResult;
    // imp_.setStiffness(10);
    AuboDriver *aubo_driver = real_robot_;
    GlideFilter<16> filter(6);
    while (enable_) {
        auto t_start = std::chrono::high_resolution_clock::now();

        drag_.pose = real_robot_->getTcpPose();
        drag_.FT = filter.eval(real_robot_->getTcpForce());
        // drag_.Judg_Filter(real_robot_->getTcpForce(), drag_.FT);

        std::cout << "Force " << drag_.FT << std::endl;
        GraResult.clear();
        CGra.Gravity_compensation_Least(Cs, drag_.pose, drag_.FT, GraResult);
        std::cout << "GraResult " << GraResult << std::endl;

        changepose.clear();
        imp_.Impen_Inter(GraResult, drag_.ThresData, drag_.SensitiData,
                         drag_.FdDrag, changepose);
        std::cout << "changepose " << changepose << std::endl;
        limitpose.clear();
        Limit_drive_.rectangle(drag_.pose, changepose, limitpose);
        senddata.clear();
        drag_.printSend(drag_.pose, limitpose, senddata);

        RtdeInputBuilderPtr builder = aubo_driver->getRtdeInputBuilder();
        builder->servoCartesian(senddata, 1);
        std::cout << "senddata: " << senddata << std::endl;
        builder->send();

        auto t_stop = std::chrono::high_resolution_clock::now();
        auto t_duration = std::chrono::duration<double>(t_stop - t_start);
        // printf("%f  ",t_duration);
        if (t_duration.count() < dt) {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(dt - t_duration.count()));
        }
    }
}
