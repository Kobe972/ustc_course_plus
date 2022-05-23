# Lab2 Lending Your Name

From this lab, Prof. An ask me to write guide in English.

## Task

In lab 2, the task is very simple. Store the n-th number of a sequence into register R7.

> In mathematics, the Fibonacci numbers, commonly denoted Fn, form a sequence, called the Fibonacci sequence, such that each number is the sum of the two preceding ones, starting from 0 and 1. That is F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2).
>
> -- From Wikipedia

As the Fibonacci sequence grows quite rapidly, we made a little difference with the origin Fibonacci sequence:

F(0) = 1

F(1) = 1

F(2) = 2

F(n) = (F(n-1) + 2 * F(n-3)) mod 1024 (1 <= n <= 16384)

**In the beginning, n is stored in R0. All other registers' initial values are 0.**

Your tasks are:

- store F(n) in register R7
- Divide your student number into four equal segments, labelling them with a, b, c and d. For example, TA's student ID is PB17000144, so a = 17, b = 0, c = 1, d=44. Store value of F(a), F(b), F(c) and F(d) at the end of your code with '.FILL' pseudo command.

## Score

### 1）Corectness (60%）

If L-version passes all the test cases, you can get 30%

### 2）Report（20%）

- Same as lab1

### 3）L-Score(20%)

- The number of lines of instructions in the program (note that the line number represents the number of lines, not the number of instructions executed in the program) is within twice the number of TA's program for full credit.

- If the number of lines is larger than Lines of TA's program, this part of score is given by the following formula:
  $Score_{vesion\,l} = 10 \times min(2 \times\frac{\texttt{Lines of TA's program}}{\texttt{Lines of your program}}, 1)$
  
- **Lines of TA's program = 19**

## Tips

From this experiment, each experiment requires using assembly code. Here are some notifications.

- Capitalize keywords (For example, use "AND, ADD..." instead of "and, add..." )
- Comma division of operands
- Decimal constants start with #, hexadecimal with lowercase x
- Write comments when necessary
- Do not to modify the value(x3000) in ORIG section unless there are special needs

Here is an example of adding two numbers together,

``` assem
.ORIG x3000
LD R1, NUMBER1
LD R2, NUMBER2
ADD R0, R1, R2 ; Add 2 numbers together
ST R0, NUMBERA
HALT
NUMBERA .FILL #0
NUMBER1 .FILL #13
NUMBER2 .FILL x1C
.END
```

ORIG and END pseudo commands will not be counted. The lines of the program above is 8.

From this lab, all your submitted codes should include the full orig and end pseudo command.

## Submission

The completed program should be structured in the directory as shown in the figure,

![image1](./images/image1.png)

### Prof. An

Use git to submit your program.

### Prof. Miao & Zhang

Please pack the lab2 folder and zip it to <font color=red>Name\_ID\_lab2</font>.zip/tar/rar/...

After that <font color=red>upload to the nut cloud</font> and the link will be placed on the course homepage.

**2021.12.12 21:00 (UTC+8 China Standard Time)**

(Since it is lab2, the deadline onely consists of number 1 and number 2).
