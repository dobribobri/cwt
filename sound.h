
#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <math.h>
#include "gnuplot_i.hpp"
#include "defines.h"

namespace little_endian_io {
    template <typename Word>
    std::ostream& write_word(std::ostream& outs, Word value, unsigned size = sizeof(Word)) {
        for (; size; --size, value >>= 8) outs.put(static_cast<char>(value & 0xFF));
        return outs;
    }
}
using namespace little_endian_io;

class Sound {
    private:
        std::vector<double> F;
        std::vector<int> time;

        std::string dtos(double i);
    public:
        Sound(std::string filename);
        Sound(std::vector<double> _F, std::vector<int> _time) : F(_F), time(_time) { };
        Sound(double frequency, double amplitude, float duration);
        void draw();
        std::vector<double> getF();
        Sound* makeInterval(float _FROM__IN_SECONDS, float _TO__IN_SECONDS);
        float duration();
        void writeToFile(std::string filename, std::string extention);
        void makeWavFile(std::string filename);
        ~Sound();
};

#endif
