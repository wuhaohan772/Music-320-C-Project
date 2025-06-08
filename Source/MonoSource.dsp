declare name "monoSource";
declare description "Monophonic midi waveform generator with FM synthesis";

import("stdfaust.lib");

process = monoSource;

monoSource = env*wave
with
{
    // Note Control Parameters
    vel = nentry("velocity", 1, 0, 1, 0.01);
    gate = button("gate");
    freqHz = ba.midikey2hz(int(nentry("midiNote", 60, 36, 96, 1)));

    // Waveform Control Parameter
    select = int(nentry("type", 0, 0, 3, 1));

    // Envelope Control Parameters
    at = nentry("attack", 0.1, 0, 1, 0.01);
    dt = nentry("decay", 0.25, 0, 1, 0.01);
    sl = nentry("sustain", 0.5, 0, 1, 0.01);
    rt = nentry("release", 1, 0, 5, 0.01);
    env = vel * en.adsre(at, dt, sl, rt, gate);

    // WORKING FM - now use real parameters
    fm_enable = checkbox("fm_enable");
    fm_ratio = nentry("fm_ratio", 0.1, 0.01, 8.0, 0.01);
    fm_depth = nentry("fm_depth", 3, 0, 10, 0.1);
    fm_type  = int(nentry("fm_type", 0, 0, 3, 1));

    // Use the ACTUAL parameters (not hardcoded)
    modulator_freq = freqHz * fm_ratio;

    // Selectable Modulator Waveform
    mod_sine = os.osc(modulator_freq);
    mod_sqr = os.square(modulator_freq);
    mod_tri = os.triangle(modulator_freq);
    mod_saw = os.sawtooth(modulator_freq);
    modulator_wave = (mod_sine * (fm_type == 0) + mod_sqr * (fm_type == 1) + mod_tri * (fm_type == 2) + mod_saw * (fm_type == 3));
    
    // Same algorithm that worked, but using real parameters
    freq_multiplier = 1.0 + (modulator_wave * (fm_depth - 1.0) * fm_enable);
    fm_freq = freqHz * freq_multiplier;
    
    // Basic Waveforms using multiplicative FM frequency
    sine = os.osc(fm_freq);
    sqr = os.square(fm_freq);
    tri = os.triangle(fm_freq);
    saw = os.sawtooth(fm_freq);
    
    // Test if waveform selection parameter works
    wave = (sine * (select == 0) + sqr * (select == 1) + 
            tri * (select == 2) + saw * (select == 3));
};