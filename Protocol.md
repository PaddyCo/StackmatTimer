Following is from a post on Reddit from /u/freundTech:

```
An aux cable usually has 3 rings: Ground, Left and Right. The stackmat timer only needs 2 of them. One for ground and one for data (It sends data on both right and left).

Now we have a physical connection. Next we have to send data over this connection. When using an aux cable for sound analog values are send to represent the sound wave. The timer doesn't do that. Instead it sends digital values only consisting of the highest and lowest value normally send over the cable. http://i.imgur.com/mRPrlxn.png

The timer sends it's signal with a fixed rate of 1200 bits per second, so you just record audio from the headphone jack and every 1/1200th of a second you check if the signal is high or low (At least that's the easy way. In reality this will slowly get out of sync with the timer, which is why I'm actually using run legth encoding).

Now you have a series of bits, that you have to turn into a packet. You can see that before each packet the signal is on 0 for a long time. While we listen for bits we check if we can find 9 0 bits directly following each other. The way the protocol works there can never be 9 0 bits following each other inside a packet, so we know that we are in between 2 packets. As soon as the signal changes to 1 we know that we are at the start of a packet.

After finding the packet we have to read and interpret it. The timer sends bytes using the Rs232 protocol. Each byte starts with a start bit (always 1), followed by 8 data bits and an end bit (always 0). With this info we can turn every 10 bits into a byte.

Each packet consists of 9 bytes. And this where the protocol gets weird. In case you are developing a similar protocol for something I would definitely recommend against doing it like that ;)

The first byte is the state byte. It's an ASCII code for one of the following letters

    "L" (Left hand on timer)

    "R" (Right hand on timer)

    "C" (Both hands on timer)

    "A" (Ready to start)

    "I" (Reset)

    "S" (Stopped)

    " " (Running and no hands on sensor)

Byte 2-6 are the digits of the timer starting with minutes, then 10 seconds, seconds, tenth seconds and hundredth seconds (I'm not sure how thousandth seconds are transmitted).
However the byte doesn't just represent the digit. It represents the ASCII code for the digit, which is the digit + 48.

Byte 7 is a checksum, that can be used to check if the packet was properly transmitted. It it the sum of all digits (the values this time. Not the ASCII codes) + 64. If it is something else the packet wasn't properly transmitted.

Bytes 8 and 9 are for some reason the ASCII codes for \n (newline) and \r (carriage return).
```