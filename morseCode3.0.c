#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buttonDotcom = 6;
const int buttonDash = 7;
const int buttonBlank = 8;
const int buzzer = 9;

//armazena os sinais Morse
String morseCode = "";
const int dotcomDuration = 200;
const int dashDuration = 500;
const int blankDuration = 1000;

//limite de caracteres por linha
const int maxCharsPerLine = 16;
int totalCharsDisplayed = 0;

void setup()
{
	pinMode(buttonDotcom, INPUT);
	pinMode(buttonDash, INPUT);
	pinMode(buttonBlank, INPUT);
	pinMode(buzzer, OUTPUT);

	lcd.begin(16, 2);
	Serial.begin(9600);
}

void loop()
{
	if (digitalRead(buttonDotcom) == HIGH)
	{
		morseCode += ".";
		tone(buzzer, 1000, dotcomDuration);
		delay(dotcomDuration);
	}

	if (digitalRead(buttonDash) == HIGH)
	{
		morseCode += "-";
		tone(buzzer, 1000, dashDuration);
		delay(dashDuration);
	}

	if (digitalRead(buttonBlank) == HIGH)
	{
		if (morseCode.length() == 0)
		{
		//se estiver vazio, imprime um espaço
			addCharToLCD(' ');
			Serial.println(" ");
		} else
		{
			char character = morseToChar(morseCode);
			addCharToLCD(character);
		}
			morseCode = "";
		delay(blankDuration);
	}
	delay(5);
}

void addCharToLCD(char character)
{
	static int charsOnLine = 0;	//contador de caracteres na linha

	//verifica se precisa pular para a próxima linha
	if (charsOnLine == maxCharsPerLine)
	{
		lcd.setCursor(0, 1);
		charsOnLine = 0;
	}

	//verifica se atingiu o total, limpa o display
	if (totalCharsDisplayed == 32)
	{
		lcd.clear();
		totalCharsDisplayed = 0;
	}

	lcd.print(character);
	Serial.print(character);
	charsOnLine++;
	totalCharsDisplayed++;
}

//converte morse para alfabeto
char morseToChar(String code)
{
	//letras A-Z e números 0-9
	String morseLetters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-.", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

	for (int i = 0; i < 36; i++)
	{
		if (code == morseLetters[i])
		{
			if (i < 26)
			{
				return char('A' + i);
			} else
			{
				return char('0' + (i - 26));
			}
		}
	}
	return '?';
}