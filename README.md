# Escape Room: The Code of Freedom

Welcome to **Escape Room: The Code of Freedom** – an interactive Arduino-based puzzle game where your mission is to solve challenges and break free before time runs out!

Created by **Rokas Pažėra**

---
![preview](https://github.com/user-attachments/assets/ff05bed1-ec45-4503-ae37-ab664d8d2ab3)

###  Challenges

1. **LED Ping Pong Game**  
   Use a potentiometer to control a paddle. Bounce the ball back **5 times** to win.
   ![pingpong](https://github.com/user-attachments/assets/3a180075-f0e2-4771-9beb-3243aeb04cb1)

2. **Morse Code Number Quiz**  
   Decode Morse code using LEDs:  
   - Green LED = `.`  
   - Red LED = `-`  
   Identify the number and press the correct button.
   ![image](https://github.com/user-attachments/assets/ab7fae52-a2c6-4e1f-949d-77cc53c1020a)
   ![image](https://github.com/user-attachments/assets/4567aa18-0b71-4ab0-b5d4-b6899d99c391)



3. **Simon Says Memory Game**  
   Watch LED sequences and listen to tones. Repeat them by pressing the correct buttons.
   ![image](https://github.com/user-attachments/assets/48b63903-6b37-4ae4-8439-b1caeb5473bf)


5. **Final Equation Challenge**  
   Solve this puzzle:  
   `(a² + b)^c / x = 12`  
   Enter the correct number sequence to escape.
   ![image](https://github.com/user-attachments/assets/2a6d1f4e-1dd0-4c0d-8af0-a1c96517a4e2)


---

## Hardware Setup

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
