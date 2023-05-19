#pragma once

#include <cstdio>
#include <iostream>
#include <sys/time.h>

class Chrono {
  public:
    /**
     * A simple timer class for measuring elapsed time.
     * @param name: a string to identify the timer.
     * @param afficherQuandDetruit: if true, the timer will print its elapsed time when it is
     * destroyed.
     */
    Chrono(std::string name, bool afficherQuandDetruit = true)
        : _name(name), _duree(0), _dureeSec(0), _pause(false),
          _afficherQuandDetruit(afficherQuandDetruit) {
        gettimeofday(&depart, &tz);
    }

    /**
     * A default constructor for the timer.
     */
    Chrono() : _duree(0), _dureeSec(0), _pause(false) { gettimeofday(&depart, &tz); }

    /**
     * A destructor for the timer. If the timer has a name and afficherQuandDetruit is true,
     * the timer will print its elapsed time when it is destroyed.
     */
    ~Chrono() {
        if (_name.size())
            if (_afficherQuandDetruit)
                print();
    }

    /**
     * Set the duration of the timer in seconds and microseconds.
     * @param sec: the number of seconds.
     * @param microSec: the number of microseconds.
     */
    void setDuree(long sec, long microSec = 0) {
        _duree = sec * 1000000L + microSec;
        _dureeSec = sec;
    }

    /**
     * Start the timer and reset the elapsed time.
     */
    void tic() {
        _pause = false;
        _duree = 0;
        _dureeSec = 0;
        gettimeofday(&depart, &tz);
    }

    /**
     * Pause or resume the timer and return the elapsed time in microseconds.
     * @param val: if true, the timer will pause; if false, it will resume.
     * @return: the elapsed time in microseconds.
     */
    long pause(bool val) {
        if (val) {
            if (!_pause) {
                gettimeofday(&fin, &tz);
                _duree += (fin.tv_sec - depart.tv_sec) * 1000000L + (fin.tv_usec - depart.tv_usec);
                _dureeSec += fin.tv_sec - depart.tv_sec;
                _pause = true;
            }
        } else {
            if (_pause) {
                gettimeofday(&depart, &tz);
                _pause = false;
            }
        }
        return _duree;
    }

    /**
     * Pause or resume the timer and return the elapsed time in seconds.
     * @param val: if true, the timer will pause; if false, it will resume.
     * @return: the elapsed time in seconds.
     */
    long pauseSec(bool val) {
        if (val) {
            if (!_pause) {
                gettimeofday(&fin, &tz);
                _duree += (fin.tv_sec - depart.tv_sec) * 1000000L + (fin.tv_usec - depart.tv_usec);
                _dureeSec += fin.tv_sec - depart.tv_sec;
                _pause = true;
            }
        } else {
            if (_pause) {
                gettimeofday(&depart, &tz);
                _pause = false;
            }
        }
        return _duree;
    }

    /**
     * Stop the timer and return the elapsed time in microseconds.
     * @return: the elapsed time in microseconds.
     */
    long tac() {
        if (_pause == false) {
            gettimeofday(&fin, &tz);
            return (fin.tv_sec - depart.tv_sec) * 1000000L + (fin.tv_usec - depart.tv_usec) +
                   _duree;
        } else {
            return _duree;
        }
    }

    /**
     * Stop the timer and return the elapsed time in seconds.
     * @return: the elapsed time in seconds.
     */
    long tacSec() {
        if (_pause == false) {
            gettimeofday(&fin, &tz);
            return (fin.tv_sec - depart.tv_sec) + _dureeSec;
        } else {
            return _dureeSec;
        }
    }

    /**
     * Print the elapsed time of the timer in a human-readable format.
     */
    void print() {
        double val = tac();

        if (_name.size())
            std::cout << _name << ": ";
        if (val < 1000.0)
            std::cout << val << " µs" << std::endl;
        else if (val < 1000000.0)
            std::cout << val / 1000.0 << " ms" << std::endl;
        else
            std::cout << val / 1000000.0 << " sec" << std::endl;
    }

  private:
    std::string _name;
    struct timeval depart, fin;
    struct timezone tz;
    long _duree;
    long _dureeSec;

    bool _pause;
    bool _afficherQuandDetruit;
};
