
#ifndef CWT_H
#define CWT_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <fstream>
#include <math.h>
#include "gnuplot_i.hpp"
#include "defines.h"
#include "sound.h"
#include "wavelet.h"

typedef std::map<std::pair<double, double>, double> Matrix;
typedef std::vector<Matrix> CWTData;

class CWT {
    private:
        Sound* sound;
        Wavelet* wavelet;
        CWTData result;

        std::string dtos(double i);
        void _draw(std::vector<double>& s, std::vector<double>& tau, std::vector<double>& A);
    public:
        int octaves;
        int step_tau;

        CWT(Sound* snd, Wavelet* wvt) : sound(snd), wavelet(wvt) { };
        void setParameters(int _OCTAVES, int _STEP_TAU);

        void make();
        CWTData makeAndGetResult();

        void cwtDataToFiles(std::string filename, std::string extention);
        void drawFileData(std::string filename);
        void draw(int _SEGMENT);
};

#endif
