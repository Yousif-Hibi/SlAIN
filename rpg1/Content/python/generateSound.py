import asyncio
import edge_tts
import soundfile as sf
from scipy.signal import resample


def change_pitch(data, samplerate, semitones):
    factor = 2 ** (semitones / 12.0)
    new_length = int(len(data) / factor)
    pitched_data = resample(data, new_length)
    return pitched_data


async def text_to_speech(text, voice='en-US-ChristopherNeural', output_file='test.wav', pitch_adjustment=-1.0):
    communicate = edge_tts.Communicate(text, voice)
    await communicate.save(output_file)
    data, samplerate = sf.read(output_file)
    pitched_data = change_pitch(data, samplerate, pitch_adjustment)
    sf.write("pitched_" + output_file, pitched_data, samplerate)
