/*
 * Created on Sun Nov 17 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once
extern "C" {
    #include <canberra.h>
}

#include <map>
#include <memory>
#include <string>

class SoundEngine
{
public:
    typedef enum {
        Enable,
        Disable,
        Modify,
        LithiumOn,
        LithiumOff,

        kNumSounds
    } Sound;

    SoundEngine();
    ~SoundEngine();

    void play_sound(Sound sound);

private:
    ca_context *_ca_context;
    std::string cache_sound_on_disk_and_get_realpath(Sound sound);
    std::string sound_filename(Sound sound);
};

