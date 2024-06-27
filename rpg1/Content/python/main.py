import asyncio
from generateText import generate_text
from generateSound import text_to_speech
import sys


def main():
    text_from_cpp = " "
    # Generate AI text
    if len(sys.argv) > 1:
        text_from_cpp = sys.argv[1]
    else:
        print("No text received from C++")   
    text = generate_text(text_from_cpp)
    print(text)

    # # Split the text into an array of strings at each period (.)
    # text_array = [sentence.strip() for sentence in text.split('\n') if sentence.strip()]
   
    # # Define parameters for TTS
    # VOICE = 'en-US-ChristopherNeural'
    # PITCH_ADJUSTMENT = -1.0  # Positive values increase pitch, negative values decrease pitch

    # # Run the text to speech and pitch adjustment for each sentence in the array
    # for i, sentence in enumerate(text_array):
    #     output_file = f"test_{i}.wav"
    #     asyncio.run(text_to_speech(sentence, VOICE, output_file, PITCH_ADJUSTMENT))


if __name__ == "__main__":
    main()
