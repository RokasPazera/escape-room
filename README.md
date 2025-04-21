# Escape Room: The Code of Freedom

Welcome to **Escape Room: The Code of Freedom** – an interactive Arduino-based puzzle game where your mission is to solve challenges and break free before time runs out!

Created by **Rokas Pažėra**

---
![Uploading image.png…]()

###  Challenges

1. **LED Ping Pong Game**  
   Use a potentiometer to control a paddle. Bounce the ball back **5 times** to win.

2. **Morse Code Number Quiz**  
   Decode Morse code using LEDs:  
   - Green LED = `.`  
   - Red LED = `-`  
   Identify the number and press the correct button.

3. **Simon Says Memory Game**  
   Watch LED sequences and listen to tones. Repeat them by pressing the correct buttons.

4. **Final Equation Challenge**  
   Solve this puzzle:  
   `(a² + b)^c / x = 12`  
   Enter the correct number sequence to escape.

---

## 🛠️ Hardware Setup

| Component        | Pin Connection                    |
|------------------|-----------------------------------|
| Green LED        | D6                                |
| Red LED          | D10                               |
| Buzzer           | D5                                |
| TM1638 Display   | STROBE D11, CLK D12, DIO D13      |
| LCD I2C Display  | I2C Address 0x27                  |
| LED Matrix       | DataIn D9, CLK D8, CS D7          |

---

## Game Logic Flow

### Challenge 1 – Ping Pong  
- Ball moves, player moves paddle  
- Missed ball = fail  
- Hit 5 times = pass

### Challenge 2 – Morse Code  
- Display Morse clue  
- Wait for correct input  
- Retry on wrong guess

### Challenge 3 – Simon Says  
- Show LED sequence  
- Player repeats  
- Mistake = restart game

### Challenge 4 – Final Equation  
- Solve displayed equation  
- Correct = win music 🎵  
- Wrong = retry

Incorrect actions or timeouts will trigger a buzzer and flashing LEDs.
