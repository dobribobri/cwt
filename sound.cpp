
#include "sound.h"

/*SOUND*/

Sound::Sound(std::string filename) {
    std::ifstream fin(filename);
    std::string buff;
    int numb = 0;
    while (getline(fin, buff)) {
        this->F.push_back(std::stod(buff));
        this->time.push_back(numb);
        numb++;
    }
}

Sound::Sound(double frequency, double amplitude, float duration) {
    this->F.erase(F.begin(), F.end());
    int dur = (int)(duration * _S_FREQ);
    for (unsigned int i = 0; i < dur; i++) {
        this->time.push_back(i);
        this->F.push_back(amplitude * sin(frequency * i / _S_FREQ));
    }
}

void Sound::draw() {
    Gnuplot gp1;
    gp1.set_title("Sound.draw()");
    gp1.reset_plot();
    gp1.set_grid();
    gp1.set_xlabel("t");
    gp1.set_ylabel("A");
    gp1.set_style("impulses").plot_xy(this->time, this->F, "SOUND");
    std::cin.get();
}

std::vector<double> Sound::getF() {
    return this->F;
}

Sound* Sound::makeInterval(float _FROM__IN_SECONDS, float _TO__IN_SECONDS) {
    int _from = (int)(_S_FREQ * _FROM__IN_SECONDS);
    int _to = (int)(_S_FREQ * _TO__IN_SECONDS);
    std::vector<double> new_f;
    std::vector<int> new_time;
    for (unsigned int i = _from; i < _to; i++) {
        new_time.push_back(this->time[i] - _from);
        new_f.push_back(this->F[i]);        
    }
    Sound* snd = new Sound(new_f, new_time);
    return snd;
}

float Sound::duration() {
    float dur = (float)this->time[time.size() - 1] / _S_FREQ;
    return dur;
}

std::string Sound::dtos(double i)
{
    std::stringstream s;
    s << i;
    return s.str();
}

void Sound::writeToFile(std::string filename, std::string extention) {
    std::ofstream fout;
    fout.open(filename + extention);
    std::string write;
    for (unsigned int i = 0; i < this->F.size(); i++) {
        write = this->dtos(F[i]) + "\n";
        fout << write;
    }
    fout.close();
}

void Sound::makeWavFile(std::string filename) {
    std::ofstream f(filename + ".wav", std::ios::binary);
    f << "RIFF----WAVEfmt ";
    write_word(f, 16, 4);      //no extention data
    write_word(f, 1, 2);       //PCM - integer samples
    write_word(f, 2, 2);       //2 channels
    write_word(f, 44100, 4);   //samples per second
    write_word(f, 176400, 4);  //(Sample Rate * BitsPerSample * Channels) / 8
    write_word(f, 4, 2);       //data block size (size of two integer samples, one for each channel, in bytes)
    write_word(f, 16, 2);      //number of bits per sample (use a multiple of 8)
    size_t data_chunk_pos = f.tellp();
    f << "data----";
    for (unsigned int i = 0; i < this->F.size(); i++) {
        write_word(f, (int)(this->F[i]), 2);
        write_word(f, (int)(this->F[i]), 2);
    }
    size_t file_length = f.tellp();
    f.seekp(data_chunk_pos + 4);
    write_word(f, file_length - data_chunk_pos + 8);
    f.seekp(0 + 4);
    write_word(f, file_length - 8, 4);
    f.close();
    return;
}

Sound::~Sound() {
    this->F.erase(F.begin(), F.end());
    this->time.erase(time.begin(), time.end());
}

//====================//
