# velvetvision
Amiga 500 compatible demo coded in C showing short full screen video clips with music

# development notes:
- this demo was not intended to represent perfectly structured clean Amiga C code. It is, more or less, spaghetti code. Quite ugly, not efficient, but it does what it should.
- I am in no way an experienced Amiga developer. My background was Java and JEE for twenty years. So concepts like efficient memory allocation and manual garbage collection were not quite familiar to me. Furthermore I had no background in Amiga Assembler programming.
- the project was coded using Visual Studio Code and compiled with VBCC. There are great tutorials on how to get started with VBCC on YouTube.
- the demo uses ptplayer for playing a 100K mod file which permanently plays in the background. The music was composed by myself independently from the demo and was converted into Amiga mod format which unfortunately led to a bit of quality loss.
- all images are AI generated art, free from classical copyrights.
- I used a tool which is called DPaint JS to convert the bitmap into .planes files. You can find it here: https://www.stef.be/dpaint/  it was really helpful!
- all bitmaps went through several stages of preprocessing: 1. downgrade to 240x240 px, 2. conversion to 4 color greyscale (GIMP), 3. conversion to .planes file
- all this preprocessing was done manually by hand and was quite an effort because I wanted to get the best possible optical results with only 4 colors
- memory management on Amiga Kickstart 1.3 is really bad. There is nothing you can rely on. A lot of strange things can happen and it's not intuitive from my point of view
- but there are also things that I found fascinating. For example Amiga supported long file names (30 chars or so) at a time where MS-DOS supported 8 chars
- painting a bitmap, even a big one in full screen, really goes super fast. I was blown away when I first saw this hardware from 1987 could paint a full screen bitmap video with 20 frames per second or more while the CPU is literally sleeping


# improvement proposals:
- memory management could be improved if bitmaps would be buffered in fast RAM while animation is already running in chip mem. Unfortunately I could not figure out how to do that in a stable way. So this code loads everything into chip mem, runs the animation, clears chip mem, loads the next animation. Not highly efficient but it works. If you want to improve this, feel free to change my code and send a commit proposal. If it runs, I will keep it :)
- if it is possible to shift bitmaps fast enough from fast RAM to chip RAM while the animation is runnig, the duration of the videos could be increased. But I'm not sure if the Amiga memory is really capable of doing this.
- double buffering. Currently I only use single buffering to save memory because I want to get the maximum out of the tiny chip mem. Double buffering would of course reduce flashing artifactss


This code is licensed under GNU GPL and can be used for your own projects as long as you mention the source

Thx!
  
