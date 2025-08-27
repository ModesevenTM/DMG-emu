#include "Audio.h"
#include "SM83.h"

Audio::Audio()
{
   // https://github.com/sysprog21/jitboy/blob/master/src/audio.c
   SDL_InitSubSystem(SDL_INIT_AUDIO);

   SDL_AudioSpec have;
   SDL_AudioSpec want;
   want.freq = AUDIO_SAMPLE_RATE;
   want.format = AUDIO_F32SYS;
   want.channels = 2;
   want.samples = AUDIO_SAMPLES;
   want.callback = audio_callback;
   want.userdata = this;

   if ((audioDev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0)) == 0) {
       printf("SDL could not open audio device: %s\n", SDL_GetError());
       exit(-1);
   }

   SDL_PauseAudioDevice(audioDev, 0);
}

double Audio::hipass(struct chan* c, float sample)
{
    float out = sample - c->capacitor;
    c->capacitor = sample - out * 0.996f;
    return out;
}

void Audio::set_note_freq(struct chan* c, const uint_fast16_t freq)
{
    c->freq_inc = freq / AUDIO_SAMPLE_RATE;
}

void Audio::chan_enable(const uint_fast8_t i, const bool enable)
{
    chans[i].enabled = enable;

    uint8_t val = (NR[5][2] & 0x80) |
        (chans[3].enabled << 3) | (chans[2].enabled << 2) |
        (chans[1].enabled << 1) | (chans[0].enabled << 0);

    NR[5][2] = val;
}

void Audio::update_env(struct chan* c)
{
    c->env.counter += c->env.inc;

    while (c->env.counter > 1.0f) {
        if (c->env.step) {
            c->volume += c->env.up ? 1 : -1;
            if (c->volume == 0 || c->volume == 15) {
                c->env.inc = 0;
            }
            c->volume = std::max<unsigned>(0, std::min<unsigned>(15, c->volume));
        }
        c->env.counter -= 1.0f;
    }
}

void Audio::update_len(struct chan* c)
{
    if (c->len.enabled) {
        c->len.counter += c->len.inc;
        if (c->len.counter > 1.0f) {
            chan_enable(c - chans, 0);
            c->len.counter = 0.0f;
        }
    }
}

bool Audio::update_freq(struct chan* c, float* pos)
{
    float inc = c->freq_inc - *pos;
    c->freq_counter += inc;

    if (c->freq_counter > 1.0f) {
        *pos = c->freq_inc - (c->freq_counter - 1.0f);
        c->freq_counter = 0.0f;
        return true;
    }
    else {
        *pos = c->freq_inc;
        return false;
    }
}

void Audio::update_sweep(struct chan* c)
{
    c->sweep.counter += c->sweep.inc;

    while (c->sweep.counter > 1.0f) {
        if (c->sweep.shift) {
            uint16_t inc = (c->sweep.freq >> c->sweep.shift);
            if (!c->sweep.up)
                inc *= -1;

            c->freq += inc;
            if (c->freq > 2047) {
                c->enabled = 0;
            }
            else {
                set_note_freq(c, 4194304 / ((2048 - c->freq) << 5));
                c->freq_inc *= 8.0f;
            }
        }
        else if (c->sweep.rate) {
            c->enabled = 0;
        }
        c->sweep.counter -= 1.0f;
    }
}

void Audio::update_square(float* samples, const bool ch2)
{
    struct chan* c = chans + ch2;
    if (!c->powered)
        return;

    set_note_freq(c, 4194304.0f / ((2048 - c->freq) << 5));
    c->freq_inc *= 8.0f;

    for (uint_fast16_t i = 0; i < AUDIO_NSAMPLES; i += 2) {
        update_len(c);

        if (c->enabled) {
            update_env(c);
            if (!ch2)
                update_sweep(c);

            float pos = 0.0f;
            float prev_pos = 0.0f;
            float sample = 0.0f;

            while (update_freq(c, &pos)) {
                c->duty_counter = (c->duty_counter + 1) & 7;
                sample += ((pos - prev_pos) / c->freq_inc) * (float)c->val;
                c->val = (c->duty & (1 << c->duty_counter)) ? 1 : -1;
                prev_pos = pos;
            }
            sample += ((pos - prev_pos) / c->freq_inc) * (float)c->val;
            sample = hipass(c, sample * (c->volume / 15.0f));

            if (!c->muted) {
                samples[i + 0] += sample * 0.25f * c->on_left * vol_l;
                samples[i + 1] += sample * 0.25f * c->on_right * vol_r;
            }
        }
    }
}

uint8_t Audio::wave_sample(const unsigned int pos, const unsigned int volume)
{
    uint8_t sample = WAV[pos / 2];
    if (pos & 1) {
        sample &= 0xF;
    }
    else {
        sample >>= 4;
    }
    return volume ? (sample >> (volume - 1)) : 0;
}

void Audio::update_wave(float* samples)
{
    struct chan* c = chans + 2;
    if (!c->powered)
        return;

    uint_fast16_t freq = 4194304.0f / ((2048 - c->freq) << 5);
    set_note_freq(c, freq);

    c->freq_inc *= 16.0f;

    for (uint_fast16_t i = 0; i < AUDIO_NSAMPLES; i += 2) {
        update_len(c);

        if (c->enabled) {
            float pos = 0.0f;
            float prev_pos = 0.0f;
            float sample = 0.0f;

            c->sample = wave_sample(c->val, c->volume);

            while (update_freq(c, &pos)) {
                c->val = (c->val + 1) & 31;
                sample += ((pos - prev_pos) / c->freq_inc) * (float)c->sample;
                c->sample = wave_sample(c->val, c->volume);
                prev_pos = pos;
            }
            sample += ((pos - prev_pos) / c->freq_inc) * (float)c->sample;

            // Replace the problematic line with the following code:
            float diff;
            if (c->volume > 0) {
                switch (c->volume) {
                case 1: diff = 7.5f; break;
                case 2: diff = 3.75f; break;
                case 3: diff = 1.5f; break;
                default: diff = 0.0f; break;
                }
                sample = hipass(c, (sample - diff) / 7.5f);

                if (!c->muted) {
                    samples[i + 0] += sample * 0.25f * c->on_left * vol_l;
                    samples[i + 1] += sample * 0.25f * c->on_right * vol_r;
                }
            }
        }
    }
}

void Audio::update_noise(float* samples)
{
    struct chan* c = chans + 3;
    if (!c->powered)
        return;

    uint_fast8_t lfsr_div_values[] = { 8, 16, 32, 48, 64, 80, 96, 112 };
    uint_fast16_t freq = 4194304 / (lfsr_div_values[c->lfsr_div] << c->freq);
    set_note_freq(c, freq);

    if (c->freq >= 14)
        c->enabled = 0;

    for (uint_fast16_t i = 0; i < AUDIO_NSAMPLES; i += 2) {
        update_len(c);

        if (c->enabled) {
            update_env(c);

            float pos = 0.0f;
            float prev_pos = 0.0f;
            float sample = 0.0f;

            while (update_freq(c, &pos)) {
                c->lfsr_reg = (c->lfsr_reg << 1) | (c->val == 1);

                if (c->lfsr_wide) {
                    c->val =
                        !(((c->lfsr_reg >> 14) & 1) ^ ((c->lfsr_reg >> 13) & 1))
                        ? 1
                        : -1;
                }
                else {
                    c->val =
                        !(((c->lfsr_reg >> 6) & 1) ^ ((c->lfsr_reg >> 5) & 1))
                        ? 1
                        : -1;
                }
                sample += ((pos - prev_pos) / c->freq_inc) * c->val;
                prev_pos = pos;
            }
            sample += ((pos - prev_pos) / c->freq_inc) * c->val;
            sample = hipass(c, sample * (c->volume / 15.0f));

            if (!c->muted) {
                samples[i + 0] += sample * 0.25f * c->on_left * vol_l;
                samples[i + 1] += sample * 0.25f * c->on_right * vol_r;
            }
        }
    }
}

void Audio::chan_trigger(uint_fast8_t i)
{
    struct chan* c = chans + i;

    chan_enable(i, 1);
    c->volume = c->volume_init;

    /* volume envelope */
    {
        uint8_t val = NR[i][2];

        c->env.step = val & 0x07;
        c->env.up = val & 0x08 ? 1 : 0;
        c->env.inc = c->env.step
            ? (64.0f / (float)c->env.step) / AUDIO_SAMPLE_RATE
            : 8.0f / AUDIO_SAMPLE_RATE;
        c->env.counter = 0.0f;
    }

    /* freq sweep */
    if (i == 0) {
        uint8_t val = NR[1][0];

        c->sweep.freq = c->freq;
        c->sweep.rate = (val >> 4) & 0x07;
        c->sweep.up = !(val & 0x08);
        c->sweep.shift = (val & 0x07);
        c->sweep.inc = c->sweep.rate ? (128.0f / (float)(c->sweep.rate)) /
            AUDIO_SAMPLE_RATE
            : 0;
        c->sweep.counter = nexttowardf(1.0f, 1.1f);
    }

    int len_max = 64;

    if (i == 2) { /* wave */
        len_max = 256;
        c->val = 0;
    }
    else if (i == 3) { /* noise */
        c->lfsr_reg = 0xFFFF;
        c->val = -1;
    }

    c->len.inc = (256.0f / (float)(len_max - c->len.load)) / AUDIO_SAMPLE_RATE;
    c->len.counter = 0.0f;
}

void Audio::channel_update(const uint16_t addr, const uint8_t val)
{
    /* Find sound channel corresponding to register address. */
    uint_fast8_t i = (addr - 0xFF10) / 5;

    switch (addr) {
    case 0xFF12:
    case 0xFF17:
    case 0xFF21: {
        chans[i].volume_init = val >> 4;
        chans[i].powered = (val >> 3) != 0;

        /* "zombie mode" stuff, needed for Prehistorik Man and similar */
        if (chans[i].powered && chans[i].enabled) {
            if ((chans[i].env.step == 0 && chans[i].env.inc != 0)) {
                if (val & 0x08) {
                    chans[i].volume++;
                }
                else {
                    chans[i].volume += 2;
                }
            }
            else {
                chans[i].volume = 16 - chans[i].volume;
            }

            chans[i].volume &= 0x0F;
            chans[i].env.step = val & 0x07;
        }
    } break;

    case 0xFF1C:
        chans[i].volume = chans[i].volume_init = (val >> 5) & 0x03;
        break;

    case 0xFF11:
    case 0xFF16:
    case 0xFF20: {
        const uint8_t duty_lookup[] = { 0x10, 0x30, 0x3C, 0xCF };
        chans[i].len.load = val & 0x3f;
        chans[i].duty = duty_lookup[val >> 6];
        break;
    }

    case 0xFF1B:
        chans[i].len.load = val;
        break;

    case 0xFF13:
    case 0xFF18:
    case 0xFF1D:
        chans[i].freq &= 0xFF00;
        chans[i].freq |= val;
        break;

    case 0xFF1A:
        chans[i].powered = (val & 0x80) != 0;
        chan_enable(i, val & 0x80);
        break;

    case 0xFF14:
    case 0xFF19:
    case 0xFF1E:
        chans[i].freq &= 0x00FF;
        chans[i].freq |= ((val & 0x07) << 8);
        /* Intentional fall-through */
    case 0xFF23:
        chans[i].len.enabled = val & 0x40 ? 1 : 0;
        if (val & 0x80)
            chan_trigger(i);

        break;

    case 0xFF22:
        chans[3].freq = val >> 4;
        chans[3].lfsr_wide = !(val & 0x08);
        chans[3].lfsr_div = val & 0x07;
        break;

    case 0xFF24:
        vol_l = ((val >> 4) & 0x07) / 7.0f;
        vol_r = (val & 0x07) / 7.0f;
        break;

    case 0xFF25:
        for (uint_fast8_t i = 0; i < 4; ++i) {
            chans[i].on_left = (val >> (4 + i)) & 1;
            chans[i].on_right = (val >> i) & 1;
        }
        break;
    }
}

void Audio::lock_audio_dev()
{
    SDL_LockAudioDevice(audioDev);
}

void Audio::unlock_audio_dev()
{
    SDL_UnlockAudioDevice(audioDev);
}


/* SDL2 style audio callback function */
void audio_callback(void* userdata, uint8_t* stream, int len)
{
    float* samples = (float*)stream;

    /* Appease unused variable warning. */
    Audio* apu = (Audio*)userdata;

    memset(stream, 0, len);

    apu->update_square(samples, 0);
    apu->update_square(samples, 1);
    apu->update_wave(samples);
    apu->update_noise(samples);
}