from random import randrange
import subprocess
from logger import logger
import matplotlib.pyplot as plt
from abc import ABC

AVG_RUNS = 30
MAX_SYSTEM_THREADS = 16
MAX_TEST_THREAD = 30


class ParallelExecutor:
    def __init__(self, program_name: str) -> None:
        self.program = program_name

    def __average_for_n_threads(self, threads_num: int) -> float:
        timings = []
        for _ in range(AVG_RUNS):
            seed = randrange(0, 2**16)
            p = subprocess.run([self.program, str(threads_num), str(seed)])
            timings.append(p.returncode)
            logger.info(f"subprocess with {threads_num} threads finished with timing {p.returncode}")

        return sum(timings) / len(timings)

    def get_graph_experemental_info(self):
        graph_data = []
        for thread_num in range(1, MAX_TEST_THREAD):
            point = [thread_num, self.__average_for_n_threads(thread_num)]
            graph_data.append(point)

        return graph_data


class ABCDataCore(ABC):
    def __init__(self, plot_data):
        self.experimental_data = plot_data

    def theoretical_average_time(self, x: int):
        pass

    def theoretical_efficiency(self, x: int):
        pass

    def theoretical_acceleration(self, x: int):
        pass

    def load_theoretical_average_time(self):
        x = [i[0] for i in self.experimental_data]
        y = [self.theoretical_average_time(i) for i in x]
        return x, y

    def load_theoretical_efficiency(self):
        x = [i[0] for i in self.experimental_data]
        y = [self.theoretical_efficiency(i) for i in x]
        return x, y

    def load_theoretical_acceleration(self):
        x = [i[0] for i in self.experimental_data]
        y = [self.theoretical_acceleration(i) for i in x]
        return x, y

    def load_practical_average_time(self):
        pass

    def load_practical_efficiency(self):
        pass

    def load_practical_acceleration(self):
        pass


class Lab1DataCore(ABCDataCore):
    def theoretical_average_time(self, x):
        base_point = self.experimental_data[0][1]
        if x < MAX_SYSTEM_THREADS:
            return base_point/x
        return base_point / MAX_SYSTEM_THREADS

    def theoretical_efficiency(self, x):
        if x <= MAX_SYSTEM_THREADS:
            return 1 
        return 1/(x - MAX_SYSTEM_THREADS)

    def theoretical_acceleration(self, x):
        if x <= MAX_SYSTEM_THREADS:
            return x 
        return MAX_SYSTEM_THREADS

    def load_practical_average_time(self):
        x = [i[0] for i in self.experimental_data]
        y = [i[1] for i in self.experimental_data]
        return x, y

    def load_practical_efficiency(self):
        base_point = self.experimental_data[0][1]
        x = [i[0] for i in self.experimental_data]
        y = [base_point/(i[1]*i[0])for i in self.experimental_data]
        return x, y

    def load_practical_acceleration(self):
        base_point = self.experimental_data[0][1]
        x = [i[0] for i in self.experimental_data]
        y = [base_point/i[1] for i in self.experimental_data]
        return x, y


class Visualizer:
    def __init__(self, binary_name: str):
        self.executor = ParallelExecutor(binary_name)
        self.experimental_data = self.executor.get_graph_experemental_info()
        self.data_core = Lab1DataCore(self.experimental_data)

    def visualize_average_time(self, name="AVG_time.png"):
        xt, yt = self.data_core.load_theoretical_average_time()
        xp, yp = self.data_core.load_practical_average_time()
        fig, ax = plt.subplots(layout='constrained')
        ax.plot(xt, yt, label="Theoretical AVG Time")
        ax.plot(xp, yp, label="Practical AVG Time")
        ax.set_xlabel("Threads")
        ax.set_ylabel("Time")
        ax.legend()
        fig.savefig(name)

    def visualize_acceleration(self, name="acceleration.png"):
        xt, yt = self.data_core.load_theoretical_acceleration()
        xp, yp = self.data_core.load_practical_acceleration()
        fig, ax = plt.subplots(layout='constrained')
        ax.plot(xt, yt, label="Theoretical Acceleration")
        ax.plot(xp, yp, label="Practical Acceleration")
        ax.set_xlabel("Threads")
        ax.set_ylabel("Acceleration")
        ax.legend()
        fig.savefig(name)

    def visualize_efficiency(self, name="efficiency.png"):
        xt, yt = self.data_core.load_theoretical_efficiency()
        xp, yp = self.data_core.load_practical_efficiency()
        fig, ax = plt.subplots(layout='constrained')
        ax.plot(xt, yt, label="Theoretical Efficiency")
        ax.plot(xp, yp, label="Practical Efficiency")
        ax.set_xlabel("Threads")
        ax.set_ylabel("Efficiency")
        ax.legend()
        fig.savefig(name)


def main():
    v = Visualizer('./a.out')
    v.visualize_average_time()
    v.visualize_efficiency()
    v.visualize_acceleration()

if __name__ == "__main__":
    main()
