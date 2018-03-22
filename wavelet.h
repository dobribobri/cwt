
#ifndef WAVELET_H
#define WAVELET_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include "gnuplot_i.hpp"
#include "defines.h"
#include "sound.h"

class Wavelet {
    private:
        std::vector<double> Psy;
        std::vector<int> time;

        int countSignChanged();
        int indexWhereSignChanges(int _TIMES);
    public:
        Wavelet(std::string filename);
        Wavelet(std::vector<double> _Psy, std::vector<int> _time) : Psy(_Psy), time(_time) { };

        void draw();
        void cut_default();
        void cut(int _NUMBER_OF_PERIODS);
        void cutFromTo(int _FROM, int _TO);
        void cutFrom(int _FROM);
        void shrink(double _S);
        Wavelet* makeNewShrinked(double _S);
        std::vector<double> getPsy();
        int numberOfCounts();
        void makeWavFile(std::string filename, float duration);
        double averageValue();
        double energyValue();
        void smoothEnds();

        ~Wavelet();
};

#endif
