/*
  ==============================================================================

    filterADSR.cpp
    Created: 1 Jul 2023 11:16:46am
    Author:  Owen Prendergast

  ==============================================================================
*/

#include "FilterADSR.h"

void FilterADSR::update (const float attack, const float decay, const float sustain, const float release)
{
    //DBG("a: " + std::to_string(attack) + " d: " + std::to_string(decay) + " s: " + std::to_string(sustain) + " r: " + std::to_string(release));
    
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
      
    setParameters (adsrParams);
}
