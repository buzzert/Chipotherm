/*
 * Created on Sun Nov 17 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "sounds.h"

#include <cassert>
#include <gio/gio.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>

// TODO: is this the right way to use gresources?
extern "C" {
    extern GResource *chipotherm_get_resource (void);
}

static const std::string sound_filepath = "/var/tmp/chipotherm/sounds";

SoundEngine::SoundEngine()
{
    ca_context_create(&_ca_context);

    // Make temp directory if necessary
    std::string path;
    std::stringstream fp_ss(sound_filepath);
    for (std::string comp; std::getline(fp_ss, comp, '/'); path += "/") {
        path += comp;
        if (path.size() > 0) {
            struct stat stat_s;
            stat(path.c_str(), &stat_s);
            if (errno == ENOENT) {
                mkdir(path.c_str(), S_IRWXU);
            }
        }
    }
}

SoundEngine::~SoundEngine()
{
    ca_context_destroy(_ca_context);
}

std::string SoundEngine::sound_filename(Sound sound)
{
    switch (sound) {
        case Sound::Enable:
            return "enable.wav";
        case Sound::Disable:
            return "disable.wav";
        case Sound::Modify:
            return "modify.wav";
        case Sound::LithiumOn:
            return "lithium_on.wav";
        case Sound::LithiumOff:
            return "lithium_off.wav";
    }

    assert(false);
}

void SoundEngine::play_sound(Sound sound)
{
    struct stat s;
    std::string realpath = sound_filepath + "/" + sound_filename(sound);
    if (stat(realpath.c_str(), &s) == -1) {
        // xxx: not ideal, but canberra doesn't work with gresources, so we need to
        // write it to this temporary path.
        std::string resource_path = "/net/buzzert/chipotherm";

        GError *error = nullptr;
        GResource *resource = chipotherm_get_resource();
        if (resource == nullptr) {
            std::cerr << "ERROR loading resource bundle" << std::endl;
            return;
        }

        std::string sound_resource = "/net/buzzert/chipotherm/sounds/" + sound_filename(sound);
        GInputStream *input_stream = g_resource_open_stream(resource, sound_resource.c_str(), G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
        if (error != nullptr) {
            std::cerr << "ERROR loading sound resource: " << error->message << std::endl;
            return;
        }

        GFile *output_file = g_file_new_for_path(realpath.c_str());
        GFileIOStream *io_stream = g_file_create_readwrite(output_file, G_FILE_CREATE_NONE, nullptr, &error);
        if (error != nullptr) {
            std::cerr << "ERROR creating temporary sound file: " << error->message << std::endl;
            return;
        }

        GOutputStream *output_stream = g_io_stream_get_output_stream(G_IO_STREAM(io_stream));
        g_output_stream_splice(output_stream, input_stream, G_OUTPUT_STREAM_SPLICE_CLOSE_TARGET, nullptr, &error);
        if (error != nullptr) {
            std::cerr << "ERROR writing to temporary sound file: " << error->message << std::endl;
            return;
        }

        g_input_stream_close(input_stream, nullptr, nullptr);
        g_object_unref(input_stream);
        g_object_unref(output_stream);
        g_object_unref(output_file);
        g_resource_unref(resource);
    }

    ca_context_play(_ca_context, 0,
                    CA_PROP_MEDIA_FILENAME, realpath.c_str(), NULL);
}
