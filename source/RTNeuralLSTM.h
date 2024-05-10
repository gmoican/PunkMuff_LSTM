#pragma once

#include <RTNeural/RTNeural.h>

class RT_LSTM
{
public:
    RT_LSTM() = default;

    void reset();
    void load_json(const nlohmann::json& weights_json);

    void process(const float* inData, float* outData, float driveParam, int numSamples);
    
    int input_size = 2;
    
    float previousDrive = 0.5f;
    float steppedValue = 0.f;
    bool changedValue = false;

private:
    RTNeural::ModelT<float, 2, 1,
        RTNeural::LSTMLayerT<float, 2, 32>,
        RTNeural::DenseT<float, 32, 1>> model;
    
    float inArray alignas(16)[2] = { 0.0, 0.0 };
};
