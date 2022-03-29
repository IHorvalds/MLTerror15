#pragma once


inline const char* param_names[] = { "Input", "Model", "Output", "Bypass" };

class Parameters {
private:
    enum param_name {
        input,
        model,
        output,
        bypass
    };
public:
    static const param_name k_input = input;
    static const param_name k_model = model;
    static const param_name k_output = output;
    static const param_name k_bypass = bypass;
};