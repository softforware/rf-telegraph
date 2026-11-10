# RF Telegraph

A wireless Morse code telegraph built with two Arduinos and a pair of 433 MHz ASK radio modules. Tap Morse code on the transmitter's key, and the receiver decodes the message and shows it as text on a 16×2 LCD.

An IEEE RF project, Fall 2025.

## How it works

**Keying:** the transmitter checks the key button every 100 ms and times each press. The timing follows standard Morse conventions: a dash is three dot-lengths, letters are separated by three units, and words by seven.

| Input | Meaning |
|---|---|
| Press shorter than 300 ms | dot |
| Press of 300 ms or longer | dash |
| 300 ms pause after a press | end of letter |
| Pause longer than 800 ms | space between words |
| Second button | clear the screen |

**Radio protocol:** each event is sent as soon as it happens, as a one-character packet using the RadioHead [`RH_ASK`](https://www.airspayce.com/mikem/arduino/RadioHead/classRH__ASK.html) driver:

| Packet | Event | Receiver action |
|---|---|---|
| `0` | dot | add `.` to the current letter |
| `1` | dash | add `-` to the current letter |
| `2` | end of letter | decode the letter and add it to the message |
| `3` | word space | add a space |
| `4` | clear | clear the message and the LCD |

Because symbols are sent the moment they're keyed, the message builds on the receiver's LCD as you type. Invalid sequences decode to `?`.

**Decoding:** dots and dashes are matched against a Morse lookup string (`key`). The matching position is read from a parallel string (`alphabet`) to get the letter. A map-based version was tried first, but Arduino doesn't natively support maps, and the workarounds were too slow to keep up with the radio link.

## Code

| Folder | What it is |
|---|---|
| [`transmitter/`](transmitter/transmitter.ino) | Reads the key and clear buttons and sends each event over RF |
| [`receiver/`](receiver/receiver.ino) | Receives events over RF, decodes letters, and drives the LCD |
| [`tinkercad-simulation/`](tinkercad-simulation/tinkercad-simulation.ino) | The first version: a single-Arduino Morse translator built in Tinkercad, with the key, decoding, and LCD on one board. Tinkercad doesn't support RF parts, so the radio was added on real hardware. |

## Hardware

**Transmitter**
- **Board:** Arduino Uno.
- **Radio:** 433 MHz ASK transmitter module, data pin on D12.
- **Morse key:** push button on D7, with a 10 kΩ pull-down resistor.
- **Clear button:** push button on D6, with a 10 kΩ pull-down resistor.
- **LED:** on D13, lit while the key is held.

**Receiver**
- **Board:** Arduino Uno.
- **Radio:** 433 MHz ASK receiver module, data pin on D11.
- **Display:** 16×2 LCD. RS → D12, E → D10, D4–D7 → D5, D4, D3, D2.
- **Contrast:** a potentiometer on LCD pin 3.
- **Backlight:** a potentiometer on LCD pin 15, through a 220 Ω resistor.

**Libraries**
- **Radio:** [RadioHead](https://www.airspayce.com/mikem/arduino/RadioHead/).
- **LCD:** LiquidCrystal, which is built into the Arduino IDE.

## Results

- **What worked:** the full system worked end to end, from keying through radio transmission and decoding to display.
- **Range:** with bare modules and no antennas, it was reliable across a room. At 10–20 steps away, transmissions started to drop.
- **Next steps:** add antennas, power the radios from a higher-voltage supply for more range, and improve scrolling for messages longer than 16 characters.

## Team

- **Ananya Gupta:** transmission code and transmitter circuit, LCD wiring.
- **Zach McKay:** receiver code and receiver circuit, LCD wiring.
- **Isaac Mantle:** Morse code logic and the Tinkercad prototype.

## Credits

- Rui Santos, ["RF 433MHz Transmitter/Receiver Module With Arduino"](https://randomnerdtutorials.com/rf-433mhz-transmitter-receiver-module-with-arduino/), Random Nerd Tutorials.
