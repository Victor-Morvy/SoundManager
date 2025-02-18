#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <map>

#include "datasource.h"
#include "wavdata.h"
#include <iostream>
#include <vector>

class SoundManager
{
public:
    SoundManager();
    SoundManager( ALCdevice* device, ALCcontext* context );
    ~SoundManager();

    void closeSoundApi();

    //with sources we can control a group of sounds to set if sound,
    //will be played in loop,
    void initialize( );
    bool loadWavFile(const std::string& soundName, const std::string& filename);
    bool loadWavFiles(const std::map<std::string, std::string> &audioFiles );
    WavData* wavData( const std::string& soundName );

    DataSource* addSource( const std::string& sourceName );
    DataSource* getSource( const std::string& sourceName );

    void update(); //update to reorganize the source buffer with multiple sounds

    //sequence play sound in source buffer
    void setSourceSounds( const std::string& sourceName, const std::vector< std::string >& soundsName );;

    void setVolume( const std::string& sourceName, float volume );
    float volume( const std::string& sourceName );

    void setPitch( const std::string& sourceName, float pitch );
    float pitch( const std::string& sourceName );

    void setLoop( const std::string& sourceName, bool loop );
    float loop( const std::string& sourceName );

    void play( const std::string& sourceName );
    void stop( const std::string& sourceName );
    bool isPlaying( const std::string& sourceName );

private:
    void openDevice();
    void createContext();

    std::map< std::string, std::unique_ptr<WavData> > _wavMap;
    std::map< std::string, std::string > _filesMap;
    std::map< std::string, std::unique_ptr<DataSource>> _dataSourceMap;

    ALCdevice*  _device;
    ALCcontext* _context;

    int _lastId; //Last id that was loaded

    bool _initialized;

};

#endif // SOUNDMANAGER_H
