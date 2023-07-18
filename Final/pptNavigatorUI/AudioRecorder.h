// MyAudioRecorder.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "CommonVariables.h


void StartRecord();
void PlayRecord();
void writeWAV(std::string binfile,std::string wavfile);


void StartRecord(std:: string textfile,std::string wavfile)
{
const int NUMPTS = sample_rate * seconds;   // 3 seconds
int sampleRate = sample_rate;  
// 'short int' is a 16-bit type; I request 16-bit samples below
                         // for 8-bit capture, you'd use 'unsigned char' or 'BYTE' 8-bit     type
 HWAVEIN      hWaveIn;
 MMRESULT result;

 WAVEFORMATEX pFormat;
 pFormat.wFormatTag=WAVE_FORMAT_PCM;     // simple, uncompressed format
 pFormat.nChannels=channel;                    //  1=mono, 2=stereo
 pFormat.nSamplesPerSec=sampleRate; // 16000
 pFormat.wBitsPerSample=bits_per_sample; //  16 for high quality, 8 for telephone-grade
 pFormat.nAvgBytesPerSec=sampleRate*channel*bits_per_sample/8;   // = nSamplesPerSec * n.Channels *    wBitsPerSample/8
 pFormat.nBlockAlign=channel*bits_per_sample/8;                  // = n.Channels * wBitsPerSample/8
 pFormat.cbSize=0;

 // Specify recording parameters

 result = waveInOpen(&hWaveIn, WAVE_MAPPER,&pFormat,
        0L, 0L, WAVE_FORMAT_DIRECT);

  WAVEHDR      WaveInHdr;
 // Set up and prepare header for input
  WaveInHdr.lpData = (LPSTR)waveIn;
  WaveInHdr.dwBufferLength = NUMPTS*2;
  WaveInHdr.dwBytesRecorded=0;
  WaveInHdr.dwUser = 0L;
  WaveInHdr.dwFlags = 0L;
  WaveInHdr.dwLoops = 0L;
  waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

 // Insert a wave input buffer
  result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));


 // Commence sampling input
  result = waveInStart(hWaveIn);


 std::cout << "recording...\n";

  Sleep(seconds * 1000);
 // Wait until finished recording

 waveInClose(hWaveIn);
 
 std::ofstream fwtxt(textfile,std::ofstream::out);
 std::ofstream fwbin("audio.bin",std::ios::binary);
 //Writing to text file
 if(fwtxt.is_open())
 {
	 for(int i=0; i <48000; i++)
		 fwtxt << waveIn[i]<<"\n";
	 fwtxt.close();
 }
 else
	 std::cout << "Problem with opening file : audio.txt\n";
 //Writing to bin file for making .wav file
 if(fwbin.is_open())
 {
	 for(int i=0; i <48000; i++)
		 fwbin.write(reinterpret_cast<char *>(&waveIn[i]),sizeof(int16_t));
	 fwbin.close();
 }
 else
	 std::cout << "Problem with opening file : audio.bin\n";
  writeWAV("audio.bin",wavfile);
  remove("audio.bin");
}

void PlayRecord()
{
	const int NUMPTS = sample_rate * seconds;   // 3 seconds
int sampleRate = sample_rate;  
// 'short int' is a 16-bit type; I request 16-bit samples below
                            // for 8-bit capture, you'd    use 'unsigned char' or 'BYTE' 8-bit types

HWAVEIN  hWaveIn;

WAVEFORMATEX pFormat;
pFormat.wFormatTag=WAVE_FORMAT_PCM;     // simple, uncompressed format
 pFormat.nChannels=channel;                    //  1=mono, 2=stereo
 pFormat.nSamplesPerSec=sampleRate; // 16000
 pFormat.wBitsPerSample=bits_per_sample; //  16 for high quality, 8 for telephone-grade
 pFormat.nAvgBytesPerSec=sampleRate*channel*bits_per_sample/8;   // = nSamplesPerSec * n.Channels *    wBitsPerSample/8
 pFormat.nBlockAlign=channel*bits_per_sample/8;                  // = n.Channels * wBitsPerSample/8
 pFormat.cbSize=0;

// Specify recording parameters

waveInOpen(&hWaveIn, WAVE_MAPPER,&pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

WAVEHDR      WaveInHdr;
// Set up and prepare header for input
WaveInHdr.lpData = (LPSTR)waveIn;
WaveInHdr.dwBufferLength = NUMPTS*2;
WaveInHdr.dwBytesRecorded=0;
WaveInHdr.dwUser = 0L;
WaveInHdr.dwFlags = 0L;
WaveInHdr.dwLoops = 0L;
waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

HWAVEOUT hWaveOut;
std::cout << "playing...\n";
waveOutOpen(&hWaveOut, WAVE_MAPPER, &pFormat, 0, 0, WAVE_FORMAT_DIRECT);
waveOutWrite(hWaveOut, &WaveInHdr, sizeof(WaveInHdr)); // Playing the data
Sleep(seconds * 1000); //Sleep for as long as there was recorded

waveInClose(hWaveIn);
waveOutClose(hWaveOut);
}
void writeWAV(std::string binfile,std::string wavfile)
{
	wav_hdr hdr;
	hdr.RIFF[0] = 'R';
	hdr.RIFF[1] = 'I';
	hdr.RIFF[2] = 'F';
	hdr.RIFF[3] = 'F';
	hdr.WAVE[0] = 'W';
	hdr.WAVE[1] = 'A';
	hdr.WAVE[2] = 'V';
	hdr.WAVE[3] = 'E';
	hdr.fmt[0] = 'f';
	hdr.fmt[1] = 'm';
	hdr.fmt[2] = 't';
	hdr.fmt[3] = ' ';
	hdr.Subchunk1Size = 16;
	hdr.AudioFormat = 1;
	hdr.NumOfChan = channel;
	hdr.SamplesPerSec = sample_rate;
	hdr.bitsPerSample = bits_per_sample;
	hdr.bytesPerSec = sample_rate*channel*bits_per_sample/2;
	hdr.blockAlign = channel*bits_per_sample/8;
	
	hdr.Subchunk2ID[0] = 'd';
	hdr.Subchunk2ID[1] = 'a';
	hdr.Subchunk2ID[2] = 't';
	hdr.Subchunk2ID[3] = 'a';
	
	
	static_assert(sizeof(wav_hdr) == 44, "");

   std::ifstream in(binfile, std::ifstream::binary);

  uint32_t fsize = in.tellg();
  in.seekg(0, std::ios::end);
  fsize = (uint32_t)in.tellg() - fsize;
  in.seekg(0, std::ios::beg);

  printf("file size: %u\n", fsize);

  hdr.ChunkSize = fsize + sizeof(wav_hdr)  - 8;
  hdr.Subchunk2Size = fsize + sizeof(wav_hdr) - 44;

    std::ofstream out(wavfile, std::ios::binary);
  out.write(reinterpret_cast<const char *>(&hdr), sizeof(hdr));

  int16_t d;
  for (int i = 0; i < fsize; ++i) {
    // TODO: read/write in blocks
    in.read(reinterpret_cast<char *>(&d), sizeof(int16_t));
    out.write(reinterpret_cast<char *>(&d), sizeof(int16_t));
  }


}

