#pragma once

#include <cassert>
#include <cstdint>
#include <functional>


namespace nikki {

// A callback based, templated, PID controller class that has minimal
// dependency.
// Users are expected to instruct the controller on how to get the
// current time by supplying a callback. For example:
//   PidController pid(1, 2, 3, [](){ return get_sys_time(); });
//
// It's up to users to define time epoch as long as number of nanoseconds
// is returned.
// This way this class is easily testable with fake time stamps.
template <typename T>
class PidController {
public:
    // A type representing number of nanoseconds.
    using NanoSecT = int64_t;
    // A user supplied callback to get number of nanoseconds.
    using GetTimeCallback = std::function<NanoSecT()>;

    // Constructor that takes all the coefficients, and a callback for 
    // getting time.
    PidController(T kp, T ki, T kd, GetTimeCallback get_time) :
                  kp_(kp),
                  ki_(ki),
                  kd_(kd),
                  get_time_(get_time) {}

    T update(T input, T target) {
        int64_t cur_time = get_time_();
        assert(cur_time > last_time_);

        T error = target - input;

        // Sum up kp and ki terms.
        error_sum_ += error;
        T output = kp_ * error + ki_ * error_sum_;
        
        // kd only contributes to the output after the first update();
        if (last_time_ != 0) {
            output += kd_ * (error - last_error_) / (cur_time - last_time_);
        }
        
        // update internal states.
        last_time_ = cur_time;
        last_error_ = error;

        return output;
    }

    // Getters
    T ki() const { return ki_; }
    T kp() const { return kp_; }
    T kd() const { return kd_; }
    GetTimeCallback time_callback() const { return get_time_; }

    // Setters
    void set_ki(T ki) { ki_ = ki; }
    void set_kp(T kp) { kp_ = kp; }
    void set_kd(T kd) { kd_ = kd; }
    void set_get_time_callback(GetTimeCallback cb) { get_time_ = cb; }

private:
    T kp_;
    T ki_;
    T kd_;
    GetTimeCallback get_time_;

    int64_t last_time_ = 0;
    T last_error_ = 0;
    T error_sum_ = 0;
};

}

