#pragma once
#ifndef CATACURSE_H
#define CATACURSE_H

#include <vector>
#include <array>
#include <string>

#ifdef __ANDROID__
// See: http://stackoverflow.com/questions/10274920/how-to-get-printf-messages-written-in-ndk-application

#if defined(RELEASE)

#define  LOGD(...)
#define  LOGE(...)

#define DUMP_STACK()

#else

#include <jni.h>
#include <android/log.h>
#include <unwind.h>
#include <dlfcn.h>
#include <cxxabi.h>

#define  LOG_TAG    "cdda"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define DUMP_STACK() dump_stack(__FILE__, __LINE__);

void dump_stack(const char* file, int line);

#endif

#endif // __ANDROID__


class nc_color;

namespace catacurses
{
class window;
enum base_color : short;
} // namespace catacurses

/**
 * Contains our own curses implementation.
 * Don't use it in the game itself, use only function of @ref catacurses.
 * Functions declared here are *not* defined in ncurses builds.
 */
namespace cata_cursesport
{
using base_color = catacurses::base_color;

//a pair of colors[] indexes, foreground and background
typedef struct {
    base_color FG;
    base_color BG;
} pairs;

//Individual lines, so that we can track changed lines
struct cursecell {
    std::string ch;
    base_color FG = static_cast<base_color>( 0 );
    base_color BG = static_cast<base_color>( 0 );

    cursecell( std::string ch ) : ch( std::move( ch ) ) { }
    cursecell() : cursecell( std::string( 1, ' ' ) ) { }

    bool operator==( const cursecell &b ) const {
        return FG == b.FG && BG == b.BG && ch == b.ch;
    }
};

struct curseline {
    bool touched;
    std::vector<cursecell> chars;
};

//The curses window struct
struct WINDOW {
    int x;//left side of window
    int y;//top side of window
    int width;
    int height;
    base_color FG;//current foreground color from attron
    base_color BG;//current background color from attron
    bool inuse;// Does this window actually exist?
    bool draw;//Tracks if the window text has been changed
    int cursorx;
    int cursory;
    std::vector<curseline> line;
};

extern std::array<pairs, 100> colorpairs;
void curses_drawwindow( WINDOW *win );

// allow extra logic for framebuffer clears
extern void handle_additional_window_clear( WINDOW *win );

} // namespace cata_cursesport

//@todo move into cata_cursesport
//used only in SDL mode for clearing windows using rendering
void clear_window_area( const catacurses::window &win );
int projected_window_width( int column_count );
int projected_window_height( int row_count );

#endif
