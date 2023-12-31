import turtle as t

length = 8

def MakeSquare(sign):
    sign = int(sign)
    if sign == 1:
        t.color('black', 'black')
    elif sign == -1:
        t.color('black', 'white')
    else:
        print("ERROR:829ifh982bd298!!!")

    t.begin_fill()
    for _ in range(4):
        t.forward(length)
        t.left(90)
    t.end_fill()
    t.forward(length)
    return 0

t.home()
t.shape('classic')
t.speed('fastest')

filename = "ISING_FRACTAL_16x16_effmc4000_intv0.300.txt"

with open(filename , 'r') as file_data:
    for line in file_data:
        if (len(line.split(',')) < 2):
            input("Next lattice")
            t.clear()
            t.home()
            print(line)
        else:
            for sign in line.split(",")[1:]:
                MakeSquare(sign)
            t.left(180)
            t.forward(length * len(line.split(",")[1:]))
            t.left(90)
            t.forward(length)
            t.left(90)

t.done()
