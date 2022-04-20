import matplotlib.pyplot as plt
lines = [[],[],[],[]]
with open("data.txt","r") as f:
    data= "_"
    while data:
        data = f.readline()
        if "#" not in data and data:
            pos, vel, acc, jerk = [float(i) for i in data[:-1].split(",")]
            lines[0].append(pos)
            lines[1].append(vel)
            lines[2].append(acc)
            lines[3].append(jerk)

plt.plot(range(len(lines[0])), lines[0], label='Position')

plt.plot(range(len(lines[0])), lines[1], label='Speed')

plt.plot(range(len(lines[0])), lines[2], label='Acceleration')

plt.plot(range(len(lines[0])), lines[3], label='Jerk')

plt.legend(loc='lower left', frameon=False)

plt.show()