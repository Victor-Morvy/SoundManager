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
    std::map<std::string, std::string> audioNameXFiles = {
        { "l", "D:/Repositories/SoundManager/sounds/15727__blackstrobe__l.wav"},
        { "f", "D:/Repositories/SoundManager/sounds/15721__blackstrobe__f.wav"},
        { "g", "D:/Repositories/SoundManager/sounds/15722__blackstrobe__g.wav"},
        { "4", "D:/Repositories/SoundManager/sounds/15733__blackstrobe__morse_4.wav"},
        { "n", "D:/Repositories/SoundManager/sounds/15739__blackstrobe__n.wav"},
        { "t", "D:/Repositories/SoundManager/sounds/15745__blackstrobe__t.wav"},
        { "k", "D:/Repositories/SoundManager/sounds/15726__blackstrobe__k.wav"}
    };

    for( auto&[i, j] : audioNameXFiles )
    {
        std::cout << "i :" << i << " j: " << j;
    }

    _soundLoader.loadWavFiles( audioNameXFiles );

    _soundLoader.addSource( "EFFECTS" );
    _soundLoader.addSource( "AMBIENT" );
    _soundLoader.addSource( "MICROFONE" );
    _soundLoader.addSource( "RADIO" );

    _soundLoader.setSourceSounds( "EFFECTS", { "l", "4", "l" } );

    _soundLoader.setSourceSounds( "RADIO", { "n", "l", "t" } );
    _soundLoader.setPitch( "RADIO", 0.75f );
    _soundLoader.setLoop( "RADIO", true );
    _soundLoader.play( "RADIO" );

    int i = 0;
    int count = 60*5; //5 seconds to stop

    bool secondStarted = false;
    while(true)
    {
        if( i > 60 && !secondStarted )
        {   std::cout << "i: " << i << std::endl;
            _soundLoader.play( "EFFECTS" );
            secondStarted = true;
        }
//        std::cout << "isPlaying? " << _soundLoader.isPlaying( "RADIO" ) << std::endl;
        _soundLoader.update();

        if( i == count ) break;
        i++;

        std::this_thread::sleep_for( std::chrono::milliseconds( 16 ) );
    };

    _soundLoader.closeSoundApi();

    return 0;
}
