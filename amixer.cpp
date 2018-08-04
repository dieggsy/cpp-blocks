#include <alsa/asoundlib.h>
#include <iostream>
#include <string>

std::string get_icon(long volume, int not_muted) {
    if (volume == 0 || !not_muted) {
        return "";
    }
    else if (volume < 50) {
        return "";
    }
    else {
        return "";
    }
}

int elem_callback(snd_mixer_elem_t *ctl, unsigned int mask) {
    if (mask && SND_CTL_EVENT_MASK_VALUE) {
        long volume;
        snd_mixer_selem_get_playback_volume(ctl, SND_MIXER_SCHN_FRONT_LEFT, &volume);
        int not_muted;
        snd_mixer_selem_get_playback_switch(ctl, SND_MIXER_SCHN_FRONT_LEFT, &not_muted);
        std::cout << get_icon(volume, not_muted) << ' ' << volume << std::endl;
    }
    return 0;
}

int main () {
    long volume;
    snd_mixer_t *mixer;

    // Load default mixer?
    snd_mixer_open(&mixer, 0);
    snd_mixer_attach(mixer, "default");
    snd_mixer_selem_register(mixer, NULL, NULL);

    snd_mixer_load(mixer);

    // Get first (master) element, whatever that means, and set a callback
    // function for events on that element
    snd_mixer_elem_t *master_element = snd_mixer_first_elem(mixer);
    snd_mixer_elem_set_callback(master_element, elem_callback);

    // Make the range pretty, I think - I would hope this doesn't affect the
    // _actual_ audio output levels
    snd_mixer_selem_set_playback_volume_range(master_element, 0, 100);

    // Call one time to prinnot_muted
    elem_callback(master_element, 1);

    // Does the thing (actually listens for events/triggers callback)
    while(snd_mixer_wait(mixer,-1) == 0) {
        snd_mixer_handle_events(mixer);
    }

    // Is this necessary?
    snd_mixer_close(mixer);
    return 0;
}
