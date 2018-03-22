#include <iostream>
#include <stdio.h>

#include "sound.h"
#include "wavelet.h"
#include "cwt.h"

/*MAIN*/
int main() {
/*
    Sound* sound = new Sound("A4.txt");
    sound->draw();
    Wavelet* wavelet = new Wavelet("A0.txt");
    wavelet->cut_default();
    wavelet->cut(NUMBER_OF_PERIODS);
    wavelet->draw();
/*
    for (int i = 0; i < 8; i++) {
        Wavelet* wvt = wavelet->makeNewShrinked(pow(2, i/12));
        wvt->draw();
        delete wvt;
    }
*/
/*
    CWT* cwt = new CWT(sound, wavelet);

    cwt->setParameters(OCTAVES, STEP_TAU);
    cwt->make();
    cwt->cwtDataToFiles("cwtresult", ".txt");

    cwt->drawFileData("cwtresult_0.txt");
*/
/*
    //Sound* sound = new Sound(3500, 440, 2);
    Sound* sound = new Sound("A4.txt");
    sound->draw();
    sound->writeToFile("sin440", ".txt");
    sound->makeWavFile("sin440");
*/
    Wavelet* wavelet1 = new Wavelet("A0.txt");
    wavelet1->cut_default();
    wavelet1->cut(16);
    wavelet1->smoothEnds();
    wavelet1->smoothEnds();
    std::cout << wavelet1->averageValue() << std::endl;
    std::cout << wavelet1->energyValue() << std::endl;
    wavelet1->makeWavFile("wavelet_A0_16", 2);
    wavelet1->draw();
    Wavelet* wavelet2 = new Wavelet("A0.txt");
    wavelet2->cut_default();
    wavelet2->cut(128);
    wavelet1->smoothEnds();
    std::cout << wavelet2->averageValue() << std::endl;
    std::cout << wavelet2->energyValue() << std::endl;
    wavelet2->makeWavFile("wavelet_A0_128", 2);
    wavelet2->draw();
    return 0;
}
