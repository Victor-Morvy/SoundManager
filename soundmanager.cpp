#include "soundmanager.h"
#include "wavdata.h"

#include <fstream>

SoundManager::SoundManager() : _device(nullptr), _context(nullptr), _initialized(false)
{
}

SoundManager::SoundManager(ALCdevice *device, ALCcontext *context) : _device(device), _context(context), _initialized(true)
{
}

SoundManager::~SoundManager()
{
    for( auto& wavdata : _wavMap )
    {
        alDeleteBuffers(1, &wavdata.second->buffer);
    }

    _wavMap.clear();
    _dataSourceMap.clear();


}

void SoundManager::closeSoundApi()
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(_context);
    alcCloseDevice(_device);
}

void
SoundManager::initialize()
{
    if( _initialized ) return;

    openDevice();
    createContext();

    _initialized = true;
}

// Função para carregar um arquivo WAV e criar um buffer OpenAL
bool
SoundManager::loadWavFile(const std::string &soundName, const std::string& filePath ) {

    if( !_initialized )
    {
        std::cerr << "Please, initialize the device before load wav files." << std::endl;
        return false;
    }

    //create unique pointer
    _wavMap[ soundName ] = std::make_unique<WavData>();
    auto* wavData = _wavMap[soundName].get();

    //load file
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filePath << std::endl;
        return false;
    }

    // Cabeçalho do arquivo WAV
    char chunkId[4];
    file.read(chunkId, 4);
    if (std::string(chunkId, 4) != "RIFF") {
        std::cerr << "Arquivo não é um WAV válido." << std::endl;
            return false;
    }

    file.ignore(4); // Ignora o tamanho do arquivo
    char format[4];
    file.read(format, 4);
    if (std::string(format, 4) != "WAVE") {
        std::cerr << "Arquivo não é um WAV válido." << std::endl;
            return false;
    }

    // Procura pelo subchunk "fmt "
    char subchunk1Id[4];
    file.read(subchunk1Id, 4);
    while (std::string(subchunk1Id, 4) != "fmt ") {
        file.ignore(4); // Ignora o tamanho do subchunk
        file.ignore(4); // Ignora os dados do subchunk
        file.read(subchunk1Id, 4);
    }

    // Lê o subchunk "fmt "
    file.ignore(4); // Ignora o tamanho do subchunk
    short audioFormat;
    file.read(reinterpret_cast<char*>(&audioFormat), 2);
    short numChannels;
    file.read(reinterpret_cast<char*>(&numChannels), 2);
    int sampleRate;
    file.read(reinterpret_cast<char*>(&sampleRate), 4);
    file.ignore(6); // Ignora byteRate e blockAlign
    short bitsPerSample;
    file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

    // Procura pelo subchunk "data"
    char subchunk2Id[4];
    file.read(subchunk2Id, 4);
    while (std::string(subchunk2Id, 4) != "data") {
        int subchunk2Size;
        file.read(reinterpret_cast<char*>(&subchunk2Size), 4);
        file.ignore(subchunk2Size); // Ignora os dados do subchunk
        file.read(subchunk2Id, 4);
    }

    // Lê o subchunk "data"
    int dataSize;
    file.read(reinterpret_cast<char*>(&dataSize), 4);
    char* data = new char[dataSize];
    file.read(data, dataSize);

    // Define o formato do áudio
    if (numChannels == 1 && bitsPerSample == 8) {
        wavData->format = AL_FORMAT_MONO8;
    } else if (numChannels == 1 && bitsPerSample == 16) {
        wavData->format = AL_FORMAT_MONO16;
    } else if (numChannels == 2 && bitsPerSample == 8) {
        wavData->format = AL_FORMAT_STEREO8;
    } else if (numChannels == 2 && bitsPerSample == 16) {
        wavData->format = AL_FORMAT_STEREO16;
    } else {
        std::cerr << "Formato de áudio não suportado." << std::endl;
            delete[] data;
        return false;
    }

    // Cria um buffer OpenAL
    alGenBuffers(1, &wavData->buffer);
    alBufferData(wavData->buffer, wavData->format, data, dataSize, sampleRate);

    wavData->size = dataSize;
    wavData->data = data;
    wavData->sampleRate = sampleRate;

    file.close();

    //    return wavData.buffer;
    return true;
}

bool SoundManager::loadWavFiles(const std::map<std::string, std::string> &soundNameXFilePath)
{
    for (const auto& file : soundNameXFilePath) {
        if( loadWavFile( file.first, file.second) )
        {
            std::cout << "Áudio carregado: " << file.second << std::endl;
        }
        else
        {
            std::cerr << "Falha ao carregar o áudio: " << file.second<< std::endl;
        }
    }

    return true;
}

WavData *SoundManager::wavData(const std::string &soundName)
{
    try {
     return _wavMap.at( soundName ).get();
    } catch (...) {
    }
    return nullptr;
}

///
/// \brief SoundLoader::generateSource
/// \param quantity (quantity of sources)
/// \return true (succes to generate source)
///
DataSource *SoundManager::addSource(const std::string& sourceName)
{
    if( _dataSourceMap.count( sourceName ) )
    {
        std::cerr << "name " << sourceName << " has already created.";
        return nullptr;
    }

    ALuint source;
    alGenSources(1, &source);

    _dataSourceMap[sourceName] = std::make_unique<DataSource>( sourceName, source );

    return _dataSourceMap[sourceName].get();
}

DataSource *SoundManager::getSource(const std::string &sourceName)
{
    try {
        return _dataSourceMap.at( sourceName ).get();
    } catch (...) {
    }
    return nullptr;
}

void SoundManager::update()
{
    for( auto& sourceP : _dataSourceMap )
    {
        if( !sourceP.second->loop() )
            continue;

        auto source = sourceP.second->source();
        ALint processedBuffers;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processedBuffers);

        if (processedBuffers > 0) {
            // Remove os buffers processados da fila
            std::vector<ALuint> processed(processedBuffers);
            alSourceUnqueueBuffers(source, processedBuffers, processed.data());

            // Reenfileira os buffers processados para repetir a sequência
            alSourceQueueBuffers(source, processedBuffers, processed.data());
        }
    }
}

void SoundManager::setSourceSounds(const std::string &sourceName, const std::vector<std::string> &soundsName)
{
    std::vector<WavData*> wavDataVector;

    for( const auto& name : soundsName )
    {
        auto* tmpWavData = wavData( name );
        if( tmpWavData )
            wavDataVector.push_back( tmpWavData );
        else
            std::cerr << name << " sound doesn't exist." << std::endl;
    }

    _dataSourceMap.at( sourceName )->setWavQueueData( wavDataVector );
}

void SoundManager::setVolume(const std::string &sourceName, float volume)
{
    _dataSourceMap.at( sourceName )->setVolume( volume );
}

float SoundManager::volume(const std::string &sourceName)
{
    return _dataSourceMap.at( sourceName )->volume();
}

void SoundManager::setPitch(const std::string &sourceName, float pitch)
{
    _dataSourceMap.at( sourceName )->setPitch( pitch );
}

float SoundManager::pitch(const std::string &sourceName)
{
    return _dataSourceMap.at( sourceName )->pitch();
}

void SoundManager::setLoop(const std::string &sourceName, bool loop)
{
    _dataSourceMap.at( sourceName )->setLoop( loop );
}

float SoundManager::loop(const std::string &sourceName)
{
    return _dataSourceMap.at( sourceName )->loop();
}

void SoundManager::play(const std::string &sourceName)
{
    _dataSourceMap.at( sourceName )->play();
}

void SoundManager::stop(const std::string &sourceName)
{
    _dataSourceMap.at( sourceName )->stop();
}

bool SoundManager::isPlaying(const std::string &sourceName)
{
    return _dataSourceMap.at( sourceName )->isPlaying();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void
SoundManager::openDevice()
{
    _device = alcOpenDevice(nullptr);
    if (!_device) {
        std::cerr << "Erro ao abrir o dispositivo de áudio." << std::endl;
    }
}

void
SoundManager::createContext()
{
    _context = alcCreateContext(_device, nullptr);
    if (!_context) {
        std::cerr << "Error when creating audio context." << std::endl;
        alcCloseDevice(_device);
    }

    alcMakeContextCurrent(_context);
}


