#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <SDL.h>

#define AUDIO_SAMPLE_RATE 48000.0

#define DMG_CLOCK_FREQ 4194304.0
#define SCREEN_REFRESH_CYCLES 70224.0
#define VERTICAL_SYNC (DMG_CLOCK_FREQ / SCREEN_REFRESH_CYCLES)

#define AUDIO_SAMPLES ((unsigned) (AUDIO_SAMPLE_RATE / VERTICAL_SYNC))
#define AUDIO_NSAMPLES (AUDIO_SAMPLES * 2)

class SM83;

class Audio
{
private:

	SDL_AudioDeviceID audioDev;

    struct chan_len_ctr {
        unsigned load : 6;
        unsigned enabled : 1;
        float counter;
        float inc;
    };

    struct chan_vol_env {
        unsigned step : 3;
        unsigned up : 1;
        float counter;
        float inc;
    };

    struct chan_freq_sweep {
        uint_fast16_t freq;
        unsigned rate : 3;
        unsigned up : 1;
        unsigned shift : 3;
        float counter;
        float inc;
    };

    struct chan {
        unsigned enabled : 1;
        unsigned powered : 1;
        unsigned on_left : 1;
        unsigned on_right : 1;
        unsigned muted : 1;

        unsigned volume : 4;
        unsigned volume_init : 4;

        uint16_t freq;
        float freq_counter;
        float freq_inc;

        int val;

        struct chan_len_ctr len;
        struct chan_vol_env env;
        struct chan_freq_sweep sweep;

        /* square */
        uint8_t duty;
        uint8_t duty_counter;

        /* noise */
        uint16_t lfsr_reg;
        bool lfsr_wide;
        int lfsr_div;

        /* wave */
        uint8_t sample;

        float capacitor;
    } chans[4];

    float vol_l, vol_r;

	double hipass(struct chan* c, float sample);
	void set_note_freq(struct chan* c, const uint_fast16_t freq);
    void chan_enable(const uint_fast8_t i, const bool enable);
    void update_env(struct chan* c);
	void update_len(struct chan* c);
	bool update_freq(struct chan* c, float* pos);
	void update_sweep(struct chan* c);
	uint8_t wave_sample(const unsigned int pos, const unsigned int volume);
	void chan_trigger(uint_fast8_t i);

public:
	// registers
	uint8_t NR[6][5] = {	// Channel 1, 2, 3, 4, Control registers
        {0},                            // dummy
		{0x80, 0xBF, 0xF3, 0xFF, 0xBF},	// Channel 1
		{0, 0x3F, 0x00, 0xFF, 0xBF},	// Channel 2
		{0x7F, 0xFF, 0x9F, 0xFF, 0xBF},	// Channel 3
		{0, 0xFF, 0x00, 0x00, 0xBF},	// Channel 4
		{0x77, 0xF3, 0xF1, 0, 0}		// Control registers
	};
	//uint8_t NR1[5] = {0x80, 0xBF, 0xF3, 0xFF, 0xBF};	// Channel 1
	//uint8_t NR2[4] = {0x3F, 0x00, 0xFF, 0xBF};			// Channel 2
	//uint8_t NR3[5] = {0x7F, 0xFF, 0x9F, 0xFF, 0xBF};	// Channel 3
	//uint8_t NR4[4] = {0xFF, 0x00, 0x00, 0xBF};			// Channel 4
	//uint8_t NR5[3] = {0x77, 0xF3, 0xF1};				// Control registers
	uint8_t WAV[16] = {0};	// Wave Pattern RAM

	SM83* sm83;

	Audio();
	void update_square(float* samples, const bool ch2);
	void update_wave(float* samples);
	void update_noise(float* samples);
    void channel_update(const uint16_t addr, const uint8_t val);
    void lock_audio_dev();
    void unlock_audio_dev();
};

void audio_callback(void* userdata, uint8_t* stream, int len);