//
// Created by remy on 9/19/22.
//

#ifndef PILGRIM_GENERAL_TIMER_H
#define PILGRIM_GENERAL_TIMER_H

#include <chrono>
#include <mutex>
#include <vector>

class [[nodiscard]] timerAdvanced2; // needed to add it as friend in timer class

// ============================================================================================= //
//                                       Begin class timer                                       //

/**
 * @brief A helper class to measure execution time for benchmarking purposes.
     * *  The timer can be set back to ZERO thanks to the reboot() function
 */
class [[nodiscard]] timer
{
public:
    friend class timerAdvanced2;
    /**
     * @brief Start (or restart) measuring time.
     */
    void start()
    {
        start_time = std::chrono::steady_clock::now();
    }

    /**
     * @brief Stop measuring time and store the elapsed time since start().
     */
    void stop()
    {
        elapsed_time += std::chrono::steady_clock::now() - start_time;
    }

    /**
     * @brief reboot the timer to zero
     */
    void reboot()
    {
        elapsed_time = std::chrono::duration<double>::zero();
    }

    /**
     * @brief Get the number of milliseconds that have elapsed between start() and stop().
     *
     * @return The number of milliseconds.
     */
    [[nodiscard]] std::chrono::milliseconds::rep ms() const
    {
        return (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time)).count();
    }

private:
    /**
     * @brief The time point when measuring started.
     */
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();

    /**
     * @brief The duration that has elapsed between start() and stop().
     */
    std::chrono::duration<double> elapsed_time = std::chrono::duration<double>::zero();

};

//                                        End class timer                                        //
// ============================================================================================= //

// ============================================================================================= //
//                                   Begin class timer_advanced                                   //

/**
 * @brief A helper class to measure execution time for benchmarking purposes.
     *  Compared to the "basic" timer, the advanced timer can be used INSIDE threads to compute the cumulative time spent in a portion of code
     *
     *  To use it :
     *  - declare the timer advanced as a global variable (outside any function)
     *  - when started, the timer return a token
     *  - feed this token to the stop method
     *  - enjoy the result of your benchmark !
     *
     *  The timer can be set back to ZERO thanks to the reboot() function
 */
class [[nodiscard]] timerAdvanced2
{
public:
    friend timer;
    /**
     * @brief Start measuring time.
         * return a token for the stop() method
     */
    int start()
    {
        this->mutex.lock();
        timer my_timer;
        this->timer_list.emplace_back(my_timer);
        int token = this->list_size;
        this->list_size++;
        this->timer_list.back().start();
        this->mutex.unlock();
        return token;
    }

    /**
     * @brief Stop measuring time and add the measured time to the total.
         * @arg start_token : the token given by the start() method
     */
    void stop(int start_token)
    {
        this->mutex.lock();
        timer_list[start_token].stop();
        this->mutex.unlock();
    }

    /**
     * @brief reboot the timer to zero
     */
    void reboot()
    {
        this->timer_list.clear();
        this->list_size = 0;
    }

    /**
     * @brief Get the number of milliseconds that have elapsed between start() and stop().
     *
     * @return The number of milliseconds.
     */
    [[nodiscard]] int ms()
    {
        std::chrono::duration<double> duration_total = std::chrono::duration<double>::zero();
        for( timer timer : timer_list){
            duration_total += timer.elapsed_time;
        }
        return (std::chrono::duration_cast<std::chrono::milliseconds>(duration_total)).count();
    }

private:
    /**
     * brief: the list of timer
     */
    std::vector<timer> timer_list;

    /**
     * @brief the timer_list size
     */
    size_t list_size = 0;

    /**
     * @brief the mutex of the timer (used to protect the vector)
     */
    std::mutex mutex;

};

//                                   Begin class timer_advance                                   //
// ============================================================================================= //


#endif // PILGRIM_GENERAL_TIMER_H
