#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <fstream>
#include <AL/al.h>
#include <AL/alc.h>
#include <chrono>

#include "soundmanager.h"

int main() {

    SoundManager _soundLoader;
    _soundLoader.initialize();

    //Audio namexsource
    //Key: name, Value: path
    std::map<std::string, std::string> audioFiles = {
        { "l", "D:/Repositories/SoundManager/sounds/15727__blackstrobe__l.wav"},
        { "f", "D:/Repositories/SoundManager/sounds/15721__blackstrobe__f.wav"},
        { "g", "D:/Repositories/SoundManager/sounds/15722__blackstrobe__g.wav"},
        { "4", "D:/Repositories/SoundManager/sounds/15733__blackstrobe__morse_4.wav"},
        { "n", "D:/Repositories/SoundManager/sounds/15739__blackstrobe__n.wav"},
        { "t", "D:/Repositories/SoundManager/sounds/15745__blackstrobe__t.wav"},
        { "k", "D:/Repositories/SoundManager/sounds/15726__blackstrobe__k.wav"}
    };

    _soundLoader.loadWavFiles( audioFiles );

    _soundLoader.addSource( "EFFECTS" );
    _soundLoader.addSource( "AMBIENT" );
    _soundLoader.addSource( "MICROFONE" );
    _soundLoader.addSource( "RADIO" );

    _soundLoader.setSoundQueue( "RADIO", { "n", "t", "l" } );
    _soundLoader.setPitch( "RADIO", 0.75f );
    _soundLoader.setLoop( "RADIO", true );
    _soundLoader.play( "RADIO" );

    int i = 0;
    int count = 60*10; //10 seconds to stop
    while(true)
    {
//        std::cout << "isPlaying? " << _soundLoader.isPlaying( "RADIO" ) << std::endl;

        if( i == count ) break;
        i++;

        std::this_thread::sleep_for( std::chrono::milliseconds( 16 ) );
    };

    _soundLoader.closeSoundApi();

    return 0;
}
