#ifndef MIDIEVAL_H
#define MIDIEVAL_H

#include <inttypes.h>

typedef struct MDV_Sequence MDV_Sequence;
typedef struct MDV_Player MDV_Player;


///// Main player API /////

 // Allocate new player
MDV_Player* mdv_new_player ();
 // Load a .cfg containing patch names (nothing complicated please)
void mdv_load_config (MDV_Player*, const char* filename);
 // Set the sequence currently being played (use load_midi)
void mdv_play_sequence (MDV_Player*, MDV_Sequence*);
 // Get this many bytes of audio.  len must be a multiple of 4
void mdv_get_audio (MDV_Player*, uint8_t* buf, int len);
 // 0 if either no sequence was given or the sequence is done
int mdv_currently_playing (MDV_Player*);
 // Delete a player
void mdv_free_player (MDV_Player*);


///// Main sequences API /////

void mdv_free_sequence (MDV_Sequence*);

void mdv_print_sequence (MDV_Sequence*);

MDV_Sequence* mdv_load_midi (const char* filename);


///// Events API /////

enum MDV_Event_Type {
    MDV_NOTE_OFF = 0x08,
    MDV_NOTE_ON = 0x09,
    MDV_NOTE_AFTERTOUCH = 0x0A,
    MDV_CONTROLLER = 0x0B,
    MDV_PROGRAM_CHANGE = 0x0C,
    MDV_CHANNEL_AFTERTOUCH = 0x0D,
    MDV_PITCH_BEND = 0x0E,
    MDV_META = 0x0F,
    MDV_SET_TEMPO = 0x10
};
typedef uint8_t MDV_Event_Type;
enum MDV_Controller {
    MDV_BANK_SELECT = 1,
    MDV_MODULATION = 2,
    MDV_DATA_ENTRY_MSB = 6,
    MDV_VOLUME = 7,
    MDV_BALANCE = 8,
    MDV_PAN = 10,
    MDV_EXPRESSION = 11,
    MDV_DATA_ENTRY_LSB = 38,
    MDV_NRPN_LSB = 98,
    MDV_NRPN_MSB = 99,
    MDV_RPN_LSB = 100,
    MDV_RPN_MSB = 101
};
typedef uint8_t MDV_Controller;

static inline int mdv_parameters_used (uint8_t t) {
    if (t == MDV_PROGRAM_CHANGE || t == MDV_CHANNEL_AFTERTOUCH)
        return 1;
    else
        return 2;
};

typedef struct MDV_Event {
    uint8_t type;
    uint8_t channel;
    uint8_t param1;
    uint8_t param2;
} MDV_Event;

void mdv_print_event (MDV_Event*);

typedef struct MDV_Timed_Event {
    uint32_t time;  // Absolute time in ticks
    MDV_Event event;
} MDV_Timed_Event;

typedef struct MDV_Sequence {
    uint32_t tpb;
    uint32_t n_events;
    MDV_Timed_Event* events;
} MDV_Sequence;


///// Patches API /////

typedef struct MDV_Sample {
     // in milliHz
    uint32_t low_freq;
    uint32_t high_freq;
    uint32_t root_freq;
     // in samples
    uint32_t loop_start;
    uint32_t loop_end;
    uint32_t envelope_rates [6];
    uint32_t envelope_offsets [6];
    uint8_t pan;
    uint8_t loop;
    uint8_t pingpong;
    uint16_t sample_rate;
    uint32_t data_size;
    int16_t* data;
} MDV_Sample;

typedef struct MDV_Patch {
     // TODO: do we need any more information?
    uint16_t volume;
    int8_t note;
    uint8_t n_samples;
    MDV_Sample* samples;
} MDV_Patch;

MDV_Patch* _mdv_load_patch (const char* filename);
void mdv_free_patch (MDV_Patch*);

void mdv_print_patch (MDV_Patch*);

typedef struct MDV_Bank {
    MDV_Patch* patches [128];
    MDV_Patch* drums [128];
} MDV_Bank;

void mdv_bank_init (MDV_Bank*);
void mdv_bank_load_config (MDV_Bank*, const char* filename);
void mdv_bank_load_patch (MDV_Bank*, uint8_t instrument, const char* filename);
void mdv_bank_load_drum (MDV_Bank*, uint8_t instrument, const char* filename);
void mdv_bank_free_patches (MDV_Bank*);


///// Auxilary player API /////

 // Reset player to initial state
void mdV_reset_player (MDV_Player*);
 // Load an individual patch from a file
void mdv_load_patch (MDV_Player*, uint8_t index, const char* filename);
void mdv_load_drum (MDV_Player*, uint8_t drum, const char* filename);

#endif
