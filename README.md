# Software-Assignment-AI25BTECH11001

# Instruction to run the code

1. Just clone the repo
2. Run the exe inside the folder codes

# How to change the inputs

In the directory in which you are running the executable, open the init.txt.
It contains data in the format:

[image (.jpg,.png,.pbm)path relative to this directory or absolute path (under 255 character and no spaces included)]

[channels 1=greyscale 3=RGB]

[output .pbm file path relative to this directory or absolute path (under 255 character and no spaces included)]

[value of K(no of most significant singular values to be used)]

For Example:

../figs/greyscale.png

1

../output/test.pbm

200

(template in also in the folder \codes)

# How to compile the code

Using gcc,
gcc main.c utilities.c methods.c -lm -O3
this will produce a.out run it with the init.txt file

# Error Handling

Syntax of the init.txt is not checked so make it correct and other error like file reading or memory allocation is checked.
