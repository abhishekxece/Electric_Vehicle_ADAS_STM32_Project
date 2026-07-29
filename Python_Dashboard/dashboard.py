import random
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

distances = []

fig, ax = plt.subplots()

def update(frame):

    value = random.randint(5, 70)

    distances.append(value)

    if len(distances) > 20:
        distances.pop(0)

    ax.clear()

    ax.plot(distances, marker='o')

    ax.set_title("EV ADAS Live Dashboard")

    ax.set_xlabel("Samples")

    ax.set_ylabel("Distance (cm)")

    ax.set_ylim(0, 80)

ani = FuncAnimation(fig, update, interval=500)

plt.show()