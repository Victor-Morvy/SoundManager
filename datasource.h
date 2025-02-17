#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "AL/al.h"
#include "wavdata.h"
#include <vector>

#include <string>

class DataSource{

public:
    DataSource( const std::string& name, ALuint source ):_name(name),_source(source){};

    ~DataSource(){
        alDeleteSources( 1, &_source );
    };

    void setSoundNameToPlay( const std::vector<std::string> soundsName );

    void setWavQueueData( const std::vector<WavData*>& soundVector )
    {
        clearBufferQueueData();

        _soundVector = soundVector;

        std::vector<ALuint> buffer;
        for( auto& wav : soundVector)
            buffer.push_back( wav->buffer );

        alSourceQueueBuffers(_source, buffer.size(), buffer.data());
    };

    void play(){
        alSourcePlay( this->_source );
    }

    void stop(){
        alSourceStop( this->_source );
    }

    bool isPlaying(){
        ALint processedBuffer;

        alGetSourcei(_source, AL_SOURCE_STATE, &processedBuffer);

        return static_cast<bool>( processedBuffer != AL_PLAYING );
    }

    const std::vector<WavData*>& soundVector(){ return _soundVector; }

    const std::string& name();
    ALuint source() const;

    float volume() const;
    void setVolume(float newVolume);

    float pitch() const;
    void setPitch(float newPitch);

    bool loop() const;
    void setLoop( bool value);

private:
    void clearBufferQueueData();

    std::string _name = "";

    ALuint _source = 0;

    std::vector<WavData*> _soundVector;

    float _volume = 1;
    float _pitch = 1;
    bool _loop = false;

};

inline const std::string& DataSource::name()
{
    return _name;
}

inline ALuint DataSource::source() const
{
    return _source;
}

inline float DataSource::volume() const
{
    return _volume;
}

inline void DataSource::setVolume(float newVolume)
{
    _volume = newVolume;
     alSourcef(_source, AL_GAIN, newVolume);
}

inline float DataSource::pitch() const
{
    return _pitch;
}

inline void DataSource::setPitch(float newPitch)
{
    _pitch = newPitch;
    alSourcef(_source, AL_PITCH, newPitch);
}

inline bool DataSource::loop() const
{
    return _loop;
}

inline void DataSource::setLoop(bool value)
{
    _loop = value;
    alSourcei(_source, AL_LOOPING, value);
}

inline void DataSource::clearBufferQueueData()
{
    for( auto& wavdata : _soundVector )
    {
        alDeleteBuffers(1, &wavdata->buffer);
    }

    _soundVector.clear();
}

#endif // DATASOURCE_H
