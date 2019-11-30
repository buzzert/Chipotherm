/*
 * Created on Tue Jan 29 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "io_control.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#define CHIP_GPIO_PIN_1 1015

float IOControl::read_temperature()
{
    if (simulate) {
        return simulated_temperature;
    } else {
        initialize_devices_if_necessary();
        tempered_read_sensors(_monitor_device.get());

        float resultC = 0.0;
        float resultF = 0.0;
        bool result = tempered_get_temperature(_monitor_device.get(), 0, &resultC);
        if (!result) {
            fprintf(stderr, "Error reading temperature from device\n");
            resultF = -1.0; // report -1 so it's really obvious something is wrong in the UI
        } else {
            resultF = (resultC * (9.0f / 5.0f) + 32.0f);
        }
        
        return resultF;
    }
}

void IOControl::set_heater_on(bool on)
{
    if (simulate) {
        _sim_heat_on = on;
    } else {
        initialize_devices_if_necessary();
        std::ofstream value_file(_relay_switch_path, std::ofstream::out);

        // On the relay, "1" actually means "open" (aka off) and "0" means "closed" (aka on)
        value_file << !on;
    }
}

bool IOControl::get_heater_on() 
{
    if (simulate) {
        return _sim_heat_on;
    } else {
        initialize_devices_if_necessary();
        bool status = false;

        std::ifstream value_file(_relay_switch_path, std::ifstream::in);
        value_file >> status;

        // See note in set_on for why this is inverted.
        return !status;
    }
}

bool IOControl::get_display_on()
{
    if (simulate) {
        // Simulation lies.
        return true;
    }

    if (!_display_device) {
        // Assume display is on
        fprintf(stderr, "Error: get_display_on with no active display device\n");
        return true;
    }

    const char *dpms_value = udev_device_get_sysattr_value(_display_device.get(), "dpms");
    if (dpms_value == nullptr) {
        fprintf(stderr, "Error: get_display_on getting dpms sysattr value\n");
        return true;
    }

    return (strcmp(dpms_value, "On") == 0);
}

void IOControl::initialize_devices_if_necessary()
{
    // Thermostat
    if (!_monitor_device) {
        do {
            char *error_str = nullptr;
            tempered_device_list *device_list = tempered_enumerate(&error_str);
            if (!device_list) {
                fprintf(stderr, "Error enumerating temperature devices: %s\n", error_str);
                break;
            }

            tempered_device *device = tempered_open(device_list, &error_str);
            if (!device) {
                fprintf(stderr, "Error opening temperature device: %s\n", error_str);
                break;
            }

            _monitor_device = std::shared_ptr<tempered_device>(device, tempered_close);
        } while (0);
    }

    // Relay GPIO switch
    if (_relay_switch_path.length() == 0) {
        do {
            /* Export pin */ {
                std::ofstream export_file("/sys/class/gpio/export", std::ofstream::out);
                if (!export_file) {
                    fprintf(stderr, "Error initializing GPIO pin: export endpoint doesn't exist\n");
                    break;
                }

                export_file << CHIP_GPIO_PIN_1;
            }

            std::stringstream base_path_string;
            base_path_string << "/sys/class/gpio/gpio" << CHIP_GPIO_PIN_1;

            /* Set pin direction */ {
                std::ofstream direction_file(base_path_string.str() + "/direction");
                if (!direction_file) {
                    fprintf(stderr, "Error initializing GPIO pin: unable to set pin direction\n");
                    break;
                }

                direction_file << "out";
            } 

            _relay_switch_path = base_path_string.str() + "/value";
            set_heater_on(false); // set to OFF as initial state
        } while (0);
    }

    // Display (for blocking hit testing when asleep)
    if (!_display_device) {
        do {
            struct udev *udev = udev_new();
            if (!udev) {
                fprintf(stderr, "Error initializing udev\n");
                break;
            }

            // Scan drm devices (display devices)
            struct udev_enumerate *enumerate = udev_enumerate_new(udev);
            udev_enumerate_add_match_subsystem(enumerate, "drm");
            udev_enumerate_scan_devices(enumerate);


            struct udev_list_entry *dev_list_entry;
            struct udev_device *active_monitor_device = nullptr;
            struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);

            // Locate active monitor device (drm device whose enabled="enabled")
            udev_list_entry_foreach(dev_list_entry, devices) {
                const char *path = udev_list_entry_get_name(dev_list_entry);
                struct udev_device *dev = udev_device_new_from_syspath(udev, path);
                if (!dev) continue;

                const char *val = udev_device_get_sysattr_value(dev, "enabled");
                if (val && strcmp(val, "enabled") == 0) {
                    active_monitor_device = dev;
                } else {
                    udev_device_unref(dev);
                }
            }

            udev_enumerate_unref(enumerate);

            if (active_monitor_device != nullptr) {
                fprintf(stderr, "Found active monitor device\n");
                _display_device = std::shared_ptr<udev_device>(active_monitor_device, udev_device_unref);
            }

            udev_unref(udev);

        } while (0);
    }
}
