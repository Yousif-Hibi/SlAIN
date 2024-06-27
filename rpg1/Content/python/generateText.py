import google.generativeai as genai
import env


def generate_text(Text):
    genai.configure(api_key=env.Key)
    model = genai.GenerativeModel(model_name='gemini-1.5-flash') 
    prompt = 'give me nothing but the 9 short Dialogue sentences comblitly changed, no numbers, no pretext, that will replace,'
    fullPrompt = prompt + Text
    response = model.generate_content(fullPrompt)
    return response.text


if __name__ == "__main__":
    print(generate_text())
