
#include "wavelet.h"

/*WAVELET*/

Wavelet::Wavelet(std::string filename) {
    std::ifstream fin(filename);
    std::string buff;
    int numb = 0;
    while (getline(fin, buff)) {
        this->Psy.push_back(std::stod(buff));
        this->time.push_back(numb);
        numb++;
    }
}

void Wavelet::draw() {
    Gnuplot gp1;
    gp1.set_title("wavelet.draw()");
    gp1.reset_plot();
    gp1.set_grid();
    gp1.set_xlabel("t");
    gp1.set_ylabel("A");
    gp1.set_style("lines").plot_xy(this->time, this->Psy, "PSY");
    std::cin.get();
}

void Wavelet::cut_default() {
    int _from = (int)(_S_FREQ * 0.1);
    int _to = (int)(_S_FREQ * 0.1 + _S_FREQ * _SUP_MS);
    this->cutFromTo(_from, _to);
}

int Wavelet::countSignChanged() {
    int count = 0;
    for (unsigned int i = 1; i < Psy.size(); i++) {
        if (Psy[i]*Psy[i-1] < 0) count++;
    }
    return count;
}

int Wavelet::indexWhereSignChanges(int _TIMES) {
    int count = 0;
    unsigned int i;
    for (i = 1; i < Psy.size(); i++) {
        if (Psy[i]*Psy[i-1] < 0) count++;
        if (count - 1 == _TIMES) break;
    }
    return i;
}

void Wavelet::cut(int _NUMBER_OF_PERIODS) {
    int index = this->indexWhereSignChanges(_NUMBER_OF_PERIODS);
    this->cutFromTo(0, index);
}

void Wavelet::cutFromTo(int _FROM, int _TO) {
    std::vector<double> new_psy;
    std::vector<int> new_time;
    for (int i = _FROM; i < _TO; i++) {
        new_time.push_back(this->time[i] - _FROM);
        new_psy.push_back(this->Psy[i]);
    }
    this->time = new_time;
    this->Psy = new_psy;
}

void Wavelet::cutFrom(int _FROM) {
    this->cutFromTo(_FROM, this->Psy.size());
}

void Wavelet::shrink(double _S) { 
    std::vector<double> npsy, new_psy;
    std::vector<int> ntime, new_time;
    for (unsigned int i = 0; i < this->time.size(); i++) {
        ntime.push_back(this->time[i] / _S);
        npsy.push_back(this->Psy[i]);
    }
    for (unsigned int i = ntime.size() - 1; i > 0; i--) {
        if (ntime[i] == ntime[i - 1]) ntime[i] = -1;
    }

    for (unsigned int i = 0; i < ntime.size(); i++) {
        if (ntime[i] == -1) {
            ntime.erase(ntime.begin() + i);
            npsy.erase(npsy.begin() + i);
            i--;
        }
    }
    for (int j = 0; j < _S; j++) {
        for (unsigned int i = 0; i < ntime.size(); i++) {
            if (ntime[i] != -1) {
                new_time.push_back(ntime[i] + ntime.size() * j);
                new_psy.push_back(npsy[i]);
            }
        }
    }
    this->time = new_time;
    this->Psy = new_psy;    
}

Wavelet* Wavelet::makeNewShrinked(double _S) {
    Wavelet* wvt = new Wavelet(this->Psy, this->time);
    wvt->shrink(_S);
    return wvt;
}

std::vector<double> Wavelet::getPsy() {
    return this->Psy;
}

int Wavelet::numberOfCounts() {
    return this->time.size();
}

void Wavelet::makeWavFile(std::string filename, float duration) {
    std::vector<double> nF;
    std::vector<int> ntime;
    int dur = (int)(duration * _S_FREQ);
    for (int j = 0; j < dur / this->time.size(); j++) {
        for (unsigned int i = 0; i < this->time.size(); i++) {
            ntime.push_back(time[i] + time.size() * j);
            nF.push_back(this->Psy[i]);
        }
    }
    Sound* sound = new Sound(nF, ntime);
    sound->makeWavFile(filename);
}

double Wavelet::averageValue() {
    double integral = 0;
    for (unsigned int i = 0; i < this->time.size(); i++) {
        integral += this->Psy[i];
    }
    return integral;
}

double Wavelet::energyValue() {
    double integral = 0;
    for (unsigned int i = 0; i < this->time.size(); i++) {
        integral += pow(this->Psy[i], 2);
    }
    return integral;
}

void Wavelet::smoothEnds() {
    double av = (this->Psy[0] + this->Psy[Psy.size() - 1]) / 2;
    int last = this->time[time.size() - 1];
    this->time.push_back(last + 1);
    this->Psy.push_back(av);
}

Wavelet::~Wavelet() {
    this->Psy.erase(Psy.begin(), Psy.end());
    this->time.erase(time.begin(), time.end());
}

//====================//
