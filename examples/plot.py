from turtle import color
import matplotlib.pyplot as plt
import numpy as np 

lines = [[],[],[],[],[],[]]
with open("data.txt","r") as f:
    data= "_"
    while data:
        data = f.readline()
        if "#" not in data and data:
            tar, pos, vel, acc, jerk, dts = [float(i) for i in data[:-1].split(",")]
            lines[0].append(tar)
            lines[1].append(pos)
            lines[2].append(vel)
            lines[3].append(acc)
            lines[4].append(jerk)
            lines[5].append(dts)
lines = np.array(lines)


# max_ = max(lines[0]) + 5
# min_ = min(lines[0]) + 5
# lines[1][lines[1]>max_] = max_
# lines[1][lines[1]<min_] = min_

plt.plot(range(len(lines[0])), lines[0], label='Target', color ="red")

plt.plot(range(len(lines[0])), lines[1], label='Position')

plt.plot(range(len(lines[0])), lines[2], label='Speed')

plt.plot(range(len(lines[0])), lines[3], label='Acceleration')

plt.plot(range(len(lines[0])), lines[4], label='Jerk', color="purple")
#plt.plot(range(len(lines[0])), lines[5] + lines[1], label='DTS', color="yellow")

plt.plot(range(len(lines[0])), np.zeros_like(lines[2]), label='Zero', color="black")


lines[3][lines[3] > 100] = 100
lines[3][lines[3] < -100] = -100
# plt.plot(range(len(lines[0])), lines[3], label='Jerk')

plt.legend(loc='upper left', frameon=False)

plt.savefig('foo.png')
plt.show()