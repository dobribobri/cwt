
#include "cwt.h"

/*CWT*/

void CWT::setParameters(int _OCTAVES, int _STEP_TAU) {
    this->octaves = _OCTAVES;
    this->step_tau = _STEP_TAU;
}

void CWT::make() {
    std::cout << "CWT started" << std::endl;
    for (float I = 0; I < this->sound->duration(); I += _SUP_MS) {
        if (I + _SUP_MS > this->sound->duration()) break;
        std::cout << " [";
        Sound* snd = this->sound->makeInterval(I, I + _SUP_MS);
        std::vector<double> F = snd->getF();
        Matrix res;
        for (double s = 0; s < this->octaves * 12; s++) {
            std::cout << ".";
            Wavelet* wvt = this->wavelet->makeNewShrinked(pow(2, s/12));
            std::vector<double> Psy_s = wvt->getPsy();
            for (int tau = 0; tau < snd->duration() * _S_FREQ; tau += step_tau) {
                if (tau + wvt->numberOfCounts() > snd->duration() * _S_FREQ) break;
                std::pair<double, double> xy = std::make_pair((double)pow(2, s/12), (double)tau);
                double integral = 0;
                for (int t = tau; t < tau + wvt->numberOfCounts(); t++) {
                    integral += F[t] * Psy_s[t - tau];
                }
                integral /= sqrt(pow(2, s/12));
                res.insert(std::make_pair(xy, integral));
            }
            delete wvt;
        }
        this->result.push_back(res);
        delete snd;
        std::cout << "]";
    }
    std::cout << "\nOK" << std::endl;
}

CWTData CWT::makeAndGetResult() {
    this->make();
    return this->result;
}

std::string CWT::dtos(double i)
{
    std::stringstream s;
    s << i;
    return s.str();
}

void CWT::cwtDataToFiles(std::string filename, std::string extention) {
    std::cout << "Writing to files..." << std::endl;
    Matrix::iterator it;
    std::string write;
    for (unsigned int i = 0; i < this->result.size(); i++) {
        std::ofstream fout;
        fout.open(filename + "_" + this->dtos(i) + extention);
        for (it = this->result[i].begin(); it != this->result[i].end(); it++) {
            write = this->dtos(it->first.first) + ";" + 
                    this->dtos(it->first.second) + ";" +
                    this->dtos(it->second) + ";\n";
            fout << write;
        }
        fout.close();
    }
    std::cout << "OK" << std::endl;
}

void CWT::_draw(std::vector<double>& s, std::vector<double>& tau, std::vector<double>& A) {
    Gnuplot gp1;
    gp1.set_title("CWT.draw()");
    gp1.reset_plot();
    gp1.set_grid();
    gp1.set_xlabel("tau");
    gp1.set_ylabel("s");
    gp1.set_zlabel("A");
    std::vector<double> ns, ntau, nA;
    ns.push_back(s[0]);
    ntau.push_back(tau[0]);
    nA.push_back(A[0]);
    for (unsigned int i = 1; i < s.size(); i++) {
        if (s[i] != s[i-1]) {
            gp1.set_style("lines").plot_xyz(ntau, ns, nA, this->dtos(ns[0]));
            ns.erase(ns.begin(), ns.end());
            ntau.erase(ntau.begin(), ntau.end());
            nA.erase(nA.begin(), nA.end());            
        }
        ns.push_back(s[i]);
        ntau.push_back(tau[i]);
        nA.push_back(A[i]);             
    }
    gp1.set_style("lines").plot_xyz(ntau, ns, nA, this->dtos(ns[0]));
    std::cin.get();    
}

void CWT::draw(int _SEGMENT) {
    Matrix::iterator it;
    std::vector<double> s, tau, A;
    for (it = this->result[_SEGMENT].begin(); it != this->result[_SEGMENT].end(); it++) {
        s.push_back(it->first.first);
        tau.push_back(it->first.second);
        A.push_back(it->second);
    }
    this->_draw(s, tau, A);        
}

void CWT::drawFileData(std::string filename) {
    std::vector<double> s, tau, A;
    std::ifstream fin(filename);
    std::string buff, elem;
    size_t pos;
    std::string delimiter = ";";
    while (getline(fin, buff)) {
        pos = buff.find(delimiter);
        elem = buff.substr(0, pos);
        s.push_back(std::stod(elem));
        buff.erase(0, pos + delimiter.length());
        pos = buff.find(delimiter);
        elem = buff.substr(0, pos);
        tau.push_back(std::stod(elem));
        buff.erase(0, pos + delimiter.length());
        pos = buff.find(delimiter);
        elem = buff.substr(0, pos);
        A.push_back(std::stod(elem));
        buff.erase(0, pos + delimiter.length());
    }
    this->_draw(s, tau, A);
}

//====================//
